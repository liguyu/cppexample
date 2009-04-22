#include "stdafx.h"
#include "XMS_SS7Monitor.h"
#include "XMS_SS7MonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "DJAcsTUPDef.h"
#include "DJAcsISUPDef.h"

#include "XMS_SS7Monitor_Sub.H"
#include "XMS_SS7Monitor_Event.H"
#include "XMS_SS7Monitor_String.H"

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
extern CXMS_SS7MonitorApp theApp;
CXMS_SS7MonitorDlg	*pdlg;

// var about XMS_Demo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_SS7Monitor.INI";

ServerID_t		cfg_ServerID;
char			cfg_VocPath[128];
int				cfg_iDispChnl;
int				cfg_iVoiceRule;

int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;
int				cfg_AppID;
int				cfg_s32DebugOn;

// var about work
ACSHandle_t		g_acsHandle = -1;

// var in XMS_Demo_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalPcm;
extern int						g_iTotalPcmOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM_IN_THIS_DEMO];

extern int						g_iTotalSS7Link;
extern int						g_iTotalSS7LinkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_SS7Link[MAX_PCM_NUM_IN_THIS_DEMO];

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
// local function declare
void	ReadFromConfig(void);
void	InitTextBox(void);

void	InitListMain(void);
void	InitListPcm(void);
void	InitListSS7Link(void);
void	InitListMsg(void);

void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 );
void	My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 );
void	SetGtD_AnalogTrunk(DeviceID_t* pDevId);
// -----------------------------------------------------------------------
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );

	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.11.111", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);
	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9000, cfg_IniName);
	cfg_iDispChnl = GetPrivateProfileInt ( "ConfigInfo", "DispChnl", 32, cfg_IniName);
	cfg_iPartWork = GetPrivateProfileInt ( "ConfigInfo", "PartWork", 0, cfg_IniName);
	cfg_iPartWorkModuleID = GetPrivateProfileInt ( "ConfigInfo", "PartWorkModuleID", 0, cfg_IniName);
	cfg_AppID = GetPrivateProfileInt ( "ConfigInfo", "AppID", 13, cfg_IniName);
	cfg_s32DebugOn = GetPrivateProfileInt ( "ConfigInfo", "DebugOn", 0, cfg_IniName);

}

void	InitListMsg(void)
{
	int	i = pdlg->m_ListMsg.GetHorizontalExtent ( );

	pdlg->m_ListMsg.SetHorizontalExtent ( i + 1000 );
}
void	InitComboISUPMsgType(void)
{
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_BLO");// 阻断
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_CGB");// 电路群阻断
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_CGU");// 电路群阻断消除
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_CPG");// 呼叫进展
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_CQM");// 电路群问讯
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_GRS");// 电路群复原
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_INF");// 信息
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_INR");// 信息请求
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_RES");// 恢复	
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_RSC");// 电路复原
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_SAM");// 后续地址
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_SGM");// 分段
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_SUS");// 暂停
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_UBL");// 阻断消除
	pdlg->m_ComboISUPMsgType.AddString("ISUP_SM_USR");// 用户至用户信息
	pdlg->m_ComboISUPMsgType.SetCurSel(0);
}
void	InitComboTUPMsgType(void)
{
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_SAM"); // 后续地址信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_SAO"); // 带有一个地址的后续地址信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_GSM"); // 一般前向建立信息信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_GRQ"); // 一般请求信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_BLO"); // 闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_UBL"); // 解除闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_RSC"); // 电路复原信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_MGB"); // 面向维护的群闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_MGU"); // 面向维护的群解除闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_HGB"); // 面向硬件故障的群闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_HGU"); // 面向硬件故障的群解除闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_GRS"); // 电路群复原信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_SGB"); // 软件产生的群闭塞信号
	pdlg->m_ComboTUPMsgType.AddString("TUP_SM_SGU"); // 软件产生的群解除闭塞消信号
	pdlg->m_ComboTUPMsgType.SetCurSel(4);           
}
void AddMsg ( char *str)
{
	static	int		iTotal_ListMsg = 0;
	char		TmpStr[256];

	sprintf ( TmpStr, "%3d: ", iTotal_ListMsg+1 );
	strcat ( TmpStr, str );
	pdlg->m_ListMsg.AddString ( TmpStr );
	iTotal_ListMsg ++;

	pdlg->m_ListMsg.SetCurSel (iTotal_ListMsg-1);
	pdlg->GetDlgItem ( IDC_LIST_MSG )->SetWindowText ( TmpStr );		

}

// --------------------------------------------------------------------------------
void	InitListTrunk(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListTrunk.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListTrunk.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListTrunk.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListTrunk.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListTrunk.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListTrunk.InsertColumn ( 4, &lvc ); 

	lvc.iSubItem = 5;
	lvc.pszText = "Called" ;
	lvc.cx = 60;
	pdlg->m_ListTrunk.InsertColumn ( 5, &lvc ); 

	lvc.iSubItem = 6;
	lvc.pszText = "Calling" ;
	lvc.cx = 60;
	pdlg->m_ListTrunk.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "DTMF" ;
	lvc.cx = 60;
	pdlg->m_ListTrunk.InsertColumn ( 7, &lvc ); 

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

void DrawTrunk_LineState( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	// 
	GetString_LineState ( StateStr, pOneTrunk->iLineState );
	pdlg->m_ListTrunk.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawTrunk_State( TRUNK_STRUCT *pOneTrunk )
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

	case TRK_WAIT_REMOVE:
		strcpy(StateStr,"DSP remove");
		break;

	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListTrunk.SetItemText ( iDispRow, 4, StateStr ); 
}

void DrawTrunk_CallInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	strcpy ( StateStr, pOneTrunk->CalleeCode );
	pdlg->m_ListTrunk.SetItemText ( iDispRow, 5, StateStr ); 

	strcpy ( StateStr, pOneTrunk->CallerCode );
	pdlg->m_ListTrunk.SetItemText ( iDispRow, 6, StateStr ); 
}

// -------------------------------------------------------------------------------------------------
void	InitListPCM(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;

	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListPCM.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "PcmID" ;
	lvc.cx = 60;
	pdlg->m_ListPCM.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "Type" ;
	lvc.cx = 80;
	pdlg->m_ListPCM.InsertColumn ( 1, &lvc ); 

	lvc.iSubItem = 2;
	lvc.pszText = "Alarm Value" ;
	lvc.cx = 80;
	pdlg->m_ListPCM.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "Sig Detect" ;
	lvc.cx = 80;
	pdlg->m_ListPCM.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "Frame Sync" ;
	lvc.cx = 80;
	pdlg->m_ListPCM.InsertColumn ( 4, &lvc ); 

	lvc.iSubItem = 5;
	lvc.pszText = "Remote Alarm" ;
	lvc.cx = 85;
	pdlg->m_ListPCM.InsertColumn ( 5, &lvc ); 
}
// -------------------------------------------------------------------------------------------------
void	InitListSS7Link(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
		LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListSS7Link.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	
	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	lvc.iSubItem = 0;
	lvc.pszText = "SS7LinkID" ;
	lvc.cx = 80;
	pdlg->m_ListSS7Link.InsertColumn ( 0, &lvc ); 

	lvc.iSubItem = 1;
	lvc.pszText = "...." ;
	lvc.cx = 80;
	pdlg->m_ListSS7Link.InsertColumn ( 1, &lvc ); 
	
}

void DrawPCM_TypeAndAlarm ( PCM_STRUCT *pOnePcm )
{
	char StateStr[100];
	int	 iDispRow;
	int	 AlarmVal;

	iDispRow = pOnePcm->iSeqID; 

	AlarmVal = pOnePcm->s32AlarmVal;

	// E1 Type
	sprintf ( StateStr, "%s", GetString_PcmType (pOnePcm->u8E1Type) );
	pdlg->m_ListPCM.SetItemText ( iDispRow, 1, StateStr ); 

	// Alarm
	sprintf ( StateStr, "0x%X", AlarmVal );
	pdlg->m_ListPCM.SetItemText ( iDispRow, 2, StateStr ); 

	if ( AlarmVal & XMS_E1PORT_MASK_LOST_SIGNAL )
		pdlg->m_ListPCM.SetItemText ( iDispRow, 3, " X" ); 
	else
		pdlg->m_ListPCM.SetItemText ( iDispRow, 3, "O" ); 

	if ( AlarmVal & XMS_E1PORT_MASK_FAS_ALARM )
		pdlg->m_ListPCM.SetItemText ( iDispRow, 4, " X" ); 
	else
		pdlg->m_ListPCM.SetItemText ( iDispRow, 4, "O" ); 

	if ( AlarmVal & XMS_E1PORT_MASK_REMOTE_ALARM )
		pdlg->m_ListPCM.SetItemText ( iDispRow, 5, " X" ); 
	else
		pdlg->m_ListPCM.SetItemText ( iDispRow, 5, "O" ); 
	
}
void DrawSS7Link_Type ( SS7LINK_STRUCT *pOneSS7Link )
{
	char StateStr[100];
	int	 iDispRow;
	
	iDispRow = pOneSS7Link->iSeqID; 
	
	sprintf ( StateStr, "%d", pOneSS7Link->u8E1Type) ;
	pdlg->m_ListSS7Link.SetItemText ( iDispRow, 1, StateStr ); 
	
}
// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;

	// m_ListMain
	pdlg->m_ListTrunk.DeleteAllItems ();
	for ( i = 0; i < g_iTotalTrunk; i ++ )
	{
		iDispRow = CalDispRow ( i ); 
		if ( iDispRow < 0 )	continue;

		// Fix content
		sprintf ( TmpStr, "%3d", iDispRow );
		pdlg->m_ListTrunk.InsertItem ( iDispRow, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_Trunk[i].m_s8ModuleID, MapTable_Trunk[i].m_s16ChannelID );
		pdlg->m_ListTrunk.SetItemText ( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk(MapTable_Trunk[i]);
		pdlg->m_ListTrunk.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );

		// Changed content
		DrawTrunk_LineState ( pOneTrunk );
		DrawTrunk_State ( pOneTrunk );
		DrawTrunk_CallInfo ( pOneTrunk );
	}

	// m_ListPcm
	pdlg->m_ListPCM.DeleteAllItems ();
	for ( i = 0; i < g_iTotalPcm; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListPCM.InsertItem ( i, TmpStr );

		// Changed content
		DrawPCM_TypeAndAlarm ( &M_OnePcm(MapTable_Pcm[i]) );
	}
	
	// m_ListSS7Link
	pdlg->m_ListSS7Link.DeleteAllItems ();
	for ( i = 0; i < g_iTotalSS7Link; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListSS7Link.InsertItem ( i, TmpStr );
		// Changed content
		DrawSS7Link_Type ( &M_OneSS7Link(MapTable_SS7Link[i]) );
		
	}
}

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_SS7MonitorDlg	*)theApp.m_pMainWnd;

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
	InitListTrunk();

	// Init m_ListPcm
	InitListPCM();
	
	// Init m_ListSS7Link
	InitListSS7Link();

	// Init m_ListMsg
	InitListMsg();

	// Init m_ComboISUPMsgType
	InitComboISUPMsgType();

	// Init m_ComboTUPMsgType
	InitComboTUPMsgType();

	// now open ACS Server
	/* Call acsOpenStream() to connect with ACS Server */
	r = XMS_acsOpenStream ( &g_acsHandle, 
						&cfg_ServerID,
						cfg_AppID,		// App Unit ID 
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

}

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
void	OpenSS7LinkDevice ( SS7LINK_STRUCT *pOneSS7Link )
{
	RetCode_t	r;
	
	if ( pOneSS7Link->bOpenFlag == false )		// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &pOneSS7Link->deviceID, NULL );
		if ( r < 0 )
		{
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenSS7LinkDevice()!" );
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
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_NONE;

	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		OpenPcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}
	for ( i = 0; i < AllDeviceRes[s8DspModID].lSS7LinkNum; i++ )
	{
		OpenSS7LinkDevice ( &AllDeviceRes[s8DspModID].pSS7Link[i] );
	}
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
	}	

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		M_OnePcm(*pDevice).bOpenFlag = true;

		// init the Device: Pcm
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		DrawPCM_TypeAndAlarm ( &M_OnePcm(*pDevice) );

		// moidfy the count
		g_iTotalPcmOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened ++;
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_SS7_LINK)
	{
		M_OneSS7Link(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		M_OneSS7Link(*pDevice).bOpenFlag = true;
		
		// init the Device: Pcm
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		DrawSS7Link_Type( &M_OneSS7Link(*pDevice) );
		
		// moidfy the count
		g_iTotalSS7LinkOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lSS7LinkOpened ++;
	}
}

// --------------------------------------------------------------------------------
void	CloseDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;

	AllDeviceRes[pDevice->m_s8ModuleID].bErrFlag = true;

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

	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).bOpenFlag = false;

		// moidfy the count
		g_iTotalPcmOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened --;

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
		DrawTrunk_LineState( pOneTrunk );
	}

	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(pAcsEvt->m_DeviceID);
		pOnePcm->u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		pOnePcm->s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);
		DrawPCM_TypeAndAlarm ( pOnePcm );
	}

}
void SendSigMsgToTrunk(void){
	char			tmpStr[256];
	char			datatStr[256];
	int				ret;
	int				selTrunk;
	TRUNK_STRUCT	pOneTrunk;
	
	selTrunk = pdlg->m_ListTrunk.GetSelectionMark();
	if(selTrunk == -1){
		AddMsg("Please select Trunk first!!!");
		return;
	}
	
	pOneTrunk = AllDeviceRes[cfg_iPartWorkModuleID].pTrunk[selTrunk];
	if (pOneTrunk.deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_TUP)
	{
		int nIndex = pdlg->m_ComboTUPMsgType.GetCurSel();
		ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, TUP_SM_BLO);

	}else if (pOneTrunk.deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_ISUP)
	{
		switch (pdlg->m_ComboISUPMsgType.GetCurSel())
		{
		case 0:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_BLO);// 阻断
			break;
		case 1:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_CGB);// 阻断
			break;
		case 2:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_CGU);  //电路群阻断消除
			break;
		case 3:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_CPG);// 呼叫进展
			break;
		case 4:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_CQM);// 电路群问讯
			break;
		case 5:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_GRS);// 电路群复原
			break;
		case 6:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_INF);// 信息
			break;
		case 7:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_INR);// 信息请求
			break;
		case 8:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_RES);// 恢复	
			break;
		case 9:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_RSC);// 电路复原
			break;
		case 10:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_SAM);// 后续地址
			break;
		case 11:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_SGM);// 分段
			break;
		case 12:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_SUS);// 暂停
			break;
		case 13:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_UBL);// 阻断消除
			break;
		case 14:
			ret = XMS_ctsSendSignalMsg(g_acsHandle, &pOneTrunk.deviceID, ISUP_SM_USR);// 用户至用户信息
			break;
		
		}
		
		

	}else{
		AddMsg("The Trunk device type is wrong.");
	}
	
	
	//AddMsg(tmpStr);
}
void	SendRawDataToSS7Link(void){
	char			tmpStr[256];
	char			dataStr[256];
	int				selTrunk;
	TRUNK_STRUCT	pOneTrunk;
	DJ_U16			u16FrameSize;
	Acs_SS7RawFrame_Data			*pData=NULL;

	selTrunk = pdlg->m_ListTrunk.GetSelectionMark();
	if(selTrunk == -1){
		AddMsg("Please select SS7 Link first!!!");
		return;
	}
	
	pOneTrunk = AllDeviceRes[cfg_iPartWorkModuleID].pTrunk[selTrunk];

	if (pOneTrunk.deviceID.m_s16DeviceSub == XMS_DEVSUB_SS7_LINK)
	{
		pdlg->GetDlgItem ( IDC_EDIT_CMD )->GetWindowText (dataStr, 256);
		sprintf ( tmpStr, "Success...Send RawData to SS7 Link %d: %s", selTrunk ,dataStr);

// 		pData->m_u8UserType = XMS_SS7_USER_ISUP;
// 		pData->m_u8LinkType = XMS_SS7_LINK_CHINA;
// 		pData->m_u8InfoType = XMS_SS7_MSG_DATA;
// 		pData->m_u8InfoLen = 255;
// 		pData->m_u8Info[0] = 0;
// 		pData->m_u8Info[1] = 6;
// 		pData->m_u8Info[2] = 5;
// 		pData->m_u8Info[3] = 4;
// 		strcat(pData, dataStr);

		int ret = XMS_ctsSendRawFrame(g_acsHandle, &pOneTrunk.deviceID,256,(void *)dataStr); 
		if (ret >0)
		{
			AddMsg(tmpStr);
		}
	}else{
		AddMsg("The Trunk Sub Device Type is wrong!!!");
	}
	
}

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

	// all device in this DSP is ready for remove 
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_READY;
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


void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{
	pOneTrunk->State = NewState;

	DrawTrunk_State ( pOneTrunk );

	// Check if ready to remove DSP hardware
	if ( (AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == TRK_FREE) )
	{
		pOneTrunk->State = TRK_WAIT_REMOVE;

		DrawTrunk_State ( pOneTrunk );

		CheckRemoveReady ( pOneTrunk->deviceID.m_s8ModuleID );
	}
}

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
	Change_State ( pOneTrunk, TRK_FREE );

	pOneTrunk->CallerCode[0]=0;
	pOneTrunk->CalleeCode[0]=0;
}


void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char					TmpDtmf, TmpGtd;
	TUP_spCallingLineAddress *pCallerID;
	memset(&pCallerID,0,sizeof(TUP_spCallingLineAddress));

}

