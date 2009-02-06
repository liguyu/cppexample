// ShowInfo.cpp: implementation of the CShowInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "ShowInfo.h"

#include "DJIPPBXGatewayDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShowInfo::CShowInfo(DWORD pDlg)
{
	m_pDlg = (CDJIPPBXGatewayDlg *)pDlg;
	m_hWnd = m_pDlg->GetSafeHwnd();
}

CShowInfo::~CShowInfo()
{

}

void CShowInfo::ShowInfo(int iIndex, int iDspID, int iVoipID)
{
	PostMessage(m_hWnd, WM_SHOWLISTCALLINFOONE, iIndex, iDspID*DSPBASE+iVoipID);
}

void CShowInfo::ShowInfo(CString strInfo)
{
	if( m_pDlg->m_listLogInfo.GetCount() > 5000 )
	{
		m_pDlg->m_listLogInfo.ResetContent();
	}

	m_pDlg->m_listLogInfo.AddString(strInfo);
}

void CShowInfo::InitShowInfo()
{
	PostMessage(m_hWnd, WM_SHOWLISTCALLINFOALL, 0, 0);
}