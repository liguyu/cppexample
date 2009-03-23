// XMSApi_ControlPlay.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_ControlPlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_Func.h"
#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Event.h"

extern	int		iOpenedIndex1;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern ControlPlay_t		g_ControlPlay;

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ControlPlay dialog


CXMSApi_ControlPlay::CXMSApi_ControlPlay(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_ControlPlay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_ControlPlay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_ControlPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_ControlPlay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_ControlPlay, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_ControlPlay)
	ON_BN_CLICKED(IDC_BUTTON_Stop, OnBUTTONStop)
	ON_BN_CLICKED(IDC_BUTTON_Pause, OnBUTTONPause)
	ON_BN_CLICKED(IDC_BUTTON_Resume, OnBUTTONResume)
	ON_BN_CLICKED(IDC_BUTTON_StepBack, OnBUTTONStepBack)
	ON_BN_CLICKED(IDC_BUTTON_Forward, OnBUTTONForward)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ControlPlay message handlers

BOOL CXMSApi_ControlPlay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char	TmpStr[32];

	GetDlgItem ( IDC_EDIT_ControlPlay_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	//g_ControlPlay . m_u16StepSize
	sprintf ( TmpStr, "%d", g_ControlPlay.m_u16StepSize );
	GetDlgItem ( IDC_EDIT_u16StepSize )->SetWindowText ( TmpStr );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMSApi_ControlPlay::OnBUTTONStop() 
{
	// TODO: Add your control notification handler code here

	g_ControlPlay.m_u16ControlType = XMS_STOP_PLAY;
	Get_u16StepSize ();

	DoFunc_ControlPlay_Real( &g_ControlPlay);
}

void CXMSApi_ControlPlay::OnBUTTONPause() 
{
	// TODO: Add your control notification handler code here

	g_ControlPlay.m_u16ControlType = XMS_PAUSE_PLAY;
	Get_u16StepSize ();

	DoFunc_ControlPlay_Real( &g_ControlPlay);
	
}

void CXMSApi_ControlPlay::OnBUTTONResume() 
{
	// TODO: Add your control notification handler code here
	
	g_ControlPlay.m_u16ControlType = XMS_RESUME_PLAY;
	Get_u16StepSize ();

	DoFunc_ControlPlay_Real( &g_ControlPlay);
}

void CXMSApi_ControlPlay::OnBUTTONStepBack() 
{
	// TODO: Add your control notification handler code here
	
	g_ControlPlay.m_u16ControlType = XMS_STEP_BACKWARD_PLAY;
	Get_u16StepSize ();

	DoFunc_ControlPlay_Real( &g_ControlPlay);
}

void CXMSApi_ControlPlay::OnBUTTONForward() 
{
	// TODO: Add your control notification handler code here
	
	g_ControlPlay.m_u16ControlType = XMS_STEP_FORWARD_PLAY;
	Get_u16StepSize ();

	DoFunc_ControlPlay_Real( &g_ControlPlay);
}

void CXMSApi_ControlPlay::OnOK() 
{
	// TODO: Add extra validation here

	Get_u16StepSize ();

	CDialog::OnOK();
}

void CXMSApi_ControlPlay::Get_u16StepSize()
{
	char	TmpStr[32];
	int		iTmpVal;

	//g_ControlPlay . m_u16StepSize
	GetDlgItem ( IDC_EDIT_u16StepSize )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_ControlPlay.m_u16StepSize = iTmpVal;

}
