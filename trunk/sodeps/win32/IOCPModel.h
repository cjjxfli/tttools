
/*!
 * \file IOCPModel.h
 * \date 2017/06/06 11:13
 *
 * \author xf.li
 * Contact: xf.li@nfyg.com.cn
 *
 * \brief 
 * ��ԭ�Ⱥ�MFC���ܽ�ϵĴ�������޸ģ�ʹ��ͨ��
 *
 * TODO: long description
 *
 * \note
*/
/*
==========================================================================

Purpose:

	* �����CIOCPModel�Ǳ�����ĺ����࣬����˵��WinSock�������˱��ģ���е�
	  ��ɶ˿�(IOCP)��ʹ�÷�������ʹ��MFC�Ի�����������������ʵ���˻�����
	  ����������ͨ�ŵĹ��ܡ�

	* ���е�PER_IO_DATA�ṹ���Ƿ�װ������ÿһ���ص������Ĳ���
	  PER_HANDLE_DATA �Ƿ�װ������ÿһ��Socket�Ĳ�����Ҳ��������ÿһ����ɶ˿ڵĲ���

	* ��ϸ���ĵ�˵����ο� http://blog.csdn.net/PiggyXP

Notes:

	* ���彲���˷������˽�����ɶ˿ڡ������������̡߳�Ͷ��Recv����Ͷ��Accept����ķ�����
	  ���еĿͻ��������Socket����Ҫ�󶨵�IOCP�ϣ����дӿͻ��˷��������ݣ�����ʵʱ��ʾ��
	  ��������ȥ��

Author:

	* PiggyXP��С��

Date:

	* 2009/10/04

==========================================================================
*/

#pragma once
#include <vector>
#include <iterator>
#include <string>
// winsock 2 ��ͷ�ļ��Ϳ�
#include <winsock2.h>
#include <MSWSock.h>
#pragma comment(lib,"ws2_32.lib")
#include <atlstr.h>

// ���������� (1024*8)
// ֮����Ϊʲô����8K��Ҳ��һ�������ϵľ���ֵ
// ���ȷʵ�ͻ��˷�����ÿ�����ݶ��Ƚ��٣���ô�����õ�СһЩ��ʡ�ڴ�
#define MAX_BUFFER_LEN        8192  
// Ĭ�϶˿�
#define DEFAULT_PORT          12345    
// Ĭ��IP��ַ
#define DEFAULT_IP            _T("127.0.0.1")

//�ص���������
typedef enum
{
	CB_CONNECT = 0,
	CB_CONNECTED,
	CB_DISCONNECT,
	CB_RECVDATA,
	CB_RECVERROR,
	CB_RECVDONE,
	CB_SENDDATA,
	CB_SENDERROR,
	CB_SENDDONE,
	CB_HANGUP,
	CB_OVERLOAD,
	CB_TIMEOUT,
}cb_type;
//�ص�����
typedef int(*cb_func) (unsigned int flow, void* arg1, void* arg2);
typedef int (*cb_error)(const char * error_msg,unsigned int len,void * arg1);

//////////////////////////////////////////////////////////////////
// ����ɶ˿���Ͷ�ݵ�I/O����������
typedef enum _OPERATION_TYPE  
{  
	ACCEPT_POSTED,                     // ��־Ͷ�ݵ�Accept����
	SEND_POSTED,                       // ��־Ͷ�ݵ��Ƿ��Ͳ���
	RECV_POSTED,                       // ��־Ͷ�ݵ��ǽ��ղ���
	NULL_POSTED                        // ���ڳ�ʼ����������

}OPERATION_TYPE;

//====================================================================================
//
//				��IO���ݽṹ�嶨��(����ÿһ���ص������Ĳ���)
//
//====================================================================================

typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED     m_Overlapped;                               // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)              
	SOCKET         m_sockAccept;                               // ������������ʹ�õ�Socket
	WSABUF         m_wsaBuf;                                   // WSA���͵Ļ����������ڸ��ص�������������
	char           m_szBuffer[MAX_BUFFER_LEN];                 // �����WSABUF�������ַ��Ļ�����
	OPERATION_TYPE m_OpType;                                   // ��ʶ�������������(��Ӧ�����ö��)

	// ��ʼ��
	_PER_IO_CONTEXT()
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));  
		ZeroMemory( m_szBuffer,MAX_BUFFER_LEN );
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = MAX_BUFFER_LEN;
		m_OpType     = NULL_POSTED;
	}
	// �ͷŵ�Socket
	~_PER_IO_CONTEXT()
	{
		if( m_sockAccept!=INVALID_SOCKET )
		{
			closesocket(m_sockAccept);
			m_sockAccept = INVALID_SOCKET;
		}
	}
	// ���û���������
	void ResetBuffer()
	{
		ZeroMemory( m_szBuffer,MAX_BUFFER_LEN );
	}

	int CopyDataToBuffer(const char * buffer,unsigned int len)
	{
		if (len > MAX_BUFFER_LEN || NULL == buffer)
		{
			return -1;
		}
		memcpy(m_szBuffer, buffer, len);
		m_wsaBuf.len = len;
		return 0;
	}
} PER_IO_CONTEXT, *PPER_IO_CONTEXT;


//====================================================================================
//
//				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================

typedef struct _PER_SOCKET_CONTEXT
{  
	SOCKET      m_Socket;                                  // ÿһ���ͻ������ӵ�Socket
	SOCKADDR_IN m_ClientAddr;                              // �ͻ��˵ĵ�ַ
	std::vector<_PER_IO_CONTEXT*> m_arrayIoContext;             // �ͻ���������������������ݣ�
	                                                       // Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����

	// ��ʼ��
	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_ClientAddr, 0, sizeof(m_ClientAddr)); 
	}

	// �ͷ���Դ
	~_PER_SOCKET_CONTEXT()
	{
		if( m_Socket!=INVALID_SOCKET )
		{
			closesocket( m_Socket );
		    m_Socket = INVALID_SOCKET;
		}
		// �ͷŵ����е�IO����������
		std::vector<_PER_IO_CONTEXT*>::iterator it = m_arrayIoContext.begin();
		for (; it != m_arrayIoContext.end(); it++)
		{
			_PER_IO_CONTEXT * tmp = *it;
			delete tmp;
		}
		m_arrayIoContext.clear();
	}

	// ��ȡһ���µ�IoContext
	_PER_IO_CONTEXT* GetNewIoContext()
	{
		_PER_IO_CONTEXT* p = new _PER_IO_CONTEXT;

		m_arrayIoContext.push_back(p);

		return p;
	}

	// ���������Ƴ�һ��ָ����IoContext
	void RemoveContext( _PER_IO_CONTEXT* pContext )
	{
		if (NULL == pContext)
		{
			return;
		}
		std::vector<_PER_IO_CONTEXT*>::iterator it = m_arrayIoContext.begin();
		for (; it != m_arrayIoContext.end(); it++)
		{
			if (pContext == *it)
			{
				delete pContext;
				pContext = NULL;
				m_arrayIoContext.erase(it);
				break;
			}
		}
	}

	_PER_IO_CONTEXT* GetIOContext(int sock_fd)
	{
		std::vector<_PER_IO_CONTEXT*>::iterator it = m_arrayIoContext.begin();
		for (; it != m_arrayIoContext.end(); it++)
		{
			if ((*it)->m_sockAccept == sock_fd)
			{
				return *it;
			}
		}
		return NULL;
	}

} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;




//====================================================================================
//
//				CIOCPModel�ඨ��
//
//====================================================================================

// �������̵߳��̲߳���
class CIOCPModel;
typedef struct _tagThreadParams_WORKER
{
	CIOCPModel* pIOCPModel;                                   // ��ָ�룬���ڵ������еĺ���
	int         nThreadNo;                                    // �̱߳��

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 

// CIOCPModel��
class CIOCPModel
{
public:
	CIOCPModel(void);
	~CIOCPModel(void);

public:

	// ����������
	bool Start();

	//	ֹͣ������
	void Stop();

	// ����Socket��
	bool LoadSocketLib();

	// ж��Socket�⣬��������
	void UnloadSocketLib() { WSACleanup(); }

	// ��ñ�����IP��ַ
	CString GetLocalIP();

	// ���ü����˿�
	void SetPort( const int& nPort ) { m_nPort=nPort; }
	void SetServerAddr(const char * ip)
	{
		m_strIP = ip;
	}

	// �����������ָ�룬���ڵ�����ʾ��Ϣ��������
	//void SetMainDlg( CDialog* p ) { m_pMain=p; }

	int DoSend(unsigned int sock_fd,const char * buffer,unsigned int len);

	CString & GetMsgInfo()
	{
		return m_msg_info;
	}

	int reg_cb_error(cb_error fn_error,void * args)
	{
		m_cb_error = fn_error;
		m_cb_error_args = args;
		return 0;
	}

	int reg_cb(cb_type type, cb_func func, void* args = NULL)
	{
		if (type <= CB_TIMEOUT)
		{
			m_func_list_[type] = func;
			m_func_args_[type] = args;
			return 0;
		}
		else
		{
			return -1;
		}
	}

protected:

	// ��ʼ��IOCP
	bool _InitializeIOCP();

	// ��ʼ��Socket
	bool _InitializeListenSocket();

	// ����ͷ���Դ
	void _DeInitialize();

	// Ͷ��Accept����
	bool _PostAccept( PER_IO_CONTEXT* pAcceptIoContext ); 

	// Ͷ�ݽ�����������
	bool _PostRecv( PER_IO_CONTEXT* pIoContext );

	bool _PostSend(PER_IO_CONTEXT* pIoContext);

	// ���пͻ��������ʱ�򣬽��д���
	bool _DoAccpet( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext );

	// ���н��յ����ݵ����ʱ�򣬽��д���
	bool _DoRecv( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext );

	bool _DoSend(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);

	// ���ͻ��˵������Ϣ�洢��������
	void _AddToContextList( PER_SOCKET_CONTEXT *pSocketContext );

	// ���ͻ��˵���Ϣ���������Ƴ�
	void _RemoveContext( PER_SOCKET_CONTEXT *pSocketContext );

	//ͨ��socket�����ȡio������
	_PER_IO_CONTEXT * _GetIOContext(unsigned int sock_fd);

	PER_SOCKET_CONTEXT * _GetSocketContext(unsigned int sock_fd);

	// ��տͻ�����Ϣ
	void _ClearContextList();

	// ������󶨵���ɶ˿���
	bool _AssociateWithIOCP( PER_SOCKET_CONTEXT *pContext);

	// ������ɶ˿��ϵĴ���
	bool HandleError( PER_SOCKET_CONTEXT *pContext,const DWORD& dwErr );

	// �̺߳�����ΪIOCP�������Ĺ������߳�
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	// ��ñ����Ĵ���������
	int _GetNoOfProcessors();

	// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ�
	bool _IsSocketAlive(SOCKET s);

	// ������������ʾ��Ϣ
	void _ShowMessage(const CString szFormat, ...) const;

	

private:

	HANDLE                       m_hShutdownEvent;              // ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�

	HANDLE                       m_hIOCompletionPort;           // ��ɶ˿ڵľ��

	HANDLE*                      m_phWorkerThreads;             // �������̵߳ľ��ָ��

	int		                     m_nThreads;                    // ���ɵ��߳�����

	CString						m_strIP;                       // �������˵�IP��ַ
	int                          m_nPort;                       // �������˵ļ����˿�

	//CDialog*                     m_pMain;                       // ������Ľ���ָ�룬����������������ʾ��Ϣ
	static CString						m_msg_info;

	CRITICAL_SECTION             m_csContextList;               // ����Worker�߳�ͬ���Ļ�����

	std::vector<PER_SOCKET_CONTEXT*>  m_arrayClientContext;          // �ͻ���Socket��Context��Ϣ        

	PER_SOCKET_CONTEXT*          m_pListenContext;              // ���ڼ�����Socket��Context��Ϣ

	LPFN_ACCEPTEX                m_lpfnAcceptEx;                // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
	LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs; 

	cb_func m_func_list_[CB_TIMEOUT + 1];
	void* m_func_args_[CB_TIMEOUT + 1];

	cb_error m_cb_error;
	void * m_cb_error_args;

};

