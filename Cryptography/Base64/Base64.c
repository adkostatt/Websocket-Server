/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

// Я капельку переделал этот код, чтобы он лучше вписывался под мои задачи

#include <Cryptography/Base64/Base64.h>

static const unsigned char base64Table[65] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @length: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 * Returns: Allocated buffer of out_len bytes of encoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer. Returned buffer is
 * nul terminated to make it easier to use as a C string. The nul terminator is
 * not included in out_len.
 */
const size_t Base64Encode(
	unsigned char* out, const unsigned char* src,
	size_t length
)
{
	unsigned char* position;
	const unsigned char* end, * in;
	size_t outLength;
	int lineLength;

	outLength = length * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	outLength += outLength / 72; /* line feeds */
	outLength++; /* nul termination */
	if (outLength < length)
		return 0; /* integer overflow */

	end = src + length;
	in = src;
	position = out;
	lineLength = 0;
	while (end - in >= 3) {
		*position++ = base64Table[in[0] >> 2];
		*position++ = base64Table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*position++ = base64Table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*position++ = base64Table[in[2] & 0x3f];
		in += 3;
		lineLength += 4;
		if (lineLength >= 72) {
			*position++ = '\n';
			lineLength = 0;
		}
	}

	if (end - in) {
		*position++ = base64Table[in[0] >> 2];
		if (end - in == 1) {
			*position++ = base64Table[(in[0] & 0x03) << 4];
			*position++ = '=';
		}
		else {
			*position++ = base64Table[((in[0] & 0x03) << 4) |
				(in[1] >> 4)];
			*position++ = base64Table[(in[1] & 0x0f) << 2];
		}
		*position++ = '=';
		lineLength += 4;
	}

	/*if (lineLength)
		*position++ = '\n';

	*position = '\0';*/
	
	return position - out;
}
