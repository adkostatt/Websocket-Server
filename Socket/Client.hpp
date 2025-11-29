#include <Socket/Socket.hpp>
#ifdef WEBSOCKET_TLS_SUPPORT
#include <openssl/ssl.h>
#endif

class Client : private Socket
{
#ifdef WEBSOCKET_TLS_SUPPORT
private:
	SSL* ssl;
#endif
public:
#ifdef WEBSOCKET_TLS_SUPPORT
	Client(
		const SOCKET socket_,
		SSL* ssl_
	) noexcept;

	Client(
		const addrinfo* addressInfo,
		SSL* ssl_
	) noexcept;

	~Client(

	) noexcept;

	const bool TlsHandshake(

	) const noexcept;

#else
	Client(
		const SOCKET socket_
	) noexcept;

	Client(
		const addrinfo* addressInfo
	) noexcept;

#endif

	// Если force = true, то будут попытки отправлять данные, пока не будет отправленно payloadLength байт(или пока подключение не разорвут)
	const bool Send(
		const char* payload,
		const int payloadLength,
		const bool force = false
	) noexcept;

	// Если force = true, то будут попытки принимать данные, пока не будет принято toReceive байт(или пока подключение не разорвут)
	const int Receive(
		char* buffer,
		const int toReceive,
		const bool force = false
	) noexcept;

	// Возможно стоит переименовать
	const bool Receivable(
		const long seconds = 0,
		const long microseconds = 0
	) noexcept;

	const bool Shutdown(
		const int channel
	) noexcept;

#ifndef WEBSOCKET_TLS_SUPPORT
	static Client* Connect(
		const char* host,
		const char* port
	) noexcept;
#endif
};
