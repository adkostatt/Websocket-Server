#include <Socket/Server.hpp>
#include <Socket/Client.hpp>

Server::Server(
	SOCKET socket_
) noexcept : Socket(socket_)
{

}

Server::Server(
	const addrinfo* addressInfo
) noexcept : Socket(addressInfo)
{

}

const bool Server::Listen(
	const INT connections
) noexcept
{
	INT iResult;

	iResult = listen(internalSocket, connections);
	if (iResult == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		return false;
	}
	return true;
}

// TODO: Сделать чтобы еще писались данные об порте и айпи клиента
Client* Server::Accept(

) noexcept
{
	SOCKET tempSocket;

	tempSocket = accept(internalSocket, NULL, NULL);
	if (tempSocket == INVALID_SOCKET)
	{
		errorCode = WSAGetLastError();
		return nullptr;
	}
	return new Client(tempSocket);
}

const bool Server::Bind(
	const addrinfo* addressInfo
) noexcept
{
	INT iResult;

	iResult = bind(internalSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		return false;
	}
	return true;
}

Server* Server::Bind(
	const char* port
) noexcept
{
	Server* server;
	const addrinfo* result;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = Server::GetAddressInfo(&hints, port);
	if ((int64_t)result < 0)
		return (Server*)result;

	server = new Server(result);
	if (!server->Bind(result))
	{
		delete server;
		return nullptr;
	}

	Server::FreeAddressInfo((addrinfo*)result);

	return server;
}
