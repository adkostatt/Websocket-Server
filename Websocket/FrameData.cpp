#include <Websocket/FrameData.hpp>
#include <algorithm>

FrameData::FrameData(
	uint8_t opcode_, uint64_t payloadLength_,
	char* payload_
) noexcept : opcode(opcode_), payloadLength(payloadLength_), payload(payload_)
{

}

FrameData::FrameData(
	uint8_t opcode_, uint64_t payloadLength_,
	char* maskedPayload, int32_t maskedKey
) noexcept : opcode(opcode_), payloadLength(payloadLength_), payload(maskedPayload)
{
	const char* keyArray = (const char*)&maskedKey;

	for (int i = 0; i < payloadLength; i++)
		maskedPayload[i] ^= keyArray[i % 4];
}
