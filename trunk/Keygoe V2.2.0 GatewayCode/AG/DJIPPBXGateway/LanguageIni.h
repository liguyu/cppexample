// LanguageIni.h: interface for the CLanguageIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGUAGEINI_H__65E38878_B920_46D4_9FE2_309AF4FD9F62__INCLUDED_)
#define AFX_LANGUAGEINI_H__65E38878_B920_46D4_9FE2_309AF4FD9F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLanguageIni  
{
public:
	CString GLoadID(const CString &strID);
	char* GLoadID(const char* pchString, char* pchReString, size_t size);
	CString GLoadString(const CString &strKey);
	char* GLoadString(const char* pchString, char* pchReString, size_t size);
	void SetLIniPath(const CString &strLIni);
	CLanguageIni();
	virtual ~CLanguageIni();

protected:
	CString m_strLIni;
};

#endif // !defined(AFX_LANGUAGEINI_H__65E38878_B920_46D4_9FE2_309AF4FD9F62__INCLUDED_)
