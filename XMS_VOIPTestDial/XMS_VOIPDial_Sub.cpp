#include "stdafx.h"
#include "XMS_VOIPTestDial.h"
#include "XMS_VOIPTestDialDlg.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_VOIPDial_Sub.H"
#include "XMS_VOIPDial_String.H"
#include "XMS_VOIPDial_Event.H"

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_VOIPTestDialApp theApp;
CXMS_VOIPTestDialDlg	*pdlg;

// var about XMS_Dial.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Dial.INI";

ServerID_t		cfg_ServerID;

int             cfg_Protocol = 0;

char            cfg_CallerAddr[20] = {0};
char            cfg_CalleeAddr[20] = {0};

int             cfg_CalleePort;
int             cfg_CallerPort;

char            cfg_CalleeUserID[20] = {0};
char            cfg_CallerUserID[20] = {0};

char            cfg_CalleeTelNo[20] = {0};
char            cfg_CallerTelNo[20] = {0};

char			cfg_VocPath[128] = {0};	

int				cfg_iPartWorkModuleID[256]={0};
char            cfg_chPartWorkModuleID[256] = {0};

//char			cfg_SimCalledNum[32] = "12345";

int             cfg_iVoiceCheck = 0;

float            cfg_iTestTime = 1.0;

long             g_iTestStartTime = 0;


LONGLONG    g_iTotalCnt = 0; 
LONGLONG    g_iTotalCallErr = 0;
LONGLONG    g_iTotalCheckErr = 0;
LONGLONG    g_iEstablishSum = 0;
LONGLONG    g_iCallSuc = 0;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 3;

// var in XMS_Dail_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int                      g_iTotalVOIP;
extern int                      g_iTotalVOIPOpened;
extern TYPE_CHANNEL_MAP_TABLE   MapTable_VOIP[MAX_VOIP_NUM_IN_THIS_DEMO];


extern int						g_iTotalVoice;
extern int						g_iTotalVoiceOpened;
extern int						g_iTotalVoiceFree;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_VOIP_NUM_IN_THIS_DEMO];

extern int						g_iTotalModule;
extern DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

CRITICAL_SECTION CriticalSection;

// -----------------------------------------------------------------------
// local function decalration
void	WriteToConfig(void);
void	ReadFromConfig(void);
void	InitTextBox(void);
void	FetchFromText();

void	InitListMain(void);
void	InitListMsg(void);
bool    GetFilePathName(VOIP_STRUCT *pOneVOIP, char *pchFilePath);

void DrawOneVOIPResult(VOIP_STRUCT *pOneVOIP);

char dtmf_table[16] = { 'D', '1','2','3',
                        '4','5','6','7',
						'8','9','0','*',
						'#', 'A','B','C' };
// -----------------------------------------------------------------------

 bool IsCorrectDtmf(VOIP_STRUCT *pOneVOIP)
 {
	 int i;

	 if (pOneVOIP == NULL)
		 return false;

	 if (pOneVOIP->DtmfCount != 10)
	 {
		 return false;
	 }

	 for (i=0; i<10; i++)
	 {
		 if (pOneVOIP->DtmfBuf[i] != dtmf_table[(pOneVOIP->iSeqID%15)])
			 return false;
	 }

	 return true;
}

// -----------------------------------------------------------------------
void   WriteMsgLog(char *pMsg)
{
	FILE   *fp;
	char   tmpTime[128], tmpDate[128];

	char   LogFileName[256] = {0}; 
	char   LogFileShortName[] = "\\SysMsgLog.txt";
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
 void WriteTestLog ( VOIP_STRUCT *pOneVOIP, char *s )
{
	FILE *fp;
	char   tmpTime[128], tmpDate[128];
	char   LogFileName[256] = {0}; 
	char   LogFileShortName[] = "\\TestLog.txt";
	

	GetCurrentDirectory(MAX_FILE_NAME_LEN-32, LogFileName);
	strcat(LogFileName, LogFileShortName);

	fp = fopen (LogFileName, "a+t" );
	if ( fp == NULL ) 
	{
		printf ( " open TestLog.txt fail\n" );
		return;
	}

	_strtime(tmpTime);
	_strdate(tmpDate);
	if (strcmp(s, "Test Start") == 0 ) 
	{
		if (cfg_iVoiceCheck == 1)
			fprintf(fp, "  [%s %s] ------VoiceCheck %s------\n", tmpDate, tmpTime, s);
		else 
			fprintf(fp, "  [%s %s] ----No VoiceCheck %s----\n", tmpDate, tmpTime, s);
	}
	else if (pOneVOIP != NULL)
	{	
	
		fprintf ( fp, " [%s %s], DeviceID(%d, %d) %s \n", tmpDate, tmpTime,
			pOneVOIP->deviceID.m_s8ModuleID,  pOneVOIP->deviceID.m_s16ChannelID, s );		
		
	}

	if (fp != NULL)
		fclose (fp);
}

void WriteTestResult()
{
	FILE *fp;
	char   tmpTime[128], tmpDate[128];
	char   LogFileName[256] = {0}; 
	char   LogFileShortName[] = "\\TestLog.txt";
	char   TmpStr[500] = {0};

	GetCurrentDirectory(MAX_FILE_NAME_LEN-32, LogFileName);
	strcat(LogFileName, LogFileShortName);

	fp = fopen (LogFileName, "a+t" );
	if ( fp == NULL ) 
	{
		printf ( "open TestLog.txt fail\n" );
		return;
	}

	_strtime(tmpTime);
	_strdate(tmpDate);

	fprintf(fp, "\n**[%s %s]***************Test End**********\n", tmpDate, tmpTime);
	pdlg->GetDlgItem(IDC_STATIC_Count)->GetWindowText(TmpStr, sizeof(TmpStr));
	fprintf(fp, "**呼叫次数：%s                            \n", TmpStr);
	pdlg->GetDlgItem(IDC_STATIC_Fail)->GetWindowText(TmpStr, sizeof(TmpStr));
	fprintf(fp, "**呼损：%s                                \n", TmpStr);
	pdlg->GetDlgItem(IDC_STATIC_DtmfErr)->GetWindowText(TmpStr, sizeof(TmpStr));
	fprintf(fp, "**误码次数: %s                            \n", TmpStr);
	pdlg->GetDlgItem(IDC_STATIC_SucRate)->GetWindowText(TmpStr, sizeof(TmpStr));
	fprintf(fp, "**失败率(呼损\\误码): %s                  \n", TmpStr);
	pdlg->GetDlgItem(IDC_STATIC_TestTimer)->GetWindowText(TmpStr, sizeof(TmpStr));
	fprintf(fp, "**测试时间: %s                            \n", TmpStr);
	fprintf(fp, "******************************************************\n");

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
	char TmpStr[200] = {0};

	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );

	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);

	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);

	GetPrivateProfileString("ConfigInfo","UserName","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);
 
	cfg_Protocol = GetPrivateProfileInt ( "ConfigInfo", "Protocol", 0, cfg_IniName);

	// Callee Info
	GetPrivateProfileString ( "ConfigInfo", "CalleeAddress", "192.168.0.1", cfg_CalleeAddr, sizeof(cfg_CalleeAddr), cfg_IniName);

	cfg_CalleePort = GetPrivateProfileInt ( "ConfigInfo", "CalleePort", 1720, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "CalleeUserID", "DongJin_One", cfg_CalleeUserID, sizeof(cfg_CalleeUserID), cfg_IniName);
	
    GetPrivateProfileString ( "ConfigInfo", "CalleeTelNo", "12345", cfg_CalleeTelNo, sizeof(cfg_CalleeTelNo), cfg_IniName);
    //

	// Caller Info
	GetPrivateProfileString ( "ConfigInfo", "CallerAddress", "192.168.0.2", cfg_CallerAddr, sizeof(cfg_CallerAddr), cfg_IniName);

	cfg_CallerPort = GetPrivateProfileInt ( "ConfigInfo", "CallerPort", 1721, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "CallerUserID", "DongJin_TWO", cfg_CallerUserID, sizeof(cfg_CallerUserID), cfg_IniName);
	
    GetPrivateProfileString ( "ConfigInfo", "CallerTelNo", "45678", cfg_CallerTelNo, sizeof(cfg_CallerTelNo), cfg_IniName);

	//
	GetPrivateProfileString ( "ConfigInfo", "VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);
	
	GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(TmpStr, cfg_chPartWorkModuleID, sizeof(TmpStr));	
	SplitStr2Int(TmpStr, ",", cfg_iPartWorkModuleID);

	cfg_iVoiceCheck = GetPrivateProfileInt ("ConfigInfo", "VoiceCheck", 0, cfg_IniName);
}

void	WriteToConfig(void)
{
	char	TmpStr[256];

	WritePrivateProfileString ( "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	WritePrivateProfileString ( "ConfigInfo", "Port", TmpStr, cfg_IniName);

	WritePrivateProfileString("ConfigInfo","UserName", cfg_ServerID.m_s8UserName,cfg_IniName);
	WritePrivateProfileString("ConfigInfo","PassWord", cfg_ServerID.m_s8UserPwd,cfg_IniName);
	
	sprintf ( TmpStr, "%d", cfg_Protocol);
	WritePrivateProfileString ( "ConfigInfo", "Protocol", TmpStr, cfg_IniName);

	// Callee Info
	WritePrivateProfileString ( "ConfigInfo", "CalleeAddress",  cfg_CalleeAddr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_CalleePort);
	WritePrivateProfileString ( "ConfigInfo", "CalleePort", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CalleeUserID", cfg_CalleeUserID, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CalleeTelNo", cfg_CalleeTelNo, cfg_IniName);
	//

	// Callee Info
	WritePrivateProfileString ( "ConfigInfo", "CallerAddress",  cfg_CallerAddr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_CallerPort);
	WritePrivateProfileString ( "ConfigInfo", "CallerPort", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CallerUserID", cfg_CallerUserID, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CallerTelNo", cfg_CallerTelNo, cfg_IniName);
	//

	WritePrivateProfileString ( "ConfigInfo", "VocPath", cfg_VocPath, cfg_IniName);

	sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
	WritePrivateProfileString("ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);

	sprintf(TmpStr, "%d", cfg_iVoiceCheck);
	WritePrivateProfileString("ConfigInfo", "VoiceCheck", TmpStr, cfg_IniName);
}

void	InitTextBox(void)
{
	char	TmpStr[256];

	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->SetWindowText ( cfg_ServerID.m_s8ServerIp );

	sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort );
	pdlg->GetDlgItem(IDC_EDIT_IPPORT)->SetWindowText ( TmpStr );	

	

	if (cfg_Protocol == 0)
	{	
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_H323))->SetCheck(1);		
		
	}
	else
	{	
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_SIP))->SetCheck(1);
	}

	// Callee Info
	pdlg->GetDlgItem(IDC_EDIT_CALLEE_ADDR)->SetWindowText(cfg_CalleeAddr);

	sprintf(TmpStr, "%d", cfg_CalleePort);
	pdlg->GetDlgItem(IDC_EDIT_CALLEE_PORT)->SetWindowText(TmpStr);

	pdlg->GetDlgItem(IDC_EDIT_CALLEE_USERID)->SetWindowText(cfg_CalleeUserID);

	pdlg->GetDlgItem(IDC_EDIT_CALLEE_TELNO)->SetWindowText(cfg_CalleeTelNo);
	//

	// Caller Info
	pdlg->GetDlgItem(IDC_EDIT_CALLER_ADDR)->SetWindowText(cfg_CallerAddr);

	sprintf(TmpStr, "%d", cfg_CallerPort);
	pdlg->GetDlgItem(IDC_EDIT_CALLER_PORT)->SetWindowText(TmpStr);

	pdlg->GetDlgItem(IDC_EDIT_CALLER_USERID)->SetWindowText(cfg_CallerUserID);

	pdlg->GetDlgItem(IDC_EDIT_CALLER_TELNO)->SetWindowText(cfg_CallerTelNo);
	//

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

	// Callee Info
	pdlg->GetDlgItem(IDC_EDIT_CALLEE_ADDR)->GetWindowText(cfg_CalleeAddr, 30);

	pdlg->GetDlgItem ( IDC_EDIT_CALLEE_PORT )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_CalleePort);

	pdlg->GetDlgItem ( IDC_EDIT_CALLEE_USERID )->GetWindowText ( cfg_CalleeUserID, 30 );

    pdlg->GetDlgItem ( IDC_EDIT_CALLEE_TELNO )->GetWindowText ( cfg_CalleeTelNo, 30 );
	//

	// Caller Info
	pdlg->GetDlgItem(IDC_EDIT_CALLER_ADDR)->GetWindowText(cfg_CallerAddr, 30);

	pdlg->GetDlgItem ( IDC_EDIT_CALLER_PORT )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_CallerPort);

	pdlg->GetDlgItem ( IDC_EDIT_CALLER_USERID )->GetWindowText ( cfg_CallerUserID, 30 );

    pdlg->GetDlgItem ( IDC_EDIT_CALLER_TELNO )->GetWindowText ( cfg_CallerTelNo, 30 );
	//

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->GetWindowText ( cfg_VocPath, 128 );	
	

	if (((CButton *)pdlg->GetDlgItem(IDC_RADIO_Voice))->GetCheck() == 1)
		cfg_iVoiceCheck = 1;
	else if (((CButton *)pdlg->GetDlgItem(IDC_RADIO_NoVoice))->GetCheck() == 1)
		cfg_iVoiceCheck = 0;

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
	int i;
	char		TmpStr[256];

	if (iTotal_ListMsg >= 5000)
	{
		for (i=0; i<iTotal_ListMsg; i++)
		{
			pdlg->m_ListMsg.DeleteString(i);
		}

		iTotal_ListMsg = 0;
	}
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
	lvc.cx = 40;
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
	lvc.pszText = "收DTMF码" ;
	lvc.cx = 80;
	pdlg->m_ListMain.InsertColumn ( 5, &lvc ); 

	lvc.iSubItem = 6;
	lvc.pszText = "Total/CallFail/CheckFail";
	lvc.cx = 100;
	pdlg->m_ListMain.InsertColumn(6, &lvc);

	lvc.iSubItem = 7;
	lvc.pszText = "FailReason" ;
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "VoiceDev" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 8, &lvc ); 

	lvc.iSubItem = 9;
	lvc.pszText = "Establish";
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn(9, &lvc);
}

void DrawMain_LineState( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL)
		return;

	
	iDispRow = pOneVOIP->iSeqID;
	if ( iDispRow < 0 )	return;

	GetString_LineState ( StateStr, pOneVOIP->iLineState );
	pdlg->m_ListMain.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawMain_State( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID;
	if ( iDispRow < 0 )	return;
	//
	switch( pOneVOIP->State ) 
	{
	case VOIP_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case VOIP_FREE:		
		strcpy(StateStr,"空闲"); 
		break ;

	case VOIP_CALLOUT:
		strcpy(StateStr,"呼出");
		break;

	case VOIP_WAIT_SETOK:
		strcpy(StateStr, "绑定语音资源");
		break;

	case VOIP_CALLOUT1:
		strcpy(StateStr, "呼出");
		break;

	case VOIP_PLAY:
		strcpy(StateStr, "放音");
		break;

	case VOIP_GETDTMF:
		strcpy(StateStr, "收DTMF码");
		break;
	
	case VOIP_FAIL:
		strcpy(StateStr,"呼出失败");
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

void DrawMain_VocInfo( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;

	if (pOneVOIP == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 
	if ( iDispRow < 0 )	return;

	if ( pOneVOIP->VocDevID.m_s16DeviceMain != 0 )
	{
		pDev = &pOneVOIP->VocDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}
	else
		strcpy(StateStr, "");

	pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 
}

void DrawMain_EstableTime(VOIP_STRUCT *pOneVOIP)
{
	char StateStr[100];
	int  iDispRow;
	
	if (pOneVOIP == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID;
	if (iDispRow < 0) 
		return;

	sprintf(StateStr, "%d", pOneVOIP->iEstablishTime);
	pdlg->m_ListMain.SetItemText(iDispRow, 9, StateStr);
}

void DrawMain_Reason ( VOIP_STRUCT *pOneVOIP, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID; 
	if ( iDispRow < 0 )	return;

	sprintf ( StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause ); 
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 
	WriteTestLog(pOneVOIP, StateStr);
}

void Refresh_DtmfInfo( VOIP_STRUCT *pOneVOIP )
{
	int   iDispRow;

	if (pOneVOIP == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID;

	pdlg->m_ListMain.SetItemText(iDispRow, 5, "");
}

void DrawVOIP_DTMF( VOIP_STRUCT *pOneVOIP )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneVOIP == NULL)
		return;

	iDispRow = pOneVOIP->iSeqID;

	strcpy(StateStr,pOneVOIP->DtmfBuf);
	pdlg->m_ListMain.SetItemText ( iDispRow, 5, StateStr ); 

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
	VOIP_STRUCT	*pOneVOIP;
	int  i;

	// m_ListMain
	pdlg->m_ListMain.DeleteAllItems ();
	for ( i = 0; i < g_iTotalVOIP; i ++ )
	{
		// 固定不变的内容
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListMain.InsertItem ( i, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_VOIP[i].m_s8ModuleID, MapTable_VOIP[i].m_s16ChannelID );
		pdlg->m_ListMain.SetItemText ( i, 1, TmpStr );

		pOneVOIP = &M_OneVOIP(MapTable_VOIP[i]);
		pdlg->m_ListMain.SetItemText ( i, 2, GetString_DeviceMain( pOneVOIP->deviceID.m_s16DeviceMain) );

		// 可能改变的内容
		DrawMain_LineState ( pOneVOIP );
		DrawMain_State ( pOneVOIP );	
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
		DrawCount_VOIP( MapTable_Module[i] );
		DrawCount_Voc ( MapTable_Module[i] );
	}
}

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_VOIPTestDialDlg *)theApp.m_pMainWnd;

	srand((unsigned)time(NULL));

	// Read From "XMS_Dial.INI"
	ReadFromConfig();

	// Init m_ListMain
	InitListMain();

	// Init m_ListMsg
	InitListMsg();

	// Init m_ListCount
	InitListCount();

	// Init Conifg's text box
	InitTextBox();

	InitializeCriticalSection(&CriticalSection); 

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

	// save to "XMS_Dial.INI"
	FetchFromText();
	WriteToConfig();

	pdlg = NULL;

	DeleteCriticalSection(&CriticalSection);
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
	char        TmpStr[500]={0};
	
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
	char        TmpStr[500]={0};

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

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	VOIP_STRUCT	    *pOneVOIP;
	VOICE_STRUCT	*pOneVoice;	

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		pOneVOIP = &M_OneVOIP(*pDevice);
		
		pOneVOIP->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation		
		
		InitVOIPChannel(pOneVOIP);
		InitVOIPcounter(pOneVOIP);
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );	

		// modify the count
		g_iTotalVOIPOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVOIPOpened++;

		DrawCount_VOIP ( pDevice->m_s8ModuleID  );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		pOneVoice = &M_OneVoice(*pDevice);

		pOneVoice->deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		pOneVoice->State = VOC_FREE;

		g_iTotalVoiceOpened ++;
		g_iTotalVoiceFree ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;

		DrawCount_Voc ( pDevice->m_s8ModuleID );
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
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseVOIPDevice()! ret=%d", r);
		AddMsg ( TmpStr );
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
	char TmpStr[500] = "";

	r = XMS_ctsCloseDevice ( g_acsHandle, pBoardDevID, NULL );

	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseBoardDevice()! ret = %d", r);
		AddMsg ( TmpStr );
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

void	Change_State ( VOIP_STRUCT *pOneVOIP, VOIP_STATE NewState )
{	
	if (pOneVOIP == NULL)
		return;

	pOneVOIP->State = NewState;

	DrawMain_State ( pOneVOIP );
}

void My_InitDtmfBuf ( VOIP_STRUCT *pOneVOIP )
{
	if (pOneVOIP == NULL)
		return;

	pOneVOIP->DtmfCount = 0;
	pOneVOIP->DtmfBuf[0] = 0; 
}

void My_AddDtmfBuf ( VOIP_STRUCT *pOneVOIP, char TmpDtmf )
{
	if (pOneVOIP == NULL)
		return;

	if ( pOneVOIP->DtmfCount < (32-1) )
	{
		pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount] = TmpDtmf; 
		pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount+1] = 0; 
		pOneVOIP->DtmfCount ++;
	}
}

int		SearchOneFreeVoice (  VOIP_STRUCT *pOneVOIP, DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8			s8ModID;

	int				i;
	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;

	if (pOneVOIP == NULL)
		return -1;

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
		VOIP_PARAM_MIXER, sizeof(paramMixer), &paramMixer);

	return r;
}

int    UnBindOneVoice(VOIP_STRUCT *pOneVOIP)
{
	MixerControlParam_t paramMixer = {0};
	RetCode_t r = -1;

	if (pOneVOIP == NULL)
		return r;

	r = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID,
		VOIP_PARAM_MIXER, sizeof(paramMixer), &paramMixer);

	return r;
}

void    InitVOIPcounter(VOIP_STRUCT *pOneVOIP)
{
	if (pOneVOIP == NULL)
		return;

	pOneVOIP->iCallErrCnt = 0;
	pOneVOIP->iCheckErrCnt = 0;
	pOneVOIP->iSucCnt = 0;
	pOneVOIP->iTotalCnt = 0;
}

void	InitVOIPChannel ( VOIP_STRUCT *pOneVOIP )
{
	if (pOneVOIP == NULL)
		return;

	Change_State ( pOneVOIP, VOIP_FREE );

	memset ( &pOneVOIP->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配Voc Device
	memset ( &pOneVOIP->VocDevIDForCheck, 0, sizeof(DeviceID_t));
	My_InitDtmfBuf ( pOneVOIP );

	pOneVOIP->iEstablishTime = 0;
	pOneVOIP->iCallOutTime = 0;

	DrawMain_VocInfo ( pOneVOIP );
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

bool    CheckRecordEnd(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{
	if (pOneVOIP == NULL)
		return false;

	if (pAcsEvt->m_s32EventType == XMS_EVT_RECORD)
	{
		return true;
	}

	return false;
}

/*放音处理*/
DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue )
{	
	DJ_U32           i = 0;	
	PlayProperty_t   playProperty;
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

void ResetVOIP ( VOIP_STRUCT *pOneVOIP )
{
	if (pOneVOIP == NULL)
		return;

	// free the used Voice Resource
	if ( pOneVOIP->VocDevID.m_s16DeviceMain != 0 )
	{
		StopRecordFile(&pOneVOIP->VocDevID);
		StopPlayFile(&pOneVOIP->VocDevID);
		XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->deviceID, &pOneVOIP->VocDevID,NULL);		

		FreeOneFreeVoice (  &pOneVOIP->VocDevID );

		memset ( &M_OneVoice(pOneVOIP->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		memset ( &pOneVOIP->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
		DrawMain_VocInfo ( pOneVOIP );
	}

	InitVOIPChannel ( pOneVOIP );
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

char   My_GetDtmfCode(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data事件*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		TRACE("RECVIODATA\n");

		if ( ( pIOData->m_u16IoType == XMS_IO_TYPE_DTMF ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);

			return *p;
		}
	}

	return -1;	// not a good DTMF
}

void VOIPWork ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt )
{
	int          iFreeVoc;
	DeviceID_t   FreeVocDeviceID;
	Acs_CallControl_Data *	pCallControl = NULL;
	char                FileName[256] = {0};
	char				TmpDtmf;
	char                TmpFileName[256] = "";
	char                chError[256] = "";
	RetCode_t           ret = 0;

	if (pOneVOIP == NULL)
		return;

	DrawOneVOIPResult(pOneVOIP);

//	if (pOneVOIP->iLineState == DCS_FREE)
	{
	}

	if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )	/*定时器事件*/
	{	
	//	Change_State(pOneVOIP, VOIP_WAITHANGUP);
		XMS_ctsClearCall ( g_acsHandle, &pOneVOIP->deviceID, 0 ,NULL );
		//
	}

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		if (pOneVOIP->State == VOIP_WAITHANGUP)
		{
			if (cfg_iVoiceCheck == 1)
			{
				StopRecordFile(&pOneVOIP->VocDevID);
				
				GetFilePathName(pOneVOIP, TmpFileName);
				sprintf(FileName, "%s\\DemoRec.%0003d", TmpFileName, pOneVOIP->iSeqID);			
			
				pOneVOIP->iSucCnt++;
				DeleteFile(FileName);//删除录音文件						
			}					
		}
		
		ResetVOIP ( pOneVOIP );	
		Refresh_DtmfInfo(pOneVOIP);
		Change_State (pOneVOIP, VOIP_FREE);
		DrawMain_EstableTime(pOneVOIP);	
			
		return ; 
	}

	switch(pOneVOIP->State)
	{
	case VOIP_FREE:		
		break;

	case VOIP_CALLOUT: // 带语音检测
		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneVOIP->iEstablishTime = GetTickCount() - pOneVOIP->iCallOutTime;
				g_iEstablishSum += pOneVOIP->iEstablishTime;
				DrawMain_EstableTime(pOneVOIP);
				g_iCallSuc++;
				
				iFreeVoc = SearchOneFreeVoice(pOneVOIP, &FreeVocDeviceID);
				if (iFreeVoc >= 0)
				{					
					pOneVOIP->u8PlayTag = 16;					
					pOneVOIP->VocDevID = FreeVocDeviceID;
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneVOIP->deviceID; 
					DrawMain_VocInfo ( pOneVOIP );
					
					BindOneFreeVoice(pOneVOIP, FreeVocDeviceID);					
				
					Change_State (pOneVOIP, VOIP_WAIT_SETOK);
				}
				else
				{
					Change_State(pOneVOIP, VOIP_WAITHANGUP);
					XMS_ctsClearCall ( g_acsHandle, &pOneVOIP->deviceID, 0, NULL );
				}	
		
			}
			else		// 呼出失败
			{
				DrawMain_Reason ( pOneVOIP, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneVOIP, VOIP_FREE );
				g_iTotalCallErr++;
				pOneVOIP->iCallErrCnt++;
			}	
		}
		break;

	case VOIP_CALLOUT1: //纯信令继续		
		if (pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT)
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneVOIP->iEstablishTime = GetTickCount() - pOneVOIP->iCallOutTime;
				g_iEstablishSum += pOneVOIP->iEstablishTime;
				g_iCallSuc++;				
				
				DrawMain_EstableTime(pOneVOIP);				

				pOneVOIP->iSucCnt++;	
			//	XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
			//	Change_State (pOneVOIP, VOIP_WAITHANGUP);

				iFreeVoc = SearchOneFreeVoice(pOneVOIP, &FreeVocDeviceID);
				if (iFreeVoc >= 0)
				{					
					pOneVOIP->u8PlayTag = 16;					
					pOneVOIP->VocDevID = FreeVocDeviceID;
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneVOIP->deviceID; 
					DrawMain_VocInfo ( pOneVOIP );
					
					BindOneFreeVoice(pOneVOIP, FreeVocDeviceID);					
				
					Change_State (pOneVOIP, VOIP_WAIT_SETOK);
				}
				else
				{
					Change_State(pOneVOIP, VOIP_WAITHANGUP);
					XMS_ctsClearCall ( g_acsHandle, &pOneVOIP->deviceID, 0, NULL );
				}	

				//停止定时器函数，设置一个新定时器
				XMS_ctsSetDevTimer ( g_acsHandle, &pOneVOIP->deviceID, 60000 + 1000 *(rand()%10 +1));		
			}
			else		// 呼出失败
			{
				pOneVOIP->iCallErrCnt++;
				g_iTotalCallErr++;
				DrawMain_Reason ( pOneVOIP, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneVOIP, VOIP_FREE );
			}
		}
		break;

	case VOIP_WAIT_SETOK:
		if (pAcsEvt->m_s32EventType == XMS_EVT_SETPARAM)
		{	
			if (cfg_iVoiceCheck == 0)
			{
				sprintf(FileName, "C:\\DJKeygoe\\Samples\\Voc\\ConfMusi.Pcm");
			}
			else
			{
				sprintf(FileName, "%s\\sig%d.pcm", cfg_VocPath, (pOneVOIP->iSeqID)%15);
				
			}

			PlayFile (&pOneVOIP->VocDevID, FileName, pOneVOIP->u8PlayTag, true);
			pOneVOIP->u8PlayTag++;
			Change_State(pOneVOIP, VOIP_PLAY);	
		}
		break;

	case VOIP_PLAY:
		if (pOneVOIP->iLineState == DCS_FREE)
		{
			ResetVOIP(pOneVOIP);
		//	Change_State (pOneVOIP, VOIP_FREE);
			break;
		}

		if (CheckPlayEnd(pOneVOIP, pAcsEvt))
		{
			UnBindOneVoice(pOneVOIP);

	    	pOneVOIP->u8PlayTag++;	

			if (cfg_iVoiceCheck == 0)
			{
				Change_State (pOneVOIP, VOIP_WAITHANGUP);
			}
			else
			{
				GetFilePathName(pOneVOIP, TmpFileName);
				sprintf(FileName, "%s\\DemoRec.%0003d", TmpFileName, pOneVOIP->iSeqID);	
				RecordFile ( pOneVOIP, FileName, 8000L*15, false);
				Change_State (pOneVOIP, VOIP_GETDTMF);
			}
		}
		break;

	case VOIP_GETDTMF:
		TmpDtmf = My_GetDtmfCode ( pOneVOIP, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneVOIP, TmpDtmf );
			DrawVOIP_DTMF ( pOneVOIP );
		}

		if (CheckRecordEnd(pOneVOIP, pAcsEvt))
		{
			// free the used Voice Resource
			if ( pOneVOIP->VocDevIDForCheck.m_s16DeviceMain != 0 )
			{			
				XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneVOIP->VocDevIDForCheck,NULL);
				XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevIDForCheck, &pOneVOIP->VocDevID, NULL);
				
				FreeOneFreeVoice (  &pOneVOIP->VocDevIDForCheck );
				
				memset ( &M_OneVoice(pOneVOIP->VocDevIDForCheck).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
				memset ( &pOneVOIP->VocDevIDForCheck, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
				
			}
			XMS_ctsClearCall ( g_acsHandle, &pOneVOIP->deviceID, 0, NULL );
			Change_State (pOneVOIP, VOIP_WAITHANGUP);
		}
		break;
	}
}

void DrawOneVOIPResult(VOIP_STRUCT *pOneVOIP)
{
	char   TmpStr[500] = {0};

	if (pOneVOIP == NULL)
		return;	
	
	sprintf(TmpStr, "%d/%d/%d", pOneVOIP->iTotalCnt, pOneVOIP->iCallErrCnt, pOneVOIP->iCheckErrCnt);
	pdlg->m_ListMain.SetItemText(pOneVOIP->iSeqID, 6, TmpStr);
}

void DrawAllVOIPResult(VOIP_STRUCT *pOneVOIP)
{
	char   MsgStr[200] = {0};
	static  unsigned long ulPostTimer = GetTickCount();
	unsigned long  ulCurTimer;

	if (pOneVOIP == NULL)
		return;

	ulCurTimer = GetTickCount();
	if (ulCurTimer - ulPostTimer >= 1000)
	{
		ulPostTimer = ulCurTimer;

		memset(MsgStr, 0, 100 );
		sprintf(MsgStr, "%d", g_iTotalCnt);
		pdlg->GetDlgItem(IDC_STATIC_Count)->SetWindowText(MsgStr);

		
		sprintf(MsgStr, "%ld", g_iTotalCallErr, g_iTotalCheckErr);
		pdlg->GetDlgItem(IDC_STATIC_Fail)->SetWindowText(MsgStr);

		sprintf(MsgStr, "%ld", g_iTotalCheckErr);
		pdlg->GetDlgItem(IDC_STATIC_DtmfErr)->SetWindowText(MsgStr);

		if (g_iTotalCnt != 0)
		{
			memset(MsgStr, 0, sizeof(MsgStr));
			sprintf(MsgStr, "%f%%/%f%%", ((float)(g_iTotalCallErr*100))/(float)(g_iTotalCnt),
				((float)(g_iTotalCheckErr*100))/(float)(g_iTotalCnt));
			pdlg->GetDlgItem(IDC_STATIC_SucRate)->SetWindowText(MsgStr);
		}

		if (g_iEstablishSum <= MAXLONGLONG && g_iCallSuc > 0)
		{
			sprintf(MsgStr, "%ld ms", g_iEstablishSum/g_iCallSuc);
			pdlg->GetDlgItem(IDC_STATIC_AvsSetUp)->SetWindowText(MsgStr);
		}	
	}
}


void DrawTotalTestTime()
{
	char  TmpStr[100] = {0};
	LONGLONG lTotlalTime = 0;
	int     iH = 0;
	int     iM = 0;
	int     iS = 0;
	
	static  unsigned long ulPostTimer = GetTickCount();
	unsigned long  ulCurTimer;
	

	lTotlalTime = (GetTickCount() - g_iTestStartTime)/1000;
	
	iH = (int)lTotlalTime/3600;
	iM = (int)(lTotlalTime - iH*3600)/60;
	iS = (int)(lTotlalTime - iH*3600 - iM * 60);
	sprintf(TmpStr, "%02d:%02d:%02d", iH, iM, iS);

	ulCurTimer = GetTickCount();
	if (ulCurTimer - ulPostTimer >= 1000)
	{
		ulPostTimer = ulCurTimer;
		pdlg->GetDlgItem(IDC_STATIC_TestTimer)->SetWindowText(TmpStr);
	}
}

DWORD WINAPI ThreadFunc(VOID) 
{ 
	int               i;
	RetCode_t         ret;
	DeviceID_t		  FreeVOIPDeviceID;
	VOIP_STRUCT	     *pOneVOIP = NULL;	
	char			  MsgStr[256] = {0};
	DJ_S8             s8ModID;
	DJ_S16            s16ChID;
	int               iTotalVOIP;
	int               iStartChn = 0, iEndChn = 0;
	VoIPCallPrivate_t voipCallParam = {0};
	int               iStart = 0;
	int               iEnd = 0;


	WriteTestLog(NULL, "Test Start");
	g_iTestStartTime = GetTickCount();

	pdlg->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);

	pdlg->GetDlgItem(IDC_EDIT_FROM)->GetWindowText(MsgStr, sizeof(MsgStr));
	if (strlen(MsgStr) == 0)
		iStartChn = 0;
	else
		iStartChn = atoi(MsgStr);

	pdlg->GetDlgItem(IDC_EDIT_To)->GetWindowText(MsgStr, sizeof(MsgStr));	
	if (strlen(MsgStr) == 0)
		iEndChn = g_iTotalVOIP;
	else
		iEndChn = atoi(MsgStr);

	if (iStart < 0 || iStart > iStartChn)
		iStart = 0;

	if (iEndChn <=0 || iEndChn > g_iTotalVOIP)
		iEndChn = g_iTotalVOIP;	
	
	while(1)
	{
		DrawTotalTestTime();

		EnterCriticalSection(&CriticalSection);
		iTotalVOIP = g_iTotalVOIP;

		if (g_iEstablishSum >= MAXLONGLONG)
			return 0;
		LeaveCriticalSection(&CriticalSection);
	
	
		for (i=iStartChn; i<iEndChn; i++)
		{
			EnterCriticalSection(&CriticalSection);
			s8ModID = MapTable_VOIP[i].m_s8ModuleID;
			s16ChID = MapTable_VOIP[i].m_s16ChannelID;	
			
			if (AllDeviceRes[s8ModID].pVOIP == NULL)
				break;	

			pOneVOIP = &AllDeviceRes[s8ModID].pVOIP[s16ChID];
			DrawAllVOIPResult(pOneVOIP);	
			
			if ( ( AllDeviceRes[s8ModID].pVOIP[s16ChID].iLineState == DCS_FREE )			
				&& AllDeviceRes[s8ModID].pVOIP[s16ChID].State == VOIP_FREE)
			{			
				
				FreeVOIPDeviceID = pOneVOIP->deviceID;
				
				// fill call parameters
				voipCallParam.m_PrivateData.m_u32DataSize = sizeof(VoIPCallPrivate_t);
				voipCallParam.m_s32Protocol = cfg_Protocol == 0 ? XMS_VOIP_PROTOCOL_H323 : XMS_VOIP_PROTOCOL_SIP;
				
				
				strcpy(voipCallParam.m_s8CalleeAddress, cfg_CalleeAddr);
				voipCallParam.m_s32CalleePort = cfg_CalleePort;
				strcpy(voipCallParam.m_s8CalleeUserID, cfg_CalleeUserID);

				strcpy(voipCallParam.m_s8CallerAddress, cfg_CallerAddr);
				voipCallParam.m_s32CallerPort = cfg_CallerPort;
				strcpy(voipCallParam.m_s8CallerUserID, cfg_CallerUserID);
               
			//	sprintf(cfg_CalleeTelNo, "%02d%02d",  pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID);
				// send make call command
				ret = XMS_ctsMakeCallOut(g_acsHandle, &FreeVOIPDeviceID,
					cfg_CallerTelNo, cfg_CalleeTelNo, (PrivateData_t *)&voipCallParam);			
				if ( ret > 0 )
				{						
					if (cfg_iVoiceCheck == 1)
					{						
						Change_State (pOneVOIP, VOIP_CALLOUT );	
					}
					else
					{					
						Change_State (pOneVOIP, VOIP_CALLOUT1);
					}

					pOneVOIP->iCallOutTime = GetTickCount();					
					pOneVOIP->iTotalCnt++;
					g_iTotalCnt++;
				}
				else
				{
					sprintf ( MsgStr, "XMS_ctsMakeCallOut() FAIL! ret = %d", ret );
					AddMsg ( MsgStr );
					WriteMsgLog(MsgStr);
				}
			}
			LeaveCriticalSection(&CriticalSection);		
		//	Sleep(1000);		
		}
		
		Sleep(30);
	}
	
   return 0; 
}  

void   CreateFileDirector(DJ_S8 s8DspModID)
{
	char pchFileName[256];

	sprintf(pchFileName, "%s\\Rec%d", cfg_VocPath, s8DspModID);
	CreateDirectory(pchFileName,NULL);
}

bool GetFilePathName(VOIP_STRUCT *pOneVOIP, char *pchFilePath)
{
	if (pchFilePath == NULL)
		return false;;

	if (pOneVOIP->deviceID.m_s8ModuleID > 0)
	{
		sprintf(pchFilePath, "%s\\Rec%d", cfg_VocPath, pOneVOIP->deviceID.m_s8ModuleID);		
	}

	return true;	
}

void	SimulateCallOut(void)
{
	DWORD dwThreadID = 0;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFunc, NULL, 0, &dwThreadID);  
}

void   StopCallOut(void)
{
}

