#ifndef __UTILS_H__
#define __UTILS_H__
#include <string.h>

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
namespace souitls
{
	const char* dump_hex(const void *data, uint32_t len, char *output, uint32_t output_len);
	int dump_hex1(const void *data, uint32_t len, char *output, uint32_t output_len, int with_blank_space);
	int dump_hex2(const void *data, uint32_t len, char *output, uint32_t output_len);

	const char* ip2str_r(unsigned int ip, char *buf);
};
#endif //!__UTILS_H__