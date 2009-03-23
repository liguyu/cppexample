// DialogAnalogUser.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogAnalogUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Event.h"


extern	int		iOpenedIndex1;
extern  int		iOpenedIndex2;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern CmdParamData_AnalogUser_t g_AnalogUser;


/////////////////////////////////////////////////////////////////////////////
// CDialogAnalogUser dialog


CDialogAnalogUser::CDialogAnalogUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAnalogUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogAnalogUser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogAnalogUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAnalogUser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogAnalogUser, CDialog)
	//{{AFX_MSG_MAP(CDialogAnalogUser)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogAnalogUser message handlers

BOOL CDialogAnalogUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char	TmpStr[256];
	
	GetDlgItem ( IDC_EDIT_USER_DEV )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
	//GetDlgItem ( IDC_EDIT_VOC_DEV )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID) );
	
	((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DISABLE ))->SetCheck( (g_AnalogUser.m_u8CallerIdOption == 0) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_FSK ))->SetCheck( (g_AnalogUser.m_u8CallerIdOption == 1) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DTMF ))->SetCheck( (g_AnalogUser.m_u8CallerIdOption == 2) ); 

	((CButton *)GetDlgItem ( IDC_RADIO_SINGLEPULSE ))->SetCheck( (g_AnalogUser.m_u8RingCadenceType == 0) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_DOUBLEPULSE ))->SetCheck( (g_AnalogUser.m_u8RingCadenceType == 1) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_TRIPPULSE ))->SetCheck( (g_AnalogUser.m_u8RingCadenceType == 2) ); 
		

	GetDlgItem ( IDC_EDIT_CALLOUTRINGTIMETOU )->SetWindowText ( "30");
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogAnalogUser::OnButton1() 
{
	char	TmpStr[256];
	int		iTmpVal;

	if ( ((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DISABLE ))->GetCheck() )
		g_AnalogUser.m_u8CallerIdOption  = 0; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_FSK ))->GetCheck() )
		g_AnalogUser.m_u8CallerIdOption  = 1; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DTMF ))->GetCheck() )
		g_AnalogUser.m_u8CallerIdOption  = 2; 
	
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SINGLEPULSE ))->GetCheck() )
		g_AnalogUser.m_u8RingCadenceType = 0; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_DOUBLEPULSE ))->GetCheck() )
		g_AnalogUser.m_u8RingCadenceType  = 1; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_TRIPPULSE ))->GetCheck() )
		g_AnalogUser.m_u8RingCadenceType  = 2; 


	//GetDlgItem ( IDC_EDIT_CALLOUTRINGTIMETOU )->GetDlgItem(TmpStr);
	memset(TmpStr,0,sizeof(TmpStr));
	CString sss;
	GetDlgItemText( IDC_EDIT_CALLOUTRINGTIMETOU ,sss);
	
	g_AnalogUser.m_u8CallOutRingTimeOut = atoi((char*)(LPCSTR)sss);

	CDialog::OnOK();		
}
