/**
 * (C) 2010 "CBOSS"
 * 
 * \file	SocketClient.h
 * \author	NguyenTran
 * \date	01.10.2010
 * 
 */

#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <sys/poll.h>

#include <unistd.h>
#include <string.h>
#include <netdb.h>

#define MAX_URL_LEN 128
#define MAX_RESPONSE_LEN 1024

/** 
 * @brief Represents MOXA socket client
 */
struct SocketClient
{
private:

	//! File discription of socket
	int m_SocketClientFD;
	
	//! Port used socket
	int m_port;

	//! Ip address of socket server
	unsigned char m_strURL[MAX_URL_LEN];	

public:
	
	/** 
	 * @brief Constructor for init setting
	 * @param ip: the ip address of socket server
	 * @param port: port of socket
	 */	
	SocketClient();

	//! Destructor
	~SocketClient();

	/** 
	 * @brief Reads len bytes from connected socket to buf
	 * 
	 * @param buf Buffer to store data
	 * @param len Size of buffer and number of bytes to read
	 * 
	 * @return Number of bytes successfully read
	 * @throws signal_interrupt in case of EINTR
	 * @throws std::exception in case of unimplemented error conditions
	 */
	size_t Read(unsigned char* buf, size_t len);

	//! Socket Poll result
	typedef enum {
		Triggered,
		Timeout
	} PollResult;

	/** 
	 * @brief Waits for unblocking of port fd. Executes poll(&fd_info,1, timeout_ms);
	 * 
	 * @param timeout_ms Timeout in milliseconds
	 * 
	 * @retval Triggered Read operation will not block
	 * @retval Timeout poll() returns timeout
	 *
	 * @throws signal_interrupt in case of EINTR
	 * @throws std::exception in case of unexpected error conditions (POLLERR,
	 * POLLHUP, etc)
	 */
	PollResult Poll(size_t timeout_ms);

	/** 
	 * @brief Waits for unblocking of port fd. Executes poll(&fd_info,1, timeout_ms);
	 * 
	 * @param timeout_ms Timeout in milliseconds
	 * 
	 * @retval Triggered Read operation will not block
	 * @retval Timeout poll() returns timeout
	 *
	 * @throws signal_interrupt in case of EINTR
	 * @throws std::exception in case of unexpected error conditions (POLLERR,
	 * POLLHUP, etc)
	 */
	size_t SendHttpRequest( char *response );
};

#endif // __SOCKET_CLIENT_H__


