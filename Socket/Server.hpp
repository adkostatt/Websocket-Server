#include <Socket/Socket.hpp>

class Server : private Socket
{
public:
	Server(
		SOCKET socket_
	) noexcept;

	Server(
		const addrinfo* addressInfo
	) noexcept;

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

	static Server* Bind(
		const char* port
	) noexcept;
};
