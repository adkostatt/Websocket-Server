#pragma once
#include <Websocket/FrameData.hpp>
#include <Websocket/Constants.hpp>

class Websocket
{
private:
	class Client* client;
	char buffer[248];

public:

	Websocket(
		class Client* client_
	) noexcept;

	Websocket(

	) noexcept;

	~Websocket(

	) noexcept;

	void ChangeClient(
		Client* client_
	) noexcept;

	// TODO: Cделать чтобы handshake принимался только с определенного path
	const bool Handshake(

	) noexcept;

	void Close(
		const StatusCode::AllowedInClose statusCode
	) noexcept;

	const bool SendFrame(
		char* payload,
		const uint64_t payloadLength,
		const uint8_t opcode = Opcode::Binary,
		const bool masked = false,
		const int32_t maskedKey = 0
	) noexcept;

	FrameData ReadFrame(
		const long seconds = 10l,
		const long microseconds = 0l
	) noexcept;
};
