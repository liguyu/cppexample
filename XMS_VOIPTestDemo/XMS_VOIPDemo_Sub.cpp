#include "stdafx.h"
#include "XMS_VOIPTestDemo.h"
#include "XMS_VOIPTestDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_VOIPDemo_Sub.H"
#include "XMS_VOIPDemo_Event.H"
#include "XMS_VOIPDemo_String.H"

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_VOIPTestDemoApp theApp;
CXMS_VOIPTestDemoDlg	*pdlg = NULL;

// var about XMS_Demo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Demo.INI";

ServerID_t		cfg_ServerID;
int             cfg_Protocol;
char			cfg_VocPath[128];

int				cfg_iPartWorkModuleID[256] = {0};
char            cfg_chPartWorkModuleID[256] = {0};
int             cfg_iVoiceCheck = 0;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 100;

// var in XMS_Demo_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalVOIP;
extern int						g_iTotalVOIPOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_VOIP[MAX_VOIP_NUM_IN_THIS_DEMO];

extern int						g_iTotalVoice;
extern int						g_iTotalVoiceOpened;
extern int						g_iTotalVoiceFree;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_VOIP_NUM_IN_THIS_DEMO];

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
void   WriteMsgLog(char *pMsg)
{
	FILE   *fp;
	char   tmpTime[128], tmpDate[128];

	char   LogFileName[256] = {0}; 
	char   LogFileShortName[] = "\\VOIPSysMsgLogDemo.txt";
	char   MsgStr[512] = {0};
		
	if (pMsg == NULL)
		return;

	GetCurrentDirectory(MAX_FILE_NAME_LEN-32, LogFileName);
	strcat(LogFileName, LogFileShortName);

	if ((fp = fopen(LogFileName, "a+t")) == NULL)
	{
		sprintf(MsgStr, "fopen(%s) error.", LogFileName);
		AddMsg(MsgStr);		
	}
	else
	{		
		_strtime(tmpTime);
		_strdate(tmpDate);
		
		sprintf(MsgStr, "%s %s  %s\n",tmpDate ,tmpTime, pMsg);
		fwrite(MsgStr, strlen(MsgStr), 1, fp);
	}

	if (fp != NULL)
		fclose(fp);	
	
}

// -----------------------------------------------------------------------
int    SplitStr2Int(DJ_S8 *str, DJ_S8 *sep, int buf[])
{
	DJ_U32   i = 0;
	DJ_S8    *p = NULL;
	DJ_S8    *pBuf = NULL;

	if ( (str==NULL)||(strlen(str)==0)||(sep==NULL)||(strlen(sep)==0) )
	{
		return -1;
	}

	pBuf = str;
	p = strtok(pBuf, sep);

	while ( p != NULL )
	{
		buf[i++] = atoi(p);
		p = strtok(NULL, sep);
	}

	return i;
}
// -----------------------------------------------------------------------
void	ReadFromConfig(void)
{
	char strTmp[256]={0};
	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );

	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);

	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);

	GetPrivateProfileString("ConfigInfo","UserName","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);

	cfg_Protocol = GetPrivateProfileInt("ConfigInfo", "Protocol", 0, cfg_IniName);

	cfg_Protocol = GetPrivateProfileInt ( "ConfigInfo", "Protocol", 0, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);

	GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(strTmp, cfg_chPartWorkModuleID, sizeof(strTmp));	
	SplitStr2Int(strTmp, ",", cfg_iPartWorkModuleID);

	cfg_iVoiceCheck = GetPrivateProfileInt("ConfigInfo", "VoiceCheck", 0, cfg_IniName);
}

void	WriteToConfig(void)
{
	char	TmpStr[256];

	WritePrivateProfileString("ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName);
	
	sprintf (TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	WritePrivateProfileString ( "ConfigInfo", "Port", TmpStr, cfg_IniName);

	WritePrivateProfileString("ConfigInfo","UserName",cfg_ServerID.m_s8UserName,cfg_IniName);
	WritePrivateProfileString("ConfigInfo","PassWord",cfg_ServerID.m_s8UserPwd,cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_Protocol);
	WritePrivateProfileString ( "ConfigInfo", "Protocol", TmpStr, cfg_IniName);
	
	WritePrivateProfileString ( "ConfigInfo", "VocPath", cfg_VocPath, cfg_IniName);

	sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
	WritePrivateProfileString("ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);

	sprintf(TmpStr, "%d", cfg_iVoiceCheck);
	WritePrivateProfileString("ConfigInfo", "VoiceCheck", TmpStr, cfg_IniName);
}

void	InitTextBox(void)
{
	char	TmpStr[256];

	pdlg->GetDlgItem(IDC_EDIT_IPADDR)->SetWindowText (cfg_ServerID.m_s8ServerIp);

	sprintf(TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->SetWindowText(TmpStr);

	if (cfg_Protocol == 0)
	{	
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_H323))->SetCheck(1);		
		
	}
	else
	{	
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_SIP))->SetCheck(1);
	}

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->SetWindowText(cfg_VocPath);


	sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
	pdlg->GetDlgItem(IDC_EDIT_ModuleID)->SetWindowText(TmpStr);

	if (cfg_iVoiceCheck == 0)
	{
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_NoVoice))->SetCheck(1);
	}
	else
	{
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_Voice))->SetCheck(1);
	}

}

void	FetchFromText(void)
{
	char	TmpStr[256];

	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->GetWindowText ( cfg_ServerID.m_s8ServerIp, 30 );

	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_ServerID.m_u32ServerPort);

	if (((CButton *)pdlg->GetDlgItem(IDC_RADIO_H323))->GetCheck() == 1)
	{
		cfg_Protocol = 0;
	}
	else if (((CButton *)pdlg->GetDlgItem(IDC_RADIO_SIP))->GetCheck() == 1)
	{
		cfg_Protocol = 1;
	}

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->GetWindowText ( cfg_VocPath, 128 );


	if (((CButton *)pdlg->GetDlgItem(IDC_RADIO_Voice))->GetCheck() == 1)
	{
		cfg_iVoiceCheck = 1;
	}
	else if(((CButton *)pdlg->GetDlgItem(IDC_RADIO_NoVoice))->GetCheck() == 1)
	{
		cfg_iVoiceCheck = 0;
	}

	pdlg->GetDlgItem(IDC_EDIT_ModuleID)->GetWindowText(cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID));
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

	pdlg->m_ListMsg.SetCurSel ( iTotal_ListMsg);
		
	iTotal_ListMsg ++;
}

// --------------------------------------------------------------------------------
void	InitListMain(void)
{
	if (pdlg == NULL)
		return;

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
	lvc.pszText = "Protocol" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 5, &lvc ); 

	lvc.iSubItem = 6;
	lvc.pszText = "Caller/Callee Address";
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 6, &lvc ); 

	lvc.iSubItem = 7;
	lvc.pszText = "Caller/Callee UserID";
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn (7, &lvc);

	lvc.iSubItem = 8;
	lvc.pszText = "Caller/Callee TelID";
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn (8, &lvc);

	lvc.iSubItem = 9;
	lvc.pszText = "VocInfo" ;
	lvc.cx = 70;
	pdlg->m_ListMain.InsertColumn ( 9, &lvc ); 
}

void DrawMain_LineState( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL || pdlg == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 

	// 
	GetString_LineState ( StateStr, pOneVOIP->iLineState );
	pdlg->m_ListMain.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawMain_State( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL || pdlg == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 

	//
	switch( pOneVOIP->State ) 
	{
	case VOIP_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case VOIP_FREE:		
		strcpy(StateStr,"空闲"); 
		break ;	

	case VOIP_RECORDFILE:
		strcpy(StateStr,"录音");
		break;

	case VOIP_PLAYRECORD:
		strcpy(StateStr,"播放留言");
		break;

	case VOIP_WAIT_ANSWERCALL:
		strcpy(StateStr,"等摘机完成");
		break;

	case VOIP_WAITBINDOK:
		strcpy(StateStr,"绑定语音资源");
		break;
	case VOIP_WAITHANGUP:
		strcpy(StateStr, "等待挂机");
		break;	
	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListMain.SetItemText ( iDispRow, 4, StateStr ); 
}


void DrawMain_CallInfo( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL || pdlg == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 

    if (pOneVOIP->iProtocol == 1)
		sprintf(StateStr, "%s", "H323");
	else
		sprintf(StateStr, "%s", "SIP");
	pdlg->m_ListMain.SetItemText(iDispRow, 5, StateStr);

	sprintf ( StateStr,"%s:%d / %s:%d", pOneVOIP->CallerAddr, pOneVOIP->iCallerPort,
		pOneVOIP->CalleeAddr, pOneVOIP->iCalleePort);
	pdlg->m_ListMain.SetItemText ( iDispRow, 6, StateStr ); 

	sprintf ( StateStr,"%s / %s", pOneVOIP->CallerUserID, pOneVOIP->CalleeUserID );
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr );

	sprintf ( StateStr,"%s / %s", pOneVOIP->CallerNum, pOneVOIP->CalleeNum );
	pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr );
}

void DrawMain_DTMF( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL || pdlg == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 

	strcpy(StateStr,pOneVOIP->DtmfBuf);
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 

}

void DrawMain_VocInfo( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;

	if (pOneVOIP == NULL || pdlg == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 

	if ( pOneVOIP->VocDevID.m_s16DeviceMain != 0 )
	{
		pDev = &pOneVOIP->VocDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}
	else
		strcpy(StateStr, "");

	pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 

}



// -------------------------------------------------------------------------------------------------
void	InitListCount(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;

	if (pdlg == NULL)
		return;
	
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
	lvc.pszText = "VOIP/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 2, &lvc ); 

	lvc.iSubItem = 3;
	lvc.pszText = "Voc/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 3, &lvc ); 

	lvc.iSubItem = 4;
	lvc.pszText = "VocFree" ;
	lvc.cx = 55;
	pdlg->m_ListCount.InsertColumn ( 4, &lvc ); 

}

void	DrawCount_VOIP ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	if (pdlg == NULL)
		return;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lVOIPNum, AllDeviceRes[s8ModID].lVOIPOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 2, TmpStr );

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalVOIP, g_iTotalVOIPOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 2, TmpStr );
}

void	DrawCount_Voc ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	if (pdlg == NULL)
		return;

	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;

	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lVocNum, AllDeviceRes[s8ModID].lVocOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 3, TmpStr );

	sprintf ( TmpStr, "%3d", AllDeviceRes[s8ModID].lVocFreeNum );
	pdlg->m_ListCount.SetItemText ( iDispRow, 4, TmpStr );

	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalVoice, g_iTotalVoiceOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 3, TmpStr );

	sprintf ( TmpStr, "%3d", g_iTotalVoiceFree );
	pdlg->m_ListCount.SetItemText ( iDispRow, 4, TmpStr );
}

// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	VOIP_STRUCT	*pOneVOIP;

	if (pdlg == NULL)
		return;

	// m_ListMain
	pdlg->m_ListMain.DeleteAllItems ();
	for ( i = 0; i < g_iTotalVOIP; i ++ )
	{
		iDispRow =  i; 	

		// 固定不变的内容
		sprintf ( TmpStr, "%3d", iDispRow );
		pdlg->m_ListMain.InsertItem ( iDispRow, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_VOIP[i].m_s8ModuleID, MapTable_VOIP[i].m_s16ChannelID );
		pdlg->m_ListMain.SetItemText ( iDispRow, 1, TmpStr );

		pOneVOIP = &M_OneVOIP(MapTable_VOIP[i]);
		pdlg->m_ListMain.SetItemText ( iDispRow, 2, GetString_DeviceMain( pOneVOIP->deviceID.m_s16DeviceMain) );

		// 可能改变的内容
		DrawMain_LineState ( pOneVOIP );
		DrawMain_State ( pOneVOIP );
		DrawMain_CallInfo ( pOneVOIP );
		DrawMain_DTMF ( pOneVOIP );
	}	

	// m_ListCount
	pdlg->m_ListCount.DeleteAllItems ();
	// 先添加行
	pdlg->m_ListCount.InsertItem ( 0, "Total" );
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListCount.InsertItem ( i+1, TmpStr );

		sprintf ( TmpStr, "%3d", MapTable_Module[i] );
		pdlg->m_ListCount.SetItemText ( i+1, 1, TmpStr );
	}

	// 再显示内容
	for ( i = 0; i < g_iTotalModule; i ++ )
	{		
		DrawCount_VOIP ( MapTable_Module[i] );
		DrawCount_Voc ( MapTable_Module[i] );
	}
}

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_VOIPTestDemoDlg	*)theApp.m_pMainWnd;

	// Read From "XMS_Demo.INI"
	ReadFromConfig();	

	// Init m_ListMain
	InitListMain();	

	// Init m_ListMsg
	InitListMsg();

	// Init m_ListCount
	InitListCount();

	// Init Conifg's text box
	InitTextBox();

	// now open ACS Server
	/*调用acsOpenStream 建立与ACS服务器的连接*/
	r = XMS_acsOpenStream ( &g_acsHandle, 
						&cfg_ServerID,
						g_u8UnitID,		// App Unit ID 
						64,				// sendQSize, in K Bytes
						64,				// recvQSize, in K Bytes
						0,
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
		WriteMsgLog(MsgStr);
	}

	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		AddMsg ( MsgStr );
		WriteMsgLog(MsgStr);
		return false;
	}
	else
	{
		sprintf ( MsgStr, "XMS_acsSetESR() OK!" );
		AddMsg ( MsgStr );
		WriteMsgLog(MsgStr);
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

	// save to "XMS_Demo.INI"
	FetchFromText();
	WriteToConfig();

	pdlg = NULL;

}

// --------------------------------------------------------------------------------
void	OpenVOIPDevice ( VOIP_STRUCT *pOneVOIP )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	if (pOneVOIP == NULL)
		return;

	pOneVOIP->State= VOIP_WAITOPEN;

	r = XMS_ctsOpenDevice ( g_acsHandle, &pOneVOIP->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenVOIPDevice()! ret=%d", r);
		AddMsg (TmpStr);
		WriteMsgLog(TmpStr);
	}
}

void	OpenVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	pOneVoice->State= VOC_WAITOPEN;

	r = XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenVoiceDevice()! ret=%d", r);
		AddMsg (TmpStr);
		WriteMsgLog(TmpStr);
	}
}


void	OpenBoardDevice ( DeviceID_t *pBoardDevID )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};

	r = XMS_ctsOpenDevice ( g_acsHandle, pBoardDevID, NULL );

	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenBoardDevice()! ret=%d", r);
		AddMsg (TmpStr);
		WriteMsgLog(TmpStr);
	}
}

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID )
{
	int			 i;

	// Open Board
	OpenBoardDevice ( &AllDeviceRes[s8DspModID].deviceID );

	// pVOIP
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVOIPNum; i++ )
	{
		OpenVOIPDevice ( &AllDeviceRes[s8DspModID].pVOIP[i] );
	}

	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}
}


// --------------------------------------------------------------------------------
void	CloseVOIPDevice ( VOIP_STRUCT *pOneVOIP )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};

	if (pOneVOIP == NULL)
		return;
		
	pOneVOIP->State= VOIP_WAITOPEN;

	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneVOIP->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseVOIPDevice()!", r);
		AddMsg (TmpStr);
		WriteMsgLog(TmpStr);
	}
}

void	CloseVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseVoiceDevice()! ret=%d", r);
		AddMsg ( TmpStr );
		WriteMsgLog(TmpStr);
	}
}


void	CloseBoardDevice ( DeviceID_t *pBoardDevID )
{
	RetCode_t	r;
	char        TmpStr[500];

	r = XMS_ctsCloseDevice ( g_acsHandle, pBoardDevID, NULL );

	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseBoardDevice()! ret=%d", r);
		AddMsg (TmpStr);
		WriteMsgLog(TmpStr);
	}
}

void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID )
{
	int			 i;

	// close Board
	CloseBoardDevice ( &AllDeviceRes[s8DspModID].deviceID );

	// pVOIP
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVOIPNum; i++ )
	{
		CloseVOIPDevice ( &AllDeviceRes[s8DspModID].pVOIP[i] );
	}

	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		CloseVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}
}

// -------------------------------------------------------------------------------------------------
void	HandleDevState ( Acs_Evt_t *pAcsEvt )
{
	VOIP_STRUCT	*pOneVOIP;
	Acs_GeneralProc_Data *pGeneralData = NULL;

	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		pOneVOIP = &M_OneVOIP(pAcsEvt->m_DeviceID);

		pOneVOIP->iLineState = pGeneralData->m_s32DeviceState;
		DrawMain_LineState( pOneVOIP );	
	}
}


/*停止录音*/
DJ_S32 StopRecordFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlRecord_t	controlRecord;

	controlRecord.m_u32ControlType = XMS_STOP_RECORD;

	r = XMS_ctsControlRecord ( g_acsHandle, pVocDevID, &controlRecord, NULL );

	return r;
}

/*录音处理*/
DJ_S32	 RecordFile ( VOIP_STRUCT *pOneVOIP, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend )
{	
	DJ_U32				i = 0;	
	RecordProperty_t	recordProperty;
	RetCode_t			r;

	memset(&recordProperty,0,sizeof(RecordProperty_t));		
	
	recordProperty.m_u32MaxSize = u32RecSize;
	recordProperty.m_s8IsMixEnable = 1;
	recordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_IP;
	recordProperty.m_MixerControl.m_u16SRC_ChID1 = pOneVOIP->deviceID.m_s16ChannelID;

	if ( bIsAppend )
		recordProperty.m_s8IsAppend = 1;
	else
		recordProperty.m_s8IsAppend = 0;

	strcpy ( recordProperty.m_s8FileName, s8FileName );

	r = XMS_ctsRecord ( g_acsHandle, &pOneVOIP->VocDevID, &recordProperty, NULL );

	return r;
}

/*停止放音*/
DJ_S32 StopPlayFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;

	controlPlay.m_u16ControlType = XMS_STOP_PLAY;

	r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );

	return r;
}

/*放音处理*/
DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue )
{	
	
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

bool    CheckRecordEnd(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{
	if (pAcsEvt->m_s32EventType == XMS_EVT_RECORD)
	{
		return true;
	}

	return false;
}

bool	CheckPlayEnd ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_PLAY )	
	{
		pMediaData = (Acs_MediaProc_Data *) FetchEventData(pAcsEvt);
		if ( pMediaData->m_u8TaskID == pOneVOIP->u8PlayTag )
			return true;
	}

	return false;
}

void My_InitDtmfBuf ( VOIP_STRUCT *pOneVOIP )
{
	pOneVOIP->DtmfCount = 0;
	pOneVOIP->DtmfBuf[0] = 0; 
}

void My_AddDtmfBuf ( VOIP_STRUCT *pOneVOIP, char TmpDtmf )
{
	if ( pOneVOIP->DtmfCount < (32-1) )
	{
		pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount] = TmpDtmf; 
		pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount+1] = 0; 
		pOneVOIP->DtmfCount ++;
	}
}

char My_GetDtmfCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data事件*/
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

int		SearchOneFreeVoice (  VOIP_STRUCT *pOneVOIP, DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8			s8ModID;
	int				i;
	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;

	s8ModID = pOneVOIP->deviceID.m_s8ModuleID;

	s8SearchModID = s8ModID;

	for ( i = 0; i < AllDeviceRes[s8SearchModID].lVocNum; i ++ )
	{
		pOneVoice = &AllDeviceRes[s8SearchModID].pVoice[i];
		if ( pOneVoice->State == VOC_FREE )
		{
			*pFreeVocDeviceID = pOneVoice->deviceID;

			// 占用该voice device 
			pOneVoice->State = VOC_USED;
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
		M_OneVoice(*pFreeVocDeviceID).State = VOC_FREE;

		AllDeviceRes[s8ModID].lVocFreeNum++;
		g_iTotalVoiceFree ++;
		DrawCount_Voc ( s8ModID );
		return	0;		// OK
	}

	return -1;			// invalid VocDeviceID
}

int    BindOneFreeVoice(VOIP_STRUCT *pOneVOIP, const DeviceID_t &FreeVocDevID)
{
	MixerControlParam_t paramMixer = {0};
	RetCode_t r = -1;

	if (pOneVOIP == NULL)
		return r;
	
	paramMixer.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
	paramMixer.m_u16SRC_ChID1 = FreeVocDevID.m_s16ChannelID;

	// send SetParam command
	r = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID,
		VOIP_PARAM_RTPMIXER, sizeof(paramMixer), &paramMixer);

	return r;
}

int    UnBindOneVoice(VOIP_STRUCT *pOneVOIP)
{
	MixerControlParam_t paramMixer = {0};
	RetCode_t r = -1;

	if (pOneVOIP == NULL)
		return r;

	r = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID,
		VOIP_PARAM_RTPMIXER, sizeof(paramMixer), &paramMixer);

	return r;
}

void	Change_State ( VOIP_STRUCT *pOneVOIP, VOIP_STATE NewState )
{
	pOneVOIP->State = NewState;

	DrawMain_State ( pOneVOIP );
}


void	InitVOIPChannel ( VOIP_STRUCT *pOneVOIP )
{
	Change_State ( pOneVOIP, VOIP_FREE );

	pOneVOIP->iCalleePort = 0;
	pOneVOIP->iCallerPort = 0;
	pOneVOIP->iProtocol = 0;

	memset(pOneVOIP->CallerAddr, 0, sizeof(pOneVOIP->CallerAddr));
	memset(pOneVOIP->CallerUserID, 0, sizeof(pOneVOIP->CallerUserID));
	
	memset(pOneVOIP->CalleeAddr, 0, sizeof(pOneVOIP->CalleeAddr));
	memset(pOneVOIP->CalleeUserID, 0, sizeof(pOneVOIP->CalleeUserID));

	memset(pOneVOIP->CallerNum, 0, sizeof(pOneVOIP->CallerNum));
	memset(pOneVOIP->CalleeNum, 0, sizeof(pOneVOIP->CalleeNum));


	My_InitDtmfBuf ( pOneVOIP );
	DrawMain_DTMF ( pOneVOIP );

	memset ( &pOneVOIP->VocDevID, 0, sizeof(DeviceID_t));		// -1表示还没有分配Voc Device
//	memset ( &pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
	DrawMain_VocInfo ( pOneVOIP );
}

void ResetVOIP ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt )
{
	if (pOneVOIP == NULL || pAcsEvt == NULL)
		return ;

	// free the used Voice Resource
	if ( pOneVOIP->VocDevID.m_s16DeviceMain > 0 )
	{
		StopPlayFile ( &pOneVOIP->VocDevID );
		StopRecordFile(&pOneVOIP->VocDevID);	

		FreeOneFreeVoice (  &pOneVOIP->VocDevID );

		memset ( &M_OneVoice(pOneVOIP->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		memset ( &pOneVOIP->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
		DrawMain_VocInfo ( pOneVOIP );
	}

	InitVOIPChannel ( pOneVOIP );
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	VOIP_STRUCT	*pOneVOIP;

	if (pDevice == NULL)
		return;

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		pOneVOIP = &M_OneVOIP(*pDevice);

		pOneVOIP->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
	
		// init this Device: VOIP
		InitVOIPChannel ( pOneVOIP );

		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );	

		// modify the count
		g_iTotalVOIPOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVOIPOpened ++;

		DrawCount_VOIP ( pDevice->m_s8ModuleID  );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		M_OneVoice(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important

		// init this Device: Voice
		M_OneVoice(*pDevice).State = VOC_FREE;

		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

		// modify the count
		g_iTotalVoiceOpened ++;
		g_iTotalVoiceFree ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;

		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}
}

void   CreateFileDirector(DJ_S8 s8DspModID)
{
	char pchFileName[256];

	sprintf(pchFileName, "%s\\Rec%d", cfg_VocPath, s8DspModID);
	CreateDirectory(pchFileName,NULL);
}

bool GetFilePathName(VOIP_STRUCT *pOneVOIP, char *pchFilePath)
{
	if (pchFilePath == NULL || pOneVOIP == NULL)
		return false;;

	if (pOneVOIP->deviceID.m_s8ModuleID > 0)
	{
		sprintf(pchFilePath, "%s\\Rec%d", cfg_VocPath, pOneVOIP->deviceID.m_s8ModuleID);		
	}

	return true;	
}


void VOIPWork ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data   *pCallControl = NULL;
	VoIPCallPrivate_t      *voipCallPrivate = NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char                    chTmpFilePath[256] = "";

	if (pOneVOIP == NULL || pAcsEvt == NULL)
		return;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		if (cfg_iVoiceCheck == 1)
		{
			StopPlayFile(&pOneVOIP->VocDevID);
			
			//删除录音文件
			GetFilePathName(pOneVOIP, chTmpFilePath);
			sprintf(FileName, "%s\\DemoRec.%0003d", chTmpFilePath, pOneVOIP->iSeqID);
			DeleteFile(FileName);
		}

		ResetVOIP ( pOneVOIP, pAcsEvt );
		Change_State(pOneVOIP, VOIP_FREE);

		return ; 
	}

	switch(pOneVOIP->State)
	{
	case VOIP_FREE:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	/*呼入事件*/
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			ASSERT(pCallControl->m_PrivData.m_u32DataSize == sizeof(VoIPCallPrivate_t));
			voipCallPrivate = (VoIPCallPrivate_t *)&(pCallControl->m_PrivData);			

			XMS_ctsAlertCall ( g_acsHandle, &pOneVOIP->deviceID, NULL );

			XMS_ctsAnswerCallIn ( g_acsHandle, &pOneVOIP->deviceID, NULL );			
			
			pOneVOIP->iProtocol = voipCallPrivate->m_s32Protocol;

			//Caller Info
			strncpy(pOneVOIP->CallerAddr, voipCallPrivate->m_s8CallerAddress, sizeof(pOneVOIP->CallerAddr));			
			strncpy(pOneVOIP->CallerUserID, voipCallPrivate->m_s8CallerUserID, sizeof(pOneVOIP->CallerUserID));
			strncpy ( pOneVOIP->CallerNum, pCallControl->m_s8CallingNum, sizeof(pOneVOIP->CallerNum));
			pOneVOIP->iCallerPort = (unsigned short)voipCallPrivate->m_s32CallerPort;
			//

			//Callee Info
			strncpy(pOneVOIP->CalleeAddr, voipCallPrivate->m_s8CalleeAddress, sizeof(pOneVOIP->CalleeAddr));			
			strncpy(pOneVOIP->CalleeUserID, voipCallPrivate->m_s8CalleeUserID, sizeof(pOneVOIP->CalleeUserID));
			strncpy ( pOneVOIP->CalleeNum, pCallControl->m_s8CalledNum, sizeof(pOneVOIP->CalleeNum));			
			pOneVOIP->iCalleePort = (unsigned short)voipCallPrivate->m_s32CalleePort;
			//

			DrawMain_CallInfo( pOneVOIP );
			
			Change_State ( pOneVOIP, VOIP_WAIT_ANSWERCALL );
		}
		break;

	case VOIP_WAIT_ANSWERCALL:		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	/*应答呼入完成事件*/
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if (cfg_iVoiceCheck == 1)
			{
				if ( SearchOneFreeVoice ( pOneVOIP,  &FreeVocDeviceID ) >= 0 )
				{
					pOneVOIP->u8PlayTag = 16;
					
					pOneVOIP->VocDevID = FreeVocDeviceID;
					
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneVOIP->deviceID; 
					
					DrawMain_VocInfo ( pOneVOIP );
					
					GetFilePathName(pOneVOIP, chTmpFilePath);
					sprintf(FileName, "%s\\DemoRec.%0003d", chTmpFilePath, pOneVOIP->iSeqID);		
					RecordFile ( pOneVOIP, FileName, 8000L*10, false);

					Change_State ( pOneVOIP, VOIP_RECORDFILE );					
				
				}
			}
			else
			{
				Change_State ( pOneVOIP, VOIP_WAITHANGUP );
			}
		}
		break;

	case VOIP_RECORDFILE:
		if ( CheckRecordEnd ( pOneVOIP, pAcsEvt) )	/*record完成事件*/
		{
			BindOneFreeVoice(pOneVOIP, pOneVOIP->VocDevID);
			Change_State ( pOneVOIP, VOIP_WAITBINDOK);
		}
		break;

	case VOIP_WAITBINDOK:
		if (pAcsEvt->m_s32EventType == XMS_EVT_SETPARAM)
		{
			GetFilePathName(pOneVOIP, chTmpFilePath);
			sprintf(FileName, "%s\\DemoRec.%0003d", chTmpFilePath, pOneVOIP->iSeqID);	
			pOneVOIP->u8PlayTag ++;
			PlayFile ( &pOneVOIP->VocDevID, FileName, pOneVOIP->u8PlayTag, true );

			Change_State (pOneVOIP, VOIP_PLAYRECORD);
		}	
		break;

	case VOIP_PLAYRECORD:
		if ( CheckPlayEnd ( pOneVOIP, pAcsEvt) )	/*play完成事件*/
		{
			// 主动挂机
			UnBindOneVoice(pOneVOIP);

			XMS_ctsClearCall ( g_acsHandle, &pOneVOIP->deviceID, 0, NULL );			

			Change_State ( pOneVOIP, VOIP_WAITHANGUP );
		}
		break;
	}
}


