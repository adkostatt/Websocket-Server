#pragma once
#include <cstdint>

struct FrameData
{
	char* payload;
	uint8_t opcode;
	uint64_t payloadLength;

	FrameData(
		uint8_t opcode_, uint64_t payloadLength_,
		char* payload_
	) noexcept;
	FrameData(
		uint8_t opcode_, uint64_t payloadLength_,
		char* maskedPayload, int32_t maskedKey
	) noexcept;
};
