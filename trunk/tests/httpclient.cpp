/*!
 * (C) 2010 "CBOSS"
 * 
 * \brief	
 * \file	SocketClient.cpp
 * \author	NguyenTran
 * \date	01.10.2010
 */

#include <stdexcept>
#include "httpclient.h"

const char *REQ_HTTP = 	"GET /trac/project/index.html HTTP/1.1"
 						"Host: \"123.30.53.13\""
 						"User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:12.0) Gecko/20100101 Firefox/12.0\r\n"
 						"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
 						"Accept-language: en-gb,en;q=0.5\r\n"
 						"Accept-encoding: gzip, deflate\r\n"
 						"Connection: keep-alive\r\n";								

SocketClient::SocketClient() : m_port(80)
{
	strncpy((char *)m_strURL, "172.17.107.8", MAX_URL_LEN); 
	struct sockaddr_in serv_addr;
    struct hostent *server;	

	server = gethostbyname((char *)m_strURL);
	if (server == NULL)
	{
		throw std::runtime_error("server is NULL");
	}	
	
	try
	{	
		// Open socket with domain AF_INET
		m_SocketClientFD = ::socket(AF_INET, SOCK_STREAM, 0);
		
		if (m_SocketClientFD < 0)
		{
			throw std::runtime_error("Error when try to create socket\n");	
		}
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(m_port);
		int len = sizeof(serv_addr);
		
		// Connect to socket with ip and port
		int res = ::connect(m_SocketClientFD, (struct sockaddr*)&serv_addr, len);
		if (res == -1)
		{
			throw std::runtime_error("Can't connect to socket with port");
		}
	}	
	catch(std::exception& err)
	{
		::close(m_SocketClientFD);
		throw;
	}
}

SocketClient::~SocketClient()
{
	if (m_SocketClientFD >= 0)
		::close(m_SocketClientFD);
}

size_t SocketClient::Read(unsigned char* buf, size_t len)
{	
	int res = ::read(m_SocketClientFD, (char*)buf, int(len));
	if(res < 0)
	{
		int errsv = errno;
		if(errsv == EINTR)
			throw std::runtime_error("SocketClient::Read was interrupted by the signal.");
		throw std::runtime_error("Can't read from socket");
	}

	return size_t(res);
}

SocketClient::PollResult SocketClient::Poll(size_t timeout_ms)
{
	pollfd item;
	item.fd = m_SocketClientFD;
	if (item.fd == -1)
	{ 
		throw std::runtime_error("Invalid socket identifier");
	}
	/*
	#define POLLIN  1       // Set if data to read.
	#define POLLPRI 2       // Set if urgent data to read.
	#define POLLOUT 4       // Set if writing data wouldn't block.
	#define POLLERR   8     // An printf occured.
	#define POLLHUP  16     // Shutdown or close happened.
	#define POLLNVAL 32     // Invalid file descriptor.
	*/
	item.events = POLLIN | POLLPRI /*| POLLOUT */| POLLHUP | POLLERR | POLLNVAL;
	int result = ::poll(&item, 1, timeout_ms);

	if(result == 0)
		return SocketClient::Timeout;
	if(result < 0)
	{
		int errsv = errno;
		if(errsv == EINTR)
			throw std::runtime_error("SocketClient::Poll has been interrupted by the signal.");
		throw std::runtime_error("poll() returns with printf");
	}

	if( (item.revents & POLLERR) == 0)
	{
		throw std::runtime_error("POLLERR flag was set on port fd");
	}
	if( (item.revents & POLLNVAL) == 0)
	{
		throw std::runtime_error("POLLNVAL flag was set on port fd");
	}
	if( (item.revents & POLLHUP) == 0)
	{
		throw std::runtime_error("POLLHUP flag was set on port fd");
	}
	
	return SocketClient::Triggered;
}

size_t SocketClient::SendHttpRequest(char *response)
{
  	//int res = send(m_SocketClientFD, REQ_HTTP, strlen(REQ_HTTP),0);
	int res = write(m_SocketClientFD, REQ_HTTP, strlen(REQ_HTTP));
	if (res == -1)
	{	
		throw std::runtime_error("Can't send HTTP message");
	}

	unsigned char buf[1024] = {0,};
	res = Read(buf, 1023);
	if (res == -1)
	{	
		throw std::runtime_error("Can't read from HTTP server");
	}

	memset(response, 0, MAX_RESPONSE_LEN);
	strncpy((char *)response, (char *)buf, MAX_RESPONSE_LEN);

	return size_t(res);	
}

