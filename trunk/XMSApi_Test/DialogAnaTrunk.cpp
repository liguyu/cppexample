// DialogAnaTrunk.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogAnaTrunk.h"

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

extern CmdParamData_AnalogTrunk_t g_AnalogTrunk;

/////////////////////////////////////////////////////////////////////////////
// CDialogAnaTrunk dialog


CDialogAnaTrunk::CDialogAnaTrunk(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAnaTrunk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogAnaTrunk)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogAnaTrunk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAnaTrunk)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogAnaTrunk, CDialog)
	//{{AFX_MSG_MAP(CDialogAnaTrunk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogAnaTrunk message handlers

BOOL CDialogAnaTrunk::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_TRUNK_DEV )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
	
	//GetDlgItem ( IDC_EDIT_VOC_DEV )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID) );

	//((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DISABLE ))->SetCheck( (g_AnalogTrunk.m_u8CallerIdOption == 0) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_FSK ))->SetCheck( (g_AnalogTrunk.m_u8CallerIdOption == 1) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DTMF ))->SetCheck( (g_AnalogTrunk.m_u8CallerIdOption == 2) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_FSKORDTMF ))->SetCheck( (g_AnalogTrunk.m_u8CallerIdOption == 3) ); 

	//sprintf(TmpStr,"%d",g_AnalogTrunk.m_u8CallInRingCount);
	//GetDlgItem ( IDC_EDIT_CALLIN_RNGCOUNT )->SetWindowText ( TmpStr );

	//((CButton *)GetDlgItem ( IDC_RADIO_DIALTONE_DONTDETECT ))->SetCheck( (g_AnalogTrunk.m_u8DialtoneDetect == 0) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_DIALTONE_DETECT ))->SetCheck( (g_AnalogTrunk.m_u8DialtoneDetect == 1) ); 

	//((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_APPFULL ))->SetCheck( (g_AnalogTrunk.m_u8CallOutMethod == 0) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_RNGBACK ))->SetCheck( (g_AnalogTrunk.m_u8CallOutMethod == 1) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_POLARITY ))->SetCheck( (g_AnalogTrunk.m_u8CallOutMethod == 2) );  

	sprintf(TmpStr,"%d",g_AnalogTrunk.m_u16CallInRingCount);
	GetDlgItem ( IDC_EDIT_CALLINRINGCOUNT )->SetWindowText ( TmpStr );

	sprintf(TmpStr,"%d",g_AnalogTrunk.m_u16CallInRingTimeOut);
	GetDlgItem ( IDC_EDIT_ANSWERCALLINTIMEOUT )->SetWindowText ( TmpStr );


	//((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_MANUAL ))->SetCheck( (g_AnalogTrunk.m_u8CallOutMode == 0) ); 
	//((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_AUTOMATIC ))->SetCheck( (g_AnalogTrunk.m_u8CallOutMode == 1) ); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogAnaTrunk::OnOK() 
{
	char	TmpStr[256];
	int		iTmpVal;

	/*if ( ((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DISABLE ))->GetCheck() )
		g_AnalogTrunk.m_u8CallerIdOption  = 0; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_FSK ))->GetCheck() )
		g_AnalogTrunk.m_u8CallerIdOption  = 1; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_DTMF ))->GetCheck() )
		g_AnalogTrunk.m_u8CallerIdOption  = 2; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLERID_FSKORDTMF ))->GetCheck() )
		g_AnalogTrunk.m_u8CallerIdOption  = 3; 
	

	GetDlgItem ( IDC_EDIT_CALLIN_RNGCOUNT )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_AnalogTrunk.m_u8CallInRingCount = iTmpVal;

	if ( ((CButton *)GetDlgItem ( IDC_RADIO_DIALTONE_DONTDETECT ))->GetCheck() )
		g_AnalogTrunk.m_u8DialtoneDetect = 0; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_DIALTONE_DETECT ))->GetCheck() )
		g_AnalogTrunk.m_u8DialtoneDetect  = 1; 
	

	if ( ((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_APPFULL ))->GetCheck() )
		g_AnalogTrunk.m_u8CallOutMethod = 0; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_RNGBACK ))->GetCheck() )
		g_AnalogTrunk.m_u8CallOutMethod  = 1; 
	else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_POLARITY ))->GetCheck() )
		g_AnalogTrunk.m_u8CallOutMethod  = 2; */

	
	GetDlgItem ( IDC_EDIT_CALLINRINGCOUNT )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_AnalogTrunk.m_u16CallInRingCount = iTmpVal;

	GetDlgItem ( IDC_EDIT_ANSWERCALLINTIMEOUT )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_AnalogTrunk.m_u16CallInRingTimeOut = iTmpVal;


	//if ( ((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_MANUAL ))->GetCheck() )
	//	g_AnalogTrunk.m_u8CallOutMode = 0; 
	//else if(((CButton *)GetDlgItem ( IDC_RADIO_CALLOUTMETHOD_AUTOMATIC ))->GetCheck() )
	//	g_AnalogTrunk.m_u8CallOutMode  = 1; 
	
	CDialog::OnOK();
}
