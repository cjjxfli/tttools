#include "utils.h"
#include <stdio.h>

namespace souitls
{
	static const char* const g_hex_str[] =
	{
		"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
		"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
		"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
		"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
		"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
		"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
		"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
		"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
		"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
		"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
		"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
		"b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
		"c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
		"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df",
		"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
		"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
	};

	static const char g_printable_char[] =
	{
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
		'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
		'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
		'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
	};

	static int __to_printable(const void *data, uint32_t len, char *output, uint32_t output_len, uint32_t *written_char_count)
	{
		if (!output_len)
		{
			return -1;
		}

		uint32_t i;
		for (i = 0; i < len && i < output_len - 1; ++i)
		{
			output[i] = g_printable_char[((uint8_t*)data)[i]];
		}

		output[i] = '\0';
		if (written_char_count)
		{
			*written_char_count = i;
		}
		return (i == len) ? 0 : 1;
	}
	static int to_printable(const void *data, uint32_t len, char *output, uint32_t output_len)
	{
		return __to_printable(data, len, output, output_len, NULL);
	}

	// 0:  all content output
	// 1:  portion content output
	// -1: error
	static int __dump_hex1(const void *data, uint32_t len, char *output, uint32_t output_len, int with_blank_space, uint32_t *written_char_count)
	{
		if (!output_len)
		{
			return -1;
		}

		uint32_t i, j, k;
		for (i = 0, j = 0, k = j; i < len && j + 3 <= output_len; ++i)
		{
			*(uint16_t*)&output[j] = *(uint16_t*)g_hex_str[((uint8_t*)data)[i]];
			j += 2;
			k = j;
			if (with_blank_space)
			{
				output[j++] = ' ';
			}
		}

		output[k] = '\0';
		if (written_char_count)
		{
			*written_char_count = k;
		}
		return (i == len) ? 0 : 1;
	}

	const char* dump_hex(const void *data, uint32_t len, char *output, uint32_t output_len)
	{
		__dump_hex1(data, len, output, output_len, 1, NULL);
		return output;
	}

	// 0:  all content output
	// 1:  portion content output
	// -1: error
	int dump_hex1(const void *data, uint32_t len, char *output, uint32_t output_len, int with_blank_space)
	{
		return __dump_hex1(data, len, output, output_len, with_blank_space, NULL);
	}

	// 0:  all content output
	// 1:  portion content output
	// -1: error
	int dump_hex2(const void *data, uint32_t len, char *output, uint32_t output_len)
	{
		/* output format
		0000: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f  ................
		0010: 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f  ................
		*/
		if (!output_len)
		{
			return -1;
		}

		uint32_t base_addr_width = 2;
		if (len)
		{
			uint32_t i;
			for (base_addr_width = 2, i = len - 1; (i >>= 8); base_addr_width += 2)
			{
				// empty
			}
		}

		int offset = 0;
		uint32_t j, byte_count;
		for (j = 0; j < len; j += byte_count)
		{
			byte_count = len - j;
			if (byte_count > 16)
			{
				byte_count = 16;
			}

			if (!output_len)
			{
				return 1;
			}
#ifdef WIN32
			int result = _snprintf(&output[offset], output_len, "%0*x: ", base_addr_width, j);
#else
			int result = snprintf(&output[offset], output_len, "%0*x: ", base_addr_width, j);
#endif
			if (result >= output_len)
			{
				// truncated
				return 1;
			}

			offset += result;
			output_len -= result;

			uint32_t written_char_count;
			result = __dump_hex1((char*)data + j, byte_count, &output[offset], output_len, 1, &written_char_count);
			if (result)
			{
				return 1;
			}

			offset += written_char_count;
			output_len -= written_char_count;

			if (output_len < (16 - byte_count) * 3 + 3)
			{
				return 1;
			}

			memset(&output[offset], ' ', (16 - byte_count) * 3);
			offset += (16 - byte_count) * 3;
			output_len -= (16 - byte_count) * 3;

			output[offset++] = ' ';
			output[offset++] = ' ';
			output[offset] = '\0';
			output_len -= 2;

			result = __to_printable((char*)data + j, byte_count, &output[offset], output_len, &written_char_count);
			if (result)
			{
				return 1;
			}

			offset += written_char_count;
			output_len -= written_char_count;

			if (output_len < 2)
			{
				return 1;
			}
#ifdef WIN32
			output[offset++] = '\r\n';
#else
			output[offset++] = '\n';
#endif
			--output_len;
		}

		output[offset++] = '\0';

		return offset;
	}

	const char* ip2str_r(unsigned int ip, char *buf)
	{
		sprintf(buf, "%hhu.%hhu.%hhu.%hhu",
			((unsigned char*)&ip)[3], ((unsigned char*)&ip)[2],
			((unsigned char*)&ip)[1], ((unsigned char*)&ip)[0]);

		return buf;
	}
};