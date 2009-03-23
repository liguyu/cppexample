// Dialog3GFaxTone.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "Dialog3GFaxTone.h"
#include "DJAcsDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog3GFaxTone dialog

extern ITP_3G_FaxToneStruct_t g_3GFaxTone;

CDialog3GFaxTone::CDialog3GFaxTone(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog3GFaxTone::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog3GFaxTone)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog3GFaxTone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog3GFaxTone)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog3GFaxTone, CDialog)
	//{{AFX_MSG_MAP(CDialog3GFaxTone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog3GFaxTone message handlers

void CDialog3GFaxTone::OnOK() 
{
	// TODO: Add extra validation here
	
	g_3GFaxTone.m_u16ChanEnable = ((CButton *)GetDlgItem ( IDC_CHECK_3GFAXTONE ))->GetCheck(); 
	CDialog::OnOK();
}
