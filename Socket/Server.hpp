#include <Socket/Socket.hpp>
#ifdef WEBSOCKET_TLS_SUPPORT
#include <openssl/ssl.h>
#endif

class Server : private Socket
{
#ifdef WEBSOCKET_TLS_SUPPORT
private:
	SSL_CTX* ctx;
public:
	Server(
		SOCKET socket_,
		const char* cert,
		const char* key
	) noexcept;

	Server(
		const addrinfo* addressInfo,
		const char* cert,
		const char* key
	) noexcept;

	~Server(

	) noexcept;

#else
public:
	Server(
		SOCKET socket_
	) noexcept;

	Server(
		const addrinfo* addressInfo
	) noexcept;
#endif

	// Устанавливает максимальную очередь подключений
	const bool Listen(
		const INT connections
	) noexcept;

	// Нужно обязательно удалять Client, как он станет не нужным(через оператор delete)
	class Client* Accept(

	) noexcept;

	const bool Bind(
		const addrinfo* addressInfo
	) noexcept;

#ifdef WEBSOCKET_TLS_SUPPORT
	static Server* Bind(
		const char* port,
		const char* cert,
		const char* key
	) noexcept;
#else
	static Server* Bind(
		const char* port
	) noexcept;
#endif
};
