/*
*业务功能处理（最主要的功能代码）
*/
#include "stdafx.h"
#include "XMS_SwitchDemo.h"
#include "XMS_SwitchDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_SwitchDemo_Sub.H"
#include "XMS_SwitchDemo_String.H"
#include "XMS_SwitchDemo_Event.H"

// Special code for CAS(SS1)
#include "..\CAS_Common_Code\CAS_Common_Cfg.h"		
CmdParamData_CAS_t g_Param_CAS;
// end of code for CAS(SS1)

// Special code for Analog
#include "..\Analog_Common_Code\Analog_Common_Code.h"		
TYPE_ANALOG_GTD_PARAM g_Param_Analog;

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_SwitchDemoApp theApp;
CXMS_SwitchDemoDlg	*pdlg;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 2;

// var about XMS_SwitchDemo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_SwitchDemo.INI";
int				cfg_iCalledLen = 4;//被叫号码长度
char			cfg_CallingNum[32] = "88886666";//主叫号码
int				cfg_iCallOutRule;//呼叫规则,2表示坐席间呼叫,3表示坐席呼外线
ServerID_t		cfg_ServerID;
char			cfg_VocPath[128];
int				cfg_iDispChnl;
int				cfg_iVoiceRule;
int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;

int				cfg_s32DebugOn;

extern CListCtrl *gm_List_DSPRes;
extern CListCtrl *gm_List_ChnRes;
extern CListCtrl *gm_List_AnalogUser;

// var in XMS_SwitchDemo_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalTrunk;
extern int						g_iTotalTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalVoice;
extern int						g_iTotalVoiceOpened;
extern int						g_iTotalVoiceFree;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_TRUNK_NUM_IN_THIS_DEMO];
extern int						g_iTotalUser;
extern int						g_iTotalUserOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_User[MAX_TRUNK_NUM_IN_THIS_DEMO];
extern int						g_iTotalModule;
extern DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

//根据E1端口分配的通道数的不同,计算当前iSeqID所在的行-----------------------------
int CalDispRow( int iSeqID )
{
	int	iRet;
	//cfg_iDispChnl每个E1端口分配的通道
	if ( cfg_iDispChnl == 30 ){
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
//更改 接口通道列表框的 State 标志位
void DrawMain_State( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	switch( pOneTrunk->State ) 
	{
	case TRK_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;
	case TRK_CONNECT:
		strcpy(StateStr,"Connect"); 
		break;
	case TRK_FREE:		
		strcpy(StateStr,"Free"); 
		break ;				
	case TRK_WELCOME:
		strcpy(StateStr,"Welcome to DonJin Keygo Bank Demo");
		break ;				
	case TRK_EXTNUM:
		strcpy(StateStr,"Input Extention Numbers") ;
		break ;
	case TRK_CALLOUT:
		strcpy(StateStr,"CALL OUT");
		break;
	case TRK_WAITCONNECT:
		strcpy(StateStr,"Wait Connect");
		break;
	case TRK_CONNECT_FAILURE:
		strcpy(StateStr,"Connect failure");
		break;
	case TRK_NOTHANDLE:
		strcpy(StateStr,"Not Handle");
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
	pdlg->m_List_ChnRes.SetItemText ( iDispRow, 4, StateStr ); 
}
//更改 接口通道列表框 called calling 标志位---------------------------------------
void DrawMain_CallInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	strcpy ( StateStr, pOneTrunk->CalledCode );
	pdlg->m_List_ChnRes.SetItemText ( iDispRow, 5, StateStr ); 

	strcpy ( StateStr, pOneTrunk->CallerCode );
	pdlg->m_List_ChnRes.SetItemText ( iDispRow, 6, StateStr ); 
}
//更改 接口通道列表框 的 DTMF 标志位
void DrawMain_DTMF( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	strcpy(StateStr,pOneTrunk->DtmfBuf);
	pdlg->m_List_ChnRes.SetItemText ( iDispRow, 7, StateStr ); 

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

	pdlg->m_List_ChnRes.SetItemText ( iDispRow, 8, StateStr ); 

}

//更改 DSP模块列表框的 Trk/Open --------------------------------------------------
void DrawCount_Trunk( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lTrunkNum, AllDeviceRes[s8ModID].lTrunkOpened );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 3, TmpStr );

	//更改统计行(第一行)的Trk/Open
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalTrunk, g_iTotalTrunkOpened );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 3, TmpStr );
}
//更改 DSP模块列表框的 Voc/Open 和 VocFree--------------------------------------------------
void DrawCount_Voc ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lVocNum, AllDeviceRes[s8ModID].lVocOpened );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 4, TmpStr );

	sprintf ( TmpStr, "%3d", AllDeviceRes[s8ModID].lVocFreeNum );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 5, TmpStr );

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalVoice, g_iTotalVoiceOpened );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 4, TmpStr );

	sprintf ( TmpStr, "%3d", g_iTotalVoiceFree );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 5, TmpStr );
}
//更改板卡设备框的 User/Open 标志位----------------------------------------------------
void DrawCount_User( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalUser, g_iTotalUserOpened );
	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 6, TmpStr );
}
//更改板卡设备框的 ErrFlag 标志位----------------------------------------------------
void DrawCount_ErrFlag ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	if ( AllDeviceRes[s8ModID].bErrFlag )
		sprintf ( TmpStr, "True" );
	else
		sprintf ( TmpStr, "False" );

	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 7, TmpStr );
}
//更改板卡列表框的 RemoveState标志位------------------------------------------
void DrawCount_RemoveState ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	switch ( AllDeviceRes[s8ModID].RemoveState )
	{
	case DSP_REMOVE_STATE_NONE:
		sprintf ( TmpStr, "Do not" );
		break;
	case DSP_REMOVE_STATE_START:
		sprintf ( TmpStr, "Start" );
		break;
	case DSP_REMOVE_STATE_READY:
		sprintf ( TmpStr, "Ready" );
		break;
	}

	pdlg->m_List_DSPRes.SetItemText ( iDispRow, 8, TmpStr );
}
//更改接口通道列表框的 LineState 标志位------------------------------------------------
void DrawMain_LineState( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	GetString_LineState( StateStr, pOneTrunk->iLineState );
	pdlg->m_List_ChnRes.SetItemText( iDispRow, 3, StateStr ); 
}

//更改 坐席资源列表框 的 DTMF-----------------------------------------------
void DrawUser_DTMF( TRUNK_STRUCT *pOneUser )
{
	TRUNK_STRUCT *pLinkTrunk = NULL;
	char StateStr[100];
	int	 iDispRow,iMainRow;	
	iDispRow = pOneUser->iUserSeqID;
	pLinkTrunk = &M_OneTrunk(pOneUser->deviceID);
	iMainRow = CalDispRow(pLinkTrunk->iSeqID); //通道接口列表框的行序
	if (iDispRow < 0 || iMainRow <0)
	{
		return;
	}
	strcpy(StateStr,pOneUser->DtmfBuf);
	pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 5, StateStr ); 
	pdlg->m_List_ChnRes.SetItemText ( iMainRow, 7, StateStr ); 
	
}
//更改 坐席资源列表框 的 LineState-----------------------------------------------
void DrawUser_LineState( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;	
	iDispRow = pOneUser->iUserSeqID; 
	GetString_LineState ( StateStr, pOneUser->iLineState );
	pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 3, StateStr ); 
}
//更改 坐席资源列表框 的 VocInfo-----------------------------------------------
void DrawUser_VocInfo( TRUNK_STRUCT *pOneUser )
{
	TRUNK_STRUCT * pLinkTrunk=NULL;
	char StateStr[100];
	int	 iDispRow,iMainRow;
	DeviceID_t	*pDev;	
	iDispRow = pOneUser->iUserSeqID;
	pLinkTrunk = &M_OneTrunk(pOneUser->deviceID);
	iMainRow = CalDispRow(pLinkTrunk->iSeqID); //通道接口列表框的行序
	
	if ( pOneUser->VocDevID.m_s16DeviceMain != -1 )	{
		pDev = &pOneUser->VocDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}else
		strcpy(StateStr, "...");	
	pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 6, StateStr ); 
	pdlg->m_List_ChnRes.SetItemText ( iMainRow, 8, StateStr ); 
	
}
//更改 坐席资源列表框 的 LinkDev-----------------------------------------------
void DrawUser_LinkDev( TRUNK_STRUCT *pOneUser )
{
	char StateStr[100];
	int	 iDispRow;	
	iDispRow = pOneUser->iUserSeqID;	
	if ( pOneUser->VocDevID.m_s16DeviceMain != -1 )
		sprintf ( StateStr, "%d,%d", pOneUser->LinkDevID.m_s8ModuleID, pOneUser->LinkDevID.m_s16ChannelID );
	else
		strcpy(StateStr, "..");	
	pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 7, StateStr ); 	
}
//更改 坐席资源列表框 的 State----------------------------------------------
void DrawUser_State( TRUNK_STRUCT *pOneUser )
{
	TRUNK_STRUCT * pLinkTrunk =NULL;
	char StateStr[100];
	int	 iUserRow,iMainRow;	
	iUserRow = pOneUser->iUserSeqID; //坐席列表框的行序
	pLinkTrunk = &M_OneTrunk(pOneUser->deviceID);
	iMainRow = CalDispRow(pLinkTrunk->iSeqID); //通道接口列表框的行序
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
	case USER_CONNECT:
		strcpy(StateStr,"Connect"); 
		break;		
	case USER_RING:
		strcpy(StateStr,"Ring");
		break;
	case USER_CONNECT_FAILURE:
		strcpy(StateStr,"Connect Failure");
		break;		
	default:
		strcpy(StateStr,"other");
		break;
	}	
	pdlg->m_List_AnalogUser.SetItemText ( iUserRow, 4, StateStr ); 
	pdlg->m_List_ChnRes.SetItemText ( iMainRow, 4, StateStr ); 
}
//更改 坐席资源列表框 的 FailReason----------------------------------------------
void DrawUser_FailReason( TRUNK_STRUCT *pOneUser,char * failReasonStr )
{
	int	 iDispRow;	
	iDispRow = pOneUser->iUserSeqID; 	
	pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 8, failReasonStr ); 
}
// 重画 DSP列表框 CHN列表框 
void ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;

	//重画CHN列表框
	pdlg->m_List_ChnRes.DeleteAllItems();
	for ( i = 0; i < g_iTotalTrunk; i ++ )
	{
		iDispRow = CalDispRow( i ); 
		if ( iDispRow < 0 )	continue;

		// Fix content
		sprintf ( TmpStr, "%3d", iDispRow );
		pdlg->m_List_ChnRes.InsertItem ( iDispRow, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_Trunk[i].m_s8ModuleID, MapTable_Trunk[i].m_s16ChannelID );
		pdlg->m_List_ChnRes.SetItemText ( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk(MapTable_Trunk[i]);//得到当前通道的 接口通道结构体指针
		pdlg->m_List_ChnRes.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );

		//Changed content
		DrawMain_LineState(pOneTrunk );
		DrawMain_State( pOneTrunk );
		DrawMain_CallInfo( pOneTrunk );
		DrawMain_DTMF( pOneTrunk );
	}



	//重画DSP列表框
	pdlg->m_List_DSPRes.DeleteAllItems ();
	pdlg->m_List_DSPRes.InsertItem ( 0, "Total");//插入统计行
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_List_DSPRes.InsertItem ( i+1, TmpStr );

		sprintf ( TmpStr, "%3d", MapTable_Module[i] );
		pdlg->m_List_DSPRes.SetItemText ( i+1, 1, TmpStr );
	}

	// Display DSP module Content
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		DrawCount_Trunk( MapTable_Module[i] );
		DrawCount_Voc( MapTable_Module[i] );
		DrawCount_ErrFlag( MapTable_Module[i] );
		DrawCount_RemoveState( MapTable_Module[i] );
	}
	//重画坐席资源列表框
	pdlg->m_List_AnalogUser.DeleteAllItems ();
	for ( i = 0; i < g_iTotalUser; i ++ )
	{
		iDispRow = i;
		
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_List_AnalogUser.InsertItem ( iDispRow, TmpStr );
		
		sprintf ( TmpStr, "%d,%d", MapTable_User[i].m_s8ModuleID, MapTable_User[i].m_s16ChannelID );
		pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 1, TmpStr );
		
		pOneTrunk = &M_OneTrunk(MapTable_User[i]);
		pdlg->m_List_AnalogUser.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );
		
		DrawUser_LineState( pOneTrunk );
		DrawUser_State( pOneTrunk );
	}
}



//初始化系统------------------------------------------------------------------
bool InitSystem(void){
	
	//AfxMessageBox("InitSystem().....");
	RetCode_t r;
	char MsgStr[160];
	pdlg = (CXMS_SwitchDemoDlg *)theApp.m_pMainWnd;
	
	ReadFromConfig();//Read From "XMS_SwitchDemo.INI"

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

	InitListDSPRes();//初始化以DSP 模块为单位的资源总体的显示列表框
	InitListChnRes();//接口通道资源的显示
	InitListAnalogUser();
	InitListMsg();
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
	if(r < 0){
		sprintf( MsgStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );		
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
		AddMsg ( MsgStr );
		return false;
	}else{
		sprintf( MsgStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
		AddMsg( MsgStr );
	}
	//设置回调函数
	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if( r < 0 ){
		sprintf( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		AddMsg( MsgStr );
		return false;
	}else{
		sprintf( MsgStr, "XMS_acsSetESR() OK!" );
		AddMsg( MsgStr );
	}
	
	InitAllDeviceRes();
	XMS_acsGetDeviceList( g_acsHandle, NULL );
	
	return true;
}
//读取初始化文件XMS_SwitchDemo.INI-----------------------------------------------------
void ReadFromConfig(void){
	//AfxMessageBox("ReadFromConfig()...");
	GetCurrentDirectory( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat( cfg_IniName, cfg_IniShortName );
	//AfxMessageBox(cfg_IniName);
	GetPrivateProfileString( "ConfigInfo", "IpAddr", "192.168.11.51", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);
	//AfxMessageBox(cfg_ServerID.m_s8ServerIp);
	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);
	GetPrivateProfileString("ConfigInfo","UserName","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);
	cfg_iDispChnl = GetPrivateProfileInt ( "ConfigInfo", "DispChnl", 30, cfg_IniName);
	cfg_iVoiceRule = GetPrivateProfileInt ( "ConfigInfo", "VoiceRule", 0, cfg_IniName);
	cfg_iPartWork = GetPrivateProfileInt ( "ConfigInfo", "PartWork", 0, cfg_IniName);
	cfg_iPartWorkModuleID = GetPrivateProfileInt ( "ConfigInfo", "PartWorkModuleID", 0, cfg_IniName);
	cfg_s32DebugOn = GetPrivateProfileInt ( "ConfigInfo", "DebugOn", 0, cfg_IniName);
	cfg_iCalledLen = GetPrivateProfileInt ( "ConfigInfo", "CalledLen", 4, cfg_IniName);
	if ( cfg_iCalledLen > 20 ) cfg_iCalledLen = 16;
	GetPrivateProfileString ( "ConfigInfo", "CallingNum", "83636988", cfg_CallingNum, sizeof(cfg_CallingNum), cfg_IniName);
	cfg_iCallOutRule = GetPrivateProfileInt ( "ConfigInfo", "CallOutRule", 2, cfg_IniName);

}
//初始化以DSP 模块为单位的资源总体的显示列表框--------------------------------------------------------------------------------
void InitListDSPRes(void)
{
	gm_List_DSPRes->InsertColumn(0,"ID",LVCFMT_CENTER,40,-1);
	gm_List_DSPRes->InsertColumn(1,"ModID",LVCFMT_CENTER,45,-1);
	gm_List_DSPRes->InsertColumn(2,"Pcm/Open",LVCFMT_CENTER,70,-1);
	gm_List_DSPRes->InsertColumn(3,"Trk/Open",LVCFMT_CENTER,70,-1);
	gm_List_DSPRes->InsertColumn(4,"Voc/Open",LVCFMT_CENTER,70,-1);
	gm_List_DSPRes->InsertColumn(5,"VocFree",LVCFMT_CENTER,55,-1);
	gm_List_DSPRes->InsertColumn(6,"User/Open",LVCFMT_CENTER,70,-1);
	gm_List_DSPRes->InsertColumn(7,"ErrFlag",LVCFMT_CENTER,60,-1);
	gm_List_DSPRes->InsertColumn(8,"RemoveState",LVCFMT_CENTER,80,-1);
	
}
//初始化接口通道资源的显示------------------------------------------------------
void InitListChnRes(void){
	gm_List_ChnRes->InsertColumn(0,"ID",LVCFMT_CENTER,30,-1);
	gm_List_ChnRes->InsertColumn(1,"Mod,Ch",LVCFMT_CENTER,50,-1);
	gm_List_ChnRes->InsertColumn(2,"Type",LVCFMT_CENTER,55,-1);
	gm_List_ChnRes->InsertColumn(3,"LineState",LVCFMT_CENTER,90,-1);
	gm_List_ChnRes->InsertColumn(4,"State",LVCFMT_CENTER,90,-1);
	gm_List_ChnRes->InsertColumn(5,"Called",LVCFMT_CENTER,60,-1);
	gm_List_ChnRes->InsertColumn(6,"Calling",LVCFMT_CENTER,60,-1);
	gm_List_ChnRes->InsertColumn(7,"DTMF",LVCFMT_CENTER,50,-1);
	gm_List_ChnRes->InsertColumn(8,"VocInfo",LVCFMT_CENTER,60,-1);
	
}

//坐席资源框-----------------------------------------------------------------------
void InitListAnalogUser(void){
	gm_List_AnalogUser->InsertColumn(0,"ID",LVCFMT_CENTER,30,-1);
	gm_List_AnalogUser->InsertColumn(1,"Mod,Ch",LVCFMT_CENTER,50,-1);
	gm_List_AnalogUser->InsertColumn(2,"Type",LVCFMT_CENTER,60,-1);
	gm_List_AnalogUser->InsertColumn(3,"LineState",LVCFMT_CENTER,70,-1);
	gm_List_AnalogUser->InsertColumn(4,"State",LVCFMT_CENTER,90,-1);
	gm_List_AnalogUser->InsertColumn(5,"DTMF",LVCFMT_CENTER,50,-1);
	gm_List_AnalogUser->InsertColumn(6,"VocInfo",LVCFMT_CENTER,60,-1);
	gm_List_AnalogUser->InsertColumn(7,"LinkDev",LVCFMT_CENTER,60,-1);
	gm_List_AnalogUser->InsertColumn(8,"FailReason",LVCFMT_CENTER,80,-1);

}
//系统资源的动态详细显示-----------------------------------------------------------
void InitListMsg(void){
	int	i = pdlg->m_List_Msg.GetHorizontalExtent();
	pdlg->m_List_Msg.SetHorizontalExtent ( i+1000 );
}
//向消息框添加信息函数--------------------------------------------------------------
void AddMsg (char *str)
{
	char		TmpStr[256]={0};
	static	int		iTotal_ListMsg = 0;
	
	if(pdlg->m_List_Msg.GetCount() >= 1000)
	{
		pdlg->m_List_Msg.ResetContent();
		iTotal_ListMsg = 0;
	}else
	{
		iTotal_ListMsg = pdlg->m_List_Msg.GetCount();
	}
	
	sprintf( TmpStr, "%3d: ", iTotal_ListMsg+1 );
	strcat( TmpStr, str );
	pdlg->m_List_Msg.AddString ( TmpStr );
	
	iTotal_ListMsg++;	
	pdlg->m_List_Msg.SetCurSel (iTotal_ListMsg-1);

}
//初始化文件XMS_SwitchDemo.INI配置框的初始化---------------------------------------
void InitTextBox(void){
	char TmpStr[256];
	pdlg->GetDlgItem(IDC_EDIT_IPADDR)->SetWindowText(cfg_ServerID.m_s8ServerIp);

	sprintf(TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	pdlg->GetDlgItem(IDC_EDIT_IPPORT)->SetWindowText(TmpStr);
	pdlg->GetDlgItem(IDC_EDIT_VOCPATH)->SetWindowText(cfg_VocPath);
	
	sprintf(TmpStr, "%d", cfg_iCalledLen);
	pdlg->GetDlgItem(IDC_EDIT_CALLEDLEN)->SetWindowText(TmpStr);
	pdlg->GetDlgItem(IDC_EDIT_CALLINGNUM)->SetWindowText(cfg_CallingNum);

	if ( cfg_iVoiceRule == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_FIX))->SetCheck ( 1 ) ;
	else if (cfg_iVoiceRule == 1 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_SAMEMOD))->SetCheck ( 1 ) ;
	else 
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_MOSTFIRST))->SetCheck ( 1 ) ;

	if ( cfg_iCallOutRule == 2 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_UU))->SetCheck ( 1 ) ;
	else if (cfg_iCallOutRule == 3 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_UT))->SetCheck ( 1 ) ;
	
	if ( cfg_iPartWork == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_PARTWORK))->SetCheck ( 0 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_PARTWORK))->SetCheck ( 1 ) ;
	
	sprintf ( TmpStr, "%d", cfg_iPartWorkModuleID );
	pdlg->GetDlgItem ( IDC_EDIT_MODULEID )->SetWindowText ( TmpStr );
	
	if ( cfg_s32DebugOn == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 0 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 1 ) ;
}
//打开接口通道设备----------------------------------------------------------------
void OpenTrunkDevice( TRUNK_STRUCT *pOneTrunk ){
	RetCode_t r;	
	if( pOneTrunk->State == TRK_WAITOPEN ){    // not Open yet
		r = XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
		if( r < 0 ){
			AddMsg("XMS_ctsOpenDevice Fail in OpenTrunkDevice()!");
		}
	}
}
//打开语音设备------------------------------------------------------------------
void OpenVoiceDevice( VOICE_STRUCT *pOneVoice ){
	RetCode_t r;
	if( pOneVoice->State == VOC_WAITOPEN )		// not Open yet
	{
		r = XMS_ctsOpenDevice( g_acsHandle, &pOneVoice->deviceID, NULL );
		if( r < 0 ){
			AddMsg( "OpenVoiceDevice() Fail!" );
		}
	}
}

//打开板卡设备------------------------------------------------------------
void OpenBoardDevice( DJ_S8 s8DspModID )
{
	RetCode_t r;
	if ( AllDeviceRes[s8DspModID].bOpenFlag == false )	// not Open yet
	{
		r = XMS_ctsOpenDevice ( g_acsHandle, &AllDeviceRes[s8DspModID].deviceID, NULL );
		
		if ( r < 0 ){
			AddMsg ( "XMS_ctsOpenDevice Fail in OpenBoardDevice()!" );
		}
	}
}
//打开模块号为s8DspModID的DSP板卡设备上的所有设备---------------------------------------------------
void OpenAllDevice_Dsp( DJ_S8 s8DspModID ){
	int i;
	
	AllDeviceRes[s8DspModID].bErrFlag = false;
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_NONE;//Do not remove the DSP hardware
	DrawCount_ErrFlag(s8DspModID);//更改板卡列表框的 标志位ErrFlag
	DrawCount_RemoveState(s8DspModID);//更改板卡列表框的 RemoveState标志位
	
	//打开板卡设备(DSP模块)
	OpenBoardDevice( s8DspModID );
	
	//打开语音通道设备
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice( &AllDeviceRes[s8DspModID].pVoice[i] );
	}
		
	//打开接口通道设备
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}
}

void OpenDeviceOK( DeviceID_t *pDevice ){
	TRUNK_STRUCT *pOneTrunk;

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )//板卡设备
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = true;
		SetGTD_ToneParam( pDevice);
	}
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )//通道接口设备
	{
		pOneTrunk = &M_OneTrunk(*pDevice);
		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;// this line is very important, must before all operation

		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )//子设备类型为坐席设备
		{
			InitUserChannel( pOneTrunk );			
			XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );			
			g_iTotalUserOpened ++; //代开的坐席数增加
			DrawCount_User( pDevice->m_s8ModuleID );
			
		}
		else
		{
			InitTrunkChannel ( pOneTrunk );			
			XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
			
		}
		// modify the count
		g_iTotalTrunkOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;

		DrawCount_Trunk ( pDevice->m_s8ModuleID );

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
		}else if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
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

}
// Special code for Analog
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

void Change_State( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{
	pOneTrunk->State = NewState;
	DrawMain_State( pOneTrunk );

	// Check if ready to remove DSP hardware
	if ( (AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == TRK_FREE) )
	{
		pOneTrunk->State = TRK_WAIT_REMOVE;
		DrawMain_State( pOneTrunk );
		CheckRemoveReady ( pOneTrunk->deviceID.m_s8ModuleID );
	}
}
void CheckRemoveReady ( DJ_S8 s8DspModID )
{
	int i;

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

// Search Free Voice for get CallerID(FSK)
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
//改变语音设备状态-----------------------------------------------------------
void Change_Voc_State ( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState )
{
	pOneVoice->State = NewState;
}
// --------------------------------------------------------------------------------
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

void My_InitDtmfBuf( TRUNK_STRUCT *pOneTrunk )
{
	pOneTrunk->DtmfCount = 0;
	pOneTrunk->DtmfBuf[0] = 0; 
}
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
void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	int retCode = XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	char tempStr[10];
	sprintf(tempStr,"retCode: %d",retCode);
	AddMsg(tempStr);
	XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
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
		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			Change_UserState( pOneTrunk, USER_WAITOPEN );
		}
		Change_State ( pOneTrunk, TRK_WAITOPEN );

		// modify the count
		g_iTotalTrunkOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened --;

		DrawCount_Trunk( pDevice->m_s8ModuleID  );
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

}

// -------------------------------------------------------------------------------------------------
void	HandleDevState ( Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT	*pOneTrunk;
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


}
//初始化中继通道设备--------------------------------------------------------
void InitTrunkChannel( TRUNK_STRUCT *pOneTrunk )
{	
	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	pOneTrunk->CallerCode[0]=0;
	pOneTrunk->CalledCode[0]=0;

	Change_State( pOneTrunk, TRK_FREE );
	My_InitDtmfBuf( pOneTrunk );
	DrawMain_DTMF( pOneTrunk );	
	DrawMain_VocInfo( pOneTrunk );
	DrawMain_CallInfo(pOneTrunk);
}
//---------------------------
void ResetTrunk( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )//有语音设备与此中继连接
	{
		My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
		FreeOneFreeVoice (  &pOneTrunk->VocDevID );
		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
	}
	if ( pOneTrunk->LinkDevID.m_s16DeviceMain != 0 ){//当前坐席有其他设备与其连接
		My_DualUnlink ( &pOneTrunk->LinkDevID, &pOneTrunk->deviceID );				
	}
	InitTrunkChannel( pOneTrunk );
	// Search Free Voice for get CallerID(FSK)
	// if you needn't CallerID, ignore next line
	PrepareForCallerID( pOneTrunk );
}
void ResetUser( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt ){
	TRUNK_STRUCT		*pLinkDev = NULL;
	TRUNK_STRUCT		*pLinkUser = NULL;
	
	if ( pOneUser->VocDevID.m_s16DeviceMain != 0 ){//当前坐席有语音资源与其连接
		My_DualUnlink( &pOneUser->VocDevID, &pOneUser->deviceID );
		FreeOneFreeVoice(  &pOneUser->VocDevID );
	}	
	if ( pOneUser->LinkDevID.m_s16DeviceMain != 0 ){//当前坐席有其他设备与其连接
		My_DualUnlink ( &pOneUser->deviceID, &pOneUser->LinkDevID );
		pLinkDev = &M_OneTrunk(pOneUser->LinkDevID);		
		memset ( &pLinkDev->LinkDevID, 0, sizeof(DeviceID_t) );// 0: Didn't alloc Link Device 
	
		XMS_ctsClearCall ( g_acsHandle, &pOneUser->LinkDevID, 0, NULL );
	}

	if ( pOneUser->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER ) 
	{
		if ( pLinkDev->UserState == USER_RING )
		{
			InitUserChannel ( pLinkDev );
		}
	}else if ( pOneUser->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK) {
		AddMsg("ResetUser() & 对方子设备类型 XMS_DEVSUB_ANALOG_TRUNK");
		if ( pLinkDev->UserState == TRK_CALLOUT )
		{
			InitTrunkChannel(pLinkDev);
		}

	}

	InitUserChannel( pOneUser );
}

//业务处理函数--------------------------------------------------------------------
void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt ){
	Acs_CallControl_Data *	pCallControl = NULL;
	DeviceID_t				FreeVocDeviceID;
	DeviceID_t				OutUserDeviceID;
	char					FileName[256];
	char					tmpStr[256];
	char					TmpDtmf, TmpGtd;
	RetCode_t		        r;
	TRUNK_STRUCT			*pLinkUser;
	TRUNK_STRUCT			*pLinkDev;
	/*XMS_EVT_CLEARCALL触发条件: 调用XMS_ctsClearCall()：成功发送挂机信号后返回
	被动挂机完成后返回的事件;对于Analog Trunk在调用API函数XMS_ctsClearCall()成功后返回;
	对于Analog User在调用API函数XMS_ctsClearCall()成功后，或在检测到对方挂机时候返回。*/
	if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ){	
		if( (pOneTrunk->State != TRK_FREE) && (pOneTrunk->State != TRK_WAIT_REMOVE) )
		{
			ResetTrunk( pOneTrunk, pAcsEvt );
			return;
		}
	}
	TmpGtd = My_GetGtdCode( pAcsEvt );
	
	if( pOneTrunk->State  != TRK_CONNECT ){
		if( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )// Busy Tone
		{
			XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);//复位设备
			XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);//清除呼叫
			ResetTrunk(pOneTrunk,pAcsEvt);
		}
	}

	switch(pOneTrunk->State)
	{
		case TRK_FREE: 
			SpecialForCas(pOneTrunk,pAcsEvt);//Special code for CAS(SS1)    XMS_DEVSUB_E1_CAS:E1一号信令话路设备	
			//XMS_EVT_CALLIN 触发条件: 对于Analog Trunk设备检测到振铃事件后返回
			if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN ){
				AddMsg("TRK_FREE & XMS_EVT_CALLIN...............................");
				pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
				
				unsigned char *p;				
				p = My_GetFskCode( pAcsEvt );
				if ( p != NULL ){
					ConvertRawFskToCallerID( p, pOneTrunk->CallerCode, 20 );
				}
				if (pOneTrunk->VocDevID.m_s16DeviceMain != 0)//如果此中继关联语音资源
				{
					My_DualUnlink( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
					FreeOneFreeVoice(  &pOneTrunk->VocDevID );					
					memset( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,0, sizeof(DeviceID_t) );	// 0: didn't alloc Device
					memset( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );						// 0: didn't alloc Device
					DrawMain_VocInfo( pOneTrunk );
				}
				
				XMS_ctsAlertCall( g_acsHandle, &pOneTrunk->deviceID, NULL );    //振铃				
				XMS_ctsAnswerCallIn( g_acsHandle, &pOneTrunk->deviceID, NULL ); //呼入应答
				
				DrawMain_CallInfo( pOneTrunk );
				Change_State( pOneTrunk, TRK_WAIT_ANSWERCALL );
			}
			break;
		case TRK_WAIT_ANSWERCALL://代表会话成功建立了			
			SpecialForCas(pOneTrunk,pAcsEvt);// Special code for CAS(SS1)
			//XMS_EVT_ANSWERCALL 调用XMS_ctsAnswerCallIn()：成功执行后返回。
			//对于Analog Trunk，当用户调用API函数XMS_cstAnswerCall()成功的时候返回;对于Analog User，在检测到对方摘机时返回。
			if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	/*Answer Call In End Event*/
			{
				AddMsg("TRK_WAIT_ANSWERCALL & XMS_EVT_ANSWERCALL 会话建立成功");
				pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
				
				if ( SearchOneFreeVoice( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
				{
					pOneTrunk->u8PlayTag = 16;					
					pOneTrunk->VocDevID = FreeVocDeviceID;					
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID;					
					DrawMain_VocInfo( pOneTrunk );					
					My_DualLink( &FreeVocDeviceID, &pOneTrunk->deviceID );						
					SetGtD_AnalogTrunk(&FreeVocDeviceID);// prepare for get Busy Tone
					Change_State( pOneTrunk, TRK_WAIT_LINKOK );
				}
			}
			break;

		case TRK_WAIT_LINKOK://设备间时隙连接建立成功: Analog Trunk, Analog User：当用户调用API函数XMS_ctsLinkDevice()成功后，向用户返回
			if ( pAcsEvt->m_s32EventType == XMS_EVT_LINKDEVICE )
			{
				AddMsg("TRK_WAIT_LINKOK & XMS_EVT_LINKDEVICE 中继通道设备与语音通道设备 时隙连接建立成功");
				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "Bank.001");
				pOneTrunk->u8PlayTag ++;
				PlayFile( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag,false);
				Change_State ( pOneTrunk, TRK_WELCOME );			
			}
			break;

		case TRK_WELCOME:
			if ( CheckPlayEnd ( pOneTrunk, pAcsEvt) )	/*play end Event*/
			{
				strcpy ( FileName, cfg_VocPath );
				strcat ( FileName, "Bank.002");
				pOneTrunk->u8PlayTag ++;
				PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag,false );
				
				Change_State ( pOneTrunk, TRK_EXTNUM );
			}
			
			TmpDtmf = My_GetDtmfCode ( pAcsEvt );
			if ( TmpDtmf != -1 )								/*DTMF key*/
			{
				My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
				DrawMain_DTMF ( pOneTrunk );				
				StopPlayFile ( &pOneTrunk->VocDevID );				
				Change_State ( pOneTrunk, TRK_EXTNUM );
			}
			break;

		case TRK_EXTNUM:
			TmpDtmf = My_GetDtmfCode ( pAcsEvt );
			if ( TmpDtmf != -1 )								/*DTMF Key*/
			{
				My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
				DrawMain_DTMF ( pOneTrunk );				
				StopPlayFile ( &pOneTrunk->VocDevID );				
				if ( pOneTrunk->DtmfCount >= 4 )//输入四位分机号
				{
					int i = GetOutUserID( pOneTrunk->DtmfBuf,  &OutUserDeviceID );//随便找了一个空闲坐席,连接通话
					if( i > -1 ){
						r = XMS_ctsMakeCallOut ( g_acsHandle, &OutUserDeviceID, pOneTrunk->DtmfBuf,"1001",NULL );
						if (r>0)
						{							
							//给外线播放背景音乐
							strcpy ( FileName, cfg_VocPath );
							strcat ( FileName, "ConfMusi.Pcm");
							pOneTrunk->u8PlayTag ++;
							PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag,false );
							
							// call out OK
							pLinkUser = &M_OneTrunk(OutUserDeviceID);
							pOneTrunk->LinkDevID = OutUserDeviceID;
							pLinkUser->LinkDevID = pOneTrunk->deviceID; 
							
							DrawUser_LinkDev ( pLinkUser );							
							Change_State ( pOneTrunk, TRK_WAITCONNECT );
							Change_UserState ( pLinkUser, USER_RING );
							My_InitDtmfBuf( pOneTrunk );
							
						}else{
							AddMsg("TRK_EXTNUM & XMS_ctsMakeCallOut() fail...");
							Change_State( pOneTrunk, TRK_CONNECT_FAILURE );	
						}
					}else{
						AddMsg("TRK_EXTNUM & 未找到空闲的坐席资源");
						Change_State( pOneTrunk, TRK_CONNECT_FAILURE );					
					}					
				}
			}
			break;
		case TRK_WAITCONNECT:
			break;	
		case TRK_CONNECT_FAILURE:
			break;
		case TRK_CALLOUT:

			if( TmpGtd  == 'h' || TmpGtd=='P')		//收到 回铃音结束h，或者爆破音P
			{
				pLinkDev = &M_OneTrunk(pOneTrunk->LinkDevID);
				Change_State(pOneTrunk,TRK_CONNECT);
				if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
				{
					Change_UserState(pLinkDev,USER_CONNECT);
				}			
			}
			if ( pAcsEvt->m_s32EventType == XMS_EVT_ANALOG_INTERFACE )
			{
				sprintf(tmpStr,"A_TRK(%d,%d) 没有接线!",pAcsEvt->m_DeviceID.m_s8ModuleID,pAcsEvt->m_DeviceID.m_s16ChannelID);
				AddMsg(tmpStr);
			}
			break;
		case TRK_CONNECT:
			if( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )// Busy Tone
			{
// 				XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);//复位设备
// 				XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);//清除呼叫
// 				ResetTrunk(pOneTrunk,pAcsEvt);
 				pLinkDev = &M_OneTrunk(pOneTrunk->LinkDevID);
				if (pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
				{
					if ( pLinkDev->VocDevID.m_s16DeviceMain != 0 )//坐席是否连接了Voc
					{
						AddMsg("USER_CONNECT & XMS_EVT_CLEARCALL Event  & 有语音资源连接");
					}else{						
						if ( SearchOneFreeVoice ( pLinkDev,  &FreeVocDeviceID ) >= 0 )
						{
							pLinkDev->VocDevID = FreeVocDeviceID;
							M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkDev->deviceID; 
							My_DualLink( &pLinkDev->VocDevID, &pLinkDev->deviceID );	
						}	
					}
					PlayTone( &pLinkDev->VocDevID, 2 ); // busy tone
					Change_UserState(pLinkDev, USER_CONNECT_FAILURE);
					DrawUser_FailReason(pLinkDev,"PEER ONHOOK");
				}
			}
			break;
		default:
			break;
	}
	
}

/* Stop Play */
DJ_S32 StopPlayFile( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;
	controlPlay.m_u16ControlType = XMS_STOP_PLAY;
	r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );
	return r;
}
void My_DualUnlink( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}
int FreeOneFreeVoice(  DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8	s8ModID;

	s8ModID = pFreeVocDeviceID->m_s8ModuleID;
	if ( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		Change_Voc_State( &M_OneVoice(*pFreeVocDeviceID), VOC_FREE);

		AllDeviceRes[s8ModID].lVocFreeNum++;
		g_iTotalVoiceFree++;
		DrawCount_Voc( s8ModID );
		return	0;		// OK
	}
	return -1;	// invalid VocDeviceID
}
char My_GetGtdCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data		*pIOData = NULL;
	char			sbuffer[200];
	char			*p;

	memset(sbuffer,0,sizeof(sbuffer));
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		if ( ( pIOData->m_u16IoType == XMS_IO_TYPE_GTG ) && ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			sprintf(sbuffer,"My_GetGtdCode: [%s] ",p);
			AddMsg(sbuffer);
			return *p;
		}
	}
	return -1;	// not a good GTD
}
// Special code for CAS(SS1)   XMS_DEVSUB_E1_CAS:E1一号信令话路设备
void SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t FreeVocDeviceID;	
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
unsigned char *My_GetFskCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	unsigned char		*p;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Evemt*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		
		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_FSK ) && ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (unsigned char *)FetchIOData(pAcsEvt);			
			return p;
		}
	}
	
	return NULL;	// not a good FSK
}
/* Handle Play */
DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue=false )
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
	//playProperty.m_u32StopMode = 
	strcpy ( playProperty.m_s8PlayContent, s8FileName );
	
	r = XMS_ctsPlay ( g_acsHandle, pVocDevID, &playProperty, NULL );
	
	return r;
}
DJ_S32	PlayTone ( DeviceID_t	*pVocDevID, int iPlayType )
{
	DJ_U32           i = 0;	
	DJ_U16           u16IoLen = 0;
	DJ_U16           u16IoType = 0;
	RetCode_t		 r;
	char		 	 IoDataBuf[MAX_SEND_IODATA_DTMF_LEN]={0};
	
	u16IoType = XMS_IO_TYPE_GTG;
	u16IoLen = 1;
	
	switch ( iPlayType )
	{
	case -1:    // Stop Play Tone
		u16IoLen = 0;
		break;
	case 0:		// Dial Tone
		IoDataBuf[0] = 'G';
		break;
	case 1:		// Ring Back Tone
		IoDataBuf[0] = 'H';
		break;
	case 2:		// Busy Tone
		IoDataBuf[0] = 'I';
		break;
	case 3:		// Busy Tone
		IoDataBuf[0] = 'J';
		break;
	case 4:		// Busy Tone
		IoDataBuf[0] = 'K';
		break;
	}
	
	//发送DTMF/TONE音/FSK/数据
	r = XMS_ctsSendIOData( g_acsHandle, pVocDevID, u16IoType,u16IoLen,IoDataBuf);	
	return	r;	
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
			sprintf(sbuffer,"Recevice DTMF");
			AddMsg(sbuffer);
			
			return *p;
		}
	}
	
	return -1;	// not a good DTMF
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
void Change_UserState ( TRUNK_STRUCT *pOneUser, USER_STATE NewState )
{
	pOneUser->UserState = NewState;
	DrawUser_State( pOneUser );
}
void InitUserChannel( TRUNK_STRUCT *pOneUser )
{
	Change_UserState( pOneUser, USER_FREE );	
	My_InitDtmfBuf( pOneUser );	
	memset( &pOneUser->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	memset( &pOneUser->LinkDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Link Device	
	DrawUser_VocInfo( pOneUser );
	DrawUser_LinkDev( pOneUser );
	DrawUser_DTMF(pOneUser);
	DrawUser_FailReason(pOneUser," ");
}

void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	char					TmpDtmf;
	DeviceID_t				FreeVocDeviceID, FreeTrkDeviceID, OutUserDeviceID;
	TRUNK_STRUCT			*pLinkTrunk = NULL;
	TRUNK_STRUCT			*pLinkUser = NULL;
	TRUNK_STRUCT			*pLinkDev = NULL;
	Acs_GeneralProc_Data	*pGeneral = NULL;
	Acs_CallControl_Data	*pCallControl = NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ) /*当前状态USER_RING,坐席挂机事件User Offhook Event*/
	{
		if(pOneUser->UserState != USER_FREE && pOneUser->UserState != USER_CONNECT)
		{
			ResetUser( pOneUser, pAcsEvt );
			return;
		}
	}
	switch(pOneUser->UserState)
	{
	case USER_FREE:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN ) /*当前状态USER_FREE,坐席摘机事件User Offhook Event*/
		{
			AddMsg("USER_FREE & XMS_EVT_CALLIN Event.............................");
			if ( SearchOneFreeVoice ( pOneUser,  &FreeVocDeviceID ) >= 0 )
			{
				pOneUser->VocDevID = FreeVocDeviceID;
				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneUser->deviceID; 
				DrawUser_VocInfo( pOneUser );
				My_DualLink( &FreeVocDeviceID, &pOneUser->deviceID );
				if ( cfg_iCallOutRule == 2 )// 坐席呼坐席
				{
					PlayTone( &pOneUser->VocDevID, 0 );// dial tone	
					Change_UserState ( pOneUser, USER_OFFHOOK );
				}else if ( cfg_iCallOutRule == 3 )//坐席呼外线
				{
					if (SearchOneFreeTrunk( &FreeTrkDeviceID ) > 0 )//找到空闲模拟中继通道
					{ 
						if (pOneUser->VocDevID.m_s16DeviceMain != 0)
						{
							My_DualUnlink(&pOneUser->deviceID ,&pOneUser->VocDevID);//断开坐席与语音设备连接
							FreeOneFreeVoice(&pOneUser->VocDevID); //释放语音资源
						}
						
						My_DualLink( &FreeTrkDeviceID, &pOneUser->deviceID );//坐席与找到的空闲中继建立连接,SendIOData通过坐席电话发送
						if( XMS_ctsMakeCallOut ( g_acsHandle, &FreeTrkDeviceID, cfg_CallingNum, pOneUser->DtmfBuf, NULL ) > 0 ){ //call out OK
							pLinkTrunk = &M_OneTrunk(FreeTrkDeviceID);							
							pOneUser->LinkDevID = FreeTrkDeviceID;
							pLinkTrunk->LinkDevID = pOneUser->deviceID; 
							DrawUser_LinkDev( pOneUser );
							
							strcpy ( pLinkTrunk->CallerCode, cfg_CallingNum );
							DrawMain_CallInfo( pLinkTrunk );
							
							Change_UserState( pOneUser, USER_CALLOUT );
							Change_State( pLinkTrunk, TRK_CALLOUT);							
						}else//call out fail
						{ 
							AddMsg ("坐席拨打外线失败 XMS_ctsMakeCallOut() FAIL!");
							Change_UserState( pOneUser, USER_CONNECT_FAILURE );
						}
					}else
					{//无法找到可用中继
						AddMsg ("USER_OFFHOOK & SearchOneFreeTrunk() FAIL! 无法找到可用中继");
						PlayTone( &pOneUser->VocDevID, 2 );// busy tone
						Change_UserState( pOneUser, USER_CONNECT_FAILURE );
					}// end of For Link User & Trunk
				}
				
			}else
			{
				AddMsg("USER_FREE & SearchOneFreeVoice() Fail...");
			}
		}else
		{
			AddMsg("USER_FREE & not XMS_EVT_CALLIN Event");
		}
		break;
		
	case USER_OFFHOOK:
		TmpDtmf = My_GetDtmfCode( pAcsEvt );
		AddMsg("USER_OFFHOOK.....................");
		if ( TmpDtmf != -1 )								/*DTMF Event*/
		{
			AddMsg("USER_OFFHOOK DTMF Event.............................");
			PlayTone(&pOneUser->VocDevID,-1);//停止播音
			My_AddDtmfBuf ( pOneUser, TmpDtmf );
			DrawUser_DTMF( pOneUser );	
			if ( pOneUser->DtmfCount >= cfg_iCalledLen ){   //拨打号码不能多于四位
				if ( cfg_iCallOutRule == 2 ){		// 坐席呼坐席
					AddMsg("坐席呼坐席.........");
					if( GetOutUserID( pOneUser->DtmfBuf,  &OutUserDeviceID ) > -1 ){//随便找了一个空闲坐席,连接通话
						if ( XMS_ctsMakeCallOut ( g_acsHandle, &OutUserDeviceID, cfg_CallingNum, pOneUser->DtmfBuf, NULL ) > 0 ){     // call out OK
							PlayTone( &pOneUser->VocDevID, 1 );	// RingBack tone							
							pLinkUser = &M_OneTrunk(OutUserDeviceID);
							pOneUser->LinkDevID = OutUserDeviceID;
							pLinkUser->LinkDevID = pOneUser->deviceID; 
							DrawUser_LinkDev ( pOneUser );
							DrawUser_LinkDev ( pLinkUser );
							Change_UserState ( pOneUser, USER_CALLOUT );
							Change_UserState ( pLinkUser, USER_RING );
							break;
						}else{//坐席呼坐席呼叫失败
							AddMsg("坐席呼坐席失败  XMS_ctsMakeCallOut() FAIL!");
							PlayTone( &pOneUser->VocDevID, 2 );// busy tone
							Change_UserState( pOneUser, USER_CONNECT_FAILURE );
						}
					}else{//无法找到可用坐席
						AddMsg ("USER_OFFHOOK & GetOutUserID() FAIL 无法找到空闲坐席");
						PlayTone( &pOneUser->VocDevID, 2 );// busy tone
						Change_UserState( pOneUser, USER_CONNECT_FAILURE );
					}// end of For Link User & User					
				}else{
					AddMsg("配置文件中没有定义此 呼叫规则");
				}		
			}
		}
		break;
	case USER_CALLOUT:
		AddMsg("USER_CALLOUT & XMS_EVT_CLEARCALL Event.............................");
		break;
	case USER_CONNECT://产生挂机事件的坐席向与其连接的坐席，发送忙音
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ) /*当前状态USER_RING,坐席挂机事件User Offhook Event*/
		{
			AddMsg("USER_CONNECT & XMS_EVT_CLEARCALL Event.............................");
			pLinkDev = &M_OneTrunk(pOneUser->LinkDevID);
			if (pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
			{
				My_DualUnlink( &pOneUser->deviceID, &pLinkDev->deviceID );//断开坐席间双向连接
				if ( pLinkDev->VocDevID.m_s16DeviceMain != 0 )//坐席是否连接了Voc
				{
					AddMsg("USER_CONNECT & XMS_EVT_CLEARCALL Event  & 有语音资源连接");
				}else{
					
					if ( SearchOneFreeVoice ( pLinkDev	,  &FreeVocDeviceID ) >= 0 )
					{
						pLinkDev->VocDevID = FreeVocDeviceID;
						M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkDev->deviceID; 
						My_DualLink( &pLinkDev->VocDevID, &pLinkDev->deviceID );	
					}	
				}
				PlayTone( &pLinkDev->VocDevID, 2 ); // busy tone
				Change_UserState(pOneUser, USER_CONNECT_FAILURE);
				Change_UserState(pLinkDev, USER_CONNECT_FAILURE);
				DrawUser_FailReason(pLinkDev,"PEER ONHOOK");				
			
				if ( pOneUser->VocDevID.m_s16DeviceMain != 0 ){//当前坐席有语音资源与其连接
					My_DualUnlink( &pOneUser->VocDevID, &pOneUser->deviceID );
					FreeOneFreeVoice(  &pOneUser->VocDevID );
					DrawUser_VocInfo(pOneUser);
				}
				InitUserChannel(pOneUser);
				
			}else if (pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)
			{
				My_DualUnlink( &pOneUser->deviceID, &pLinkDev->deviceID );//断开坐席和中继间双向连接
				if ( pLinkDev->VocDevID.m_s16DeviceMain != 0 )//中继是否连接Voc
				{
					AddMsg("USER_CONNECT & XMS_EVT_CLEARCALL Event  & 有语音资源连接");
				}else{
					
					if ( SearchOneFreeVoice ( pLinkDev	,  &FreeVocDeviceID ) >= 0 )
					{
						pLinkDev->VocDevID = FreeVocDeviceID;
						M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkDev->deviceID; 
						My_DualLink( &pLinkDev->VocDevID, &pLinkDev->deviceID );	
					}	
				}
				ResetUser( pOneUser, pAcsEvt );
				XMS_ctsClearCall(g_acsHandle, &pLinkDev->deviceID,0,NULL );
				
				Change_State(pLinkDev, TRK_CONNECT_FAILURE);
				
			}
			
		}
		break;
	
	// User to User's call
	case USER_RING:	
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT ) 
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);//连接数据获取
			pLinkDev = &M_OneTrunk(pOneUser->LinkDevID);
			if (pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)//坐席呼叫处理.....................
			{
				if ( pCallControl->m_s32AcsEvtState == 1 )	//呼叫成功，坐席摘机
				{
					AddMsg("USER_RING & XMS_EVT_CALLOUT Event.............................");
					Change_UserState ( pOneUser, USER_OFFHOOK );
					pLinkUser = &M_OneTrunk(pOneUser->LinkDevID);					
					if ( pLinkUser->VocDevID.m_s16DeviceMain != 0 )//连接坐席语音设备相连
					{
						PlayTone ( &pLinkUser->VocDevID, -1 );//stop playtone wait tone
						My_DualUnlink ( &pLinkUser->VocDevID, &pLinkUser->deviceID );				
						FreeOneFreeVoice (  &pLinkUser->VocDevID );				
						memset ( &pLinkUser->VocDevID, 0, sizeof(DeviceID_t) );	// 0: Didn't alloc Voc Device					
						DrawUser_VocInfo ( pLinkUser );//连接的设备是坐席，改变VocInfo					
					}				
					My_DualLink ( &pOneUser->deviceID, &pLinkUser->deviceID );//建立时隙连接				
					Change_UserState ( pOneUser, USER_CONNECT );
					Change_UserState ( pLinkUser, USER_CONNECT );  //如果连接设备为坐席，改变状态
				}else{ //呼叫失败
					
					AddMsg("USER_RING & 连接失败原因：");
					char errMsg[100];
					sprintf(errMsg,"Err=%d,Cause=%d",pCallControl->m_s32AcsEvtErrCode,pCallControl->m_s32CallClearCause);
					AddMsg(errMsg);
					pLinkUser = &M_OneTrunk(pOneUser->LinkDevID);
					Change_UserState(pOneUser, USER_CONNECT_FAILURE);
					Change_UserState(pLinkUser, USER_CONNECT_FAILURE);
					DrawUser_FailReason( pLinkUser, "Peer Nobody");		
					My_DualUnlink( &pOneUser->deviceID, &pLinkUser->deviceID );//断开坐席间双向连接
					
					//???????????????????????????? 已经有语音资源与连接坐席连接了啊	
					if ( pLinkUser->VocDevID.m_s16DeviceMain != 0 ){
						AddMsg("此坐席已经有语音设备。。。。。。。。");
						PlayTone(&pLinkUser->VocDevID,2);
					}else if ( SearchOneFreeVoice ( pLinkUser	,  &FreeVocDeviceID ) >= 0 )
					{
						pLinkUser->VocDevID = FreeVocDeviceID;
						M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkUser->deviceID; 
						My_DualLink( &pLinkUser->VocDevID, &pLinkUser->deviceID );
						PlayTone(&pLinkUser->VocDevID,2);
					}else{
						AddMsg("无空闲语音资源");
					}				
				}// 坐席呼叫处理结束...........................
			}else if (pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)//中继呼叫处理开始......................
			{
				if ( pCallControl->m_s32AcsEvtState == 1 )	//呼叫成功，坐席摘机
				{
					AddMsg("USER_RING & XMS_EVT_CALLOUT Event.............................");
					Change_UserState ( pOneUser, USER_OFFHOOK );
					pLinkTrunk = &M_OneTrunk(pOneUser->LinkDevID);
					
					PlayTone ( &pLinkTrunk->VocDevID, -1 );//stop playtone wait tone 
					if ( pLinkTrunk->VocDevID.m_s16DeviceMain != 0 )
					{
						My_DualUnlink ( &pLinkTrunk->VocDevID, &pLinkTrunk->deviceID );				
						FreeOneFreeVoice (  &pLinkTrunk->VocDevID );				
						memset ( &pLinkTrunk->VocDevID, 0, sizeof(DeviceID_t) );	// 0: Didn't alloc Voc Device					
						DrawMain_VocInfo ( pLinkTrunk );//连接的设备是中继，改变VocInfo					
					}				
					My_DualLink ( &pOneUser->deviceID, &pLinkTrunk->deviceID );//建立时隙连接				
					Change_UserState ( pOneUser, USER_CONNECT );
					Change_State ( pLinkTrunk, TRK_CONNECT );  //如果连接设备为坐席，改变状态
				}else{ //呼叫失败
					AddMsg("USER_RING & XMS_EVT_CALLOUT Event connect fail.............................");
					pLinkTrunk = &M_OneTrunk(pOneUser->LinkDevID);
					Change_UserState(pOneUser, USER_CONNECT_FAILURE);
					Change_UserState(pLinkTrunk, USER_CONNECT_FAILURE);	
					My_DualUnlink( &pLinkTrunk->deviceID, &pLinkUser->deviceID );//断开坐席间双向连接
					
					//???????????????????????????? 已经有语音资源与连接坐席连接了啊													
					if ( SearchOneFreeVoice ( pLinkTrunk,  &FreeVocDeviceID ) >= 0 )
					{
						pLinkTrunk->VocDevID = FreeVocDeviceID;
						M_OneVoice(FreeVocDeviceID).UsedDevID = pLinkTrunk->deviceID; 
						My_DualLink( &pLinkTrunk->VocDevID, &pLinkTrunk->deviceID );
						PlayTone(&pLinkTrunk->VocDevID,2);
					}else{
						AddMsg("无空闲语音资源");
					}				
				}//中继呼叫处理结束
			}else{
				AddMsg("子设备类型有误");
			}				
		}else{
			AddMsg("USER_RING & not XMS_EVT_CALLOUT event");
		}
		break;
	default:
		break;
	}
}



int	 GetOutUserID (  char *pDtmf, DeviceID_t *pUserDeviceID )
{
	int		iTmp;
	DJ_S8	s8ModID;
	DJ_S16	s16ChID;
	static	int	iLoopStart = 0;
	
	if ( cfg_iCallOutRule == 2 || cfg_iCallOutRule == 3)		// Select a Fix Channel by DTMF's tail 3
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
int	 SearchOneFreeTrunk( DeviceID_t *pFreeTrkDeviceID )
{
	DJ_S8	s8ModID;
	DJ_S16	s16ChID;
	for (int i=0; i<g_iTotalTrunk; i++)
	{
		s8ModID = MapTable_Trunk[i].m_s8ModuleID;
		s16ChID = MapTable_Trunk[i].m_s16ChannelID;		
		if ( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState == DCS_FREE )
			&& ( AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK ) )
		{
			*pFreeTrkDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			return 2;
		}
	}
	return -1;
}
bool IsTrunk ( DJ_S16 s16DevSub )
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
void	FetchFromText(void)
{
	char	TmpStr[256];
	
	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->GetWindowText ( cfg_ServerID.m_s8ServerIp, 30 );
	
	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_ServerID.m_u32ServerPort);
	
	pdlg->GetDlgItem ( IDC_EDIT_CALLEDLEN )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_iCalledLen );
	
	pdlg->GetDlgItem ( IDC_EDIT_CALLINGNUM )->GetWindowText ( cfg_CallingNum, 30 );
	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->GetWindowText ( cfg_VocPath, 128 );

	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_UT))->GetCheck ( ) )
		cfg_iCallOutRule = 3;
	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_UU))->GetCheck ( ) )
		cfg_iCallOutRule = 2;
	
	pdlg->GetDlgItem ( IDC_EDIT_CALLINGNUM )->GetWindowText ( cfg_CallingNum, 30 );
	
	if ( ((CButton *)pdlg->GetDlgItem (IDC_CHECK_PARTWORK))->GetCheck ( ) )
		cfg_iPartWork = 1;
	else
		cfg_iPartWork = 0;
	
	pdlg->GetDlgItem ( IDC_EDIT_MODULEID )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_iPartWorkModuleID);
	
	if ( ((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->GetCheck ( ) )
		cfg_s32DebugOn = 1;
	else
		cfg_s32DebugOn = 0;
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
	
	WritePrivateProfileString ( "ConfigInfo", "SimCalledRNum", cfg_CallingNum, cfg_IniName);
	
	sprintf ( TmpStr, "%d", cfg_iPartWork);
	WritePrivateProfileString ( "ConfigInfo", "PartWork", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%s", cfg_VocPath);
	WritePrivateProfileString ( "ConfigInfo", "VocPath", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iPartWorkModuleID);
	WritePrivateProfileString ( "ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);
	
	sprintf ( TmpStr, "%d", cfg_s32DebugOn);
	WritePrivateProfileString ( "ConfigInfo", "DebugOn", TmpStr, cfg_IniName);
}

void	ExitSystem(void) 
{
	RetCode_t	r;
	
	/*
	// close all device	
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
	CloseAllDevice_Dsp ( MapTable_Module[i] );
	}
	*/
	
	r = XMS_acsCloseStream ( g_acsHandle, NULL );
	
//	FreeAllDeviceRes ();
	
	// save to "XMS_Dial.INI"
	FetchFromText();
	WriteToConfig();
	
}