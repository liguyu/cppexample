// ShowInfo.h: interface for the CShowInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWINFO_H__075632FC_CF55_4BA9_A899_80545530340B__INCLUDED_)
#define AFX_SHOWINFO_H__075632FC_CF55_4BA9_A899_80545530340B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int DSPBASE = 10000;

class CDJIPPBXGatewayDlg;

class CShowInfo  
{
public:
	CShowInfo(DWORD pDlg);
	virtual ~CShowInfo();

	void ShowInfo(int iIndex, int iDspID, int iVoipID);
	void ShowInfo(CString strInfo);
	void InitShowInfo();

protected:
	CDJIPPBXGatewayDlg *m_pDlg;
	HWND m_hWnd;
};

#endif // !defined(AFX_SHOWINFO_H__075632FC_CF55_4BA9_A899_80545530340B__INCLUDED_)
