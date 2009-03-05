/****************************************************************************/
/* 文件名:	xkdef.h															*/
/* 作	者: 	陈文越                                                      */
/* 版	本: 	1.0                                                         */
/* 创建日期:		2006.10.30                                              */
/* 描述:	定义头文件														*/
/* 历史信息:                                                                */
/*		修改日期:															*/
/*		修改人:                                                             */
/*		修改内容:															*/
/****************************************************************************/
/* 

  主要处理三种情况下的机器地址. 


  1. 给定主机域名 (例如: "www.delta.com"), 查询主机的IP地址

  2. 给定主机IP地址 (例如: 10.6.17.184), 查询主机域名

  3. 什么也没指定，那么取当前主机地址
     
*/

#ifndef __XK_HOSTINFO_H
#define __XK_HOSTINFO_H

#include "xkdef.h"

#ifdef UNIX
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#else
    #include <winsock2.h>
#endif
    
enum hostType {NAME, ADDRESS};
const int HOST_NAME_LENGTH = 64;

class CXKHostInfo
{

private:

	#ifdef UNIX
		char m_cSearchHostDB;     // search the host database flag
	#endif

	struct hostent *m_pHost;    // Entry within the host address database

public:

    // 默认构造函数
    CXKHostInfo();

    // 给定主机名字或IP地址
    CXKHostInfo(string& hostName, hostType type);
 
    // 析构，关闭主机域名数据库
    ~CXKHostInfo()
    {
		#ifdef UNIX
			endhostent();
		#endif
    }

	#ifdef UNIX

		// 在数据库中得到下一个主机名字
		char GetNextHost();

		// 打开域名数据库
		void OpenHostDb()
		{
			endhostent();
			m_cSearchHostDB = 1;
			sethostent(1);
		}

	#endif

    // 返回主机IP地址
    char* GetHostIPAddress() 
    {
		if( XK_NULL == m_pHost ) return XK_NULL;

        struct in_addr *addr_ptr;
		// 主机域名数据库中第一个记录
        addr_ptr = (struct in_addr *)*m_pHost->h_addr_list;
		// 改变地址为标准网络IP地址
        return inet_ntoa(*addr_ptr);
    }    
    
    // 返回主机名字
    char* GetHostName()
    {
        return ((XK_NULL != m_pHost )?m_pHost->h_name:XK_NULL);
    }

private:

	#ifdef WIN32
		void DetectErrorGethostbyname();
		void DetectErrorGethostbyaddr();
	#endif
};

#endif	// __XK_HOSTINFO_H
