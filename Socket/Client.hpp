#include <Socket/Socket.hpp>

class Client : private Socket
{
public:
	Client(
		const SOCKET socket_
	) noexcept;

	Client(
		const addrinfo* addressInfo
	) noexcept;

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

	static Client* Connect(
		const char* host,
		const char* port
	) noexcept;
};
