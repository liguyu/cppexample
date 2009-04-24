/****************************************************************************/
/* File name:	xksocket.cpp												*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	套接字类实现文件											*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#include "../StdAfx.h"
#include "xksocket.h"
#include "xkexception.h"

const int MSG_HEADER_LEN = 6;

//////////////////////////////////////////////////////////////////////
CXKSocket::CXKSocket(int pNumber)
{
    m_nPortNumber = pNumber;
    m_nBlocking = 1;
	
	if ( (m_hSocket=socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "";
		DetectErrorOpenWinSocket(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_CREATESOCKET_FAIL, errorCode, errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "%s\n" ,"unix: error getting host by name" );
#endif
	}
	
    /* 
	   set the initial address of client that shall be communicated with to 
	   any address as long as they are using the same port number. 
	   The m_tAddrClient structure is used in the future for storing the actual
	   address of client applications with which communication is going
	   to start
	*/
    m_tAddrClient.sin_family = AF_INET;
    m_tAddrClient.sin_addr.s_addr = htonl(INADDR_ANY);
    m_tAddrClient.sin_port = htons(m_nPortNumber);
}

Tbool CXKSocket::SetDebug(int debugToggle)
{
	
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_DEBUG,(char *)&debugToggle,sizeof(debugToggle)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "DEBUG option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n" , XK_ERR_SETSOCKETOPT_FAIL, errorCode, errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" ,XK_ERR_SETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif

		return XK_FALSE;
	}

	return XK_TRUE;
}

Tbool CXKSocket::SetReuseAddr(int reuseToggle)
{
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&reuseToggle,sizeof(reuseToggle)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "REUSEADDR option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n" , XK_ERR_SETSOCKETOPT_FAIL, errorCode, errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "%s\n" ,"unix: error getting host by name" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
} 

Tbool CXKSocket::SetKeepAlive(int aliveToggle)
{
	
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_KEEPALIVE,(char *)&aliveToggle,sizeof(aliveToggle)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "ALIVE option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x](%d) %s\n",XK_ERR_SETSOCKETOPT_FAIL, errorCode, errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
} 

Tbool CXKSocket::SetLingerSeconds(int seconds)
{
	struct linger lingerOption;
	
	if ( seconds > 0 )
	{
		lingerOption.l_linger = seconds;
		lingerOption.l_onoff = 1;
	}
	else lingerOption.l_onoff = 0;
	
	
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,sizeof(struct linger)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "LINGER option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SETSOCKETOPT_FAIL, errorCode, errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
}

Tbool CXKSocket::SetLingerOnOff(bool lingerOn)
{
	struct linger lingerOption;
	
	if ( lingerOn ) lingerOption.l_onoff = 1;
	else lingerOption.l_onoff = 0;
	
	
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,sizeof(struct linger)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "LINGER option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT,  "[0x%x]:%s\n", XK_ERR_SETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
}

Tbool CXKSocket::SetSendBufSize(int sendBufSize)
{
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char *)&sendBufSize,sizeof(sendBufSize)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "SENDBUFSIZE option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT,  "[0x%x]:%s\n", XK_ERR_SETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif

		return XK_FALSE;
	}

	return XK_TRUE;
} 

Tbool CXKSocket::SetReceiveBufSize(int receiveBufSize)
{
	if ( setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char *)&receiveBufSize,sizeof(receiveBufSize)) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "RCVBUF option:";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "%s\n" ,"unix: error getting host by name );
#endif

		return XK_FALSE;
	}

	return XK_TRUE;
	
}

Tbool CXKSocket::SetSocketBlocking(int blockingToggle)
{
    if (blockingToggle)
    {
        if (GetSocketBlocking()) return XK_TRUE;
        else m_nBlocking = 1;
	}
	else
	{
		if (!GetSocketBlocking()) return XK_TRUE;
		else m_nBlocking = 0;
	}
	
#ifdef WIN32
	if (ioctlsocket(m_hSocket,FIONBIO,(unsigned long *)m_nBlocking) == -1)
	{
		int errorCode;
		string errorMsg = "Blocking option: ";
		DetectErrorSetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SETSOCKETBOLCKING_FAIL, errorCode ,errorMsg.c_str() );

		return XK_FALSE;
	}
#endif
	
#ifdef UNIX
	if (ioctl(m_hSocket,FIONBIO,(char *)&m_nBlocking) == -1)
	{
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" ,XK_ERR_SETSOCKETBOLCKING_FAIL, "unix: error getting host by name" );

		return XK_FALSE;
	}
#endif

	return XK_TRUE;
}

int CXKSocket::GetDebug()
{
    int myOption;
    int myOptionLen = sizeof(myOption);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_DEBUG,(char *)&myOption,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get DEBUG option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" ,XK_ERR_GETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif

		return -1;
	}
    
    return myOption;
}

int CXKSocket::GetReuseAddr()
{
    int myOption;        
    int myOptionLen = sizeof(myOption);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&myOption,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get REUSEADDR option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_GETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif

		return -1;
	}
	
	return myOption;
}

int CXKSocket::GetKeepAlive()
{
    int myOption;        
    int myOptionLen = sizeof(myOption);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_KEEPALIVE,(char *)&myOption,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get KEEPALIVE option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT,  "[0x%x]:%s\n" ,XK_ERR_GETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif

		return -1;
	}
	
    return myOption;    
}

int CXKSocket::GetLingerSeconds()
{
	struct linger lingerOption;
	int myOptionLen = sizeof(struct linger);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get LINER option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "%s\n" ,"unix: error getting host by name" );
#endif
		return -1;
	}
	
	return lingerOption.l_linger;
}

Tbool CXKSocket::GetLingerOnOff()
{
	struct linger lingerOption;
	int myOptionLen = sizeof(struct linger);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get LINER option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" ,XK_ERR_GETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif
		return XK_FALSE;
	}
	
	if ( lingerOption.l_onoff == 1 ) return XK_TRUE;
	else return XK_FALSE;
}

int CXKSocket::GetSendBufSize()
{
    int sendBuf;
    int myOptionLen = sizeof(sendBuf);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char *)&sendBuf,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get SNDBUF option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" ,XK_ERR_GETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif
		return -1;
	}
	
    return sendBuf;
}    

int CXKSocket::GetReceiveBufSize()
{
    int rcvBuf;
    int myOptionLen = sizeof(rcvBuf);
	
	if ( getsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char *)&rcvBuf,&myOptionLen) == -1 )
	{
#ifdef WIN32
		int errorCode;
		string errorMsg = "get RCVBUF option: ";
		DetectErrorGetSocketOption(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_GETSOCKETOPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" ,XK_ERR_GETSOCKETOPT_FAIL, "unix: error getting host by name" );
#endif
		return -1;
	}
	
    return rcvBuf;
}

#ifdef WIN32
void CXKSocket::DetectErrorOpenWinSocket(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("Successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem or the associated service provider has failed.");
	else if ( *errCode == WSAEAFNOSUPPORT )
		errMsg.append("The specified address family is not supported.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("No more socket descriptors are available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available. The socket cannot be created.");
	else if ( *errCode == WSAEPROTONOSUPPORT )
		errMsg.append("The specified protocol is not supported.");
	else if ( *errCode == WSAEPROTOTYPE )
		errMsg.append("The specified protocol is the wrong type for this socket.");
	else if ( *errCode == WSAESOCKTNOSUPPORT )
		errMsg.append("The specified socket type is not supported in this address family.");
	else errMsg.append("unknown problems!");
}

void CXKSocket::DetectErrorSetSocketOption(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("optval is not in a valid part of the process address space or optlen parameter is too small.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEINVAL )
		errMsg.append("level is not valid, or the information in optval is not valid.");
	else if ( *errCode == WSAENETRESET )
		errMsg.append("Connection has timed out when SO_KEEPALIVE is set.");
	else if ( *errCode == WSAENOPROTOOPT )
		errMsg.append("The option is unknown or unsupported for the specified provider or socket (see SO_GROUP_PRIORITY limitations).");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("Connection has been reset when SO_KEEPALIVE is set.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else errMsg.append("unknown problem!");
}

void CXKSocket::DetectErrorGetSocketOption(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("One of the optval or the optlen parameters is not a valid part of the user address space, or the optlen parameter is too small.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The level parameter is unknown or invalid.");
	else if ( *errCode == WSAENOPROTOOPT )
		errMsg.append("The option is unknown or unsupported by the indicated protocol family.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	
	else errMsg.append("unknown problems!");
}

#endif

ostream& operator<<(ostream& io,CXKSocket& s)
{
	string flagStr = "";
	
	io << "--------------- Summary of socket settings -------------------" << endl;
	io << "   Socket Id:     " << s.GetSocketId() << endl;
	io << "   port #:        " << s.GetPortNumber() << endl;
	io << "   debug:         " << (flagStr = s.GetDebug()? "true":"false" ) << endl;
	io << "   reuse addr:    " << (flagStr = s.GetReuseAddr()? "true":"false" ) << endl;
	io << "   keep alive:    " << (flagStr = s.GetKeepAlive()? "true":"false" ) << endl;
	io << "   send buf size: " << s.GetSendBufSize() << endl;
	io << "   recv bug size: " << s.GetReceiveBufSize() << endl;
	io << "   m_nBlocking:      " << (flagStr = s.GetSocketBlocking()? "true":"false" ) << endl;
	io << "   linger on:     " << (flagStr = s.GetLingerOnOff()? "true":"false" ) << endl;
	io << "   linger seconds: " << s.GetLingerSeconds() << endl;
	io << "----------- End of Summary of socket settings ----------------" << endl;
	return io;
}

CXKTcpSocket::~CXKTcpSocket()
{
}

Tbool CXKTcpSocket::BindSocket()
{
   	if (bind(m_hSocket,(struct sockaddr *)&m_tAddrClient,sizeof(struct sockaddr_in))==-1)
	{
#ifdef WIN32
		int errorCode = 0;
		string errorMsg = "error calling bind(): \n";
		DetectErrorBind(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SOCKETBIND_FAIL, errorCode ,errorMsg.c_str() );
#endif
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SOCKETBIND_FAIL, "unix: error calling bind()" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
}

#ifdef WIN32

void CXKTcpSocket::DetectErrorBind(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("A process on the machine is already bound to the same\n");
		errMsg.append("fully-qualified address and the socket has not been marked\n"); 
		errMsg.append("to allow address re-use with SO_REUSEADDR. For example,\n");
		errMsg.append("IP address and port are bound in the af_inet case");
	}
	else if ( *errCode == WSAEADDRNOTAVAIL )
		errMsg.append("The specified address is not a valid address for this machine.");
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The name or the namelen parameter is not a valid part of\n");
		errMsg.append("the user address space, the namelen parameter is too small,\n");
		errMsg.append("the name parameter contains incorrect address format for the\n");
		errMsg.append("associated address family, or the first two bytes of the memory\n");
		errMsg.append("block specified by name does not match the address family\n");
		errMsg.append("associated with the socket descriptor s.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The socket is already bound to an address. ");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("Not enough buffers available, too many connections.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else errMsg.append("unknown problems!");
}

void CXKTcpSocket::DetectErrorRecv(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("The buf parameter is not completely contained in a valid part of the user address space.");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("The socket is not connected.");
	else if ( *errCode == WSAEINTR )
		errMsg.append("The (m_nBlocking) call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAENETRESET )
	{
		errMsg.append("The connection has been broken due to the keep-alive activity\n");
		errMsg.append("detecting a failure while the operation was in progress.");
	}
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
	{
		errMsg.append("MSG_OOB was specified, but the socket is not stream-style\n");
		errMsg.append("such as type SOCK_STREAM, out-of-band data is not supported\n");
		errMsg.append("in the communication domain associated with this socket, or\n");
		errMsg.append("the socket is unidirectional and supports only send operations.");
	}
	else if ( *errCode == WSAESHUTDOWN )
	{
		errMsg.append("The socket has been shut down; it is not possible to recv on a\n");
		errMsg.append("socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.");
	}
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and the receive operation would block.");
	else if ( *errCode == WSAEMSGSIZE )
		errMsg.append("The message was too large to fit into the specified buffer and was truncated.");
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The socket has not been bound with bind, or an unknown flag\n");
		errMsg.append("was specified, or MSG_OOB was specified for a socket with\n");
		errMsg.append("SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.");
	}
	else if ( *errCode == WSAECONNABORTED )
	{
		errMsg.append("The virtual circuit was terminated due to a time-out or\n");
		errMsg.append("other failure. The application should close the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAETIMEDOUT )
	{
		errMsg.append("The connection has been dropped because of a network\n");
		errMsg.append("failure or because the peer system failed to respond.");
	}
	else if ( *errCode == WSAECONNRESET )
	{
		errMsg.append("The virtual circuit was reset by the remote side executing a\n");
		errMsg.append("\"hard\" or \"abortive\" close. The application should close\n");
		errMsg.append("the socket as it is no longer usable. On a UDP datagram socket\n");
		errMsg.append("this error would indicate that a previous send operation\n");
		errMsg.append("resulted in an ICMP \"Port Unreachable\" message.");
	}
	else errMsg.append("unknown problems!");
}

void CXKTcpSocket::DetectErrorConnect(int* errCode,string& errMsg)
{

	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED ){
		errMsg.append("A successful WSAStartup must occur before using this function.");
		TRACE0("A successful WSAStartup must occur before using this function.");
	}
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("The socket's local address is already in use and the socket\n");
		errMsg.append("was not marked to allow address reuse with SO_REUSEADDR. This\n");
		errMsg.append("error usually occurs when executing bind, but could be delayed\n");
		errMsg.append("until this function if the bind was to a partially wild-card\n");
		errMsg.append("address (involving ADDR_ANY) and if a specific address needs\n");
		errMsg.append("to be committed at the time of this function.");
	}
	else if ( *errCode == WSAEINTR )
		errMsg.append("The (m_nBlocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or\n");
		errMsg.append("the service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEALREADY )
	{
		errMsg.append("A nonblocking connect call is in progress on the specified socket.\n");
		errMsg.append("Note In order to preserve backward compatibility, this error is\n");
		errMsg.append("reported as WSAEINVAL to Windows Sockets 1.1 applications that\n");
		errMsg.append("link to either WINSOCK.DLL or WSOCK32.DLL.");
	}
	else if ( *errCode == WSAEADDRNOTAVAIL )
		errMsg.append("The remote address is not a valid address (such as ADDR_ANY).");
	else if ( *errCode == WSAEAFNOSUPPORT )
		errMsg.append("Addresses in the specified family cannot be used with this socket.");
	else if ( *errCode == WSAECONNREFUSED )
		errMsg.append("The attempt to connect was forcefully rejected.");
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The name or the namelen parameter is not a valid part of\n");
		errMsg.append("the user address space, the namelen parameter is too small,\n");
		errMsg.append("or the name parameter contains incorrect address format for\n");
		errMsg.append("the associated address family.");
	}
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The parameter s is a listening socket, or the destination\n");
		errMsg.append("address specified is not consistent with that of the constrained\n");
		errMsg.append("group the socket belongs to.");
	}
	else if ( *errCode == WSAEISCONN )
		errMsg.append("The socket is already connected (connection-oriented sockets only).");
	else if ( *errCode == WSAENETUNREACH )
		errMsg.append("The network cannot be reached from this host at this time.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available. The socket cannot be connected.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAETIMEDOUT )
		errMsg.append("Attempt to connect timed out without establishing a connection.");
	else if ( *errCode == WSAEWOULDBLOCK )
	{
		errMsg.append("The socket is marked as nonblocking and the connection\n");
		errMsg.append("cannot be completed immediately.");
	}
	else if ( *errCode == WSAEACCES )
	{
		errMsg.append("Attempt to connect datagram socket to broadcast address failed\n");
		errMsg.append("because setsockopt option SO_BROADCAST is not enabled.");
	}
	else errMsg.append("unknown problems!");	
	
	
}

void CXKTcpSocket::DetectErrorAccept(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this FUNCTION.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("The addrlen parameter is too small or addr is not a valid part of the user address space.");
	else if ( *errCode == WSAEINTR )
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The listen function was not invoked prior to accept.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("The queue is nonempty upon entry to accept and there are no descriptors available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
		errMsg.append("The referenced socket is not a type that supports connection-oriented service.");
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and no connections are present to be accepted.");
	else errMsg.append("unknown problems!");
}

void CXKTcpSocket::DetectErrorListen(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("The socket's local address is already in use and the socket was\n");
		errMsg.append("not marked to allow address reuse with SO_REUSEADDR. This error\n");
		errMsg.append("usually occurs during execution of the bind function, but could\n");
		errMsg.append("be delayed until this function if the bind was to a partially\n");
		errMsg.append("wild-card address (involving ADDR_ANY) and if a specific address\n");
		errMsg.append("needs to be \"committed\" at the time of this function.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress, or the service\n");
		errMsg.append("provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The socket has not been bound with bind.");
	else if ( *errCode == WSAEISCONN )
		errMsg.append("The socket is already connected.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("No more socket descriptors are available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
		errMsg.append("The referenced socket is not of a type that supports the listen operation.");
	else errMsg.append("unknown problems!");
}

void CXKTcpSocket::DetectErrorSend(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();
	
	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEACCES )
	{
		errMsg.append("The requested address is a broadcast address,\n");
		errMsg.append("but the appropriate flag was not set. Call setsockopt\n");
		errMsg.append("with the SO_BROADCAST parameter to allow the use of the broadcast address.");
	}
	else if ( *errCode == WSAEINTR )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call was canceled\n");
		errMsg.append("through WSACancelBlockingCall.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A m_nBlocking Windows Sockets 1.1 call is in progress,\n");
		errMsg.append("or the service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The buf parameter is not completely contained in a\n");
		errMsg.append("valid part of the user address space.");
	}
	else if ( *errCode == WSAENETRESET )
	{
		errMsg.append("The connection has been broken due to the keep-alive\n");
		errMsg.append("activity detecting a failure while the operation was in progress.");
	}
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("The socket is not connected.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
	{
		errMsg.append("MSG_OOB was specified, but the socket is not stream-style\n");
		errMsg.append("such as type SOCK_STREAM, out-of-band data is not supported\n");
		errMsg.append("in the communication domain associated with this socket,\n");
		errMsg.append("or the socket is unidirectional and supports only receive operations.");
	}
	else if ( *errCode == WSAESHUTDOWN )
	{
		errMsg.append("The socket has been shut down; it is not possible to send\n");
		errMsg.append("on a socket after shutdown has been invoked with how set\n");
		errMsg.append("to SD_SEND or SD_BOTH.");
	}
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and the requested operation would block.\n");
	else if ( *errCode == WSAEMSGSIZE )
	{
		errMsg.append("The socket is message oriented, and the message is larger\n");
		errMsg.append("than the maximum supported by the underlying transport.");
	}
	else if ( *errCode == WSAEHOSTUNREACH )
		errMsg.append("The remote host cannot be reached from this host at this time.");
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The socket has not been bound with bind, or an unknown flag\n");
		errMsg.append("was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.");
	}
	else if ( *errCode == WSAECONNABORTED )
	{
		errMsg.append("The virtual circuit was terminated due to a time-out or \n");
		errMsg.append("other failure. The application should close the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAECONNRESET )
	{
		errMsg.append("The virtual circuit was reset by the remote side executing a \"hard\" \n");
		errMsg.append("or \"abortive\" close. For UPD sockets, the remote host was unable to\n");
		errMsg.append("deliver a previously sent UDP datagram and responded with a\n");
		errMsg.append("\"Port Unreachable\" ICMP packet. The application should close\n");
		errMsg.append("the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAETIMEDOUT )
	{
		errMsg.append("The connection has been dropped, because of a network failure\n");
		errMsg.append("or because the system on the other end went down without notice.");
	}
	else errMsg.append("unknown problems!");
}

#endif

Tbool CXKTcpSocket::ConnectToServer(string& serverNameOrAddr,hostType hType)
{ 
/* 
when this method is called, a client socket has been built already,
so we have the m_hSocket and m_nPortNumber ready.

a CXKHostInfo instance is created, no matter how the server's name is 
  given (such as www.yuchen.net) or the server's address is given (such
  as 169.56.32.35), we can use this CXKHostInfo instance to get the 
  IP address of the server
*/
	
	CXKHostInfo serverInfo(serverNameOrAddr,hType);
	
    // Store the IP address and socket port number	
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverInfo.GetHostIPAddress());
    serverAddress.sin_port = htons(m_nPortNumber);
	
    // Connect to the given address
	if (connect(m_hSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1)
	{
#ifdef WIN32
		int errorCode = 0;
		string errorMsg = "error calling connect():\n";
		TRACE0("Error calling connect():\n");
		DetectErrorConnect(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SOCKETCONNECT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n" , XK_ERR_SOCKETCONNECT_FAIL, "unix: error calling connect()" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
}

CXKTcpSocket* CXKTcpSocket::AcceptClient(string& clientHost)
{
	int newSocket;   // the new socket file descriptor returned by the accept systme call
	
    // the length of the client's address
    int clientAddressLen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddress;    // Address of the client that sent data
	
    // Accepts a new client connection and stores its socket file descriptor
	
	if ((newSocket = accept(m_hSocket, (struct sockaddr *)&clientAddress,&clientAddressLen)) == -1)
	{
#ifdef WIN32
		int errorCode = 0;
		string errorMsg = "error calling accept(): \n";
		DetectErrorAccept(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SOCKETACCEPT_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SOCKETACCEPT_FAIL, "unix: error calling accept()" );
#endif
		return XK_NULL;
	}
	
    
	// Get the host name given the address
    char *sAddress = inet_ntoa((struct in_addr)clientAddress.sin_addr);
	CXKHostInfo clientInfo(string(sAddress),ADDRESS);
	char* hostName = clientInfo.GetHostName();
    clientHost += string(hostName);
	
    // Create and return the new CXKTcpSocket object
    CXKTcpSocket* retSocket = new CXKTcpSocket();
	retSocket->SetSocketId(newSocket);
    return retSocket;
}

Tbool CXKTcpSocket::ListenToClient(int totalNumPorts)
{
	if (listen(m_hSocket,totalNumPorts) == -1)
	{
#ifdef WIN32
		int errorCode = 0;
		string errorMsg = "error calling listen():\n";
		DetectErrorListen(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SOCKETLISTEN_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SOCKETLISTEN_FAIL, "unix: error calling listen()" );
#endif
		return XK_FALSE;
	}

	return XK_TRUE;
}       

int CXKTcpSocket::SendMessage(Tbyte *msgbuf, int msglen)
{
	if( msglen> (MAX_RECV_LEN /*- MSG_HEADER_LEN*/) || msgbuf == XK_NULL )
		return -1;

	int numBytes;  // the number of bytes sent
	
				   /* 
				   for each message to be sent, add a header which shows how long this message
				   is. This header, regardless how long the real message is, will always be
				   of the length MSG_HEADER_LEN.
	*/
	char charMsg[MAX_RECV_LEN+1];
	char msgLength[MSG_HEADER_LEN+1];
    sprintf(msgLength,"%6d",msglen);
	memcpy(charMsg, msgLength, MSG_HEADER_LEN);
	memcpy(charMsg + MSG_HEADER_LEN, msgbuf, msglen);
	
	// Sends the message to the connected host
	numBytes = send(m_hSocket, charMsg, msglen + MSG_HEADER_LEN, 0);
	if (numBytes == -1)
	{
#ifdef WIN32
		int errorCode = 0;
		string errorMsg = "error calling send():\n";
		DetectErrorSend(&errorCode,errorMsg);
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SOCKETSEND_FAIL, errorCode ,errorMsg.c_str() );
#endif
		
#ifdef UNIX
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SOCKETSEND_FAIL, "unix: error calling send()" );
#endif
	}
	
	return numBytes;
}

#ifdef WIN32

int CXKTcpSocket::XPrecieveMessage(Tbyte *msgbuf, int buflen)
{
	if( buflen> MAX_RECV_LEN ) return 0;

	int received = 0;                 // The number of bytes received
    int msgSize = buflen;	          // The number of bytes wanted to receive
    int numBytes = 0;                 // The number of bytes currently recieved
	int totalRecvNum = 0;
	bool headerFinished = false;
	
	char charMsg[MAX_RECV_LEN+1];
	char msgLength[MSG_HEADER_LEN+1];
	memset(msgLength,0,sizeof(msgLength));
	
	// 先收消息长度再收消息体
	msgSize	= MSG_HEADER_LEN;
	while ( received < msgSize )
	{
		msgSize		-= received;
		numBytes = recv(m_hSocket,charMsg+received,msgSize,0);
		if( numBytes> 0 )
		{
			received	+= numBytes;
		}
		else if (numBytes == -1)
		{
			int errorCode = 0;
			string errorMsg = "error calling recv():\n";
			DetectErrorRecv(&errorCode,errorMsg);
			xkPrint(XK_LEVEL_PRINT, "[0x%x]:(%d) %s\n", XK_ERR_SOCKETRECV_FAIL, errorCode ,errorMsg.c_str() );

			return numBytes;
	//		if( errCode == WSAECONNRESET )
	//		{
	//			xkPrint(XK_LEVEL_PRINT, "!! your party has shut down the connection... \n");
	//		}
		}
		else
		{
			return 0;
		}
		
		if ( !headerFinished )
		{
			if ( received == MSG_HEADER_LEN )
			{
				memcpy(msgLength, charMsg, received);
				headerFinished = true;
				received = 0;
				memset(charMsg,0,sizeof(charMsg));
				msgSize = atoi(msgLength);
			}
		}
		
	}
	memcpy(msgbuf, charMsg, received);

    return msgSize;
}

#endif

int CXKTcpSocket::RecieveMessage(Tbyte *msgbuf, int buflen)
{
	int numBytes;  // The number of bytes recieved
	
#ifdef WIN32
	return XPrecieveMessage(msgbuf, buflen);
#endif
	
	// retrieve the length of the message received
	
	char msgLength[MSG_HEADER_LEN+1];
	memset(msgLength,0,sizeof(msgLength));
	
	numBytes = recv(m_hSocket,msgLength,MSG_HEADER_LEN,0);
	if (numBytes == -1)
	{
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SOCKETRECV_FAIL, "unix: error calling recv()" );
	}
	
	// receive the real message
	numBytes = recv(m_hSocket ,(char *)msgbuf ,atoi(msgLength),0);
	if (numBytes == -1)
	{
		xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_SOCKETRECV_FAIL, "unix: error calling recv()" );
	}
	
    return numBytes;
}


