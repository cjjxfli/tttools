#include "stdafx.h"
#include "Client.h"


CClient::CClient()
{
	m_is_connected = false;
}


CClient::~CClient()
{
}

int CClient::init(int sock_type)
{
	m_sock_fd = m_socket.create(sock_type);
	return 0;
}

int CClient::fini()
{
	m_socket.close(m_sock_fd);
	return 0;
}

int CClient::send(const void* buf, unsigned buf_size, unsigned& sent_len)
{
	if (!m_is_connected)
	{
		return -1;
	}
	int ret = m_socket.send(m_sock_fd, buf, buf_size, sent_len);
	if (0 == ret)
	{
		return sent_len;
	}
	return ret;
}

int CClient::receive(void* buf, unsigned buf_size, unsigned& received_len)
{
	int ret = m_socket.receive(m_sock_fd,buf,buf_size,received_len);
	if (ret == 0)
	{
		return received_len;
	}
	return ret;
}

int CClient::connect(const string addr, port_t port)
{
	int ret = m_socket.connect(m_sock_fd,addr,port);
	if (ret == 0)
	{
		m_is_connected = true;
		m_socket.set_nonblock(m_sock_fd);
		return 0;
	}
	return -1;
}

int CClient::close()
{
	if (m_is_connected)
	{
		m_socket.close(m_sock_fd);
		m_is_connected = false;
	}
	return 0;
}

int CClient::send_once(const string addr, port_t port, const void* buf, unsigned buf_size, int sock_type)
{
	init(sock_type);
	connect(addr, port);
	unsigned sent_len = 0;
	send(buf, buf_size, sent_len);
	fini();
	return 0;
}
