#include "stdafx.h"
#include "XMS_Dial.h"
#include "XMS_DialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "DJAcsISUPDef.h"
#include "XMS_Dial_Sub.H"
#include "XMS_Dial_String.H"
#include "XMS_Dial_Event.H"

// Special code for CAS(SS1)
#include "..\CAS_Common_Code\CAS_Common_Cfg.h"		
CmdParamData_CAS_t g_Param_CAS;
// end of code for CAS(SS1)

// Special code for Analog
#include "..\Analog_Common_Code\Analog_Common_Code.h"		
TYPE_ANALOG_GTD_PARAM g_Param_Analog;

void	SetGTD_ToneParam ( DeviceID_t *pDevice );
// end of code for Analog

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_DialApp theApp;
CXMS_DialDlg	*pdlg;

// var about XMS_Dial.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Dial.INI";

ServerID_t		cfg_ServerID;
int				cfg_iCalledLen = 5;
char			cfg_CallingNum[32] = "83636988";

char			cfg_VocPath[128] = "";	// no use in XMS_Dial
int				cfg_iDispChnl = 32;		// always disp 32 channel
int				cfg_iVoiceRule = 0;		// search voice in Fix Relationship

int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;

int				cfg_iCallOutRule = 0;
char			cfg_SimCalledNum[32] = "12345";

int				cfg_s32DebugOn;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 1;

// var in XMS_Dail_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalPcm;
extern int						g_iTotalPcmOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM_IN_THIS_DEMO];

extern int						g_iTotalTrunk;
extern int						g_iTotalTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalUser;
extern int						g_iTotalUserOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_User[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalVoice;
extern int						g_iTotalVoiceOpened;
extern int						g_iTotalVoiceFree;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalModule;
extern DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];
// -----------------------------------------------------------------------
// local function decalration
void	WriteToConfig(void);
void	ReadFromConfig(void);
void	InitTextBox(void);
void	FetchFromText();

void	InitListMain(void);
void	InitListPcm(void);
void	InitListMsg(void);

// -----------------------------------------------------------------------
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );

	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);

	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);

	GetPrivateProfileString("ConfigInfo","UserName","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);

	cfg_iCalledLen = GetPrivateProfileInt ( "ConfigInfo", "CalledLen", 5, cfg_IniName);
	if ( cfg_iCalledLen > 20 )
		cfg_iCalledLen = 20;

	GetPrivateProfileString ( "ConfigInfo", "CallingNum", "83636988", cfg_CallingNum, sizeof(cfg_CallingNum), cfg_IniName);

	cfg_iCallOutRule = GetPrivateProfileInt ( "ConfigInfo", "CallOutRule", 0, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "SimCalledRNum", "12345", cfg_SimCalledNum, sizeof(cfg_SimCalledNum), cfg_IniName);
	
	cfg_iPartWork = GetPrivateProfileInt ( "ConfigInfo", "PartWork", 0, cfg_IniName);

	cfg_iPartWorkModuleID = GetPrivateProfileInt ( "ConfigInfo", "PartWorkModuleID", 0, cfg_IniName);

	cfg_s32DebugOn = GetPrivateProfileInt ( "ConfigInfo", "DebugOn", 0, cfg_IniName);
}

void	WriteToConfig(void)
{
	char	TmpStr[256];

	WritePrivateProfileString ( "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName);
	
	sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	WritePrivateProfileString ( "ConfigInfo", "Port", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iCalledLen);
	WritePrivateProfileString ( "ConfigInfo", "CalledLen", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CallingNum", cfg_CallingNum, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iCallOutRule);
	WritePrivateProfileString ( "ConfigInfo", "CallOutRule", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "SimCalledRNum", cfg_SimCalledNum, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iPartWork);
	WritePrivateProfileString ( "ConfigInfo", "PartWork", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iPartWorkModuleID);
	WritePrivateProfileString ( "ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_s32DebugOn);
	WritePrivateProfileString ( "ConfigInfo", "DebugOn", TmpStr, cfg_IniName);
}

void	InitTextBox(void)
{
	char	TmpStr[256];

	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->SetWindowText ( cfg_ServerID.m_s8ServerIp );

	sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort );
	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->SetWindowText ( TmpStr );

	sprintf ( TmpStr, "%d", cfg_iCalledLen );
	pdlg->GetDlgItem ( IDC_EDIT_CalledLen )->SetWindowText ( TmpStr );

	pdlg->GetDlgItem ( IDC_EDIT_CallingNum )->SetWindowText ( cfg_CallingNum );

	if ( cfg_iCallOutRule == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_Sequence))->SetCheck ( 1 ) ;
	else if ( cfg_iCallOutRule == 1 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_Loop))->SetCheck ( 1 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_ByDtmf))->SetCheck ( 1 ) ;

	pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum )->SetWindowText ( cfg_SimCalledNum );

	if ( cfg_iPartWork == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_PartWork))->SetCheck ( 0 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_PartWork))->SetCheck ( 1 ) ;

	sprintf ( TmpStr, "%d", cfg_iPartWorkModuleID );
	pdlg->GetDlgItem ( IDC_EDIT_ModuleID )->SetWindowText ( TmpStr );

	if ( cfg_s32DebugOn == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 0 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 1 ) ;
}

void	FetchFromText(void)
{
	char	TmpStr[256];

	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->GetWindowText ( cfg_ServerID.m_s8ServerIp, 30 );

	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_ServerID.m_u32ServerPort);

	pdlg->GetDlgItem ( IDC_EDIT_CalledLen )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_iCalledLen );

	pdlg->GetDlgItem ( IDC_EDIT_CallingNum )->GetWindowText ( cfg_CallingNum, 30 );

	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Sequence))->GetCheck ( ) )
		 cfg_iCallOutRule = 0;
	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Loop))->GetCheck ( ) )
		 cfg_iCallOutRule = 1;
	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_ByDtmf))->GetCheck ( ) ) 
		 cfg_iCallOutRule = 2;
		
	pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum )->GetWindowText ( cfg_SimCalledNum, 30 );

	if ( ((CButton *)pdlg->GetDlgItem (IDC_CHECK_PartWork))->GetCheck ( ) )
		cfg_iPartWork = 1;
	else
		cfg_iPartWork = 0;

	pdlg->GetDlgItem ( IDC_EDIT_ModuleID )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_iPartWorkModuleID);

	if ( ((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->GetCheck ( ) )
		cfg_s32DebugOn = 1;
	else
		cfg_s32DebugOn = 0;
}

// --------------------------------------------------------------------------------
void	InitListMsg(void)
{
	int	i = pdlg->m_ListMsg.GetHorizontalExtent ( );

	pdlg->m_ListMsg.SetHorizontalExtent ( i + 1000 );

	

}

void AddMsg ( char *str)
{
	static	int		iTotal_ListMsg = 0;
	char		TmpStr[256];

	sprintf ( TmpStr, "%3d: ", iTotal_ListMsg+1 );
	strcat ( TmpStr, str );
	pdlg->m_ListMsg.AddString ( TmpStr );

//	pdlg->m_ListMsg.SetCurSel ( iTotal_ListMsg);
		
	iTotal_ListMsg ++;
}

// --------------------------------------------------------------------------------
void	InitListMain(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListMain.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListMain.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListMain.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListMain.InsertColumn ( 4, &lvc ); 

	lvc.iSubItem = 5;
	lvc.pszText = "Called" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 5, &lvc ); 

	lvc.iSubItem = 6;
	lvc.pszText = "Calling" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "LinkDev" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "FailReason" ;
	lvc.cx = 75;
	pdlg->m_ListMain.InsertColumn ( 8, &lvc ); 
}

int		CalDispRow ( int iSeqID )
{
	int	iRet;

	if ( cfg_iDispChnl == 30 )
	{
		if ( (iSeqID % 16) == 0 )
			return -1;
	
		iRet = (iSeqID / 32) * 30 + (iSeqID % 32);
		iRet --;
		if ( (iSeqID % 32) > 16 )
			iRet --;
		return iRet;

	}

	if ( cfg_iDispChnl == 31 )
	{
		if ( (iSeqID % 32) == 0 )
			return -1;

		iRet = (iSeqID / 32) * 31 + (iSeqID % 32);
		iRet --;
		return iRet;
	}

	return iSeqID;
}

void DrawMain_LineState( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	// 
	GetString_LineState ( StateStr, pOneTrunk->iLineState );
	pdlg->m_ListMain.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawMain_State( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	//
	switch( pOneTrunk->State ) 
	{
	case TRK_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case TRK_FREE:		
		strcpy(StateStr,"Free"); 
		break ;

	case TRK_NOTHANDLE:
		strcpy(StateStr,"NotHandle");
		break ;

	case TRK_CALLOUT:
		strcpy(StateStr,"CallOut");
		break;

	case TRK_SIM_CALLOUT	:
		strcpy(StateStr,"SimCallOut");
		break;
	case TRK_SIM_LINK:
		strcpy(StateStr,"SimLinkOk");
		break;

	case TRK_SIM_ANALOG_OFFHOOK	:
		strcpy(StateStr,"OffHook");
		break;

	case TRK_SIM_ANALOG_DIALING	:
		strcpy(StateStr,"Dialing");
		break;

	case TRK_LINK:
		strcpy(StateStr, "Link");
		break;

	case TRK_PLAYDTMF:
		strcpy(StateStr, "PlayDtmf");
		break;

	case TRK_HANGUP:
		strcpy(StateStr, "HangUp");
		break;
		
	case TRK_FAIL:
		strcpy(StateStr,"FAIL");
		break;

	case TRK_WAIT_REMOVE:
		strcpy(StateStr,"Remove DSP");
		break;

	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListMain.SetItemText ( iDispRow, 4, StateStr ); 
}

void DrawMain_CallInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	strcpy ( StateStr, pOneTrunk->CalleeCode );
	pdlg->m_ListMain.SetItemText ( iDispRow, 5, StateStr ); 

	strcpy ( StateStr, pOneTrunk->CallerCode );
	pdlg->m_ListMain.SetItemText ( iDispRow, 6, StateStr ); 
}

void DrawMain_LinkDev( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	if ( pOneTrunk->LinkDevID.m_s16DeviceMain != 0 )
		sprintf ( StateStr, "%d,%d", pOneTrunk->LinkDevID.m_s8ModuleID, pOneTrunk->LinkDevID.m_s16ChannelID );
	else
		sprintf ( StateStr, "" );
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 
}

void DrawMain_Reason ( TRUNK_STRUCT *pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	sprintf ( StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause ); 
	pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 
}

void DrawMain_Analog_Reason ( TRUNK_STRUCT *pOneTrunk, char *ErrStr )
{
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	pdlg->m_ListMain.SetItemText ( iDispRow, 8, ErrStr ); 
}

// -------------------------------------------------------------------------------------------------
void	InitListPcm(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;

	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListPcm.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "PcmID" ;
	lvc.cx = 60;
	pdlg->m_ListPcm.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "Type" ;
	lvc.cx = 80;
	pdlg->m_ListPcm.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "Alarm Value" ;
	lvc.cx = 80;
	pdlg->m_ListPcm.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "Sig Detect" ;
	lvc.cx = 80;
	pdlg->m_ListPcm.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "Frame Sync" ;
	lvc.cx = 80;

	pdlg->m_ListPcm.InsertColumn ( 4, &lvc ); 
	lvc.iSubItem = 5;
	lvc.pszText = "Remote Alarm" ;
	lvc.cx = 85;
	pdlg->m_ListPcm.InsertColumn ( 5, &lvc ); 
}

void DrawPcm_TypeAndAlarm ( PCM_STRUCT *pOnePcm )
{
	char StateStr[100];
	int	 iDispRow;
	int	 AlarmVal;

	iDispRow = pOnePcm->iSeqID; 

	AlarmVal = pOnePcm->s32AlarmVal;

	// E1 Type
	sprintf ( StateStr, "%s", GetString_PcmType (pOnePcm->u8E1Type) );
	pdlg->m_ListPcm.SetItemText ( iDispRow, 1, StateStr ); 

	// Alarm
	sprintf ( StateStr, "0x%X", AlarmVal );
	pdlg->m_ListPcm.SetItemText ( iDispRow, 2, StateStr ); 

	if ( AlarmVal & XMS_E1PORT_MASK_LOST_SIGNAL )
		pdlg->m_ListPcm.SetItemText ( iDispRow, 3, " X" ); 
	else
		pdlg->m_ListPcm.SetItemText ( iDispRow, 3, "O" ); 

	if ( AlarmVal & XMS_E1PORT_MASK_FAS_ALARM )
		pdlg->m_ListPcm.SetItemText ( iDispRow, 4, " X" ); 
	else
		pdlg->m_ListPcm.SetItemText ( iDispRow, 4, "O" ); 

	if ( AlarmVal & XMS_E1PORT_MASK_REMOTE_ALARM )
		pdlg->m_ListPcm.SetItemText ( iDispRow, 5, " X" ); 
	else
		pdlg->m_ListPcm.SetItemText ( iDispRow, 5, "O" ); 
	
}

// -------------------------------------------------------------------------------------------------
void	InitListUser(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListUser.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListUser.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListUser.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListUser.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListUser.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListUser.InsertColumn ( 4, &lvc ); 

	lvc.iSubItem = 5;
	lvc.pszText = "DTMF" ;
	lvc.cx = 60;
	pdlg->m_ListUser.InsertColumn ( 5, &lvc ); 

	lvc.iSubItem = 6;
	lvc.pszText = "VocInfo" ;
	lvc.cx = 70;
	pdlg->m_ListUser.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "LinkDev" ;
	lvc.cx = 70;
	pdlg->m_ListUser.InsertColumn ( 7, &lvc ); 
}

void DrawUser_State( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = pOneUser->iUserSeqID; 

	switch( pOneUser->UserState ) 
	{
	case USER_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case USER_FREE:		
		strcpy(StateStr,"Free"); 
		break ;

	case USER_OFFHOOK:
		strcpy(StateStr,"OffHook"); 
		break ;
				
	case USER_CALLOUT:
		strcpy(StateStr,"CallOut"); 
		break ;

	case USER_WAITHANGUP:
		strcpy(StateStr,"WaitHangUp"); 
		break ;

	case USER_LINK:
		strcpy(StateStr,"Link"); 
		break;

	case USER_WAIT_REMOVE:
		strcpy(StateStr,"Remove DSP");
		break;

	case USER_RING:
		strcpy(StateStr,"Ring");
		break;

	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListUser.SetItemText ( iDispRow, 4, StateStr ); 
}

void DrawUser_LineState( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = pOneUser->iUserSeqID; 

	// 
	GetString_LineState ( StateStr, pOneUser->iLineState );
	pdlg->m_ListUser.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawUser_VocInfo( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;

	iDispRow = pOneUser->iUserSeqID; 

	if ( pOneUser->VocDevID.m_s16DeviceMain != -1 )
	{
		pDev = &pOneUser->VocDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}
	else
		strcpy(StateStr, "");

	pdlg->m_ListUser.SetItemText ( iDispRow, 6, StateStr ); 

}

void DrawUser_DTMF( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = pOneUser->iUserSeqID;

	strcpy(StateStr,pOneUser->DtmfBuf);
	pdlg->m_ListUser.SetItemText ( iDispRow, 5, StateStr ); 

}

void DrawUser_LinkDev( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = pOneUser->iUserSeqID;

	if ( pOneUser->VocDevID.m_s16DeviceMain != -1 )
		sprintf ( StateStr, "%d,%d", pOneUser->LinkDevID.m_s8ModuleID, pOneUser->LinkDevID.m_s16ChannelID );
	else
		strcpy(StateStr, "");

	pdlg->m_ListUser.SetItemText ( iDispRow, 7, StateStr ); 

}

// -------------------------------------------------------------------------------------------------
void	InitListCount(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListCount.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 40;
	pdlg->m_ListCount.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "ModID" ;
	lvc.cx = 45;
	pdlg->m_ListCount.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "Pcm/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "Trk/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "Voc/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 4, &lvc ); 

	lvc.iSubItem = 5;
	lvc.pszText = "VocFree" ;
	lvc.cx = 55;
	pdlg->m_ListCount.InsertColumn ( 5, &lvc ); 

	lvc.iSubItem = 6;
	lvc.pszText = "User/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "ErrFlag" ;
	lvc.cx = 60;
	pdlg->m_ListCount.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "RemoveState" ;
	lvc.cx = 80;
	pdlg->m_ListCount.InsertColumn ( 8, &lvc ); 
}

void	DrawCount_Pcm ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d /%3d", AllDeviceRes[s8ModID].lPcmNum, AllDeviceRes[s8ModID].lPcmOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 2, TmpStr );

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d /%3d", g_iTotalPcm, g_iTotalPcmOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 2, TmpStr );

}

void	DrawCount_Trunk ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lTrunkNum, AllDeviceRes[s8ModID].lTrunkOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 3, TmpStr );

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalTrunk, g_iTotalTrunkOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 3, TmpStr );
}

void	DrawCount_Voc ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lVocNum, AllDeviceRes[s8ModID].lVocOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 4, TmpStr );

	sprintf ( TmpStr, "%3d", AllDeviceRes[s8ModID].lVocFreeNum );
	pdlg->m_ListCount.SetItemText ( iDispRow, 5, TmpStr );

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalVoice, g_iTotalVoiceOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 4, TmpStr );

	sprintf ( TmpStr, "%3d", g_iTotalVoiceFree );
	pdlg->m_ListCount.SetItemText ( iDispRow, 5, TmpStr );
}

void	DrawCount_User ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalUser, g_iTotalUserOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 6, TmpStr );
}

void	DrawCount_ErrFlag ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	if ( AllDeviceRes[s8ModID].bErrFlag )
		sprintf ( TmpStr, "T" );
	else
		sprintf ( TmpStr, "" );

	pdlg->m_ListCount.SetItemText ( iDispRow, 7, TmpStr );
}

void	DrawCount_RemoveState ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	switch ( AllDeviceRes[s8ModID].RemoveState )
	{
	case DSP_REMOVE_STATE_NONE:
		sprintf ( TmpStr, "" );
		break;
	case DSP_REMOVE_STATE_START:
		sprintf ( TmpStr, "Start" );
		break;
	case DSP_REMOVE_STATE_READY:
		sprintf ( TmpStr, "Ready" );
		break;
	}

	pdlg->m_ListCount.SetItemText ( iDispRow, 8, TmpStr );
}

// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;

	// m_ListMain
	pdlg->m_ListMain.DeleteAllItems ();
	for ( i = 0; i < g_iTotalTrunk; i ++ )
	{
		iDispRow = CalDispRow ( i ); 
		if ( iDispRow < 0 )	continue;

		// Fix content
		sprintf ( TmpStr, "%3d", iDispRow );
		pdlg->m_ListMain.InsertItem ( iDispRow, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_Trunk[i].m_s8ModuleID, MapTable_Trunk[i].m_s16ChannelID );
		pdlg->m_ListMain.SetItemText ( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk(MapTable_Trunk[i]);
		pdlg->m_ListMain.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );

		// Changed content
		DrawMain_LineState ( pOneTrunk );
		DrawMain_State ( pOneTrunk );
		DrawMain_CallInfo (pOneTrunk );
	}

	// m_ListPcm
	pdlg->m_ListPcm.DeleteAllItems ();
	for ( i = 0; i < g_iTotalPcm; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListPcm.InsertItem ( i, TmpStr );

		// Changed content
		DrawPcm_TypeAndAlarm ( &M_OnePcm(MapTable_Pcm[i]) );
	}

	// m_ListUser
	pdlg->m_ListUser.DeleteAllItems ();
	for ( i = 0; i < g_iTotalUser; i ++ )
	{
		iDispRow = i;

		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListUser.InsertItem ( iDispRow, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_User[i].m_s8ModuleID, MapTable_User[i].m_s16ChannelID );
		pdlg->m_ListUser.SetItemText ( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk(MapTable_User[i]);
		pdlg->m_ListUser.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );

		DrawUser_LineState ( pOneTrunk );
		DrawUser_State ( pOneTrunk );
	}

	// m_ListCount
	pdlg->m_ListCount.DeleteAllItems ();
	// Insert Row
	pdlg->m_ListCount.InsertItem ( 0, "Total" );
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListCount.InsertItem ( i+1, TmpStr );

		sprintf ( TmpStr, "%3d", MapTable_Module[i] );
		pdlg->m_ListCount.SetItemText ( i+1, 1, TmpStr );
	}

	// Display Content
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		DrawCount_Pcm ( MapTable_Module[i] );
		DrawCount_Trunk ( MapTable_Module[i] );
		DrawCount_Voc ( MapTable_Module[i] );
		DrawCount_ErrFlag ( MapTable_Module[i] );
		DrawCount_RemoveState ( MapTable_Module[i] );
	}
}


/********************************************************************************
Function:		���������豸GTDʹ��(ʹ�ܼ��Tone��/FSK/DTMF/PVD)
Input:			DeviceID_t* pDevId				�����豸DeviceID
Output:			Void
Description:	
********************************************************************************/
void SetGtd_AnalogTrunk(DeviceID_t* pDevId)
{
	//========Set GTG Begin========
				
	CmdParamData_Voice_t cmdVoc;
	
	memset(&cmdVoc,0,sizeof(cmdVoc));

	cmdVoc.m_u8GtdCtrlValid = 1 ; //Enable GTD
	cmdVoc.m_VocGtdControl.m_u8ChannelEnable = 1;//Enable Gtd channel
	cmdVoc.m_VocGtdControl.m_u8DTMFEnable = 1;
	cmdVoc.m_VocGtdControl.m_u8MR2FEnable = 1;
	cmdVoc.m_VocGtdControl.m_u8MR2BEnable = 1;
	cmdVoc.m_VocGtdControl.m_u8GTDEnable = 1;
	cmdVoc.m_VocGtdControl.m_u8FSKEnable = 1;
	
	cmdVoc.m_VocGtdControl.m_u8EXTEnable = 0x2;		// Enable PVD Detect

	strcpy((char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0],"GHIJK");

	DJ_U16 u16ParamType = VOC_PARAM_UNIPARAM;
	DJ_U16 u16ParamSize = sizeof(cmdVoc);
	void* p = (void*) &cmdVoc;

	int r = XMS_ctsSetParam( g_acsHandle,pDevId,u16ParamType,u16ParamSize,(void*)p);
	
	char szbuffer[1024];
	memset(szbuffer,0,sizeof(szbuffer));
	sprintf(szbuffer,"Set GTD ret = %d\n",r);
	AddMsg ( szbuffer );

	//========Set GTG End  ========
}

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_DialDlg	*)theApp.m_pMainWnd;

	// Read From "XMS_Dial.INI"
	ReadFromConfig();

	// Special code for CAS(SS1)
	// Read From "C:\\DJKeygoe\\Samples\\CAS_Common_Code\\SaXMS_CAS_Cfg.INI"
	if ( CAS_Common_Cfg_ReadCfg ( &g_Param_CAS ) != 0 )
	{
		sprintf ( MsgStr, "CAS_Common_Cfg_ReadCfg \"C:\\DJKeygoe\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI\" Error!" );
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
	}
	// end of code for CAS(SS1)

	// Special code for Analog
	// Read From "C:\\DJKeygoe\\Samples\\Analog_Common_Code\\XMS_Analog_Cfg.INI"
	if ( Analog_Common_Cfg_ReadCfg ( &g_Param_Analog ) != 0 )
	{
		sprintf ( MsgStr, "Analog_Common_Cfg_ReadCfg \"C:\\DJKeygoe\\Samples\\Analog_Common_Code\\XMS_Analog_Cfg.INI\" Error!" );
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
	}
	// end of code for CAS(SS1)

	// Init m_ListMain
	InitListMain();

	// Init m_ListUser
	InitListUser();

	// Init m_ListPcm
	InitListPcm();

	// Init m_ListMsg
	InitListMsg();

	// Init m_ListCount
	InitListCount();

	// Init Conifg's text box
	InitTextBox();

	// now open ACS Server   ����APP��Keygoe����ģ�������
	/* Call acsOpenStream() to connect with ACS Server */
	r = XMS_acsOpenStream ( &g_acsHandle, 
						&cfg_ServerID,
						g_u8UnitID,		// App Unit ID 
						32,				// sendQSize, in K Bytes
						32,				// recvQSize, in K Bytes
						cfg_s32DebugOn,	// Debug On/Off
						NULL);

	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );

		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
		AddMsg ( MsgStr );
		return false;
	}
	else
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
		AddMsg ( MsgStr );
	}

	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );        //���ûص�
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		AddMsg ( MsgStr );
		return false;
	}
	else
	{
		sprintf ( MsgStr, "XMS_acsSetESR() OK!" );
		AddMsg ( MsgStr );
	}

	InitAllDeviceRes ();
	
	XMS_acsGetDeviceList ( g_acsHandle, NULL );   //��ȡ�豸��Դ�б�

	return true;
}

void	ExitSystem() 
{
	RetCode_t	r;
	
	/*
	// close all device	
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		CloseAllDevice_Dsp ( MapTable_Module[i] );
	}
	*/

	r = XMS_acsCloseStream ( g_acsHandle, NULL );  //�ͷ�APP������ģ�������

	FreeAllDeviceRes ();

	// save to "XMS_Dial.INI"
	FetchFromText();
	WriteToConfig();

}

// --------------------------------------------------------------------------------
/**********************************************************************
Function:		�򿪻�ȡ����Դ�б��е��м��豸
Input:			TRUNK_STRUCT *pOneTrunk  �м��豸�ṹ ;   	
Output:			Void
Description:
**********************************************************************/
void	OpenTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t	r;
	
	if ( pOneTrunk->State == TRK_WAITOPEN )		// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
		if ( r < 0 )
		{
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenTrunkDevice()!" );
		}
	}
}

void	OpenVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
	RetCode_t	r;
	
	if ( pOneVoice->State == VOC_WAITOPEN )		// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
		if ( r < 0 )
		{
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenVoiceDevice()!" );
		}
	}
}

void	OpenPcmDevice ( PCM_STRUCT *pOnePcm )
{
	RetCode_t	r;
	
	if ( pOnePcm->bOpenFlag == false )		// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
		if ( r < 0 )
		{
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenPcmDevice()!" );
		}
	}
}

void	OpenBoardDevice (  DJ_S8 s8DspModID )
{
	RetCode_t	r;

	if ( AllDeviceRes[s8DspModID].bOpenFlag == false )	// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &AllDeviceRes[s8DspModID].deviceID, NULL );

		if ( r < 0 )
		{
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenBoardDevice()!" );
		}
	}
}

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID )
{
	int			 i;

	AllDeviceRes[s8DspModID].bErrFlag = false;
	DrawCount_ErrFlag (s8DspModID);
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_NONE;
	DrawCount_RemoveState (s8DspModID);

	// Open Board
	OpenBoardDevice ( s8DspModID );


	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}

	// pPcm
	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		OpenPcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}

	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}
}

void	ReOpen_AllDevice (void)
{
	int		i;

	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		OpenAllDevice_Dsp ( MapTable_Module[i] );
	}
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;
	VOICE_STRUCT	*pOneVoice;
	PCM_STRUCT		*pOnePcm;

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation

		SetGTD_ToneParam(pDevice);
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);

		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation

		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			InitUserChannel ( pOneTrunk );

			XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

			Change_State ( pOneTrunk, TRK_NOTHANDLE );		// This is Analog_User, do not handle in TrunkWork()

			// modify the count
			g_iTotalUserOpened ++;
			DrawCount_User ( pDevice->m_s8ModuleID );

		}
		else
		{
			InitTrunkChannel ( pOneTrunk );

			XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

		}

		// modify the count
		g_iTotalTrunkOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;

		DrawCount_Trunk ( pDevice->m_s8ModuleID  );


		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
		{
			// Set AnalogTrunk
			void* p = NULL;
			
			CmdParamData_AnalogTrunk_t cmdAnalogTrunk;
			DJ_U16 u16ParamType = ANALOGTRUNK_PARAM_UNIPARAM ;
			DJ_U16 u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);

			memset(&cmdAnalogTrunk,0,sizeof(cmdAnalogTrunk));

			cmdAnalogTrunk.m_u16CallInRingCount = 1;
			cmdAnalogTrunk.m_u16CallInRingTimeOut = 6000; //6 seconds

			p = (void*)&cmdAnalogTrunk;

			RetCode_t r;

			r = XMS_ctsSetParam( g_acsHandle, & pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
			char szbuffer[1024];
			memset(szbuffer,0,sizeof(szbuffer));
			sprintf(szbuffer,"Set AnalogTrunk  ret = %d\n",r);
			AddMsg ( szbuffer );
		}
		else if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			// Set AnalogUser
			void* p = NULL;

			CmdParamData_AnalogUser_t g_AnalogUser;
			DJ_U16 u16ParamType = ANALOGUSER_PARAM_UNIPARAM ;
			DJ_U16 u16ParamSize = sizeof(CmdParamData_AnalogUser_t);

			memset(&g_AnalogUser,0,sizeof(g_AnalogUser));

			g_AnalogUser.m_u8CallerIdOption  = 1; 
			g_AnalogUser.m_u8RingCadenceType = 0; 
			g_AnalogUser.m_u8CallOutRingTimeOut = 30 ;

			p = (void*)&g_AnalogUser;

			RetCode_t r ;

			r = XMS_ctsSetParam( g_acsHandle, & pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
			char szbuffer[1024];
			memset(szbuffer,0,sizeof(szbuffer));
			sprintf(szbuffer,"Set AnalogTrunk  ret = %d\n",r);
			AddMsg ( szbuffer );
		}
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		pOneVoice = &M_OneVoice(*pDevice);

		pOneVoice->deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		Change_Voc_State ( pOneVoice, VOC_FREE);

		g_iTotalVoiceOpened ++;
		g_iTotalVoiceFree ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;

		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(*pDevice);

		pOnePcm->deviceID.m_CallID = pDevice->m_CallID;		// this is very important

		// init the Device: Pcm
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		DrawPcm_TypeAndAlarm ( &M_OnePcm(*pDevice) );

		// moidfy the count
		g_iTotalPcmOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened ++;

		DrawCount_Pcm  ( pDevice->m_s8ModuleID );
	}
}
// --------------------------------------------------------------------------------
void	CloseDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;
	VOICE_STRUCT	*pOneVoice;
	PCM_STRUCT		*pOnePcm;

	AllDeviceRes[pDevice->m_s8ModuleID].bErrFlag = true;
	DrawCount_ErrFlag (pDevice->m_s8ModuleID);

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = false;
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);

		Change_State ( pOneTrunk, TRK_WAITOPEN );

		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			Change_UserState ( pOneTrunk, USER_WAITOPEN );

			// modify the count
			g_iTotalUserOpened --;
			DrawCount_User ( pDevice->m_s8ModuleID );
		}
		else
		{

		}

		// modify the count
		g_iTotalTrunkOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened --;

		DrawCount_Trunk ( pDevice->m_s8ModuleID  );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		pOneVoice = &M_OneVoice(*pDevice);

		Change_Voc_State ( pOneVoice, VOC_WAITOPEN);

		g_iTotalVoiceOpened --;
		g_iTotalVoiceFree --;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum --;

		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(*pDevice);

		pOnePcm->bOpenFlag = false;

		// moidfy the count
		g_iTotalPcmOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened --;

		DrawCount_Pcm  ( pDevice->m_s8ModuleID );
	}
}

// --------------------------------------------------------------------------------
void	CloseTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t	r;
	
	pOneTrunk->State= TRK_WAITOPEN;

	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	if ( r < 0 )
	{
		AddMsg ( "XMS_ctsCloseDevice Fail in CloseTrunkDevice()!" );
	}
}

void	CloseVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
	RetCode_t	r;
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
	if ( r < 0 )
	{
		AddMsg ( "XMS_ctsCloseDevice Fail in CloseVoiceDevice()!" );
	}
}

void	ClosePcmDevice ( PCM_STRUCT *pOnePcm )
{
	RetCode_t	r;
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
	if ( r < 0 )
	{
		AddMsg ( "XMS_ctsCloseDevice Fail in ClosePcmDevice()!" );
	}
}

void	CloseBoardDevice ( DeviceID_t *pBoardDevID )
{
	RetCode_t	r;

	r = XMS_ctsCloseDevice ( g_acsHandle, pBoardDevID, NULL );

	if ( r < 0 )
	{
		AddMsg ( "XMS_ctsCloseDevice Fail in CloseBoardDevice()!" );
	}
}

void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID )
{
	int			 i;

	// close Board
	CloseBoardDevice ( &AllDeviceRes[s8DspModID].deviceID );

	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		CloseTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}

	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		CloseVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}

	// pPcm
	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		ClosePcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}
}

// -------------------------------------------------------------------------------------------------
void	HandleDevState ( Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT	*pOneTrunk;
	PCM_STRUCT		*pOnePcm;
	Acs_GeneralProc_Data *pGeneralData = NULL;

	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(pAcsEvt->m_DeviceID);

		pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
		DrawMain_LineState( pOneTrunk );

		if ( pAcsEvt->m_DeviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
			DrawUser_LineState( pOneTrunk );
	}

	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(pAcsEvt->m_DeviceID);

		pOnePcm->u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		pOnePcm->s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);

		DrawPcm_TypeAndAlarm ( pOnePcm );
	}

}

// -------------------------------------------------------------------------------------------------
void	CheckRemoveReady ( DJ_S8 s8DspModID )
{
	int			i;

	// check device : INTERFACE_CH
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i ++ )
	{
		if ( (AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAITOPEN)
			&& (AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAIT_REMOVE) 
			&& (AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_NOTHANDLE) )
		{
			return;
		}

		if ( AllDeviceRes[s8DspModID].pTrunk[i].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			if ( (AllDeviceRes[s8DspModID].pTrunk[i].UserState != USER_WAITOPEN)
				&& (AllDeviceRes[s8DspModID].pTrunk[i].UserState != USER_WAIT_REMOVE) )
			{
				return;
			}
		}

	}

	// check device : VOICE
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i ++ )
	{
		if ( (AllDeviceRes[s8DspModID].pVoice[i].State != VOC_WAITOPEN)
			&& (AllDeviceRes[s8DspModID].pVoice[i].State != VOC_WAIT_REMOVE) )
		{
			return;
		}
	}

	// all device in this DSP is ready for remove 
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_READY;
	DrawCount_RemoveState ( s8DspModID );
}

void	Remove_OneDsp(void)
{
	POSITION	pos;
	int			index, i;
	DJ_S8		s8DspModID;

	pos = pdlg->m_ListCount.GetFirstSelectedItemPosition();
	
	index = pdlg->m_ListCount.GetNextSelectedItem ( pos);
	if ( ( index <= 0 ) || ( index > g_iTotalModule ) )
	{
		AfxMessageBox ( "Please select a valid DSP module in the upper list!" );
		return;
	}

	s8DspModID = MapTable_Module[index-1];		// because the first Row is Total, so minus 1
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_START;
	DrawCount_RemoveState ( s8DspModID );

	// handle device : INTERFACE_CH
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i ++ )
	{
		if ( AllDeviceRes[s8DspModID].pTrunk[i].State == TRK_FREE )
			Change_State ( &AllDeviceRes[s8DspModID].pTrunk[i], TRK_WAIT_REMOVE );

		if ( AllDeviceRes[s8DspModID].pTrunk[i].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			if ( AllDeviceRes[s8DspModID].pTrunk[i].UserState == USER_FREE )
				Change_UserState ( &AllDeviceRes[s8DspModID].pTrunk[i], USER_WAIT_REMOVE );
		}
	}

	// handle device : VOICE
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i ++ )
	{
		if ( AllDeviceRes[s8DspModID].pVoice[i].State == VOC_FREE )
		{
			Change_Voc_State ( &AllDeviceRes[s8DspModID].pVoice[i], VOC_WAIT_REMOVE);
		}
	}

	// check if all device is WAIT_OPEN or WAIT_REMOVE
	CheckRemoveReady ( s8DspModID );
}

// -----------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void	Change_Voc_State ( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState )
{
	pOneVoice->State = NewState;

	// Check if ready to remove DSP hardware
	if ( (AllDeviceRes[pOneVoice->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == VOC_FREE) )
	{
		pOneVoice->State = VOC_WAIT_REMOVE;

		CheckRemoveReady ( pOneVoice->deviceID.m_s8ModuleID );
	}
}

void	Change_UserState ( TRUNK_STRUCT *pOneTrunk, USER_STATE NewState )
{
	pOneTrunk->UserState = NewState;

	DrawUser_State ( pOneTrunk );
	// Check if ready to remove DSP hardware
	if ( (AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == USER_FREE) )
	{
		pOneTrunk->UserState = USER_WAIT_REMOVE;

		DrawUser_State ( pOneTrunk );

		CheckRemoveReady ( pOneTrunk->deviceID.m_s8ModuleID );
	}
}

void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{
	pOneTrunk->State = NewState;

	DrawMain_State ( pOneTrunk );

	// Check if ready to remove DSP hardware
	if ( (AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == TRK_FREE) )
	{
		pOneTrunk->State = TRK_WAIT_REMOVE;

		DrawMain_State ( pOneTrunk );

		CheckRemoveReady ( pOneTrunk->deviceID.m_s8ModuleID );
	}
}

/**********************************************************************
Function:	 ����Tone���ź�
Input:		 DeviceID_t	*pVocDevID  �����豸DeviceID ; 
			 iPlayType  Tone������   0  ������   1  ������   2   �һ�æ��   -1  ֹͣ����Tone��  
Output:		 1  success  ;   else  failure  
Description: 
**********************************************************************/

DJ_S32	PlayTone ( DeviceID_t	*pVocDevID, int iPlayType )
{
	DJ_U32           i = 0;	
	DJ_U16           u16IoLen = 0;
	DJ_U16           u16IoType = 0;
	RetCode_t		 r;
	char		 	 IoDataBuf[MAX_SEND_IODATA_DTMF_LEN]={0};
	
	if ( iPlayType == -1 )	// Stop Play Tone
	{
		u16IoLen = 0;
		u16IoType = XMS_IO_TYPE_GTG;
	}
	else
	{
		u16IoType = XMS_IO_TYPE_GTG;
		u16IoLen = 1;

		switch ( iPlayType )
		{
		case 0:		// Dial Tone
			IoDataBuf[0] = 'G';
			break;
		case 1:		// Ring Back Tone
			IoDataBuf[0] = 'H';
			break;
		case 2:		// Busy Tone
			IoDataBuf[0] = 'I';
			break;
		}
	}

	r = XMS_ctsSendIOData ( g_acsHandle, pVocDevID, u16IoType,u16IoLen,IoDataBuf);

	return	r;

}

DJ_S32	StopPlayTone ( DeviceID_t	*pVocDevID )
{
	return PlayTone ( pVocDevID, -1 );
}


/**********************************************************************
Function:		���Tone�������Ƿ����
Input:			TRUNK_STRUCT *pOneTrunk  �м��豸�ṹ ;
				Acs_Evt_t *pAcsEvt       �¼����ṹͷָ��        	
Output:			True  �������  ;   False  δ�յ���������¼�
Description:
**********************************************************************/
bool	CheckPlayGTGEnd ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_SENDIODATA )
	{
		return true;
	}

	return false;
}


/**********************************************************************
Function:	��FSK��ʽ����	
Input:		 DeviceID_t	*pVocDevID    �����豸DeviceID ;
			 char *CallerIDStr        ��Ҫ���͵�FSK���к���					       	
Output:		 r  ����ִ�н��          1  sussess ;  else  failure 	
Description: 
			 step 1   ConvertCallerIDToRawFsk()    �����к���ת����ָ����FSK���ݸ�ʽ
			 step 2   XMS_ctsSendIOData ()         ����FSK����
**********************************************************************/
DJ_S32	PlayFSKCallerID ( DeviceID_t	*pVocDevID, char *CallerIDStr )
{
	DJ_U32           i = 0, len;
	DJ_U16           u16IoType = 0;
	DJ_U16           u16IoLen = 0;
	unsigned char		 	 IoDataBuf[128];
	RetCode_t		 r;

	if ( CallerIDStr == NULL )	// Stop Play FSK Caller ID
	{
		u16IoLen = 0;
	}
	else
	{
		u16IoType = XMS_IO_TYPE_FSK;

		len = ConvertCallerIDToRawFsk ( CallerIDStr, IoDataBuf, 128 );

		u16IoLen = (DJ_U16) len;
	}

	r = XMS_ctsSendIOData ( g_acsHandle, pVocDevID, u16IoType,u16IoLen,IoDataBuf);

	return	r;

}

DJ_S32	StopPlayFSKCallerID ( DeviceID_t	*pVocDevID )
{
	return PlayFSKCallerID ( pVocDevID, NULL );
}



/**********************************************************************
Function:      ���FSK�����Ƿ������		
Input:		   TRUNK_STRUCT *pOneTrunk  �м��豸�ṹ ;
			   Acs_Evt_t *pAcsEvt       �¼����ṹͷָ��  					       	
Output:		   True  �������  ;   False  δ�յ���������¼�
Description:
**********************************************************************/
bool	CheckPlayFSKEnd ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_SENDIODATA )
	{
		return true;
	}

	return false;
}


/**********************************************************************
Function:		����DTMF˫��Ƶ	
Input:			DeviceID_t	*pVocDevID    �����豸DeviceID ;
				char *CallerIDStr         ��Ҫ���͵�DTMF����
Output:			r  ����ִ�н��          1  sussess ;  else  failure 			
Description:
**********************************************************************/
DJ_S32	PlayDTMF ( DeviceID_t	*pVocDevID, char *DtmfStr )
{
	DJ_U32           i = 0, len;
	DJ_U16           u16IoType = 0;
	DJ_U16           u16IoLen = 0;
	char		 	 IoDataBuf[MAX_SEND_IODATA_DTMF_LEN]={0};
	RetCode_t		 r;

	if ( DtmfStr == NULL )	// Stop Play Dtmf
	{
		u16IoLen = 0;
	}
	else
	{
		u16IoType = XMS_IO_TYPE_DTMF;

		len = strlen(DtmfStr);
		if ( len > MAX_SEND_IODATA_DTMF_LEN )
			len = MAX_SEND_IODATA_DTMF_LEN;
		u16IoLen = (DJ_U16) len;

		memcpy ( IoDataBuf, DtmfStr, len );
	}

	r = XMS_ctsSendIOData ( g_acsHandle, pVocDevID, u16IoType,u16IoLen,IoDataBuf);

	return	r;

}


DJ_S32	StopPlayDTMF ( DeviceID_t	*pVocDevID )
{
	return PlayDTMF ( pVocDevID, NULL );
}

void My_InitDtmfBuf ( TRUNK_STRUCT *pOneTrunk )
{
	pOneTrunk->DtmfCount = 0;
	pOneTrunk->DtmfBuf[0] = 0; 
}

void My_AddDtmfBuf ( TRUNK_STRUCT *pOneTrunk, char TmpDtmf )
{
	if ( pOneTrunk->DtmfCount < (32-1) )
	{
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount] = TmpDtmf; 
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount+1] = 0; 
		pOneTrunk->DtmfCount ++;
	}
}




/**********************************************************************
Function:	  ��ȡ���յ���DTMF˫��Ƶ����	
Input:		  Acs_Evt_t *pAcsEvt       �¼����ṹͷָ��					       	
Output:		  *p					   DTMF˫��Ƶ�ַ�����   		
Description:
**********************************************************************/
char My_GetDtmfCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*Receive IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if ( ( pIOData->m_u16IoType == XMS_IO_TYPE_DTMF ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);

			return *p;
		}
	}

	return -1;	// not a good DTMF
}


/**********************************************************************
Function:		���Tone���źź�PVD	������	
Input:			Acs_Evt_t *pAcsEvt    �¼����ṹͷָ�� 				       	
Output:			Tone������������
Description:	
**********************************************************************/
char My_GetGtdOrPVDCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char   sbuffer[200];
	char		*p;

	memset(sbuffer,0,sizeof(sbuffer));
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if ( ( ( pIOData->m_u16IoType== XMS_IO_TYPE_GTG ) || ( pIOData->m_u16IoType== XMS_IO_TYPE_PVD ) )
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			sprintf(sbuffer,"[%s]",p);
			AddMsg(sbuffer);

			return *p;
		}
	}

	return -1;	// not a good GTD
}


/**********************************************************************
Function:	  ���˫���ʱ϶����	
Input:		  DeviceID_t *pDev1	, DeviceID_t *pDev2     �����˫�����ӵ��豸��DeviceID						       	
Output:		  Void	
Description:  
**********************************************************************/
void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}


/**********************************************************************
Function:	  �Ͽ�˫���ʱ϶����	
Input:		  DeviceID_t *pDev1	, DeviceID_t *pDev2     ����жϿ����ӵ��豸��DeviceID						       	
Output:		  Void	
Description:  
**********************************************************************/
void	My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}


/**********************************************************************
Function:	  ��ȡһ�����е������豸	
Input:		  TRUNK_STRUCT *pOneTrunk		�м��豸�ṹ ;
			  DeviceID_t *pFreeVocDeviceID  �ò���ΪOutput��������Ż�ȡ���������豸DeviceID 							       	
Output:		  DeviceID_t *pFreeVocDeviceID  �����ȡ�ɹ�����Ż�ȡ���������豸ID������ΪNULL 	
Description:  
			  cfg_iVoiceRule = 0           �����豸���м��豸����һһ�󶨹�ϵ
			  cfg_iVoiceRule = 1		   ��ȡ���м���ͬһDSP�������豸
			  cfg_iVoiceRule = 2		   ��ȡ��������������DSP�ϵ������豸
**********************************************************************/
int		SearchOneFreeVoice (  TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8			s8ModID;
	DJ_S16			s16ChID;
	int				i;
	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;
	long			lNowMostFreeNum;

	s8ModID = pOneTrunk->deviceID.m_s8ModuleID;

	if ( cfg_iVoiceRule == 0 )			// Fix relationship between Trunk & Voice
	{
		s16ChID = pOneTrunk->deviceID.m_s16ChannelID;
		
		i = pOneTrunk->iModSeqID;

		if ( i < AllDeviceRes[s8ModID].lVocNum )
		{
			pOneVoice = &AllDeviceRes[s8ModID].pVoice[i];
			if ( pOneVoice->State != VOC_WAITOPEN )
			{
				*pFreeVocDeviceID = pOneVoice->deviceID;

				// use this voice device 
				Change_Voc_State ( pOneVoice, VOC_USED);
				AllDeviceRes[s8ModID].lVocFreeNum--;
				g_iTotalVoiceFree --;
				DrawCount_Voc ( s8ModID );
				return i;
			}
		}

		return -1;
	}

	if ( cfg_iVoiceRule == 1 )			// Search in Same Module
	{
		s8SearchModID = s8ModID;
	}
	else		// Search in Most free resource module
	{
		s8SearchModID = -1;
		lNowMostFreeNum = -1;
		for ( i = 0; i < g_iTotalModule; i ++ )
		{
			if ( AllDeviceRes[MapTable_Module[i]].lVocFreeNum > lNowMostFreeNum )
			{
				s8SearchModID = MapTable_Module[i];
				lNowMostFreeNum = AllDeviceRes[MapTable_Module[i]].lVocFreeNum;
			}
		}
	}

	for ( i = 0; i < AllDeviceRes[s8SearchModID].lVocNum; i ++ )
	{
		pOneVoice = &AllDeviceRes[s8SearchModID].pVoice[i];
		if ( pOneVoice->State == VOC_FREE )
		{
			*pFreeVocDeviceID = pOneVoice->deviceID;

			// use this voice device 
			Change_Voc_State ( pOneVoice, VOC_USED);
			AllDeviceRes[s8SearchModID].lVocFreeNum--;
			g_iTotalVoiceFree --;
			DrawCount_Voc ( s8SearchModID );
			return i;
		}
	}

	return -1;
}


/**********************************************************************
Function:	  �ͷű�ռ�õ������豸	
Input:		  DeviceID_t *pFreeVocDeviceID     �豻�ͷŵ������豸��DeviceID						       	
Output:		  
Description:  
**********************************************************************/
int		FreeOneFreeVoice (  DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8	s8ModID;

	s8ModID = pFreeVocDeviceID->m_s8ModuleID;
	if ( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		Change_Voc_State ( &M_OneVoice(*pFreeVocDeviceID), VOC_FREE);

		AllDeviceRes[s8ModID].lVocFreeNum++;
		g_iTotalVoiceFree ++;
		DrawCount_Voc ( s8ModID );
		return	0;		// OK
	}

	return -1;			// invalid VocDeviceID
}

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
	Change_State ( pOneTrunk, TRK_FREE );

	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	memset ( &pOneTrunk->LinkDevID, 0, sizeof(DeviceID_t) );	// 0: didn't alloc Link Device

	DrawMain_LinkDev ( pOneTrunk );
}


void ResetTrunk ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT		 * pLinkUser = NULL;

	// free the link dev
	if ( pOneTrunk->LinkDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneTrunk->LinkDevID, &pOneTrunk->deviceID );

		pLinkUser = &M_OneTrunk(pOneTrunk->LinkDevID);

		My_DualLink ( &pLinkUser->VocDevID, &pLinkUser->deviceID );
		PlayTone ( &pLinkUser->VocDevID, 2 );		// busy tone

		memset ( &pLinkUser->LinkDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Link Device
	}

	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );

		FreeOneFreeVoice (  &pOneTrunk->VocDevID );

		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
//		DrawMain_VocInfo ( pOneTrunk );
	}

	InitTrunkChannel ( pOneTrunk );
}



// Special code for CAS(SS1)
/**********************************************************************
Function:	  ���CAS�����MFC�������̣���SS1֮������������Ҫ����	
Input:			TRUNK_STRUCT *pOneTrunk  �м��豸�ṹ ;
				Acs_Evt_t *pAcsEvt       �¼����ṹͷָ��  Output:					  
Description:  
**********************************************************************/
void	SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t				FreeVocDeviceID;

	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_E1_CAS )
		return;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_START )	/* start receive MFC, need a Voice Device */
	{
		if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
		{
			pOneTrunk->VocDevID = FreeVocDeviceID;
			M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
//			DrawMain_VocInfo ( pOneTrunk );
			My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );

			g_Param_CAS.m_VocDevID = FreeVocDeviceID;
			XMS_ctsSetParam ( g_acsHandle, &pOneTrunk->deviceID, 
				CAS_PARAM_UNIPARAM, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );
		}
	}
    if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
//	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_END )	/* receive MFC end, release Voice Device */
	{
		My_DualUnlink(&pOneTrunk->VocDevID,&pOneTrunk->deviceID );

		FreeOneFreeVoice (  &pOneTrunk->VocDevID );

		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device 
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
//		DrawMain_VocInfo ( pOneTrunk );
	}
}
// end of code for CAS(SS1)



// Special code for Analog
/**********************************************************************
Function:	  ����Tone�����ģ��    ���͡�Ƶ�ʡ���ͣʱ���	
Input:		  DeviceID_t *pDevice     Board�豸��DeviceID						       	
Output:		  
Description:  
**********************************************************************/
void	SetGTD_ToneParam ( DeviceID_t *pDevice )
{
	RetCode_t					r;
	DJ_U16						u16ParamType, u16ParamSize;
	CmdParamData_GtdFreq_t		TmpGtdFreq;
	CmdParamData_GtdProtoType_t	TmpGtdProto;

	// ---------- set Freq ----------
	u16ParamType = 	BOARD_PARAM_SETGTDFREQ ;
	u16ParamSize = sizeof(CmdParamData_GtdFreq_t);

	// freq0
	TmpGtdFreq.m_u16Freq_Index = 0;
	TmpGtdFreq.m_u16Freq_Coef = g_Param_Analog.m_u16Freq0;
	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdFreq );
		
	// freq1
	TmpGtdFreq.m_u16Freq_Index = 1;
	TmpGtdFreq.m_u16Freq_Coef = g_Param_Analog.m_u16Freq1;
	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdFreq );

	// ---------- set DialTone ----------
	u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);
	
	memset ( &TmpGtdProto, 0, sizeof(CmdParamData_GtdProtoType_t) );
	TmpGtdProto.m_u16GtdID = 48;		// DialTone, we use GTD's ID 48
	TmpGtdProto.m_u16Freq_Mask = g_Param_Analog.m_u16DialTone_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 0;
	TmpGtdProto.m_u16Repeat_Count = 1;
	TmpGtdProto.m_u16Min_On_Time1 = g_Param_Analog.m_u16DialTone_On_Time/15;		// the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set RingBack Tone ----------
	u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);
	
	memset ( &TmpGtdProto, 0, sizeof(CmdParamData_GtdProtoType_t) );
	TmpGtdProto.m_u16GtdID = 49;		// DialTone, we use GTD's ID 49
	TmpGtdProto.m_u16Freq_Mask = g_Param_Analog.m_u16RingBackTone_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count = 1;
	TmpGtdProto.m_u16Min_On_Time1 = (g_Param_Analog.m_u16RingBackTone_On_Time * 
		(100 - g_Param_Analog.m_u16RingBackTone_TimeDeviation)/100 ) / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = (g_Param_Analog.m_u16RingBackTone_On_Time * 
		(100 + g_Param_Analog.m_u16RingBackTone_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = (g_Param_Analog.m_u16RingBackTone_Off_Time * 
		(100 - g_Param_Analog.m_u16RingBackTone_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = (g_Param_Analog.m_u16RingBackTone_Off_Time * 
		(100 + g_Param_Analog.m_u16RingBackTone_TimeDeviation))/100  / 15;		// the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set Busy Tone0 ----------
	u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);
	
	memset ( &TmpGtdProto, 0, sizeof(CmdParamData_GtdProtoType_t) );
	TmpGtdProto.m_u16GtdID = 50;		// BusyTone0, we use GTD's ID 50
	TmpGtdProto.m_u16Freq_Mask = g_Param_Analog.m_u16BusyTone0_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count = 1;
	TmpGtdProto.m_u16Min_On_Time1 = (g_Param_Analog.m_u16BusyTone0_On_Time * 
		(100 - g_Param_Analog.m_u16BusyTone0_TimeDeviation)/100 ) / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = (g_Param_Analog.m_u16BusyTone0_On_Time * 
		(100 + g_Param_Analog.m_u16BusyTone0_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = (g_Param_Analog.m_u16BusyTone0_Off_Time * 
		(100 - g_Param_Analog.m_u16BusyTone0_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = (g_Param_Analog.m_u16BusyTone0_Off_Time * 
		(100 + g_Param_Analog.m_u16BusyTone0_TimeDeviation))/100  / 15;		// the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set Busy Tone1 ----------
	u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);
	
	memset ( &TmpGtdProto, 0, sizeof(CmdParamData_GtdProtoType_t) );
	TmpGtdProto.m_u16GtdID = 51;		// BusyTone1, we use GTD's ID 51
	TmpGtdProto.m_u16Freq_Mask = g_Param_Analog.m_u16BusyTone1_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count = 1;
	TmpGtdProto.m_u16Min_On_Time1 = (g_Param_Analog.m_u16BusyTone1_On_Time * 
		(100 - g_Param_Analog.m_u16BusyTone1_TimeDeviation)/100 ) / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = (g_Param_Analog.m_u16BusyTone1_On_Time * 
		(100 + g_Param_Analog.m_u16BusyTone1_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = (g_Param_Analog.m_u16BusyTone1_Off_Time * 
		(100 - g_Param_Analog.m_u16BusyTone1_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = (g_Param_Analog.m_u16BusyTone1_Off_Time * 
		(100 + g_Param_Analog.m_u16BusyTone1_TimeDeviation))/100  / 15;		// the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set Busy Tone2 ----------
	u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);
	
	memset ( &TmpGtdProto, 0, sizeof(CmdParamData_GtdProtoType_t) );
	TmpGtdProto.m_u16GtdID = 52;		// BusyTone2, we use GTD's ID 52
	TmpGtdProto.m_u16Freq_Mask = g_Param_Analog.m_u16BusyTone2_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count = 1;
	TmpGtdProto.m_u16Min_On_Time1 = (g_Param_Analog.m_u16BusyTone2_On_Time * 
		(100 - g_Param_Analog.m_u16BusyTone2_TimeDeviation)/100 ) / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = (g_Param_Analog.m_u16BusyTone2_On_Time * 
		(100 + g_Param_Analog.m_u16BusyTone2_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = (g_Param_Analog.m_u16BusyTone2_Off_Time * 
		(100 - g_Param_Analog.m_u16BusyTone2_TimeDeviation))/100  / 15;		// the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = (g_Param_Analog.m_u16BusyTone2_Off_Time * 
		(100 + g_Param_Analog.m_u16BusyTone2_TimeDeviation))/100  / 15;		// the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );
}
// end of code for Analog

void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	TRUNK_STRUCT		 *	pLinkUser = NULL;
	DeviceID_t				FreeVocDeviceID;
	Acs_GeneralProc_Data *  pGeneral=NULL;
	char					TmpGtd;
	char					TmpStr[128];

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*Clear Event*/
	{
		if ( (pOneTrunk->State != TRK_FREE) && (pOneTrunk->State != TRK_WAIT_REMOVE) )
		{
			ResetTrunk ( pOneTrunk, pAcsEvt );
			return;
		}
	}

	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		break;

	case TRK_CALLOUT://��ȡ������ɹ��¼���˫���м�ͨ������ϯͨ��
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);

		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( pCallControl->m_s32AcsEvtState == 1 )	// Call Out OK��Callee Offhook
			{
				pLinkUser = &M_OneTrunk(pOneTrunk->LinkDevID);

				// unlink voc
				// next line will trigger a BUG
				//My_DualUnink (  pUserDevID, &pTrunk[pUserDevID->m_s16ChannelID].VocDevID );
				//

				// Link trunk and user
				if(pOneTrunk->deviceID.m_s8MachineID == pLinkUser->deviceID.m_s8MachineID)
				{
					My_DualLink (  &pOneTrunk->deviceID, &pOneTrunk->LinkDevID );
				}

				Change_State ( pOneTrunk, TRK_LINK );
				Change_UserState ( pLinkUser, USER_LINK );
			}
			else		// Call Out Fail
			{
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );

				pLinkUser = &M_OneTrunk(pOneTrunk->LinkDevID);
	
				PlayTone ( &pLinkUser->VocDevID, 2 );		// busy tone

				memset ( &pLinkUser->LinkDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Link Device
				Change_UserState ( pLinkUser, USER_WAITHANGUP );
			}
		}
		break;

	case TRK_SIM_CALLOUT://�Զ����(SIM_CALLOUT)�ɹ���˫��Link�����豸
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);

		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if(  pAcsEvt->m_DeviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
			{
				DrawMain_Analog_Reason ( pOneTrunk, "" ); 

				XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, g_Param_Analog.m_u16NoDialToneAfterOffHook );	// start a timer: 5 Seconds

				Change_State(pOneTrunk,TRK_SIM_ANALOG_OFFHOOK);
				return;
			}

			if ( pCallControl->m_s32AcsEvtState == 1 )	// Call Out OK��Callee Offhook
			{
				if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
				{
					pOneTrunk->VocDevID = FreeVocDeviceID;

					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 

//					DrawMain_VocInfo ( pOneTrunk );

					My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );
					//PlayDTMF ( &pOneTrunk->VocDevID, "12345" );		// play dtmf

				}

				Change_State ( pOneTrunk, TRK_SIM_LINK );
				//Change_State ( pOneTrunk, TRK_PLAYDTMF );
			}
			else		// Call Out Fail
			{
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );
			}
		}
		break;

	case TRK_SIM_LINK://����DTMF˫��Ƶ"12345"
		{
			if ( pAcsEvt->m_s32EventType == XMS_EVT_LINKDEVICE)
			{

				PlayDTMF ( &pOneTrunk->VocDevID, "12345" );		// play dtmf
				
				Change_State ( pOneTrunk, TRK_PLAYDTMF );
			}
		}
		break;
	case TRK_SIM_ANALOG_OFFHOOK:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )//ģ���м����,��ⲻ����������ʱ�һ�
		{
			// time out
			DrawMain_Analog_Reason ( pOneTrunk, "Wait Dial Tone TimeOut" ); 

			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

			Change_State ( pOneTrunk, TRK_HANGUP );
		}

		TmpGtd = My_GetGtdOrPVDCode ( pAcsEvt );
		if ( TmpGtd != -1 )
		{
			if ( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )		// Busy Toneģ���м������⵽æ���һ�
			{	
				sprintf ( TmpStr, "Busy Tone : %c", TmpGtd );
				DrawMain_Analog_Reason ( pOneTrunk, TmpStr ); 

				XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

				Change_State ( pOneTrunk, TRK_HANGUP );
			}
			else if ( TmpGtd == 'G' )		// Dial Tone  ģ���м����ʱ,��⵽������Ȼ����DTMF˫��Ƶ
			{
				// start semd DTMF
				 int ret = XMS_ctsSendIOData(g_acsHandle,&pAcsEvt->m_DeviceID,XMS_IO_TYPE_DTMF,strlen(cfg_SimCalledNum),cfg_SimCalledNum);
				 if( ret > 0 )
				 {
					XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, g_Param_Analog.m_u16Ringback_NoAnswerTime );	// start a timer: 40 Seconds
					Change_State(pOneTrunk,TRK_SIM_ANALOG_DIALING);
				 }
				 else
				 {
					 ResetTrunk(pOneTrunk,pAcsEvt);
				 }
			}
		}
		break;

	case TRK_SIM_ANALOG_DIALING:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )   //ģ���м��������δ��Ӧ��ʱ
		{
			// time out
			DrawMain_Analog_Reason ( pOneTrunk, "Wait Callee Offhook TimeOut" ); 

			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

			Change_State ( pOneTrunk, TRK_HANGUP );
		}

		TmpGtd = My_GetGtdOrPVDCode ( pAcsEvt );
		if ( TmpGtd != -1 )
		{
			if ( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )		// Busy Tone
			{	
				sprintf ( TmpStr, "Callee Busy Tone : %c", TmpGtd );
				DrawMain_Analog_Reason ( pOneTrunk, TmpStr ); 

				XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

				Change_State ( pOneTrunk, TRK_HANGUP );
			}
			else if(TmpGtd == 'h' || TmpGtd == 'b' ) 
			{
				sprintf ( TmpStr, "Callee Answer : %c", TmpGtd );
				DrawMain_Analog_Reason ( pOneTrunk, TmpStr ); 

				PlayDTMF ( &pOneTrunk->VocDevID, "12345" );		// play dtmf

				Change_State ( pOneTrunk, TRK_PLAYDTMF );
			}
		}
		break;

	case TRK_LINK:
		break;

	case TRK_PLAYDTMF:
		if ( CheckPlayGTGEnd ( pOneTrunk, pAcsEvt) )	/*play Over Event˫��Ƶ������ɺ�һ�*/
		{
			// Clear Call
			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

			Change_State ( pOneTrunk, TRK_HANGUP );
		}
		break;

	}
}

void	InitUserChannel ( TRUNK_STRUCT *pOneUser )
{
	Change_UserState ( pOneUser, USER_FREE );

	My_InitDtmfBuf ( pOneUser );

	memset ( &pOneUser->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	memset ( &pOneUser->LinkDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Link Device

	DrawUser_VocInfo ( pOneUser );
	DrawUser_LinkDev ( pOneUser );
}

void ResetUser ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT		*pLinkTrunk = NULL;
	TRUNK_STRUCT		*pLinkUser = NULL;

	if ( pOneUser->VocDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneUser->VocDevID, &pOneUser->deviceID );

		FreeOneFreeVoice (  &pOneUser->VocDevID );

	}

	if ( pOneUser->LinkDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneUser->LinkDevID, &pOneUser->deviceID );

		pLinkTrunk = &M_OneTrunk(pOneUser->LinkDevID);

		memset ( &pLinkTrunk->LinkDevID, 0, sizeof(DeviceID_t) );		// 0: Didn't alloc Link Device 
		XMS_ctsClearCall ( g_acsHandle, &pLinkTrunk->deviceID, 0, NULL );

		if ( pOneUser->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER ) 
		{
			pLinkUser = pLinkTrunk;
			if ( pLinkUser->UserState == USER_RING ) 
				InitUserChannel ( pLinkUser );
		}

	}

	InitUserChannel ( pOneUser );
}

bool	IsTrunk ( DJ_S16 s16DevSub )
{

	if ( (s16DevSub == XMS_DEVSUB_E1_PCM)
		|| (s16DevSub == XMS_DEVSUB_E1_CAS)
		|| (s16DevSub == XMS_DEVSUB_E1_DSS1)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_TUP)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_ISUP) 
		|| (s16DevSub == XMS_DEVSUB_ANALOG_TRUNK) 
		)
	{
		return true;
	}

	return false;
}

bool	IsDigitTrunk ( DJ_S16 s16DevSub )
{

	if ( (s16DevSub == XMS_DEVSUB_E1_PCM)
		|| (s16DevSub == XMS_DEVSUB_E1_CAS)
		|| (s16DevSub == XMS_DEVSUB_E1_DSS1)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_TUP)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_ISUP) 
		)
	{
		return true;
	}

	return false;
}

int	 GetOutUserID (  char *pDtmf, DeviceID_t *pUserDeviceID )
{
	int		iTmp;
	DJ_S8	s8ModID;
	DJ_S16	s16ChID;
	static	int	iLoopStart = 0;

	if ( cfg_iCallOutRule == 2 )		// Select a Fix Channel by DTMF's tail 3
	{
		iTmp = 0;
		sscanf ( pDtmf, "%d", &iTmp ); 
		iTmp %= 1000;

		s8ModID = MapTable_Trunk[iTmp].m_s8ModuleID;
		s16ChID = MapTable_Trunk[iTmp].m_s16ChannelID;

		if ( iTmp >= g_iTotalTrunk )
			return -1;

		if ( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState == DCS_FREE )
			&& ( AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER ) )
		{
			*pUserDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			return iTmp;
		}
		return -1;
	}

	return -1;
}


/********************************************************************************
Function:		��ȡһ�����е�������Դ
Input:			char *pDtmf						����ı��к���
				DeviceID_t *pFreeTrkDeviceID	��ȡ���Ŀ����м�DeviceID  (�ò���ΪOutput����)
Output:			DeviceID_t *pFreeTrkDeviceID	��ȡ���Ŀ����м�DeviceID;
				-1								δ��ȡ�����������Ŀ����м�
Description:	
				cfg_iCallOutRule = 2				ͨ�����к����3λѡ��ƥ��ͨ��
				cfg_iCallOutRule = 0				ʼ�մ�0ͨ����ʼ����
				cfg_iCallOutRule = 1				��0ͨ������ѭ������
********************************************************************************/

int	 SearchOneFreeTrunk (  char *pDtmf, DeviceID_t *pFreeTrkDeviceID )
{
	int		i, iTmp;
	DJ_S8	s8ModID;
	DJ_S16	s16ChID;
	static	int	iLoopStart = 0;

	if ( cfg_iCallOutRule == 2 )		// Select a Fix Channel by DTMF's tail 3
	{
		iTmp = 0;
		sscanf ( pDtmf, "%d", &iTmp ); 
		iTmp %= 1000;

		s8ModID = MapTable_Trunk[iTmp].m_s8ModuleID;
		s16ChID = MapTable_Trunk[iTmp].m_s16ChannelID;

		if ( iTmp >= g_iTotalTrunk )
			return -1;

		if ( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState == DCS_FREE )
			//&& IsDigitTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
			&& IsTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
		{
			*pFreeTrkDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			return iTmp;
		}
		return -1;
	}

	// Sequence Mode or Loop Mode
	if ( cfg_iCallOutRule == 0 )		// Sequence Mode
	{
		iLoopStart = 0;
	}

	//if ( cfg_iCallOutRule == 1 )		// Loop Mode
	for ( i = 0; i < g_iTotalTrunk; i ++ )
	{
		s8ModID = MapTable_Trunk[iLoopStart].m_s8ModuleID;
		s16ChID = MapTable_Trunk[iLoopStart].m_s16ChannelID;

		if ( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState == DCS_FREE )
			//&& IsDigitTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
			&& IsTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
		{
			*pFreeTrkDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			iTmp = iLoopStart;
			iLoopStart = (iLoopStart + 1 ) % g_iTotalTrunk;
			return iTmp;
		}
		iLoopStart = (iLoopStart + 1 ) % g_iTotalTrunk;
	}

	return -1;
}

void UserWork ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t			FreeVocDeviceID, FreeTrkDeviceID;
	DeviceID_t			OutUserDeviceID;
	char				TmpDtmf;
	int					i;
	RetCode_t			r;
	char				MsgStr[256];
	TRUNK_STRUCT		*pLinkTrunk = NULL;
	TRUNK_STRUCT		*pLinkUser = NULL;
	Acs_GeneralProc_Data* pGeneral=NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*Clear Call Event*/
	{		
		if ( (pOneUser->UserState != USER_FREE) && (pOneUser->UserState != USER_WAIT_REMOVE) )
		{
			ResetUser ( pOneUser, pAcsEvt );
			return ; 
		}
	}

	switch(pOneUser->UserState)
	{
	case USER_FREE://�ȴ���ϯ�û�ժ���������Ų�����
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	/*User Offhook Event*/
		{

			if ( SearchOneFreeVoice ( pOneUser,  &FreeVocDeviceID ) >= 0 )
			{
				pOneUser->VocDevID = FreeVocDeviceID;

				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneUser->deviceID; 

				DrawUser_VocInfo ( pOneUser );

				My_DualLink ( &FreeVocDeviceID, &pOneUser->deviceID );
				PlayTone ( &pOneUser->VocDevID, 0 );		// dial tone

				Change_UserState ( pOneUser, USER_OFFHOOK );
			}

		}
		break;

	case USER_OFFHOOK://��⵽�û�����֮��ֹͣ�Ų�����;���밴��֮��ѡ���Ӧ�м�ͨ�������������Ż�����
		TmpDtmf = My_GetDtmfCode ( pAcsEvt );
		if ( TmpDtmf != -1 )								/*DTMF*/
		{
			My_AddDtmfBuf ( pOneUser, TmpDtmf );
			DrawUser_DTMF ( pOneUser );

			StopPlayTone ( &pOneUser->VocDevID );

			if ( pOneUser->DtmfCount >= cfg_iCalledLen )
			{
				// For Link User & User, add this in V2.2.0
				if ( cfg_iCallOutRule == 2 )		// "Dtmf Tail 3" Mode
				{
					i = GetOutUserID  ( pOneUser->DtmfBuf,  &OutUserDeviceID );
					if ( i > 0 )		// this will trigger a User To User's call
					{
						r = XMS_ctsMakeCallOut ( g_acsHandle, &OutUserDeviceID, cfg_CallingNum, pOneUser->DtmfBuf, NULL );
						if ( r > 0 )
						{
							PlayTone ( &pOneUser->VocDevID, 1 );		// RingBack tone

							// call out OK
							pLinkUser = &M_OneTrunk(OutUserDeviceID);

							pOneUser->LinkDevID = OutUserDeviceID;
							pLinkUser->LinkDevID = pOneUser->deviceID; 
							DrawUser_LinkDev ( pOneUser );
							DrawUser_LinkDev ( pLinkUser );

							Change_UserState ( pOneUser, USER_CALLOUT );
							Change_UserState ( pLinkUser, USER_RING );

							XMS_ctsSetDevTimer ( g_acsHandle, &OutUserDeviceID, g_Param_Analog.m_u16SendFSKCallerIDTime );	// start a timer: 1.5 Seconds
							break;
						}
						else
						{
							sprintf ( MsgStr, "XMS_ctsMakeCallOut() FAIL! ret = %d", r );
							AddMsg ( MsgStr );
						}
					}
				}
				// end of For Link User & User

				i = SearchOneFreeTrunk ( pOneUser->DtmfBuf,  &FreeTrkDeviceID );
				if ( i > 0 )
				{
					r = XMS_ctsMakeCallOut ( g_acsHandle, &FreeTrkDeviceID, cfg_CallingNum, pOneUser->DtmfBuf, NULL );
					if ( r > 0 )
					{
						// call out OK
						pLinkTrunk = &M_OneTrunk(FreeTrkDeviceID);

						pOneUser->LinkDevID = FreeTrkDeviceID;
						pLinkTrunk->LinkDevID = pOneUser->deviceID; 
						DrawUser_LinkDev ( pOneUser );
						DrawMain_LinkDev ( pLinkTrunk );

						strcpy ( pLinkTrunk->CallerCode, cfg_CallingNum );
						strcpy ( pLinkTrunk->CalleeCode, pOneUser->DtmfBuf );
						DrawMain_CallInfo ( pLinkTrunk );

						Change_UserState ( pOneUser, USER_CALLOUT );
						Change_State ( pLinkTrunk, TRK_CALLOUT );
						break;
					}
					else
					{
						sprintf ( MsgStr, "XMS_ctsMakeCallOut() FAIL! ret = %d", r );
						AddMsg ( MsgStr );
					}
				}

				PlayTone ( &pOneUser->VocDevID, 2 );		// busy tone

				Change_UserState ( pOneUser, USER_WAITHANGUP );
			}
		}
		break;

	case USER_CALLOUT:
		break;

	case USER_LINK:
		break;

	case USER_WAITHANGUP:
		break;

	// User to User's call
	case USER_RING://��ϯ�û���Ϊ��������ʱ����FSK����
		// send FSK Caller ID
		if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )
		{
			if ( SearchOneFreeVoice ( pOneUser,  &FreeVocDeviceID ) >= 0 )
			{
				pOneUser->VocDevID = FreeVocDeviceID;

				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneUser->deviceID; 

				DrawUser_VocInfo ( pOneUser );

				My_DualLink ( &FreeVocDeviceID, &pOneUser->deviceID );
				
				PlayFSKCallerID ( &pOneUser->VocDevID, cfg_CallingNum );
			}
			//
		}

		if ( CheckPlayFSKEnd ( pOneUser, pAcsEvt ) )
		{
			// free voice 
			StopPlayFSKCallerID ( &pOneUser->VocDevID );
			if ( pOneUser->VocDevID.m_s16DeviceMain != 0 )
			{
				My_DualUnlink ( &pOneUser->VocDevID, &pOneUser->deviceID );

				FreeOneFreeVoice (  &pOneUser->VocDevID );

				memset ( &pOneUser->VocDevID, 0, sizeof(DeviceID_t) );		// 0: Didn't alloc Voc Device

				DrawUser_VocInfo ( pOneUser );
			}
		}

		// end of handle FSK Caller ID

		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	/*User Offhook Event*/
		{
			pLinkUser = &M_OneTrunk(pOneUser->LinkDevID);
			
			StopPlayTone ( &pLinkUser->VocDevID );
			if ( pLinkUser->VocDevID.m_s16DeviceMain != 0 )
			{
				My_DualUnlink ( &pLinkUser->VocDevID, &pLinkUser->deviceID );

				FreeOneFreeVoice (  &pLinkUser->VocDevID );

				memset ( &pLinkUser->VocDevID, 0, sizeof(DeviceID_t) );		// 0: Didn't alloc Voc Device

				DrawUser_VocInfo ( pLinkUser );
			}

			My_DualLink ( &pOneUser->deviceID, &pLinkUser->deviceID );

			Change_UserState ( pLinkUser, USER_LINK );
			Change_UserState ( pOneUser, USER_LINK );
		}
		break;
	}
}

void ChannelWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		UserWork ( pOneTrunk, pAcsEvt );
	else
		TrunkWork ( pOneTrunk, pAcsEvt );
}

void	SimulateCallOut(void)
{
	int				i;
	DeviceID_t		FreeTrkDeviceID;
	DeviceID_t		FreeVocDeviceID;
	TRUNK_STRUCT	*pLinkTrunk = NULL;
	RetCode_t		r;
	char			MsgStr[256];

	i = SearchOneFreeTrunk ( cfg_SimCalledNum,  &FreeTrkDeviceID );
	if ( i > 0 )
	{
		CString sCalled,sCalling;
		pdlg->GetDlgItemText(IDC_EDIT_SimCalledNum,sCalled);
		memset(cfg_SimCalledNum,0,sizeof(cfg_SimCalledNum));
		strcpy(cfg_SimCalledNum,(char*)(LPCSTR)sCalled);

		pdlg->GetDlgItemText(IDC_EDIT_CallingNum,sCalling);
		memset(cfg_CallingNum,0,sizeof(cfg_CallingNum));
		strcpy(cfg_CallingNum,(char*)(LPCSTR)sCalling);
		
		pLinkTrunk = &M_OneTrunk(FreeTrkDeviceID);

		// ====================Added for Analog Trunk
		if ( FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
		{
			if ( SearchOneFreeVoice ( pLinkTrunk,  &FreeVocDeviceID ) >= 0 )
			{
				pLinkTrunk->VocDevID = FreeVocDeviceID;
				M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkTrunk->deviceID; 

				My_DualLink(&FreeTrkDeviceID,&FreeVocDeviceID);
				SetGtd_AnalogTrunk(&FreeVocDeviceID);
			}
			else
			{
				MessageBox(NULL,"No VoiceDevice is Free!","SearchOneFreeVoice Failed!",MB_OK);
				return ;
			}
		}
		/************************************************************************/
		/* ԭʼ���к���                                                         */
		/***********************************************************************
		ISUP_spOriginalCalledNumber SP_ocn={0};
		SP_ocn.m_u8NatureOfAddressIndicator=3; //������Ч���룬����ʵ�������
		SP_ocn.m_u8OddEvenIndicator=0; //��ʹ�ã���������ֵ
		SP_ocn.m_u8AddressPresentationRestrictedIndicator=0; //��ʾ����
		SP_ocn.m_u8NumberingPlanIndicator=1; //ISDN �绰����ƻ���E.164��
		strcpy(SP_ocn.m_s8AddressSignal, "000000"); //ԭ���ı��У�С��16 λ
		if(XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,ISUP_SP_OriginalCalledNumber, sizeof(SP_ocn),&SP_ocn)<0)
		{
			return;
		}
		************************************************************************/

		/************************************************************************/
		/*���͸ķ��ĺ���RedirectingNumber                                       */
		/************************************************************************
		ISUP_spRedirectingNumber SP_ocn={0};
        SP_ocn.m_u8NatureOfAddressIndicator=3; //������Ч���룬����ʵ�������
        SP_ocn.m_u8OddEvenIndicator=0; //��ʹ�ã���������ֵ
        SP_ocn.m_u8NumberingPlanIndicator=1; //ISDN �绰����ƻ���E.164��
        strcpy(SP_ocn.m_s8AddressSignal, "8989"); //ԭ���ı��У�С��16 λ
        if(XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,ISUP_SP_RedirectingNumber, sizeof(SP_ocn),&SP_ocn)<0)
        {
            return;
        }
		************************************************************************/

		r = XMS_ctsMakeCallOut ( g_acsHandle, &FreeTrkDeviceID, cfg_CallingNum, cfg_SimCalledNum, NULL );
		if ( r > 0 )
		{
			DrawMain_LinkDev ( pLinkTrunk );
			strcpy ( pLinkTrunk->CallerCode, cfg_CallingNum );
			strcpy ( pLinkTrunk->CalleeCode, cfg_SimCalledNum );
			DrawMain_CallInfo ( pLinkTrunk );
			Change_State ( pLinkTrunk, TRK_SIM_CALLOUT );
		}else
		{
			sprintf ( MsgStr, "XMS_ctsMakeCallOut() FAIL! ret = %d", r );
			AddMsg ( MsgStr );
		}

		/************************************************************************/
		/*���ͺ�����ַ                                                          */
		/***********************************************************************
		ISUP_spSubsequentNumber SP_ssn={0};
        SP_ssn.m_u8OddEvenIndicator=0; //��ʹ�ã���������ֵ
        strcpy(SP_ssn.m_s8AddressSignal, "78");//������ַ��������16λ        
        //���ú�����ַ�źŵ�ֵ
        r = XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, ISUP_SP_SubsequentNumber,sizeof(SP_ssn), &SP_ssn);
        if (r < 0)
        {
            AddMsg("XMS_ctsSetParam:ISUP subsequent number fail.");
            return ;
        }          
        r = XMS_ctsSendSignalMsg(g_acsHandle, &FreeTrkDeviceID, ISUP_SM_SAM);        
        if( r < 0)
        {
            AddMsg("XMS_ctsSendSignalMsg:ISUP subsequent number fail");
            return ;
        }else{
            AddMsg("XMS_ctsSendSignalMsg:ISUP subsequent number first ok.....");
        }   
		************************************************************************/
	}

}
