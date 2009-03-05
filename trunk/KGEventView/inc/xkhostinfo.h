/****************************************************************************/
/* �ļ���:	xkdef.h															*/
/* ��	��: 	����Խ                                                      */
/* ��	��: 	1.0                                                         */
/* ��������:		2006.10.30                                              */
/* ����:	����ͷ�ļ�														*/
/* ��ʷ��Ϣ:                                                                */
/*		�޸�����:															*/
/*		�޸���:                                                             */
/*		�޸�����:															*/
/****************************************************************************/
/* 

  ��Ҫ������������µĻ�����ַ. 


  1. ������������ (����: "www.delta.com"), ��ѯ������IP��ַ

  2. ��������IP��ַ (����: 10.6.17.184), ��ѯ��������

  3. ʲôҲûָ������ôȡ��ǰ������ַ
     
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

    // Ĭ�Ϲ��캯��
    CXKHostInfo();

    // �����������ֻ�IP��ַ
    CXKHostInfo(string& hostName, hostType type);
 
    // �������ر������������ݿ�
    ~CXKHostInfo()
    {
		#ifdef UNIX
			endhostent();
		#endif
    }

	#ifdef UNIX

		// �����ݿ��еõ���һ����������
		char GetNextHost();

		// ���������ݿ�
		void OpenHostDb()
		{
			endhostent();
			m_cSearchHostDB = 1;
			sethostent(1);
		}

	#endif

    // ��������IP��ַ
    char* GetHostIPAddress() 
    {
		if( XK_NULL == m_pHost ) return XK_NULL;

        struct in_addr *addr_ptr;
		// �����������ݿ��е�һ����¼
        addr_ptr = (struct in_addr *)*m_pHost->h_addr_list;
		// �ı��ַΪ��׼����IP��ַ
        return inet_ntoa(*addr_ptr);
    }    
    
    // ������������
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
