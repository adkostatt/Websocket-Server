#include <Socket/Client.hpp>

#ifdef WEBSOCKET_TLS_SUPPORT
#define SEND(payload, payloadLength) SSL_write(ssl, payload, payloadLength)
#define READ() SSL_read(ssl, buffer, toReceive);
#else
#define SEND(payload, payloadLength) send(internalSocket, payload, payloadLength, 0)
#define READ() recv(internalSocket, buffer, toReceive, force ? MSG_WAITALL : 0);
#endif

#ifdef WEBSOCKET_TLS_SUPPORT

Client::Client(
	const SOCKET socket_,
	SSL* ssl_
) noexcept : Socket(socket_), ssl(ssl_)
{
	SSL_set_fd(ssl, internalSocket);
}

Client::Client(
	const addrinfo* addressInfo,
	SSL* ssl_
) noexcept : Socket(addressInfo), ssl(ssl_)
{
	SSL_set_fd(ssl, internalSocket);
}

Client::~Client(

) noexcept
{
	SSL_shutdown(ssl);
	SSL_free(ssl);
}

const bool Client::TlsHandshake(

) const noexcept
{
	return SSL_accept(ssl) > 0;
}

#else
Client::Client(
	const SOCKET socket_
) noexcept : Socket(socket_)
{

}

Client::Client(
	const addrinfo* addressInfo
) noexcept : Socket(addressInfo)
{

}
#endif

const bool Client::Send(
	const char* payload,
	const int payloadLength,
	const bool force
) noexcept
{
	INT iResult;
	int sent = 0;

	if (!force)
	{
		iResult = SEND(payload, payloadLength);
		if (iResult == SOCKET_ERROR)
		{
			errorCode = WSAGetLastError();
			WS_DEBUG("Client::Send - " << errorCode)
			return false;
		}
	}
	else
	{
		do
		{
			iResult = SEND(payload + sent, payloadLength - sent);
			if (iResult == SOCKET_ERROR)
			{
				errorCode = WSAGetLastError();
				WS_DEBUG("Client::Send - " << errorCode)
				return false;
			}
			sent += iResult;

		} while (sent < payloadLength);
	}

	return iResult == payloadLength;
}

const int Client::Receive(
	char* buffer,
	const int toReceive,
	const bool force
) noexcept
{
	INT iResult;

	iResult = READ();

	if (iResult < 0)
	{
		errorCode = WSAGetLastError();
		WS_DEBUG("Client::Receive - " << errorCode)
		return 0;
	}

	return iResult;
}

const bool Client::Receivable(
	const long seconds,
	const long microseconds
) noexcept
{
#ifdef WEBSOCKET_TLS_SUPPORT
	if (SSL_has_pending(ssl))
		return true;
#endif
	INT iResult;
	timeval timeout{ seconds, microseconds };
	fd_set readFdSet;

	FD_ZERO(&readFdSet);
	FD_SET(internalSocket, &readFdSet);

	iResult = select(internalSocket+1, &readFdSet, NULL, NULL, &timeout); // Возвращает >0 при ошибке,
	// в противном случае возвращает количество сокетов из readFdSet, которые готовы быть прочитанными

	if (iResult < 0)
	{
		errorCode = iResult;
		WS_DEBUG("Client::Receivable - " << errorCode)
		return false;
	}

	return iResult;
}

const bool Client::Shutdown(
	const int channel
) noexcept
{
	INT iResult;

	iResult = shutdown(internalSocket, channel);
	if (iResult == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		WS_DEBUG("Client::Shutdown - " << errorCode)
		return false;
	}
	return true;
}

#ifndef WEBSOCKET_TLS_SUPPORT
Client* Client::Connect(
	const char* host,
	const char* port
) noexcept
{
	INT iResult;
	const addrinfo* ptr;
	addrinfo hints;
	SOCKET tempSocket;
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	ptr = Client::GetAddressInfo(&hints, port, host);
	if ((int64_t)ptr < 0)
		return (Client*)ptr;

	for (; ptr != NULL; ptr = ptr->ai_next)
	{
		tempSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (tempSocket == INVALID_SOCKET)
			return (Client*)Client::EncodeErrorCode(WSAGetLastError());

		iResult = connect(tempSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			WS_DEBUG("Client::Connect - " << iResult)
			closesocket(tempSocket);
			continue;
		}
		break;
	}

	Client::FreeAddressInfo((addrinfo*)ptr);
	return new Client(tempSocket);
}
#endif
