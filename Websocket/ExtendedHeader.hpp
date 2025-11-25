#pragma once
#include <cstdint>

namespace ExtendedHeader
{
	struct EH16
	{
		uint16_t payloadLength;
	};
	struct EH16M
	{
		uint16_t payloadLength;
		int32_t maskedKey;
	};

	struct EH64
	{
		uint64_t payloadLength;
	};
	struct EH64M
	{
		uint64_t payloadLength;
		int32_t maskedKey;
	};

	const EH16 CreateExtendedHeader(
		const uint16_t payloadLength
	) noexcept;
	const EH16M CreateExtendedHeader(
		const uint16_t payloadLength, const int32_t maskingKey
	) noexcept;
	const EH64 CreateExtendedHeader(
		const uint64_t payloadLength
	) noexcept;
	const EH64M CreateExtendedHeader(
		const uint64_t payloadLength, const int32_t maskingKey
	) noexcept;
}
