// LogInfo.cpp: implementation of the CLogInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "LogInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CLogInfo::CLogInfo()
//{
//
//}

CLogInfo::CLogInfo(CString strFilePath)
{
	m_strFilePath = strFilePath;
	m_uiOpenFlags = CFile::modeReadWrite|CFile::shareDenyNone;
	CFileFind clsFileFind;
	if( clsFileFind.FindFile(m_strFilePath) )
	{
		clsFileFind.Close();
		if( m_file.Open(m_strFilePath, m_uiOpenFlags) )
		{
			m_file.Seek(m_file.GetLength(), CFile::begin);
			m_file.WriteString("---------------------------------------------------------------------------\n");
			m_file.Close();
		}
	}
	else
	{
		if( m_file.Open(m_strFilePath, CFile::modeCreate|m_uiOpenFlags) )
		{
			m_file.WriteString("---------------------------------------------------------------------------\n");
			m_file.Close();
		}
	}
}

CLogInfo::~CLogInfo()
{
}

BOOL CLogInfo::WriteLogInfo(CString strLogInfo, int iLogType /* = LOGTYPE_NORMAL */, int iLogArg /* = LOGTARG_NORMAL */)
{
	if( m_file.Open(m_strFilePath, m_uiOpenFlags) )
	{
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		CString strInfo;
		strInfo.Format("%04d%02d%02d %02d:%02d:%02d %s\n", systemTime.wYear, systemTime.wMonth, 
			systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, strLogInfo);
		TRACE(strInfo);

		DWORD dwFileLen = m_file.GetLength();
		if( dwFileLen > LOGFILEMAXSIZE )
		{
			m_file.Close();
			CString strLogFile;
			strLogFile.Format("%s%04d%02d%02d %02d:%02d:%02d", m_strFilePath, systemTime.wYear, systemTime.wMonth, 
			systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
			CopyFile(m_strFilePath, strLogFile, FALSE);
			if( m_file.Open(m_strFilePath, CFile::modeCreate|m_uiOpenFlags) )
			{
				m_file.WriteString(strInfo);
				m_file.Close();
			}
		}
		else
		{
			m_file.Seek(dwFileLen, CFile::begin);
			m_file.WriteString(strInfo);
			m_file.Close();
		}

		return TRUE;
	}

	return FALSE;
}
