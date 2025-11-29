#include <Websocket/ExtendedHeader.hpp>

namespace ExtendedHeader
{
	const EH16 CreateExtendedHeader(
		const uint16_t payloadLength
	) noexcept
	{
		return EH16{
			payloadLength
		};
	}
	const EH16M CreateExtendedHeader(
		const uint16_t payloadLength, const int32_t maskingKey
	) noexcept
	{
		return EH16M{
			payloadLength,
			maskingKey
		};
	}
	const EH64 CreateExtendedHeader(
		const uint64_t payloadLength
	) noexcept
	{
		return EH64{
			payloadLength
		};
	}
	const EH64M CreateExtendedHeader(
		const uint64_t payloadLength, const int32_t maskingKey
	) noexcept
	{
		return EH64M{
			payloadLength,
			maskingKey
		};
	}
}
