#pragma once
#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
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

	static const INT InitializeWSA(
		const WORD version
	) noexcept;

	static void CleanupWSA(

	) noexcept;
};
