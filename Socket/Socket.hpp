#pragma once
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#include <iostream>
#define WS_DEBUG(msg) std::cout << msg << std::endl;
#else
#define WS_DEBUG(msg)
#endif

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef int SOCKET;
typedef int INT;

#define closesocket(sock) close(sock)
#define WSAGetLastError() 0
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define ZeroMemory(addr, size) memset(addr, 0, size)
#endif
#include <cstdint>

class Socket
{
protected:
	SOCKET internalSocket = INVALID_SOCKET;
	INT errorCode = 0; // TODO: Придумать как работать с errorCode

public:
	Socket(
		const SOCKET socket_
	) noexcept;
	// addressInfo НЕ будет очищен при вызове деструктора ~Socket
	Socket(
		const addrinfo* addressInfo
	) noexcept;
	~Socket(

	) noexcept;

	void Close(

	) const noexcept;

	static const int64_t EncodeErrorCode(
		const int64_t errorCode
	) noexcept;

	static const int64_t DecodeErrorCode(
		const int64_t errorCode
	) noexcept;

	static void FreeAddressInfo(
		addrinfo* addressInfo
	) noexcept;

	static const addrinfo* GetAddressInfo(
		const addrinfo* hints,
		const char* port = NULL,
		const char* host = NULL
	) noexcept;

#ifdef _WIN32
	static const INT InitializeWSA(
		const WORD version
	) noexcept;

	static void CleanupWSA(

	) noexcept;
#endif
};
