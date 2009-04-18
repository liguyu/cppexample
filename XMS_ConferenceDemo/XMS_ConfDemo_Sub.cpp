
#include "stdafx.h"
#include "XMS_ConfDemo.h"
#include "XMS_ConfDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_ConfDemo_Sub.H"
#include "XMS_ConfDemo_Event.H"
#include "XMS_ConfDemo_String.H"

// Special code for CAS(SS1)
#include "..\CAS_Common_Code\CAS_Common_Cfg.h"	
#include "..\Analog_Common_Code\Analog_Common_Code.h"		
	
CmdParamData_CAS_t g_Param_CAS;
// end of code for CAS(SS1)

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_ConfDemoApp theApp;
CXMS_ConfDemoDlg	*pdlg;

// var about XMS_ConfDemo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_ConfDemo.INI";

ServerID_t		cfg_ServerID;
char			cfg_VocPath[128];
int				cfg_iDispChnl = 32;		// always disp 32 channel

int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;

int				cfg_s32DebugOn;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 4;

// var in XMS_ConfDemo_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_CONFDEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

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

extern int						g_iTotalConf;
extern int						g_iTotalConfOpened;
extern int						g_iTotalConfFree;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Conf[MAX_TRUNK_NUM_IN_THIS_DEMO];

// -----------------------------------------------------------------------
// local function decalration
// -----------------------------------------------------------------------
// next functions are about Config
// -----------------------------------------------------------------------
void	WriteToConfig(void);
void	ReadFromConfig(void);
void	InitTextBox(void);
void	FetchFromText();

// -----------------------------------------------------------------------
void	InitListMain(void);
void	InitListPcm(void);
void	InitListMsg(void);
void	InitListConf(void);
void	InitListCount(void);

void	OccupyOneConf ( CONF_STRUCT *pOneConf );
int		FreeOneConf (  DeviceID_t *pFreeConfDeviceID );
void	Do_LeaveConf ( TRUNK_STRUCT *pOneTrunk );
// -----------------------------------------------------------------------
// next functions are about Config
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

// -----------------------------------------------------------------------
// next functions are about Display
// -----------------------------------------------------------------------
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

// -------------------------------------------------------------------------------------------------
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
	lvc.cx = 40;
	pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "VocInfo" ;
	lvc.cx = 55;
	pdlg->m_ListMain.InsertColumn ( 8, &lvc ); 

	lvc.iSubItem = 9;
	lvc.pszText = "ConfInfo" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 9, &lvc ); 
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

	case TRK_SELECT:
		strcpy(StateStr,"Select");
		break;	

	case TRK_CONF_CONFING:
		strcpy(StateStr,"Confing");
		break;

	case TRK_WELCOME:
		strcpy(StateStr,"WELCOME");
		break;

	case TRK_CONF_ERROR:
		strcpy(StateStr,"ConfErr");
		break;

	case TRK_WAIT_ANSWERCALL:
		strcpy(StateStr,"Wait Answer");
		break;

	case TRK_WAIT_LINKOK:
		strcpy(StateStr,"Wait LinkOK");
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

void DrawMain_ConfInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	if ( pOneTrunk->ConfDevID.m_s16DeviceMain != 0 )
	{
		pDev = &pOneTrunk->ConfDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}
	else
		strcpy(StateStr, "");

	pdlg->m_ListMain.SetItemText ( iDispRow, 9, StateStr ); 

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
void	InitListConf(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListConf.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "ConfGroup" ;
	lvc.cx = 70;
	pdlg->m_ListConf.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 60;
	pdlg->m_ListConf.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListConf.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "Member" ;
	lvc.cx = 50;
	pdlg->m_ListConf.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "Detail Infomation" ;
	lvc.cx = 250;
	pdlg->m_ListConf.InsertColumn ( 4, &lvc ); 
}

void DrawConf_State( CONF_STRUCT *pOneConf )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = pOneConf->iSeqID; 

	//
	switch( pOneConf->State ) 
	{
	case CONF_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case CONF_FREE:		
		strcpy(StateStr,"Free"); 
		break ;

	case CONF_USED:
		strcpy(StateStr,"*Use*"); 
		break;

	case CONF_WAIT_REMOVE:
		strcpy(StateStr,"Remove DSP");
		break;

	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListConf.SetItemText ( iDispRow, 2, StateStr ); 
}

char	* GetString_ConfModeStr ( int iConfMode )
{
	switch ( iConfMode )
	{
	case CONF_MODE_NONE:
		return " ";
		break;

	case CONF_MODE_ADD:
		return "A";
		break;

	case CONF_MODE_LISTEN:
		return "L";
		break;

	case CONF_MODE_SPEAKONLY:
		return "S";
		break;

	case CONF_MODE_PLAY:
		return "P";
		break;
	}

	return "";
}

void DrawConf_DetailInfo ( CONF_STRUCT *pOneConf )
{
	char StateStr[256];
	char TmpS[100];
	int	 iDispRow, i;

	iDispRow = pOneConf->iSeqID; 

	sprintf ( StateStr, "%d", pOneConf->memberNum);
	pdlg->m_ListConf.SetItemText ( iDispRow, 3, StateStr ); 

	strcpy ( StateStr, "" );
	strcpy ( TmpS, "" );
	for ( i = 0; i < pOneConf->memberNum; i ++ )
	{
		sprintf ( TmpS, "(%d,%d) ", 
			pOneConf->Member[i].m_s8ModuleID, pOneConf->Member[i].m_s16ChannelID );
		strcat ( StateStr, TmpS );
	}
	pdlg->m_ListConf.SetItemText ( iDispRow, 4, StateStr ); 
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
	lvc.pszText = "Conf/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "ConfFree" ;
	lvc.cx = 60;
	pdlg->m_ListCount.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "ErrFlag" ;
	lvc.cx = 60;
	pdlg->m_ListCount.InsertColumn ( 8, &lvc ); 

	lvc.iSubItem = 9;
	lvc.pszText = "RemoveState" ;
	lvc.cx = 80;
	pdlg->m_ListCount.InsertColumn ( 9, &lvc ); 
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

void	DrawCount_Conf ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lConfNum, AllDeviceRes[s8ModID].lConfOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 6, TmpStr );

	sprintf ( TmpStr, "%3d",AllDeviceRes[s8ModID].lConfFreeNum );
	pdlg->m_ListCount.SetItemText ( iDispRow, 7, TmpStr );

	// total
	iDispRow = 0;

	sprintf ( TmpStr, "%3d/%3d", g_iTotalConf, g_iTotalConfOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 6, TmpStr );

	sprintf ( TmpStr, "%3d", g_iTotalConfFree );
	pdlg->m_ListCount.SetItemText ( iDispRow, 7, TmpStr );
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

	pdlg->m_ListCount.SetItemText ( iDispRow, 8, TmpStr );
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

	pdlg->m_ListCount.SetItemText ( iDispRow, 9, TmpStr );
}

// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;
	CONF_STRUCT		*pOneConf;

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

	// m_ListConf
	pdlg->m_ListConf.DeleteAllItems ();
	for ( i = 0; i < g_iTotalConf; i ++ )
	{
		// Fix content
		sprintf ( TmpStr, "%03d", i );
		pdlg->m_ListConf.InsertItem ( i, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_Conf[i].m_s8ModuleID, MapTable_Conf[i].m_s16ChannelID );
		pdlg->m_ListConf.SetItemText ( i, 1, TmpStr );

		pOneConf = &M_OneConf(MapTable_Conf[i]);
		// Changed content
		DrawConf_State ( pOneConf );
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
		DrawCount_Conf ( MapTable_Module[i] );
		DrawCount_ErrFlag ( MapTable_Module[i] );
		DrawCount_RemoveState ( MapTable_Module[i] );
	}
}

// -----------------------------------------------------------------------
// next functions are about Init & Exit
// -----------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_ConfDemoDlg	*)theApp.m_pMainWnd;

	// Read From "XMS_ConfDemo.INI"
	ReadFromConfig();

	// Special code for CAS(SS1)
	// Read From "C:\\DJKeygoe\\Samples\\CAS_Common_Code\\SaXMS_CAS_Cfg.INI"
	if ( CAS_Common_Cfg_ReadCfg ( &g_Param_CAS ) != 0 )
	{
		sprintf ( MsgStr, "CAS_Common_Cfg_ReadCfg \"C:\\DJKeygoe\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI\" Error!" );
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
	}
	// end of code for CAS(SS1)

	// Init m_ListMain
	InitListMain();

	// Init m_ListPcm
	InitListPcm();

	// Init m_ListMsg
	InitListMsg();

	// Init m_ListConf
	InitListConf();

	// Init m_ListCount
	InitListCount();

	// Init Conifg's text box
	InitTextBox();

	// now open ACS Server
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

	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
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

	XMS_acsGetDeviceList ( g_acsHandle, NULL );

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

	r = XMS_acsCloseStream ( g_acsHandle, NULL );


	FreeAllDeviceRes ();

	// save to "XMS_ConDemo.INI"
	FetchFromText();
	WriteToConfig();

}

// -----------------------------------------------------------------------
// next functions are about Open & Close
// -----------------------------------------------------------------------
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

void	OpenConfDevice ( CONF_STRUCT *pOneConf )
{
	RetCode_t	r;
	
	if ( pOneConf->State == CONF_WAITOPEN ) 		// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &pOneConf->deviceID, NULL );
		if ( r < 0 )
		{
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenConfDevice()!" );
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

	// pVoice must first,because we want to PrepareForCallerID
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}

	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}

	// pPcm
	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		OpenPcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}

	// pConf
	for ( i = 0; i < AllDeviceRes[s8DspModID].lConfNum; i++ )
	{
		OpenConfDevice ( &AllDeviceRes[s8DspModID].pConf[i] );
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

// --------------------------------------------------------------------------------
void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = true;
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

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_CONFERENCE )
	{
		M_OneConf(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important

		// init the Device: Conf
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		XMS_ctsClearConf ( g_acsHandle, pDevice, NULL );
		
		InitConfChannel ( &M_OneConf(*pDevice) );

		// modify the Count

		g_iTotalConfOpened ++;
		g_iTotalConfFree ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lConfOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lConfFreeNum ++;

		DrawCount_Conf ( pDevice->m_s8ModuleID );
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

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_CONFERENCE )
	{
		M_OneConf(*pDevice).State = CONF_WAITOPEN;

		// modify the Count
		g_iTotalConfOpened --;
		g_iTotalConfFree --;
		AllDeviceRes[pDevice->m_s8ModuleID].lConfOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lConfFreeNum --;

		DrawCount_Conf ( pDevice->m_s8ModuleID );
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

void	CloseConfDevice ( CONF_STRUCT *pOneConf )
{
	RetCode_t	r;
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneConf->deviceID, NULL );
	if ( r < 0 )
	{
		AddMsg ( "XMS_ctsCloseDevice Fail in CloseConfDevice()!" );
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

	// pConf
	for ( i = 0; i < AllDeviceRes[s8DspModID].lConfNum; i++ )
	{
		CloseConfDevice ( &AllDeviceRes[s8DspModID].pConf[i] );
	}
}

// -----------------------------------------------------------------------
// next functions are about Real Work
// -----------------------------------------------------------------------
void	HandleDevState ( Acs_Evt_t *pAcsEvt )
{
	Acs_GeneralProc_Data *pGeneralData = NULL;

	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		M_OneTrunk(pAcsEvt->m_DeviceID).iLineState = pGeneralData->m_s32DeviceState;
		DrawMain_LineState( &M_OneTrunk(pAcsEvt->m_DeviceID) );
	}

	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(pAcsEvt->m_DeviceID).u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		M_OnePcm(pAcsEvt->m_DeviceID).s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);

		DrawPcm_TypeAndAlarm ( &M_OnePcm(pAcsEvt->m_DeviceID) );
	}

	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_CONFERENCE )
	{
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

	// check device : CONF
	for ( i = 0; i < AllDeviceRes[s8DspModID].lConfNum; i ++ )
	{
		if ( (AllDeviceRes[s8DspModID].pConf[i].State != CONF_WAITOPEN)
			&& (AllDeviceRes[s8DspModID].pConf[i].State != CONF_WAIT_REMOVE) )
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

	// handle device : CONF
	for ( i = 0; i < AllDeviceRes[s8DspModID].lConfNum; i ++ )
	{
		if ( AllDeviceRes[s8DspModID].pConf[i].State == CONF_FREE )
		{
			Change_Conf_State ( &AllDeviceRes[s8DspModID].pConf[i], CONF_WAIT_REMOVE);
		}
	}

	// check if all device is WAIT_OPEN or WAIT_REMOVE
	CheckRemoveReady ( s8DspModID );
}

// --------------------------------------------------------------------------------
/* Stop Play */
DJ_S32 StopPlayFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;

	controlPlay.m_u16ControlType = XMS_STOP_PLAY;

	r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );

	return r;
}

/* Handle Play */
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

/* Handle Record */
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

DJ_S32	 RecordFile_Conf ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend, DeviceID_t *pConfDevID )
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

	// record conf
	recordProperty.m_s8IsMixEnable = 1;

	recordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_CONF;
	recordProperty.m_MixerControl.m_u16SRC_ChID1 = pConfDevID->m_s16ChannelID;
	recordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_NULL;
	//recordProperty.m_MixerControl.m_u16SRC_ChID2	// don't care when XMS_MIXER_FROM_NULL
	//

	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &recordProperty, NULL );

	return r;
}

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
void My_AddCallerBuf ( TRUNK_STRUCT *pOneTrunk, char TmpDtmf )
{
	if ( pOneTrunk->DtmfCount < (20-1) )
	{
		pOneTrunk->CallerCode[pOneTrunk->DtmfCount] = TmpDtmf;
		pOneTrunk->CallerCode[pOneTrunk->DtmfCount+1] = 0;
		pOneTrunk->DtmfCount ++;
	}
}
char My_GetDtmfCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO DataÊÂ¼þ*/
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

	//cmdVoc.m_VocGtdControl.m_u8EXTEnable = 0x2;		// Enable PVD Detect

	strcpy((char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0],"GHIJK");	// Detect Busy Tone

	DJ_U16 u16ParamType = VOC_PARAM_UNIPARAM;
	DJ_U16 u16ParamSize = sizeof(cmdVoc);
	void* p = (void*) &cmdVoc;

	int r = XMS_ctsSetParam( g_acsHandle,pDevId,u16ParamType,u16ParamSize,(void*)p);

	//========Set GTG End  ========
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

RetCode_t  My_JoinToConf ( DeviceID_t *pConfDevID, DeviceID_t *pVocDevID)
{
	RetCode_t				r = -1;
	CmdParamData_Conf_t		confParam = {0};
	CONF_STRUCT				*pOneConf;

	confParam.m_u8InputOpt = XMS_CONF_INPUT_OPT_NORMAL;
	confParam.m_u8OutputOpt = XMS_CONF_OUTPUT_OPT_NORMAL;	

	r = XMS_ctsJoinToConf ( g_acsHandle, pConfDevID, pVocDevID, &confParam, NULL);

	if ( r > 0 )
	{
		pOneConf = &M_OneConf (*pConfDevID);

		pOneConf->Member[pOneConf->memberNum] = *pVocDevID;
		pOneConf->memberNum ++;
	
		DrawConf_DetailInfo ( pOneConf );

		if ( pOneConf->memberNum == 1 )
		{
			OccupyOneConf ( pOneConf );
		}
	}else
	{
		AddMsg ( "XMS_ctsJoinToConf() Fail in My_JoinToConf()!" );
	}

	return	r;
}

bool	IsDeviceEqual ( DeviceID_t *pDevID1, DeviceID_t *pDevID2 )
{
	if ( ( pDevID1->m_s16DeviceMain == pDevID2->m_s16DeviceMain )
		&& ( pDevID1->m_s8ModuleID == pDevID2->m_s8ModuleID )
		&& ( pDevID1->m_s16ChannelID == pDevID2->m_s16ChannelID ) )
	{
		return true;
	}

	return false;
}


RetCode_t  My_LeaveFromConf ( DeviceID_t *pConfDevID, DeviceID_t *pVocDevID)
{
	int						i, j;
	RetCode_t				r = -1;
	CmdParamData_Conf_t		confParam = {0};
	CONF_STRUCT				*pOneConf;
	
	confParam.m_u8InputOpt = XMS_CONF_INPUT_OPT_NORMAL;
	r = XMS_ctsLeaveFromConf ( g_acsHandle, pConfDevID, pVocDevID, &confParam, NULL);
	if ( r > 0 )
	{
		pOneConf = &M_OneConf (*pConfDevID);
		for ( i = 0; i < pOneConf->memberNum; i ++ )
		{
			if ( ( pOneConf->Member[i].m_s16DeviceMain == pVocDevID->m_s16DeviceMain )
				&& ( pOneConf->Member[i].m_s8ModuleID == pVocDevID->m_s8ModuleID )
				&& ( pOneConf->Member[i].m_s16ChannelID == pVocDevID->m_s16ChannelID ) )
			{
				break;
			}else{
				AddMsg("No the conference member");
				return -1;
			}
				
		}
		if ( i < pOneConf->memberNum)
		{
			if ( i < (pOneConf->memberNum-1) )
			{
				for ( j = i; j < pOneConf->memberNum-1; j ++ )		// move
					pOneConf->Member[j] = pOneConf->Member[j+1];  
			}
			
			if ( pOneConf->memberNum > 0 )
				pOneConf->memberNum --;
			
			DrawConf_DetailInfo ( pOneConf );

			if ( pOneConf->memberNum == 0 )
				FreeOneConf ( &pOneConf->deviceID );
		}
	}
	else
	{
		AddMsg ( "XMS_ctsLeaveFromConf() Fail in My_LeaveFromConf()!" );
	}

	return	r;
}

int		SearchOneFreeVoice (  DeviceID_t *pFreeVocDeviceID, SEARCH_RULE SearchRule, DJ_S8 s8RefModID )
{
	int				i;
	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;
	long			lNowMaxFreeNum;

	if ( SearchRule == SEARCH_RULE_SAME_MODULE )			// Search in Same Module
	{
		s8SearchModID = s8RefModID;
	}
	else		// Search in Max free resource module
	{
		s8SearchModID = -1;
		lNowMaxFreeNum = -1;
		for ( i = 0; i < g_iTotalModule; i ++ )
		{
			if ( AllDeviceRes[MapTable_Module[i]].lVocFreeNum > lNowMaxFreeNum )
			{
				s8SearchModID = MapTable_Module[i];
				lNowMaxFreeNum = AllDeviceRes[MapTable_Module[i]].lVocFreeNum;
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
			Change_Voc_State ( &AllDeviceRes[s8SearchModID].pVoice[i], VOC_USED);
			AllDeviceRes[s8SearchModID].lVocFreeNum--;
			g_iTotalVoiceFree --;
			DrawCount_Voc ( s8SearchModID );
			return i;
		}
	}

	return -1;
}

int		FreeOneVoice (  DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8	s8ModID;

	s8ModID = pFreeVocDeviceID->m_s8ModuleID;
	if ( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		Change_Voc_State ( &M_OneVoice(*pFreeVocDeviceID), VOC_FREE);
		memset ( &M_OneVoice(*pFreeVocDeviceID).UsedDevID,0, sizeof(DeviceID_t) );// 0:didn't alloc  Device 
		AllDeviceRes[s8ModID].lVocFreeNum++;
		g_iTotalVoiceFree ++;
		DrawCount_Voc ( s8ModID );
		return	0;		// OK
	}

	return -1;			// invalid VocDeviceID
}

int		SearchOneFreeConf ( DeviceID_t *pFreeConfDeviceID )
{
	int				i;
	static	int		iLoopStart = 0;
	CONF_STRUCT		*pOneConf;
	DJ_S8			s8SearchModID;
	long			lNowMaxFreeNum;

	// Search in Max free voice resource module
	{
		s8SearchModID = -1;
		lNowMaxFreeNum = -1;
		for ( i = 0; i < g_iTotalModule; i ++ )
		{
			if ( AllDeviceRes[MapTable_Module[i]].lVocFreeNum > lNowMaxFreeNum )
			{
				s8SearchModID = MapTable_Module[i];
				lNowMaxFreeNum = AllDeviceRes[MapTable_Module[i]].lVocFreeNum;
			}
		}
	}

	for ( i = 0; i < AllDeviceRes[s8SearchModID].lConfNum; i ++ )
	{
		pOneConf = &AllDeviceRes[s8SearchModID].pConf[i];
		if ( pOneConf->State == CONF_FREE )
		{
			*pFreeConfDeviceID = pOneConf->deviceID;

			/*
			// use this voice device 
			pOneConf->State = CONF_USED;
			DrawConf_State( pOneConf );

			AllDeviceRes[s8SearchModID].lConfFreeNum--;
			g_iTotalConfFree --;

			DrawCount_Conf ( s8SearchModID );
			*/
			return i;
		}
	}

	return -1;
}

int		FreeOneConf (  DeviceID_t *pFreeConfDeviceID )
{
	DJ_S8	s8ModID;

	s8ModID = pFreeConfDeviceID->m_s8ModuleID;
	if ( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		Change_Conf_State ( &M_OneConf(*pFreeConfDeviceID), CONF_FREE);

		AllDeviceRes[pFreeConfDeviceID->m_s8ModuleID].lConfFreeNum++;
		g_iTotalConfFree ++;

		DrawCount_Conf ( pFreeConfDeviceID->m_s8ModuleID );

		return	0;		// OK
	}

	return -1;			// invalid VocDeviceID
}

void	OccupyOneConf ( CONF_STRUCT *pOneConf )
{
	// maybe use this voice device 
	if ( pOneConf->State == CONF_FREE )
	{
		Change_Conf_State ( pOneConf, CONF_USED);

		AllDeviceRes[pOneConf->deviceID.m_s8ModuleID].lConfFreeNum--;
		g_iTotalConfFree --;

		DrawCount_Conf(pOneConf->deviceID.m_s8ModuleID);
	}
}

void	Change_Conf_State ( CONF_STRUCT *pOneConf, CONF_STATE NewState )
{
	pOneConf->State = NewState;

	DrawConf_State ( pOneConf );

	// Check if ready to remove DSP hardware
	if ( (AllDeviceRes[pOneConf->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == CONF_FREE) )
	{
		pOneConf->State = CONF_WAIT_REMOVE;
	
		DrawConf_State ( pOneConf );

		CheckRemoveReady ( pOneConf->deviceID.m_s8ModuleID );
	}
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
void	PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk )
{
	DeviceID_t FreeVocDeviceID;
	
	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )
		return;
	
	if (SearchOneFreeVoice ( &FreeVocDeviceID, SEARCH_RULE_SAME_MODULE, pOneTrunk->deviceID.m_s8ModuleID ) >= 0 )
	{
		pOneTrunk->VocDevID = FreeVocDeviceID;
		
		M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
		
		DrawMain_VocInfo ( pOneTrunk );
		
		My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );
		SetGtD_AnalogTrunk(&FreeVocDeviceID);
	}else{
		AddMsg("No free voc for PrepareForCallerID...");
	}
}
void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
	Change_State ( pOneTrunk, TRK_FREE );

	pOneTrunk->CallerCode[0]=0;
	pOneTrunk->CalleeCode[0]=0;

	My_InitDtmfBuf ( pOneTrunk );
	DrawMain_DTMF ( pOneTrunk );

	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
	DrawMain_VocInfo ( pOneTrunk );

	memset ( &pOneTrunk->ConfDevID, 0, sizeof(DeviceID_t) );		// 0:didn't alloc  Device
	DrawMain_ConfInfo ( pOneTrunk );
}

void ResetTrunk ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	if ( pOneTrunk->State == TRK_CONF_CONFING )
	{
		Do_LeaveConf ( pOneTrunk );

		FreeOneVoice ( &pOneTrunk->VocDevID );

		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc  Device
		DrawMain_VocInfo ( pOneTrunk );
	}

	StopPlayFile ( &pOneTrunk->VocDevID );

	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneTrunk->deviceID, &pOneTrunk->VocDevID );
		FreeOneVoice ( &pOneTrunk->VocDevID );
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc  Device
		DrawMain_VocInfo ( pOneTrunk );
	}
	InitTrunkChannel ( pOneTrunk );
}

void	InitConfChannel ( CONF_STRUCT *pOneConf )
{
	Change_Conf_State ( pOneConf, CONF_FREE);

	pOneConf->memberNum = 0;
	memset ( pOneConf->Member, 0,	sizeof(DeviceID_t)*MAX_MEMBER_PER_CONF_GROUP );

	DrawConf_DetailInfo( pOneConf );
}

void	Do_LeaveConf ( TRUNK_STRUCT *pOneTrunk )
{
	CONF_STRUCT				*pOneConf;

	pOneConf = &M_OneConf(pOneTrunk->ConfDevID);
	
	My_DualUnlink ( &pOneTrunk->deviceID, &pOneTrunk->VocDevID );  
	My_LeaveFromConf ( &pOneTrunk->ConfDevID, &pOneTrunk->VocDevID );
	
	if ( pOneConf->memberNum == 0 )
		XMS_ctsClearConf ( g_acsHandle, &pOneConf->deviceID, NULL );
}

// Special code for CAS(SS1)
void	SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t				FreeVocDeviceID;

	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_E1_CAS )
		return;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_START )	/* start receive MFC, need a Voice Device */
	{
		if ( SearchOneFreeVoice ( &FreeVocDeviceID, SEARCH_RULE_SAME_MODULE, pOneTrunk->deviceID.m_s8ModuleID ) >= 0 )
		{
			// add next line from V2.2.0, link by Application Program
			My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );

			pOneTrunk->VocDevID = FreeVocDeviceID;
			DrawMain_VocInfo ( pOneTrunk );

			g_Param_CAS.m_VocDevID = FreeVocDeviceID;
			XMS_ctsSetParam ( g_acsHandle, &pOneTrunk->deviceID, 
				CAS_PARAM_UNIPARAM, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );
		}else{
			AddMsg("No free voice for SpecialForCas...");
		}
	}

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_END )	/* receive MFC end, release Voice Device */
	{
		// add next line from V2.2.0, unlink by Application Program
		My_DualUnlink(&pOneTrunk->VocDevID,&pOneTrunk->deviceID );	
		FreeOneVoice (  &pOneTrunk->VocDevID );
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc  Device
		DrawMain_VocInfo ( pOneTrunk );
	}
}
// end of code for CAS(SS1)

void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	DeviceID_t				FreeVocDeviceID;
	DeviceID_t				FreeConfDeviceID;
	char					FileName[256];
	char					TmpDtmf, TmpGtd;
	int						iConfNo;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*Clear Call*/
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
	case TRK_FREE:
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
        {
            unsigned char     *p;
			
            p = My_GetFskCode ( pAcsEvt );//recv FSK  callerid
            if ( p != NULL )
            {
                ConvertRawFskToCallerID ( p, pOneTrunk->CallerCode, 20 );
            }    
			
            TmpDtmf = My_GetDtmfCode(pAcsEvt);//recv Dtmf  callerid
            if ( TmpDtmf != -1 )                        
            {
                My_AddCallerBuf( pOneTrunk, TmpDtmf);
            }
        }

		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	/*Call In*/
		{

			XMS_ctsAlertCall ( g_acsHandle, &pOneTrunk->deviceID, NULL );
			XMS_ctsAnswerCallIn ( g_acsHandle, &pOneTrunk->deviceID, NULL );

			if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )
			{
				pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
				sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
				sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
			}

			DrawMain_CallInfo( pOneTrunk );
			
			Change_State ( pOneTrunk, TRK_WAIT_ANSWERCALL );
		}
		break;

	case TRK_WAIT_ANSWERCALL:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	
		{
			iConfNo = 0; 
			if ( (iConfNo < 0) || (iConfNo >= g_iTotalConf) )
			{
				Change_State ( pOneTrunk, TRK_CONF_ERROR );
				break;
			}
			if ( (M_OneConf(MapTable_Conf[iConfNo]).State != CONF_FREE)
				&& (M_OneConf(MapTable_Conf[iConfNo]).State != CONF_USED) )
			{
				Change_State ( pOneTrunk, TRK_CONF_ERROR );
				break;
			}			
			FreeConfDeviceID = M_OneConf(MapTable_Conf[iConfNo]).deviceID;	
			pOneTrunk->ConfDevID = FreeConfDeviceID;
			My_JoinToConf ( &FreeConfDeviceID, &pOneTrunk->VocDevID);	
			Change_State ( pOneTrunk, TRK_CONF_CONFING );
		}
		break;

	case TRK_CONF_CONFING:
		break;

	case TRK_CONF_ERROR:
		break;

	}
}

