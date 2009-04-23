#include "stdafx.h"
#include <time.h>

#include "XMSTest_FaxDemo.h"
#include "XMSTest_FaxDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_FaxDemo_Sub.H"
#include "XMS_FaxDemo_Event.H"
#include "XMS_FaxDemo_String.H"

// -----------------------------------------------------------------------
// variable declaration
extern CXMSTest_FAXDemoApp theApp;
CXMSTest_FAXDemoDlg	*pdlg;

CRITICAL_SECTION CriticalSection;

// var about XMS_Demo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_Demo.INI";

ServerID_t		cfg_ServerID;
int				cfg_iCalledLen = 5;
char			cfg_RcvFaxPath[128];
int				cfg_iDispChnl = 30;
int             cfg_s32Record = 0;
//int				cfg_iVoiceRule;

int				cfg_iPartWorkModuleID[256] = {0};
char            cfg_chPartWorkModuleID[256] = {0};
int				cfg_s32SendFax;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 1;

// var in XMS_Demo_Event.CPP
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

// add by chenwy 2009-02-18
extern int                     g_iTotalVoip;
extern int                     g_iTotalVoipOpened;
extern int                     g_iTotalVoipFree;
extern TYPE_CHANNEL_MAP_TABLE  MapTable_Voip[MAX_TRUNK_NUM_IN_THIS_DEMO];
// add end

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

void   WriteMsgLog(char *pMsg)
{
    FILE   *fp;
    char   tmpTime[128], tmpDate[128];
    
    char   LogFileName[256] = {0}; 
    char   LogFileShortName[] = "\\FaxDemoLog.txt";
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
	
	sprintf(MsgStr, "%s %s: %s\n", tmpTime, tmpDate, pMsg);
	fwrite(MsgStr, strlen(MsgStr), 1, fp);
    }
    
    if (fp != NULL)
	fclose(fp);	
}

//--------------------------------------------------------------------
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
    
    cfg_iDispChnl = GetPrivateProfileInt ( "ConfigInfo", "DispChnl", 30, cfg_IniName);
    
    GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
    strncpy(strTmp, cfg_chPartWorkModuleID, sizeof(strTmp));	
    SplitStr2Int(strTmp, ",", cfg_iPartWorkModuleID);
    
    GetPrivateProfileString ( "ConfigInfo", "RcvFaxPath", "c:\\DJXMS\\Temp", cfg_RcvFaxPath, sizeof(cfg_RcvFaxPath), cfg_IniName);
    
    cfg_s32SendFax = GetPrivateProfileInt ( "ConfigInfo", "IsSendFax", 0, cfg_IniName);
    
    cfg_s32Record = GetPrivateProfileInt("ConfigInfo", "RecordSwitch", 0, cfg_IniName);
    
}

void	WriteToConfig(void)
{
    char	TmpStr[256];
    
    WritePrivateProfileString("ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName);
    
    sprintf (TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
    WritePrivateProfileString ( "ConfigInfo", "Port", TmpStr, cfg_IniName);	
    
    sprintf(TmpStr, "%d", cfg_iDispChnl);
    WritePrivateProfileString ( "ConfigInfo", "DispChnl", TmpStr, cfg_IniName);
    
    sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
    WritePrivateProfileString("ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);
    
    WritePrivateProfileString ( "ConfigInfo", "RcvFaxPath", cfg_RcvFaxPath, cfg_IniName);
    
    sprintf ( TmpStr, "%d", cfg_s32SendFax);
    WritePrivateProfileString ( "ConfigInfo", "DebugOn", TmpStr, cfg_IniName);
    
    sprintf(TmpStr, "%d", cfg_s32Record);
    WritePrivateProfileString("ConfigInfo", "RecordSwitch", TmpStr, cfg_IniName);

    sprintf(TmpStr, "%d", cfg_s32SendFax);
    WritePrivateProfileString("ConfigInfo", "IsSendFax", TmpStr, cfg_IniName);
}

void	InitTextBox(void)
{
    char	TmpStr[256];
    
    pdlg->GetDlgItem(IDC_EDIT_IPADDR)->SetWindowText(cfg_ServerID.m_s8ServerIp);
    
    sprintf(TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
    pdlg->GetDlgItem ( IDC_EDIT_IPPORT )->SetWindowText(TmpStr);
    
    
    sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_ModuleID))->SetWindowText(TmpStr);
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_RCVFAXPATH))->SetWindowText(cfg_RcvFaxPath);
    
    if ( cfg_s32SendFax == 0 )
	((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 0 ) ;
    else
	((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->SetCheck ( 1 ) ;
    
    if (cfg_s32Record == 0)
    {
	((CButton *)pdlg->GetDlgItem(IDC_CHECK_Record))->SetCheck(0);
    }
    else
    {
	((CButton *)pdlg->GetDlgItem(IDC_CHECK_Record))->SetCheck(1);
    } 
}

void	FetchFromText(void)
{
    char	TmpStr[256];
    
    pdlg->GetDlgItem (IDC_EDIT_IPADDR)->GetWindowText(cfg_ServerID.m_s8ServerIp, 30);
    
    pdlg->GetDlgItem (IDC_EDIT_IPPORT)->GetWindowText(TmpStr, 30);
    sscanf ( TmpStr, "%d", &cfg_ServerID.m_u32ServerPort);
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_RCVFAXPATH))->GetWindowText(cfg_RcvFaxPath, 128);
    
    ((CEdit *)pdlg->GetDlgItem(IDC_EDIT_ModuleID))->GetWindowText(cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID));
    
    if (((CButton *)pdlg->GetDlgItem (IDC_CHECK_DEBUG))->GetCheck() == 1)
    {
	cfg_s32SendFax = 1;
    }
    else
    {
	cfg_s32SendFax = 0;
    }
    
    if (((CButton *)pdlg->GetDlgItem(IDC_CHECK_Record))->GetCheck() == 1)
    {
	cfg_s32Record = 1;
    }
    else
    {
	cfg_s32Record = 0;
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

int CalDispRow ( int iSeqID )
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
/*
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
*/
void DrawMain_LineState( VOIP_STRUCT *pOneVoip )
{
    char StateStr[100];
    int	 iDispRow;
    
 //   iDispRow = CalDispRow(pOneVoip->iSeqID); 
  //  if ( iDispRow < 0 )	return;

	iDispRow = pOneVoip->iSeqID;
    
    // 
    GetString_LineState ( StateStr, pOneVoip->iLineState );
    pdlg->m_ListMain.SetItemText ( iDispRow, 3, StateStr ); 
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
/*
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
		  strcpy(StateStr,"空闲"); 
		  break ;	
		  
		    case TRK_FAX_SEND:
		    strcpy(StateStr,"发送传真");
		    break;
		    
		      case TRK_FAX_RECEIVE:
		      strcpy(StateStr,"接收传真");
		      break;
		      
			case TRK_WAIT_ANSWERCALL:
			strcpy(StateStr,"等摘机完成");
			break;
			
			  case TRK_WAITHANGUP:
			  strcpy(StateStr, "等待挂机");
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
						
						  pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 
						  
						    }
						    
						      void DrawMain_FaxInfo( TRUNK_STRUCT *pOneTrunk )
						      {
						      char StateStr[100];
						      int	 iDispRow;
						      DeviceID_t	*pDev;
						      
							iDispRow = CalDispRow(pOneTrunk->iSeqID); 
							if ( iDispRow < 0 )	return;
							
							  if ( pOneTrunk->FaxDevID.m_s16DeviceMain != 0 )
							  {
							  pDev = &pOneTrunk->FaxDevID;
							  sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
							  }
							  else
							  strcpy(StateStr, "");
							  
							    pdlg->m_ListMain.SetItemText ( iDispRow, 9, StateStr ); 
							    
							      }
*/
// -------------------------------------------------------------------------------------------------
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

void DrawFax_FilePath(FAX_STRUCT *pOneFax, char *pFilePathStr)
{
    int iDispRow;
    
    if (pOneFax == NULL)
	return;
    
    iDispRow = pOneFax->iSeqID;
    
    pdlg->m_ListFax.SetItemText(iDispRow, 5, pFilePathStr);
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
	    sprintf ( StateStr, "%s(%d,%d):传真成功完成。St = %d, Err = %d, ErrStep = %d", pFaxHandle, 
		pOneFax->deviceID.m_s8ModuleID, pOneFax->deviceID.m_s16ChannelID, 
		pFaxData->m_s32AcsEvtState, pFaxData->m_s32AcsEvtErrCode, pFaxData->m_u8ErrorStep );			
	}
	else
	{
	    // add by chenwy 2008-02-18
	    /*			if (iEvtType == XMS_EVT_SENDFAX)
	    {
	    M_OneTrunk(pOneFax->UsedDevID).u32FaxSendFail++;
	    }
	    else if (iEvtType == XMS_EVT_RECVFAX)
	    {
	    M_OneTrunk(pOneFax->UsedDevID).u32FaxRecvFail++;
	    }
	    */
	    if (iEvtType == XMS_EVT_SENDFAX)
	    {
		M_OneVoip(pOneFax->UsedDevID).u32FaxSendFail++;
	    }
	    else if (iEvtType == XMS_EVT_RECVFAX)
	    {
		M_OneVoip(pOneFax->UsedDevID).u32FaxRecvFail++;
	    }
	    // add end
	    sprintf ( StateStr, "%s(%d,%d):传真发生错误。St = %d, Err = %d, ErrStep = %d", pFaxHandle,
		pOneFax->deviceID.m_s8ModuleID, pOneFax->deviceID.m_s16ChannelID,
		pFaxData->m_s32AcsEvtState, pFaxData->m_s32AcsEvtErrCode, pFaxData->m_u8ErrorStep );
	    WriteMsgLog(StateStr);
	}
    }
    
    pdlg->m_ListFax.SetItemText ( iDispRow, 4, StateStr ); 
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
    lvc.cx = 40;
    pdlg->m_ListCount.InsertColumn ( 1, &lvc ); 
    
    lvc.iSubItem = 2;
    lvc.pszText = "Pcm/Open" ;
    lvc.cx = 70;
    pdlg->m_ListCount.InsertColumn ( 2, &lvc ); 
    
    lvc.iSubItem = 3;
    lvc.pszText = "Trk/Open" ;
    lvc.cx = 60;
    pdlg->m_ListCount.InsertColumn ( 3, &lvc ); 
    
    lvc.iSubItem = 4;
    lvc.pszText = "Voc/Open" ;
    lvc.cx = 60;
    pdlg->m_ListCount.InsertColumn ( 4, &lvc ); 
    
    lvc.iSubItem = 5;
    lvc.pszText = "VocFree" ;
    lvc.cx = 50;
    pdlg->m_ListCount.InsertColumn ( 5, &lvc ); 
    
    lvc.iSubItem = 6;
    lvc.pszText = "Fax/Open";
    lvc.cx = 60;
    pdlg->m_ListCount.InsertColumn(6, &lvc);
    
    lvc.iSubItem = 7;
    lvc.pszText = "FaxFree";
    lvc.cx = 50;
    pdlg->m_ListCount.InsertColumn(7, &lvc);
    // add by chenwy 2009-02-18
    lvc.iSubItem = 8;
    lvc.pszText = "Voip/Open";
    lvc.cx = 65;
    pdlg->m_ListCount.InsertColumn(8, &lvc);
    // add end
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
/*
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
*/
void	DrawCount_User ( DJ_S8	s8ModID )
{
    char	TmpStr[256];
    int		iDispRow;
    
    // total
    iDispRow = 0;
    sprintf ( TmpStr, "%3d/%3d", g_iTotalUser, g_iTotalUserOpened );
    pdlg->m_ListCount.SetItemText ( iDispRow, 6, TmpStr );
}

void    DrawCount_Fax(DJ_S8   s8ModID)
{
    char   TmpStr[256];
    int    iDispRow;
    
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

// add by chenwy 2009-02-18
void	DrawCount_Voip ( DJ_S8	s8ModID )
{
    char	TmpStr[256];
    int	iDispRow;
    
    iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;
    
    sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lVoipNum, AllDeviceRes[s8ModID].lVoipOpened );
    pdlg->m_ListCount.SetItemText ( iDispRow, 8, TmpStr );
    
    // total
    iDispRow = 0;
    sprintf ( TmpStr, "%3d/%3d", g_iTotalVoip, g_iTotalVoipOpened );
    pdlg->m_ListCount.SetItemText ( iDispRow, 8, TmpStr );
}
// add end

void	DrawCount_Voc(DJ_S8	s8ModID)
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

// -------------------------------------------------------------------------------------------------
/*void	ReDrawAll (void)
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
			DrawMain_CallInfo ( pOneTrunk );
			DrawMain_DTMF ( pOneTrunk );
			}
			
			  // m_ListFax
			  pdlg->m_ListFax.DeleteAllItems();
			  for (i = 0; i < g_iTotalFax; i++)
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
					DrawCount_Fax(MapTable_Module[i]);
					DrawCount_Voip(MapTable_Module[i]); // add by chenwy 2009-02-18
					}
}*/
// --------------------------------------------------------------------------------
bool	InitSystem()
{
    RetCode_t	r;
    char		MsgStr[160];
    
    pdlg = (CXMSTest_FAXDemoDlg	*)theApp.m_pMainWnd;
    
    // Read From "XMS_Demo.INI"
    ReadFromConfig();
    
    // Init m_ListMain
    InitListMain();
    
    // Init m_ListFax;
    InitListFax();
    
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
    
    // save to "XMS_Demo.INI"
    FetchFromText();
    WriteToConfig();
    
    DeleteCriticalSection(&CriticalSection);
}

// --------------------------------------------------------------------------------
/*
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
*/
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
    RetCode_t    r;
    
    pOneFax->State = FAX_WAITOPEN;
    
    r = XMS_ctsOpenDevice(g_acsHandle, &pOneFax->deviceID, NULL);
    if (r < 0)
    {
	AddMsg("XMS_ctsOpenDevice Fail in OpenFaxDevice()!");
    }
}

// add by chenwy 2009-02-18
void    OpenVoipDevice(VOIP_STRUCT *pOneVoip)
{
    RetCode_t    r;
    
    pOneVoip->iState = VOIP_WAITOPEN;
    
    r = XMS_ctsOpenDevice(g_acsHandle, &pOneVoip->deviceID, NULL);
    if (r < 0)
    {
	AddMsg("XMS_ctsOpenDevice Fail in OpenVoipDevice()!");
    }
}
// add end

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
    
    /*	// pTrunk
    for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
    {
    OpenTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
    }
    */
    // pVoice
    for ( i = 0; i < AllDeviceRes[s8DspModID].lVoipNum; i++ )
    {
	OpenVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
    }
    
    // pFax
    for (i=0; i<AllDeviceRes[s8DspModID].lFaxNum; i++)
    {
	OpenFaxDevice(&AllDeviceRes[s8DspModID].pFax[i]);
    }
    // add by chenwy 2009-02-18
    // pVoip
    for (i=0; i<AllDeviceRes[s8DspModID].lVoipNum; i++)
    {
	OpenVoipDevice(&AllDeviceRes[s8DspModID].pVoip[i]);
    }
    // add end
    // pPcm
    for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
    {
	OpenPcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
    }
}


// --------------------------------------------------------------------------------
/*
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
*/
void    CloseFaxDevice(FAX_STRUCT *pOneFax)
{
    RetCode_t    r;
    
    r = XMS_ctsCloseDevice(g_acsHandle, &pOneFax->deviceID, NULL);
    if (r < 0)
    {
	AddMsg("XMS_ctsCloseDevice Fail in CloseFaxDevice()!");
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
    /*	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
    {
    CloseTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
    }
    */
    // pVoice
    for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
    {
	CloseVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
    }
    
    // pFax
    for (i=0; i<AllDeviceRes[s8DspModID].lFaxNum; i++)
    {
	CloseFaxDevice(&AllDeviceRes[s8DspModID].pFax[i]);
    }
    
    // pPcm
    for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
    {
	ClosePcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
    }
}

// -------------------------------------------------------------------------------------------------
/*void	HandleDevState ( Acs_Evt_t *pAcsEvt )
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
*/
void	HandleDevState ( Acs_Evt_t *pAcsEvt )
{
    VOIP_STRUCT	*pOneVoip;
    PCM_STRUCT		*pOnePcm;
    FAX_STRUCT      *pOneFax;
    Acs_GeneralProc_Data *pGeneralData = NULL;
    
    pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
    if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP )
    {
	pOneVoip = &M_OneVoip(pAcsEvt->m_DeviceID);
	
	pOneVoip->iLineState = pGeneralData->m_s32DeviceState;
	DrawMain_LineState( pOneVoip );	

	if (pOneVoip->iLineState == DCS_FREE && pOneVoip->iState == VOIP_WAITHANGUP)
		pOneVoip->iState = VOIP_FREE;
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
// -----------------------------------------------------------------------------

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
/*
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
*/
/*录音处理*/
DJ_S32	 RecordFile ( VOIP_STRUCT *pOneVoip, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend )
{	
    DJ_U32				i = 0;	
    RecordProperty_t	recordProperty;
    RetCode_t			r;
    
    memset(&recordProperty,0,sizeof(RecordProperty_t));		
    
    recordProperty.m_u32MaxSize = u32RecSize;
    recordProperty.m_s8IsMixEnable = 1;
    recordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_IP;
    recordProperty.m_MixerControl.m_u16SRC_ChID1 = pOneVoip->deviceID.m_s16ChannelID;
    
    if ( bIsAppend )
	recordProperty.m_s8IsAppend = 1;
    else
	recordProperty.m_s8IsAppend = 0;
    
    strcpy ( recordProperty.m_s8FileName, s8FileName );
    
    r = XMS_ctsRecord ( g_acsHandle, &pOneVoip->VocDevID, &recordProperty, NULL );
    
    return r;
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

int SearchOneFreeVoice (  DeviceID_t *pFreeVocDeviceID, SEARCH_RULE SearchRule, DJ_S8 s8RefModID, DJ_S16 s16RefChID)
{
    int				i;
    static	int		iLoopStart = 0;
    VOICE_STRUCT	*pOneVoice;
    DJ_S8			s8SearchModID;
    long			lNowMaxFreeNum;
    
    if (SearchRule == SEARCH_RULE_FIX_MODULE)
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
    else if ( SearchRule == SEARCH_RULE_SAME_MODULE )			// Search in Same Module
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

int FreeOneVoice (  DeviceID_t *pFreeVocDeviceID )
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

int SearchOneFreeFax (DeviceID_t *pVoipDev,  DeviceID_t *pFreeFaxDeviceID )
{
    int				i;
    static	int		iLoopStart = 0;
    FAX_STRUCT		*pOneFax;
    DJ_S8			s8SearchModID;
    long			lNowMaxFreeNum;
    
    // Search in Max free resource module
  /*  {
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
    }*/
    
	s8SearchModID = pVoipDev->m_s8ModuleID;
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

int FreeOneFax (  DeviceID_t *pFreeFaxDeviceID )
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
/*
void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{
pOneTrunk->State = NewState;

  DrawMain_State ( pOneTrunk );
  }
  
    
      void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
      {
      Change_State ( pOneTrunk, TRK_FREE );
      
	pOneTrunk->CallerCode[0]=0;
	pOneTrunk->CalleeCode[0]=0;
	
	  My_InitDtmfBuf ( pOneTrunk );
	  DrawMain_DTMF ( pOneTrunk );
	  
	    memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// -1表示还没有分配Voc Device
	    DrawMain_VocInfo(pOneTrunk );
	    DrawMain_FaxInfo(pOneTrunk);
	    }
	    
	      void ResetTrunk ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
	      {	
	      // free the used Voice Resource
	      if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	      {
	      My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
	      
		FreeOneVoice (  &pOneTrunk->VocDevID );
		
		  memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		  memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
		  DrawMain_VocInfo ( pOneTrunk );		
		  }
		  
		    // free the used Fax Resource
		    if ( pOneTrunk->FaxDevID.m_s16DeviceMain != 0 )
		    {
		    // stop send or receive fax
		    if ( pOneTrunk->State == TRK_FAX_SEND )
		    {
		    XMS_ctsStopSendFax ( g_acsHandle, &pOneTrunk->FaxDevID, NULL );
		    }
		    else if ( pOneTrunk->State == TRK_FAX_RECEIVE )
		    {
		    XMS_ctsStopReceiveFax ( g_acsHandle, &pOneTrunk->FaxDevID, NULL );
		    }
		    
		      // free the fax device
		      FreeOneFax ( &pOneTrunk->FaxDevID );
		      
			memset ( &M_OneFax(pOneTrunk->FaxDevID).UsedDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
			memset ( &pOneTrunk->FaxDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
			DrawMain_FaxInfo ( pOneTrunk );
			}
			
			  InitTrunkChannel ( pOneTrunk );
			  }
*/
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

void   CreateFileDirector(DJ_S8 s8DspModID)
{
    char pchFileName[256];
    
    sprintf(pchFileName, "%s\\Rec%d", cfg_RcvFaxPath, s8DspModID);
    CreateDirectory(pchFileName,NULL);
}
/*
bool GetFilePathName(TRUNK_STRUCT *pOneTrunk, char *pchFilePath)
{
if (pchFilePath == NULL)
return false;

  if (pOneTrunk->deviceID.m_s8ModuleID > 0)
  {
		sprintf(pchFilePath, "%s\\Rec%d", cfg_RcvFaxPath, pOneTrunk->deviceID.m_s8ModuleID);		
		}
		
		  return true;	
		  }
		  
		    bool  BindOneFaxResToTrunk(TRUNK_STRUCT *pOneTrunk)
		    {
		    bool   bBindOk = false;
		    
		      DeviceID_t FreeFaxDeviceID;
		      DeviceID_t FreeVocDeviceID_ForFax;
		      
			if ((SearchOneFreeFax(&FreeFaxDeviceID)) >= 0)
			{
			DrawFax_DetailInfo ( &M_OneFax(FreeFaxDeviceID), NULL, 0);
			
			  //	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForFax, SEARCH_RULE_SAME_MODULE, FreeFaxDeviceID.m_s8ModuleID))>=0)
			  if ((SearchOneFreeVoice(&FreeVocDeviceID_ForFax, SEARCH_RULE_FIX_MODULE, FreeFaxDeviceID.m_s8ModuleID, FreeFaxDeviceID.m_s16ChannelID))>=0)
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
				
				  DJ_S32 RecvFax(TRUNK_STRUCT *pOneTrunk)
				  {
				  RetCode_t r;
				  char     chTmpFilePath[128] = "";
				  char     FileName[256] = "";
				  char     MsgStr[256] = "";
				  
				    if ((pOneTrunk->VocDevID.m_s8ModuleID >0)&&(pOneTrunk->FaxDevID.m_s8ModuleID > 0))
				    {
				    GetFilePathName(pOneTrunk, chTmpFilePath);	
				    sprintf ( FileName, "%s\\XMS_FaxDemo_%03d.tif", chTmpFilePath, pOneTrunk->iSeqID );
				    
				      DrawFax_FilePath(&M_OneFax(pOneTrunk->FaxDevID), FileName);
				      
					r = XMS_ctsReceiveFax ( g_acsHandle, 
					&pOneTrunk->FaxDevID,
					&pOneTrunk->VocDevID,
					FileName, "DJ83636988", NULL );
					if ( r < 0 )
					{
					sprintf ( MsgStr, "XMS_ctsReceiveFax ( %s ) Fail! ret=%d", FileName, r);
					AddMsg ( MsgStr );
					WriteMsgLog(MsgStr);
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
						GetFilePathName(pOneTrunk, chTmpFilePath);
						sprintf ( FileName, "%s\\XMS_FaxDemo_%03d.tif", chTmpFilePath, pOneTrunk->iSeqID );	
						
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
							
							  void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
							  {
							  Acs_CallControl_Data *	pCallControl = NULL;
							  DeviceID_t              FreeVocDeviceID_ForRecord;
							  Acs_FaxProc_Data        *pFaxData;
							  char					FileName[256];
							  char                    chTmpFilePath[256] = "";	
							  char                    MsgStr[256] = "";	
							  bool                    bRet = false;
							  int                     r;
							  
							    //	DrawOneTrunkFaxResult(pOneTrunk);
							    
							      if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	//拆线事件
							      {			
							      //删除传真文件
							      if (pOneTrunk->State == TRK_WAITHANGUP)
							      {
							      DrawFax_FilePath(&M_OneFax(pOneTrunk->FaxDevID), "");
							      
								GetFilePathName(pOneTrunk, chTmpFilePath);
								sprintf(FileName, "%s\\XMS_FaxDemo_%03d.tif", chTmpFilePath, pOneTrunk->iSeqID);
								
								  if (cfg_s32Record == 1)
								  {
								  StopRecordFile(&pOneTrunk->VocDevID_ForRec);
								  XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->VocDevID, &pOneTrunk->VocDevID_ForRec, NULL);
								  FreeOneVoice(&pOneTrunk->VocDevID_ForRec);
								  memset(&pOneTrunk->VocDevID_ForRec, 0, sizeof(DeviceID_t));
								  }		
								  }
								  
								    ResetTrunk ( pOneTrunk, pAcsEvt );
								    Change_State ( pOneTrunk, TRK_FREE );		
								    
								      return ; 
								      }
								      
									switch(pOneTrunk->State)
									{
									case TRK_FREE:
									if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	//呼入事件
									{
									pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
									
									  XMS_ctsAlertCall ( g_acsHandle, &pOneTrunk->deviceID, NULL );
									  
									    XMS_ctsAnswerCallIn ( g_acsHandle, &pOneTrunk->deviceID, NULL );
									    
									      sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
									      sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
									      DrawMain_CallInfo( pOneTrunk );
									      
										Change_State ( pOneTrunk, TRK_WAIT_ANSWERCALL );
										}
										break;
										
										  case TRK_WAIT_ANSWERCALL:
										  if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	//应答呼入完成事件
										  {
										  pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);				
										  
										    if ((bRet = BindOneFaxResToTrunk(pOneTrunk)))
										    {
										    if (cfg_s32Record == 1)
										    {
										    //	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForRecord, SEARCH_RULE_SAME_MODULE, pOneTrunk->VocDevID.m_s8ModuleID))>=0)
										    if ((SearchOneFreeVoice(&FreeVocDeviceID_ForRecord, SEARCH_RULE_FIX_MODULE, pOneTrunk->VocDevID.m_s8ModuleID, pOneTrunk->VocDevID.m_s16ChannelID + 30))>=0)
										    {
										    XMS_ctsLinkDevice(g_acsHandle, &pOneTrunk->VocDevID, &FreeVocDeviceID_ForRecord, NULL);
										    pOneTrunk->VocDevID_ForRec = FreeVocDeviceID_ForRecord;
										    M_OneVoice(FreeVocDeviceID_ForRecord).UsedDevID = pOneTrunk->deviceID;
										    DrawMain_VocInfo(pOneTrunk);
										    
										      GetFilePathName(pOneTrunk, chTmpFilePath);
										      sprintf (FileName, "%s\\XMS_FaxDial_%03d_%3d.pcm", chTmpFilePath, pOneTrunk->iSeqID, pOneTrunk->u32FaxCount);						
										      RecordFile(&pOneTrunk->VocDevID, &FreeVocDeviceID_ForRecord, FileName, 8000L*800, false);
										      }
										      }
										      
											r = RecvFax(pOneTrunk);	
											//	pOneTrunk->u32FaxCount++;				
											Change_State ( pOneTrunk, TRK_FAX_RECEIVE );
											}
											
											  if ((!bRet) ||(r < 0)) 
											  {
											  XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
											  Change_State ( pOneTrunk, TRK_WAITHANGUP );
											  
											    sprintf ( MsgStr, "BindOneFaxResToTrunk (%d, %d) Fail!", pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID);
											    AddMsg ( MsgStr );
											    WriteMsgLog(MsgStr);
											    }
											    
											      }
											      break;
											      
												case TRK_FAX_RECEIVE:
												if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVFAX )	//Recv fax完成事件
												{
												pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);
												
												  DrawFax_DetailInfo ( &M_OneFax(pOneTrunk->FaxDevID), pFaxData, XMS_EVT_RECVFAX);
												  
												    r = SendFax(pOneTrunk);
												    if (r < 0)
												    {
												    XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
												    Change_State ( pOneTrunk, TRK_WAITHANGUP );
												    
												      sprintf ( MsgStr, "SendFax (%d, %d) Fail!", pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID);
												      AddMsg ( MsgStr );
												      WriteMsgLog(MsgStr);
												      }
												      
													Change_State ( pOneTrunk, TRK_FAX_SEND );
													}	
													break;
													
													  case TRK_FAX_SEND:
													  if ( pAcsEvt->m_s32EventType == XMS_EVT_SENDFAX )	//Send fax完成事件
													  {
													  pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);
													  
													    DrawFax_DetailInfo ( &M_OneFax(pOneTrunk->FaxDevID), pFaxData, XMS_EVT_SENDFAX);
													    // 主动挂机
													    //		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			
													    
													      Change_State ( pOneTrunk, TRK_WAITHANGUP );
													      }
													      break;
													      }
													      }
*/
// add by chenwy 2008-02-18
bool GetFilePathName(VOIP_STRUCT *pOneVoip, char *pchFilePath)
{
    if (pchFilePath == NULL)
	return false;
    
    if (pOneVoip->deviceID.m_s8ModuleID > 0)
    {
	sprintf(pchFilePath, "%s\\Rec%d", cfg_RcvFaxPath, pOneVoip->deviceID.m_s8ModuleID);		
    }
    
    return true;	
}
/*停止放音*/
DJ_S32 StopPlayFile ( DeviceID_t *pVocDevID )
{
    RetCode_t		r;
    ControlPlay_t	controlPlay;
    
    controlPlay.m_u16ControlType = XMS_STOP_PLAY;
    
    r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );
    
    return r;
}

void DrawMain_State( VOIP_STRUCT *pOneVoip )
{
    char StateStr[100];
    int	 iDispRow;
    
   // iDispRow = CalDispRow(pOneVoip->iSeqID); 
  //  if ( iDispRow < 0 )	return;
	iDispRow = pOneVoip->iSeqID;
    //
    switch( pOneVoip->iState ) 
    {
    case VOIP_WAITOPEN:
	strcpy(StateStr,"Wait Open"); 
	break;
	
    case VOIP_FREE:		
	strcpy(StateStr,"空闲"); 
	break ;	
	
    case VOIP_FAX_SEND:
	strcpy(StateStr,"发送传真");
	break;
	
    case VOIP_FAX_RECEIVE:
	strcpy(StateStr,"接收传真");
	break;
	
    case VOIP_WAIT_ANSWERCALL:
	strcpy(StateStr,"等摘机完成");
	break;
	
    case VOIP_WAITHANGUP:
	strcpy(StateStr, "等待挂机");
	break;
	case VOIP_FAX_T38START:
	strcpy(StateStr, "T38启动");
	break;
    default:
	strcpy(StateStr,"other");
	break;
    }
    
    pdlg->m_ListMain.SetItemText ( iDispRow, 4, StateStr ); 
}

void	Change_State ( VOIP_STRUCT *pOneVoip, VOIP_STATE NewState )
{
    pOneVoip->iState = NewState;
    
    DrawMain_State ( pOneVoip );
}

void DrawMain_VocInfo( VOIP_STRUCT *pOneVoip )
{
/*	char StateStr[100];
int	 iDispRow;
DeviceID_t	*pDev1, *pDev2;

  iDispRow = CalDispRow(pOneVoip->iSeqID); 
  if ( iDispRow < 0 )	return;
  
    if ( pOneVoip->VocDevID.m_s16DeviceMain != 0 )
    {
    pDev1 = &pOneVoip->VocDevID;
    pDev2 = &pOneVoip->VocDevID_ForRec;
    sprintf ( StateStr, "(%d,%d)(%d,%d)", pDev1->m_s8ModuleID, pDev1->m_s16ChannelID,
    pDev2->m_s8ModuleID, pDev2->m_s16ChannelID); 
    }
    else
    strcpy(StateStr, "");
    
      pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 
    */
    char StateStr[100];
    int	 iDispRow;
    DeviceID_t	*pDev;
    
    if (pOneVoip == NULL || pdlg == NULL)
	return;
    
   // iDispRow = CalDispRow(pOneVoip->iSeqID);
	iDispRow = pOneVoip->iSeqID;
    
    if ( pOneVoip->VocDevID.m_s16DeviceMain != 0 )
    {
	pDev = &pOneVoip->VocDevID;
	sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
    }
    else
	strcpy(StateStr, "");
    
    pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 
    
}

void DrawMain_FaxInfo( VOIP_STRUCT *pOneVoip )
{
    char StateStr[100];
    int	 iDispRow;
    DeviceID_t	*pDev;
    
   // iDispRow = CalDispRow(pOneVoip->iSeqID); 
   // if ( iDispRow < 0 )	return;
	iDispRow = pOneVoip->iSeqID;
    
    if ( pOneVoip->FaxDevID.m_s16DeviceMain != 0 )
    {
	pDev = &pOneVoip->FaxDevID;
	sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
    }
    else
	strcpy(StateStr, "");
    
    pdlg->m_ListMain.SetItemText ( iDispRow, 9, StateStr ); 
    
}

void My_InitDtmfBuf ( VOIP_STRUCT *pOneVoip )
{
    pOneVoip->DtmfCount	= 0;
    pOneVoip->DtmfBuf[0]	= 0; 
}


void DrawMain_CallInfo( VOIP_STRUCT *pOneVoip )
{
/*	char StateStr[100];
int	 iDispRow;

  iDispRow = CalDispRow(pOneVoip->iSeqID); 
  if ( iDispRow < 0 )	return;
  
    strcpy ( StateStr, pOneVoip->addrCallee.tszTelNo );
    pdlg->m_ListMain.SetItemText ( iDispRow, 5, StateStr ); 
    
      strcpy ( StateStr, pOneVoip->addrCaller.tszTelNo );
      pdlg->m_ListMain.SetItemText ( iDispRow, 6, StateStr ); 
    */
    char StateStr[100];
    int	 iDispRow;
    
    if (pOneVoip == NULL || pdlg == NULL)
	return;
    
//    iDispRow = CalDispRow(pOneVoip->iSeqID); 
	iDispRow = pOneVoip->iSeqID;
    
    if (pOneVoip->iProtocol == 1)
	sprintf(StateStr, "%s", "H323");
    else
	sprintf(StateStr, "%s", "SIP");
    pdlg->m_ListMain.SetItemText(iDispRow, 5, StateStr);
    
    sprintf ( StateStr,"%s:%d / %s:%d", pOneVoip->addrCaller.tszAddr, pOneVoip->addrCaller.port,
	pOneVoip->addrCallee.tszAddr, pOneVoip->addrCallee.port);
    pdlg->m_ListMain.SetItemText ( iDispRow, 6, StateStr ); 
    
    sprintf ( StateStr,"%s / %s", pOneVoip->addrCaller.tszUserID, pOneVoip->addrCallee.tszUserID );
    pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr );
    
    sprintf ( StateStr,"%s / %s", pOneVoip->addrCaller.tszTelNo, pOneVoip->addrCallee.tszTelNo );
    pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr );
}

void DrawMain_DTMF( VOIP_STRUCT *pOneVoip )
{
    char StateStr[100];
    int	 iDispRow;
    
   // iDispRow = CalDispRow(pOneVoip->iSeqID); 
    //if ( iDispRow < 0 )	return;
		iDispRow = pOneVoip->iSeqID;
    
    strcpy(StateStr,pOneVoip->DtmfBuf);
    pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 
}

void	InitVoipChannel ( VOIP_STRUCT *pOneVoip )
{
    Change_State ( pOneVoip, VOIP_FREE );
    
    pOneVoip->addrCallee.tszTelNo[0]=0;
    pOneVoip->addrCaller.tszTelNo[0]=0;
    
    My_InitDtmfBuf ( pOneVoip );
    DrawMain_DTMF ( pOneVoip );
    
    memset ( &pOneVoip->VocDevID, 0, sizeof(DeviceID_t) );		// -1表示还没有分配Voc Device
    DrawMain_VocInfo(pOneVoip );
    DrawMain_FaxInfo(pOneVoip);
}

void ResetVoip ( VOIP_STRUCT *pOneVoip, Acs_Evt_t *pAcsEvt )
{
    if (pOneVoip == NULL || pAcsEvt == NULL)
	return ;
    // free the used Voice Resource
    if ( pOneVoip->VocDevID.m_s16DeviceMain != 0 )
    {
	FreeOneVoice (  &pOneVoip->VocDevID );
	
	memset ( &M_OneVoice(pOneVoip->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
	memset ( &pOneVoip->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	DrawMain_VocInfo ( pOneVoip );		
    }
    
    // free the used Fax Resource
    if ( pOneVoip->FaxDevID.m_s16DeviceMain != 0 )
    {
	// stop send or receive fax
	if ( pOneVoip->iState == VOIP_FAX_SEND )
	{
	    XMS_ctsStopSendFax ( g_acsHandle, &pOneVoip->FaxDevID, NULL );
	}
	else if ( pOneVoip->iState == VOIP_FAX_RECEIVE )
	{
	    XMS_ctsStopReceiveFax ( g_acsHandle, &pOneVoip->FaxDevID, NULL );
	}
	
	// free the fax device
	FreeOneFax ( &pOneVoip->FaxDevID );
	
	memset ( &M_OneFax(pOneVoip->FaxDevID).UsedDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	memset ( &pOneVoip->FaxDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	DrawMain_FaxInfo ( pOneVoip );
    }
    
    InitVoipChannel ( pOneVoip );
}

bool  BindOneFaxResToVoip(VOIP_STRUCT *pOneVoip)
{
    bool   bBindOk = false;
    
    DeviceID_t FreeFaxDeviceID;
    DeviceID_t FreeVocDeviceID_ForFax;
    
    if ((SearchOneFreeFax(&pOneVoip->deviceID, &FreeFaxDeviceID)) >= 0)
    {
	DrawFax_DetailInfo ( &M_OneFax(FreeFaxDeviceID), NULL, 0);
	
	//	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForFax, SEARCH_RULE_SAME_MODULE, FreeFaxDeviceID.m_s8ModuleID))>=0)
	if ((SearchOneFreeVoice(&FreeVocDeviceID_ForFax, SEARCH_RULE_SAME_MODULE, FreeFaxDeviceID.m_s8ModuleID, FreeFaxDeviceID.m_s16ChannelID))>=0)
	{
	    DrawFax_State(&M_OneFax(FreeFaxDeviceID));
	    pOneVoip->VocDevID = FreeVocDeviceID_ForFax;
	    M_OneVoice(FreeVocDeviceID_ForFax).UsedDevID = pOneVoip->deviceID;
	    pOneVoip->FaxDevID = FreeFaxDeviceID;
	    M_OneFax(FreeFaxDeviceID).UsedDevID = pOneVoip->deviceID;
	    
	//    My_DualLink(&pOneVoip->deviceID, &pOneVoip->VocDevID);
	    DrawMain_VocInfo(pOneVoip);
	    DrawMain_FaxInfo(pOneVoip);
	    
	    bBindOk = true;
	}
    }
    
    return (bBindOk);
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
    //	TRUNK_STRUCT	*pOneTrunk;
//hn add    
/*	CmdParamData_Voice_t cmdv={0};
	
	memset(&cmdv,0,sizeof(cmdv));
	cmdv.m_u8GtdCtrlValid = 1;
	XMS_ctsSetParam(g_acsHandle,pDevice,0,sizeof(cmdv),(void *)&cmdv);
//add end    
*/    if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
    {
	AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
    }
    
    /*	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
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
}*/
    
    if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
    {
	M_OneVoice(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
	
	// init this Device: Voice
	M_OneVoice(*pDevice).State = VOC_FREE;
	
	XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
	XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );

	//add for disable GTD
			CmdParamData_Voice_t cmdv={0};
			memset(&cmdv,0,sizeof(cmdv));
			cmdv.m_u8GtdCtrlValid = 1;
			XMS_ctsSetParam(g_acsHandle, pDevice, 0,sizeof(cmdv),(void *)&cmdv);

	// modify the count
	g_iTotalVoiceOpened ++;
	g_iTotalVoiceFree ++;
	AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
	AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;
	
	DrawCount_Voc ( pDevice->m_s8ModuleID );
    }
    
    if (pDevice->m_s16DeviceMain == XMS_DEVMAIN_FAX)
    {
	M_OneFax(*pDevice).deviceID.m_CallID = pDevice->m_CallID;
	
	XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
	XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);
	
	M_OneFax(*pDevice).State = FAX_FREE;
	DrawFax_State( &M_OneFax(*pDevice) );
	
	g_iTotalFaxOpened++;
	g_iTotalFaxFree++;
	AllDeviceRes[pDevice->m_s8ModuleID].lFaxOpened++;
	AllDeviceRes[pDevice->m_s8ModuleID].lFaxFreeNum++;
	DrawCount_Fax(pDevice->m_s8ModuleID);
    }
    // add by chenwy 2009-02-18
    if (pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP)
    {
	VOIP_STRUCT	*pOneVoip   = &M_OneVoip(*pDevice);
	M_OneVoip(*pDevice).deviceID.m_CallID = pDevice->m_CallID;
	
	XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
	XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);
	
	InitVoipChannel ( pOneVoip );
	
	g_iTotalVoipOpened++;
	g_iTotalVoipFree++;
	AllDeviceRes[pDevice->m_s8ModuleID].lVoipOpened++;
	AllDeviceRes[pDevice->m_s8ModuleID].lVoipFreeNum++;
	DrawCount_Voip(pDevice->m_s8ModuleID);
    }
    // add end
    
    if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
    {
	M_OnePcm(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
	
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

DJ_S32 RecvFax(VOIP_STRUCT *pOneVoip)
{
    RetCode_t r;
    char     chTmpFilePath[128] = "";
    char     FileName[256] = "";
    char     MsgStr[256] = "";
    FaxPrivate_t		faxPrivatData;
    if ((pOneVoip->VocDevID.m_s8ModuleID >0)&&(pOneVoip->FaxDevID.m_s8ModuleID > 0))
    {
	memset(&faxPrivatData, 0, sizeof(FaxPrivate_t));
	faxPrivatData.m_PrivateData.m_u32DataSize = sizeof(FaxPrivate_t);
	faxPrivatData.m_u8WorkMode = XMS_FAX_WORKMODE_SERVER;
	faxPrivatData.m_VoIPDevice = pOneVoip->deviceID;
	
	GetFilePathName(pOneVoip, chTmpFilePath);	
	sprintf ( FileName, "%s\\XMS_FaxDemo_%03d.tif", chTmpFilePath, pOneVoip->iSeqID );
	
	DrawFax_FilePath(&M_OneFax(pOneVoip->FaxDevID), FileName);
	
	r = XMS_ctsReceiveFax ( g_acsHandle, 
	    &pOneVoip->FaxDevID,
	    &pOneVoip->VocDevID,
	    FileName, "DJ83636988", (PrivateData_t *)&faxPrivatData );
	
	if ( r < 0 )
	{
	    sprintf ( MsgStr, "XMS_ctsReceiveFax ( %s ) Fail! ret=%d", FileName, r);
	    AddMsg ( MsgStr );
	    WriteMsgLog(MsgStr);
	}
	
	Change_State ( pOneVoip, VOIP_FAX_RECEIVE );
    }
    
    return r;
}

void StopFax(VOIP_STRUCT *pOneVoip)
{
	if(cfg_s32SendFax)
	{
		XMS_ctsStopSendFax(g_acsHandle,&pOneVoip->FaxDevID,NULL);
	}
	else
	{
		XMS_ctsStopReceiveFax(g_acsHandle,&pOneVoip->FaxDevID,NULL);
	}
}

DJ_S32   SendFax(VOIP_STRUCT *pOneVoip)
{
    char    FileName[256] = "";
    char    chTmpFilePath[128] = "";
    char    MsgStr[256] = "";
    RetCode_t r = -1;
    FaxPrivate_t		faxPrivatData;	
    if ((pOneVoip->VocDevID.m_s8ModuleID >0)&&(pOneVoip->FaxDevID.m_s8ModuleID > 0))
    {
	memset(&faxPrivatData, 0, sizeof(FaxPrivate_t));
	faxPrivatData.m_PrivateData.m_u32DataSize = sizeof(FaxPrivate_t);
	faxPrivatData.m_u8WorkMode = XMS_FAX_WORKMODE_SERVER;
	faxPrivatData.m_VoIPDevice = pOneVoip->deviceID;
	
	GetFilePathName(pOneVoip, chTmpFilePath);
	sprintf ( FileName, "%s\\sendfax1.tif", "C:\\DJKeygoe\\Temp");//chTmpFilePath, pOneVoip->iSeqID );	
	
	DrawFax_FilePath(&M_OneFax(pOneVoip->FaxDevID), FileName);
	
	r = XMS_ctsSendFax(g_acsHandle, 
	    &pOneVoip->FaxDevID,
	    &pOneVoip->VocDevID,
	    FileName, "DJ83636987", (PrivateData_t *)&faxPrivatData );
	if ( r < 0 )
	{
	    sprintf ( MsgStr, "XMS_ctsSendFax ( %s ) Fail! ret=%d", FileName, r);
	    AddMsg ( MsgStr );
	    WriteMsgLog(MsgStr);
	}
	
	Change_State (pOneVoip, VOIP_FAX_SEND);
    }
    
    return r;
}

void	ReDrawAll (void)
{
    char	TmpStr[256];
    int		i, iDispRow;
    VOIP_STRUCT	*pOneVoip;	
    FAX_STRUCT      *pOneFax;
    
    // m_ListMain
    pdlg->m_ListMain.DeleteAllItems ();
    for ( i = 0; i < g_iTotalVoip; i ++ )
    {
//	iDispRow = CalDispRow ( i ); 
//	if ( iDispRow < 0 )	continue;
			iDispRow = i;
	
	// 固定不变的内容
	sprintf ( TmpStr, "%3d", iDispRow );
	pdlg->m_ListMain.InsertItem ( iDispRow, TmpStr );
	
	sprintf ( TmpStr, "%d,%d", MapTable_Voip[i].m_s8ModuleID, MapTable_Voip[i].m_s16ChannelID );
	pdlg->m_ListMain.SetItemText ( iDispRow, 1, TmpStr );
	
	pOneVoip = &M_OneVoip(MapTable_Voip[i]);
	pdlg->m_ListMain.SetItemText ( iDispRow, 2, GetString_DeviceMain( pOneVoip->deviceID.m_s16DeviceMain) );
	
	// 可能改变的内容
	DrawMain_LineState ( pOneVoip );
	DrawMain_State ( pOneVoip );
	DrawMain_CallInfo ( pOneVoip );
	DrawMain_DTMF ( pOneVoip );
    }
    
    // m_ListFax
    pdlg->m_ListFax.DeleteAllItems();
    for (i = 0; i < g_iTotalFax; i++)
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
	DrawCount_Voip ( MapTable_Module[i] );
	DrawCount_Voc ( MapTable_Module[i] );
	DrawCount_Fax(MapTable_Module[i]);
	DrawCount_Voip(MapTable_Module[i]); // add by chenwy 2009-02-18
    }
}

void VoipWork ( VOIP_STRUCT *pOneVoip, Acs_Evt_t *pAcsEvt )
{
    Acs_CallControl_Data *	pCallControl = NULL;
    Acs_FaxProc_Data        *pFaxData;
    VoIPCallPrivate_t	*voipCallPrivate = NULL;
    char                    MsgStr[256] = "";	
    int                     r;
    
    if (pOneVoip == NULL || pAcsEvt == NULL)
	return;
    
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{			
	    DrawFax_FilePath(&M_OneFax(pOneVoip->FaxDevID), "");		
		StopFax(pOneVoip);    		    
	    ResetVoip ( pOneVoip, pAcsEvt );
	    Change_State ( pOneVoip, VOIP_FREE );		
	}

    switch(pOneVoip->iState)
    {
    case VOIP_FREE:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )	/*呼入事件*/
	{
	    pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
	    
	    ASSERT(pCallControl->m_PrivData.m_u32DataSize == sizeof(VoIPCallPrivate_t));
	    voipCallPrivate = (VoIPCallPrivate_t *)&(pCallControl->m_PrivData);
	    
	    XMS_ctsAlertCall ( g_acsHandle, &pOneVoip->deviceID, NULL );
	    
	    XMS_ctsAnswerCallIn ( g_acsHandle, &pOneVoip->deviceID, NULL );
	    pOneVoip->iProtocol = voipCallPrivate->m_s32Protocol;
	    
	    //Caller Info
	    strncpy(pOneVoip->addrCallee.tszAddr, voipCallPrivate->m_s8CallerAddress, sizeof(pOneVoip->addrCallee.tszAddr));			
	    strncpy(pOneVoip->addrCallee.tszUserID, voipCallPrivate->m_s8CallerUserID, sizeof(pOneVoip->addrCallee.tszUserID));
	    strncpy(pOneVoip->addrCallee.tszTelNo, pCallControl->m_s8CallingNum, sizeof(pOneVoip->addrCallee.tszTelNo));
	    pOneVoip->addrCallee.port = (unsigned short)voipCallPrivate->m_s32CallerPort;
	    //
	    
	    //Callee Info
	    strncpy(pOneVoip->addrCaller.tszAddr, voipCallPrivate->m_s8CallerAddress, sizeof(pOneVoip->addrCaller.tszAddr));			
	    strncpy(pOneVoip->addrCaller.tszUserID, voipCallPrivate->m_s8CallerUserID, sizeof(pOneVoip->addrCaller.tszUserID));
	    strncpy(pOneVoip->addrCaller.tszTelNo, pCallControl->m_s8CallingNum, sizeof(pOneVoip->addrCaller.tszTelNo));
	    pOneVoip->addrCallee.port = (unsigned short)voipCallPrivate->m_s32CallerPort;
	    //
	    
	    DrawMain_CallInfo( pOneVoip );
	    Change_State ( pOneVoip, VOIP_WAIT_ANSWERCALL );
	}
	break;
	
    case VOIP_WAIT_ANSWERCALL:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	//应答呼入完成事件
	{
		if(BindOneFaxResToVoip(pOneVoip))
		{
//add for disable GTD
/*			CmdParamData_Voice_t cmdv={0};
			memset(&cmdv,0,sizeof(cmdv));
			cmdv.m_u8GtdCtrlValid = 1;
			XMS_ctsSetParam(g_acsHandle,&pOneVoip->VocDevID,0,sizeof(cmdv),(void *)&cmdv);
*/
			Change_State ( pOneVoip, VOIP_FAX_T38START );
		}
	}
	break;
    case VOIP_FAX_T38START:
	if (pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA)
	{
	    Acs_IO_Data *pIOEvt = (Acs_IO_Data *)FetchEventData(pAcsEvt);
	    
	    if ((pIOEvt->m_u16IoType == XMS_IO_TYPE_VOIP_T38START) &&
		(pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP))
	    {
// 			CmdParamData_Voice_t cmdv={0};
// 			memset(&cmdv,0,sizeof(cmdv));
// 			cmdv.m_u8GtdCtrlValid = 1;
// 			XMS_ctsSetParam(g_acsHandle,&pOneVoip->deviceID,0,sizeof(cmdv),(void *)&cmdv);

		MixerControlParam_t paramMixer;
		memset(&paramMixer, 0, sizeof(MixerControlParam_t));
		
		r = XMS_ctsSetParam(g_acsHandle, 
		    &pOneVoip->deviceID, 
		    VOIP_PARAM_RTPMIXER,
				                sizeof(paramMixer),
						(MixerControlParam_t *)&paramMixer);	
		
		if( cfg_s32SendFax )
		{
		    r	= SendFax(pOneVoip);
		}
		else
		{
		    r = RecvFax(pOneVoip);
		}
	    }
	    //	
	    if (r < 0) 
	    {
		XMS_ctsClearCall ( g_acsHandle, &pOneVoip->deviceID, 0, NULL );
		Change_State ( pOneVoip, VOIP_WAITHANGUP );
		
		sprintf ( MsgStr, " (%d, %d) :Call RecvFax Fail!", pOneVoip->deviceID.m_s8ModuleID, pOneVoip->deviceID.m_s16ChannelID);
		AddMsg ( MsgStr );
		WriteMsgLog(MsgStr);
	    }
	    pOneVoip->u32FaxCount++;	
	    if( cfg_s32SendFax )
	    {
		Change_State ( pOneVoip, VOIP_FAX_SEND );
	    }
	    else
	    {
		Change_State ( pOneVoip, VOIP_FAX_RECEIVE );
	    }
	}
	break;
    case VOIP_FAX_RECEIVE:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVFAX )	//Recv fax完成事件
	{
	    pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);
	    
	    DrawFax_DetailInfo ( &M_OneFax(pOneVoip->FaxDevID), pFaxData, XMS_EVT_RECVFAX);
	    
	    Change_State ( pOneVoip, VOIP_WAITHANGUP );
		// 主动挂机
	    XMS_ctsClearCall ( g_acsHandle, &pOneVoip->deviceID, 0, NULL );
	}	
	break;
	
    case VOIP_FAX_SEND:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_SENDFAX )	/*Send fax完成事件*/
	{
	    pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);
	    
	    DrawFax_DetailInfo ( &M_OneFax(pOneVoip->FaxDevID), pFaxData, XMS_EVT_SENDFAX);
	    
	    Change_State ( pOneVoip, VOIP_WAITHANGUP );
		// 主动挂机
	 //   XMS_ctsClearCall ( g_acsHandle, &pOneVoip->deviceID, 0, NULL );
	}
	break;
    case VOIP_WAITHANGUP:
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{			
	    DrawFax_FilePath(&M_OneFax(pOneVoip->FaxDevID), "");		
		    		    
	    ResetVoip ( pOneVoip, pAcsEvt );
	    Change_State ( pOneVoip, VOIP_FREE );		
	}
	break;
    default:
	break;
	}
}
