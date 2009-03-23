// XMS_Test_ParamVoIP.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMS_Test_ParamVoIP.h"

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
extern	int		iOpenedIndex2;
extern	int		iOpenedIndex3;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern MixerControlParam_t	g_VoIP_Param;

/////////////////////////////////////////////////////////////////////////////
// CXMS_Test_ParamVoIP dialog


CXMS_Test_ParamVoIP::CXMS_Test_ParamVoIP(CWnd* pParent /*=NULL*/)
	: CDialog(CXMS_Test_ParamVoIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMS_Test_ParamVoIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMS_Test_ParamVoIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMS_Test_ParamVoIP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMS_Test_ParamVoIP, CDialog)
	//{{AFX_MSG_MAP(CXMS_Test_ParamVoIP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMS_Test_ParamVoIP message handlers

BOOL CXMS_Test_ParamVoIP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_IPDEV )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
	GetDlgItem ( IDC_EDIT_VOCDEV )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID) );

	((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->SetCheck ( 
			(g_VoIP_Param.m_u8SRC1_Ctrl == XMS_MIXER_FROM_NULL) );
	((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->SetCheck ( 
			(g_VoIP_Param.m_u8SRC1_Ctrl == XMS_MIXER_FROM_INPUT) );
	((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->SetCheck ( 
			(g_VoIP_Param.m_u8SRC1_Ctrl == XMS_MIXER_FROM_PLAY) );
	((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->SetCheck ( 
			(g_VoIP_Param.m_u8SRC1_Ctrl == XMS_MIXER_FROM_CONF) );
	((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_IP ))->SetCheck ( 
			(g_VoIP_Param.m_u8SRC1_Ctrl == XMS_MIXER_FROM_IP) );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CXMS_Test_ParamVoIP::OnOK() 
{
	// TODO: Add extra validation here
	
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->GetCheck () ) 
		g_VoIP_Param.m_u8SRC1_Ctrl = XMS_MIXER_FROM_NULL;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->GetCheck () )
		g_VoIP_Param.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->GetCheck () ) 
		g_VoIP_Param.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->GetCheck () ) 
		g_VoIP_Param.m_u8SRC1_Ctrl = XMS_MIXER_FROM_CONF;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_IP ))->GetCheck () ) 
		g_VoIP_Param.m_u8SRC1_Ctrl = XMS_MIXER_FROM_IP;
	CDialog::OnOK();
}
