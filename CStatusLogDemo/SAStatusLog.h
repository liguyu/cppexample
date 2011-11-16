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

#ifndef CSAStatusLog_Header_Thingy
#define CSAStatusLog_Header_Thingy

/////////////////////////////////////////////////////////////////////////////

class CSAStatusLog
{
public:
   /////////////////////////////////////////////////////////////////////////////

	CSAStatusLog();
   ~CSAStatusLog();

   /////////////////////////////////////////////////////////////////////////////
   // call this first!
   void		Init(const char *pOutputFilename);

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

protected:
   /////////////////////////////////////////////////////////////////////////////
   // work buffer
	enum {TBUF_SIZE = 3000};
   char		m_tBuf[TBUF_SIZE];

   CString  m_csAppName;
	CString	m_csFileName;
	
   /////////////////////////////////////////////////////////////////////////////
   // protection
   CRITICAL_SECTION  m_crit;

   /////////////////////////////////////////////////////////////////////////////
   // controlling stuff
	BOOL		m_bEnable;
   BOOL     m_bPrintTime;
   BOOL     m_bPrintAppName;

   /////////////////////////////////////////////////////////////////////////////
   // string utils
   CString GetBaseDir(const CString & path);
   CString GetBaseName(const CString & path);

};

#endif