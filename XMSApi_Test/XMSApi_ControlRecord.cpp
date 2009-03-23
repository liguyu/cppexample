// XMSApi_ControlRecord.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_ControlRecord.h"

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

extern ControlRecord_t		g_ControlRecord;

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ControlRecord dialog


CXMSApi_ControlRecord::CXMSApi_ControlRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_ControlRecord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_ControlRecord)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_ControlRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_ControlRecord)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_ControlRecord, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_ControlRecord)
	ON_BN_CLICKED(IDC_BUTTON_Stop, OnBUTTONStop)
	ON_BN_CLICKED(IDC_BUTTON_Pause, OnBUTTONPause)
	ON_BN_CLICKED(IDC_BUTTON_Resume, OnBUTTONResume)
	ON_BN_CLICKED(IDC_BUTTON_StepBack, OnBUTTONStepBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ControlRecord message handlers

BOOL CXMSApi_ControlRecord::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char	TmpStr[32];

	GetDlgItem ( IDC_EDIT_ControlRecord_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	//g_ControlRecord . m_u32StepSize
	sprintf ( TmpStr, "%d", g_ControlRecord.m_u32StepSize );
	GetDlgItem ( IDC_EDIT_u32StepSize )->SetWindowText ( TmpStr );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMSApi_ControlRecord::OnOK() 
{
	// TODO: Add extra validation here
	
	Get_u32StepSize ();

	CDialog::OnOK();
}

void CXMSApi_ControlRecord::Get_u32StepSize()
{
	char	TmpStr[32];
	int		iTmpVal;

	//g_ControlRecord . m_u32StepSize
	GetDlgItem ( IDC_EDIT_u32StepSize )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_ControlRecord.m_u32StepSize = iTmpVal;

}

void CXMSApi_ControlRecord::OnBUTTONStop() 
{
	// TODO: Add your control notification handler code here
	
	g_ControlRecord.m_u32ControlType = XMS_STOP_RECORD;
	Get_u32StepSize ();

	DoFunc_ControlRecord_Real( &g_ControlRecord);
}

void CXMSApi_ControlRecord::OnBUTTONPause() 
{
	// TODO: Add your control notification handler code here
	g_ControlRecord.m_u32ControlType = XMS_PAUSE_RECORD;
	Get_u32StepSize ();

	DoFunc_ControlRecord_Real( &g_ControlRecord);
	
}

void CXMSApi_ControlRecord::OnBUTTONResume() 
{
	// TODO: Add your control notification handler code here
	g_ControlRecord.m_u32ControlType = XMS_RESUME_RECORD;
	Get_u32StepSize ();

	DoFunc_ControlRecord_Real( &g_ControlRecord);
	
}

void CXMSApi_ControlRecord::OnBUTTONStepBack() 
{
	// TODO: Add your control notification handler code here
	g_ControlRecord.m_u32ControlType = XMS_STEP_BACKWARD_RECORD;
	Get_u32StepSize ();

	DoFunc_ControlRecord_Real( &g_ControlRecord);
	
}
