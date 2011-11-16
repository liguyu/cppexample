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
   m_bPrintTime = FALSE;
	m_csFileName = "";

   m_csAppName = "";

   // we'll make sure only one call uses the critical stuff at a time
   InitializeCriticalSection(&m_crit);
}

/////////////////////////////////////////////////////////////////////////////

CSAStatusLog::~CSAStatusLog()
{
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
		   fprintf(fp,"%s : ",ct.Format("%m/%d/%Y %H:%M:%S"));
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
