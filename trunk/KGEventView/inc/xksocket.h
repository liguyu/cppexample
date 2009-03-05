/****************************************************************************/
/* File name:	xksocket.h  												*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	套接字类定义文件											*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#ifndef __XK_SOCKET_H
#define __XK_SOCKET_H

#include "xkdef.h"
#include "xkhostinfo.h"

#ifdef UNIX
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream.h>
    #include <sys/types.h>
    #include <stropts.h>
    #include <sys/filio.h>
#else
    #include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#endif

// so far we only consider the TCP socket, UDP will be added in later release
//const int MAX_RECV_LEN = 1048576;
const int MAX_RECV_LEN	= 8096;
const int MAX_MSG_LEN	= 1024;

class CXKSocket
{
protected:

	/*
	   only used when the socket is used for client communication
	   once this is done, the next necessary call is setSocketId(int)
	*/
	CXKSocket() {}
	void SetSocketId(int socketFd) { m_hSocket = socketFd; }

protected:

    int m_nPortNumber;        // Socket port number    
    int m_hSocket;          // Socket file descriptor

    int m_nBlocking;          // Blocking flag
    int m_nBindFlag;          // Binding flag

    struct sockaddr_in m_tAddrClient;    // Address of the client that sent data

public:

    CXKSocket(int);                       // given a port number, create a socket

    virtual ~CXKSocket()
    {
		#ifdef UNIX
            close(m_hSocket);
        #else
            closesocket(m_hSocket);
        #endif
     }
public:

	// socket options : ON/OFF

    Tbool SetDebug(int);
    Tbool SetReuseAddr(int);
    Tbool SetKeepAlive(int);
    Tbool SetLingerOnOff(bool);
	Tbool SetLingerSeconds(int);
    Tbool SetSocketBlocking(int);

    // size of the send and receive buffer

    Tbool SetSendBufSize(int);
    Tbool SetReceiveBufSize(int);

    // retrieve socket option settings

    int  GetDebug();
    int  GetReuseAddr();
    int  GetKeepAlive();
    int  GetSendBufSize();
    int  GetReceiveBufSize();
    int  GetSocketBlocking() { return m_nBlocking; }
	int  GetLingerSeconds();
    Tbool GetLingerOnOff();
	
    // returns the socket file descriptor
    int GetSocketId() { return m_hSocket; }

	// returns the port number
	int GetPortNumber() { return m_nPortNumber; }

	// show the socket 
	friend ostream& operator<<(ostream&,CXKSocket&);

private:

	// Gets the system error
	#ifdef WIN32
		void DetectErrorOpenWinSocket(int*,string&);
		void DetectErrorSetSocketOption(int*,string&);
		void DetectErrorGetSocketOption(int*,string&);
	#endif

    #ifdef UNIX
		char *sGetError()
		{
           return strerror(errno);
        }
	#endif
};

class CXKTcpSocket : public CXKSocket
{

private:

	#ifdef WIN32
		// Windows NT version of the MSG_WAITALL option
		int XPrecieveMessage(Tbyte *msgbuf, int buflen);
	#endif

public:

	/* 
	   Constructor. used for creating instances dedicated to client 
	   communication:

	   when accept() is successful, a m_hSocket is generated and returned
	   this socket id is then used to build a new socket using the following
	   constructor, therefore, the next necessary call should be setSocketId()
	   using this newly generated socket fd
	*/
	CXKTcpSocket() {};
	~CXKTcpSocket();

	// Constructor.  Used to create a new TCP socket given a port
	CXKTcpSocket(int portId) : CXKSocket(portId) { };

	/*
	   Sends a message to a connected host. The number of bytes sent is returned
	   can be either server call or client call
	*/
	int SendMessage(Tbyte *msgbuf, int msglen);

	/*
	   receive messages and stores the message in a buffer
	*/
	int RecieveMessage(Tbyte *msgbuf, int buflen);

	/*
	   Binds the socket to an address and port number
	   a server call
	*/
	Tbool BindSocket();

	/*
	   accepts a connecting client.  The address of the connected client 
	   is stored in the parameter
	   a server call
	*/
	CXKTcpSocket* AcceptClient(string&);

	// Listens to connecting clients, a server call
	Tbool ListenToClient(int numPorts = 5);

	// connect to the server, a client call
	virtual Tbool ConnectToServer(string&,hostType);

private:

	void DetectErrorBind(int*,string&);
	void DetectErrorSend(int*,string&);
	void DetectErrorRecv(int*,string&);
	void DetectErrorConnect(int*,string&);
    void DetectErrorAccept(int*,string&);
	void DetectErrorListen(int*,string&);

};

#endif	// __XK_SOCKET_H
        
