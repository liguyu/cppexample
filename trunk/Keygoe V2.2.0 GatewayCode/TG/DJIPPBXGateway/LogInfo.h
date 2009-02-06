// LogInfo.h: interface for the CLogInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGINFO_H__2A3E25C6_A2C5_41E9_BBED_B311ED50CECC__INCLUDED_)
#define AFX_LOGINFO_H__2A3E25C6_A2C5_41E9_BBED_B311ED50CECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int LOGFILEMAXSIZE = 1024*1024*2;

enum LOGTYPE
{
	LOGTYPE_NORMAL,
		LOGTYPE_ALTER,
		LOGTYPE_ERROR
};

enum LOGARG
{
	LOGTARG_NORMAL,
		LOGARG_HIGHLEVEL,
		LOGARG_ULTIMATE
};

class CLogInfo  
{
public:
//	CLogInfo();
	CLogInfo(CString strFilePath);
	virtual ~CLogInfo();

	BOOL WriteLogInfo(CString strLogInfo, int iLogType = LOGTYPE_NORMAL, int iLogArg = LOGTARG_NORMAL);

protected:
	CStdioFile m_file;
	CString m_strFilePath;
	UINT m_uiOpenFlags;
};

#endif // !defined(AFX_LOGINFO_H__2A3E25C6_A2C5_41E9_BBED_B311ED50CECC__INCLUDED_)
