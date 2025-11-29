#pragma once
#include <cstdint>

constexpr uint8_t FIN = 0x80;            // 10000000
constexpr uint8_t RSV1 = 0x40;           // 01000000
constexpr uint8_t RSV2 = 0x20;           // 00100000
constexpr uint8_t RSV3 = 0x10;           // 00010000
constexpr uint8_t OPCODE = 0xF;          // 00001111

constexpr uint8_t MASKED = 0x80;         // 10000000
constexpr uint8_t PAYLOAD_LENGTH = 0x7F; // 01111111


namespace Opcode
{
	constexpr uint8_t Contintuation = 0;
	enum NonControl : uint8_t
	{
		Text = 1,
		Binary = 2,
		Reserved,
		Reserved1,
		Reserver2,
		Reserved3,
		Reserved4
	};
	enum Control : uint8_t
	{
		Close = 8,
		Ping = 9,
		Pong = 10,
		ControlReserved,
		ControlReserved1,
		ControlReserved2,
		ControlReserved3,
		ControlReserved4
	};
}

namespace StatusCode
{
	enum AllowedInClose : uint16_t
	{
		NormalClosure = 1000,
		GoingAway = 1001,
		ProtocolError = 1002,
		InvalidPayload = 1007,
		PolicyViolated = 1008,
		MessageBig = 1009,
		Unsupported = 1010,
		InternalError = 1011
	};
	enum NotAllowedInClose : uint16_t
	{
		Reserved = 1004,
		NoCodeReceived = 1005,
		AbnormallyClose = 1006,
		TLSHandshakeFailed = 1015
	};
}
