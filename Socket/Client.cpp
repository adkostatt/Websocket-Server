#include <Socket/Client.hpp>

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
		iResult = send(internalSocket, payload, payloadLength, NULL);
		if (iResult == SOCKET_ERROR)
		{
			errorCode = WSAGetLastError();
			return false;
		}
	}
	else
	{
		do
		{
			iResult = send(internalSocket, payload - sent, payloadLength - sent, NULL);
			if (iResult == SOCKET_ERROR)
			{
				errorCode = WSAGetLastError();
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

	iResult = recv(internalSocket, buffer, toReceive, force ? MSG_WAITALL : NULL);

	if (iResult < 0)
	{
		errorCode = WSAGetLastError();
		return 0;
	}

	return iResult;
}

const bool Client::Receivable(
	const long seconds,
	const long microseconds
) noexcept
{
	INT iResult;
	const timeval timeout{ seconds, microseconds };
	fd_set readFdSet;

	FD_ZERO(&readFdSet);
	FD_SET(internalSocket, &readFdSet);

	iResult = select(0, &readFdSet, NULL, NULL, &timeout); // Возвращает <0 при ошибке,
	// в противном случае возвращает количество сокетов из readFdSet, которые готовы быть прочитанными

	if (iResult < 0)
	{
		errorCode = iResult;
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
		return false;
	}
	return true;
}

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
			closesocket(tempSocket);
			continue;
		}
		break;
	}

	Client::FreeAddressInfo((addrinfo*)ptr);
	return new Client(tempSocket);
}
