// DialogAtrunkFlash.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogAtrunkFlash.h"
#include "DJAcsDataDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DialogAtrunkFlash dialog

extern CmdParamData_AnalogTrunkFlash_t g_ATrunkFlash;
DialogAtrunkFlash::DialogAtrunkFlash(CWnd* pParent /*=NULL*/)
	: CDialog(DialogAtrunkFlash::IDD, pParent)
{
	//{{AFX_DATA_INIT(DialogAtrunkFlash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DialogAtrunkFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogAtrunkFlash)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogAtrunkFlash, CDialog)
	//{{AFX_MSG_MAP(DialogAtrunkFlash)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogAtrunkFlash message handlers

BOOL DialogAtrunkFlash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//((CButton *)GetDlgItem ( IDC_CHECK_AT_CHANNEL_ENABLED ))->SetCheck(g_ATrunkFlash.m_u8Channel_Enable); 
	((CButton *)GetDlgItem ( IDC_CHECK_AT_CHANNEL_ENABLED ))->SetCheck(1); 
	GetDlgItem ( IDC_EDIT_AT_TYPE )->SetWindowText ( "0" );
	GetDlgItem ( IDC_EDIT_AT_TXSTATE )->SetWindowText ( "2" );
	GetDlgItem ( IDC_EDIT_AT_RXSTATE )->SetWindowText ( "0" );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DialogAtrunkFlash::OnOK() 
{
	// TODO: Add extra validation here
	char TmpStr[20];
	int iTmpVal;

	g_ATrunkFlash.m_u8Channel_Enable = ((CButton *)GetDlgItem ( IDC_CHECK_AT_CHANNEL_ENABLED ))->GetCheck(); 

	GetDlgItem ( IDC_EDIT_AT_TYPE )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_ATrunkFlash.m_u8Type = iTmpVal;

	GetDlgItem ( IDC_EDIT_AT_TXSTATE )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_ATrunkFlash.m_u8Tx_State = iTmpVal;
	
	GetDlgItem ( IDC_EDIT_AT_RXSTATE )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_ATrunkFlash.m_u8Rx_State = iTmpVal;

	CDialog::OnOK();
}
