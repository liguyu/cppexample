// XMSApi_ConfParam.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_ConfParam.h"

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
extern	int		iOpenedIndex2;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern CmdParamData_Conf_t	g_Conf_Param;

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ConfParam dialog


CXMSApi_ConfParam::CXMSApi_ConfParam(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_ConfParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_ConfParam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_ConfParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_ConfParam)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_ConfParam, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_ConfParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ConfParam message handlers

extern int		g_IsLeaveConf;

BOOL CXMSApi_ConfParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	GetDlgItem ( IDC_EDIT_Conf_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
	
	GetDlgItem ( IDC_EDIT_Media_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID) );

	//g_Conf_Param . m_u8InputOpt
	if ( g_Conf_Param.m_u8InputOpt == 0 )
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_0 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_0 ))->SetCheck(0);
	if ( g_Conf_Param.m_u8InputOpt == 1 )
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_1 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_1 ))->SetCheck(0);
	if ( g_Conf_Param.m_u8InputOpt == 2 )
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_2 ))->SetCheck(0);

	//g_Conf_Param . m_u8OutputOpt
	if ( g_Conf_Param.m_u8OutputOpt == 0 )
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_0 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_0 ))->SetCheck(0);
	if ( g_Conf_Param.m_u8OutputOpt == 1 )
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_1 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_1 ))->SetCheck(0);
	if ( g_Conf_Param.m_u8OutputOpt == 2 )
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_2 ))->SetCheck(0);
	
	if ( g_IsLeaveConf )
	{
		((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_0 ))->EnableWindow(false);

		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_0 ))->EnableWindow(false);
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_1 ))->EnableWindow(false);
		((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_2 ))->EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMSApi_ConfParam::OnOK() 
{
	// TODO: Add extra validation here

	//g_Conf_Param . m_u8InputOpt
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_0 ))->GetCheck() )
		g_Conf_Param.m_u8InputOpt = 0;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_1 ))->GetCheck() )
		g_Conf_Param.m_u8InputOpt = 1;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_InputOpt_2 ))->GetCheck() )
		g_Conf_Param.m_u8InputOpt = 2;


	//g_Conf_Param . m_u8OutputOpt
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_0 ))->GetCheck() )
		g_Conf_Param.m_u8OutputOpt = 0;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_1 ))->GetCheck() )
		g_Conf_Param.m_u8OutputOpt = 1;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_OutputOpt_2 ))->GetCheck() )
		g_Conf_Param.m_u8OutputOpt = 2;
	
	CDialog::OnOK();
}
