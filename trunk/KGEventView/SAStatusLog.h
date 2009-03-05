/*********************************************************************

   Copyright (C) 1999 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied

**********************************************************************/

#ifndef CSAStatusLog_Header_Thingy
#define CSAStatusLog_Header_Thingy

#define  __MAX_LOGFILE_SIZE__				0x4000000			// File size 64M
/////////////////////////////////////////////////////////////////////////////

class CSAStatusLog
{
public:
   /////////////////////////////////////////////////////////////////////////////
   	enum{OT_READ, OT_WRITE};
	CSAStatusLog();
   ~CSAStatusLog();
	BOOL		m_fMapOpen;				// �򿪱�־
	HANDLE      hFile;
	HANDLE		m_hFileMap;				// ӳ���ļ����
	BOOL		m_bEnable;
   /////////////////////////////////////////////////////////////////////////////
   // call this first!
   void		Init(const char *pOutputFilename);
   void     InitPath(const char *pOutputFilename);
   /////////////////////////////////////////////////////////////////////////////
   // output text, just like TRACE or printf
   BOOL		StatusOut(const char* fmt, ...);

   /////////////////////////////////////////////////////////////////////////////
   // turn it on or off
   void		Enable(BOOL bEnable);

   /////////////////////////////////////////////////////////////////////////////
   // timestamp each line?
   void     PrintTime(BOOL b) {m_bPrintTime = b;}

   /////////////////////////////////////////////////////////////////////////////
   // print the application name?
   void     PrintAppName(BOOL b) {m_bPrintAppName = b;}

   /////////////////////////////////////////////////////////////////////////////
   // override the default app name, which is the name the EXE (minus the ".exe")
   void     SetAppName(const char *pName) {m_csAppName = pName;}

   CString  GetBaseDir(const CString & path);

   int      MapOpen(int nType);
   void     MapClose();
   BOOL     FolderExist(CString strPath);
   int      ReadNextLine(char* strBuf, int nBufSize);
   int      ReadFirstLine(char* strBuf, int nBufSize);
protected:
   /////////////////////////////////////////////////////////////////////////////
   // work buffer
   enum {TBUF_SIZE = 3000};
   char		m_tBuf[TBUF_SIZE];

   CString     m_csAppName;
   CString	   m_csFileName;

   /////////////////////////////////////////////////////////////////////////////
   // protection
   CRITICAL_SECTION  m_crit;

   /////////////////////////////////////////////////////////////////////////////
   // controlling stuff
   BOOL     m_bPrintTime;
   BOOL     m_bPrintAppName;

   int			m_nType;				// �ļ���ģʽ
   TCHAR		*m_lpbMapAddress;		// ӳ���ļ�ָ��
   LONG 		m_qwFileOffset;			// ƫ�Ƶ�ַ 
   LONG		    m_qwFileSize;			// �򿪵��ļ���С
   BOOL		    m_bWrited;				// д��־

   /////////////////////////////////////////////////////////////////////////////
   // string utils

   CString GetBaseName(const CString & path);
};

#endif