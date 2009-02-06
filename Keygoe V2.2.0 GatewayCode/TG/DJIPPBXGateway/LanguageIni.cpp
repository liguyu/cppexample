// LanguageIni.cpp: implementation of the CLanguageIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "LanguageIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLanguageIni::CLanguageIni()
{

}

CLanguageIni::~CLanguageIni()
{

}

void CLanguageIni::SetLIniPath(const CString &strLIni)
{
	m_strLIni = strLIni;
}

char* CLanguageIni::GLoadString(const char* pchString, char* pchReString, size_t size)
{
	ASSERT(pchString!=NULL);
	ASSERT(pchReString!=NULL);
	GetPrivateProfileString("STRING", pchString, pchString, pchReString, size, m_strLIni);

	return pchReString;
}

CString CLanguageIni::GLoadString(const CString &strKey)
{
	ASSERT(!strKey.IsEmpty());
	char chString[256];
	GetPrivateProfileString("STRING", strKey, strKey, chString, 256, m_strLIni);

	return chString;
}

char* CLanguageIni::GLoadID(const char* pchString, char* pchReString, size_t size)
{
	ASSERT(pchString!=NULL);
	ASSERT(pchReString!=NULL);
	GetPrivateProfileString("ID", pchString, pchString, pchReString, size, m_strLIni);

	return pchReString;
}

CString CLanguageIni::GLoadID(const CString &strID)
{
	ASSERT(!strID.IsEmpty());
	char chString[256];
	GetPrivateProfileString("ID", strID, strID, chString, 256, m_strLIni);

	return chString;
}
