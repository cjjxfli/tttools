#pragma once
#include <string.h>

//数据blob
typedef struct tagBlobType
{
	inline tagBlobType()
	{
		memset(this, 0, sizeof(tagBlobType));
	}
	int len;//数据长度
	char* data;	//数据缓冲区
	void* owner;	//组件指针
	unsigned char ext_type;//扩展数据类型
	unsigned short ext_len;//扩展数据长度
	void* extdata;  //扩展数据
}blob_type;

const int MAX_BLOB_DATA_LEN = (1 << 24);//16M
const unsigned short MAX_BLOB_EXTDATA_LEN = 0xffff;//64k