#include <Http/Http.hpp>
#include <algorithm>

Http::Http(
	char* data,
	int dataLength
) noexcept
{
	method = nullptr;
	int headersStart = 0;
	int payloadStart = 0;
	int spaces = 0;

	if (dataLength < 13)
		return;

	for (int i = 0; i < dataLength; i++)
	{
		char& dataChar = data[i];

		if (dataChar == '\r' and data[i + 1] == '\n')
		{
			if (spaces != 2)
			{
				method = nullptr;
				return;
			}

			headersStart = i + 1;
			break;
		}
		else if (dataChar == ' ')
		{
			spaces++;
			if (spaces > 2)
			{
				method = nullptr;
				return;
			}

			dataChar = '\0';
			if (!method)
			{
				method = data;
				path = data + i + 1;
			}
		}
	}

	if (headersStart < 12)
	{
		method = nullptr;
		return;
	}

	for (int i = headersStart; i < dataLength; i++)
	{
		bool terminated = true;
		char* lastName = nullptr;
		char& dataChar = data[i];

		if (terminated)
		{
			lastName = data + i + 1;
			terminated = false;
		}

		if (dataChar == ':' and data[i + 1] == ' ')
		{
			dataChar = '\0';
			headers[lastName] = data + i + 2;
		}

		else if (dataChar == '\r' and data[i + 1] == '\n')
		{
			terminated = true;
			dataChar = '\0';
			if (data[i + 2] == '\r' and data[i + 3] == '\n')
			{
				payloadStart = i + 4;
				break;
			}
		}
	}

	if (payloadStart < 40)
	{
		method = nullptr;
		return;
	}

	payload = data + payloadStart;
	data[dataLength] = '\0';
}

const int Http::CreateHttp(
	char* buffer,
	const Headers* headers
) noexcept
{
	int offset = 10;

	for (;
		buffer[offset + 1] != '\n' or buffer[offset] != '\r';
		offset++)
	{
	}
	offset += 2;

	for (int i = 0; i < headers->count; i++)
	{
		const char* name = headers->names[i];
		const char* value = headers->values[i];

		for (int j = 0;
			name[j] != '\0';
			j++)
		{
			buffer[offset++] = name[j];
		}

		buffer[offset++] = ':';
		buffer[offset++] = ' ';

		for (int j = 0;
			value[j] != '\0';
			j++)
		{
			buffer[offset++] = value[j];
		}

		buffer[offset++] = '\r';
		buffer[offset++] = '\n';
	}

	buffer[offset++] = '\r';
	buffer[offset] = '\n';

	return offset + 1;
}

const int Http::CreateHttp(
	char* buffer,
	const Headers* headers,
	const char* payload,
	const size_t payloadLength
)
{
	const int offset = CreateHttp(buffer, headers);

	std::copy_n(payload, payloadLength, buffer + offset);

	return offset + payloadLength;
}
