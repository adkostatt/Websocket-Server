#pragma once
#include <cstdint>

#include <Websocket/Constants.hpp>

struct Header
{
	uint8_t bits1; // FIN + RSV1 + RSV2 + RSV3 + OPCODE(4)
	uint8_t bits2; // MASKED + PAYLOAD LENGTH(7)

	bool GetFin(
	
	) const noexcept;
	void SetFin(
		bool flag
	) noexcept;

	bool GetRsv1(
	
	) const noexcept;
	void SetRsv1(
		bool flag
	) noexcept;

	bool GetRsv2(
	
	) const noexcept;
	void SetRsv2(
		bool flag
	) noexcept;

	bool GetRsv3(
	
	) const noexcept;
	void SetRsv3(
		bool flag
	) noexcept;

	uint8_t GetOpcode(
	
	) const noexcept;
	void SetOpcode(
		uint8_t opcode
	) noexcept;

	bool GetMasked(
	
	) const noexcept;
	void SetMasked(
		bool flag
	) noexcept;

	uint8_t GetLength(
	
	) const noexcept;
	void SetLength(
		const uint8_t length
	) noexcept;

	Header(
	
	) noexcept;

	Header(
		const uint8_t bits1_,
		const uint8_t bits2_
	) noexcept;

	Header(
		const uint16_t bits
	) noexcept;

	Header(
		bool fin, bool rsv1, bool rsv2,
		bool rsv3, const uint8_t opcode,
		bool masked, const uint64_t length
	) noexcept;
};
