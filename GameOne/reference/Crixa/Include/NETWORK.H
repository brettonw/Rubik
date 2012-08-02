//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/15/97
//
//------------------------------------------------------------------------------

#ifndef	NETWORK
#define	NETWORK

//------------------------------------------------------------------------------
class	Network																																		//	network connection class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
static	char				hostname[256];																							//	the name of the host
static	hostent			*hostentry;																									//	descriptor record for the host
				sockaddr_in	address;																										//	address record for the network connection
				SOCKET			sock;																												//	the network connection
				char				*buffer;																										//	the read buffer
				int					bufferSize;																									//	the size of the buffer in bytes
				int					current;																										//	the current point to read from
				int					end;																												//	the end of the data in the buffer
				ulong				noblock;																										//	whether or not the socket blocks
	public:																																				//	members available externally
				Network (int port, Bool nob, int bSize = 32);														//	constructor
				Network (SOCKET sock, Bool nob, int bSize = 32);												//	constructor
				~Network (void);																												//	destructor
				int					Read (void *data, long size, int num = 1);									//	read data from the connection
				int					Write (const void *data, long size);												//	write data to the connection
				void				Connect (cstr name, short port);														//	connect to the specified target
				void				Listen (void);																							//	listen for connections on this port
				Network			*Accept (void);																							//	accept any new connections
static	void				Open (void);																								//	start up the network subsystem
static	void				Close (void);																								//	close down the network subsystem
static	cstr				HostName (void);																						//	return a pointer to the host name
};																																							//	end

//------------------------------------------------------------------------------
inline	cstr	Network::HostName (void)																					//	return a pointer to the host name
{																																								//	begin
	return hostname;																															//	return a pointer to the hostname field
}																																								//	end

//------------------------------------------------------------------------------

#endif	//NETWORK
