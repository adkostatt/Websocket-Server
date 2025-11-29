/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include <stddef.h>

#ifndef BASE64_H
#define BASE64_H

#if defined(__cplusplus)
extern "C"
#endif
const size_t Base64Encode(
	unsigned char* out, const unsigned char* src,
	size_t length
);

#endif /* BASE64_H */
