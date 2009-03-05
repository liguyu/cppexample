/*********************************************************************

   Copyright (C) 1999 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

**********************************************************************/

#include "stdafx.h"
#include "SAStatusLog.h"

/////////////////////////////////////////////////////////////////////////////

CSAStatusLog::CSAStatusLog()
{
	m_bEnable = FALSE;
    m_bPrintTime = TRUE;
	m_csFileName = "";
	m_fMapOpen=FALSE;
    m_bPrintAppName=FALSE;
    m_csAppName = "";

   // we'll make sure only one call uses the critical stuff at a time
   InitializeCriticalSection(&m_crit);
}

/////////////////////////////////////////////////////////////////////////////

CSAStatusLog::~CSAStatusLog()
{
   MapClose();
   DeleteCriticalSection(&m_crit);
}

/////////////////////////////////////////////////////////////////////////////

void CSAStatusLog::Init(const char *pOutputFilename)
{
	m_bEnable = TRUE;

   // get application path and name
	char buf[MAX_PATH+1];
	DWORD res = GetModuleFileName(AfxGetInstanceHandle(), buf, MAX_PATH);

	CString appDir = GetBaseDir(buf);

   m_csAppName = GetBaseName(buf);

   m_csFileName.Format("%s\\%s", appDir, pOutputFilename);

   // overwrite the old one!
   remove(m_csFileName);
}
void CSAStatusLog::InitPath(const char *pOutputFilename)
{
	m_bEnable = TRUE;

    m_csFileName.Format("%s",  pOutputFilename);

   // overwrite the old one!
}
/////////////////////////////////////////////////////////////////////////////

void CSAStatusLog::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CSAStatusLog::StatusOut(const char* fmt, ...)
{
	if (m_csFileName.IsEmpty())
		return FALSE;

	if (!m_bEnable)
		return TRUE;

	if (!AfxIsValidString(fmt, -1))
		return FALSE;

   EnterCriticalSection(&m_crit);

   // parse that string format
   try
   {
	   va_list argptr;
	   va_start(argptr, fmt);
	   _vsnprintf(m_tBuf, TBUF_SIZE, fmt, argptr);
	   va_end(argptr);
   }
   catch (...)
   {
      m_tBuf[0] = 0;
   }

   BOOL bOK = FALSE;

   // output 
	FILE *fp = fopen(m_csFileName, "a");
	if (fp)
	{
      if (m_bPrintAppName)
      {
         fprintf(fp,"%s : ", m_csAppName);
      }

      if (m_bPrintTime)
      {
		   CTime ct ; 
		   ct = CTime::GetCurrentTime();
		   fprintf(fp,"%s  :",ct.Format("%m/%d/%Y %H:%M:%S"));
      }

      fprintf(fp, "%s\n", m_tBuf);
		
      fclose(fp);

      bOK = TRUE;
	}

   LeaveCriticalSection(&m_crit);

	return bOK;
}

/////////////////////////////////////////////////////////////////////////////

CString	CSAStatusLog::GetBaseName(const CString &path)
{
	CString out = path;

	int iSlashPos = path.ReverseFind('\\');
	if (iSlashPos !=-1) 
	{
		out = out.Mid(iSlashPos+1);
	}
	else
	{
		iSlashPos = path.ReverseFind('/');
		if (iSlashPos !=-1) 
		{
			out = out.Mid(iSlashPos+1);
		}
	}

   int iDotPos = out.ReverseFind('.');
   if (iDotPos>0)
   {
      out = out.Left(iDotPos);
   }

	return out;
}

/////////////////////////////////////////////////////////////////////////////

CString CSAStatusLog::GetBaseDir(const CString & path)
{
	CString out = "";
	int iSlashPos = path.ReverseFind('\\');
	if (iSlashPos !=-1) 
	{
		out = path.Left(iSlashPos);
	} 
	else
	{
		iSlashPos = path.ReverseFind('/');
		if (iSlashPos !=-1) 
		{
			out = path.Left(iSlashPos);
		} 
	}

	return out;
}
int CSAStatusLog::MapOpen(int nType)
{
	if( m_csFileName.IsEmpty() ) 
		return -1; 
	
	m_nType	= nType;
	CString strPath	= m_csFileName;
	//	if( !FolderExist( strPath ) )

    // 创建文件对象

    HANDLE hFile = (HANDLE)CreateFile(strPath, GENERIC_READ,\
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        TRACE1("创建文件对象失败,错误代码:%d\n ", GetLastError());
        return -1;
    }
	// 得到文件尺寸
    DWORD dwFileSizeHigh;
    m_qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
    m_qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
    m_qwFileSize+=sizeof(char);
	if( m_qwFileSize >= __MAX_LOGFILE_SIZE__)
	{
		CloseHandle(hFile);
		return -1;
	}
    // 创建文件映射对象
	if( nType == OT_WRITE )
		m_hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, __MAX_LOGFILE_SIZE__, NULL);
	else
		m_hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (m_hFileMap == NULL)
    {
		// 关闭文件对象
		CloseHandle(hFile);
        TRACE1("%s",GetLastError);
		TRACE1("创建文件映射对象失败,错误代码:%d ", GetLastError());
        return -1;
    }
    // 得到系统分配粒度
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    DWORD dwGran = SysInfo.dwAllocationGranularity;
		  //   当数据写满60%时，为防数据溢出，需要在其后开辟一新的映射视图   

	// 得到文件尺寸
    __int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
    qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
    // 关闭文件对象
    CloseHandle(hFile);
    // 块大小
    DWORD dwBlockBytes = 1000 * dwGran;
    if (qwFileSize < 1000 * dwGran)
        dwBlockBytes = (DWORD)qwFileSize;
	
    // 映射视图
    m_lpbMapAddress = (TCHAR *)MapViewOfFile(m_hFileMap,FILE_MAP_READ, 0, 0, dwBlockBytes);
	if (m_lpbMapAddress == NULL)
    {
		AfxMessageBox("映射文件映射失败,错误代码:%d ", GetLastError());
		return -1;
	}
	
	m_fMapOpen		= TRUE;
	
	return 0;
}

void CSAStatusLog::MapClose()
{
	if( !m_fMapOpen )	return;
     
	UnmapViewOfFile(m_lpbMapAddress);
    CloseHandle(m_hFileMap);
  
	m_fMapOpen		= FALSE;
}
BOOL CSAStatusLog::FolderExist(CString strPath)
{
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
		rValue = TRUE;   
	}
	FindClose(hFind);
    return rValue;
}
int CSAStatusLog::ReadFirstLine(char* strBuf, int nBufSize)
{
	if( strBuf == NULL || nBufSize <=0 || m_lpbMapAddress == NULL )
	{
		return 0;
	}
	
	int nReadCnt	= 0;
	TCHAR	*p1 = NULL;
	TCHAR	*p2 = m_lpbMapAddress;
	if((p1 = strstr(p2,"\r\n")) != NULL)
	{   
		nReadCnt	= p1 - p2 + 1;
		strncpy( strBuf, p2, nReadCnt );
		strBuf[nReadCnt]	= '\0';
		
		m_qwFileOffset	= nReadCnt + 1;
	}   
	
	return nReadCnt; 
}

int CSAStatusLog::ReadNextLine(char* strBuf, int nBufSize)
{
	if( strBuf == NULL || nBufSize <=0 || m_lpbMapAddress == NULL )
	{
		return 0;
	}
	
	int nReadCnt	= 0;
	TCHAR	*p1 = NULL;
	TCHAR	*p2 = m_lpbMapAddress + m_qwFileOffset;
	if((p1 = strstr(p2,"\r\n")) != NULL)
	{   
		nReadCnt	= p1 - p2 + 1;
		strncpy( strBuf, p2, nReadCnt );
		strBuf[nReadCnt]	= '\0';
		
		m_qwFileOffset	+= nReadCnt + 1;
	}   
	
	return nReadCnt; 
}