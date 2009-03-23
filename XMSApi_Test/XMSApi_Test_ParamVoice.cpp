// XMSApi_Test_ParamVoice.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_Test_ParamVoice.h"

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

extern CmdParamData_Voice_t	g_Voice_Param;

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_Test_ParamVoice dialog


CXMSApi_Test_ParamVoice::CXMSApi_Test_ParamVoice(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_Test_ParamVoice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_Test_ParamVoice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_Test_ParamVoice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_Test_ParamVoice)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_Test_ParamVoice, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_Test_ParamVoice)
	ON_BN_CLICKED(IDC_CHECK_u8InputCtrlValid, OnCHECKu8InputCtrlValid)
	ON_BN_CLICKED(IDC_CHECK_u8OutputCtrlValid, OnCHECKu8OutputCtrlValid)
	ON_BN_CLICKED(IDC_CHECK_u8GtdCtrlValid, OnCHECKu8GtdCtrlValid)
	ON_BN_CLICKED(IDC_CHECK_In_u8AgcEnable, OnCHECKInu8AgcEnable)
	ON_BN_CLICKED(IDC_CHECK_In_u8EcEnable, OnCHECKInu8EcEnable)
	ON_BN_CLICKED(IDC_CHECK_Out_u8AgcEnable, OnCHECKOutu8AgcEnable)
	ON_BN_CLICKED(IDC_RADIO_Out_Silence, OnRADIOOutSilence)
	ON_BN_CLICKED(IDC_RADIO_Out_Input, OnRADIOOutInput)
	ON_BN_CLICKED(IDC_RADIO_Out_Play, OnRADIOOutPlay)
	ON_BN_CLICKED(IDC_RADIO_Out_Mixer, OnRADIOOutMixer)
	ON_BN_CLICKED(IDC_CHECK_Gtd_u8ChannelEnable, OnCHECKGtdu8ChannelEnable)
	ON_BN_CLICKED(IDC_CHECK_u8PlayGainMode, OnCHECKu8PlayGainMode)
	ON_BN_CLICKED(IDC_CHECK_Gtd_u8MR2BEnable, OnCHECKGtdu8MR2BEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_Test_ParamVoice message handlers

BOOL CXMSApi_Test_ParamVoice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_ParamVoice_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	// g_Voice_Param . m_u8InputCtrlValid
	((CButton *)GetDlgItem ( IDC_CHECK_u8InputCtrlValid ))->SetCheck(g_Voice_Param.m_u8InputCtrlValid); 

	// g_Voice_Param . m_u8OutputCtrlValid
	((CButton *)GetDlgItem ( IDC_CHECK_u8OutputCtrlValid ))->SetCheck(g_Voice_Param.m_u8OutputCtrlValid); 

	// g_Voice_Param . m_u8GtdCtrlValid
	((CButton *)GetDlgItem ( IDC_CHECK_u8GtdCtrlValid ))->SetCheck(g_Voice_Param.m_u8GtdCtrlValid); 

	// ----- 
	// g_Voice_Param . m_VocInputControl .m_u8AgcEnable
	((CButton *)GetDlgItem ( IDC_CHECK_In_u8AgcEnable ))->SetCheck(g_Voice_Param.m_VocInputControl.m_u8AgcEnable); 

	// g_Voice_Param . m_VocInputControl .m_u8AgcMode
	((CButton *)GetDlgItem ( IDC_RADIO_In_ALS_Mode ))->SetCheck( (g_Voice_Param.m_VocInputControl.m_u8AgcMode==0) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_In_AGC_Mode ))->SetCheck( (g_Voice_Param.m_VocInputControl.m_u8AgcMode==1) ); 
	
	// g_Voice_Param . m_VocInputControl .m_u8EcEnable
	((CButton *)GetDlgItem ( IDC_CHECK_In_u8EcEnable ))->SetCheck(g_Voice_Param.m_VocInputControl.m_u8EcEnable); 

	// g_Voice_Param . m_VocInputControl .m_u8EcRefType
	((CButton *)GetDlgItem ( IDC_RADIO_In_Ref_Input ))->SetCheck( (g_Voice_Param.m_VocInputControl.m_u8EcRefType==0) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_In_Ref_Output ))->SetCheck( (g_Voice_Param.m_VocInputControl.m_u8EcRefType==1) ); 

	// g_Voice_Param . m_VocInputControl .m_u8VadEnable
	((CButton *)GetDlgItem ( IDC_CHECK_In_u8VadEnable ))->SetCheck(g_Voice_Param.m_VocInputControl.m_u8VadEnable); 

	// g_Voice_Param . m_VocInputControl .m_u8TadEnable
	((CButton *)GetDlgItem ( IDC_CHECK_In_u8TadEnable ))->SetCheck(g_Voice_Param.m_VocInputControl.m_u8TadEnable); 

	// g_Voice_Param . m_VocInputControl .m_u16FixGain
	sprintf ( TmpStr, "%d", g_Voice_Param.m_VocInputControl.m_u16FixGain );
	GetDlgItem ( IDC_EDIT_In_u16FixGain )->SetWindowText ( TmpStr );

	// g_Voice_Param . m_VocInputControl .m_u16EcRefCh
	sprintf ( TmpStr, "%d", g_Voice_Param.m_VocInputControl.m_u16EcRefCh );
	GetDlgItem ( IDC_EDIT_In_u16EcRefCh )->SetWindowText ( TmpStr );

	// ----- 
	// g_Voice_Param . m_VocOutputControl . m_u8AgcEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Out_u8AgcEnable ))->SetCheck(g_Voice_Param.m_VocOutputControl.m_u8AgcEnable); 

	// g_Voice_Param . m_VocOutputControl . m_u8AgcMode
	((CButton *)GetDlgItem ( IDC_RADIO_Out_ALS_Mode ))->SetCheck( (g_Voice_Param.m_VocOutputControl.m_u8AgcMode==0) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_Out_AGC_Mode ))->SetCheck( (g_Voice_Param.m_VocOutputControl.m_u8AgcMode==1) ); 

	// g_Voice_Param . m_VocOutputControl . m_u8OutputType
	((CButton *)GetDlgItem ( IDC_RADIO_Out_Silence ))->SetCheck( (g_Voice_Param.m_VocOutputControl.m_u8OutputType==0) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_Out_Input ))->SetCheck( (g_Voice_Param.m_VocOutputControl.m_u8OutputType==1) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_Out_Play ))->SetCheck( (g_Voice_Param.m_VocOutputControl.m_u8OutputType==2) ); 
	((CButton *)GetDlgItem ( IDC_RADIO_Out_Mixer ))->SetCheck( (g_Voice_Param.m_VocOutputControl.m_u8OutputType==3) ); 

	// g_Voice_Param . m_u16PlayFixGain
	sprintf ( TmpStr, "%d", g_Voice_Param.m_u16PlayFixGain );
	GetDlgItem ( IDC_EDIT_Play_u16FixGain )->SetWindowText ( TmpStr );

	// g_Voice_Param . m_VocOutputControl . m_u16FixGain
	sprintf ( TmpStr, "%d", g_Voice_Param.m_VocOutputControl.m_u16FixGain );
	GetDlgItem ( IDC_EDIT_Out_u16FixGain )->SetWindowText ( TmpStr );

	// g_Voice_Param . m_VocOutputControl . m_MixerControl
		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u8SRC1_Ctrl
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_NULL) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_INPUT) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_PLAY) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_CONF) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl == XMS_MIXER_FROM_IP) );

		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u8SRC2_Ctrl
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_NULL) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_INPUT) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_PLAY) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_CONF) );
		((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 ))->SetCheck ( 
			(g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl == XMS_MIXER_FROM_IP) );

		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u16SRC_ChID1
		sprintf ( TmpStr, "%d", g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID1 );
		GetDlgItem ( IDC_EDIT_Out_u16SRC_ChID1 )->SetWindowText ( TmpStr );
		
		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u16SRC_ChID2
		sprintf ( TmpStr, "%d", g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID2 );
		GetDlgItem ( IDC_EDIT_Out_u16SRC_ChID2 )->SetWindowText ( TmpStr );

	// ----- 
	// g_Voice_Param . m_VocGtdControl . m_u8ChannelEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8ChannelEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8ChannelEnable); 

	//g_Voice_Param . m_VocGtdControl . m_u8DTMFEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8DTMFEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8DTMFEnable); 

	//g_Voice_Param . m_VocGtdControl . m_u8MR2FEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8MR2FEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8MR2FEnable); 

	//g_Voice_Param . m_VocGtdControl . m_u8FskEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8MR2BEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8FSKEnable); 

	//g_Voice_Param . m_VocGtdControl . m_u8GTDEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8GTDEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8GTDEnable); 

	//g_Voice_Param . m_VocGtdControl . m_u8EXTEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8DPDEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8EXTEnable&EXT_ENABLE_DPD); 

	//g_Voice_Param . m_VocGtdControl . m_u8EXTEnable
	((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8PVDEnable ))->SetCheck(g_Voice_Param.m_VocGtdControl.m_u8EXTEnable&EXT_ENABLE_PVD); 

	//g_Voice_Param . m_VocGtdControl . m_u8GTDID
	//GetDlgItem ( IDC_EDIT_Gtd_u8GTDID )->SetWindowText (g_Voice_Param.m_VocGtdControl.m_u8GTDID);


	// handle enable disable
	HandleEnableDisable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMSApi_Test_ParamVoice::OnOK() 
{
	// TODO: Add extra validation here
	char	TmpStr[256];
	int		iTmpVal;

	// g_Voice_Param.m_u8PlayGainMode
	g_Voice_Param.m_u8PlayGainMode = ((CButton *)GetDlgItem ( IDC_CHECK_u8PlayGainMode ))->GetCheck(); 

	// g_Voice_Param . m_u8InputCtrlValid
	g_Voice_Param.m_u8InputCtrlValid = ((CButton *)GetDlgItem ( IDC_CHECK_u8InputCtrlValid ))->GetCheck(); 

	// g_Voice_Param . m_u8OutputCtrlValid
	g_Voice_Param.m_u8OutputCtrlValid = ((CButton *)GetDlgItem ( IDC_CHECK_u8OutputCtrlValid ))->GetCheck(); 

	// g_Voice_Param . m_u8GtdCtrlValid
	g_Voice_Param.m_u8GtdCtrlValid = ((CButton *)GetDlgItem ( IDC_CHECK_u8GtdCtrlValid ))->GetCheck(); 

	// ----- 
	// g_Voice_Parm.m_u16PlayFixGain
	GetDlgItem ( IDC_EDIT_Play_u16FixGain )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_Voice_Param.m_u16PlayFixGain = iTmpVal;

	// g_Voice_Param . m_VocInputControl .m_u8AgcEnable
	g_Voice_Param.m_VocInputControl.m_u8AgcEnable = ((CButton *)GetDlgItem ( IDC_CHECK_In_u8AgcEnable ))->GetCheck(); 

	// g_Voice_Param . m_VocInputControl .m_u8AgcMode
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_In_ALS_Mode ))->GetCheck() )
		g_Voice_Param.m_VocInputControl.m_u8AgcMode = 0;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_In_AGC_Mode ))->GetCheck() )
		g_Voice_Param.m_VocInputControl.m_u8AgcMode = 1; 
	
	// g_Voice_Param . m_VocInputControl .m_u8EcEnable
	g_Voice_Param.m_VocInputControl.m_u8EcEnable = ((CButton *)GetDlgItem ( IDC_CHECK_In_u8EcEnable ))->GetCheck();

	// g_Voice_Param . m_VocInputControl .m_u8EcRefType
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_In_Ref_Input ))->GetCheck() )
		g_Voice_Param.m_VocInputControl.m_u8EcRefType = 0; 
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_In_Ref_Output ))->GetCheck() )
		g_Voice_Param.m_VocInputControl.m_u8EcRefType = 1;

	// g_Voice_Param . m_VocInputControl .m_u8VadEnable
	g_Voice_Param.m_VocInputControl.m_u8VadEnable = ((CButton *)GetDlgItem ( IDC_CHECK_In_u8VadEnable ))->GetCheck(); 

	// g_Voice_Param . m_VocInputControl .m_u8TadEnable
	g_Voice_Param.m_VocInputControl.m_u8TadEnable = ((CButton *)GetDlgItem ( IDC_CHECK_In_u8TadEnable ))->GetCheck(); 

	// g_Voice_Param . m_VocInputControl .m_u16FixGain
	GetDlgItem ( IDC_EDIT_In_u16FixGain )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_Voice_Param.m_VocInputControl.m_u16FixGain = iTmpVal;

	// g_Voice_Param . m_VocInputControl .m_u16EcRefCh
	GetDlgItem ( IDC_EDIT_In_u16EcRefCh )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_Voice_Param.m_VocInputControl.m_u16EcRefCh = iTmpVal;

	// ----- 
	// g_Voice_Param . m_VocOutputControl . m_u8AgcEnable
	g_Voice_Param.m_VocOutputControl.m_u8AgcEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Out_u8AgcEnable ))->GetCheck(); 

	// g_Voice_Param . m_VocOutputControl . m_u8AgcMode
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Out_ALS_Mode ))->GetCheck() )
		g_Voice_Param.m_VocOutputControl.m_u8AgcMode = 0; 
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Out_AGC_Mode ))->GetCheck() )
		g_Voice_Param.m_VocOutputControl.m_u8AgcMode = 1; 

	// g_Voice_Param . m_VocOutputControl . m_u8OutputType
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Out_Silence ))->GetCheck() )
		g_Voice_Param.m_VocOutputControl.m_u8OutputType = 0; 
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Out_Input ))->GetCheck() )
		g_Voice_Param.m_VocOutputControl.m_u8OutputType = 1; 
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Out_Play ))->GetCheck() )
		g_Voice_Param.m_VocOutputControl.m_u8OutputType = 2; 
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Out_Mixer ))->GetCheck() )
		g_Voice_Param.m_VocOutputControl.m_u8OutputType = 3; 

	// g_Voice_Param . m_VocOutputControl . m_u16FixGain
	GetDlgItem ( IDC_EDIT_Out_u16FixGain )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_Voice_Param.m_VocOutputControl.m_u16FixGain = iTmpVal;

	// g_Voice_Param . m_VocOutputControl . m_MixerControl
		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u8SRC1_Ctrl
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_NULL;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT ))->GetCheck () )
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_CONF;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_IP;

		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u8SRC2_Ctrl
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_NULL;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 ))->GetCheck () )
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_PLAY;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_CONF;
		if ( ((CButton *)GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 ))->GetCheck () ) 
			g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_IP;

		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u16SRC_ChID1
		GetDlgItem ( IDC_EDIT_Out_u16SRC_ChID1 )->GetWindowText ( TmpStr, 20 );
		sscanf ( TmpStr, "%d", &iTmpVal );
		g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID1 = iTmpVal;
				
		// g_Voice_Param . m_VocOutputControl . m_MixerControl . m_u16SRC_ChID2
		GetDlgItem ( IDC_EDIT_Out_u16SRC_ChID2 )->GetWindowText ( TmpStr, 20 );
		sscanf ( TmpStr, "%d", &iTmpVal );
		g_Voice_Param.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID2 = iTmpVal;

	// ----- 
	// g_Voice_Param . m_VocGtdControl . m_u8ChannelEnable
	g_Voice_Param.m_VocGtdControl.m_u8ChannelEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8ChannelEnable ))->GetCheck(); 

	//g_Voice_Param . m_VocGtdControl . m_u8DTMFEnable
	g_Voice_Param.m_VocGtdControl.m_u8DTMFEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8DTMFEnable ))->GetCheck(); 

	//g_Voice_Param . m_VocGtdControl . m_u8MR2FEnable
	g_Voice_Param.m_VocGtdControl.m_u8MR2FEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8MR2FEnable ))->GetCheck(); 

	//g_Voice_Param . m_VocGtdControl . m_u8FskEnable
	g_Voice_Param.m_VocGtdControl.m_u8FSKEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8MR2BEnable ))->GetCheck(); 

	//g_Voice_Param . m_VocGtdControl . m_u8GTDEnable
	g_Voice_Param.m_VocGtdControl.m_u8GTDEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8GTDEnable ))->GetCheck(); 

	//g_Voice_Param . m_VocGtdControl . m_u8GTDEnable
	//g_Voice_Param.m_VocGtdControl.m_u8DPDEnable = ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8DPDEnable ))->GetCheck(); 
	
	//g_Voice_Param . m_VocGtdControl . m_u8GTDEnable
    g_Voice_Param.m_VocGtdControl.m_u8EXTEnable = 0;
	if ( ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8DPDEnable ))->GetCheck() )
		g_Voice_Param.m_VocGtdControl.m_u8EXTEnable |= EXT_ENABLE_DPD; 

	if ( ((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8PVDEnable ))->GetCheck() )
		g_Voice_Param.m_VocGtdControl.m_u8EXTEnable |= EXT_ENABLE_PVD; 

	memset(TmpStr,0,sizeof(TmpStr));
	GetDlgItem ( IDC_EDIT_Gtd_u8GTDID )->GetWindowText ((char*) &g_Voice_Param.m_VocGtdControl.m_u8GTDID[0], 20 );

	CDialog::OnOK();
}

void CXMSApi_Test_ParamVoice::HandleEnableDisable()
{
	bool	bNow;
	bool	bNowSub;

	// g_Voice_Param . m_u8InputCtrlValid
	bNow = (((CButton *)GetDlgItem ( IDC_CHECK_u8InputCtrlValid ))->GetCheck()==1);
	// ----- 
	bNowSub =  (((CButton *)GetDlgItem ( IDC_CHECK_In_u8AgcEnable ))->GetCheck()==1);
	GetDlgItem ( IDC_CHECK_In_u8AgcEnable )->EnableWindow(bNow);
	GetDlgItem ( IDC_RADIO_In_ALS_Mode )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_In_AGC_Mode )->EnableWindow(bNow && bNowSub);

	bNowSub =  (((CButton *)GetDlgItem ( IDC_CHECK_In_u8EcEnable ))->GetCheck()==1);
	GetDlgItem ( IDC_CHECK_In_u8EcEnable )->EnableWindow(bNow);
	GetDlgItem ( IDC_RADIO_In_Ref_Input )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_In_Ref_Output )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_EDIT_In_u16EcRefCh )->EnableWindow(bNow && bNowSub);

	GetDlgItem ( IDC_CHECK_In_u8VadEnable )->EnableWindow(bNow);
	GetDlgItem ( IDC_CHECK_In_u8TadEnable )->EnableWindow(bNow);
	GetDlgItem ( IDC_EDIT_In_u16FixGain )->EnableWindow(bNow);
		
	bNow = (((CButton *)GetDlgItem ( IDC_CHECK_u8PlayGainMode ))->GetCheck()==1);
	GetDlgItem ( IDC_EDIT_Play_u16FixGain )->EnableWindow(bNow);

	// g_Voice_Param . m_u8OutputCtrlValid
	bNow = (((CButton *)GetDlgItem ( IDC_CHECK_u8OutputCtrlValid ))->GetCheck()==1);
	// ----- 
	bNowSub =  (((CButton *)GetDlgItem ( IDC_CHECK_Out_u8AgcEnable ))->GetCheck()==1);
	GetDlgItem ( IDC_CHECK_Out_u8AgcEnable )->EnableWindow(bNow);
	GetDlgItem ( IDC_RADIO_Out_ALS_Mode )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_Out_AGC_Mode )->EnableWindow(bNow && bNowSub);

	GetDlgItem ( IDC_RADIO_Out_Silence )->EnableWindow(bNow);
	GetDlgItem ( IDC_RADIO_Out_Input )->EnableWindow(bNow);
	GetDlgItem ( IDC_RADIO_Out_Play )->EnableWindow(bNow);
	GetDlgItem ( IDC_RADIO_Out_Mixer )->EnableWindow(bNow);
	GetDlgItem ( IDC_EDIT_Out_u16FixGain )->EnableWindow(bNow);
	
	bNowSub =  (((CButton *)GetDlgItem ( IDC_RADIO_Out_Mixer ))->GetCheck()==1);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_NULL2 )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_INPUT2 )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_PLAY2 )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_CONF2 )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_RADIO_MIXER_FROM_RTP2 )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_EDIT_Out_u16SRC_ChID1 )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_EDIT_Out_u16SRC_ChID2 )->EnableWindow(bNow && bNowSub);

	// g_Voice_Param . m_u8GtdCtrlValid
	bNow = (((CButton *)GetDlgItem ( IDC_CHECK_u8GtdCtrlValid ))->GetCheck()==1);
	// ----- 	
	bNowSub =  (((CButton *)GetDlgItem ( IDC_CHECK_Gtd_u8ChannelEnable ))->GetCheck()==1);
	GetDlgItem ( IDC_CHECK_Gtd_u8ChannelEnable )->EnableWindow(bNow);
	GetDlgItem ( IDC_CHECK_Gtd_u8DTMFEnable )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_CHECK_Gtd_u8MR2FEnable )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_CHECK_Gtd_u8MR2BEnable )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_CHECK_Gtd_u8GTDEnable )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_CHECK_Gtd_u8DPDEnable )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_CHECK_Gtd_u8PVDEnable )->EnableWindow(bNow && bNowSub);
	GetDlgItem ( IDC_EDIT_Gtd_u8GTDID )->EnableWindow(bNow && bNowSub);
}

void CXMSApi_Test_ParamVoice::OnCHECKu8InputCtrlValid() 
{
	// TODO: Add your control notification handler code here

	HandleEnableDisable();
}

void CXMSApi_Test_ParamVoice::OnCHECKu8OutputCtrlValid() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnCHECKu8GtdCtrlValid() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnCHECKInu8AgcEnable() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnCHECKInu8EcEnable() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnCHECKOutu8AgcEnable() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnRADIOOutSilence() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnRADIOOutInput() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnRADIOOutPlay() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnRADIOOutConfSum() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnRADIOOutRtpInput() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnRADIOOutMixer() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnCHECKGtdu8ChannelEnable() 
{
	// TODO: Add your control notification handler code here
	HandleEnableDisable();
	
}

void CXMSApi_Test_ParamVoice::OnCHECKu8PlayGainMode() 
{
	HandleEnableDisable();
}

void CXMSApi_Test_ParamVoice::OnCHECKGtdu8MR2BEnable() 
{
	// TODO: Add your control notification handler code here
	
}
