#include "stdafx.h"
#include "XMS_HizDemo.h"
#include "XMS_HizDemoDlg.h"
#include "LogFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "XMS_HizDemo_Sub.H"
#include "XMS_HizDemo_Event.H"
#include "XMS_HizDemo_String.H"

// Special code for CAS(SS1)
#include "..\CAS_Common_Code\CAS_Common_Cfg.h"		
CmdParamData_CAS_t g_Param_CAS;
// end of code for CAS(SS1)

// Special code for Analog
#include "..\Analog_Common_Code\Analog_Common_Code.h"		
// end of code for Analog

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_DemoApp theApp;
CXMS_DemoDlg	*pdlg;

// var about XMS_Demo.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\XMS_HizDemo.INI";

ServerID_t		cfg_ServerID;
char			cfg_VocPath[128];
int				cfg_iDispChnl;
int				cfg_iVoiceRule;
int				cfg_LogOn;
int				cfg_LogLevel;
int				cfg_iPartWork;
int				cfg_iPartWorkModuleID[256] = {0};
char            cfg_chPartWorkModuleID[256] = {0};
int				cfg_s32DebugOn;

int				cfg_oneRecFileEachTrunk;

// var about work
ACSHandle_t		g_acsHandle = -1;
DJ_U8			g_u8UnitID = 15;
RecordProperty_t	g_RecordProperty;
CmdParamData_Voice_t	g_Voice_Param;
long			g_connectNumber = 0;
long			g_disconnectNumber = 0;

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

extern DJ_U8     g_u8IsStartFlag;

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

typedef  struct 
{
	DJ_U8                   m_u8MonitorTxUnitID;     //the dsp ID of Tx channel
	DJ_U8                   m_u8MonitorRxUnitID;     //the dsp ID of Rx channel
	DJ_U8                   m_U8Ref[2];
	DJ_U16                  m_u16MonitorTxCHID;      //Tx channel ID
	DJ_U16                  m_u16MonitorRxCHID;      //Rx channel ID
}ChInfo;
typedef struct
{
	DJ_U8                   m_u8IsUsed;          //is used flag
	DJ_U8                   m_u8Ref;             //reserve
	DJ_U16                  m_u16SeqID;          //the sequnce ID of this struct for a EM&VOC channal	
	ChInfo                  m_stMonitorGroupInfo;   //the channel info of linking with E1
}MGNode;

typedef struct
{
	DJ_U16                  m_u16BGCount;        //the count of the EM channel including EM channel and VOC channel
	DJ_U16                  m_u16Ref;
	
	MGNode              *   m_pstMGNode;
}MGNodeINFO;

MGNodeINFO                  g_stMGNodeInfo;
int							g_NumbersOfMonitorGroup = 0;
MonitorGroupInfo			g_MonitorGroupInfo[100];
#define  MAX_APPNAME_LEN    30


/////////////////////////////////////////////////////////////////////////////////////////////
BOOL IsBGTxChannel( DeviceID_t * pDevID )
{
	int i = 0;
    
	if ( NULL == pDevID) return FALSE;
    if ( NULL == g_stMGNodeInfo.m_pstMGNode ) return FALSE;
	
	for ( i = 0; i < g_stMGNodeInfo.m_u16BGCount; i++ )
	{
		if ( (pDevID->m_s8ModuleID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u8MonitorTxUnitID )
			&& (pDevID->m_s16ChannelID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u16MonitorTxCHID ) )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL IsBGRxChannel( DeviceID_t * pDevID )
{
	int i = 0;
    
	if ( NULL == pDevID) return FALSE;
    if ( NULL == g_stMGNodeInfo.m_pstMGNode ) return FALSE;
	
	for ( i = 0; i < g_stMGNodeInfo.m_u16BGCount; i++ )
	{
		if ( (pDevID->m_s8ModuleID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u8MonitorRxUnitID )
			&& (pDevID->m_s16ChannelID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u16MonitorRxCHID ))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL IsBGChannel( DeviceID_t * pDevID )
{
	if ( IsBGTxChannel(pDevID) )
	{
		return TRUE;
	}
	else if ( IsBGRxChannel(pDevID) )
	{
		return TRUE;
	}
	
	return FALSE;
}

MGNode * GetMGNode( DJ_U8 u8UnitID, DJ_U16 u16CHID )
{
	int i = 0;
    
    if ( NULL == g_stMGNodeInfo.m_pstMGNode ) return NULL;
	
	for ( i = 0; i < g_stMGNodeInfo.m_u16BGCount; i++ )
	{
		if ( ((u8UnitID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u8MonitorRxUnitID )
			&& (u16CHID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u16MonitorRxCHID ))
			|| ((u8UnitID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u8MonitorTxUnitID )
			&& (u16CHID == g_stMGNodeInfo.m_pstMGNode[i].m_stMonitorGroupInfo.m_u16MonitorTxCHID )))
		{
			return &g_stMGNodeInfo.m_pstMGNode[i];
		}
	}
	
	return NULL;
}

TRUNK_STRUCT * GetOtherBGTrunk( DJ_U8 u8UnitID, DJ_U16 u16CHID )
{
	int		i, j;
	
	for ( i = 0; i < MAX_DSP_MODULE_NUMBER_OF_XMS; i ++ )
	{
		if ( AllDeviceRes[i].lFlag == 1 )
		{
			for ( j = 0; j < AllDeviceRes[i].lTrunkNum; j ++ )
			{
				if ( (u8UnitID == AllDeviceRes[i].pTrunk[j].deviceID.m_s8ModuleID )
					&& (u16CHID == AllDeviceRes[i].pTrunk[j].deviceID.m_s16ChannelID))
				{
					return &AllDeviceRes[i].pTrunk[j];
				}
			}
		}
	}
	
	return NULL;
}

int InitMGNode( TRUNK_STRUCT * pOneTrunk )
{
    BOOL    bIsTxChannel = FALSE;
    MGNode * pMGNode = NULL;
	TRUNK_STRUCT * pOtherTrunk = NULL;
	DJ_U8 u8OtherUnitID = 0;
	DJ_U16 u16OtherCHID = 0;
	
    if ( NULL == pOneTrunk ) return -1;
	
	if ( pOneTrunk->u8IsBGFlag == TRUE )
	{
		return 0;
	}
	
	if ( IsBGChannel( &pOneTrunk->deviceID ) )
	{
		pOneTrunk->u8IsBGFlag = TRUE;
	}
	else
	{
		pOneTrunk->u8IsBGFlag = FALSE;
		return 0;
	}
	
	if ( IsBGTxChannel(&pOneTrunk->deviceID) )
	{
		pOneTrunk->u8IsBGTxFlag = TRUE;
	}
	else
	{
		pOneTrunk->u8IsBGTxFlag = FALSE;		
	}
	
	pMGNode = GetMGNode( pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID );
	if ( NULL == pMGNode ) return -1;
	
    if ( ( pOneTrunk->deviceID.m_s8ModuleID == pMGNode->m_stMonitorGroupInfo.m_u8MonitorTxUnitID)
		&& (pOneTrunk->deviceID.m_s16ChannelID == pMGNode->m_stMonitorGroupInfo.m_u16MonitorTxCHID))
	{
		u8OtherUnitID = pMGNode->m_stMonitorGroupInfo.m_u8MonitorRxUnitID;
		u16OtherCHID  = pMGNode->m_stMonitorGroupInfo.m_u16MonitorRxCHID;
	}
	else if ( ( pOneTrunk->deviceID.m_s8ModuleID == pMGNode->m_stMonitorGroupInfo.m_u8MonitorRxUnitID)
		&& (pOneTrunk->deviceID.m_s16ChannelID == pMGNode->m_stMonitorGroupInfo.m_u16MonitorRxCHID))
	{
		u8OtherUnitID = pMGNode->m_stMonitorGroupInfo.m_u8MonitorTxUnitID;
		u16OtherCHID  = pMGNode->m_stMonitorGroupInfo.m_u16MonitorTxCHID;	
	}
	else
	{
		return -1;
	}
	
	pOtherTrunk = GetOtherBGTrunk( u8OtherUnitID, u16OtherCHID );
	if ( NULL == pOtherTrunk )
	{
		return -1;
	}
	
	pOtherTrunk->BGDevID = pOneTrunk->deviceID;
	pOneTrunk->BGDevID = pOtherTrunk->deviceID;
	
	pOtherTrunk->u8IsBGFlag = TRUE;
	
    if ( TRUE == pOneTrunk->u8IsBGTxFlag )
	{
		pOtherTrunk->u8IsBGTxFlag = FALSE;
	}
	else
	{
		pOtherTrunk->u8IsBGTxFlag = TRUE;		
	}
	
	return 0;
}

DJ_S32 ReadMGNode(MGNode * info)
{
	static char strAppName[ ] = "MONITOR_CFG";
	static char strKey[MAX_APPNAME_LEN];
	
	TRACE("******** ReadBindInfo\n");
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "MonitorTxUnitID[%d]", info->m_u16SeqID + 1);
	info->m_stMonitorGroupInfo.m_u8MonitorTxUnitID = GetPrivateProfileInt(strAppName, strKey, 255, cfg_IniName);
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "MonitorTxCHID[%d]", info->m_u16SeqID + 1);
	info->m_stMonitorGroupInfo.m_u16MonitorTxCHID  = GetPrivateProfileInt(strAppName, strKey, 65535, cfg_IniName);
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "MonitorRxUnitID[%d]", info->m_u16SeqID + 1);
	info->m_stMonitorGroupInfo.m_u8MonitorRxUnitID = GetPrivateProfileInt(strAppName, strKey, 255, cfg_IniName);
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "MonitorRxCHID[%d]", info->m_u16SeqID + 1);
	info->m_stMonitorGroupInfo.m_u16MonitorRxCHID  = GetPrivateProfileInt(strAppName, strKey, 65535, cfg_IniName);
	
	return 0;
}

//get record property information from ini
DJ_S32 ReadRecordProperty(void)
{
	char strAppName[ ] = "RECORD_PROPERTY";
	char strKey[MAX_APPNAME_LEN]={0};
	
	TRACE("******** ReadBindInfo\n");
	memset(&g_RecordProperty, 0, sizeof(RecordProperty_t) );
	memset(&g_Voice_Param, 0, sizeof (CmdParamData_Voice_t) );	
	
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "EncodeType");
	switch (GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName))
	{
	case 0:
		g_RecordProperty.m_u8EncodeType = XMS_Alaw_type;
		break;
	case 1:
		g_RecordProperty.m_u8EncodeType = XMS_Ulaw_type;
		break;
	case 2:
		g_RecordProperty.m_u8EncodeType = XMS_Vox_type;
		break;
	case 3:
		g_RecordProperty.m_u8EncodeType = XMS_Linear_8bit;
		break;
	case 4:
		g_RecordProperty.m_u8EncodeType = XMS_Linear_16bit;
		break;
	default:
		g_RecordProperty.m_u8EncodeType = XMS_Alaw_type;
		break;
	}
	
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "SRCMode");
	switch (GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName))
	{
	case 0:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_8K;
		break;
	case 1:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_6K;
		break;
	case 2:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_GTG;
		break;
	case 3:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_FSK;
		break;
	case 4:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_RTP;
		break;
	case 5:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_FAX;
		break;
	default:
		g_RecordProperty.m_u8SRCMode = XMS_SRC_8K;
		break;
	}
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "StopMode");
	switch (GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName))
	{
	case 0:
		g_RecordProperty.m_u8StopMode = XMS_Normal_Stop;
		break;
	case 1:
		g_RecordProperty.m_u8StopMode = XMS_Single_Code;
		break;
	case 2:
		g_RecordProperty.m_u8StopMode = XMS_Any_Code;
		break;
	default:
		g_RecordProperty.m_u8StopMode = XMS_Normal_Stop;
		break;
	}
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "AGCEnable");
	g_Voice_Param.m_VocInputControl.m_u8AgcEnable = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "FixGain");
	g_Voice_Param.m_VocInputControl.m_u16FixGain = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
	
	return 0;
}
//get monitor group information from ini
DJ_S32 ReadMonitorGroupInfo(void)
{
	static char strAppName[ ] = "MONITOR_CFG";
	static char strKey[MAX_APPNAME_LEN];
	
	TRACE("******** ReadBindInfo\n");
	memset(&g_MonitorGroupInfo, 0, sizeof(MonitorGroupInfo) );
	
	memset(strKey, 0, MAX_APPNAME_LEN);
	sprintf(strKey, "NumsOfMonitorGroup");
	g_NumbersOfMonitorGroup = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
	
	for (int i=0; i<g_NumbersOfMonitorGroup; i++)
	{
		memset(strKey, 0, MAX_APPNAME_LEN);
		sprintf(strKey, "MonitorFirstDspModuleID[%d]", i + 1);
		g_MonitorGroupInfo[i].m_MonitorFirstDspModuleID  = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
		
		memset(strKey, 0, MAX_APPNAME_LEN);
		sprintf(strKey, "MonitorFirstE1[%d]", i + 1);
		g_MonitorGroupInfo[i].m_MonitorFirstE1 = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
		
		memset(strKey, 0, MAX_APPNAME_LEN);
		sprintf(strKey, "MonitorSecondDspModuleID[%d]", i + 1);
		g_MonitorGroupInfo[i].m_MonitorSecondModuleID  = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
		
		memset(strKey, 0, MAX_APPNAME_LEN);
		sprintf(strKey, "MonitorSecondE1[%d]", i + 1);
		g_MonitorGroupInfo[i].m_MonitorSecondE1  = GetPrivateProfileInt(strAppName, strKey, 0, cfg_IniName);
		
		
	}
	
	return 0;
}
// void ReadMGNodeInfo(void)
// {
// 	static char strAppName[ ] = "MONITOR_CFG";
// 	static char strKey[MAX_APPNAME_LEN];	
// 	
// 	TRACE("******** ReadMGNodeInfo\n");
// 
// 	memset(&g_stMGNodeInfo, 0, sizeof(MGNodeINFO) );
// 
// 	///////////////////////////////////////////////////////////////////////////////////////
// 	//get the EM init config
// 	memset(strKey, 0, MAX_APPNAME_LEN);
// 	strcpy(strKey, "NumsOfMonitorGroup");
// 
// 	g_stMGNodeInfo.m_u16BGCount = GetPrivateProfileInt(strAppName,strKey,0, cfg_IniName);
// 	///////////////////////////////////////////////////////////////////////////////////////
// 	
// 	g_stMGNodeInfo.m_pstMGNode = (MGNode*)malloc(sizeof(MGNode)*(g_stMGNodeInfo.m_u16BGCount+1));
// 	if ( NULL == g_stMGNodeInfo.m_pstMGNode )
// 	{
// 		return;
// 	}
// 
// 	memset( (DJ_U8*)g_stMGNodeInfo.m_pstMGNode, 0, sizeof(MGNode)*(g_stMGNodeInfo.m_u16BGCount+1) );
// 
// 	for (int i = 0; i < g_stMGNodeInfo.m_u16BGCount; i++)
// 	{
// 		g_stMGNodeInfo.m_pstMGNode[i].m_u16SeqID = i;
// 		g_stMGNodeInfo.m_pstMGNode[i].m_u8IsUsed = FALSE;
// 
// 		ReadMGNode( &g_stMGNodeInfo.m_pstMGNode[i] );
// 	}
// }

//read ini config file,-----------------------------------------------------------------------
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

void	ReadFromConfig(void)
{
	char strTmp[256]={0};
	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );
	
	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);
	
	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9000, cfg_IniName);
	
	GetPrivateProfileString("ConfigInfo","UserName","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);
	
	cfg_iDispChnl = GetPrivateProfileInt ( "ConfigInfo", "DispChnl", 30, cfg_IniName);
	cfg_iVoiceRule = GetPrivateProfileInt ( "ConfigInfo", "VoiceRule", 0, cfg_IniName);
	cfg_iPartWork = GetPrivateProfileInt ( "ConfigInfo", "PartWork", 0, cfg_IniName);
	cfg_s32DebugOn = GetPrivateProfileInt ( "ConfigInfo", "DebugOn", 0, cfg_IniName);
	cfg_oneRecFileEachTrunk = GetPrivateProfileInt ( "ConfigInfo", "OneRecFileEachTrunk", 0, cfg_IniName);
	
	cfg_LogOn = GetPrivateProfileInt ( "ConfigInfo", "LogOn", 0, cfg_IniName);
	cfg_LogLevel = GetPrivateProfileInt ( "ConfigInfo", "LogLevel", 0, cfg_IniName);

	GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(strTmp, cfg_chPartWorkModuleID, sizeof(strTmp));
	SplitStr2Int(strTmp, ",", cfg_iPartWorkModuleID);
	
	ReadRecordProperty();
	ReadMonitorGroupInfo();
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
	
	sprintf ( TmpStr, "%s", cfg_chPartWorkModuleID);
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
	else if ( cfg_iVoiceRule == 2 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_MostFreeFist))->SetCheck ( 1 ) ;
	else if ( cfg_iVoiceRule == 3 )
		((CButton *)pdlg->GetDlgItem (IDC_RADIO_Seq))->SetCheck ( 1 ) ;
	
	
	if ( cfg_iPartWork == 0 )
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_PartWork))->SetCheck ( 0 ) ;
	else
		((CButton *)pdlg->GetDlgItem (IDC_CHECK_PartWork))->SetCheck ( 1 ) ;
	
	sprintf ( TmpStr, "%s", cfg_chPartWorkModuleID );
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
	else if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_MostFreeFist))->GetCheck ( ) )
		cfg_iVoiceRule = 2;
    else if ( ((CButton *)pdlg->GetDlgItem (IDC_RADIO_Seq))->GetCheck ( ) )
		cfg_iVoiceRule = 3;
	
	if ( ((CButton *)pdlg->GetDlgItem (IDC_CHECK_PartWork))->GetCheck ( ) )
		cfg_iPartWork = 1;
	else
		cfg_iPartWork = 0;
	
	// 	pdlg->GetDlgItem ( IDC_EDIT_ModuleID )->GetWindowText ( TmpStr, 30 );
	// 	sscanf ( TmpStr, "%d", &cfg_iPartWorkModuleID);
	pdlg->GetDlgItem(IDC_EDIT_ModuleID)->GetWindowText(cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID));
	
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
	lvc.pszText = "SMT" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 
	
	lvc.iSubItem = 8;
	lvc.pszText = "VocInfo" ;
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn ( 8, &lvc ); 
	
	lvc.iSubItem = 9;
	lvc.pszText = "ReleaseCause" ;
	lvc.cx = 70;
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
		
	case TRK_CONNECT:
		strcpy(StateStr,"Connect");
		break ;
	case TRK_RECORDFILE:
		strcpy(StateStr,"RecordFile");
		break ;
	case TRK_DISCONNECT:
		strcpy(StateStr,"Disonnect");
		break ;
		
		
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
/************************************************************************/
/*function:系统初始化                                                   */
/*input: void                                                           */
/*output:如果建立连接失败或者设置回调函数失败则返回FALSE                */
/************************************************************************/
bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];
	
	pdlg = (CXMS_DemoDlg	*)theApp.m_pMainWnd;
	
	// Read From "XMS_HizDemo.INI"
	ReadFromConfig();
	InitLogCfg();		//初始化日志

	// Special code for CAS(SS1)
	// Read From "C:\\DJKeygoe\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI"
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
	
	// Init m_ListCount
	InitListCount();
	
	// Init Conifg's text box
	InitTextBox();
	
	// now open ACS Server
	/* Call acsOpenStream() to connect with ACS Server */
	cfg_s32DebugOn = 1;
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
		WriteLog(LEVEL_ERROR, MsgStr);
		return false;
	}
	else
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
		AddMsg ( MsgStr );
		WriteLog(LEVEL_DEBUG,MsgStr);
	}
	
	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		AddMsg ( MsgStr );
		WriteLog(LEVEL_ERROR, MsgStr);
		return false;
	}else
	{
		sprintf ( MsgStr, "XMS_acsSetESR() OK!" );
		AddMsg ( MsgStr );
		WriteLog(LEVEL_DEBUG,MsgStr);
	}
	
	InitAllDeviceRes ();
	
	XMS_acsGetDeviceList ( g_acsHandle, NULL );
	
	return true;
}
/************************************************************************/
/* function:退出系统                                                    */
/* input: void                                                          */
/* output:void                                                          */
/* description:关闭所有设备，断开Keygoe连接，写配置文件                 */
/************************************************************************/
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
	
	// save to "XMS_HizDemo.INI"
	FetchFromText();
	WriteToConfig();
	WriteLog(LEVEL_INFO,"ExitSystem\n");
	
}
/************************************************************************/
/* function: 打开中继设备                                               */
/* input: 中继设备结构体指针                                            */
/* output: void                                                         */
/************************************************************************/
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
/************************************************************************/
/* function: 打开语音设备                                               */
/* input: 语音设备结构体指针                                            */
/* output: void                                                         */
/************************************************************************/
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
/************************************************************************/
/* function: 打开PCM设备                                                */
/* input: PCM设备结构体指针                                             */
/* output: void                                                         */
/************************************************************************/
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
/************************************************************************/
/* function: 打开Board设备                                              */
/* input: Board设备结构体指针                                           */
/* output: void                                                         */
/************************************************************************/
void	OpenBoardDevice (  DJ_S8 s8DspModID )
{
	RetCode_t	r;
	
	if ( AllDeviceRes[s8DspModID].bOpenFlag == false )	// 还没有Open
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
		// this line is very important, must before all operation
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = true;
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);
		// this line is very important, must before all operation
		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		
		
		// init this Device: Trunk
		InitTrunkChannel ( pOneTrunk );
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		// modify the count
		g_iTotalTrunkOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;
		
		DrawCount_Trunk ( pDevice->m_s8ModuleID  );
		
		if(pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_DIGITAL_REC)
		{
			
			DigRecSwitchMode sm;
			memset(&sm,0, sizeof(DigRecSwitchMode));
			sm.m_u8Enable =1;  //Enable
			sm.m_u8SwitchMode = 1; //AVAYA
			sm.m_u8EncodeType =1; //A-Law
			
			XMS_ctsSetParam (g_acsHandle, &(pOneTrunk->deviceID), XMS_DIGREC_PARAM_UNI, sizeof(DigRecSwitchMode), (void *)&sm);
		}
		
		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_REC )
		{
			// Set AnalogTrunk
			/*
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
			*/
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
		if (g_Voice_Param.m_VocInputControl.m_u8AgcEnable)
		{
			g_Voice_Param.m_u8InputCtrlValid = 1;
			XMS_ctsSetParam(g_acsHandle,pDevice,VOC_PARAM_UNIPARAM,sizeof(g_Voice_Param),(void *)&g_Voice_Param);
		}
		
		
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
	
	if ( TRUE == pOneTrunk->u8IsRecordFlag )
	{
		StopRecordFile(&pOneTrunk->VocDevID);
	}
	
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

// -----------------------------------------------------------------------------
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
DJ_S32	 RecordFile_NoMix ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend )
{	
	DJ_U32				i = 0;	
	RetCode_t			r;
	
	g_RecordProperty.m_u32MaxSize = u32RecSize;
	
	if ( bIsAppend )
		g_RecordProperty.m_s8IsAppend = 1;
	else
		g_RecordProperty.m_s8IsAppend = 0;
	
	strcpy ( g_RecordProperty.m_s8FileName, s8FileName );
	g_RecordProperty.m_s8IsMixEnable = 0;
	
	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &g_RecordProperty, NULL );
	
	return r;
}
/* Handle Record */
DJ_S32	 RecordFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend ,int voc1Chn)
{	
	DJ_U32				i = 0;	
	RetCode_t			r;
	
	g_RecordProperty.m_u32MaxSize = u32RecSize;
	if ( bIsAppend )
		g_RecordProperty.m_s8IsAppend = 1;
	else
		g_RecordProperty.m_s8IsAppend = 0;	
	g_RecordProperty.m_s8IsMixEnable = 1;
    g_RecordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
	g_RecordProperty.m_MixerControl.m_u16SRC_ChID1 = voc1Chn;
	g_RecordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
	g_RecordProperty.m_MixerControl.m_u16SRC_ChID2 = pVocDevID->m_s16ChannelID;
	strcpy ( g_RecordProperty.m_s8FileName, s8FileName );
	
	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &g_RecordProperty, NULL );
	
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

char *My_GetFskCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Evemt*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		
		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_FSK ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			
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
	
	s8ModID = pOneTrunk->deviceID.m_s8ModuleID;
	
	//cfg_iVoiceRule = 0;
	
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
	for ( i = iLoopStart; i < AllDeviceRes[s8SearchModID].lVocNum; i ++ )
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
			
			if ( cfg_iVoiceRule == 3 )
			{
				iLoopStart = i;
			}			
			return i;
		}
	}	
	iLoopStart = 0;
	for ( i = iLoopStart; i < AllDeviceRes[s8SearchModID].lVocNum; i ++ )
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
			if ( cfg_iVoiceRule == 3 )
			{
				iLoopStart = i;
			}			
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
void	PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk )
{
	DeviceID_t FreeVocDeviceID;	
	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_REC )
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
	
	pOneTrunk->u8RecordCounter = 0;
	pOneTrunk->u8IsRecordFlag = FALSE;
	
	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	DrawMain_VocInfo ( pOneTrunk );
}
/************************************************************************/
/* function:重置中继设备                                                */
/* input: 中继设备指针，事件指针                                        */
/* output:void                                                          */
/************************************************************************/
void ResetTrunk ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );		
		if (TRUE == pOneTrunk->u8IsRecordFlag )
		{
			StopRecordFile(&pOneTrunk->VocDevID);
		}		
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
/************************************************************************/
/* function: 设置模拟中继参数                                           */
/* input: 设备ID                                                        */
/* output: void                                                         */
/************************************************************************/
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
	char szbuffer[1024];
	memset(szbuffer,0,sizeof(szbuffer));
	sprintf(szbuffer,"Set GTD ret = %d\n",r);
	AddMsg ( szbuffer );
	
	//========Set GTG End  ========
}
/************************************************************************/
/* function: 数字中继高阻录音和信令监控处理函数                         */
/* input: 中继设备结构体指针，事件指针                                  */
/* output: void                                                         */
/* description: 包括ISDN、TUP、ISUP的处理                               */
/************************************************************************/
void TrunkWork_ISDN_SS7(TRUNK_STRUCT *pEventTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t				FreeVoc1DeviceID;
	DeviceID_t				FreeVoc2DeviceID;
	char					FileName[256] = {0};
	char                    str[512] = {0};
	char					MsgStr[512] = {0};
	DeviceID_t *            pDev = &pAcsEvt->m_DeviceID;
	PSMON_EVENT             SMevt= NULL;
    static  DJ_U32          m_u32Counter = 0;
	int						iPos = 0;
	int						monitorFirstDspModuleID;
	int						monitorFirstE1;
	int						monitorSecondDspModuleID;
	int						monitorSecondE1;	
	if ( XMS_EVT_SIGMON != pAcsEvt->m_s32EventType)
	{
		AddMsg("Event type != XMS_EVT_SIGMON ");
		return;
	}	
	if (g_NumbersOfMonitorGroup == 0)
	{
		strcpy(MsgStr, "g_NumbersOfMonitorGroup == 0");
		AddMsg(MsgStr);
		WriteLog(LEVEL_ERROR,MsgStr);
		return;
	}
	
	SMevt = (PSMON_EVENT)FetchEventData(pAcsEvt);	
	//根据当前产生监控事件的通道定位监控组信息
	for (int i=0; i<g_NumbersOfMonitorGroup; i++)
	{
		if ((pAcsEvt->m_DeviceID.m_s8ModuleID == g_MonitorGroupInfo[i].m_MonitorFirstDspModuleID 
			&& pAcsEvt->m_DeviceID.m_s16ChannelID/32 + 1 == g_MonitorGroupInfo[i].m_MonitorFirstE1) ||
			(pAcsEvt->m_DeviceID.m_s8ModuleID == g_MonitorGroupInfo[i].m_MonitorSecondModuleID 
			&& pAcsEvt->m_DeviceID.m_s16ChannelID/32 + 1 == g_MonitorGroupInfo[i].m_MonitorSecondE1))
		{		
			monitorFirstDspModuleID = g_MonitorGroupInfo[i].m_MonitorFirstDspModuleID;
			monitorFirstE1 = g_MonitorGroupInfo[i].m_MonitorFirstE1;
			monitorSecondDspModuleID = g_MonitorGroupInfo[i].m_MonitorSecondModuleID;
			monitorSecondE1 = g_MonitorGroupInfo[i].m_MonitorSecondE1;
			i = g_NumbersOfMonitorGroup;	
			sprintf(MsgStr,"Find the monitor group, monitorFirstDspModuleID:%d, monitorFirstE1:d,
													monitorSecondDspModuleID:%d,monitorSecondE1:%d",
													monitorFirstDspModuleID,monitorFirstE1,
													monitorSecondDspModuleID,monitorSecondE1);
			WriteLog(LEVEL_DEBUG, MsgStr);
			break;
		}
	}
	//没有找到监控组信息
	if (i == g_NumbersOfMonitorGroup)
	{
		sprintf(str,"The trunk not exist in the ini file! Trk(%d,%d), E1:%d.", pAcsEvt->m_DeviceID.m_s8ModuleID, pAcsEvt->m_DeviceID.m_s16ChannelID,pAcsEvt->m_DeviceID.m_s16ChannelID/32 + 1);
		AddMsg(str);
		WriteLog(LEVEL_ERROR, str);
		return;
	}

	TRUNK_STRUCT *pOneRecordTrunk1 = &AllDeviceRes[monitorFirstDspModuleID].pTrunk[SMevt->Chn + (monitorFirstE1-1)*32];
	TRUNK_STRUCT *pOneRecordTrunk2 = &AllDeviceRes[monitorSecondDspModuleID].pTrunk[SMevt->Chn + (monitorSecondE1-1)*32];
	if ( pOneRecordTrunk1 == NULL || pOneRecordTrunk2 == NULL )
	{
		AddMsg("Record Trunk is not exist");
		WriteLog(LEVEL_ERROR, "Record Trunk is not exist");
		return;
	}	
	switch(SMevt->EventType)
	{
	case SMON_EVT_Call_Generate:
		if ( g_u8IsStartFlag)
		{
			if (pOneRecordTrunk1->State == TRK_FREE && pOneRecordTrunk2->State == TRK_FREE)
			{			
				int iRecord1Pos = CalDispRow(pOneRecordTrunk1->iSeqID); 
				int iRecord2Pos = CalDispRow(pOneRecordTrunk2->iSeqID);				
				sprintf(str,"%d", SMevt->MsgType);
				pdlg->m_ListMain.SetItemText(iRecord1Pos, 7, str);
				pdlg->m_ListMain.SetItemText(iRecord1Pos, 6, SMevt->Caller_ID);
				pdlg->m_ListMain.SetItemText(iRecord1Pos, 5, SMevt->Called_ID);
				pdlg->m_ListMain.SetItemText(iRecord2Pos, 7, str);
				pdlg->m_ListMain.SetItemText(iRecord2Pos, 6, SMevt->Caller_ID);
				pdlg->m_ListMain.SetItemText(iRecord2Pos, 5, SMevt->Called_ID);				
				Change_State(pOneRecordTrunk1,TRK_CONNECT);
				Change_State(pOneRecordTrunk2,TRK_CONNECT);				
				sprintf(str,"EvtTrk(%d, %d), SMON_EVT_Call_Generate, RecTrk1(%d, %d), RecTrk2(%d, %d)", 
					pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID,
					pOneRecordTrunk1->deviceID.m_s8ModuleID, pOneRecordTrunk1->deviceID.m_s16ChannelID,
					pOneRecordTrunk2->deviceID.m_s8ModuleID, pOneRecordTrunk2->deviceID.m_s16ChannelID);
				AddMsg(str);
				WriteLog(LEVEL_DEBUG,str);
			}else{
				sprintf(str,"trunk state is wrong! EvtTrk(%d, %d), SMON_EVT_Call_Generate, RecTrk1(%d,%d) state=%s,RecTrk2(%d,%d) state=%s", 
					pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID,
					pOneRecordTrunk1->deviceID.m_s8ModuleID, pOneRecordTrunk1->deviceID.m_s16ChannelID, GetString_State(pOneRecordTrunk1->State),
					pOneRecordTrunk2->deviceID.m_s8ModuleID, pOneRecordTrunk2->deviceID.m_s16ChannelID, GetString_State(pOneRecordTrunk2->State));
				AddMsg(str);
				WriteLog(LEVEL_ERROR,str);
				return;
			}	
		}else
		{
			sprintf(str,"Please start record! EvtTrk(%d, %d), SMON_EVT_Call_Generate",
				pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID);
			AddMsg(str);
			WriteLog(LEVEL_ERROR, str);
			return;
		}		
		break;
	case SMON_EVT_Call_Connect:
		g_connectNumber++;			
		sprintf(MsgStr, "%d", g_connectNumber);
		pdlg->GetDlgItem(IDC_STATIC_CONNUM)->SetWindowText(MsgStr);
		if ( g_u8IsStartFlag)
		{			
			if (pOneRecordTrunk1->State == TRK_CONNECT && pOneRecordTrunk2->State == TRK_CONNECT)
			{				
				int iRecord1Pos = CalDispRow(pOneRecordTrunk1->iSeqID); 
				int iRecord2Pos = CalDispRow(pOneRecordTrunk2->iSeqID); 
				TRUNK_STRUCT *pSearchVocTrunk;
				//在语音资源较多的DSP查找空闲语音，pOneRecordTrunk1和pOneRecordTrunk2可以位于不同的DSP，
				//不影响混音录音，因为查找的Voc在同一个DSP上找，Voc和Trk双向时隙连接可以跨DSP
				if (AllDeviceRes[monitorFirstDspModuleID].lVocFreeNum >= AllDeviceRes[monitorSecondDspModuleID].lVocFreeNum )
				{
					pSearchVocTrunk = pOneRecordTrunk1;
				}else{
					pSearchVocTrunk = pOneRecordTrunk2;
				}
				
				int voc1Chn = SearchOneFreeVoice ( pSearchVocTrunk,  &FreeVoc1DeviceID );
				if ( voc1Chn >= 0 )
				{
					pOneRecordTrunk1->VocDevID = FreeVoc1DeviceID;
					M_OneVoice(FreeVoc1DeviceID).UsedDevID = pOneRecordTrunk1->deviceID; 
					DrawMain_VocInfo ( pOneRecordTrunk1 );
					My_DualLink ( &FreeVoc1DeviceID, &pOneRecordTrunk1->deviceID ); 
				}else{
					sprintf(str,"Not Find free voc1Chn, pSearchVocTrunk(%d, %d).", 
						pSearchVocTrunk->deviceID.m_s8ModuleID,	pSearchVocTrunk->deviceID.m_s16ChannelID);
					AddMsg(str);
					WriteLog(LEVEL_ERROR, str);
					return;
				}			
				
				int voc2Chn = SearchOneFreeVoice ( pSearchVocTrunk,  &FreeVoc2DeviceID );
				if (  voc2Chn >= 0 )
				{
					pOneRecordTrunk2->VocDevID = FreeVoc2DeviceID;
					M_OneVoice(FreeVoc2DeviceID).UsedDevID = pOneRecordTrunk2->deviceID; 
					DrawMain_VocInfo ( pOneRecordTrunk2 );
					My_DualLink ( &FreeVoc2DeviceID, &pOneRecordTrunk2->deviceID ); 					
				}else
				{
					sprintf(str,"Not Find free voc2Chn, pSearchVocTrunk(%d, %d).", 
						pSearchVocTrunk->deviceID.m_s8ModuleID,	pSearchVocTrunk->deviceID.m_s16ChannelID);
					AddMsg(str);
					WriteLog(LEVEL_ERROR, str);
				}

				//获取系统时间，用来组成录音文件名
				CString str;
				CTime tm;					
				tm=CTime::GetCurrentTime();
				str=tm.Format("%Y%m%d%H%M%S");
				
				if ( XMS_DEVSUB_HIZ_SS7 == pAcsEvt->m_DeviceID.m_s16DeviceSub || XMS_DEVSUB_HIZ_SS7_64K_LINK == pAcsEvt->m_DeviceID.m_s16DeviceSub)
				{
					if (cfg_oneRecFileEachTrunk)
					{//每个通道时钟只录一个文件
						sprintf ( FileName, "%s\\SS7Rec-%d-%d-%d.pcm", cfg_VocPath,monitorFirstDspModuleID,pOneRecordTrunk1->deviceID.m_s16ChannelID,pOneRecordTrunk2->deviceID.m_s16ChannelID,str);
					}else
					{//每个通道每次录一个文件，通过后面精确到秒时间区分
						sprintf ( FileName, "%s\\SS7Rec-%d-%d-%d-%s.pcm", cfg_VocPath,monitorFirstDspModuleID,pOneRecordTrunk1->deviceID.m_s16ChannelID,pOneRecordTrunk2->deviceID.m_s16ChannelID,str);
					}
				}else if ( XMS_DEVSUB_HIZ_PRI == pAcsEvt->m_DeviceID.m_s16DeviceSub || XMS_DEVSUB_HIZ_PRI_LINK == pAcsEvt->m_DeviceID.m_s16DeviceSub)
				{
					if (cfg_oneRecFileEachTrunk)
					{//每个通道时钟只录一个文件
						sprintf ( FileName, "%s\\ISDNRec-%d-%d-%d.pcm", cfg_VocPath,monitorFirstDspModuleID,pOneRecordTrunk1->deviceID.m_s16ChannelID,pOneRecordTrunk2->deviceID.m_s16ChannelID,str);
					}else
					{//每个通道每次录一个文件，通过后面精确到秒时间区分
						sprintf ( FileName, "%s\\ISDNRec-%d-%d-%d-%s.pcm", cfg_VocPath,monitorFirstDspModuleID,pOneRecordTrunk1->deviceID.m_s16ChannelID,pOneRecordTrunk2->deviceID.m_s16ChannelID,str);
					}								
				}					
				//start record in the mix way
				RecordFile ( &pOneRecordTrunk2->VocDevID, FileName, 8000L*3600L*24, false ,voc1Chn);		// we record for 24 hours
				pOneRecordTrunk1->u8IsRecordFlag = TRUE;
				pOneRecordTrunk2->u8IsRecordFlag = TRUE;
				++pOneRecordTrunk1->u8RecordCounter;
				++pOneRecordTrunk2->u8RecordCounter;
				Change_State ( pOneRecordTrunk1, TRK_RECORDFILE );	
				Change_State ( pOneRecordTrunk2, TRK_RECORDFILE );	
				sprintf(str,"EvtTrk(%d, %d), SMON_EVT_Call_Connect, RecTrk1(%d,%d), RecTrk2(%d,%d)", 
					pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID,
					pOneRecordTrunk1->deviceID.m_s8ModuleID, pOneRecordTrunk1->deviceID.m_s16ChannelID, GetString_State(pOneRecordTrunk1->State),
					pOneRecordTrunk2->deviceID.m_s8ModuleID, pOneRecordTrunk2->deviceID.m_s16ChannelID, GetString_State(pOneRecordTrunk2->State));
				AddMsg(str);
				WriteLog(LEVEL_DEBUG, str);
				sprintf(str, "RecordFile:%s,  RecTrk1(%d,%d), RecTrk2(%d,%d)", FileName,
					pOneRecordTrunk1->deviceID.m_s8ModuleID, pOneRecordTrunk1->deviceID.m_s16ChannelID,
					pOneRecordTrunk2->deviceID.m_s8ModuleID, pOneRecordTrunk2->deviceID.m_s16ChannelID,);		
				AddMsg(str);
				WriteLog(LEVEL_DEBUG, str);
			}else{
				sprintf(str,"trunk state is wrong! EvtTrk(%d, %d), SMON_EVT_Call_Connect, RecTrk1(%d,%d) state=%s, RecTrk2(%d,%d) state=%s", 
					pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID,
					pOneRecordTrunk1->deviceID.m_s8ModuleID, pOneRecordTrunk1->deviceID.m_s16ChannelID, GetString_State(pOneRecordTrunk1->State),
					pOneRecordTrunk2->deviceID.m_s8ModuleID, pOneRecordTrunk2->deviceID.m_s16ChannelID, GetString_State(pOneRecordTrunk2->State));
				AddMsg(str);
				WriteLog(LEVEL_ERROR,str);
				return;
			}			
		}else
		{
			sprintf(str,"Please start record! EvtTrk(%d, %d), SMON_EVT_Call_Connect",
				pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID);
			AddMsg(str);
			WriteLog(LEVEL_ERROR, str);
			return;
		}			
		break;		
	case SMON_EVT_Call_Disconnect:	
		g_disconnectNumber++;
		sprintf(MsgStr, "%d", g_connectNumber);
		pdlg->GetDlgItem(IDC_STATIC_DISCONNUM)->SetWindowText(MsgStr);
		
		if (!g_u8IsStartFlag)
		{
			AddMsg("SMON_EVT_Call_Disconnect===please start record.");
			return;
		}		
		if (pOneRecordTrunk1->State == TRK_RECORDFILE && pOneRecordTrunk2->State == TRK_RECORDFILE)
		{
			int iRecord1Pos = CalDispRow(pOneRecordTrunk1->iSeqID); 
			int iRecord2Pos = CalDispRow(pOneRecordTrunk2->iSeqID); 			
			sprintf(str,"DSP:%d Record1Pos:%d Record2Pos:%d MsgType:%d,ReleaseReason:%d",
				monitorFirstDspModuleID,iRecord1Pos,iRecord2Pos,SMevt->MsgType,SMevt->ReleaseReason);
			AddMsg(str);			
			StopRecordFile(&pOneRecordTrunk1->VocDevID);
			StopRecordFile(&pOneRecordTrunk2->VocDevID);
			ResetTrunk ( pOneRecordTrunk1, pAcsEvt );
			ResetTrunk ( pOneRecordTrunk2, pAcsEvt );
			TRACE("********** (DSP: %d, CH: %d)Call_DisConnect ***** \n", pEventTrunk->deviceID.m_s8ModuleID, pEventTrunk->deviceID.m_s16ChannelID );
		}else
		{
			AddMsg("SMON_EVT_Call_Disconnect: trunk state is wrong.");
			return;
		}	
		break;
	}	
}
/************************************************************************/
/* function: 模拟中继高阻录音处理函数                                   */
/* input: 中继设备结构体指针，事件结构体指针                            */
/* output: void                                                         */
/************************************************************************/
void TrunkWork_Analog ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char					TmpDtmf;
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		if (g_u8IsStartFlag){
			char	 *p;			
			p = My_GetFskCode ( pAcsEvt );
			if ( p != NULL )
			{
				ConvertRawFskToCallerID ((unsigned char *) p, pOneTrunk->CallerCode, 20 );
				DrawMain_CallInfo(pOneTrunk);
				AddMsg(pOneTrunk->CallerCode);
			}				
			if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )
			{				
				// release the Voice for get FSK
				My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );				
				FreeOneFreeVoice (  &pOneTrunk->VocDevID );
				DrawMain_VocInfo ( pOneTrunk );				
				DrawMain_CallInfo( pOneTrunk );				
				// start record
				if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
				{
					pOneTrunk->VocDevID = FreeVocDeviceID;				
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
					DrawMain_VocInfo ( pOneTrunk );				
					My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );
					
					//get system time
					CString					str;
					CTime					tm;					
					tm=CTime::GetCurrentTime();
					str=tm.Format("%Y%m%d-%H%M%S");
					sprintf ( FileName, "%s\\AnalogHizRec-%d-%d-%s.pcm", cfg_VocPath,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,str);
					
					//start record in the no mix way					
					RecordFile_NoMix( &pOneTrunk->VocDevID, FileName, 8000L*3600L*24, false );
					Change_State ( pOneTrunk, TRK_RECORDFILE );
				}else{
					AddMsg("No free voice device.");
					return;
				}
			}
		}else
		{
			AddMsg("Please start record.");
			return;
		}		
		break;
		
	case TRK_RECORDFILE:
		pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);		
		TmpDtmf = My_GetDtmfCode ( pAcsEvt );
		if ( TmpDtmf != -1 )	
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawMain_DTMF ( pOneTrunk );
		}		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )
		{
			StopRecordFile ( &pOneTrunk->VocDevID );			
			ResetTrunk ( pOneTrunk, pAcsEvt );			
			Change_State ( pOneTrunk, TRK_FREE );
		}		
		break;
	}
}
/************************************************************************/
/* function: 数字录音模块处理函数                                       */
/* input: 中继设备结构体指针，事件指针                                  */
/* output: void                                                         */
/* description: 处理数字录音模块的录音过程                              */
/************************************************************************/
void TrunkWork_Digital ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char					TmpDtmf;
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		if ( g_u8IsStartFlag)
		{
			if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )
			{
				pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
				sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
				sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
				DrawMain_CallInfo( pOneTrunk );
				
				//search free voice,digital hiz record,the voice trunk number must the same with pOneTrunk
				VOICE_STRUCT *pOneVoice = &AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].pVoice[pOneTrunk->deviceID.m_s16ChannelID];
				
				if (pOneVoice != NULL)
				{
					if ( pOneVoice->State != VOC_WAITOPEN && pOneVoice->State != VOC_USED)
					{
						FreeVocDeviceID = pOneVoice->deviceID;						
						Change_Voc_State ( pOneVoice, VOC_USED);						
						DrawCount_Voc( pOneTrunk->deviceID.m_s8ModuleID );						
					}					
					pOneTrunk->VocDevID = FreeVocDeviceID;				
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
					DrawMain_VocInfo ( pOneTrunk );				
					My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );
					//get system time
					CString					str;
					CTime					tm;					
					tm=CTime::GetCurrentTime();
					str=tm.Format("%Y%m%d-%H%M%S");
					sprintf ( FileName, "%s\\DigHizRec-%d-%d-%s.pcm", cfg_VocPath,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,str);
					//start record in the no mix way					
					RecordFile_NoMix( &pOneTrunk->VocDevID, FileName, 8000L*3600L*24, false );// we record for 24 hours
					Change_State ( pOneTrunk, TRK_RECORDFILE );	
				}else{
					AddMsg("No Free Voice device!");
					return;
				}				
			}
		}else{
			AddMsg("Please start record.");
			return;
		}
		break;		
	case TRK_RECORDFILE:		
		TmpDtmf = My_GetDtmfCode ( pAcsEvt );
		if ( TmpDtmf != -1 )	
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawMain_DTMF ( pOneTrunk );
		}		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	// On Hook
		{
			StopRecordFile ( &pOneTrunk->VocDevID );			
			ResetTrunk ( pOneTrunk, pAcsEvt );			
			Change_State ( pOneTrunk, TRK_FREE );
		}		
		break;
	}
}