#include <Socket/Socket.hpp>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#ifdef WEBSOCKET_TLS_SUPPORT
#pragma comment (lib, "libssl.lib")
#endif

Socket::Socket(
	const SOCKET socket_
) noexcept : internalSocket(socket_)
{

}
Socket::Socket(
	const addrinfo* addressInfo
) noexcept
{
	internalSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
	if (internalSocket == INVALID_SOCKET)
		errorCode = WSAGetLastError();
}
Socket::~Socket(

) noexcept
{
	Close();
}

void Socket::Close(

) const noexcept
{
	closesocket(internalSocket);
}

// errorCode - 11032 (Потому что максимально большой номер ошибки от WSA2 это 11031, тоесть это гарантия того, что число будет отрицательным). 
const int64_t Socket::EncodeErrorCode(
	const int64_t errorCode
) noexcept
{
	return errorCode - 11032;
}

// errorCode + 11032 (Потому что максимально большой номер ошибки от WSA2 это 11031). 
const int64_t Socket::DecodeErrorCode(
	const int64_t errorCode
) noexcept
{
	return errorCode + 11032;
}

void Socket::FreeAddressInfo(
	addrinfo* addressInfo
) noexcept
{
	freeaddrinfo(addressInfo);
}

// Если возвращаемое значение меньше нуля, то используй DecodeErrorCode на нем, чтобы получить ошибку от WS2
const addrinfo* Socket::GetAddressInfo(
	const addrinfo* hints,
	const char* port,
	const char* host
) noexcept
{
	addrinfo* result = nullptr;
	INT iResult;

	iResult = getaddrinfo(host, port, hints, &result);
	if (iResult != 0)
	{
		if (result)
			FreeAddressInfo(result);
		return (const addrinfo*)EncodeErrorCode(iResult);
	}
	return result;
}

const INT Socket::InitializeWSA(
	const WORD version
) noexcept
{
	WSADATA wsaData;
	return WSAStartup(version, &wsaData);
}

void Socket::CleanupWSA(

) noexcept
{
	WSACleanup();
}
