#pragma once
#include <unordered_map>
#include <string_view>

// Мб сделать что-то вроде struct Property
// И там const char* name и const char* value
struct Headers
{
	const char** names;
	const char** values;
	const int count;
};

// TODO: Поддержка протоколов сжатия
struct Http
{
	const char* method;
	const char* path;
	const char* payload;
	std::unordered_map<std::string_view, const char*> headers;
	
	// data будет изменен, а также нужна гарантия, что размер data больше, чем dataLength на 3 байта
	Http(
		char* data,
		int dataLength
	) noexcept;

	static const int CreateHttp(
		char* buffer,
		const Headers* headers
	) noexcept;

	// Первые байты в buffer должны быть: Method Path Version\r\n
	static const int CreateHttp(
		char* buffer,
		const Headers* headers,
		const char* payload,
		const size_t payloadLength
	);
};
