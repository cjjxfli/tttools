#include "stdafx.h"
#include "yg_spp_utils.h"
#include <string.h>
#include <stdio.h>

namespace yg_utils
{
	const char* ip2str_r(unsigned int ip, char *buf)
	{
		sprintf(buf, "%hhu.%hhu.%hhu.%hhu",
			((unsigned char*)&ip)[3], ((unsigned char*)&ip)[2],
			((unsigned char*)&ip)[1], ((unsigned char*)&ip)[0]);

		return buf;
	}

	BOOL CheckUShortNumber(CString cstrNum, unsigned short & out_number)
	{
		//先按unsigned long类型处理
		char   *stop_at = NULL;
		unsigned long number = strtoul(cstrNum.GetBuffer(), &stop_at, 0);
		if (number > 32767)
		{
			return FALSE;
		}
		out_number = (unsigned short)number;
		return TRUE;
	}

	BOOL CheckUIntNumber(CString cstrNum, unsigned int & out_number)
	{
		//先按unsigned long类型处理
		char   *stop_at = NULL;
		unsigned long number = strtoul(cstrNum.GetBuffer(), &stop_at, 0);
		if (number > 2147483647)
		{
			return FALSE;
		}
		out_number = (unsigned int)number;
		return TRUE;
	}

	BOOL CheckULongNumber(CString cstrNum, unsigned long & out_number)
	{
		//先按unsigned long类型处理
		char   *stop_at = NULL;
		unsigned long number = strtoul(cstrNum.GetBuffer(), &stop_at, 0);
		if (number > 9223372036854775807)
		{
			return FALSE;
		}
		out_number = number;
		return TRUE;
	}
};