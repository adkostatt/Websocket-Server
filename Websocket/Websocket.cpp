#include <Websocket/Websocket.hpp>
#include <Websocket/Header.hpp>
#include <Websocket/ExtendedHeader.hpp>
#include <Socket/Client.hpp>
#include <Http/Http.hpp>

#include <Cryptography/SHA1.h>
#include <Cryptography/Base64.h>

#include <algorithm>

Websocket::Websocket(
	Client* client_
) noexcept : client(client_), buffer()
{

}

Websocket::Websocket(

) noexcept : client(nullptr), buffer()
{

}

Websocket::~Websocket(

) noexcept
{
	if (client)
		delete client;
}

void Websocket::ChangeClient(
	Client* client_
) noexcept
{
	client = client_;
}

const bool Websocket::Handshake(

) noexcept
{
	/*if (!client->Receivable(1))
	{
		WS_DEBUG("Websocket::Handshake - timeout")
		return false;
	}*/

	int received = client->Receive(buffer, sizeof(buffer) - 3);
	Http parsedHttp(buffer, received);

	if (!parsedHttp.method)
	{
		WS_DEBUG("Websocket::Handshake - invalid http")
		return false;
	}

	if (parsedHttp.headers.find("Sec-WebSocket-Key") == parsedHttp.headers.end())
	{
		WS_DEBUG("Websocket::Handshake - corrupted headers")
		return false;
	}

	const char* websocketKey = parsedHttp.headers["Sec-WebSocket-Key"];

	if (strlen(websocketKey) != 24)	// Согласно документации mdn это текст из 16 байт, который закодирован в base64(что всегда будет текстом длинной 24 байт)
	{
		WS_DEBUG("Websocket::Handshake - invalid Sec-WebSocket-Key " << websocketKey)
		return false;
	}

	char tmpAccept[60];
	std::copy_n(websocketKey, 24, tmpAccept);
	std::copy_n("258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 36, tmpAccept + 24);

	char websocketAccept[29];
	SHA1_(websocketAccept, tmpAccept, 60); // Помоему оно капельку сломано
	Base64Encode((unsigned char*)tmpAccept, (const unsigned char*)websocketAccept, 20);
	std::copy_n(tmpAccept, 28, websocketAccept);
	websocketAccept[28] = '\0';

	std::copy_n("HTTP/1.1 101 Switching Protocols\r\n", 34, buffer);
	const char* names[]{"Upgrade", "Connection", "Sec-WebSocket-Accept"};
	const char* values[]{"websocket", "Upgrade", websocketAccept};
	Headers headers{ names, values, 3 };

	received = Http::CreateHttp(buffer, &headers);

	return client->Send(buffer, received);
}

void Websocket::Close(
	const StatusCode::AllowedInClose statusCode
) noexcept
{
	// TODO: Сделать отправку close frame и тд
}

const bool Websocket::SendFrame(
	char* payload,
	const uint64_t payloadLength,
	const uint8_t opcode,
	const bool masked,
	const int32_t maskedKey
) noexcept
{
	Header header(
		true, false, false, false,
		opcode, masked, payloadLength
	);
	bool sent = client->Send((char*)&header, sizeof(Header));

	if (!sent)
	{
		WS_DEBUG("Websocket::SendFrame - fail")
		return false;
	}

	const uint8_t length = header.GetLength();
	switch (length)
	{
	case 126:
		sent = client->Send((char*)(uint16_t*)&payloadLength, sizeof(uint16_t));
		break;
	case 127:
		sent = client->Send((char*)&payloadLength, sizeof(uint64_t));
		break;
	}
	if (!sent)
	{
		WS_DEBUG("Websocket::SendFrame - fail")
		return false;
	}

	if (masked)
	{
		char* maskedArray = (char*)&maskedKey;

		sent = client->Send(maskedArray, sizeof(int32_t));
		if (!sent)
		{
			WS_DEBUG("Websocket::SendFrame - fail")
			return false;
		}

		for (int i = 0; i < payloadLength; i++)
			payload[i] ^= maskedArray[i % 4];
	}

	return client->Send(payload, payloadLength);
}
// TODO: Сделать Websocket::Receivable
FrameData Websocket::ReadFrame(
	const long seconds,
	const long microseconds
) noexcept
{
	int received;

	if (!client->Receivable(seconds, microseconds))
	{
		WS_DEBUG("Websocket::ReadFrame - timeout")
		return { 0, 0, nullptr };
	}

	received = client->Receive(buffer, sizeof(Header));

	if (received != sizeof(Header))
	{
		WS_DEBUG("Websocket::ReadFrame - corrupted headers")
		return { 0, 0, nullptr };
	}

	Header* header = (Header*)buffer;
	uint64_t toReceive = header->GetLength();
	uint8_t opcode = header->GetOpcode();
	bool masked = header->GetMasked();
	int32_t maskedKey;

	if (!client->Receivable(0, 500))
	{
		WS_DEBUG("Websocket::ReadFrame - timeout")
		return { 0, 0, nullptr };
	}

	if (toReceive <= 125 and masked)
	{
		received = client->Receive((char*)&maskedKey, sizeof(int32_t));
		if (received != sizeof(int32_t))
		{
			WS_DEBUG("Websocket::ReadFrame - corrupted maskedKey")
			return { 0, 0, nullptr };
		}
	}
	else if (toReceive == 126)
	{
		if (masked)
		{
			received = client->Receive(buffer, sizeof(ExtendedHeader::EH16M));
			if (received != sizeof(ExtendedHeader::EH16M))
			{
				WS_DEBUG("Websocket::ReadFrame - corrupted extended headers")
				return { 0, 0, nullptr };
			}

			ExtendedHeader::EH16M* extendedHeader = (ExtendedHeader::EH16M*)buffer;
			toReceive = extendedHeader->payloadLength;
			maskedKey = extendedHeader->maskedKey;
		}
		else
		{
			received = client->Receive((char*)toReceive, sizeof(uint16_t));
			if (received != sizeof(ExtendedHeader::EH16))
			{
				WS_DEBUG("Websocket::ReadFrame - corrupted extended headers")
				return { 0, 0, nullptr };
			}
		}
	}
	else if (toReceive == 127)
	{
		if (masked)
		{
			received = client->Receive(buffer, sizeof(ExtendedHeader::EH64M));
			if (received != sizeof(ExtendedHeader::EH64M))
			{
				WS_DEBUG("Websocket::ReadFrame - corrupted extended headers")
				return { 0, 0, nullptr };
			}

			ExtendedHeader::EH64M* extendedHeader = (ExtendedHeader::EH64M*)buffer;
			toReceive = extendedHeader->payloadLength;
			maskedKey = extendedHeader->maskedKey;
		}
		else
		{
			received = client->Receive((char*)toReceive, sizeof(uint64_t));
			if (received != sizeof(ExtendedHeader::EH64))
			{
				WS_DEBUG("Websocket::ReadFrame - corrupted extended headers")
				return { 0, 0, nullptr };
			}
		}
	}
	else
	{
		if (!client->Receivable(0, 500))
		{
			WS_DEBUG("Websocket::ReadFrame - timeout")
			return { 0, 0, nullptr };
		}
	}

	if (toReceive > sizeof(buffer))
	{
		WS_DEBUG("Websocket::ReadFrame - buffer overflow " << toReceive)
		return { 0, 0, nullptr };
	}

	received = client->Receive(buffer, toReceive);
	if (received != toReceive)
	{
		WS_DEBUG("Websocket::ReadFrame - corrupted headers")
		return { 0, 0, nullptr };
	}

	if (masked)
		return { opcode, toReceive, buffer, maskedKey };
	else
		return { opcode, toReceive, buffer };
}
