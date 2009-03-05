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
	BOOL		m_fMapOpen;				// 打开标志
	HANDLE      hFile;
	HANDLE		m_hFileMap;				// 映射文件句柄
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

   int			m_nType;				// 文件打开模式
   TCHAR		*m_lpbMapAddress;		// 映射文件指针
   LONG 		m_qwFileOffset;			// 偏移地址 
   LONG		    m_qwFileSize;			// 打开的文件大小
   BOOL		    m_bWrited;				// 写标志

   /////////////////////////////////////////////////////////////////////////////
   // string utils

   CString GetBaseName(const CString & path);
};

#endif