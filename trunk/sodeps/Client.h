#pragma once
#ifdef WIN32
#include "win32/socket.h"
#else
#include "unix/socket.h"
#endif

using namespace sockcommu;

class CClient
{
public:
	CClient();
	~CClient();

	int init(int sock_type = TCP_SOCKET);
	int fini();
	int send(const void* buf, unsigned buf_size, unsigned& sent_len);
	int receive(void* buf, unsigned buf_size, unsigned& received_len);
	int connect(const string addr, port_t port);
	int close();

	int send_once(const string addr, port_t port, const void* buf, unsigned buf_size, int sock_type = TCP_SOCKET);

private:
	CSocket m_socket;
	int m_sock_fd;
	bool m_is_connected;
};

