#include "socket.h"
#include <ws2ipdef.h>
#include <Ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

using namespace sockcommu;

CSocket::CSocket()
{
	init();
}

CSocket::~CSocket()
{
	WSACleanup();
}

int CSocket::init()
{
	WORD wVersionRequested;
	WSADATA wsaData;    // 这结构是用于接收Wjndows Socket的结构信息的  
	int err;

	wVersionRequested = MAKEWORD(1, 1);   // 请求1.1版本的WinSock库  

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
	{
		return -1;          // 返回值为零的时候是表示成功申请WSAStartup  
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) 
	{
		// 检查这个低字节是不是1，高字节是不是1以确定是否我们所请求的1.1版本  
		// 否则的话，调用WSACleanup()清除信息，结束函数  
		WSACleanup();
		return -1;
	}
	return 0;
}

string CSocketAddr::in_n2s(ip_4byte_t addr)
{
	char buf[INET_ADDRSTRLEN];
	const char* p = inet_ntop(AF_INET, &addr, buf, sizeof(buf));
    return p ? p : string();
}

int CSocketAddr::in_s2n(const string& addr, ip_4byte_t& addr_4byte)
{
    struct in_addr sinaddr;
    errno = 0;
	int ret = inet_pton(AF_INET, addr.c_str(), &sinaddr);

    if (ret < 0)
    {
        if (errno != 0)
            return 0-errno;
        else
            return ret;
    }
    else if (ret == 0)
    {
        return -1;
    }
    else
    {
		addr_4byte.s_addr = sinaddr.s_addr;
        return 0;    //ret;
    }
}

//////////////////////////////////////////////////////////////////////////
//
//	return >0, on success
//	return <=0, on -errno or unknown error
//
int CSocket::create(int sock_type)
{
    errno = 0;
    int fd = 0;
    switch(sock_type)
    {
    case TCP_SOCKET:
        fd =::socket(PF_INET, SOCK_STREAM, 0);
        break;
    case UDP_SOCKET:
        fd =::socket(PF_INET, SOCK_DGRAM, 0);
        break;
    case UNIX_SOCKET:
        fd =::socket(PF_UNIX, SOCK_STREAM, 0);
        break;
    default:
        break;
    }

    if(fd < 0)
    {
        return errno ? -errno : fd;
    }
    else
    {
        return fd;
    }
}

void CSocket::close(int fd)
{
    ::closesocket(fd);
}

//
//	return 0, on success
//	return < 0, on -errno or unknown error
//
int CSocket::bind(int fd, const string &server_address, port_t port)
{
	ip_4byte_t ip = { 0 };
    int ret = CSocketAddr::in_s2n(server_address, ip);
    if(ret < 0) 
        return ret;

    return bind(fd, ip, port);    
}
int CSocket::bind(int fd, ip_4byte_t ip, port_t port)
{
    CSocketAddr addr;
    addr.set_family(AF_INET);
    addr.set_port(port);
    addr.set_numeric_ipv4(ip);

    errno = 0;
    int ret = ::bind(fd, addr.addr(), addr.length());
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}

//
//	bind on *:port
//	return 0, on success
//	return < 0, on -errno or unknown error
//
int CSocket::bind_any(int fd, port_t port)
{
    CSocketAddr addr;
    addr.set_family(AF_INET);
    addr.set_port(port);
	ULONG ip = htonl(INADDR_ANY);
	addr.set_numeric_ipv4(*((in_addr *)&ip));

    errno = 0;
    int ret = ::bind(fd, addr.addr(), addr.length());
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}

// int CSocket::bind(int fd, const string& path)
// {
//     struct sockaddr_in addr;
// 	memset(&addr, 0, sizeof (addr));
// 	addr.sin_family = PF_UNIX;
//     strncpy(addr.sun_path, path.c_str(), path.length());
//     socklen_t addrlen = SUN_LEN(&addr);
//     addr.sun_path[0] = '\0';
// 
//     errno = 0;
//     int ret = ::bind (fd, (struct sockaddr *)&addr, addrlen);
//     return (ret < 0) ? (errno ? -errno : ret) : 0;
// }
//
//	return 0, on success
//	return < 0, on -errno or unknown error
//
int CSocket::listen(int fd, int backlog)
{
    errno = 0;
    int ret = ::listen(fd, backlog);
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}

//
//	accept a new connection, and attach it into the client_socket parameter
//	return 0, on success
//	return < 0, on -errno or unknown error
//
int CSocket::accept(int fd)
{
    errno = 0;
    int ret =::accept(fd, NULL, NULL);

    if (ret <= 0)
        return errno ? -errno : ret;
    else
    {
        return ret;
    }
}

//
//	return 0, on success
//	return < 0, on -errno or unknown error
//
int CSocket::connect(int fd, const string& address, port_t port)
{
	ip_4byte_t ip = { 0 };
    int ret = CSocketAddr::in_s2n(address, ip);
    if (ret < 0) 
        return ret;

    return connect(fd, ip, port);
}

//
//	return 0, on success
//	return < 0, on -errno or unknown error
//
int CSocket::connect(int fd, ip_4byte_t address, port_t port)
{
    CSocketAddr addr;
    addr.set_family(AF_INET);
    addr.set_port(port);
    addr.set_numeric_ipv4(address);
    errno = 0;
    int ret = ::connect(fd, addr.addr(), addr.length());
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}

// int CSocket::connect(int fd, const string& path)
// {
//     struct sockaddr_un addr;
//     bzero (&addr, sizeof (struct sockaddr_un));
//     addr.sun_family = PF_UNIX;
//     strncpy(addr.sun_path, path.c_str(), path.length());
//     socklen_t addrlen = SUN_LEN(&addr);
//     addr.sun_path[0] = '\0';
// 
//     errno = 0;
//     int ret = ::connect(fd, (struct sockaddr*)&addr, addrlen);
//     return (ret < 0) ? (errno ? -errno : ret) : 0;
// }


//
//	return 0, on success
//	return < 0, on -errno or unknown error
//
//	to receive data buffer: buf/buf_size
//	received data length: received_len
//
int CSocket::receive(int fd, void *buf, unsigned buf_size, unsigned& received_len, int flag /* = 0 */)
{
    errno = received_len = 0;
    int bytes =::recv(fd, (char*)buf, buf_size, flag);
    if(bytes < 0)
    {
        return errno ? -errno : bytes;
    }
    else
    {
        received_len = bytes;
        return 0;
    }
}

int CSocket::receive(int fd, void* buf, unsigned buf_size, unsigned& received_len, CSocketAddr& addr) 
{
    errno = received_len = 0;
	int addr_len = addr.length();
	int bytes = ::recvfrom(fd, (char*)buf, buf_size, MSG_TRUNC, addr.addr(), &addr_len);
    if(bytes < 0)
    {
        return errno ? -errno : bytes;
    }
    else
    {
        received_len = bytes;
        return 0;
    }
}
//
//	return 0, on success
//	return < 0, on -errno or unknown error
//
//	to be sent data buffer: buf/buf_size
//	done data length: sent_len
//
int CSocket::send(int fd, const void *buf, unsigned buf_size, unsigned& sent_len, int flag /* = 0 */)
{
    errno = 0;
	int bytes = ::send(fd, (char*)buf, buf_size, flag);

    if(bytes < 0)
    {
        return errno ? -errno : bytes;
    }
    else
    {
        sent_len = bytes;
        return 0;
    }
}
int CSocket::send(int fd, const void *buf, unsigned buf_size, unsigned& sent_len, CSocketAddr& addr)
{
    errno = 0;
	int bytes = ::sendto(fd, (char*)buf, buf_size, 0, addr.addr(), addr.length());

    if(bytes < 0)
    {
        return errno ? -errno : bytes;
    }
    else
    {
        sent_len = bytes;
        return 0;
    }
}

int CSocket::shutdown(int fd)
{
	int ret = ::shutdown(fd, SD_BOTH);
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}

//////////////////////////////////////////////////////////////////////////

int CSocket::get_peer_name(int fd, ip_4byte_t& peer_address, port_t& peer_port)
{
    CSocketAddr addr;
	int addr_len = addr.length();
    int ret = ::getpeername(fd, addr.addr(), &addr_len);
    if (ret < 0)
        return errno ? -errno : ret;

    peer_address = addr.get_numeric_ipv4();
    peer_port = addr.get_port();
    return 0;
}

int CSocket::get_peer_name(int fd, string & peer_address, port_t & peer_port)
{
	ip_4byte_t ip = { 0 };
    int ret = get_peer_name(fd, ip, peer_port);
    if (ret < 0) return ret;

    peer_address = CSocketAddr::in_n2s(ip);
    return 0;
}

int CSocket::get_sock_name(int fd, ip_4byte_t& socket_address, port_t & socket_port)
{
    CSocketAddr addr;
	int addr_len = addr.length();
    int ret = ::getsockname(fd, addr.addr(), &addr_len);
    if (ret < 0)
        return errno ? -errno : ret;

    socket_address = addr.get_numeric_ipv4();
    socket_port = addr.get_port();
    return 0;
}

int CSocket::get_sock_name(int fd, string & socket_address, port_t & socket_port)
{
	ip_4byte_t ip = { 0 };
    int ret = get_sock_name(fd, ip, socket_port);
    if (ret < 0)
        return ret;

    socket_address = CSocketAddr::in_n2s(ip);
    return 0;
}

int CSocket::set_reuseaddr(int fd)
{
    int optval = 1;
    unsigned optlen = sizeof(optval);
	int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, optlen);
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}

int CSocket::set_nonblock(int fd)
{
	unsigned long ul = 1;
	int ret = ioctlsocket(fd, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式。  
    return (ret < 0) ? (errno ? -errno : ret) : 0;
}
void CSocket::set_timeout(int fd, int ms)
{
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(tv));
}


