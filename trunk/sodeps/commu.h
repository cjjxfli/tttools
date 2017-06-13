#pragma once
#include <string.h>

//����blob
typedef struct tagBlobType
{
	inline tagBlobType()
	{
		memset(this, 0, sizeof(tagBlobType));
	}
	int len;//���ݳ���
	char* data;	//���ݻ�����
	void* owner;	//���ָ��
	unsigned char ext_type;//��չ��������
	unsigned short ext_len;//��չ���ݳ���
	void* extdata;  //��չ����
}blob_type;

const int MAX_BLOB_DATA_LEN = (1 << 24);//16M
const unsigned short MAX_BLOB_EXTDATA_LEN = 0xffff;//64k