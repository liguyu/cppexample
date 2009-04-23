#include "stdafx.h"
#include "XMSTest_FaxDial.h"
#include "XMSTest_FaxDialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_FaxDial_Sub.H"
#include "XMS_FaxDial_String.H"
#include "XMS_FaxDial_Event.H"

// -----------------------------------------------------------------------
// variable declaration
extern CXMSTest_FaxDialApp theApp;
CXMSTest_FaxDialDlg	*pdlg;

// var about XMS_Dial.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Dial.INI";

int             g_iCallCount = 0;

long             g_iTestStartTime = 0;

LONGLONG        g_iTotalCall = 0;
LONGLONG        g_iTotalSendFail = 0;
LONGLONG        g_iTotalRecvFail = 0;

float           cfg_iTestTime = 1.0;

int             cfg_s32DebugOn = 0;
int             cfg_s32Record = 0;
int		cfg_sIsSendFax	= 1;

ServerID_t		cfg_ServerID;
int				cfg_iCalledLen = 5;
char			cfg_CallingNum[32] = "83636988";

char			cfg_RcvFaxPath[128] = "";	
char            cfg_SendFaxPath[18] = "";
int				cfg_iDispChnl = 32;		// always disp 32 channel

int				cfg_iPartWorkModuleID[256]={0};
char            cfg_chPartWorkModuleID[256] = {0};

char			cfg_SimCalledNum[32] = "12345";

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

extern int                      g_iTotalFax;
extern int                      g_iTotalFaxOpened;
extern int                      g_iTotalFaxFree;
extern TYPE_CHANNEL_MAP_TABLE   MapTable_Fax[MAX_TRUNK_NUM_IN_THIS_DEMO];

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
void   WriteTestLog(char *pMsg)
{
    FILE   *fp;
    char   tmpTime[128], tmpDate[128];
    
    char   LogFileName[256] = {0}; 
    char   LogFileShortName[] = "\\TestLog.txt";
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
	
	if (strcmp(pMsg, "Test Start") == 0)
	{
	    sprintf(MsgStr, "\n%s %s************************%s**********************\n", tmpDate, tmpTime,pMsg);
	}
	else
	{
	    sprintf(MsgStr, "%s %s  %s\n",tmpDate ,tmpTime, pMsg);			
	}
	
	fwrite(MsgStr, strlen(MsgStr), 1, fp);
    }
    
    if (fp != NULL)
	fclose(fp);		
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
	sprintf(TmpStr, "fopen(%s) error.", LogFileName);
	return;
    }
    
    _strtime(tmpTime);
    _strdate(tmpDate);
    
    fprintf(fp, "\n**[%s %s]***************Test End**********\n", tmpDate, tmpTime);
    pdlg->GetDlgItem(IDC_STATIC_SendStat)->GetWindowText(TmpStr, sizeof(TmpStr));
    fprintf(fp, "**Send Fail：%s                            \n", TmpStr);
    pdlg->GetDlgItem(IDC_STATIC_RecvStat)->GetWindowText(TmpStr, sizeof(TmpStr));
    fprintf(fp, "**Recv Fail：%s                                \n", TmpStr);
    pdlg->GetDlgItem(IDC_STATIC_Total)->GetWindowText(TmpStr, sizeof(TmpStr));
    fprintf(fp, "**Call Total: %s                            \n", TmpStr);
    pdlg->GetDlgItem(IDC_STATIC_FailRate)->GetWindowText(TmpStr, sizeof(TmpStr));
    fprintf(fp, "**Fail Rate: %s                  \n", TmpStr);
    pdlg->GetDlgItem(IDC_STATIC_TestTimer)->GetWindowText(TmpStr, sizeof(TmpStr));
    fprintf(fp, "**Test Time: %s                            \n", TmpStr);
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
    char TmpStr[200];
    GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
    strcat ( cfg_IniName, cfg_IniShortName );
    
    GetPrivateProfileString("ConfigInfo", "TestTime", "0.00", TmpStr, sizeof(TmpStr), cfg_IniName);
    sscanf(TmpStr, "%f", &cfg_iTestTime);
    
    GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);
    
    cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);
    
    GetPrivateProfileString ( "ConfigInfo", "RecvFaxPath", "..\\Recv Fax\\", cfg_RcvFaxPath, sizeof(cfg_RcvFaxPath), cfg_IniName);
    
    GetPrivateProfileString ( "ConfigInfo", "SendFaxPath", "..\\Send Fax\\", cfg_SendFaxPath, sizeof(cfg_SendFaxPath), cfg_IniName);
    
    cfg_iCalledLen = GetPrivateProfileInt ( "ConfigInfo", "CalledLen", 5, cfg_IniName);
    if ( cfg_iCalledLen > 20 )
	cfg_iCalledLen = 20;
    
    GetPrivateProfileString ( "ConfigInfo", "CallingNum", "83636988", cfg_CallingNum, sizeof(cfg_CallingNum), cfg_IniName);
    
    GetPrivateProfileString ( "ConfigInfo", "SimCalledRNum", "12345", cfg_SimCalledNum, sizeof(cfg_SimCalledNum), cfg_IniName);	
    
    GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
    strncpy(TmpStr, cfg_chPartWorkModuleID, sizeof(TmpStr));	
    SplitStr2Int(TmpStr, ",", cfg_iPartWorkModuleID);
    
    cfg_s32DebugOn = GetPrivateProfileInt ( "ConfigInfo", "DebugOn", 0, cfg_IniName);
    
    cfg_s32Record = GetPrivateProfileInt ("ConfigInfo", "RecordSwitch", 0, cfg_IniName);
    
    cfg_sIsSendFax = GetPrivateProfileInt ( "ConfigInfo", "IsSendFax", 1, cfg_IniName);
}

void	WriteToConfig(void)
{
    char	TmpStr[256];
    
    sprintf(TmpStr, "%f", cfg_iTestTime);
    WritePrivateProfileString("ConfigInfo", "TestTime", TmpStr, cfg_IniName);
    
    WritePrivateProfileString ( "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName);
    
    WritePrivateProfileString("ConfigInfo", "RecvFaxPath", cfg_RcvFaxPath, cfg_IniName);
    
    WritePrivateProfileString("ConfigInfo", "SendFaxPath", cfg_SendFaxPath, cfg_IniName);
    
    sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
    WritePrivateProfileString ( "ConfigInfo", "Port", TmpStr, cfg_IniName);
    
    sprintf ( TmpStr, "%d", cfg_iCalledLen);
    WritePrivateProfileString ( "ConfigInfo", "CalledLen", TmpStr, cfg_IniName);
    
    WritePrivateProfileString ( "ConfigInfo", "CallingNum", cfg_CallingNum, cfg_IniName);
    
    WritePrivateProfileString ( "ConfigInfo", "SimCalledRNum", cfg_SimCalledNum, cfg_IniName);
    
    sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
    WritePrivateProfileString("ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);
    
    sprintf ( TmpStr, "%d", cfg_s32DebugOn);
    WritePrivateProfileString ( "ConfigInfo", "DebugOn", TmpStr, cfg_IniName);
    
    sprintf(TmpStr, "%d", cfg_s32Record);
    WritePrivateProfileString("ConfigInfo", "RecordSwitch", TmpStr, cfg_IniName);
    
    sprintf ( TmpStr, "%d", cfg_sIsSendFax);
    WritePrivateProfileString ( "ConfigInfo", "IsSendFax", TmpStr, cfg_IniName);
}

void	InitTextBox(void)
{
    char	TmpStr[256];
    
    sprintf(TmpStr, "%f", cfg_iTestTime);
    pdlg->GetDlgItem(IDC_EDIT_TestTime)->SetWindowText(TmpStr);
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_IPADDR))->SetWindowText ( cfg_ServerID.m_s8ServerIp );
    
    sprintf ( TmpStr, "%d", cfg_ServerID.m_u32ServerPort );
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_IPPORT))->SetWindowText ( TmpStr );	
    
    sprintf ( TmpStr, "%d", cfg_iCalledLen );
    ((CEdit *)pdlg->GetDlgItem ( IDC_EDIT_CalledLen ))->SetWindowText ( TmpStr );
    
    ((CEdit *)pdlg->GetDlgItem ( IDC_EDIT_CallingNum ))->SetWindowText(cfg_CallingNum);
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_SimCalledNum))->SetWindowText(cfg_SimCalledNum);
    
    ((CEdit *)pdlg->GetDlgItem ( IDC_EDIT_SendFaxName ))->SetWindowText(cfg_SendFaxPath );
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_RCVFAXPATH))->SetWindowText(cfg_RcvFaxPath);
    
    sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_ModuleID))->SetWindowText(TmpStr);
    
    if ( cfg_s32DebugOn == 0 )
	((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 0 ) ;
    else
	((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 1 ) ;
    
    if ( cfg_s32Record == 0)
	((CButton *)pdlg->GetDlgItem(IDC_CHECK_Record))->SetCheck(0);
    else
	((CButton *)pdlg->GetDlgItem(IDC_CHECK_Record))->SetCheck(1);

    if( cfg_sIsSendFax == 0 )
	((CButton *)pdlg->GetDlgItem(IDC_CHECK_SENDFAX))->SetCheck(0);
    else
	((CButton *)pdlg->GetDlgItem(IDC_CHECK_SENDFAX))->SetCheck(1);
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
    
    ((CEdit *)pdlg->GetDlgItem ( IDC_EDIT_CallingNum ))->GetWindowText ( cfg_CallingNum, 30 );
    
    ((CEdit *)pdlg->GetDlgItem ( IDC_EDIT_SimCalledNum ))->GetWindowText ( cfg_SimCalledNum, 30 );
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_SendFaxName))->GetWindowText(cfg_SendFaxPath, 128);
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_RCVFAXPATH))->GetWindowText(cfg_RcvFaxPath, 128);
    
    pdlg->GetDlgItem(IDC_EDIT_ModuleID)->GetWindowText(cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID));
    
    if (((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->GetCheck() == 1)
    {
	cfg_s32DebugOn = 1;
    }
    else
    {
	cfg_s32DebugOn = 0;
    }
    
    if (((CButton *)pdlg->GetDlgItem(IDC_CHECK_Record))->GetCheck() == 1)
    {
	cfg_s32Record = 1;
    }
    else
    {
	cfg_s32Record = 0;
    }

    if (((CButton *)pdlg->GetDlgItem(IDC_CHECK_SENDFAX))->GetCheck() == 1)
    {
	cfg_sIsSendFax = 1;
    }
    else
    {
	cfg_sIsSendFax = 0;
    }
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
    lvc.pszText = "收DTMF码" ;
    lvc.cx = 60;
    pdlg->m_ListMain.InsertColumn ( 5, &lvc ); 
    
    lvc.iSubItem = 6;
    lvc.pszText = "VoiceDev" ;
    lvc.cx = 60;
    pdlg->m_ListMain.InsertColumn ( 6, &lvc ); 
    
    lvc.iSubItem = 7;
    lvc.pszText = "FailReason" ;
    lvc.cx = 75;
    pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 
    
    lvc.iSubItem = 8;
    lvc.pszText = "FaxDevice";
    lvc.cx = 60;
    pdlg->m_ListMain.InsertColumn(8, &lvc);
    
    lvc.iSubItem = 9;
    lvc.pszText = "Send Total/Fail";
    lvc.cx = 80;
    pdlg->m_ListMain.InsertColumn(9, &lvc);
    
    lvc.iSubItem = 10;
    lvc.pszText = "Recv Total/Fail";
    lvc.cx = 80;
    pdlg->m_ListMain.InsertColumn(10, &lvc);
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

void   DrawMain_LineState( TRUNK_STRUCT *pOneTrunk )
{
    char StateStr[100];
    int	 iDispRow;
    
    iDispRow = CalDispRow(pOneTrunk->iSeqID); 
    if ( iDispRow < 0 )	return;
    
    // 
    GetString_LineState ( StateStr, pOneTrunk->iLineState );
    pdlg->m_ListMain.SetItemText ( iDispRow, 3, StateStr ); 
}

void   DrawMain_State( TRUNK_STRUCT *pOneTrunk )
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
	strcpy(StateStr,"空闲"); 
	break ;
	
    case TRK_NOTHANDLE:
	strcpy(StateStr,"不处理");
	break ;
	
    case TRK_CALLOUT:
	strcpy(StateStr,"呼出");
	break;
	
    case TRK_FAX_SEND:
	strcpy(StateStr,"发送传真");
	break;	
	
    case TRK_FAX_RECEIVE:
	strcpy(StateStr, "接收传真");
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


void   DrawMain_VocInfo( TRUNK_STRUCT *pOneTrunk )
{
    char StateStr[100];
    int	 iDispRow;
    DeviceID_t	*pDev1, *pDev2;
    
    iDispRow = CalDispRow(pOneTrunk->iSeqID); 
    if ( iDispRow < 0 )	return;
    
    if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
    {
	pDev1 = &pOneTrunk->VocDevID;
	pDev2 = &pOneTrunk->VocDevID_ForRec;
	sprintf ( StateStr, "(%d,%d)(%d,%d)", pDev1->m_s8ModuleID, pDev1->m_s16ChannelID,
	    pDev2->m_s8ModuleID, pDev2->m_s16ChannelID); 
    }
    else
	strcpy(StateStr, "");
    
    pdlg->m_ListMain.SetItemText ( iDispRow, 6, StateStr ); 
}

void   DrawMain_FaxInfo(TRUNK_STRUCT *pOneTrunk)
{
    char StateStr[100];
    int  iDispRow;
    DeviceID_t *pDev;
    
    iDispRow = CalDispRow(pOneTrunk->iSeqID);
    if (iDispRow < 0) return;
    
    if (pOneTrunk->FaxDevID.m_s16DeviceMain != 0)
    {
	pDev = &pOneTrunk->FaxDevID;
	sprintf(StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID);
    }
    else
	strcpy(StateStr, "");
    
    pdlg->m_ListMain.SetItemText(iDispRow, 8, StateStr);
}

void   DrawMain_Reason ( TRUNK_STRUCT *pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause )
{
    char StateStr[100];
    int	 iDispRow;
    
    iDispRow = CalDispRow(pOneTrunk->iSeqID); 
    if ( iDispRow < 0 )	return;
    
    sprintf ( StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause ); 
    pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 
}

void   Refresh_DtmfInfo( TRUNK_STRUCT *pOneTrunk )
{
    int   iDispRow;
    
    iDispRow = pOneTrunk->iSeqID;
    
    pdlg->m_ListMain.SetItemText(iDispRow, 5, "");
}

void   DrawTrunk_DTMF( TRUNK_STRUCT *pOneTrunk )
{
    char StateStr[100];
    int	 iDispRow;
    
    iDispRow = pOneTrunk->iSeqID;
    
    strcpy(StateStr,pOneTrunk->DtmfBuf);
    pdlg->m_ListMain.SetItemText ( iDispRow, 5, StateStr ); 
}

void    InitListFax(void)
{
    LV_COLUMN	lvc;
    DWORD dwExStyle;
    
    dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
	LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
    pdlg->m_ListFax.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
    
    lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    
    lvc.iSubItem = 0;
    lvc.pszText = "ID" ;
    lvc.cx = 30;
    pdlg->m_ListFax.InsertColumn ( 0, &lvc ); 
    
    lvc.iSubItem = 1;
    lvc.pszText = "Mod,Ch" ;
    lvc.cx = 50;
    pdlg->m_ListFax.InsertColumn ( 1, &lvc ); 
    
    lvc.iSubItem = 2;
    lvc.pszText = "LineState" ;
    lvc.cx = 70;
    pdlg->m_ListFax.InsertColumn ( 2, &lvc ); 
    
    lvc.iSubItem = 3;
    lvc.pszText = "State" ;
    lvc.cx = 70;
    pdlg->m_ListFax.InsertColumn ( 3, &lvc ); 
    
    lvc.iSubItem = 4;
    lvc.pszText = "Detail Infomation" ;
    lvc.cx = 250;
    pdlg->m_ListFax.InsertColumn ( 4, &lvc ); 
    
    lvc.iSubItem = 5;
    lvc.pszText = "Fax File Path";
    lvc.cx = 250;
    pdlg->m_ListFax.InsertColumn(5, &lvc);
}

void DrawFax_FilePath(FAX_STRUCT *pOneFax, char *pFilePathStr)
{
    int iDispRow;
    
    if (pOneFax == NULL)
	return;
    
    iDispRow = pOneFax->iSeqID;
    
    pdlg->m_ListFax.SetItemText(iDispRow, 5, pFilePathStr);
}

void DrawFax_LineState(FAX_STRUCT *pOneFax)
{
    char StateStr[100];
    int  iDispRow;
    
    iDispRow = pOneFax->iSeqID; 
    
    sprintf ( StateStr, "0x%X", pOneFax->iLineState );
    pdlg->m_ListFax.SetItemText ( iDispRow, 2, StateStr ); 
}

void DrawFax_State( FAX_STRUCT *pOneFax )
{
    char StateStr[100];
    int	 iDispRow;
    
    iDispRow = pOneFax->iSeqID; 
    
    //
    switch( pOneFax->State ) 
    {
    case FAX_WAITOPEN:
	strcpy(StateStr,"Wait Open"); 
	break;
	
    case FAX_FREE:		
	strcpy(StateStr,"空闲"); 
	break ;
	
    case FAX_USED:
	strcpy(StateStr,"*占用*");
	break;
	
    default:
	strcpy(StateStr,"other");
	break;
    }
    
    pdlg->m_ListFax.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawFax_DetailInfo ( FAX_STRUCT *pOneFax, Acs_FaxProc_Data *pFaxData, int iEvtType)
{
    char StateStr[100] = {0};
    char pFaxHandle[100] = {0};
    int	 iDispRow;
    
    iDispRow = pOneFax->iSeqID; 
    
    if ( pFaxData == NULL )
    {
	sprintf ( StateStr, "" );
    }
    else
    {
	if (iEvtType == XMS_EVT_SENDFAX)
	{			
	    strcpy(pFaxHandle, "Send");
	}
	else if (iEvtType == XMS_EVT_RECVFAX)
	{			
	    strcpy(pFaxHandle, "Recv");
	}
	
	if ( pFaxData->m_s32AcsEvtState == 1)
	{
	    sprintf ( StateStr, "%s(Fax:%d,%d)(Trunk:%d,%d):传真成功完成。St = %d, Err=%d, ErrStep=%d", pFaxHandle,
		pOneFax->deviceID.m_s8ModuleID, pOneFax->deviceID.m_s16ChannelID,
		M_OneTrunk(pOneFax->UsedDevID).deviceID.m_s8ModuleID, M_OneTrunk(pOneFax->UsedDevID).deviceID.m_s16ChannelID,
		pFaxData->m_s32AcsEvtState, pFaxData->m_s32AcsEvtErrCode, pFaxData->m_u8ErrorStep );
	}
	else
	{
	    if (iEvtType == XMS_EVT_SENDFAX)
	    {
		g_iTotalSendFail++;
		M_OneTrunk(pOneFax->UsedDevID).u32FaxSendFail++;
	    }
	    else if (iEvtType == XMS_EVT_RECVFAX)
	    {
		g_iTotalRecvFail++;
		M_OneTrunk(pOneFax->UsedDevID).u32FaxRecvFail++;
	    }			
	    
	    sprintf ( StateStr, "%s(Fax:%d,%d)(Trunk:%d,%d):传真发生错误。St=%d, Err=%d, ErrStep=%d", pFaxHandle,
		pOneFax->deviceID.m_s8ModuleID, pOneFax->deviceID.m_s16ChannelID,
		M_OneTrunk(pOneFax->UsedDevID).deviceID.m_s8ModuleID, M_OneTrunk(pOneFax->UsedDevID).deviceID.m_s16ChannelID,
		pFaxData->m_s32AcsEvtState, pFaxData->m_s32AcsEvtErrCode, pFaxData->m_u8ErrorStep );
	    
	    WriteTestLog(StateStr);
	}
    }
    
    pdlg->m_ListFax.SetItemText(iDispRow, 4, StateStr ); 
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
    lvc.pszText = "Fax/Open";
    lvc.cx = 70;
    pdlg->m_ListCount.InsertColumn(6, &lvc);
    
    lvc.iSubItem = 7;
    lvc.pszText = "FaxFree";
    lvc.cx = 55;
    pdlg->m_ListCount.InsertColumn(7, &lvc);
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

void    DrawCount_Fax(DJ_S8   s8ModID)
{
    char    TmpStr[256];
    int     iDispRow;
    
    iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;
    
    sprintf(TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lFaxNum, AllDeviceRes[s8ModID].lFaxOpened);
    pdlg->m_ListCount.SetItemText(iDispRow, 6, TmpStr);
    
    sprintf(TmpStr, "%3d", AllDeviceRes[s8ModID].lFaxFreeNum);
    pdlg->m_ListCount.SetItemText(iDispRow, 7, TmpStr);
    
    // total
    iDispRow = 0;
    sprintf(TmpStr, "%3d/%3d", g_iTotalFax, g_iTotalFaxOpened);
    pdlg->m_ListCount.SetItemText(iDispRow, 6, TmpStr);
    
    sprintf(TmpStr, "%3d", g_iTotalFaxFree);
    pdlg->m_ListCount.SetItemText(iDispRow, 7, TmpStr);
}


// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
    char	TmpStr[256];
    int		i, iDispRow;
    TRUNK_STRUCT	*pOneTrunk;
    FAX_STRUCT      *pOneFax;
    
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
    
    // m_ListFax
    pdlg->m_ListFax.DeleteAllItems ();
    for ( i = 0; i < g_iTotalFax; i ++ )
    {
	// 固定不变的内容
	sprintf ( TmpStr, "%3d", i );
	pdlg->m_ListFax.InsertItem ( i, TmpStr );
	
	sprintf ( TmpStr, "%d,%d", MapTable_Fax[i].m_s8ModuleID, MapTable_Fax[i].m_s16ChannelID );
	pdlg->m_ListFax.SetItemText ( i, 1, TmpStr );
	
	pOneFax = &M_OneFax(MapTable_Fax[i]);
	// 可能改变的内容
	DrawFax_LineState ( pOneFax );
	DrawFax_State ( pOneFax );
    }
    
    // m_ListCount
    pdlg->m_ListCount.DeleteAllItems();
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
    
    pdlg = (CXMSTest_FaxDialDlg	*)theApp.m_pMainWnd;
    
    // Read From "XMS_Dial.INI"
    ReadFromConfig();
    
    // Init m_ListMain
    InitListMain();
    
    // Init m_ListPcm
    InitListPcm();
    
    // Init m_ListMsg
    InitListMsg();
    
    // Init m_ListCount
    InitListCount();
    
    // Init m_ListFax
    InitListFax();
    
    // Init Conifg's text box
    InitTextBox();
    
    InitializeCriticalSection(&CriticalSection); 
    
    // now open ACS Server
    /*调用acsOpenStream 建立与ACS服务器的连接*/
    r = XMS_acsOpenStream ( &g_acsHandle, 
	&cfg_ServerID,
	g_u8UnitID,		// App Unit ID 
	128,				// sendQSize, in K Bytes
	128,				// recvQSize, in K Bytes
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
    
    // save to "XMS_Dial.INI"
    FetchFromText();
    WriteToConfig();
    
    DeleteCriticalSection(&CriticalSection);
}

// --------------------------------------------------------------------------------
void	OpenTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
    RetCode_t	r;
    
    pOneTrunk->State= TRK_WAITOPEN;
    
    r = XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
    if ( r < 0 )
    {
	AddMsg ( "XMS_ctsOpenDevice Fail in OpenTrunkDevice()!" );
    }
}

void	OpenVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
    RetCode_t	r;
    
    pOneVoice->State= VOC_WAITOPEN;
    
    r = XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
    if ( r < 0 )
    {
	AddMsg ( "XMS_ctsOpenDevice Fail in OpenVoiceDevice()!" );
    }
}

void    OpenFaxDevice(FAX_STRUCT *pOneFax)
{
    RetCode_t   r;
    char   TmpStr[100] = "";
    
    pOneFax->State = FAX_WAITOPEN;
    
    r = XMS_ctsOpenDevice(g_acsHandle, &pOneFax->deviceID, NULL);
    if ( r < 0)
    {
	sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenFaxDevice()! r=%d", r);
	AddMsg(TmpStr);
    }
}

void	OpenPcmDevice ( PCM_STRUCT *pOnePcm )
{
    RetCode_t	r;
    
    r = XMS_ctsOpenDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
    if ( r < 0 )
    {
	AddMsg ( "XMS_ctsOpenDevice Fail in OpenPcmDevice()!" );
    }
}

void	OpenBoardDevice ( DeviceID_t *pBoardDevID )
{
    RetCode_t	r;
    
    r = XMS_ctsOpenDevice ( g_acsHandle, pBoardDevID, NULL );
    
    if ( r < 0 )
    {
	AddMsg ( "XMS_ctsOpenDevice Fail in OpenBoardDevice()!" );
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
    
    // pFax
    for (i = 0; i < AllDeviceRes[s8DspModID].lFaxNum; i++)
    {
	OpenFaxDevice(&AllDeviceRes[s8DspModID].pFax[i]);
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
    FAX_STRUCT      *pOneFax;
//	CmdParamData_Voice_t vocParam = {0};
    
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
	
	XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
	XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

	CmdParamData_Voice_t cmdv={0};
	
	memset(&cmdv,0,sizeof(cmdv));
	cmdv.m_u8GtdCtrlValid = 1;
	XMS_ctsSetParam(g_acsHandle,pDevice,0,sizeof(cmdv),(void *)&cmdv);
	
	g_iTotalVoiceOpened ++;
	g_iTotalVoiceFree ++;
	AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
	AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;
	
	DrawCount_Voc ( pDevice->m_s8ModuleID );
    }
    
    if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_FAX)
    {
	pOneFax = &M_OneFax(*pDevice);
	pOneFax->deviceID.m_CallID = pDevice->m_CallID;
	
	XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
	XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );		
	
	pOneFax->State = FAX_FREE;
	
	DrawFax_State( &M_OneFax(*pDevice) );
	
	g_iTotalFaxOpened++;
	g_iTotalFaxFree++;
	AllDeviceRes[pDevice->m_s8ModuleID].lFaxOpened++;
	AllDeviceRes[pDevice->m_s8ModuleID].lFaxFreeNum++;
	
	DrawCount_Fax(pDevice->m_s8ModuleID);
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
    FAX_STRUCT      *pOneFax;
    Acs_GeneralProc_Data *pGeneralData = NULL;
    
    pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
    if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
    {
	pOneTrunk = &M_OneTrunk(pAcsEvt->m_DeviceID);
	
	pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
	DrawMain_LineState( pOneTrunk );		
    }
    
    if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_FAX)
    {
	pOneFax = &M_OneFax(pAcsEvt->m_DeviceID);
	
	pOneFax->iLineState = pGeneralData->m_s32DeviceState;
	DrawFax_LineState(pOneFax);
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
    pOneTrunk->State = NewState;
    
    DrawMain_State ( pOneTrunk );
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
    pOneTrunk->u32FaxCount = 0;
    pOneTrunk->u32FaxRecvFail = 0;
    pOneTrunk->u32FaxSendFail = 0;
}

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
    Change_State ( pOneTrunk, TRK_FREE );
    
    memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配Voc Device
    My_InitDtmfBuf ( pOneTrunk );
    
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
DJ_S32	 RecordFile ( DeviceID_t *pVocDevID, DeviceID_t *pVocDevID_ForRecord, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend )
{	
    DJ_U32				i = 0;	
    RecordProperty_t	recordProperty;
    RetCode_t			r;	
    
    memset(&recordProperty,0,sizeof(RecordProperty_t));		
    
    recordProperty.m_u32MaxSize = u32RecSize;
    recordProperty.m_s8IsMixEnable = 1;
    recordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
    recordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
    recordProperty.m_MixerControl.m_u16SRC_ChID1 = pVocDevID->m_s16ChannelID;
    recordProperty.m_MixerControl.m_u16SRC_ChID2 = pVocDevID_ForRecord->m_s16ChannelID;
    
    if ( bIsAppend )
	recordProperty.m_s8IsAppend = 1;
    else
	recordProperty.m_s8IsAppend = 0;
    
    strcpy (recordProperty.m_s8FileName, s8FileName );
    
    r = XMS_ctsRecord ( g_acsHandle, pVocDevID_ForRecord, &recordProperty, NULL );
    
    return r;
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

int		SearchOneFreeFax ( DeviceID_t *pFreeFaxDeviceID )
{
    int				i;
    static	int		iLoopStart = 0;
    FAX_STRUCT		*pOneFax;
    DJ_S8			s8SearchModID;
    long			lNowMaxFreeNum;
    
    // Search in Max free resource module
    {
	s8SearchModID = -1;
	lNowMaxFreeNum = -1;
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
	    if ( AllDeviceRes[MapTable_Module[i]].lFaxFreeNum > lNowMaxFreeNum )
	    {
		s8SearchModID = MapTable_Module[i];
		lNowMaxFreeNum = AllDeviceRes[MapTable_Module[i]].lFaxFreeNum;
	    }
	}
    }
    
    for ( i = 0; i < AllDeviceRes[s8SearchModID].lFaxNum; i ++ )
    {
	pOneFax = &AllDeviceRes[s8SearchModID].pFax[i];
	if ( pOneFax->State == FAX_FREE )
	{
	    *pFreeFaxDeviceID = pOneFax->deviceID;
	    
	    // 占用该fax device 
	    pOneFax->State = FAX_USED;
	    DrawFax_State( pOneFax );
	    
	    AllDeviceRes[s8SearchModID].lFaxFreeNum--;
	    g_iTotalFaxFree --;
	    
	    DrawCount_Fax ( s8SearchModID );
	    return i;
	}
    }
    
    return -1;
}

int		FreeOneFax (  DeviceID_t *pFreeFaxDeviceID )
{
    DJ_S8	s8ModID;
    
    s8ModID = pFreeFaxDeviceID->m_s8ModuleID;
    if ( AllDeviceRes[s8ModID].lFlag == 1 )
    {
	M_OneFax(*pFreeFaxDeviceID).State = FAX_FREE;
	DrawFax_State( &M_OneFax(*pFreeFaxDeviceID) );
	
	AllDeviceRes[s8ModID].lFaxFreeNum++;
	g_iTotalFaxFree ++;
	
	DrawCount_Fax ( s8ModID );
	return	0;		// OK
    }
    
    return -1;			// invalid VocDeviceID
}

int		SearchOneFreeVoice (  DeviceID_t *pFreeVocDeviceID, SEARCH_RULE SearchRule, DJ_S8 s8RefModID/*, DJ_S16 s16RefChID*/ )
{
    int				i;
    static	int		iLoopStart = 0;
    VOICE_STRUCT	*pOneVoice;
    DJ_S8			s8SearchModID;
    long			lNowMaxFreeNum;
    //	DJ_S8           s8DspModID;
    //	DJ_S16          s16ChannelID;
    
    /*	if (SearchRule == SEARCH_RULE_FIX_MODULE)
    {
    if ( s16RefChID < AllDeviceRes[s8RefModID].lVocNum )
    {
    pOneVoice = &AllDeviceRes[s8RefModID].pVoice[s16RefChID];
    if ( pOneVoice->State != VOC_WAITOPEN )
    {
				*pFreeVocDeviceID = pOneVoice->deviceID;
				
				  // 占用该voice device 
				  pOneVoice->State = VOC_USED;
				  AllDeviceRes[s8RefModID].lVocFreeNum--;
				  g_iTotalVoiceFree --;
				  DrawCount_Voc ( s8RefModID );
				  return i = s16RefChID;
				  }
				  }
				  return -1;
				  
				    }
    else */
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
	    
	    // 占用该voice device 
	    AllDeviceRes[s8SearchModID].pVoice[i].State = VOC_USED;
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
	M_OneVoice(*pFreeVocDeviceID).State = VOC_FREE;
	
	AllDeviceRes[s8ModID].lVocFreeNum++;
	g_iTotalVoiceFree ++;
	DrawCount_Voc ( s8ModID );
	return	0;		// OK
    }
    
    return -1;			// invalid VocDeviceID
}

void ResetTrunk ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
    // free the used Voice Resource
    if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
    {
	My_DualUnlink ( &pOneTrunk->deviceID, &pOneTrunk->VocDevID );
	FreeOneVoice ( &pOneTrunk->VocDevID );
	
	memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	DrawMain_VocInfo ( pOneTrunk );
    }
    
    // free the used Fax Resource
    if ( pOneTrunk->FaxDevID.m_s16DeviceMain != 0 )
    {
	// stop send or receive fax
	if (pOneTrunk->State == TRK_FAX_SEND)
	{
	    XMS_ctsStopSendFax ( g_acsHandle, &pOneTrunk->FaxDevID, NULL );
	}
	else if (pOneTrunk->State == TRK_FAX_RECEIVE)
	{
	    XMS_ctsStopReceiveFax ( g_acsHandle, &pOneTrunk->FaxDevID, NULL );
	}
	
	// free the fax device
	FreeOneFax(&pOneTrunk->FaxDevID);
	
	memset ( &M_OneFax(pOneTrunk->FaxDevID).UsedDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	memset ( &pOneTrunk->FaxDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	DrawMain_FaxInfo ( pOneTrunk );
    }
    
    InitTrunkChannel ( pOneTrunk );
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

DJ_S32 RecvFax(TRUNK_STRUCT *pOneTrunk)
{
    RetCode_t r = -1;
    char     chTmpFilePath[128] = "";
    char     FileName[256] = "";
    char     MsgStr[256] = "";
    
    if ((pOneTrunk->VocDevID.m_s8ModuleID >0)&&(pOneTrunk->FaxDevID.m_s8ModuleID > 0))
    {
	GetFilePathName(pOneTrunk, chTmpFilePath);	
	sprintf ( FileName, "%s\\XMS_FaxDial_%03d.tif", chTmpFilePath, pOneTrunk->iSeqID );
	
	DrawFax_FilePath(&M_OneFax(pOneTrunk->FaxDevID), FileName);
	
	r = XMS_ctsReceiveFax ( g_acsHandle, 
	    &pOneTrunk->FaxDevID,
	    &pOneTrunk->VocDevID,
	    FileName, "DJ83636988", NULL );
	if ( r < 0 )
	{
	    sprintf ( MsgStr, "XMS_ctsReceiveFax ( %s ) Fail! ret=%d", FileName, r);
	    AddMsg ( MsgStr );
	}
	
	Change_State ( pOneTrunk, TRK_FAX_RECEIVE );
    }
    
    return r;
}

DJ_S32   SendFax(TRUNK_STRUCT *pOneTrunk)
{
    char    FileName[256] = "";
    char    chTmpFilePath[128] = "";
    char    MsgStr[256] = "";
    RetCode_t r = -1;
    
    if ((pOneTrunk->VocDevID.m_s8ModuleID >0)&&(pOneTrunk->FaxDevID.m_s8ModuleID > 0))
    {		
	sprintf(FileName, "%s\\sendfax%d.tif", cfg_SendFaxPath, (pOneTrunk->iSeqID)%4+1);
	sprintf(FileName, "%s\\sendfax1.tif", cfg_SendFaxPath);
	DrawFax_FilePath(&M_OneFax(pOneTrunk->FaxDevID), FileName);
	
	r = XMS_ctsSendFax(g_acsHandle, 
	    &pOneTrunk->FaxDevID,
	    &pOneTrunk->VocDevID,
	    FileName, "DJ83636987", NULL );
	if ( r < 0 )
	{
	    sprintf ( MsgStr, "XMS_ctsSendFax ( %s ) Fail! ret=%d", FileName, r);
	    AddMsg ( MsgStr );
	    WriteMsgLog(MsgStr);
	}
	
	Change_State (pOneTrunk, TRK_FAX_SEND);
    }	
    
    return r;
}

bool  BindOneFaxResToTrunk(TRUNK_STRUCT *pOneTrunk)
{
    bool   bBindOk = false;
    
    DeviceID_t FreeFaxDeviceID;
    DeviceID_t FreeVocDeviceID_ForFax;
    
    if ((SearchOneFreeFax(&FreeFaxDeviceID)) >= 0)
    {
	DrawFax_DetailInfo ( &M_OneFax(FreeFaxDeviceID), NULL, 0);
	
	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForFax, SEARCH_RULE_SAME_MODULE, FreeFaxDeviceID.m_s8ModuleID))>=0)
	    //	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForFax, SEARCH_RULE_FIX_MODULE, FreeFaxDeviceID.m_s8ModuleID, FreeFaxDeviceID.m_s16ChannelID))>=0)
	{
	    DrawFax_State(&M_OneFax(FreeFaxDeviceID));
	    pOneTrunk->VocDevID = FreeVocDeviceID_ForFax;
	    M_OneVoice(FreeVocDeviceID_ForFax).UsedDevID = pOneTrunk->deviceID;
	    pOneTrunk->FaxDevID = FreeFaxDeviceID;
	    M_OneFax(FreeFaxDeviceID).UsedDevID = pOneTrunk->deviceID;
	    
	    My_DualLink(&pOneTrunk->deviceID, &pOneTrunk->VocDevID);
	    DrawMain_VocInfo(pOneTrunk);
	    DrawMain_FaxInfo(pOneTrunk);
	    
	    bBindOk = true;
	}
    }
    
    return (bBindOk);
}

void DrawOneTrunkResult(TRUNK_STRUCT *pOneTrunk)
{
    char   TmpStr[500] = {0};
    
    if (pOneTrunk == NULL)
	return;
    
    sprintf(TmpStr, "%d/%d", pOneTrunk->u32FaxCount, pOneTrunk->u32FaxSendFail);
    pdlg->m_ListMain.SetItemText(pOneTrunk->iSeqID, 9, TmpStr);
    
    sprintf(TmpStr, "%d/%d", pOneTrunk->u32FaxCount, pOneTrunk->u32FaxRecvFail);
    pdlg->m_ListMain.SetItemText(pOneTrunk->iSeqID, 10, TmpStr);
    
    
    /*	if ((g_iTotalCall>=MAXLONGLONG) ||
    ( (g_iTestStartTime!=0)&&((GetTickCount() - g_iTestStartTime)>=cfg_iTestTime*60*60*1000) ) )
    {
    if (pOneTrunk->State == DCS_FREE)
    {
    Change_State(pOneTrunk, TRK_WAITSTOP);	
    }		
    }
    */
}

void DrawAllTrunkResult()
{
    char   MsgStr[100] = {0};	
    
    memset(MsgStr, 0, 100 );
    sprintf(MsgStr, "%ld", g_iTotalSendFail);
    pdlg->GetDlgItem(IDC_STATIC_SendStat)->SetWindowText(MsgStr);
    
    sprintf(MsgStr, "%ld", g_iTotalRecvFail);
    pdlg->GetDlgItem(IDC_STATIC_RecvStat)->SetWindowText(MsgStr);
    
    sprintf(MsgStr, "%ld", g_iTotalCall);
    pdlg->GetDlgItem(IDC_STATIC_Total)->SetWindowText(MsgStr);
    
    TRACE("TotalCall=%d, %d, %d\n",g_iTotalCall,g_iTotalSendFail,g_iTotalRecvFail);
    
    if (g_iTotalCall != 0)
    {
	memset(MsgStr, 0, sizeof(MsgStr));
	sprintf(MsgStr, "%f%%", ((float)((g_iTotalSendFail+g_iTotalRecvFail)*100))/(float)(g_iTotalCall*2));
	pdlg->GetDlgItem(IDC_STATIC_FailRate)->SetWindowText(MsgStr);
    }
}

void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
    Acs_CallControl_Data *pCallControl = NULL;
    Acs_FaxProc_Data	 *pFaxData = NULL;
    char                FileName[256] = "";	
    char                TmpFileName[512] = "";
    char                MsgStr[256] = "";
    bool                bRet = false;
    int                 r = -1;
    char                recordFile[256] = "";
    DeviceID_t          FreeVocDeviceID_ForRecord;
    
    DrawOneTrunkResult(pOneTrunk);
    
    if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL || pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )	/*拆线事件*/
    {
	if (pOneTrunk->State == TRK_WAITHANGUP)
	{
	    DrawFax_FilePath(&M_OneFax(pOneTrunk->FaxDevID), "");
	    
	    GetFilePathName(pOneTrunk, TmpFileName);
	    sprintf (FileName, "%s\\XMS_FaxDial_%03d.tif", TmpFileName, pOneTrunk->iSeqID);
	    
	    if (cfg_s32Record == 1)
	    {
		StopRecordFile(&pOneTrunk->VocDevID_ForRec);
		XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->VocDevID, &pOneTrunk->VocDevID_ForRec, NULL);
		FreeOneVoice(&pOneTrunk->VocDevID_ForRec);
		memset(&pOneTrunk->VocDevID_ForRec, 0, sizeof(DeviceID_t));		
	    }
	}
	
	g_iCallCount--;
	ResetTrunk ( pOneTrunk, pAcsEvt );	
	Change_State (pOneTrunk, TRK_FREE);
	
	return ; 
    }
    
    switch(pOneTrunk->State)
    {
    case TRK_FREE:
	break;
	
    case TRK_CALLOUT:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
	{
	    pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
	    
	    XMS_ctsSetDevTimer(g_acsHandle, &pOneTrunk->deviceID, 6000*60);
	    
	    if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
	    {				
		if ((bRet = BindOneFaxResToTrunk(pOneTrunk)))
		{
		/*	CmdParamData_Voice_t cmdv={0};
			memset(&cmdv,0,sizeof(cmdv));
			cmdv.m_u8GtdCtrlValid = 1;
			XMS_ctsSetParam(g_acsHandle,&pOneTrunk->VocDevID,0,sizeof(cmdv),(void *)&cmdv);
*/
		    if (cfg_s32Record == 1)
		    {
			if ((SearchOneFreeVoice(&FreeVocDeviceID_ForRecord, SEARCH_RULE_SAME_MODULE, pOneTrunk->VocDevID.m_s8ModuleID))>=0)
			    //	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForRecord, SEARCH_RULE_FIX_MODULE, pOneTrunk->VocDevID.m_s8ModuleID, pOneTrunk->VocDevID.m_s16ChannelID + 30))>=0)
			{
			    XMS_ctsLinkDevice(g_acsHandle, &pOneTrunk->VocDevID, &FreeVocDeviceID_ForRecord, NULL);
			    pOneTrunk->VocDevID_ForRec = FreeVocDeviceID_ForRecord;
			    M_OneVoice(FreeVocDeviceID_ForRecord).UsedDevID = pOneTrunk->deviceID;
			    DrawMain_VocInfo(pOneTrunk);
			    
			    GetFilePathName(pOneTrunk, TmpFileName);
			    sprintf (FileName, "%s\\XMS_FaxDial_%03d_%3d.pcm", TmpFileName, pOneTrunk->iSeqID, pOneTrunk->u32FaxCount);						
			    RecordFile(&pOneTrunk->VocDevID, &FreeVocDeviceID_ForRecord, FileName, 8000L*800, false);
			}
		    }
		    pOneTrunk->u32FaxCount++;
		    g_iTotalCall++;									
		}
		if( cfg_sIsSendFax )
		{
		    
		    r = SendFax(pOneTrunk);
		}
		else
		{
		    r = RecvFax(pOneTrunk);
		}
		if( r< 0 )
		{
		    XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
		    Change_State ( pOneTrunk, TRK_WAITHANGUP );
		    
		    sprintf ( MsgStr, "%s (%d, %d) Fail!", cfg_sIsSendFax?"SendFax":"RecvFax",pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID);
		    AddMsg ( MsgStr );
		    WriteMsgLog(MsgStr);
		}
	    }
	    else		// 呼出失败
	    {
		DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
		Change_State ( pOneTrunk, TRK_FREE );
	    }		
	}
	break;
	
    case TRK_FAX_SEND:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_SENDFAX )	/* Send Fax 完成事件*/
	{
	    pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);
	    
	    DrawFax_DetailInfo ( &M_OneFax(pOneTrunk->FaxDevID), pFaxData, XMS_EVT_SENDFAX );
//		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
	    
	    Change_State (pOneTrunk, TRK_WAITHANGUP);
	}
	break;		
	
    case TRK_FAX_RECEIVE:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVFAX )	/* Receive Fax 完成事件*/
	{
	    pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);
	    
	    DrawFax_DetailInfo ( &M_OneFax(pOneTrunk->FaxDevID), pFaxData, XMS_EVT_RECVFAX );
	    XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
	    
	    Change_State (pOneTrunk, TRK_WAITHANGUP);
	}
	break;
    }
}


int CalDispSeq(int iDspRow)
{
    int ret;	
    
    return ret = (iDspRow / 15) + 1 + iDspRow;
}

bool  IsStopAllTrunk(int iTotalTrunk)
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
	    continue;
	
	if (IsDigitTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
	{
	    pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];
	    
	    if(pOneTrunk->State != TRK_WAITSTOP)
		return false;		
	}
	
	Sleep(10);
    }
    
    return true;
}

bool IsStopTime(int iTotalTrunk)
{
    DJ_U8 s8ModID;
    DJ_U16 s16ChID;
    TRUNK_STRUCT   *pOneTrunk;
    int i;
    
    if (((GetTickCount()-g_iTestStartTime)>=(cfg_iTestTime*60*60*1000))
	|| (g_iTotalCall>=MAXLONGLONG))
    {
	for (i = 0; i < iTotalTrunk; i++)
	{
	    s8ModID = MapTable_Trunk[i].m_s8ModuleID;
	    s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
	    
	    if (AllDeviceRes[s8ModID].pTrunk == NULL)
		continue;
	    
	    pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];	
	    
	    if (IsDigitTrunk(pOneTrunk->deviceID.m_s16DeviceSub)
		&& (pOneTrunk->iLineState != DCS_FREE))
	    {
		return false;
	    }
	}
	return true;
    }
    else
    {
	return false;
    }
}

void DrawTotalTestTime()
{
    char  TmpStr[100] = {0};
    LONGLONG lTotlalTime = 0;
    int     iH = 0;
    int     iM = 0;
    int     iS = 0;
    
    lTotlalTime = (GetTickCount() - g_iTestStartTime)/1000;
    
    iH = (int)lTotlalTime/3600;
    iM = (int)(lTotlalTime - iH*3600)/60;
    iS = (int)(lTotlalTime - iH*3600 - iM * 60);
    sprintf(TmpStr, "%02d:%02d:%02d", iH, iM, iS);	
    
    pdlg->GetDlgItem(IDC_STATIC_TestTimer)->SetWindowText(TmpStr);
}

DWORD WINAPI ThreadFunc(VOID) 
{ 
    int             i, r = -1;
    bool            bRet = false;
    RetCode_t       ret;
    TRUNK_STRUCT	*pOneTrunk = NULL;	
    char			MsgStr[256];
    DJ_S8           s8ModID;
    DJ_S16          s16ChID;
    int             iTotalTrunk;
    int             iTotalFaxFree;
    int             iTotalFax;
    int             iSeqID;
    static int      iCount = -1;	
    int             iCallCount = 0;
    
    WriteTestLog("Test Start");
    g_iTestStartTime = GetTickCount();
    
    pdlg->GetDlgItem(IDC_EDIT_Start)->EnableWindow(FALSE);
    
	// close all voc GTD
/*	for(int dspID= 0;dspID< g_iTotalModule;dspID++)
	{
		for(int vocID= 0;vocID< g_iTotalVoiceOpened;vocID++)
		{
			CmdParamData_Voice_t cmdv={0};
			memset(&cmdv,0,sizeof(cmdv));
			cmdv.m_u8GtdCtrlValid = 1;
			XMS_ctsSetParam(g_acsHandle,&AllDeviceRes[2].pVoice[vocID].deviceID,0,sizeof(cmdv),(void *)&cmdv);
		}
	}*/
    while(1)
    {	
	DrawTotalTestTime();
	
	EnterCriticalSection(&CriticalSection);
	
	iTotalTrunk = g_iTotalTrunk;
	iTotalFaxFree = g_iTotalFaxFree;
	iTotalFax = g_iTotalFax;
	iCallCount = g_iCallCount;
	
	DrawAllTrunkResult();
	
	//		if (IsStopAllTrunk(iTotalTrunk))
	if (IsStopTime(iTotalTrunk))
	{
	    WriteTestResult();		
	    pdlg->GetDlgItem(IDC_EDIT_Start)->EnableWindow(TRUE);
	    return 0;
	}		
	
	LeaveCriticalSection(&CriticalSection);	
	
	if ((iCallCount == 0) 
	    && ((GetTickCount()-g_iTestStartTime)<=(cfg_iTestTime*60*60*1000))
	    && (g_iTotalCall<=MAXLONGLONG))
	{
	    iCount++;
	    
	    for (i = 0; i < iTotalFax; i++)//iTotalFax
	    {
		EnterCriticalSection(&CriticalSection);						
		
		iSeqID = (CalDispSeq(i) + iCount * (iTotalFax/30)*32)%(iTotalTrunk);			
		
		s8ModID = MapTable_Trunk[iSeqID].m_s8ModuleID;
		s16ChID = MapTable_Trunk[iSeqID].m_s16ChannelID;	
		
		if (AllDeviceRes[s8ModID].pTrunk == NULL)
		    continue;
		
		pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];					
		
		if ((pOneTrunk->iLineState == DCS_FREE) 			
		    && (IsDigitTrunk(pOneTrunk->deviceID.m_s16DeviceSub)))
		{	
		    ret = XMS_ctsMakeCallOut(g_acsHandle, &pOneTrunk->deviceID, cfg_CallingNum, cfg_SimCalledNum, NULL);
		    if ( ret > 0 )
		    {	
			g_iCallCount++;
			Change_State(pOneTrunk, TRK_CALLOUT);				
		    }
		    else
		    {
			sprintf(MsgStr, "XMS_ctsMakeCallOut() FAIL! ret = %d", ret);
			AddMsg(MsgStr);
			WriteMsgLog(MsgStr);
		    }
		}			
		
		LeaveCriticalSection(&CriticalSection);
		Sleep(100);
	    }
	}	
	
	Sleep(1000);
    }
    
    return 0; 
}  

void   CreateFileDirector(DJ_S8 s8DspModID)
{
    char pchFileName[256];
    
    sprintf(pchFileName, "%s\\RecFax%d", cfg_SendFaxPath, s8DspModID);
    CreateDirectory(pchFileName,NULL);
}

bool GetFilePathName(TRUNK_STRUCT *pOneTrunk, char *pchFilePath)
{
    if (pchFilePath == NULL)
	return false;
    
    if (pOneTrunk->deviceID.m_s8ModuleID > 0)
    {
	sprintf(pchFilePath, "%s\\RecFax%d", cfg_RcvFaxPath, pOneTrunk->deviceID.m_s8ModuleID);		
    }
    
    return true;	
}

void	SimulateCallOut(void)
{
    DWORD dwThreadID = 0;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFunc, NULL, 0, &dwThreadID);  
}

