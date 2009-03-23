// XMSApi_PlaySet.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_PlaySet.h"

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

extern PlayProperty_t		g_PlayProperty;

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_PlaySet dialog


CXMSApi_PlaySet::CXMSApi_PlaySet(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_PlaySet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_PlaySet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_PlaySet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_PlaySet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_PlaySet, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_PlaySet)
	ON_BN_CLICKED(IDC_RADIO_Normal_Stop, OnRADIONormalStop)
	ON_BN_CLICKED(IDC_RADIO_Single_Code, OnRADIOSingleCode)
	ON_BN_CLICKED(IDC_RADIO_Any_Code, OnRADIOAnyCode)
	ON_BN_CLICKED(IDC_RADIO_PLAY_TYPE_FILE, OnRadioPlayTypeFile)
	ON_BN_CLICKED(IDC_RADIO_PLAY_TYPE_INDEX, OnRadioPlayTypeIndex)
	ON_BN_CLICKED(IDC_RADIO_PLAY_FILE_NORMAL, OnRadioPlayFileNormal)
	ON_BN_CLICKED(IDC_RADIO_PLAY_FILE_QUEUE, OnRadioPlayFileQueue)
	ON_BN_CLICKED(IDC_RADIO_PLAY_TONE, OnRadioPlayTone)
	ON_BN_CLICKED(IDC_RADIO_PLAY_TYPE_FILE_QUEUE, OnRadioPlayTypeFileQueue)
	ON_BN_CLICKED(IDC_RADIO_PLAY_TYPE_INDEX_QUEUE, OnRadioPlayTypeIndexQueue)
	ON_EN_CHANGE(IDC_EDIT_Play_u32MaxTime, OnChangeEDITPlayu32MaxTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_PlaySet message handlers

BOOL CXMSApi_PlaySet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_Play_DevcieID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	//playpropetry . m_u32TaskID
	sprintf ( TmpStr, "%d", g_PlayProperty.m_u8TaskID );
	GetDlgItem ( IDC_EDIT_Play_u32TaskID )->SetWindowText ( TmpStr );
	
	//playpropetry . m_u32MaxSize
	sprintf ( TmpStr, "%d", g_PlayProperty.m_u32MaxSize );
	GetDlgItem ( IDC_EDIT_Play_u32MaxSize )->SetWindowText ( TmpStr );

	// playpropetry . m_u32MaxTime
	sprintf ( TmpStr, "%d", g_PlayProperty.m_u32MaxTime );
	GetDlgItem ( IDC_EDIT_Play_u32MaxTime )->SetWindowText ( TmpStr );

	//playpropetry . m_u8SrcMode
	if ( g_PlayProperty.m_u8SrcMode == XMS_SRC_8K )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_8K ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_8K ))->SetCheck(0);
	if ( g_PlayProperty.m_u8SrcMode == XMS_SRC_6K )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_6K ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_6K ))->SetCheck(0);
	if ( g_PlayProperty.m_u8SrcMode == XMS_SRC_GTG )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_GTG ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_GTG ))->SetCheck(0);
	if ( g_PlayProperty.m_u8SrcMode == XMS_SRC_FSK )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FSK ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FSK ))->SetCheck(0);
	if ( g_PlayProperty.m_u8SrcMode == XMS_SRC_FAX )
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FAX ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_SRC_FAX ))->SetCheck(0);

	//playpropetry . m_u8DecodeType
	if ( g_PlayProperty.m_u8DecodeType == XMS_Alaw_type )
		((CButton *)GetDlgItem ( IDC_RADIO_Alaw_type ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Alaw_type ))->SetCheck(0);
	if ( g_PlayProperty.m_u8DecodeType == XMS_Ulaw_type )
		((CButton *)GetDlgItem ( IDC_RADIO_Ulaw_type ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Ulaw_type ))->SetCheck(0);
	if ( g_PlayProperty.m_u8DecodeType == XMS_Vox_type )
		((CButton *)GetDlgItem ( IDC_RADIO_Vox_type ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Vox_type ))->SetCheck(0);
	if ( g_PlayProperty.m_u8DecodeType == XMS_Linear_8bit )
		((CButton *)GetDlgItem ( IDC_RADIO_Linear_8bit ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Linear_8bit ))->SetCheck(0);
	if ( g_PlayProperty.m_u8DecodeType == XMS_Linear_16bit )
		((CButton *)GetDlgItem ( IDC_RADIO_Play_Linear_16bit ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Play_Linear_16bit ))->SetCheck(0);

	//playpropetry . m_u32StopMode
	if ( g_PlayProperty.m_u32StopMode == XMS_Normal_Stop )
		((CButton *)GetDlgItem ( IDC_RADIO_Normal_Stop ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Normal_Stop ))->SetCheck(0);
	if ( g_PlayProperty.m_u32StopMode == XMS_Single_Code )
		((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->SetCheck(0);
	if ( g_PlayProperty.m_u32StopMode == XMS_Any_Code )
		((CButton *)GetDlgItem ( IDC_RADIO_Any_Code ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_Any_Code ))->SetCheck(0);

	//playpropetry . m_u32StopCode
	sprintf ( TmpStr, "%d", g_PlayProperty.m_u32StopCode );
	GetDlgItem ( IDC_EDIT_Play_u32StopCode )->SetWindowText ( TmpStr );

	//playpropetry . m_u16FileOffset
	sprintf ( TmpStr, "%d", g_PlayProperty.m_u16FileOffset );
	GetDlgItem ( IDC_EDIT_Play_u16FileOffset )->SetWindowText ( TmpStr );

	//playpropetry . m_u16PlayType
	if ( g_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_FILE )
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE ))->SetCheck(0);
	if ( g_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_INDEX )
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX ))->SetCheck(0);
	if ( g_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_FILE_QUEUE )
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE_QUEUE ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE_QUEUE ))->SetCheck(0);
	if ( g_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_INDEX_QUEUE )
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX_QUEUE ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX_QUEUE ))->SetCheck(0);

	//playpropetry . m_u16PlayIndex
	sprintf ( TmpStr, "%d", g_PlayProperty.m_u16PlayIndex );
	GetDlgItem ( IDC_EDIT_Play_u16PlayIndex )->SetWindowText ( TmpStr );

	//playpropetry . m_s8PlayContext
	GetDlgItem ( IDC_EDIT_Play_s8PlayContent )->SetWindowText (g_PlayProperty.m_s8PlayContent);

	// -------------------------------------------------------------------------------
	// Handle Enable Disable
	HandleEnableDisable ();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMSApi_PlaySet::OnOK() 
{
	// TODO: Add extra validation here
	char	TmpStr[256];
	int		iTmpVal;

	//playpropetry . m_u32MaxSize
	GetDlgItem ( IDC_EDIT_Play_u32MaxSize )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_PlayProperty.m_u32MaxSize );

	// playpropetry . m_u32MaxTime
	GetDlgItem ( IDC_EDIT_Play_u32MaxTime )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_PlayProperty.m_u32MaxTime );

	//playpropetry . m_u8SrcMode
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_8K ))->GetCheck() )
		g_PlayProperty.m_u8SrcMode = XMS_SRC_8K;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_6K ))->GetCheck() )
		g_PlayProperty.m_u8SrcMode = XMS_SRC_6K;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_GTG ))->GetCheck() )
		g_PlayProperty.m_u8SrcMode = XMS_SRC_GTG;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_FSK ))->GetCheck() )
		g_PlayProperty.m_u8SrcMode = XMS_SRC_FSK;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_SRC_FAX ))->GetCheck() )
		g_PlayProperty.m_u8SrcMode = XMS_SRC_FAX;

	//playpropetry . m_u8DecodeType
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Alaw_type ))->GetCheck() )
		g_PlayProperty.m_u8DecodeType = XMS_Alaw_type;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Ulaw_type ))->GetCheck() )
		g_PlayProperty.m_u8DecodeType = XMS_Ulaw_type;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Vox_type ))->GetCheck() )
		g_PlayProperty.m_u8DecodeType = XMS_Vox_type;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Linear_8bit ))->GetCheck() )
		g_PlayProperty.m_u8DecodeType = XMS_Linear_8bit;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Play_Linear_16bit ))->GetCheck() )
		g_PlayProperty.m_u8DecodeType = XMS_Linear_16bit;

	//playpropetry . m_u32StopMode
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Normal_Stop ))->GetCheck() )
		g_PlayProperty.m_u32StopMode = XMS_Normal_Stop;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->GetCheck() )
		g_PlayProperty.m_u32StopMode = XMS_Single_Code;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_Any_Code ))->GetCheck() )
		g_PlayProperty.m_u32StopMode = XMS_Any_Code;

	//playpropetry . m_u32StopCode
	GetDlgItem ( IDC_EDIT_Play_u32StopCode )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_PlayProperty.m_u32StopCode );

	//playpropetry . m_u16FileOffset
	GetDlgItem ( IDC_EDIT_Play_u16FileOffset )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_PlayProperty.m_u16FileOffset = iTmpVal;

	//playpropetry . m_u16PlayType
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE ))->GetCheck() )
		g_PlayProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX ))->GetCheck() )
		g_PlayProperty.m_u16PlayType = XMS_PLAY_TYPE_INDEX;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE_QUEUE ))->GetCheck() )
		g_PlayProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE_QUEUE;
	if ( ((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX_QUEUE ))->GetCheck() )
		g_PlayProperty.m_u16PlayType = XMS_PLAY_TYPE_INDEX_QUEUE;

	//playpropetry . m_u16PlayIndex
	GetDlgItem ( IDC_EDIT_Play_u16PlayIndex )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	g_PlayProperty.m_u16PlayIndex = iTmpVal;

	// -------------------------------------------------------------------------------
	
	CDialog::OnOK();
}

void CXMSApi_PlaySet::OnRADIONormalStop() 
{
	// TODO: Add your control notification handler code here
	
	// Handle Enable Disable
	HandleEnableDisable ();
}

void CXMSApi_PlaySet::OnRADIOSingleCode() 
{
	// TODO: Add your control notification handler code here
	
	// Handle Enable Disable
	HandleEnableDisable ();
}

void CXMSApi_PlaySet::OnRADIOAnyCode() 
{
	// TODO: Add your control notification handler code here
	
	// Handle Enable Disable
	HandleEnableDisable ();
}

void CXMSApi_PlaySet::OnRadioPlayTypeFile() 
{
	// TODO: Add your control notification handler code here

	// Handle Enable Disable
	HandleEnableDisable ();	
}

void CXMSApi_PlaySet::OnRadioPlayTypeIndex() 
{
	// TODO: Add your control notification handler code here
	
	// Handle Enable Disable
	HandleEnableDisable ();	
}

void CXMSApi_PlaySet::HandleEnableDisable()
{
	Enable_File (true);
}

void CXMSApi_PlaySet::Enable_File(bool IsEnable)
{
	bool	bNow;

	// handle all
	//playpropetry . m_u32TaskID
	GetDlgItem ( IDC_EDIT_Play_u32TaskID )->EnableWindow(IsEnable);
	
	//playpropetry . m_u32MaxSize
	GetDlgItem ( IDC_EDIT_Play_u32MaxSize )->EnableWindow(IsEnable);

	// playpropetry . m_u32MaxTime
	GetDlgItem ( IDC_EDIT_Play_u32MaxTime )->EnableWindow(IsEnable);

	//playpropetry . m_u8SrcMode
	GetDlgItem ( IDC_RADIO_SRC_8K )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_SRC_6K )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_SRC_GTG )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_SRC_FSK )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_SRC_FAX )->EnableWindow(IsEnable);

	//playpropetry . m_u8DecodeType
	GetDlgItem ( IDC_RADIO_Alaw_type )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_Ulaw_type )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_Vox_type )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_Linear_8bit )->EnableWindow(IsEnable);

	//playpropetry . m_u32StopMode
	GetDlgItem ( IDC_RADIO_Normal_Stop )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_Single_Code )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_Any_Code )->EnableWindow(IsEnable);

	//playpropetry . m_u32StopCode
	GetDlgItem ( IDC_EDIT_Play_u32StopCode )->EnableWindow(IsEnable);

	//playpropetry . m_u16FileOffset
	GetDlgItem ( IDC_EDIT_Play_u16FileOffset )->EnableWindow(IsEnable);

	//playpropetry . m_u16PlayType
	GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_PLAY_TYPE_FILE_QUEUE )->EnableWindow(IsEnable);
	GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX_QUEUE )->EnableWindow(IsEnable);

	//playpropetry . m_u16PlayIndex
	GetDlgItem ( IDC_EDIT_Play_u16PlayIndex )->EnableWindow(IsEnable);

	// handle some special
	if ( IsEnable )
	{
		// Enable/Disable : m_u32StopMode
		bNow = (((CButton *)GetDlgItem ( IDC_RADIO_Single_Code ))->GetCheck()==1);
		GetDlgItem ( IDC_EDIT_Play_u32StopCode )->EnableWindow(bNow);

		bNow = (((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX ))->GetCheck()==1) 
			|| (((CButton *)GetDlgItem ( IDC_RADIO_PLAY_TYPE_INDEX_QUEUE ))->GetCheck()==1);
		GetDlgItem ( IDC_EDIT_Play_u16PlayIndex )->EnableWindow(bNow);
	}
}

void CXMSApi_PlaySet::Enable_Tone(bool IsEnable)
{
}

void CXMSApi_PlaySet::OnRadioPlayFileNormal() 
{
	// TODO: Add your control notification handler code here
	
	// Handle Enable Disable
	HandleEnableDisable ();	
}

void CXMSApi_PlaySet::OnRadioPlayFileQueue() 
{
	// TODO: Add your control notification handler code here
	// Handle Enable Disable
	HandleEnableDisable ();	
	
}

void CXMSApi_PlaySet::OnRadioPlayTone() 
{
	// TODO: Add your control notification handler code here
	// Handle Enable Disable
	HandleEnableDisable ();	
	
}

void CXMSApi_PlaySet::OnRadioPlayTypeFileQueue() 
{
	// TODO: Add your control notification handler code here
	// Handle Enable Disable
	HandleEnableDisable ();	
	
}

void CXMSApi_PlaySet::OnRadioPlayTypeIndexQueue() 
{
	// TODO: Add your control notification handler code here
	// Handle Enable Disable
	HandleEnableDisable ();	
	
}

void CXMSApi_PlaySet::OnChangeEDITPlayu32MaxTime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	//HandleCheckPlayTime();

	CString playtime;

	//memset(buff,0,sizeof(buff));
	GetDlgItem(IDC_EDIT_Play_u32MaxTime)->GetWindowText(playtime) ;
	if( atoi((char*)(LPCSTR)playtime) < 0 )
		GetDlgItem(IDC_EDIT_Play_u32MaxTime)->SetWindowText("0");
	
	
}
