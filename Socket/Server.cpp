#include <Socket/Server.hpp>
#include <Socket/Client.hpp>

#ifdef WEBSOCKET_TLS_SUPPORT

inline SSL_CTX* CreateContext(
	const char* cert,
	const char* key
) noexcept
{
	SSL_CTX* ctx;
	const SSL_METHOD* method = TLS_server_method();
	ctx = SSL_CTX_new(method);
	
	if (!ctx)
	{
		WS_DEBUG("CreateContext ctx is nullptr")
		return nullptr;
	}

	else if (SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM) <= 0)
	{
		WS_DEBUG("SSL_CTX_use_certificate_file - " << SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM))
		return nullptr;
	}

	else if (SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) <= 0)
	{
		WS_DEBUG("SSL_CTX_use_PrivateKey_file - " << SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM))
		return nullptr;
	}

	return ctx;
}

Server::Server(
	SOCKET socket_,
	const char* cert,
	const char* key
) noexcept : Socket(socket_)
{
	ctx = CreateContext(cert, key);
}

Server::Server(
	const addrinfo* addressInfo,
	const char* cert,
	const char* key
) noexcept : Socket(addressInfo)
{
	ctx = CreateContext(cert, key);
}

Server::~Server(

) noexcept
{
	SSL_CTX_free(ctx);
}

#else
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
#endif

const bool Server::Listen(
	const INT connections
) noexcept
{
	INT iResult;

	iResult = listen(internalSocket, connections);
	if (iResult == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		WS_DEBUG("Server::Listen - " << errorCode)
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
		WS_DEBUG("Server::Accept - " << errorCode)
		return nullptr;
	}
#ifdef WEBSOCKET_TLS_SUPPORT
	return new Client(tempSocket, SSL_new(ctx));
#else
	return new Client(tempSocket);
#endif
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
		WS_DEBUG("Server::Bind - " << errorCode)
		return false;
	}
	return true;
}

Server* Server::Bind(
#ifdef WEBSOCKET_TLS_SUPPORT
	const char* port,
	const char* cert,
	const char* key
#else
	const char* port
#endif
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
	{
		WS_DEBUG("Server::Bind - " << result)
		return (Server*)result;
	}

#ifdef WEBSOCKET_TLS_SUPPORT
	server = new Server(result, cert, key);
#else
	server = new Server(result);
#endif
	if (!server->Bind(result))
	{
		delete server;
		return nullptr;
	}

	Server::FreeAddressInfo((addrinfo*)result);

	return server;
}
