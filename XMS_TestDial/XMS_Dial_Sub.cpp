#include "stdafx.h"
#include "XMS_TestDial.h"
#include "XMS_TestDialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_Dial_Sub.H"
#include "XMS_Dial_String.H"
#include "XMS_Dial_Event.H"

// Special code for CAS(SS1)
#include "..\CAS_Common_Code\CAS_Common_Cfg.h"				
CmdParamData_CAS_t g_Param_CAS;
// end of code for CAS(SS1)

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_TestDialApp theApp;
CXMS_TestDialDlg	*pdlg;

// var about XMS_Dial.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Dial.INI";

ServerID_t		cfg_ServerID;
int				cfg_iCalledLen = 5;
char			cfg_CallingNum[32] = "83636988";

char			cfg_VocPath[128] = "";	
int				cfg_iDispChnl = 32;		// always disp 32 channel
int				cfg_iVoiceRule;		// search voice in Fix Relationship

int				cfg_iPartWorkModuleID[256]={0};
char            cfg_chPartWorkModuleID[256] = {0};

char			cfg_SimCalledNum[32] = "12345";

int             cfg_iVoiceCheck = 0;

float            cfg_iTestTime = 1.0;

long             g_iTestStartTime = 0;

int				 g_iElapseTime = 100000;  //100 个 1000 毫秒

LONGLONG    g_iTotalCnt = 0; 
LONGLONG    g_iTotalCallErr = 0;
LONGLONG    g_iTotalCheckErr = 0;
LONGLONG    g_iEstablishSum = 0;
LONGLONG    g_iCallSuc = 0;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 2;

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

CRITICAL_SECTION CriticalSection;

// -----------------------------------------------------------------------
// local function decalration
void	WriteToConfig(void);
void	ReadFromConfig(void);
void	InitTextBox(void);
void	FetchFromText();

void	InitListMain(void);
void	InitListPcm(void);
void	InitListMsg(void);
bool    GetFilePathName(TRUNK_STRUCT *pOneTrunk, char *pchFilePath);

void DrawOneTrunkResult(TRUNK_STRUCT *pOneTrunk);

char dtmf_table[16] = { 'D', '1','2','3',
                        '4','5','6','7',
						'8','9','0','*',
						'#', 'A','B','C' };
// -----------------------------------------------------------------------
 bool IsCorrectDtmf(TRUNK_STRUCT *pOneTrunk)
 {
	 int i;

	 if (pOneTrunk == NULL)
		 return false;

	 if (pOneTrunk->DtmfCount != 10)
	 {
		 return false;
	 }

	 for (i=0; i<10; i++)
	 {
		 if (pOneTrunk->DtmfBuf[i] != dtmf_table[(pOneTrunk->iSeqID%15)])
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
 void WriteTestLog ( TRUNK_STRUCT *pOneTrunk, char *s )
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
	else if (pOneTrunk != NULL)
	{	
	
		fprintf ( fp, " [%s %s], DeviceID(%d, %d) %s \n", tmpDate, tmpTime,
			pOneTrunk->deviceID.m_s8ModuleID,  pOneTrunk->deviceID.m_s16ChannelID, s );		
		
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

	GetPrivateProfileString("ConfigInfo", "TestTime", "0.00", TmpStr, sizeof(TmpStr), cfg_IniName);
	sscanf(TmpStr, "%f", &cfg_iTestTime);

	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);

	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);

	cfg_iCalledLen = GetPrivateProfileInt ( "ConfigInfo", "CalledLen", 5, cfg_IniName);
	if ( cfg_iCalledLen > 20 )
		cfg_iCalledLen = 20;

	GetPrivateProfileString ( "ConfigInfo", "CallingNum", "83636988", cfg_CallingNum, sizeof(cfg_CallingNum), cfg_IniName);

	GetPrivateProfileString ( "ConfigInfo", "SimCalledRNum", "12345", cfg_SimCalledNum, sizeof(cfg_SimCalledNum), cfg_IniName);	

	cfg_iVoiceRule = GetPrivateProfileInt ( "ConfigInfo", "VoiceRule", 0, cfg_IniName);
	
	GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(TmpStr, cfg_chPartWorkModuleID, sizeof(TmpStr));	
	SplitStr2Int(TmpStr, ",", cfg_iPartWorkModuleID);

	cfg_iVoiceCheck = GetPrivateProfileInt ("ConfigInfo", "VoiceCheck", 0, cfg_IniName);
}

void	WriteToConfig(void)
{
	char	TmpStr[256];

	sprintf(TmpStr, "%f", cfg_iTestTime);
	WritePrivateProfileString("ConfigInfo", "TestTime", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "VocPath", cfg_VocPath, cfg_IniName);
	
	sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	WritePrivateProfileString ( "ConfigInfo", "Port", TmpStr, cfg_IniName);

	sprintf ( TmpStr, "%d", cfg_iCalledLen);
	WritePrivateProfileString ( "ConfigInfo", "CalledLen", TmpStr, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "CallingNum", cfg_CallingNum, cfg_IniName);

	WritePrivateProfileString ( "ConfigInfo", "SimCalledRNum", cfg_SimCalledNum, cfg_IniName);

	sprintf(TmpStr, "%d", cfg_iVoiceRule);
	WritePrivateProfileString("ConfigInfo", "VoiceRule", TmpStr, cfg_IniName);

	sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
	WritePrivateProfileString("ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);

	sprintf(TmpStr, "%d", cfg_iVoiceCheck);
	WritePrivateProfileString("ConfigInfo", "VoiceCheck", TmpStr, cfg_IniName);
}

void	InitTextBox(void)
{
	char	TmpStr[256];

	sprintf(TmpStr, "%f", cfg_iTestTime);
	pdlg->GetDlgItem(IDC_EDIT_TestTime)->SetWindowText(TmpStr);

	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->SetWindowText ( cfg_ServerID.m_s8ServerIp );

	sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort );
	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->SetWindowText ( TmpStr );	

	sprintf ( TmpStr, "%d", cfg_iCalledLen );
	pdlg->GetDlgItem ( IDC_EDIT_CalledLen )->SetWindowText ( TmpStr );

	pdlg->GetDlgItem ( IDC_EDIT_CallingNum )->SetWindowText ( cfg_CallingNum );

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->SetWindowText(cfg_VocPath);

	if ( cfg_iVoiceRule == 0 )
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_Fix))->SetCheck(1) ;
	else if ( cfg_iVoiceRule == 1 )
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_SameModule))->SetCheck(1) ;
	else 
		((CButton *)pdlg->GetDlgItem(IDC_RADIO_MostFreeFist))->SetCheck(1) ;

	pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum )->SetWindowText ( cfg_SimCalledNum );

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

	pdlg->GetDlgItem(IDC_EDIT_TestTime)->GetWindowText(TmpStr, sizeof(TmpStr));
	sscanf(TmpStr, "%f", &cfg_iTestTime);

	pdlg->GetDlgItem ( IDC_EDIT_IPADDR )->GetWindowText ( cfg_ServerID.m_s8ServerIp, 30 );

	pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_ServerID.m_u32ServerPort);

	pdlg->GetDlgItem ( IDC_EDIT_CalledLen )->GetWindowText ( TmpStr, 30 );
	sscanf ( TmpStr, "%d", &cfg_iCalledLen );

	pdlg->GetDlgItem ( IDC_EDIT_CallingNum )->GetWindowText ( cfg_CallingNum, 30 );

	pdlg->GetDlgItem ( IDC_EDIT_VOCPATH )->GetWindowText ( cfg_VocPath, 128 );	
	
	pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum )->GetWindowText ( cfg_SimCalledNum, 30 );

	if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Fix))->GetCheck ( ) == 1)
		cfg_iVoiceRule = 0;
	else if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_SameModule))->GetCheck() == 1)
		cfg_iVoiceRule = 1;
	else
		cfg_iVoiceRule = 2;

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

	if (pdlg == NULL)
		return;
	
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

	if (pOneTrunk == NULL || pdlg == NULL)
		return;

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

	if (pOneTrunk == NULL || pdlg == NULL)
		return;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	//
	switch( pOneTrunk->State ) 
	{
	case TRK_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case TRK_FREE:		
		strcpy(StateStr,"空闲"); 
		break ;

	case TRK_NOTHANDLE:
		strcpy(StateStr,"不处理");
		break ;

	case TRK_CALLOUT:
		strcpy(StateStr,"呼出");
		break;

	case TRK_CALLOUT1:
		strcpy(StateStr, "呼出");
		break;
	
	case TRK_WAIT_LINKOK:
		strcpy(StateStr,"等Link完成");
		break;

	case TRK_PLAY:
		strcpy(StateStr, "放音");
		break;

	case TRK_GETDTMF:
		strcpy(StateStr, "收DTMF码");
		break;
	
	case TRK_FAIL:
		strcpy(StateStr,"呼出失败");
		break;

	case TRK_WAITHANGUP:
		strcpy(StateStr, "等待挂机");
		break;
	
	case TRK_WAITSTOP:
		strcpy(StateStr, "测试停止");
		break;

	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListMain.SetItemText ( iDispRow, 4, StateStr ); 
}


void DrawMain_VocInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;

	if (pOneTrunk == NULL || pdlg == NULL)
		return;

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

void DrawMain_EstableTime(TRUNK_STRUCT *pOneTrunk)
{
	char StateStr[100];
	int  iDispRow;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;

	iDispRow = CalDispRow(pOneTrunk->iSeqID);
	if (iDispRow < 0) 
		return;

	sprintf(StateStr, "%d", pOneTrunk->iEstablishTime);
	pdlg->m_ListMain.SetItemText(iDispRow, 9, StateStr);
}

void DrawMain_Reason ( TRUNK_STRUCT *pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneTrunk == NULL || pdlg == NULL)
		return;

	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;

	sprintf ( StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause ); 
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 
	WriteTestLog(pOneTrunk, StateStr);
}

void Refresh_DtmfInfo( TRUNK_STRUCT *pOneTrunk )
{
	int   iDispRow;

	if (pOneTrunk == NULL || pdlg == NULL)
		return;

	iDispRow = pOneTrunk->iSeqID;

	pdlg->m_ListMain.SetItemText(iDispRow, 5, "");
}

void DrawTrunk_DTMF( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;

	if (pOneTrunk == NULL || pdlg == NULL)
		return;

	iDispRow = pOneTrunk->iSeqID;

	strcpy(StateStr,pOneTrunk->DtmfBuf);
	pdlg->m_ListMain.SetItemText ( iDispRow, 5, StateStr ); 

}

// -------------------------------------------------------------------------------------------------
void	InitListPcm(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;

	if (pdlg == NULL)
		return;

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

	if (pdlg == NULL)
		return;

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
	
	if (NULL == pdlg)
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
}

void	DrawCount_Pcm ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;

	if (NULL == pdlg)
		return ;

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

	if (pdlg == NULL)
		return;

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

	if (NULL == pdlg)
		return;

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


// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;

	if (pdlg == NULL)
		return;

	// m_ListMain
	pdlg->m_ListMain.DeleteAllItems ();
	for ( i = 0; i < g_iTotalTrunk; i ++ )
	{
		iDispRow = CalDispRow ( i ); 
		if ( iDispRow < 0 )	continue;

		// 固定不变的内容
		sprintf ( TmpStr, "%3d", iDispRow );
		pdlg->m_ListMain.InsertItem ( iDispRow, TmpStr );

		sprintf ( TmpStr, "%d,%d", MapTable_Trunk[i].m_s8ModuleID, MapTable_Trunk[i].m_s16ChannelID );
		pdlg->m_ListMain.SetItemText ( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk(MapTable_Trunk[i]);
		pdlg->m_ListMain.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );

		// 可能改变的内容
		DrawMain_LineState ( pOneTrunk );
		DrawMain_State ( pOneTrunk );	
	}

	// m_ListPcm
	pdlg->m_ListPcm.DeleteAllItems ();
	for ( i = 0; i < g_iTotalPcm; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListPcm.InsertItem ( i, TmpStr );

		// 可能改变的内容
		DrawPcm_TypeAndAlarm ( &M_OnePcm(MapTable_Pcm[i]) );
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
		DrawCount_Pcm ( MapTable_Module[i] );
		DrawCount_Trunk ( MapTable_Module[i] );
		DrawCount_Voc ( MapTable_Module[i] );
	}
}

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CXMS_TestDialDlg	*)theApp.m_pMainWnd;

	// Read From "XMS_Dial.INI"
	ReadFromConfig();

	// Special code for CAS(SS1)
	// Read From "C:\\DJXMS\\Samples\\CAS_Common_Code\\SaXMS_CAS_Cfg.INI"
	if ( CAS_Common_Cfg_ReadCfg ( &g_Param_CAS ) != 0 )
	{
		sprintf ( MsgStr, "CAS_Common_Cfg_ReadCfg \"C:\\DJXMS\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI\" Error!" );
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
void	OpenTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};

	if (pOneTrunk == NULL)
		return;
	
	pOneTrunk->State= TRK_WAITOPEN;

	r = XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenTrunkDevice()! ret=%d", r);
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

void	OpenPcmDevice ( PCM_STRUCT *pOnePcm )
{
	RetCode_t	r;
	char        TmpStr[500]={0};
	
	r = XMS_ctsOpenDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenPcmDevice()! ret=%d", r);
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

	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}

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
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;
	VOICE_STRUCT	*pOneVoice;
	PCM_STRUCT		*pOnePcm;

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);

		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation

		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			Change_State ( pOneTrunk, TRK_NOTHANDLE );		// 中继通道不处理该user通道		
		}
		else
		{
			InitTrunkChannel(pOneTrunk);
			InitTrunkcounter(pOneTrunk);

			XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

		}

		// modify the count
		g_iTotalTrunkOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;

		DrawCount_Trunk ( pDevice->m_s8ModuleID  );
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
void	CloseTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	if (pOneTrunk == NULL)
		return;

	pOneTrunk->State= TRK_WAITOPEN;

	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseTrunkDevice()! ret=%d", r);
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

void	ClosePcmDevice ( PCM_STRUCT *pOnePcm )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in ClosePcmDevice()! ret=%d", r);
		AddMsg (TmpStr);
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

void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{	
	if (pOneTrunk == NULL)
		return;

	pOneTrunk->State = NewState;

	DrawMain_State ( pOneTrunk );
}

void My_InitDtmfBuf ( TRUNK_STRUCT *pOneTrunk )
{
	if (pOneTrunk == NULL)
		return;

	pOneTrunk->DtmfCount = 0;
	pOneTrunk->DtmfBuf[0] = 0; 
}

void My_AddDtmfBuf ( TRUNK_STRUCT *pOneTrunk, char TmpDtmf )
{
	if (pOneTrunk == NULL)
		return;

	if ( pOneTrunk->DtmfCount < (32-1) )
	{
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount] = TmpDtmf; 
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount+1] = 0; 
		pOneTrunk->DtmfCount ++;
	}
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

int		SearchOneFreeVoice (  TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8			s8ModID;
	DJ_S16			s16ChID;
	int				i;
	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;
	long			lNowMostFreeNum;

	if (pOneTrunk == NULL)
		return -1;

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

				// 占用该voice device 
				pOneVoice->State = VOC_USED;
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

void    InitTrunkcounter(TRUNK_STRUCT *pOneTrunk)
{
	if (pOneTrunk == NULL)
		return;

	pOneTrunk->iCallErrCnt = 0;
	pOneTrunk->iCheckErrCnt = 0;
	pOneTrunk->iSucCnt = 0;
	pOneTrunk->iTotalCnt = 0;
}

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
	if (pOneTrunk == NULL)
		return;

	Change_State ( pOneTrunk, TRK_FREE );

	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配Voc Device
	My_InitDtmfBuf ( pOneTrunk );

	pOneTrunk->iEstablishTime = 0;
	pOneTrunk->iCallOutTime = 0;

	DrawMain_VocInfo ( pOneTrunk );
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

bool    CheckRecordEnd(TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt)
{
	if (pOneTrunk == NULL)
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

void ResetTrunk ( TRUNK_STRUCT *pOneTrunk )
{
	if (pOneTrunk == NULL)
		return;

	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		StopRecordFile(&pOneTrunk->VocDevID);
		StopPlayFile(&pOneTrunk->VocDevID);
		
		My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );

		FreeOneFreeVoice (  &pOneTrunk->VocDevID );

		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
		DrawMain_VocInfo ( pOneTrunk );
	}

	InitTrunkChannel ( pOneTrunk );
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

char   My_GetDtmfCode(TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt)
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

// Special code for CAS(SS1)
void	SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t				FreeVocDeviceID;

	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_E1_CAS )
		return;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_START )	/* 开始接收MFC，需要一个Voc设备*/
	{
		if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
		{
			pOneTrunk->VocDevID = FreeVocDeviceID;
			M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
			DrawMain_VocInfo ( pOneTrunk );

			g_Param_CAS.m_VocDevID = FreeVocDeviceID;
			XMS_ctsSetParam ( g_acsHandle, &pOneTrunk->deviceID, 
				CAS_PARAM_UNIPARAM, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );
		}
	//	TRACE("(%d,%d) MFC_START\n", pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID);
	}

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_END )	/* 接收MFC完毕，释放Voc设备*/
	{
		FreeOneFreeVoice (  &pOneTrunk->VocDevID );

		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	//	TRACE("(%d,%d) MFC_START\n", pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID);
		DrawMain_VocInfo ( pOneTrunk );
	}
}
// end of code for CAS(SS1)

void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	int          iFreeVoc;
	DeviceID_t   FreeVocDeviceID;
	Acs_CallControl_Data *	pCallControl = NULL;
	char                FileName[256] = {0};
	char				TmpDtmf;
	char                TmpFileName[256] = "";
	char                chError[256] = "";

	if (pOneTrunk == NULL)
		return;

	DrawOneTrunkResult(pOneTrunk);

	if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )	/*定时器事件*/
	{
		XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, 0 );
		Change_State(pOneTrunk, TRK_WAITHANGUP);
		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0 ,NULL );

		return;
	}

	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		if (pOneTrunk->State == TRK_WAITHANGUP)
		{
			if (cfg_iVoiceCheck == 1)
			{
				StopRecordFile(&pOneTrunk->VocDevID);
				
				GetFilePathName(pOneTrunk, TmpFileName);
				sprintf(FileName, "%s\\DemoRec.%0003d", TmpFileName, pOneTrunk->iSeqID);
			
				DeleteFile(FileName); //删除录音文件
				
				if(!IsCorrectDtmf(pOneTrunk))
				{
					sprintf (TmpFileName, "%s.%d.pcm", FileName,pOneTrunk->iTotalCnt);
					rename ( FileName, TmpFileName );
					
					pOneTrunk->iCheckErrCnt++;
					g_iTotalCheckErr++;
					sprintf(chError, "GetDtmf [%s] error, count=%d，ErrCount=%d", pOneTrunk->DtmfBuf, pOneTrunk->iTotalCnt,pOneTrunk->iCheckErrCnt);
					WriteTestLog(pOneTrunk, chError);				
				}
				else
				{
					pOneTrunk->iSucCnt++;
					DeleteFile(FileName);//删除录音文件
				}			

			//	Refresh_DtmfInfo(pOneTrunk);
			}					
		}
		
		ResetTrunk ( pOneTrunk );	
		Refresh_DtmfInfo(pOneTrunk);
		Change_State (pOneTrunk, TRK_FREE);
		DrawMain_EstableTime(pOneTrunk);		
			
		return ; 
	}

	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		break;

	case TRK_CALLOUT: // 带语音检测
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneTrunk->iEstablishTime = GetTickCount() - pOneTrunk->iCallOutTime;
				g_iEstablishSum += pOneTrunk->iEstablishTime;
				DrawMain_EstableTime(pOneTrunk);
				g_iCallSuc++;
				
				iFreeVoc = SearchOneFreeVoice(pOneTrunk, &FreeVocDeviceID);
				if (iFreeVoc >= 0)
				{
					pOneTrunk->u8PlayTag = 16;					
					pOneTrunk->VocDevID = FreeVocDeviceID;
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
					DrawMain_VocInfo ( pOneTrunk );
					My_DualLink(&pOneTrunk->deviceID, &FreeVocDeviceID);					

					Change_State (pOneTrunk, TRK_WAIT_LINKOK);
				}
				else
				{
					Change_State(pOneTrunk, TRK_WAITHANGUP);
					XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
				}							

				//停止定时器函数，设置一个新定时器
			//	XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, 0 );
			//	XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, g_iElapseTime );
			}
			else		// 呼出失败
			{
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );
				g_iTotalCallErr++;
				pOneTrunk->iCallErrCnt++;
			}	
		}
		
		break;

	case TRK_CALLOUT1: //纯信令继续
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		if (pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT)
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneTrunk->iEstablishTime = GetTickCount() - pOneTrunk->iCallOutTime;
				g_iEstablishSum += pOneTrunk->iEstablishTime;
				DrawMain_EstableTime(pOneTrunk);
				g_iCallSuc++;

				pOneTrunk->iSucCnt++;	
				XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
				Change_State (pOneTrunk, TRK_WAITHANGUP);

				//停止定时器函数，设置一个新定时器
		//		XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->VocDevID, 0 );
		//		XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->VocDevID, g_iElapseTime );
			}
			else		// 呼出失败
			{
				pOneTrunk->iCallErrCnt++;
				g_iTotalCallErr++;
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );
			}
		}

		break;

	case TRK_WAIT_LINKOK:
		if (pAcsEvt->m_s32EventType == XMS_EVT_LINKDEVICE)
		{
			pOneTrunk->u8PlayTag++;
			sprintf(FileName, "%s\\sig%d.pcm", cfg_VocPath, (pOneTrunk->iSeqID)%15);
			PlayFile (&pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, true);
			Change_State(pOneTrunk, TRK_PLAY);	
		}
		break;

	case TRK_PLAY:
		if (CheckPlayEnd(pOneTrunk, pAcsEvt))
		{
			pOneTrunk->u8PlayTag++;	
			GetFilePathName(pOneTrunk, TmpFileName);
			sprintf(FileName, "%s\\DemoRec.%0003d", TmpFileName, pOneTrunk->iSeqID);		
			RecordFile ( &pOneTrunk->VocDevID, FileName, 8000L*15, false);

			Change_State (pOneTrunk, TRK_GETDTMF);
		}
		break;

	case TRK_GETDTMF:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}

		if (CheckRecordEnd(pOneTrunk, pAcsEvt))
		{
		//	XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
			Change_State (pOneTrunk, TRK_WAITHANGUP);
		}
		break;
	}
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

void DrawOneTrunkResult(TRUNK_STRUCT *pOneTrunk)
{
	char   TmpStr[500] = {0};

	if (pOneTrunk == NULL || pdlg == NULL)
		return;	
	
	sprintf(TmpStr, "%d/%d/%d", pOneTrunk->iTotalCnt, pOneTrunk->iCallErrCnt, pOneTrunk->iCheckErrCnt);
	pdlg->m_ListMain.SetItemText(pOneTrunk->iSeqID, 6, TmpStr);

}

void DrawAllTrunkResult(TRUNK_STRUCT *pOneTrunk)
{
	char   MsgStr[100] = {0};
	static  unsigned long ulPostTimer = GetTickCount();
	unsigned long  ulCurTimer;

	if (pOneTrunk == NULL)
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
			TRACE("Establise=%d, g_iCallSuc=%d\n", g_iEstablishSum, g_iCallSuc);
		}	
	}
}

void InitTrunkData(int  iTotalTrunk)
{
	int    i;	
	DJ_S8         s8ModID;
	DJ_S16        s16ChID;	
	TRUNK_STRUCT  *pOneTrunk = NULL;

		for (i=0; i<iTotalTrunk; i++)
		{
			s8ModID = MapTable_Trunk[i].m_s8ModuleID;
			s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
			
			if (AllDeviceRes[s8ModID].pTrunk == NULL)	
				break;

			if (IsDigitTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
			{
				pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];

			//	if (pOneTrunk->iLineState != DCS_FREE)
			//		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0 ,NULL );
				pOneTrunk->iCallOutTime = 0;
				pOneTrunk->iCallErrCnt = 0;
				pOneTrunk->iCheckErrCnt = 0;
				pOneTrunk->iEstablishTime = 0;
				pOneTrunk->iSucCnt = 0;
				pOneTrunk->iTotalCnt = 0;		
			}
		
		}	

//	memset(pOneTrunk->CalleeCode, 0, sizeof(pOneTrunk->CalleeCode));
//	memset(pOneTrunk->CallerCode, 0, sizeof(pOneTrunk->CallerCode));
}

bool  IsOverTimeTest(int iTotalTrunk)
{

	if (g_iTotalCnt > MAXLONGLONG 
		|| ((GetTickCount() - g_iTestStartTime) >= cfg_iTestTime*60*60*1000))
	{		
		return true;
	}

	return false;
}

void InitStatData()
{
	g_iTotalCnt = 0; 
	g_iTotalCheckErr = 0;
	g_iTotalCallErr = 0;
	g_iEstablishSum = 0;
}


void DrawTotalTestTime()
{
	char  TmpStr[100] = {0};
	LONGLONG lTotlalTime = 0;
	int     iH = 0;
	int     iM = 0;
	int     iS = 0;

	if (NULL == pdlg)
		return;

	lTotlalTime = (GetTickCount() - g_iTestStartTime)/1000;
	
	iH = (int)lTotlalTime/3600;
	iM = (int)(lTotlalTime - iH*3600)/60;
	iS = (int)(lTotlalTime - iH*3600 - iM * 60);
	sprintf(TmpStr, "%02d:%02d:%02d", iH, iM, iS);
	
	
	pdlg->GetDlgItem(IDC_STATIC_TestTimer)->SetWindowText(TmpStr);
}

DWORD WINAPI ThreadFunc(VOID) 
{ 
	int             i;
	RetCode_t       ret;
	DeviceID_t		FreeTrkDeviceID;
	TRUNK_STRUCT	*pOneTrunk = NULL;	
	char			MsgStr[256];
	DJ_S8           s8ModID;
	DJ_S16          s16ChID;
	int             iTotalTrunk;
	int             iStartChn = 0, iEndChn = 0;

	if (pdlg == NULL)
		return 0;

	WriteTestLog(NULL, "Test Start");
//	InitStatData();
//	InitTrunkData(g_iTotalTrunk);

	g_iTestStartTime = GetTickCount();

	pdlg->GetDlgItem(IDC_EDIT_Start)->EnableWindow(FALSE);
	
	while(1)
	{
		DrawTotalTestTime();

		EnterCriticalSection(&CriticalSection);

		iTotalTrunk = g_iTotalTrunk;		
	
		if (IsOverTimeTest(iTotalTrunk))
		{
			WriteTestResult();		
			pdlg->GetDlgItem(IDC_EDIT_Start)->EnableWindow(TRUE);
			LeaveCriticalSection(&CriticalSection);
			return 0;
		}
		
		LeaveCriticalSection(&CriticalSection);

	
		for (i=0; i<g_iTotalTrunk; i++)
		{
			EnterCriticalSection(&CriticalSection);
			s8ModID = MapTable_Trunk[i].m_s8ModuleID;
			s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
			
			if (AllDeviceRes[s8ModID].pTrunk == NULL)
			{
				LeaveCriticalSection(&CriticalSection);
				break;
			}

			pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];
			DrawAllTrunkResult(pOneTrunk);

			if ( ( pOneTrunk->iLineState == DCS_FREE )
				&& IsDigitTrunk(pOneTrunk->deviceID.m_s16DeviceSub) 			
				&& pOneTrunk->State == TRK_FREE)
			{			
				
				FreeTrkDeviceID = pOneTrunk->deviceID;
				ret = XMS_ctsMakeCallOut ( g_acsHandle, &FreeTrkDeviceID, cfg_CallingNum, cfg_SimCalledNum, NULL );
				if ( ret > 0 )
				{						
					if (cfg_iVoiceCheck == 1)
					{						
						Change_State (pOneTrunk, TRK_CALLOUT );	
					}
					else
					{					
						Change_State (pOneTrunk, TRK_CALLOUT1);
					}

					pOneTrunk->iCallOutTime = GetTickCount();					
					pOneTrunk->iTotalCnt++;
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

bool GetFilePathName(TRUNK_STRUCT *pOneTrunk, char *pchFilePath)
{
	if (pchFilePath == NULL)
		return false;

	if (pOneTrunk->deviceID.m_s8ModuleID > 0)
	{
		sprintf(pchFilePath, "%s\\Rec%d", cfg_VocPath, pOneTrunk->deviceID.m_s8ModuleID);		
	}

	return true;	
}

void	SimulateCallOut(void)
{
	DWORD dwThreadID = 0;
	HANDLE hThread;
	
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFunc, NULL, 0, &dwThreadID);  

	if (hThread != NULL)
	{		
		CloseHandle(hThread);
	}
}

