#include "stdafx.h"
#include "XMS_Demo.h"
#include "XMS_DemoDlg.h"

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
#include "XMS_Demo_Sub.H"
#include "XMS_Demo_Event.H"
#include "XMS_Demo_String.H"

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
extern CXMS_DemoApp theApp;
CXMS_DemoDlg	*pdlg;

// var about XMS_Demo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Demo.INI";

ServerID_t		cfg_ServerID;
char			cfg_VocPath[128];
int				cfg_iDispChnl;
int				cfg_iVoiceRule;

int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;

int				cfg_s32DebugOn;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 2;

// var in XMS_Demo_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalPcm;
extern int						g_iTotalPcmOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM_IN_THIS_DEMO];

extern int						g_iTotalTrunk;
extern int						g_iTotalTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];

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

void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 );
void	My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 );
void	SetGtD_AnalogTrunk(DeviceID_t* pDevId);
// -----------------------------------------------------------------------
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );

	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);

	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);

	GetPrivateProfileString("ConfigInfo","UserName","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);

	cfg_iDispChnl = GetPrivateProfileInt ( "ConfigInfo", "DispChnl", 30, cfg_IniName);

	cfg_iVoiceRule = GetPrivateProfileInt ( "ConfigInfo", "VoiceRule", 0, cfg_IniName);

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
	
	WritePrivateProfileString ( "ConfigInfo", "VocPath", cfg_VocPath, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iDispChnl);
	WritePrivateProfileString ( "ConfigInfo", "DispChnl", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iVoiceRule);
	WritePrivateProfileString ( "ConfigInfo", "VoiceRule", TmpStr, cfg_IniName);

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

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->SetWindowText ( cfg_VocPath );

	if ( cfg_iDispChnl == 30 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO30))->SetCheck ( 1 ) ;
	else if ( cfg_iDispChnl == 31 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO31))->SetCheck ( 1 ) ;
	else if ( cfg_iDispChnl == 32 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO32))->SetCheck ( 1 ) ;

	if ( cfg_iVoiceRule == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_Fix))->SetCheck ( 1 ) ;
	else if ( cfg_iVoiceRule == 1 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_SameModule))->SetCheck ( 1 ) ;
	else 
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_MostFreeFist))->SetCheck ( 1 ) ;

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

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->GetWindowText ( cfg_VocPath, 128 );

	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO30))->GetCheck ( ) == 1 )
	{
		cfg_iDispChnl = 30;
	}
	else if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO31))->GetCheck ( ) == 1 )
	{
		cfg_iDispChnl = 31;
	}
	else if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO32))->GetCheck ( ) == 1 )
	{
		cfg_iDispChnl = 32;
	}

	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Fix))->GetCheck ( ) )
		cfg_iVoiceRule = 0;
	else if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_SameModule))->GetCheck ( ) )
		cfg_iVoiceRule = 1;
	else
		cfg_iVoiceRule = 2;

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
	lvc.pszText = "DTMF" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "VocInfo" ;
	lvc.cx = 70;
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
				
	case TRK_WELCOME:
		strcpy(StateStr,"Welcome to DonJin Keygo Bank Demo");
		break ;
				
	case TRK_ACCOUNT:
		strcpy(StateStr,"Please input your acount") ;
		break ;

	case TRK_PASSWORD:
		strcpy(StateStr,"Please input your password");
		break;

	case TRK_SELECT:
		strcpy(StateStr,"Please select function");
		break;

	case TRK_RECORDFILE:
		strcpy(StateStr,"Record");
		break;

	case TRK_PLAYRESULT:
		strcpy(StateStr,"Play result");
		break;

	case TRK_PLAYRECORD:
		strcpy(StateStr,"Play Back");
		break;

	case TRK_WAIT_ANSWERCALL:
		strcpy(StateStr,"Wait offhook ok");
		break;

	case TRK_WAIT_LINKOK:
		strcpy(StateStr,"Wait Link ok");
		break;

	case TRK_WAIT_REMOVE:
		strcpy(StateStr,"DSP remove");
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

void DrawMain_DTMF( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	strcpy(StateStr,pOneTrunk->DtmfBuf);
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 

}

void DrawMain_VocInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		pDev = &pOneTrunk->VocDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}
	else
		strcpy(StateStr, "");

	pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 

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
	lvc.pszText = "ErrFlag" ;
	lvc.cx = 60;
	pdlg->m_ListCount.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "RemoveState" ;
	lvc.cx = 80;
	pdlg->m_ListCount.InsertColumn ( 7, &lvc ); 
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

void	DrawCount_ErrFlag ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	if ( AllDeviceRes[s8ModID].bErrFlag )
		sprintf ( TmpStr, "T" );
	else
		sprintf ( TmpStr, "" );

	pdlg->m_ListCount.SetItemText ( iDispRow, 6, TmpStr );
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

	pdlg->m_ListCount.SetItemText ( iDispRow, 7, TmpStr );
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
		DrawMain_CallInfo ( pOneTrunk );
		DrawMain_DTMF ( pOneTrunk );
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

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_DemoDlg	*)theApp.m_pMainWnd;

	// Read From "XMS_Demo.INI"
	ReadFromConfig();

	// Special code for CAS(SS1)
	// Read From "C:\\DJKeygoe\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI"
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

	// Init m_ListPcm
	InitListPcm();

	// Init m_ListMsg
	InitListMsg();

	// Init m_ListCount
	InitListCount();

	// Init Conifg's text box
	InitTextBox();

	// now open ACS Server   建立APP与流程模块之间的连接
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

	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );   //设置回调函数
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
	int			i;

	// close all device
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		CloseAllDevice_Dsp ( MapTable_Module[i] );
	}

	r = XMS_acsCloseStream ( g_acsHandle, NULL );  //断开业务程序与流程模块之间的连接

	FreeAllDeviceRes ();

	// save to "XMS_Demo.INI"
	FetchFromText();
	WriteToConfig();

}

// --------------------------------------------------------------------------------
/********************************************************************************
Function:		打开获取到的中继设备资源
Input:			TRUNK_STRUCT *pOneTrunk			中继通道结构；
Output:			Void
Description:
********************************************************************************/
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

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = true;

		SetGTD_ToneParam ( pDevice);
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);

		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation

		// init this Device: Trunk
		InitTrunkChannel ( pOneTrunk );

		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

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
			cmdAnalogTrunk.m_u16CallInRingCount = 3;			// 3 times RingBack
			cmdAnalogTrunk.m_u16CallInRingTimeOut = 6000;		// 60 seconds

			p = (void*)&cmdAnalogTrunk;

			RetCode_t r ;
			r = XMS_ctsSetParam( g_acsHandle, & pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
			char szbuffer[1024];
			memset(szbuffer,0,sizeof(szbuffer));
			sprintf(szbuffer,"Set AnalogTrunk  ret = %d\n",r);
			AddMsg ( szbuffer );

			// Search Free Voice for get CallerID(FSK)
			// if you needn't CallerID, ignore next line
			PrepareForCallerID ( pOneTrunk );
		}
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		M_OneVoice(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important

		// init this Device: Voice
		Change_Voc_State ( &M_OneVoice(*pDevice), VOC_FREE);

		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

		// Build Index
		if ( g_iTotalVoiceOpened == 0 )
		{
			My_BuildIndex ( pDevice );
		}

		// modify the count
		g_iTotalVoiceOpened ++;
		g_iTotalVoiceFree ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;

		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		M_OnePcm(*pDevice).bOpenFlag = true;

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

		// modify the count
		g_iTotalTrunkOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened --;

		DrawCount_Trunk ( pDevice->m_s8ModuleID  );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		// init this Device: Voice
		Change_Voc_State ( &M_OneVoice(*pDevice), VOC_WAITOPEN);

		// modify the count
		g_iTotalVoiceOpened --;
		g_iTotalVoiceFree --;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum --;

		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).bOpenFlag = false;

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
			&& (AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAIT_REMOVE) )
		{
			return;
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




/********************************************************************************
Function:		建立全局放音索引
Input:			DeviceID_t	*pVocDevID			语音设备的DeviceID
Output:			Void
Description:
********************************************************************************/
void	My_BuildIndex ( DeviceID_t	*pVocDevID )
{
	RetCode_t	r;
	char		FileName[256], MsgStr[256];
	int			i;
	PlayProperty_t  playProperty;
	
	r = XMS_ctsInitPlayIndex ( g_acsHandle, pVocDevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_ctsInitPlayIndex() FAIL. ret = r", r );
		AddMsg ( MsgStr );
		return ;
	}

	memset ( &playProperty, 0, sizeof(playProperty) );
	for ( i = 0; i < 17; i ++ )
	{
		if ( i == 16 )
		{
			playProperty.m_u16PlayType = XMS_BUILD_INDEX_FILE;
			sprintf ( FileName, "%s\\bank.008", cfg_VocPath );
		}
		else
		{
			playProperty.m_u16PlayType = XMS_BUILD_INDEX_RAM;
			sprintf ( FileName, "%s\\D%d", cfg_VocPath, i );
		}

		strcpy ( playProperty.m_s8PlayContent, FileName );
	
		r = XMS_ctsBuildPlayIndex ( g_acsHandle, pVocDevID, &playProperty, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "XMS_ctsBuildPlayIndex(%s) FAIL. ret = r", FileName, r );
			AddMsg ( MsgStr );
		}
	}

}

/* Stop Play */
DJ_S32 StopPlayFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;

	controlPlay.m_u16ControlType = XMS_STOP_PLAY;

	r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );

	return r;
}

/********************************************************************************
Function:		文件放音
Input:			DeviceID_t	*pVocDevID			语音设备的DeviceID;
				DJ_S8 *s8FileName				语音文件路径;
				DJ_U8 u8PlayTag					文件放音类型(XMS_PLAY_TYPE_FILE_QUEUE 文件队列/XMS_PLAY_TYPE_FILE普通文件放音);
				bool bIsQueue					文件放音的TaskID
Output:			1	success   ;  else  failure 
Description:
********************************************************************************/
DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue )
{	
	DJ_U32           i = 0;	
	PlayProperty_t  playProperty;
	RetCode_t		 r;

	memset(&playProperty,0,sizeof(playProperty));		
	
	if ( bIsQueue )
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE_QUEUE;	
	else
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;	

	playProperty.m_u8TaskID = u8PlayTag;

	strcpy ( playProperty.m_s8PlayContent, s8FileName );
	
	r = XMS_ctsPlay ( g_acsHandle, pVocDevID, &playProperty, NULL );

	return r;
}

DJ_S32	 PlayIndex ( DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, bool bIsQueue )
{	
	DJ_U32           i = 0;	
	PlayProperty_t  playProperty;
	RetCode_t		 r;

	memset(&playProperty,0,sizeof(playProperty));		
	
	if ( bIsQueue )
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_INDEX_QUEUE;	
	else
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_INDEX;	

	playProperty.m_u8TaskID = u8PlayTag;

	playProperty.m_u16PlayIndex = u16Index;
	
	r = XMS_ctsPlay ( g_acsHandle, pVocDevID, &playProperty, NULL );

	return r;
}


/********************************************************************************
Function:		检查放音是否完成
Input:			TRUNK_STRUCT *pOneTrunk 	中继通道结构;
				Acs_Evt_t *pAcsEvt			返回事件头指针
Output:			True  放音完成  ;   false  不是放音完成事件
Description:
********************************************************************************/
bool	CheckPlayEnd ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_PLAY )	
	{
		pMediaData = (Acs_MediaProc_Data *) FetchEventData(pAcsEvt);
		if ( pMediaData->m_u8TaskID == pOneTrunk->u8PlayTag )
			return true;
	}

	return false;
}

/* Stop Record */
DJ_S32 StopRecordFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlRecord_t	controlRecord;

	controlRecord.m_u32ControlType = XMS_STOP_RECORD;

	r = XMS_ctsControlRecord ( g_acsHandle, pVocDevID, &controlRecord, NULL );

	return r;
}

/********************************************************************************
Function:		文件录音
Input:			DeviceID_t	*pVocDevID			语音设备的DeviceID;
				DJ_S8 *s8FileName				语音文件路径;
				DJ_U32 u32RecSize				录音文件长度(0为不指定长度);
				bool bIsAppend					是否追加写入(0 重新创建录音文件; 1追加写入)
Output:			1	success   ;  else  failure 
Description:
********************************************************************************/
DJ_S32	 RecordFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend )
{	
	DJ_U32				i = 0;	
	RecordProperty_t	recordProperty;
	RetCode_t			r;

	memset(&recordProperty,0,sizeof(RecordProperty_t));		
	
	recordProperty.m_u32MaxSize = u32RecSize;

	if ( bIsAppend )
		recordProperty.m_s8IsAppend = 1;
	else
		recordProperty.m_s8IsAppend = 0;

	strcpy ( recordProperty.m_s8FileName, s8FileName );

	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &recordProperty, NULL );

	return r;
}




/********************************************************************************
Function:		检查录音是否完成
Input:			TRUNK_STRUCT *pOneTrunk 	中继通道结构;
				Acs_Evt_t *pAcsEvt			返回事件头指针
Output:			True  录音完成(停止)  ;   false  不是录音完成(停止)事件
Description:
********************************************************************************/
bool	CheckRecordEnd ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECORD )	
	{
		return true;
	}

	return false;
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


/********************************************************************************
Function:		获取DTMF双音频(用户按键)
Input:			Acs_Evt_t *pAcsEvt			返回事件头指针
Output:			char *   双音频数据 ;  -1  不是双音频事件
Description:
********************************************************************************/
char My_GetDtmfCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char   sbuffer[200];
	char		*p;

	memset(sbuffer,0,sizeof(sbuffer));
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_DTMF ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			sprintf(sbuffer,"[%s]",p);
			AddMsg(sbuffer);

			return *p;
		}
	}

	return -1;	// not a good DTMF
}


/********************************************************************************
Function:		获取FSK数据
Input:			Acs_Evt_t *pAcsEvt			返回事件头指针
Output:			char *   FSK数据 ;  NULL	不是正常的FSK事件
Description:
********************************************************************************/
unsigned char *My_GetFskCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	unsigned char		*p;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Evemt*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_FSK ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (unsigned char *)FetchIOData(pAcsEvt);

			return p;
		}
	}

	return NULL;	// not a good FSK
}


/********************************************************************************
Function:		获取Tone音数据类型(GTD)
Input:			Acs_Evt_t *pAcsEvt			返回事件头指针
Output:			char *   Tone音数据类型( G 拨号音 H  回铃音  I  挂机忙音) ;  -1	不是正常的Tone音事件
Description:
********************************************************************************/
char My_GetGtdCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char   sbuffer[200];
	char		*p;

	memset(sbuffer,0,sizeof(sbuffer));
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_GTG ) 
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

void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}

void	My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}


/********************************************************************************
Function:		获取一个空闲的语音资源
Input:			TRUNK_STRUCT *pOneTrunk			中继通道结构;
				DeviceID_t *pFreeVocDeviceID    获取到的空闲语音DeviceID  (该参数为Output参数)
Output:			DeviceID_t *pFreeVocDeviceID	获取到的空闲语音DeviceID;
				-1								未获取到满足条件的空闲语音
Description:	
				cfg_iVoiceRule = 0				寻找与中继通道设备对应通道号的语音资源一一捆绑
				cfg_iVoiceRule = 1				在与中继通道设备同一DSP上查找空闲语音资源
				cfg_iVoiceRule = 2				在空闲语音资源数最多的DSP上查找空闲语音资源
********************************************************************************/
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

			// user this voice device 
			Change_Voc_State ( pOneVoice, VOC_USED);
			AllDeviceRes[s8SearchModID].lVocFreeNum--;
			g_iTotalVoiceFree --;
			DrawCount_Voc ( s8SearchModID );
			return i;
		}
	}

	return -1;
}

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

// Search Free Voice for get CallerID(FSK)
/********************************************************************************
Function:		准备获取FSK主叫号码 (在呼叫产生之前调用,只对模拟中继有效)
Input:			TRUNK_STRUCT *pOneTrunk				中继通道结构
Output:			Void
Description:	
				step 1    获取空闲语音
				step 2    语音与模拟中继设备进行双向连接
				step 3    GTD使能
********************************************************************************/
void	PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk )
{
	DeviceID_t FreeVocDeviceID;

	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )
		return;

	if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
	{
		pOneTrunk->VocDevID = FreeVocDeviceID;

		M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 

		DrawMain_VocInfo ( pOneTrunk );

		My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );
		SetGtD_AnalogTrunk(&FreeVocDeviceID);
	}
}

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
	Change_State ( pOneTrunk, TRK_FREE );

	pOneTrunk->CallerCode[0]=0;
	pOneTrunk->CalleeCode[0]=0;

	My_InitDtmfBuf ( pOneTrunk );
	DrawMain_DTMF ( pOneTrunk );

	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	DrawMain_VocInfo ( pOneTrunk );
}

void ResetTrunk ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	StopPlayFile ( &pOneTrunk->VocDevID );

	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );

		FreeOneFreeVoice (  &pOneTrunk->VocDevID );

		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
		DrawMain_VocInfo ( pOneTrunk );
	}

	InitTrunkChannel ( pOneTrunk );

	// Search Free Voice for get CallerID(FSK)
	// if you needn't CallerID, ignore next line
	PrepareForCallerID ( pOneTrunk );
}

// Special code for CAS(SS1)
/********************************************************************************
Function:		完成MFC接续(只针对SS1有效)
Input:			TRUNK_STRUCT *pOneTrunk				中继通道结构
				Acs_Evt_t *pAcsEvt					返回事件头指针
Output:			Void
Description:	
				step 1    获取空闲语音
				step 2    语音与中继设备进行双向连接
				step 3    XMS_ctsSetParam完成 MFC接续
********************************************************************************/
void	SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t				FreeVocDeviceID;

	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_E1_CAS )
		return;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_START )	/* start receive MFC, need a Voice Device */
	{
		if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
		{
			// add next line from V2.2.0, link by Application Program
			My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );

			pOneTrunk->VocDevID = FreeVocDeviceID;
			M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
			DrawMain_VocInfo ( pOneTrunk );

			g_Param_CAS.m_VocDevID = FreeVocDeviceID;
			XMS_ctsSetParam ( g_acsHandle, &pOneTrunk->deviceID, 
				CAS_PARAM_UNIPARAM, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );
		}
	}

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_END )	/* receive MFC end, release Voice Device */
	{
		// add next line from V2.2.0, unlink by Application Program
		My_DualUnlink(&pOneTrunk->VocDevID,&pOneTrunk->deviceID );		

		FreeOneFreeVoice (  &pOneTrunk->VocDevID );

		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device 
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
		DrawMain_VocInfo ( pOneTrunk );
	}
}
// end of code for CAS(SS1)

// Special code for Analog
/********************************************************************************
Function:		设置Tone音包络(拨号音,回铃音,挂机忙音等)
Input:			DeviceID_t *pDevice				Board设备DeviceID
Output:			Void
Description:	
********************************************************************************/
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

void SetGtD_AnalogTrunk(DeviceID_t* pDevId)
{
	//========Set GTG Begin========
	CmdParamData_Voice_t cmdVoc;
	memset(&cmdVoc,0,sizeof(cmdVoc));
	cmdVoc.m_u8GtdCtrlValid = 1 ;						//Enable GTD
	cmdVoc.m_VocGtdControl.m_u8ChannelEnable = 1;		//Enable Gtd channel
	cmdVoc.m_VocGtdControl.m_u8DTMFEnable = 1;			// Detect DTMF
	cmdVoc.m_VocGtdControl.m_u8GTDEnable = 1;			// Detect GTD 
	cmdVoc.m_VocGtdControl.m_u8FSKEnable = 1;			// Detect FSK for receive CallerID

	cmdVoc.m_VocGtdControl.m_u8EXTEnable = 0x2;		// Enable PVD Detect

	strcpy((char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0],"GHIJK");	// Detect Busy Tone

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
/********************************************************************************
Function:		业务流程主体函数
Input:			TRUNK_STRUCT *pOneTrunk				中继通道结构;
				Acs_Evt_t *pAcsEvt					返回事件头指针
********************************************************************************/
void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	char					TmpDtmf, TmpGtd;
	char					FileName[256];
	char					tempStr[100];
	DeviceID_t				FreeVocDeviceID;
	Acs_CallControl_Data *	pCallControl = NULL;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*clear Event*/
	{
		if ( (pOneTrunk->State != TRK_FREE) && (pOneTrunk->State != TRK_WAIT_REMOVE) )
		{
			ResetTrunk ( pOneTrunk, pAcsEvt );
			return ; 
		}
	}

	if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
	{
		TmpGtd = My_GetGtdCode ( pAcsEvt );
		if ( TmpGtd  != -1 ) 
		{
			if ( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )	// Busy Tone
			{
				XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);
				XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);
				ResetTrunk(pOneTrunk,pAcsEvt);
			}
		}
	}

	switch(pOneTrunk->State)
	{
	case TRK_FREE:							//等待呼叫并应答
		SpecialForCas(pOneTrunk,pAcsEvt);	// Special code for CAS(SS1)
		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
		{
			unsigned char	 *p;
			p = My_GetFskCode ( pAcsEvt );
			if ( p != NULL )
			{
				ConvertRawFskToCallerID ( p, pOneTrunk->CallerCode, 20 );
			}	
		}

		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	// Call In Event
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_ISUP )
			{
				// 获取原始被叫号码
				XMS_ctsGetParam(g_acsHandle, &pOneTrunk->deviceID, ISUP_SP_OriginalCalledNumber,0,NULL);
				// 获取改发的号码   
				XMS_ctsGetParam(g_acsHandle, &pOneTrunk->deviceID, ISUP_SP_RedirectingNumber,0,NULL);
				
				/************************************************************************/
				/* 显示：主叫地址性质指示码  被叫地址性质指示码
				   0：备用	1：用户号码	2：不知	3：国内（有效）号码	4：国际号码         */
				/************************************************************************/
				switch (pCallControl->m_u8CallingAddressIndicator)
				{
				case 0:strcpy(tempStr,"主叫地址性质指示码: 0-备用");break;
				case 1:strcpy(tempStr,"主叫地址性质指示码: 1-用户号码");break;
				case 2:strcpy(tempStr,"主叫地址性质指示码: 2-不知");break;
				case 3:strcpy(tempStr,"主叫地址性质指示码: 3-国内有效号码");break;
				case 4:strcpy(tempStr,"主叫地址性质指示码: 4-国际号码");break;
				default:strcpy(tempStr,"主叫地址性质指示码: Unknown");break;
				}
				AddMsg(tempStr);
				switch(pCallControl->m_u8CalledAddressIndicator)
				{
				case 0:strcpy(tempStr,"被叫地址性质指示码: 0-备用");break;
				case 1:strcpy(tempStr,"被叫地址性质指示码: 1-用户号码");break;
				case 2:strcpy(tempStr,"被叫地址性质指示码: 2-不知");break;
				case 3:strcpy(tempStr,"被叫地址性质指示码: 3-国内有效号码");break;
				case 4:strcpy(tempStr,"被叫地址性质指示码: 4-国际号码");break;
				default:strcpy(tempStr,"被叫地址性质指示码: Unknown");break;
				}				
				AddMsg(tempStr);
			}else if (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_TUP)
			{
				// 获取原始被叫号码
				XMS_ctsGetParam(g_acsHandle, &pOneTrunk->deviceID, TUP_SP_OriginalCalledAddress,0,NULL);
				/************************************************************************/
				/* TUP显示：主叫地址性质指示码  被叫地址性质指示码
				   0：市内用户号码	1：备用	2：国内有效号码	3：国际号码				    */
				/************************************************************************/
				switch (pCallControl->m_u8CallingAddressIndicator)
				{
				case 0:strcpy(tempStr,"主叫地址性质指示码: 0-市内用户号码");break;
				case 1:strcpy(tempStr,"主叫地址性质指示码: 1-备用");break;
				case 2:strcpy(tempStr,"主叫地址性质指示码: 2-国内有效号码");break;
				case 3:strcpy(tempStr,"主叫地址性质指示码: 3-国际号码");break;
				default:strcpy(tempStr,"主叫地址性质指示码: Unknown");break;
				}
				AddMsg(tempStr);
				switch(pCallControl->m_u8CalledAddressIndicator)
				{
				case 0:strcpy(tempStr,"被叫地址性质指示码: 0-市内用户号码");break;
				case 1:strcpy(tempStr,"被叫地址性质指示码: 1-备用");break;
				case 2:strcpy(tempStr,"被叫地址性质指示码: 2-国内有效号码");break;
				case 3:strcpy(tempStr,"被叫地址性质指示码: 3-国际号码");break;
				default:strcpy(tempStr,"被叫地址性质指示码: Unknown");break;
				}				
				AddMsg(tempStr);
			}
			
			if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )	 
			{
				My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
				FreeOneFreeVoice (  &pOneTrunk->VocDevID );
				memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
				memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
				DrawMain_VocInfo ( pOneTrunk );
			}
		
			XMS_ctsAlertCall ( g_acsHandle, &pOneTrunk->deviceID, NULL );
			XMS_ctsAnswerCallIn ( g_acsHandle, &pOneTrunk->deviceID, NULL );
			if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )//如果是数字中继(TUP、ISUP、ISDN)直接获取主被叫号码
			{
				sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
				sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
			}
			DrawMain_CallInfo( pOneTrunk );
			Change_State ( pOneTrunk, TRK_WAIT_ANSWERCALL );
		}
		break;

	case TRK_WAIT_ANSWERCALL:			//应答完成，并连接空闲语音
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);

		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	/*Answer Call In End Event*/
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
			{
				pOneTrunk->u8PlayTag = 16;

				pOneTrunk->VocDevID = FreeVocDeviceID;

				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 

				DrawMain_VocInfo ( pOneTrunk );

				My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID ); 

				if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
				{
					SetGtD_AnalogTrunk(&FreeVocDeviceID);		// prepare for get Busy Tone
				}

				Change_State ( pOneTrunk, TRK_WAIT_LINKOK );
			}
		}
		break;

	case TRK_WAIT_LINKOK:			//连接完成，播放语音"Bank.001"
		if ( pAcsEvt->m_s32EventType == XMS_EVT_LINKDEVICE )	/*LinkDevice End*/
		{
			strcpy ( FileName, cfg_VocPath );
			strcat ( FileName, "\\bank.001");
			pOneTrunk->u8PlayTag ++;
			PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag );

			Change_State ( pOneTrunk, TRK_WELCOME );
		}
		break;

	case TRK_WELCOME:				//检测放音完成事件,播放语音"Bank.002"
		if ( CheckPlayEnd ( pOneTrunk, pAcsEvt) )	/*play end Event*/
		{
			strcpy ( FileName, cfg_VocPath );
			strcat ( FileName, "\\bank.002");
			pOneTrunk->u8PlayTag ++;
			PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag );

			Change_State ( pOneTrunk, TRK_ACCOUNT );
		}

		TmpDtmf = My_GetDtmfCode ( pAcsEvt );//如果检测到按键，停止播放语音
		if ( TmpDtmf != -1 )								/*DTMF key*/
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawMain_DTMF ( pOneTrunk );

			StopPlayFile ( &pOneTrunk->VocDevID );
				
			Change_State ( pOneTrunk, TRK_ACCOUNT );
		}
		break;

	case TRK_ACCOUNT:
		// if play end
			// do nothing

		TmpDtmf = My_GetDtmfCode ( pAcsEvt );		//获取用户按键直至8位
		if ( TmpDtmf != -1 )								/*DTMF Key*/
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawMain_DTMF ( pOneTrunk );

			StopPlayFile ( &pOneTrunk->VocDevID );
			
			if ( pOneTrunk->DtmfCount >= 8 )
			{
				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.003");
				pOneTrunk->u8PlayTag ++;
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag );//播放语音文件"Bank.003"

				My_InitDtmfBuf ( pOneTrunk );
				Change_State ( pOneTrunk, TRK_PASSWORD );
			}
		}
		break;

	case TRK_PASSWORD:
		// if play end
			// do nothing

		TmpDtmf = My_GetDtmfCode ( pAcsEvt );				//提示输入密码，获取用户按键直至6位
		if ( TmpDtmf != -1 )								/*DTMF key*/
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawMain_DTMF ( pOneTrunk );

			StopPlayFile ( &pOneTrunk->VocDevID );

			if ( pOneTrunk->DtmfCount >= 6 )
			{
				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.004");
				pOneTrunk->u8PlayTag ++;
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag );//播放语音文件"Bank.004"

				My_InitDtmfBuf ( pOneTrunk );
				Change_State ( pOneTrunk, TRK_SELECT );
			}
		}
		break;

	case TRK_SELECT:
		// if play end
			// do nothing

		TmpDtmf = My_GetDtmfCode ( pAcsEvt );
		if ( TmpDtmf == -1 )								/*DTMF Key*/
			break;

		My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
		DrawMain_DTMF ( pOneTrunk );

		pOneTrunk->u8PlayTag ++;
		strcpy ( FileName, cfg_VocPath );
		switch(TmpDtmf)
		{
		case '1':	 
			{
				StopPlayFile ( &pOneTrunk->VocDevID );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.005");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				PlayIndex ( &pOneTrunk->VocDevID, 5, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 12, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 8, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 11, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 9, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 10, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 15, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 8, pOneTrunk->u8PlayTag, true );

				PlayIndex ( &pOneTrunk->VocDevID, 16, pOneTrunk->u8PlayTag, true );		// 索引队列放音

				Change_State ( pOneTrunk, TRK_PLAYRESULT );
			}
			break;

		case '2':
				StopPlayFile ( &pOneTrunk->VocDevID );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.006");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );
				
				PlayIndex ( &pOneTrunk->VocDevID, 0, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 15, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 4, pOneTrunk->u8PlayTag, true );
				PlayIndex ( &pOneTrunk->VocDevID, 8, pOneTrunk->u8PlayTag, true );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.008");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				Change_State ( pOneTrunk, TRK_PLAYRESULT );
			break;

		case '3':
				StopPlayFile ( &pOneTrunk->VocDevID );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.007");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );
				
				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\D3");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\D13");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\D7");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\D12");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "\\bank.008");
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true );

				Change_State ( pOneTrunk, TRK_PLAYRESULT );
			break;

		case '4':
			StopPlayFile ( &pOneTrunk->VocDevID );

			sprintf ( FileName, "%s\\DemoRec.%03d", cfg_VocPath, pOneTrunk->iSeqID );
			RecordFile ( &pOneTrunk->VocDevID, FileName, 8000L*10, false );		//开始录音

			Change_State ( pOneTrunk, TRK_RECORDFILE );
			break;

		default:
			break;
		}
		break;

	case TRK_PLAYRESULT:
	case TRK_PLAYRECORD:
		if ( CheckPlayEnd ( pOneTrunk, pAcsEvt) )	/* Play End Event 检查放音完成并挂机*/
		{
			// Clear Call
			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );

			Change_State ( pOneTrunk, TRK_HANGON );
		}
		break;

	case TRK_RECORDFILE:
		//
		TmpDtmf = My_GetDtmfCode ( pAcsEvt );
		if ( TmpDtmf != -1 )								/*DTMF Key  获取到用户按键 则停止录音  */
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawMain_DTMF ( pOneTrunk );

			StopRecordFile ( &pOneTrunk->VocDevID );
		}

		if ( CheckRecordEnd ( pOneTrunk, pAcsEvt) )	/*record end*/
		{
			pOneTrunk->u8PlayTag ++;

			sprintf ( FileName, "%s\\DemoRec.%03d", cfg_VocPath, pOneTrunk->iSeqID );
			PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag );

			Change_State ( pOneTrunk, TRK_PLAYRECORD );
		}
		break;
	}
}

