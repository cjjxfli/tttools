/*!
* \file yg_spp_utils.h
* \date 2017/05/05 15:47
*
* \author xf.li
* Contact: xf.li@nfyg.com.cn
*
* \brief
*
* TODO: long description
* 一些常用函数的封装
* \note
*/
#ifndef __YG_SPP_UTILS_H__
#define __YG_SPP_UTILS_H__
#include <time.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>  
#include <sstream>
#include <algorithm>
#include <iterator>

namespace yg_utils
{
	const char* ip2str_r(unsigned int ip, char *buf);

	BOOL CheckUShortNumber(CString cstrNum, unsigned short & out_number);
	BOOL CheckUIntNumber(CString cstrNum, unsigned int & out_number);
	BOOL CheckULongNumber(CString cstrNum, unsigned long & out_number);
};

#endif //!__YG_SPP_UTILS_H__