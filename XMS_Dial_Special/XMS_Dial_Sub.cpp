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
#include "DJAcsTUPDef.h"
#include "ITPISDN.h"
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
int				cfg_iDispChnl = 32;		// always disp 32 channel
int				cfg_iVoiceRule = 0;		// search voice in Fix Relationship
int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;
int				cfg_s32DebugOn;
int				cfg_iCallOutRule = 0;
int				cfg_CallingAddrIndicator = 1;
int				cfg_CalledAddrIndicator = 1;
char			cfg_SimCalledNum[32] = "12345";
char			cfg_OriCalledNum[32] =	"111";
char			cfg_SubseqNum[32]	 = "22,33";
char			cfg_RedirectingNum[32] ="444";
char			cfg_VocPath[128] = "";
char			cfg_CallingNum[32] = "83636988";


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
	cfg_CallingAddrIndicator = GetPrivateProfileInt ( "ConfigInfo", "CallingAddrIndicator", 1, cfg_IniName);
	cfg_CalledAddrIndicator = GetPrivateProfileInt ( "ConfigInfo", "CalledAddrIndicator", 1, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "CallingNum", "83636988", cfg_CallingNum, sizeof(cfg_CallingNum), cfg_IniName);

	cfg_iCallOutRule = GetPrivateProfileInt ( "ConfigInfo", "CallOutRule", 0, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "SimCalledRNum", "12345", cfg_SimCalledNum, sizeof(cfg_SimCalledNum), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "OriCalledNum", "111", cfg_OriCalledNum, sizeof(cfg_OriCalledNum), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "SubseqNum", "22,33", cfg_SubseqNum, sizeof(cfg_SubseqNum), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "RedirectingNum", "444", cfg_RedirectingNum, sizeof(cfg_RedirectingNum), cfg_IniName);

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

	sprintf ( TmpStr, "%d", cfg_CallingAddrIndicator);
	WritePrivateProfileString ( "ConfigInfo", "CallingAddrIndicator", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_CalledAddrIndicator);
	WritePrivateProfileString ( "ConfigInfo", "CalledAddrIndicator", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CallingNum", cfg_CallingNum, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iCallOutRule);
	WritePrivateProfileString ( "ConfigInfo", "CallOutRule", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "SimCalledRNum", cfg_SimCalledNum, cfg_IniName);
	WritePrivateProfileString ( "ConfigInfo", "OriCalledNum", cfg_OriCalledNum, cfg_IniName);
	WritePrivateProfileString ( "ConfigInfo", "SubseqNum", cfg_SubseqNum, cfg_IniName);
	WritePrivateProfileString ( "ConfigInfo", "RedirectingNum", cfg_RedirectingNum, cfg_IniName);

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

	sprintf ( TmpStr, "%d", cfg_CallingAddrIndicator );
	pdlg->GetDlgItem ( IDC_EDIT_CallingAddrIndicator )->SetWindowText ( TmpStr );

	sprintf ( TmpStr, "%d", cfg_CalledAddrIndicator );
	pdlg->GetDlgItem ( IDC_EDIT_CalledAddrIndicator )->SetWindowText ( TmpStr );

	pdlg->GetDlgItem ( IDC_EDIT_CallingNum )->SetWindowText ( cfg_CallingNum );

	if ( cfg_iCallOutRule == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_Sequence))->SetCheck ( 1 ) ;
	else if ( cfg_iCallOutRule == 1 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_Loop))->SetCheck ( 1 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_ByDtmf))->SetCheck ( 1 ) ;

	pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum )->SetWindowText ( cfg_SimCalledNum );
	pdlg->GetDlgItem ( IDC_EDIT_ORICALNUM )->SetWindowText ( cfg_OriCalledNum );
	pdlg->GetDlgItem ( IDC_EDIT_REDNUM )->SetWindowText ( cfg_RedirectingNum );
	pdlg->GetDlgItem ( IDC_EDIT_SUBNUM )->SetWindowText ( cfg_SubseqNum );

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

	pdlg->GetDlgItem ( IDC_EDIT_CallingAddrIndicator )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_CallingAddrIndicator );
	
	pdlg->GetDlgItem ( IDC_EDIT_CalledAddrIndicator )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_CalledAddrIndicator );

	pdlg->GetDlgItem ( IDC_EDIT_CallingNum )->GetWindowText ( cfg_CallingNum, 30 );

	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Sequence))->GetCheck ( ) )
		 cfg_iCallOutRule = 0;
	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Loop))->GetCheck ( ) )
		 cfg_iCallOutRule = 1;
	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_ByDtmf))->GetCheck ( ) ) 
		 cfg_iCallOutRule = 2;
		
	pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum )->GetWindowText ( cfg_SimCalledNum, 30 );
	pdlg->GetDlgItem ( IDC_EDIT_ORICALNUM )->GetWindowText ( cfg_OriCalledNum, 30 );
	pdlg->GetDlgItem ( IDC_EDIT_SUBNUM )->GetWindowText ( cfg_SubseqNum, 30 );
	pdlg->GetDlgItem ( IDC_EDIT_REDNUM )->GetWindowText ( cfg_RedirectingNum, 30 );

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
Function:		设置语音设备GTD使能(使能检测Tone音/FSK/DTMF/PVD)
Input:			DeviceID_t* pDevId				语音设备DeviceID
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

	// now open ACS Server   建立APP与Keygoe流程模块的链接
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

	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );        //设置回调
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
	
	XMS_acsGetDeviceList ( g_acsHandle, NULL );   //获取设备资源列表

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

	r = XMS_acsCloseStream ( g_acsHandle, NULL );  //释放APP与流程模块的连接

	FreeAllDeviceRes ();

	// save to "XMS_Dial.INI"
	FetchFromText();
	WriteToConfig();

}

// --------------------------------------------------------------------------------
/**********************************************************************
Function:		打开获取到资源列表中的中继设备
Input:			TRUNK_STRUCT *pOneTrunk  中继设备结构 ;   	
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
/************************************************************************/
/* Function: XMS_EVT_DEVICESTATE事件处理函数                            */
/************************************************************************/
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

	/************************************************************************/
	/* ISDN发送后续地址                                                     */
	/************************************************************************/
    if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
		 pAcsEvt->m_DeviceID.m_s16DeviceSub == XMS_DEVSUB_E1_DSS1 )
    {
        pOneTrunk = &M_OneTrunk(pAcsEvt->m_DeviceID);		
        pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
		if (0x705 == pGeneralData->m_s32DeviceState)
        {
			CString subsequentNum;		//后续地址
			CString getItem;			//临时字符串
			char	*tempCh=NULL;
			char    tempCh1[32]={0};
			pdlg->GetDlgItemText(IDC_EDIT_SUBNUM,subsequentNum);
			
			while(!subsequentNum.IsEmpty())							//遍历字符串，先按逗号分开，存放到容器中
			{
				int pos = subsequentNum.Find(',');					//从第一个逗号开始查找，并返回逗号的位置
				if ( -1 == pos )									//没有找到逗号
				{
					tempCh=subsequentNum.GetBuffer(subsequentNum.GetLength());
					if ( tempCh[subsequentNum.GetLength()-1] == '.')
					{
						tempCh[subsequentNum.GetLength()-1]='\0';
						sprintf(tempCh1,"1%s",tempCh);
					}else
					{
						sprintf(tempCh1,"0%s",tempCh);
					}
					
					XMS_ctsSetParam(g_acsHandle, &pOneTrunk->deviceID, ISDN_PARAM_APPENDCALLEE,sizeof(tempCh1), tempCh1);					
					subsequentNum = "";
				}else
				{
					getItem = subsequentNum.Left(pos);				//获得逗号左边的字符串
					if(!getItem.IsEmpty())							//如果逗号间没有字符串，则不做任何处理
					{
						tempCh=getItem.GetBuffer(getItem.GetLength());
						sprintf(tempCh1,"0%s",tempCh);
						XMS_ctsSetParam(g_acsHandle, &pOneTrunk->deviceID, ISDN_PARAM_APPENDCALLEE,sizeof(tempCh1), tempCh1);					
					}
					subsequentNum = subsequentNum.Right(subsequentNum.GetLength()-pos-1); //截取该逗号以后的字符串，进入下一轮查找
				}
			} //end while 	
        }
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
Function:	 播放Tone音信号
Input:		 DeviceID_t	*pVocDevID  语音设备DeviceID ; 
			 iPlayType  Tone音类型   0  拨号音   1  回铃音   2   挂机忙音   -1  停止播放Tone音  
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
Function:		检查Tone音播放是否完成
Input:			TRUNK_STRUCT *pOneTrunk  中继设备结构 ;
				Acs_Evt_t *pAcsEvt       事件包结构头指针        	
Output:			True  播放完成  ;   False  未收到播放完成事件
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
Function:	送FSK制式主叫	
Input:		 DeviceID_t	*pVocDevID    语音设备DeviceID ;
			 char *CallerIDStr        需要发送的FSK主叫号码					       	
Output:		 r  函数执行结果          1  sussess ;  else  failure 	
Description: 
			 step 1   ConvertCallerIDToRawFsk()    将主叫号码转换成指定的FSK数据格式
			 step 2   XMS_ctsSendIOData ()         发送FSK数据
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
Function:      检查FSK数据是否发送完毕		
Input:		   TRUNK_STRUCT *pOneTrunk  中继设备结构 ;
			   Acs_Evt_t *pAcsEvt       事件包结构头指针  					       	
Output:		   True  发送完成  ;   False  未收到发送完成事件
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
Function:		发送DTMF双音频	
Input:			DeviceID_t	*pVocDevID    语音设备DeviceID ;
				char *CallerIDStr         需要发送的DTMF数据
Output:			r  函数执行结果          1  sussess ;  else  failure 			
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
Function:	  获取接收到的DTMF双音频按键	
Input:		  Acs_Evt_t *pAcsEvt       事件包结构头指针					       	
Output:		  *p					   DTMF双音频字符数据   		
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
Function:		检测Tone音信号和PVD	爆破音	
Input:			Acs_Evt_t *pAcsEvt    事件包结构头指针 				       	
Output:			Tone音或爆破音类型
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
Function:	  完成双向的时隙连接	
Input:		  DeviceID_t *pDev1	, DeviceID_t *pDev2     需进行双向连接的设备的DeviceID						       	
Output:		  Void	
Description:  
**********************************************************************/
void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}


/**********************************************************************
Function:	  断开双向的时隙连接	
Input:		  DeviceID_t *pDev1	, DeviceID_t *pDev2     需进行断开连接的设备的DeviceID						       	
Output:		  Void	
Description:  
**********************************************************************/
void	My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}


/**********************************************************************
Function:	  获取一个空闲的语音设备	
Input:		  TRUNK_STRUCT *pOneTrunk		中继设备结构 ;
			  DeviceID_t *pFreeVocDeviceID  该参数为Output参数，存放获取到的语音设备DeviceID 							       	
Output:		  DeviceID_t *pFreeVocDeviceID  如果获取成功，存放获取到的语音设备ID，否则为NULL 	
Description:  
			  cfg_iVoiceRule = 0           语音设备和中继设备建立一一绑定关系
			  cfg_iVoiceRule = 1		   获取与中继在同一DSP的语音设备
			  cfg_iVoiceRule = 2		   获取空闲语音数最多的DSP上的语音设备
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
Function:	  释放被占用的语音设备	
Input:		  DeviceID_t *pFreeVocDeviceID     需被释放的语音设备的DeviceID						       	
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
Function:	  完成CAS信令的MFC接续过程，除SS1之外的其他信令不需要调用	
Input:			TRUNK_STRUCT *pOneTrunk  中继设备结构 ;
				Acs_Evt_t *pAcsEvt       事件包结构头指针  Output:					  
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
Function:	  设置Tone音检测模板    类型、频率、响停时间等	
Input:		  DeviceID_t *pDevice     Board设备的DeviceID						       	
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

	case TRK_CALLOUT://获取到外呼成功事件，双向中继通道与坐席通道
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);

		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( pCallControl->m_s32AcsEvtState == 1 )	// Call Out OK，Callee Offhook
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

	case TRK_SIM_CALLOUT://自动外呼(SIM_CALLOUT)成功，双向Link语音设备
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

			if ( pCallControl->m_s32AcsEvtState == 1 )	// Call Out OK，Callee Offhook
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

	case TRK_SIM_LINK://播放DTMF双音频"12345"
		{
			if ( pAcsEvt->m_s32EventType == XMS_EVT_LINKDEVICE)
			{

				PlayDTMF ( &pOneTrunk->VocDevID, "12345" );		// play dtmf
				
				Change_State ( pOneTrunk, TRK_PLAYDTMF );
			}
		}
		break;
	case TRK_SIM_ANALOG_OFFHOOK:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )//模拟中继外呼,检测不到拨号音超时挂机
		{
			// time out
			DrawMain_Analog_Reason ( pOneTrunk, "Wait Dial Tone TimeOut" ); 

			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

			Change_State ( pOneTrunk, TRK_HANGUP );
		}

		TmpGtd = My_GetGtdOrPVDCode ( pAcsEvt );
		if ( TmpGtd != -1 )
		{
			if ( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )		// Busy Tone模拟中继外呼检测到忙音挂机
			{	
				sprintf ( TmpStr, "Busy Tone : %c", TmpGtd );
				DrawMain_Analog_Reason ( pOneTrunk, TmpStr ); 

				XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			

				Change_State ( pOneTrunk, TRK_HANGUP );
			}
			else if ( TmpGtd == 'G' )		// Dial Tone  模拟中继外呼时,检测到拨号音然后发送DTMF双音频
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
		if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )   //模拟中继外呼被叫未响应超时
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
		if ( CheckPlayGTGEnd ( pOneTrunk, pAcsEvt) )	/*play Over Event双音频播放完成后挂机*/
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
Function:		获取一个空闲的语音资源
Input:			char *pDtmf						外呼的被叫号码
				DeviceID_t *pFreeTrkDeviceID	获取到的空闲中继DeviceID  (该参数为Output参数)
Output:			DeviceID_t *pFreeTrkDeviceID	获取到的空闲中继DeviceID;
				-1								未获取到满足条件的空闲中继
Description:	
				cfg_iCallOutRule = 2				通过被叫号码后3位选择匹配通道
				cfg_iCallOutRule = 0				始终从0通道开始查找
				cfg_iCallOutRule = 1				从0通道往后循环查找
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
	case USER_FREE://等待坐席用户摘机，并播放拨号音
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

	case USER_OFFHOOK://检测到用户按键之后停止放拨号音;收齐按键之后选择对应中继通道呼出，并播放回铃音
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
	case USER_RING://坐席用户作为被叫振铃时，送FSK主叫
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
/************************************************************************/
/* Function:外呼函数                                                    */
/************************************************************************/
void SimulateCallOut(void)
{
	int				i;
	DeviceID_t		FreeTrkDeviceID;
	DeviceID_t		FreeVocDeviceID;
	TRUNK_STRUCT	*pLinkTrunk = NULL;
	RetCode_t		r;
	char			MsgStr[256];
	char			TmpStr[10];
	i = SearchOneFreeTrunk ( cfg_SimCalledNum,  &FreeTrkDeviceID );
	if ( i > 0 )
	{
		CString sCalled;			//被叫号码
		CString sCalling;			//主叫号码
		CString redirectingNum;		//改发的号码
		CString subsequentNum;		//后续地址
		CString oriCalledNumber;	//原始被叫号码
		
		pdlg->GetDlgItemText(IDC_EDIT_SimCalledNum,sCalled);
		memset(cfg_SimCalledNum,0,sizeof(cfg_SimCalledNum));
		strcpy(cfg_SimCalledNum,(char*)(LPCSTR)sCalled);

		pdlg->GetDlgItemText(IDC_EDIT_CallingNum,sCalling);
		memset(cfg_CallingNum,0,sizeof(cfg_CallingNum));
		strcpy(cfg_CallingNum,(char*)(LPCSTR)sCalling);
		
		pdlg->GetDlgItemText(IDC_EDIT_REDNUM,redirectingNum);

		pdlg->GetDlgItemText(IDC_EDIT_SUBNUM,subsequentNum);
		
		pdlg->GetDlgItemText(IDC_EDIT_ORICALNUM,oriCalledNumber);

		pdlg->GetDlgItem ( IDC_EDIT_ORICALNUM )->GetWindowText ( TmpStr, 30 );
		sscanf ( TmpStr, "%s", &cfg_OriCalledNum );

		pdlg->GetDlgItem ( IDC_EDIT_CallingAddrIndicator )->GetWindowText ( TmpStr, 30 );
		sscanf ( TmpStr, "%d", &cfg_CallingAddrIndicator );
		
		pdlg->GetDlgItem ( IDC_EDIT_CalledAddrIndicator )->GetWindowText ( TmpStr, 30 );
		sscanf ( TmpStr, "%d", &cfg_CalledAddrIndicator );
		
		pLinkTrunk = &M_OneTrunk(FreeTrkDeviceID);

		if ( FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )		//Analog Trunk
		{
			if ( SearchOneFreeVoice ( pLinkTrunk,  &FreeVocDeviceID ) >= 0 )
			{
				pLinkTrunk->VocDevID = FreeVocDeviceID;
				M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkTrunk->deviceID; 

				My_DualLink(&FreeTrkDeviceID,&FreeVocDeviceID);
				SetGtd_AnalogTrunk(&FreeVocDeviceID);
			}else
			{
				MessageBox(NULL,"No VoiceDevice is Free!","SearchOneFreeVoice Failed!",MB_OK);
				return ;
			}
		}else if ( FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_ISUP )
		{
			/************************************************************************/
			/* 原始被叫号码  OriginalCalledNumber                                   */
			/************************************************************************/
			ISUP_spOriginalCalledNumber SP_ocn={0};
			SP_ocn.m_u8NatureOfAddressIndicator=3; //国内有效号码，根据实际情况填
			SP_ocn.m_u8OddEvenIndicator=0; //不使用，可填任意值
			SP_ocn.m_u8AddressPresentationRestrictedIndicator=0; //显示允许
			SP_ocn.m_u8NumberingPlanIndicator=1; //ISDN 电话号码计划（E.164）
			strcpy(SP_ocn.m_s8AddressSignal, oriCalledNumber); //原来的被叫，小于16 位
			if (!oriCalledNumber.IsEmpty())
			{
				if(XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,ISUP_SP_OriginalCalledNumber, sizeof(SP_ocn),&SP_ocn)<0)
				{
					return;
				}
			}
			/************************************************************************/
			/*发送改发的号码RedirectingNumber                                       */
			/************************************************************************/
			ISUP_spRedirectingNumber redNum={0};
			redNum.m_u8NatureOfAddressIndicator=3; //国内有效号码，根据实际情况填
			redNum.m_u8OddEvenIndicator=0; //不使用，可填任意值
			redNum.m_u8NumberingPlanIndicator=1; //ISDN 电话号码计划（E.164）
			strcpy(redNum.m_s8AddressSignal, redirectingNum); //原来的被叫，小于16 位
			if (!redirectingNum.IsEmpty())
			{
				if (XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,ISUP_SP_RedirectingNumber, sizeof(redNum),&redNum)<0)
				{
					return;
				}
			}
			/************************************************************************/
			/* 前向呼叫指示码                                                       */
			/************************************************************************/
			ISUP_spForwardCallIndicator forCalInd={0};
			forCalInd.m_u8NationalInternatoinalIndicator = 0;//0：呼叫作为一个国内呼叫  1：呼叫作为一个国际呼叫
			forCalInd.m_u8EndToEndMethodIndicator = 0;//0：端到端方法不可获得 1：传递方法可获得 2：SCCP方法可获得 3：传递及SCCP方法都可获得
			forCalInd.m_u8InterworkingIndicator = 0;//0：不会碰到互通 1：会碰到互通
			forCalInd.m_u8EndToEndInformationIndicator = 0;//0：端到端信息不可获得 1：端到端信息可获得
			forCalInd.m_u8ISDNUserPartIndicator = 0; //0：非全程应用ISDN用户部分 1：全程应用ISDN用户部分
			forCalInd.m_u8ISDNUserPartPreferenceIndicator = 1;//0：ISDN用户部分全程优先1：非全程需要ISDN用户部分2：全程需要ISDN用户部分3：备用
			forCalInd.m_u8ISDNAccessIndicator = 1;//0：始发接入非ISDN  1：始发接入ISDN
			forCalInd.m_u8SCCPMethodIndicator = 0;//0：无指示 1：无接续方法可获得	2：面向接续方法可获得 3：无接续及面向接续方法都可获得
			XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID,ISUP_SP_ForwardCallIndicator, sizeof(ISUP_SP_ForwardCallIndicator), &forCalInd);

			/************************************************************************/
			/* 主叫用户类别
			0：来源未知							1：话务员，法语
			2：话务员，英语						3：话务员，德语
			4：话务员，俄语						5：话务员，西班牙语
			6：双方协商采用的语言（汉语）		7：双方协商采用的语言
			8：双方协商采用的语言（日语）		9：国内话务员（具有插入性能）
			10：普通用户，长－长（市）间使用	11：优先用户
			12：数据呼叫						13：测试呼叫
			16：普通、免费						17：普通、定期
			18：普通，用户表、立即				19：普通，打印机、立即
			20：优先、免费						21：优先、定期
			24：普通用户，在市－市间使用
			其它：备用																*/
			/************************************************************************/
			ISUP_spCallingPartCategory _spCallingPartCategory;
			_spCallingPartCategory.m_u8CallingPartyCategory = 10;
			XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID,ISUP_SP_CallingPartCategory, sizeof(ISUP_spCallingPartCategory), &_spCallingPartCategory);

			/************************************************************************/
			/* 主叫用户号码---地址性质指示码
			0：备用	1：用户号码	2：不知	3：国内（有效）号码	4：国际号码				*/
			/************************************************************************/
			ISUP_spCallingPartNumber callingParNum={0};
			callingParNum.m_u8NatureOfAddressIndicator=cfg_CallingAddrIndicator;//地址性质指示码,用户号码，根据实际情况填
			callingParNum.m_u8OddEvenIndicator=0;							//不使用，可填任意值
			callingParNum.m_u8Screening=3;									//网络提供，一般都取该值
			callingParNum.m_u8AddressPresentationRestrictedIndicator=0;		//显示允许
			callingParNum.m_u8NumberingPlanIndicator=1;						//ISDN电话号码计划(E.164)
			callingParNum.m_u8NumberIncompleteIndicator=0;					//主叫不全指示：完全
			strcpy(callingParNum.m_s8AddressSignal, cfg_CallingNum);		//完整主叫，小于32位
			if (XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, ISUP_SP_CallingPartNumber, sizeof(callingParNum), &callingParNum)<0)
			{
				return;
			}
			/************************************************************************/
			/* 被叫用户号码---地址性质指示码
			0：备用	1：用户号码	2：不知	3：国内（有效）号码	4：国际号码				*/
			/************************************************************************/
			ISUP_spCalledPartNumber calledParNum={0};
			calledParNum.m_u8NatureOfAddressIndicator=cfg_CalledAddrIndicator;//地址性质指示码,用户号码，根据实际情况填
			calledParNum.m_u8OddEvenIndicator=0;							//不使用，可填任意值
			calledParNum.m_u8NumberingPlanIndicator=1;						//ISDN电话号码计划(E.164)
			strcpy(calledParNum.m_s8AddressSignal, cfg_SimCalledNum);		//被叫号码
			if (XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, ISUP_SP_CalledPartNumber, sizeof(calledParNum), &calledParNum)<0)
			{
				return;
			}
		}else if (FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_TUP)
		{
			/************************************************************************/
			/* TUP原始被叫号码  OriginalCalledNumber                                */
			/************************************************************************/
			TUP_spIaiFirstIndicator  TUP_FirInd={0};
			TUP_FirInd.m_u8CallingLineIdentityIndicator = 1;
			TUP_FirInd.m_u8OriginalCalledAddressIndicator = 1;
			if(XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID,TUP_SP_IaiFirstIndicator, sizeof(TUP_FirInd), &TUP_FirInd)<0)
			{
				return;
			}
			TUP_spOriginalCalledAddress TUP_oriCalledNum={0};
			TUP_oriCalledNum.m_u8NatureOfAddressIndicator=3;				//国内有效号码，根据实际情况填
			strcpy(TUP_oriCalledNum.m_s8AddressSignal, oriCalledNumber);	//原来的被叫，小于16 位
			if (!oriCalledNumber.IsEmpty())
			{
				if(XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,TUP_SP_OriginalCalledAddress, sizeof(TUP_oriCalledNum),&TUP_oriCalledNum)<0)
				{
					return;
				}
			}			
			/************************************************************************/
			/* TUP主叫用户号码---地址性质指示码
			0：市内用户号码	1：备用	2：国内有效号码	3：国际号码						*/
			/************************************************************************/
			TUP_spCallingLineAddress TUP_CallingLineAddr={0};
			TUP_CallingLineAddr.m_u8NatureOfAddressIndicator=cfg_CallingAddrIndicator;	//地址性质表示语
			TUP_CallingLineAddr.m_u8NumberOfAddressSignal=0;							//主叫地址信号的数量
			TUP_CallingLineAddr.m_u8PresentationIndicator=0;							//提供主叫用户线标识表示语
			TUP_CallingLineAddr.m_u8IncompleteIndicator=0;								//主叫用户线标识不全表示语
			strcpy(TUP_CallingLineAddr.m_s8AddressSignal, cfg_CallingNum);				//完整主叫，小于32位
			if (XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, TUP_SP_CallingLineAddress, sizeof(TUP_CallingLineAddr), &TUP_CallingLineAddr)<0)
			{
				return;
			}
			/************************************************************************/
			/* TUP被叫用户号码---地址性质指示码
			0：市内用户号码	1：备用	2：国内有效号码	3：国际号码						*/
			/************************************************************************/
			TUP_spCalledPartAddress TUP_CalledPartAddr={0};
			TUP_CalledPartAddr.m_u8NatureOfAddressIndicator=cfg_CalledAddrIndicator;	//地址性质指示码,用户号码，根据实际情况填
			TUP_CalledPartAddr.m_u8NumberOfAddressSignal=1;								//被叫号码的数量
			strcpy(TUP_CalledPartAddr.m_s8AddressSignal, cfg_SimCalledNum);				//被叫号码
			if (XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, TUP_SP_CalledPartAddress, sizeof(TUP_CalledPartAddr), &TUP_CalledPartAddr)<0)
			{
				return;
			}
		}else if (FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_E1_DSS1)
		{
			/************************************************************************/
			/* ISDN原始被叫号码设置                                                 */
			/************************************************************************/
			XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,ISDN_PARAM_ORINUMBER,strlen(cfg_OriCalledNum),&cfg_OriCalledNum);
			/************************************************************************/
			/* ISDN设置呼叫类型                                                     */
			/************************************************************************/
			//XMS_ctsSetParam(g_acsHandle,& FreeTrkDeviceID,ISDN_PARAM_CALLTYPE,1,"1");
			
			/************************************************************************/
			/*ISDN 主叫号码类型 被叫号码类型
			0：未知	1：国际号码	2：国内号码	3：网络特定号码	4：用户号码	7：扩展保留*/
			/************************************************************************/
			ITP_Q931_CALL_PARAM ISDN_CallingNum={0};
			ISDN_CallingNum.m_u8CallerType = cfg_CallingAddrIndicator;	//主叫号码类型
			ISDN_CallingNum.m_u8CalleeType = cfg_CalledAddrIndicator;	//被叫号码类型
			XMS_ctsSetParam(g_acsHandle,&FreeTrkDeviceID,ISDN_PARAM_CALLOUT,sizeof(ITP_Q931_CALL_PARAM),&ISDN_CallingNum);
 
		}
		
		//开始进行外呼
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

		if ( FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_ISUP )
		{
			/************************************************************************/
			/*ISUP发送后续地址                                                      */
			/************************************************************************/
			ISUP_spSubsequentNumber SP_ssn={0};
			SP_ssn.m_u8OddEvenIndicator=0;							//不使用，可填任意值
			CString getItem;
			while(!subsequentNum.IsEmpty())							//遍历字符串，先按逗号分开，存放到容器中
			{
				int pos = subsequentNum.Find(',');					//从第一个逗号开始查找，并返回逗号的位置
				if ( -1 == pos )									//没有找到逗号
				{
					strcpy(SP_ssn.m_s8AddressSignal, subsequentNum);//后续地址，不超过16位    
					XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, ISUP_SP_SubsequentNumber,sizeof(SP_ssn), &SP_ssn);
					XMS_ctsSendSignalMsg(g_acsHandle, &FreeTrkDeviceID, ISUP_SM_SAM); 
					subsequentNum = "";
				}else
				{
					getItem = subsequentNum.Left(pos);				//获得逗号左边的字符串
					if(!getItem.IsEmpty())							//如果逗号间没有字符串，则不做任何处理
					{
						strcpy(SP_ssn.m_s8AddressSignal, getItem);	//后续地址，不超过16位 						
						XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, ISUP_SP_SubsequentNumber,sizeof(SP_ssn), &SP_ssn);
						XMS_ctsSendSignalMsg(g_acsHandle, &FreeTrkDeviceID, ISUP_SM_SAM);         
					}
					subsequentNum = subsequentNum.Right(subsequentNum.GetLength()-pos-1); //截取该逗号以后的字符串，进入下一轮查找
				}
			} //end while                                    
		}else if (FreeTrkDeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			FreeTrkDeviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_TUP)
		{
			/************************************************************************/
			/*TUP发送后续地址                                                       */
			/************************************************************************/
			TUP_spSubsequentAddress TUP_ssn={0};
			CString getItem;
			while(!subsequentNum.IsEmpty())							//遍历字符串，先按逗号分开，存放到容器中
			{
				int pos = subsequentNum.Find(',');					//从第一个逗号开始查找，并返回逗号的位置
				if ( -1 == pos )									//没有找到逗号
				{
					strcpy(TUP_ssn.m_s8AddressSignal, subsequentNum);//后续地址，不超过16位    
					XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, TUP_SP_SubsequentAddress,sizeof(TUP_ssn), &TUP_ssn);
					XMS_ctsSendSignalMsg(g_acsHandle, &FreeTrkDeviceID, TUP_SM_SAM); 
					subsequentNum = "";
				}else
				{
					getItem = subsequentNum.Left(pos);				//获得逗号左边的字符串
					if(!getItem.IsEmpty())							//如果逗号间没有字符串，则不做任何处理
					{
						strcpy(TUP_ssn.m_s8AddressSignal, getItem);	//后续地址，不超过16位 						
						XMS_ctsSetParam(g_acsHandle, &FreeTrkDeviceID, TUP_SP_SubsequentAddress,sizeof(TUP_ssn), &TUP_ssn);
						XMS_ctsSendSignalMsg(g_acsHandle, &FreeTrkDeviceID, TUP_SM_SAM);         
					}
					subsequentNum = subsequentNum.Right(subsequentNum.GetLength()-pos-1); //截取该逗号以后的字符串，进入下一轮查找
				}
			} //end while 
		}
	}
}
