//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "network.h"
#include "error mgr.h"

//------------------------------------------------------------------------------
char		Network::hostname[256];																									//	the name of the host
hostent	*Network::hostentry;																										//	descriptor record for the host

//------------------------------------------------------------------------------
inline	void	NetError (int result, cstr call, cstr file, int line)							//	handle an error from a socket function
{																																								//	begin
	if (result == SOCKET_ERROR)																										//	if the result is an error
		Error (call, file, line);																										//	bail out
}																																								//	end

//------------------------------------------------------------------------------
#define	NetCheck(a)	Check (a, NetError)																					//	macro to facilitate error checking

//------------------------------------------------------------------------------
Network::Network (int port, Bool nob, int bufSize)															//	constructor
{																																								//	begin
	noblock = nob;																																//	save the no blocking flag
	bufferSize = bufSize << 10;																										//	set the buffer size in kilobytes
	buffer = new char[bufferSize];																								//	allocate the read buffer
	current = end = 0;																														//	initialize the reader
	sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);														//	build the connection socket
	address.sin_family = AF_INET;																									//	this is an internet TCP socket
	address.sin_port = htons (port);																							//	set the port
	address.sin_addr.s_addr = *(int *) (hostentry->h_addr_list[0]);								//	use my address
	int		length = sizeof (address);																							//	figure the size of the memory occupied by my address
	NetCheck (bind (sock, (sockaddr *) &address, length));												//	bind the socket to the requested address and port
	NetCheck (getsockname (sock, (sockaddr *) &address, &length));								//	get back the resulting binding
}																																								//	end

//------------------------------------------------------------------------------
Network::Network (SOCKET sok, Bool nob, int bufSize)														//	constructor
{																																								//	begin
	noblock = nob;																																//	save the no blocking flag
	bufferSize = bufSize << 10;																										//	set the buffer size in kilobytes
	buffer = new char[bufferSize];																								//	allocate the read buffer
	current = end = 0;																														//	initialize the reader
	sock = sok;																																		//	copy the socket
	int		length = sizeof (address);																							//	figure the size of the memory occupied by my address
	NetCheck (getsockname (sock, (sockaddr *) &address, &length));								//	get the socket binding
}																																								//	end

//------------------------------------------------------------------------------
Network::~Network (void)																												//	destructor
{																																								//	begin
	NetCheck (closesocket (sock));																								//	close the socket
	delete[] buffer;																															//	release the buffer
}																																								//	end

//------------------------------------------------------------------------------
int		Network::Read (void *data, long size, int num)														//	read data from the connection
{																																								//	begin
	int		bytesToMove = num * size,																								//	compute the number of bytes to move
				bytesRead = 0;																													//	start with no bytes read
	char	*dst = (char *) data;																										//	typecast the receiver
	while (bytesRead < bytesToMove)																								//	loop until we've read the requested number of bytes
		if (current < end)																													//	if there is data in the buffer
			while ((bytesRead < bytesToMove) && (current < end))											//	loop until it is all read or the request is fulfilled
				dst[bytesRead++] = buffer[current++];																		//	copying bytes from the buffer to the receiver
		else																																				//	otherwise, we want to fill the buffer
		{																																						//	begin
			current = 0;																															//	reset the reader
			end = recv (sock, buffer, bufferSize, 0);																	//	read some data from the connection
			if ((end == 0) || ((end == SOCKET_ERROR) && (bytesRead == 0)))						//	if there is no data to be read and we haven't started reading
				break;																																	//	bail out of the loop
		}																																						//	end
	return bytesRead;																															//	return the number of bytes read
}																																								//	end

//------------------------------------------------------------------------------
int		Network::Write (const void *data, long size)															//	write data to the connection
{																																								//	begin
	return send (sock, (const char *) data, size, 0);															//	send the data
}																																								//	end

//------------------------------------------------------------------------------
void	Network::Connect (cstr name, short port)																	//	connect to the specified target
{																																								//	begin
	hostent	*targetentry = gethostbyname (name);																	//	get the address of the target
	Assert (targetentry);																													//	be sure the result is valid
	sockaddr_in	addr;																															//	address structure for the target
	addr.sin_family = AF_INET;																										//	this is an internet TCP socket
	addr.sin_port = htons (port);																									//	set the port we want to connect to
	addr.sin_addr.s_addr = *(int *) (targetentry->h_addr_list[0]);								//	and the ip address
	NetCheck (connect (sock, (sockaddr *) &addr, sizeof (addr)));									//	make the connection
	NetCheck (ioctlsocket (sock, FIONBIO, &noblock));															//	set the socket blocking state
}																																								//	end

//------------------------------------------------------------------------------
void	Network::Listen (void)																										//	listen for connections on this port
{																																								//	begin
	NetCheck (listen (sock, 5));																									//	set the socket to listen
}																																								//	end

//------------------------------------------------------------------------------
Network	*Network::Accept (void)																									//	accept any new connections
{																																								//	begin
	SOCKET	newsock = accept (sock, 0, 0);																				//	try to accept a new connection
	if (newsock != INVALID_SOCKET)																								//	if the operation is successful
		return new Network (newsock, noblock, bufferSize >> 10);										//	return a new connection with the same characteristics as this listener
	return 0;																																			//	return a null pointer
}																																								//	end

//------------------------------------------------------------------------------
void	Network::Open (void)																											//	start up the network subsystem
{																																								//	begin
	WSADATA			data;																															//	place to get info about the windows sockets driver
	WORD				version = MAKEWORD (1, 1);																				//	I want version 1.1 or later
	NetCheck (WSAStartup (version, &data));																				//	start up the network subsystem
	Assert (data.wVersion == version);																						//	be sure the implementation is usable
	NetCheck (gethostname (hostname, 256));																				//	get the host name
	hostentry = gethostbyname (hostname);																					//	get the host entry associated with this machine
}																																								//	end

//------------------------------------------------------------------------------
void	Network::Close (void)																											//	close down the network subsystem
{																																								//	begin
	NetCheck (WSACleanup ());																											//	close the windows sockets implementation
}																																								//	end

//------------------------------------------------------------------------------
