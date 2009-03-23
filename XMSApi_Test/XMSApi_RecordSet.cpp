// XMSApi_RecordSet.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_RecordSet.h"

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

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern RecordProperty_t		g_RecordProperty;
/////////////////////////////////////////////////////////////////////////////
// CXMSApi_RecordSet dialog


CXMSApi_RecordSet::CXMSApi_RecordSet(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_RecordSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_RecordSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_RecordSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_RecordSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_RecordSet, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_RecordSet)
	ON_BN_CLICKED(IDC_RADIO_Normal_Stop, OnRADIONormalStop)
	ON_BN_CLICKED(IDC_RADIO_Single_Code, OnRADIOSingleCode)
	ON_BN_CLICKED(IDC_RADIO_Any_Code, OnRADIOAnyCode)
	ON_BN_CLICKED(IDC_CHECK_s8MixEnable, OnCHECKs8MixEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_RecordSet message handlers

BOOL CXMSApi_RecordSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_Record_DevcieID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	// g_RecordProperty . m_u32MaxSize
	sprintf ( TmpStr, "%d", g_RecordProperty.m_u32MaxSize );
	GetDlgItem ( IDC_EDIT_Record_u32MaxSize )->SetWindowText ( TmpStr );

	// g_RecordProperty . m_u32MaxTime
	sprintf ( TmpStr, "%d", g_RecordProperty.m_u32MaxTime );
	GetDlgItem ( IDC_EDIT_Record_u32MaxTime )->SetWindowText ( TmpStr );

	// g_RecordProperty . m_u8EncodeType
	if ( g_RecordProperty.m_u8EncodeType == XMS_Alaw_type )
		((CButton *)GetDlgItem ( IDC_RADIO_Alaw_type ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Alaw_type ))->SetCheck(0);
	if ( g_RecordProperty.m_u8EncodeType == XMS_Ulaw_type )
		((CButton *)GetDlgItem ( IDC_RADIO_Ulaw_type ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Ulaw_type ))->SetCheck(0);
	if ( g_RecordProperty.m_u8EncodeType == XMS_Vox_type )
		((CButton *)GetDlgItem ( IDC_RADIO_Vox_type ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Vox_type ))->SetCheck(0);
	if ( g_RecordProperty.m_u8EncodeType == XMS_Linear_8bit )
		((CButton *)GetDlgItem ( IDC_RADIO_Linear_8bit ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Linear_8bit ))->SetCheck(0);
	if ( g_RecordProperty.m_u8EncodeType == XMS_Linear_16bit )
		((CButton *)GetDlgItem ( IDC_RADIO_Linear_16bit ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Linear_16bit ))->SetCheck(0);

	// g_RecordProperty . m_u8SRCMode
	if ( g_RecordProperty.m_u8SRCMode == XMS_SRC_8K )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_8K ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_8K ))->SetCheck(0);
	if ( g_RecordProperty.m_u8SRCMode == XMS_SRC_6K )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_6K ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_6K ))->SetCheck(0);
	if ( g_RecordProperty.m_u8SRCMode == XMS_SRC_GTG )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_GTG ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_GTG ))->SetCheck(0);
	if ( g_RecordProperty.m_u8SRCMode == XMS_SRC_FSK )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FSK ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FSK ))->SetCheck(0);
	if ( g_RecordProperty.m_u8SRCMode == XMS_SRC_FAX )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FAX ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FAX ))->SetCheck(0);

	// g_RecordProperty . m_u8StopMode
	if ( g_RecordProperty.m_u8StopMode == XMS_Normal_Stop )
		((CButton *)GetDlgItem ( IDC_RADIO_Normal_Stop ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Normal_Stop ))->SetCheck(0);
	if ( g_RecordProperty.m_u8StopMode == XMS_Single_Code )
		((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->SetCheck(0);
	if ( g_RecordProperty.m_u8StopMode == XMS_Any_Code )
		((CButton *)GetDlgItem ( IDC_RADIO_Any_Code ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Any_Code ))->SetCheck(0);

	// g_RecordProperty . m_u8StopCode
	sprintf ( TmpStr, "%d", g_RecordProperty.m_u8StopCode );
	GetDlgItem ( IDC_EDIT_Record_u8StopCode )->SetWindowText ( TmpStr );

	// g_RecordProperty . m_s8IsMixEnable
	((CButton *)GetDlgItem ( IDC_CHECK_s8MixEnable ))->SetCheck(g_RecordProperty.m_s8IsMixEnable); 

	// g_RecordProperty . m_MixerControl . m_u8SRC1_Ctrl
	if ( g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_NULL )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_INPUT )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_PLAY )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_CONF )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_IP )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP ))->SetCheck(0);

	// g_RecordProperty . m_MixerControl . m_u8SRC2_Ctrl
	if ( g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_NULL )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_INPUT )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_PLAY )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_CONF )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 ))->SetCheck(0);
	if ( g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_IP )
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 ))->SetCheck(0);

	// g_RecordProperty . m_MixerControl . m_u16SRC_ChID1
	sprintf ( TmpStr, "%d", g_RecordProperty.m_MixerControl.m_u16SRC_ChID1 );
	GetDlgItem ( IDC_EDIT_Record_u16SRC_ChID1 )->SetWindowText ( TmpStr );
	
	// g_RecordProperty . m_MixerControl . m_u16SRC_ChID2
	sprintf ( TmpStr, "%d", g_RecordProperty.m_MixerControl.m_u16SRC_ChID2 );
	GetDlgItem ( IDC_EDIT_Record_u16SRC_ChID2 )->SetWindowText ( TmpStr );

	// g_RecordProperty . m_s8IsAppend
	((CButton *)GetDlgItem ( IDC_CHECK_s8IsAppend ))->SetCheck(g_RecordProperty.m_s8IsAppend);

	// g_RecordProperty . m_s8FileName
	GetDlgItem ( IDC_EDIT_Record_s8FileName )->SetWindowText (g_RecordProperty.m_s8FileName);

	// Handle Enable Disable
	HandleEnableDisable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CXMSApi_RecordSet::OnRADIONormalStop() 
{
	// TODO: Add your control notification handler code here

	// Handle Enable Disable
	HandleEnableDisable();
}

void CXMSApi_RecordSet::OnRADIOSingleCode() 
{
	// TODO: Add your control notification handler code here

	// Handle Enable Disable
	HandleEnableDisable();
}

void CXMSApi_RecordSet::OnRADIOAnyCode() 
{
	// TODO: Add your control notification handler code here

	// Handle Enable Disable
	HandleEnableDisable();
}

void CXMSApi_RecordSet::OnOK() 
{
	// TODO: Add extra validation here
	char	TmpStr[256];
	int		iTmpVal;

	// g_RecordProperty . m_u32MaxSize
	GetDlgItem ( IDC_EDIT_Record_u32MaxSize )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_RecordProperty.m_u32MaxSize );
	
	// g_RecordProperty . m_u32MaxTime
	GetDlgItem ( IDC_EDIT_Record_u32MaxTime )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_RecordProperty.m_u32MaxTime );

	// g_RecordProperty . m_u8EncodeType
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Alaw_type ))->GetCheck() )
		g_RecordProperty.m_u8EncodeType = XMS_Alaw_type;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Ulaw_type ))->GetCheck() )
		g_RecordProperty.m_u8EncodeType = XMS_Ulaw_type;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Vox_type ))->GetCheck() )
		g_RecordProperty.m_u8EncodeType = XMS_Vox_type;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Linear_8bit ))->GetCheck() )
		g_RecordProperty.m_u8EncodeType = XMS_Linear_8bit;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Linear_16bit ))->GetCheck() )
		g_RecordProperty.m_u8EncodeType = XMS_Linear_16bit;

	// g_RecordProperty . m_u8SRCMode
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_8K ))->GetCheck() )
		g_RecordProperty.m_u8SRCMode = XMS_SRC_8K;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_6K ))->GetCheck() )
		g_RecordProperty.m_u8SRCMode = XMS_SRC_6K;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_GTG ))->GetCheck() )
		g_RecordProperty.m_u8SRCMode = XMS_SRC_GTG;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_FSK ))->GetCheck() )
		g_RecordProperty.m_u8SRCMode = XMS_SRC_FSK;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_FAX ))->GetCheck() )
		g_RecordProperty.m_u8SRCMode = XMS_SRC_FAX;

	// g_RecordProperty . m_u8StopMode
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Normal_Stop ))->GetCheck() )
		g_RecordProperty.m_u8StopMode = XMS_Normal_Stop;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->GetCheck() )
		g_RecordProperty.m_u8StopMode = XMS_Single_Code;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Any_Code ))->GetCheck() )
		g_RecordProperty.m_u8StopMode = XMS_Any_Code;

	// g_RecordProperty . m_u8StopCode
	GetDlgItem ( IDC_EDIT_Record_u8StopCode )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_RecordProperty.m_u8StopCode = iTmpVal;

	// g_RecordProperty . m_s8IsMixEnable
	g_RecordProperty.m_s8IsMixEnable = ((CButton *)GetDlgItem ( IDC_CHECK_s8MixEnable ))->GetCheck();

	// g_RecordProperty . m_MixerControl . m_u8SRC1_Ctrl
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_NULL;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_CONF;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_IP;

	// g_RecordProperty . m_MixerControl . m_u8SRC2_Ctrl
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_NULL;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_PLAY;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_CONF;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 ))->GetCheck() )
		g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_IP;

	// g_RecordProperty . m_MixerControl . m_u16SRC_ChID1
	GetDlgItem ( IDC_EDIT_Record_u16SRC_ChID1 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_RecordProperty.m_MixerControl.m_u16SRC_ChID1 = iTmpVal;
	
	// g_RecordProperty . m_MixerControl . m_u16SRC_ChID2
	GetDlgItem ( IDC_EDIT_Record_u16SRC_ChID2 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_RecordProperty.m_MixerControl.m_u16SRC_ChID2 = iTmpVal;

	// g_RecordProperty . m_s8IsAppend
	g_RecordProperty . m_s8IsAppend = ((CButton *)GetDlgItem ( IDC_CHECK_s8IsAppend ))->GetCheck();
	
	CDialog::OnOK();
}

void CXMSApi_RecordSet::HandleEnableDisable()
{
	bool	bNow;

	// Enable/Disable : m_u32StopMode
	bNow = (((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->GetCheck()==1);
	GetDlgItem ( IDC_EDIT_Record_u8StopCode )->EnableWindow(bNow);

	// Enable/Disable : m_s8MixEnable
	bNow = (((CButton *)GetDlgItem ( IDC_CHECK_s8MixEnable ))->GetCheck()==1);
	{
		// g_RecordProperty . m_MixerControl . m_u8SRC1_Ctrl
		GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP )->EnableWindow(bNow);

		// g_RecordProperty . m_MixerControl . m_u8SRC2_Ctrl
		GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 )->EnableWindow(bNow);
		GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 )->EnableWindow(bNow);

		// g_RecordProperty . m_MixerControl . m_u16SRC_ChID1
		GetDlgItem ( IDC_EDIT_Record_u16SRC_ChID1 )->EnableWindow(bNow);
	
		// g_RecordProperty . m_MixerControl . m_u16SRC_ChID2
		GetDlgItem ( IDC_EDIT_Record_u16SRC_ChID2 )->EnableWindow(bNow);
	}

}

void CXMSApi_RecordSet::OnCHECKs8MixEnable() 
{
	// TODO: Add your control notification handler code here
	
	// Handle Enable Disable
	HandleEnableDisable();
}
