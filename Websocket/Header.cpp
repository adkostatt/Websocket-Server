#include <Websocket/Header.hpp>

bool Header::GetFin(

) const noexcept
{
	return bits1 >> 7; // Хз лучше ли, чем через &
}
void Header::SetFin(
	bool flag
) noexcept
{
	bits1 |= flag ? FIN : FIN ^ 255;
}

bool Header::GetRsv1(

) const noexcept
{
	return bits1 & RSV1;
}
void Header::SetRsv1(
	bool flag
) noexcept
{
	bits1 |= flag ? RSV1 : RSV1 ^ 255;
}

bool Header::GetRsv2(

) const noexcept
{
	return bits1 & RSV2;
}
void Header::SetRsv2(
	bool flag
) noexcept
{
	bits1 |= flag ? RSV2 : RSV2 ^ 255;
}

bool Header::GetRsv3(

) const noexcept
{
	return bits1 & RSV3;
}
void Header::SetRsv3(
	bool flag
) noexcept
{
	bits1 |= flag ? RSV3 : RSV3 ^ 255;
}

uint8_t Header::GetOpcode(

) const noexcept
{
	return bits1 & OPCODE;
}
void Header::SetOpcode(
	uint8_t opcode
) noexcept
{
	bits1 |= opcode & OPCODE;
}

bool Header::GetMasked(

) const noexcept
{
	return bits2 >> 7;
}
void Header::SetMasked(
	bool flag
) noexcept
{
	bits2 |= flag ? MASKED : MASKED ^ 255;
}

uint8_t Header::GetLength(

) const noexcept
{
	return bits2 & PAYLOAD_LENGTH;
}
void Header::SetLength(
	const uint8_t length
) noexcept
{
	bits2 |= length & PAYLOAD_LENGTH;
}

Header::Header(

) noexcept : bits1(0), bits2(0)
{

}

Header::Header(
	const uint8_t bits1_,
	const uint8_t bits2_
) noexcept : bits1(bits1_), bits2(bits2_)
{

}

Header::Header(
	const uint16_t bits
) noexcept
{
	const uint8_t* bits8 = (decltype(bits8))bits;
	bits1 = bits8[0];
	bits2 = bits8[1];
}

Header::Header(
	bool fin, bool rsv1, bool rsv2,
	bool rsv3, const uint8_t opcode,
	bool masked, const uint64_t length
) noexcept : bits1(0), bits2(0)
{
	if (fin)
		bits1 |= FIN;
	if (rsv1)
		bits1 |= RSV1;
	if (rsv2)
		bits1 |= RSV2;
	if (rsv3)
		bits1 |= RSV3;
	bits1 |= opcode & OPCODE;

	if (masked)
		bits2 |= MASKED;

	if (length <= 125)
		bits2 |= length & PAYLOAD_LENGTH;
	else if (length <= UINT16_MAX)
		bits2 |= 126;
	else
		bits2 |= 127;
}
