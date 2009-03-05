#include "xkhostinfo.h"
#include "xkexception.h"
CXKHostInfo::CXKHostInfo()
{

#ifdef UNIX
	OpenHostDb();
	// winLog<<"UNIX version CXKHostInfo() is called...\n";
#endif

#ifdef WIN32
	
	char sName[HOST_NAME_LENGTH+1];
	memset(sName,0,sizeof(sName));
	gethostname(sName,HOST_NAME_LENGTH);

	m_pHost = gethostbyname(sName);
	if (m_pHost == XK_NULL)	DetectErrorGethostbyname();
#endif

}

CXKHostInfo::CXKHostInfo(string& hostName,hostType type)
{
#ifdef UNIX
	m_cSearchHostDB = 0;
#endif
	
	if (type == NAME)
	{
		// 返回主机名字
		m_pHost = gethostbyname(hostName.c_str());
		
		if (m_pHost == XK_NULL)
		{
#ifdef WIN32
				DetectErrorGethostbyname();
#endif
				
#ifdef UNIX
				xkPrint(XK_LEVEL_PRINT, %s\n" ,"unix: error getting host by name" );
#endif
		}
	}
	else if (type == ADDRESS)
	{
		// 返回主机地址
		unsigned long netAddr = inet_addr(hostName.c_str());
		if (netAddr == -1)
		{
			xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n", XK_ERR_GETHOSTADDR_FAIL, "Error calling inet_addr()" );
		}
		
		m_pHost = gethostbyaddr((char *)&netAddr, sizeof(netAddr), AF_INET);
		if (m_pHost == XK_NULL)
		{
#ifdef WIN32
			DetectErrorGethostbyaddr( );
#endif
			
#ifdef UNIX
			xkPrint(XK_LEVEL_PRINT, "[0x%x]:%s\n",  XK_ERR_GETHOSTADDR_FAIL, "unix: error getting host by name" );
#endif
		}
	}
	else
	{
		xkPrint(XK_LEVEL_PRINT, "%s\n", "unknown host type: host name/address has to be given " );
	}
}

#ifdef WIN32
void CXKHostInfo::DetectErrorGethostbyname()
{
	int	errCode = WSAGetLastError();
	string errorMsg;
	
	if ( errCode == WSANOTINITIALISED )
		errorMsg.append("need to call WSAStartup to initialize socket system on Window system.");
	else if ( errCode == WSAENETDOWN )
		errorMsg.append("The network subsystem has failed.");
	else if ( errCode == WSAHOST_NOT_FOUND )
		errorMsg.append("Authoritative Answer Host not found.");
	else if ( errCode == WSATRY_AGAIN )
		errorMsg.append("Non-Authoritative Host not found, or server failure.");
	else if ( errCode == WSANO_RECOVERY )
		errorMsg.append("Nonrecoverable error occurred.");
	else if ( errCode == WSANO_DATA )
		errorMsg.append("Valid name, no data record of requested type.");
	else if ( errCode == WSAEINPROGRESS )
		errorMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( errCode == WSAEFAULT )
		errorMsg.append("The name parameter is not a valid part of the user address space.");
	else if ( errCode == WSAEINTR )
		errorMsg.append("A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");

	xkPrint(XK_LEVEL_PRINT, "%s\n", XK_ERR_GETHOSTNAME_FAIL, errorMsg.c_str() );
}
#endif

#ifdef WIN32
void CXKHostInfo::DetectErrorGethostbyaddr( )
{
	int	errCode = WSAGetLastError();
	string errorMsg;

	if ( errCode == WSANOTINITIALISED )
		errorMsg.append("A successful WSAStartup must occur before using this function.");
	if ( errCode == WSAENETDOWN )
		errorMsg.append("The network subsystem has failed.");
	if ( errCode == WSAHOST_NOT_FOUND )
		errorMsg.append("Authoritative Answer Host not found.");
	if ( errCode == WSATRY_AGAIN )
		errorMsg.append("Non-Authoritative Host not found, or server failed."); 
	if ( errCode == WSANO_RECOVERY )
		errorMsg.append("Nonrecoverable error occurred.");
	if ( errCode == WSANO_DATA )
		errorMsg.append("Valid name, no data record of requested type.");
	if ( errCode == WSAEINPROGRESS )
		errorMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	if ( errCode == WSAEAFNOSUPPORT )
		errorMsg.append("The type specified is not supported by the Windows Sockets implementation.");
	if ( errCode == WSAEFAULT )
		errorMsg.append("The addr parameter is not a valid part of the user address space, or the len parameter is too small.");
	if ( errCode == WSAEINTR )
		errorMsg.append("A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");

	xkPrint(XK_LEVEL_PRINT, "%s\n", XK_ERR_GETHOSTADDR_FAIL, errorMsg.c_str() );
}
#endif

#ifdef UNIX
char CXKHostInfo::GetNextHost()
{
	// winLog<<"UNIX GetNextHost() is called...\n";
    // 从数据库中得到下一主机地址
    if (m_cSearchHostDB == 1)
    {
         if ((m_pHost = gethostent()) == XK_NULL)
              return 0;
         else
              return 1;
    }
    return 0;
}
#endif