// GatewayWork.cpp: implementation of the CGatewayWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "GatewayWork.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************/
/* 全局变量                                                             */
/************************************************************************/
ACSHandle_t		g_acsHandle;
ServerID_t		g_cfgServerID;
DJ_U8			g_cfgAppID;
DJ_U32			g_cfgRevQueSize;
DJ_U32			g_cfgSndQueSize;
DJ_S32			g_cfgDebugOn;

int				g_cfgiVoipDspID[MAX_DSP_SUM];
int				g_cfgiDigiDspID[MAX_DSP_SUM][MAX_DSP_E1_SUM];
int				g_cfgAppIsLog;
CString			g_strCfgIni;
PBXADDRESS		g_cfgPBXADDRESS;
ROUTINGCFG		g_cfgOutRoutingString;
CHANGECALLERRULECFG g_cfgCCRuleString;
CHANGECALLERRULECFG g_cfgCCRuleStringEx;
VOIPROUTINGCFG	g_cfgVOIPRouting;
int				g_cfgVOIPCallOutRuleType;
int				g_cfgVOIPDTMFType;
int				g_cfgVOIPVOCVALUE;
CString			DTRKSTATE_STRING[MAX_STATE_SUM];

CString			VOIPSTATE_STRING[6];
CString			CALLDRE_STRING[3];

CLogInfo		*g_pLogInfo;
CShowInfo		*g_pShowInfo;

TYPE_XMS_DSP_DEVICE_RES	g_AllDeviceRes[MAX_DSP_SUM];

void InitPara()
{
	g_acsHandle = 0;
	memset(&g_cfgServerID, 0, sizeof(g_cfgServerID));
	g_cfgAppID = 0;
	g_cfgRevQueSize = 0;
	g_cfgSndQueSize = 0;
	g_cfgDebugOn = 0;
	memset(g_cfgiVoipDspID, 0, sizeof(int)*MAX_DSP_SUM);
	memset(g_cfgiDigiDspID, 0, sizeof(int)*MAX_DSP_SUM*MAX_DSP_E1_SUM);
	g_pLogInfo = NULL;
	g_pShowInfo = NULL;
	// g_AllDeviceRes构造函数已经初始化

	DTRKSTATE_STRING[TRK_WAITOPEN] = theApp.m_clsLIni.GLoadString("TRK_WAITOPEN");
	DTRKSTATE_STRING[TRK_FREE] = theApp.m_clsLIni.GLoadString("TRK_FREE");
	DTRKSTATE_STRING[TRK_CALLOUT] = theApp.m_clsLIni.GLoadString("TRK_CALLOUT");
	DTRKSTATE_STRING[TRK_CALLIN] = theApp.m_clsLIni.GLoadString("TRK_CALLIN");
	DTRKSTATE_STRING[TRK_NOTHANDLE] = theApp.m_clsLIni.GLoadString("TRK_NOTHANDLE");
	DTRKSTATE_STRING[TRK_WAIT_ANSERCALL] = theApp.m_clsLIni.GLoadString("TRK_WAIT_ANSERCALL");
	DTRKSTATE_STRING[TRK_CONNECT] = theApp.m_clsLIni.GLoadString("TRK_CONNECT");
	DTRKSTATE_STRING[TRK_HANGUP] = theApp.m_clsLIni.GLoadString("TRK_HANGUP");
}

/************************************************************************/
/* 功能函数                                                             */
/************************************************************************/
int InitDJKeygoe()
{
	InitPara();

	ReadConfig();

	VOIPSTATE_STRING[VOIP_WAITOPEN] = theApp.m_clsLIni.GLoadID("VOIP_WAITOPEN");
	VOIPSTATE_STRING[VOIP_FREE] = theApp.m_clsLIni.GLoadID("VOIP_FREE");
	VOIPSTATE_STRING[VOIP_HANGUP] = theApp.m_clsLIni.GLoadID("VOIP_HANGUP");
	VOIPSTATE_STRING[VOIP_CALLOUT] = theApp.m_clsLIni.GLoadID("VOIP_CALLOUT");
	VOIPSTATE_STRING[VOIP_CONNECT] = theApp.m_clsLIni.GLoadID("VOIP_CONNECT");
	VOIPSTATE_STRING[VOIP_WAIT_ANSWERCALL] = theApp.m_clsLIni.GLoadID("VOIP_WAIT_ANSWERCALL");

	CALLDRE_STRING[CALLDRE_NOTCALL] = theApp.m_clsLIni.GLoadID("CALLDRE_NOTCALL");
	CALLDRE_STRING[CALLDRE_CALLOUT] = theApp.m_clsLIni.GLoadID("CALLDRE_CALLOUT");
	CALLDRE_STRING[CALLDRE_CALLIN] = theApp.m_clsLIni.GLoadID("CALLDRE_CALLIN");

	RetCode_t iResult = XMS_acsOpenStream( &g_acsHandle, 
		&g_cfgServerID, 
		g_cfgAppID, 
		g_cfgRevQueSize, 
		g_cfgSndQueSize, 
		g_cfgDebugOn, 
		NULL);
	if(iResult != ACSPOSITIVE_ACK)
	{
		if(g_cfgAppIsLog)
		{
			char chErrorInfo[1024] = {0};
			sprintf(chErrorInfo, "%s XMS_acsOpenStream(%s, %d) %s %s %d", 
				theApp.m_clsLIni.GLoadString("InitDJKeygoeUse"), 
				g_cfgServerID.m_s8ServerIp, g_cfgServerID.m_u32ServerPort, 
				theApp.m_clsLIni.GLoadString("Fail"), 
				theApp.m_clsLIni.GLoadString("ReturnValue"), iResult);

			g_pLogInfo->WriteLogInfo(chErrorInfo, LOGTYPE_ERROR);
		}

		return -1;
	}

	iResult = XMS_acsSetESR(g_acsHandle, (EsrFunc)XMSEventProc, 0, 1);
	if(iResult != ACSPOSITIVE_ACK)
	{
		if(g_cfgAppIsLog)
		{
			char chErrorInfo[1024] = {0};
			sprintf(chErrorInfo, "%s XMS_acsSetESR() %s %s %d", 
				theApp.m_clsLIni.GLoadString("InitDJKeygoeUse"), 
				theApp.m_clsLIni.GLoadString("Fail"), 
				theApp.m_clsLIni.GLoadString("ReturnValue"), iResult);

			g_pLogInfo->WriteLogInfo(chErrorInfo, LOGTYPE_ERROR);
		}

		return -2;
	}

	iResult = XMS_acsGetDeviceList(g_acsHandle, NULL);
	if(iResult != ACSPOSITIVE_ACK)
	{
		if(g_cfgAppIsLog)
		{
			char chErrorInfo[1024] = {0};
			sprintf(chErrorInfo, "%s XMS_acsGetDeviceList() %s %s %d", 
				theApp.m_clsLIni.GLoadString("InitDJKeygoeUse"), 
				theApp.m_clsLIni.GLoadString("Fail"), 
				theApp.m_clsLIni.GLoadString("ReturnValue"), iResult);

			g_pLogInfo->WriteLogInfo(chErrorInfo, LOGTYPE_ERROR);
		}

		return -3;
	}

	if(g_cfgAppIsLog)
	{
		char chErrorInfo[1024] = {0};
		sprintf(chErrorInfo, theApp.m_clsLIni.GLoadString("InitDJKeygoeSuccess"));

		g_pLogInfo->WriteLogInfo(chErrorInfo);
	}

	return 1;
}

void ExitDJKeygoe()
{
	for(int s8DspModID = 0; s8DspModID < MAX_DSP_SUM; ++s8DspModID)
	{
		// Close Board
		if( CfgIsUseDsp(s8DspModID) )
		{
			if( g_AllDeviceRes[s8DspModID].bOpenFlag == true )
			{
				XMS_ctsCloseDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].deviceID, NULL);
			}
		}
		else
		{
			continue;
		}

		// Close Trunk
		for(int i = 0; i < g_AllDeviceRes[s8DspModID].lPcmNum; ++i)
		{
			if( CfgIsUsePcm(s8DspModID, i) )
			{
				// pPcm
				if(g_AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag == true)
				{
					XMS_ctsCloseDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pPcm[i].deviceID, NULL);
				}

				// pInterf
				for(int j = 32*i; j < g_AllDeviceRes[s8DspModID].lInterfNum && j < 32*(i+1); ++j)
				{
					XMS_ctsCloseDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pInterf[j].deviceID, NULL);
				}
			}
		}

		// Close VOIP & Voice
		if( CfgIsUseVOIP(s8DspModID) )
		{
			for(int i = 0; i < g_AllDeviceRes[s8DspModID].lVOIPNum; ++i)
			{
				XMS_ctsCloseDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pVOIP[i].deviceID, NULL );
				XMS_ctsCloseDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pVoice[i].deviceID, NULL);

				memset(&g_AllDeviceRes[s8DspModID].pVOIP[i].VocDevID, 0, sizeof(DeviceID_t));
				memset(&g_AllDeviceRes[s8DspModID].pVoice[i].UsedDevID, 0, sizeof(DeviceID_t));
			}
		}
	}

	XMS_acsCloseStream(g_acsHandle, NULL);

	// 释放g_AllDeviceRes中new的内存
	for(s8DspModID = 0; s8DspModID < MAX_DSP_SUM; ++s8DspModID)
	{
		if(g_AllDeviceRes[s8DspModID].pPcm != NULL)
		{
			delete g_AllDeviceRes[s8DspModID].pPcm;
		}

		if(g_AllDeviceRes[s8DspModID].pInterf != NULL)
		{
			delete g_AllDeviceRes[s8DspModID].pInterf;
		}

		if(g_AllDeviceRes[s8DspModID].pVOIP != NULL)
		{
			delete g_AllDeviceRes[s8DspModID].pVOIP;
		}

		if(g_AllDeviceRes[s8DspModID].pVoice != NULL)
		{
			delete g_AllDeviceRes[s8DspModID].pVoice;
		}
	}

	if(g_cfgAppIsLog)
	{
		char chErrorInfo[1024] = {0};
		sprintf(chErrorInfo, theApp.m_clsLIni.GLoadString("ExitDJKeygoeSuccess"));

		g_pLogInfo->WriteLogInfo(chErrorInfo);
	}
}

void ReadConfig(char *pIniFilePath)
{
	CString strLogDir;
	if(pIniFilePath == NULL)
	{
//		char chMdlFlName[MAX_PATH];
//		GetModuleFileName(NULL, chMdlFlName, MAX_PATH);
//		CString strMdlFlName = chMdlFlName;
//		strMdlFlName.Replace(".exe", ".ini");
//		strcpy(chMdlFlName, strMdlFlName);
//		pIniFilePath = 	chMdlFlName;
		char chValue[MAX_PATH];
		DWORD dwType = REG_SZ, dwValueLen = MAX_PATH;
		memset(chValue, 0, MAX_PATH);
		HKEY hKey;
		RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\DJXMS"), &hKey);
		// RegSetValueEx(hKey, _T("InstallDir"), NULL, REG_SZ, (LPBYTE)chValue, strlen(chValue));
		RegQueryValueEx(hKey, _T("InstallDir"), NULL, &dwType, (LPBYTE)chValue, &dwValueLen);
		RegCloseKey(hKey);

		CString strXMSDir;
		if(dwType == REG_SZ)
		{
			strXMSDir = chValue;
		}
		else
		{
			strXMSDir = _T("C:\\DJKeygoe\\");
		}

		strLogDir = strXMSDir + _T("Log");
		CreateDirectory(strLogDir, NULL);

		strXMSDir += _T("Ini\\DJITPVoIP-1.ini");
		char chMdlFlName[MAX_PATH];
		strcpy(chMdlFlName, strXMSDir);
		pIniFilePath = 	chMdlFlName;
	}
	g_strCfgIni = pIniFilePath;

	// Log File
	CString strLogPath = strLogDir + _T("\\DJIPGateway.log");

	// Read ServerID
	GetPrivateProfileString("XMSCONFIG", "XMSIP", "192.168.10.33", 
		g_cfgServerID.m_s8ServerIp, sizeof(g_cfgServerID.m_s8ServerIp), pIniFilePath);

	g_cfgServerID.m_u32ServerPort = GetPrivateProfileInt("XMSCONFIG", "XMSPORT", 9000, pIniFilePath);

	GetPrivateProfileString("XMSCONFIG", "XMSUSER", "", 
		g_cfgServerID.m_s8UserName, sizeof(g_cfgServerID.m_s8UserName), pIniFilePath);
	GetPrivateProfileString("XMSCONFIG", "XMSPASSWORD", "", 
		g_cfgServerID.m_s8UserPwd, sizeof(g_cfgServerID.m_s8UserPwd), pIniFilePath);

	// Read App Info
	g_cfgAppID = GetPrivateProfileInt("XMSCONFIG", "XMSAPPID", 111, pIniFilePath);
	g_cfgRevQueSize = GetPrivateProfileInt("XMSCONFIG", "XMSRCVQUESIZE", 32, pIniFilePath);
	g_cfgSndQueSize = GetPrivateProfileInt("XMSCONFIG", "XMSSNDQUESIZE", 32, pIniFilePath);
	g_cfgDebugOn = GetPrivateProfileInt("XMSCONFIG", "XMSDEBUGON", 0, pIniFilePath);

	// Read VOIP DSP ID String 如果为空则表示所有都配置为可用
	char chStrTemp[1024] = {0};
	GetPrivateProfileString("XMSCONFIG", "VOIPDSPID", "", chStrTemp, sizeof(chStrTemp), pIniFilePath);
	CString strDspIDTemp;
	int iPos = -1;
	do
	{
		AfxExtractSubString(strDspIDTemp, chStrTemp, ++iPos, ',');
		if( !strDspIDTemp.IsEmpty() )
		{
			int iDspIndex = atoi(strDspIDTemp);
			if( iDspIndex >= 0 && iDspIndex < MAX_DSP_SUM )
			{
				g_cfgiVoipDspID[iDspIndex] = USE_ROUTING_NORMAL;
			}
		}
		else
		{
			break;
		}
	}
	while(iPos < MAX_DSP_SUM);
	if( strlen(chStrTemp) == 0 )
	{
		for(int iDspIndex = 1; iDspIndex < MAX_DSP_SUM; ++iDspIndex)
		{
			g_cfgiVoipDspID[iDspIndex] = USE_ROUTING_NORMAL;
		}
	}

	// Read D DSP ID String 如果为空则表示所有都配置为可用
	GetPrivateProfileString("XMSCONFIG", "DIGIDSPID", "", chStrTemp, sizeof(chStrTemp), pIniFilePath);
	iPos = -1;
	do
	{
		AfxExtractSubString(strDspIDTemp, chStrTemp, ++iPos, ',');
		if( !strDspIDTemp.IsEmpty() )
		{
			CString strTemp1;
			AfxExtractSubString(strTemp1, strDspIDTemp, 0, '-');
			int iDspIndex = atoi(strTemp1);
			if( iDspIndex < 0 || iDspIndex >= MAX_DSP_SUM )
			{
				continue;
			}
			AfxExtractSubString(strTemp1, strDspIDTemp, 1, '-');
			CString strTemp2;
			int iIndex = -1;
			do
			{
				AfxExtractSubString(strTemp2, strTemp1, ++iIndex, '.');
				if( !strTemp2.IsEmpty() )
				{
					int iDspE1Index = atoi(strTemp2);
					if(iDspE1Index >= 0 && iDspE1Index < MAX_DSP_E1_SUM)
					{
						g_cfgiDigiDspID[iDspIndex][iDspE1Index] = USE_ROUTING_NORMAL;
					}
				}
				else
				{
					break;
				}
			}
			while(iIndex < MAX_DSP_E1_SUM);
		}
		else
		{
			break;
		}
	}
	while(iPos < MAX_DSP_SUM);
	if( strlen(chStrTemp) == 0 )
	{
		for(int iDspIndex = 1; iDspIndex < MAX_DSP_SUM; ++iDspIndex)
		{
			for(int iDspE1Index = 0; iDspE1Index < MAX_DSP_E1_SUM; ++iDspE1Index)
			{
				g_cfgiDigiDspID[iDspIndex][iDspE1Index] = USE_ROUTING_NORMAL;
			}
		}
	}

	// Read App Is Write LogFile
	g_cfgAppIsLog = GetPrivateProfileInt("APPCONFIG", "ISWRITELOGFILE", 0, pIniFilePath);
	if(g_cfgAppIsLog)
	{
		g_pLogInfo = new CLogInfo(strLogPath);
	}

	// IP PBX Config Info
	GetPrivateProfileString("IPPBX", "IP", "", g_cfgPBXADDRESS.m_chIPAddr, 
		sizeof(g_cfgPBXADDRESS.m_chIPAddr), pIniFilePath);
	g_cfgPBXADDRESS.m_uiPort = GetPrivateProfileInt("IPPBX", "PORT", 5060, pIniFilePath);
	char chIPPBXProtocol[10];
	memset(chIPPBXProtocol, 0, sizeof(chIPPBXProtocol));
	GetPrivateProfileString("IPPBX", "PBXPROTOCOL", "SIP", chIPPBXProtocol,  sizeof(chIPPBXProtocol), pIniFilePath);
	CString strProtocol = chIPPBXProtocol;
	if(strProtocol.CompareNoCase("SIP") == 0)
	{
		g_cfgPBXADDRESS.m_ushProtocol = XMS_VOIP_PROTOCOL_SIP;
	}
	else if(strProtocol.CompareNoCase("H323") == 0)
	{
		g_cfgPBXADDRESS.m_ushProtocol = XMS_VOIP_PROTOCOL_H323;
	}
	else
	{
		g_cfgPBXADDRESS.m_ushProtocol = XMS_VOIP_PROTOCOL_SIP;
	}

	// Routing Config
	int iRoutingSum = GetPrivateProfileInt("ROUTING", "OutRoutingSum", 0, pIniFilePath);
	g_cfgOutRoutingString.iRoutingSum = iRoutingSum;
	if(g_cfgOutRoutingString.iRoutingSum > 0)
	{
		g_cfgOutRoutingString.pStrRoutingString = new CString[g_cfgOutRoutingString.iRoutingSum+1];
	}
	CString strTemp;
	for(int iRoutingIndex = 1; iRoutingIndex <= iRoutingSum; ++iRoutingIndex)
	{
		strTemp.Format("Out%dCallNum", iRoutingIndex);
		GetPrivateProfileString("ROUTING", strTemp, "", chStrTemp,  sizeof(chStrTemp), pIniFilePath);
		g_cfgOutRoutingString.pStrRoutingString[iRoutingIndex] = chStrTemp;

		strTemp.Format("Out%dE1", iRoutingIndex);
		GetPrivateProfileString("ROUTING", strTemp, "", chStrTemp,  sizeof(chStrTemp), pIniFilePath);
		iPos = -1;
		do
		{
			AfxExtractSubString(strDspIDTemp, chStrTemp, ++iPos, ',');
			if( !strDspIDTemp.IsEmpty() )
			{
				CString strTemp1;
				AfxExtractSubString(strTemp1, strDspIDTemp, 0, '-');
				int iDspIndex = atoi(strTemp1);
				if( iDspIndex < 0 || iDspIndex >= MAX_DSP_SUM )
				{
					continue;
				}
				AfxExtractSubString(strTemp1, strDspIDTemp, 1, '-');
				CString strTemp2;
				int iIndex = -1;
				do
				{
					AfxExtractSubString(strTemp2, strTemp1, ++iIndex, '.');
					if( !strTemp2.IsEmpty() )
					{
						int iDspE1Index = atoi(strTemp2);
						if(iDspE1Index >= 0 && iDspE1Index < MAX_DSP_E1_SUM)
						{
							g_cfgiDigiDspID[iDspIndex][iDspE1Index] = iRoutingIndex;
						}
					}
					else
					{
						break;
					}
				}
				while(iIndex < MAX_DSP_E1_SUM);
			}
			else
			{
				break;
			}
		}
		while(iPos < MAX_DSP_SUM);
	}

	// Change Caller Rule Config VOIP->PSTN CallOut
	g_cfgCCRuleString.iRuleSum = GetPrivateProfileInt("CHANGECALLERRULE", "RuleSum", 0, pIniFilePath);
	if(g_cfgCCRuleString.iRuleSum > 0)
	{
		g_cfgCCRuleString.pStrRuleString = new CString[g_cfgCCRuleString.iRuleSum+1];
	}
	for(int iRuleIndex = 1; iRuleIndex <= g_cfgCCRuleString.iRuleSum; ++iRuleIndex)
	{
		strTemp.Format("Rule%d", iRuleIndex);
		GetPrivateProfileString("CHANGECALLERRULE", strTemp, "", chStrTemp,  sizeof(chStrTemp), pIniFilePath);
		g_cfgCCRuleString.pStrRuleString[iRuleIndex] = chStrTemp;
	}

	// VOIP Routing
	g_cfgVOIPRouting.iRoutingSum = GetPrivateProfileInt("VOIPROUTING", "RoutingSum", 0, pIniFilePath);
	if(g_cfgVOIPRouting.iRoutingSum > 0)
	{
		g_cfgVOIPRouting.pStrRoutingString = new CString[g_cfgVOIPRouting.iRoutingSum+1];
		g_cfgVOIPRouting.pAddress = new PBXADDRESS[g_cfgVOIPRouting.iRoutingSum+1];
	}
	for(int iVOIPRoutingIndex = 1; iVOIPRoutingIndex <= g_cfgVOIPRouting.iRoutingSum; ++iVOIPRoutingIndex)
	{
		strTemp.Format("CallNum%d", iVOIPRoutingIndex);
		GetPrivateProfileString("VOIPROUTING", strTemp, "", chStrTemp,  sizeof(chStrTemp), pIniFilePath);
		g_cfgVOIPRouting.pStrRoutingString[iVOIPRoutingIndex] = chStrTemp;

		strTemp.Format("IP%d", iVOIPRoutingIndex);
		GetPrivateProfileString("VOIPROUTING", strTemp, "", 
			g_cfgVOIPRouting.pAddress[iVOIPRoutingIndex].m_chIPAddr, 
			sizeof(g_cfgVOIPRouting.pAddress[iVOIPRoutingIndex].m_chIPAddr), pIniFilePath);
		strTemp.Format("PORT%d", iVOIPRoutingIndex);
		g_cfgVOIPRouting.pAddress[iVOIPRoutingIndex].m_uiPort = GetPrivateProfileInt("VOIPROUTING", 
			strTemp, 5060, pIniFilePath);
		strTemp.Format("PBXPROTOCOL%d", iVOIPRoutingIndex);
		GetPrivateProfileString("VOIPROUTING", strTemp, "SIP", chIPPBXProtocol,  sizeof(chIPPBXProtocol), pIniFilePath);
		CString strProtocol = chIPPBXProtocol;
		if(strProtocol.CompareNoCase("SIP") == 0)
		{
			g_cfgVOIPRouting.pAddress[iVOIPRoutingIndex].m_ushProtocol = XMS_VOIP_PROTOCOL_SIP;
		}
		else if(strProtocol.CompareNoCase("H323") == 0)
		{
			g_cfgVOIPRouting.pAddress[iVOIPRoutingIndex].m_ushProtocol = XMS_VOIP_PROTOCOL_H323;
		}
		else
		{
			g_cfgVOIPRouting.pAddress[iVOIPRoutingIndex].m_ushProtocol = XMS_VOIP_PROTOCOL_SIP;
		}
	}

	// Change Caller Rule Config PSTN->VOIP CallOut
	g_cfgCCRuleStringEx.iRuleSum = GetPrivateProfileInt("CHANGECALLERRULEEX", "RuleSum", 0, pIniFilePath);
	if(g_cfgCCRuleStringEx.iRuleSum > 0)
	{
		g_cfgCCRuleStringEx.pStrRuleString = new CString[g_cfgCCRuleStringEx.iRuleSum+1];
	}
	for(iRuleIndex = 1; iRuleIndex <= g_cfgCCRuleStringEx.iRuleSum; ++iRuleIndex)
	{
		strTemp.Format("Rule%d", iRuleIndex);
		GetPrivateProfileString("CHANGECALLERRULEEX", strTemp, "", chStrTemp,  sizeof(chStrTemp), pIniFilePath);
		g_cfgCCRuleStringEx.pStrRuleString[iRuleIndex] = chStrTemp;
	}

	// VOIP CallOut RuleType
	g_cfgVOIPCallOutRuleType = GetPrivateProfileInt("VOIPCALLOUTTYPE", "RuleType", VCT_DEFAULT, pIniFilePath);

	// VOIP DTMF TYPE
	g_cfgVOIPDTMFType = GetPrivateProfileInt("VOIPDTMFTYPE", "Type", VDT_RTPDTMF, pIniFilePath);

	// VOIP VOC VALUE 混音音量
	g_cfgVOIPVOCVALUE = GetPrivateProfileInt("VOIPVOC", "Value", 1024, pIniFilePath);
}

DJ_Void	XMSEventProc(DJ_S32 esrParam)
{
	Acs_Evt_t *pAcsEvt = (Acs_Evt_t *)esrParam;
	char chTemp256[256] = {0};
	char chTemp128[128] = {0};

	int iShowEvtInfo = 1;
	switch(pAcsEvt->m_s32EventType)
	{
		case XMS_EVT_QUERY_DEVICE:
			{
				Acs_Dev_List_Head_t *pAcsDevList = (Acs_Dev_List_Head_t *)FetchEventData(pAcsEvt);

				// 收到设备变化的事件 增加/调整该设备资源
				AddDeviceRes(pAcsDevList);
			}
			break;

		case XMS_EVT_QUERY_DEVICE_END:	// 获取设备列表结束
			break;

		case XMS_EVT_QUERY_ONE_DSP_END:	// 一个DSP上的所有资源已经返回完
			{
				g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lUsed = 1;
				OpenOneDspAllDevice(pAcsEvt->m_DeviceID.m_s8ModuleID);
			}
			break;

		case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:	// 一个DSP已被移除
			{
				int s8DspModID = pAcsEvt->m_DeviceID.m_s8ModuleID;

				/* 移除整个DSP的资源信息 */
				// Board
				g_AllDeviceRes[s8DspModID].lFlag = 0;
				g_AllDeviceRes[s8DspModID].lUsed = 0;
				g_AllDeviceRes[s8DspModID].bOpenFlag = 0;
				g_AllDeviceRes[s8DspModID].bErrFlag = 0;
				memset(&g_AllDeviceRes[s8DspModID].deviceID, 0, sizeof(DeviceID_t));

				// Pcm
				if( g_AllDeviceRes[s8DspModID].lPcmNum != 0 )
				{
					if(g_AllDeviceRes[s8DspModID].pPcm != NULL)
					{
						memset(g_AllDeviceRes[s8DspModID].pPcm, 0, 
							g_AllDeviceRes[s8DspModID].lPcmNum*sizeof(PCM_STRUCT));
					}
				}
				g_AllDeviceRes[s8DspModID].lPcmNum = 0;
				g_AllDeviceRes[s8DspModID].lPcmOpened = 0;

				// Interf
				if( g_AllDeviceRes[s8DspModID].lInterfNum != 0 )
				{
					if(g_AllDeviceRes[s8DspModID].pInterf != NULL)
					{
						memset(g_AllDeviceRes[s8DspModID].pInterf, 0, 
							g_AllDeviceRes[s8DspModID].lInterfNum*sizeof(INTERFACE_STRUCT));
					}
				}
				g_AllDeviceRes[s8DspModID].lInterfNum = 0;
				g_AllDeviceRes[s8DspModID].lInterfOpened = 0;

				// VOICE
				if( g_AllDeviceRes[s8DspModID].lVocNum != 0 )
				{
					if(g_AllDeviceRes[s8DspModID].pVoice != NULL)
					{
						memset(g_AllDeviceRes[s8DspModID].pVoice, 0, 
							g_AllDeviceRes[s8DspModID].lVocNum*sizeof(VOICE_STRUCT));
					}
				}
				g_AllDeviceRes[s8DspModID].lVocNum = 0;
				g_AllDeviceRes[s8DspModID].lVocOpened = 0;

				// VOIP
				if( g_AllDeviceRes[s8DspModID].lVOIPNum != 0 )
				{
					if(g_AllDeviceRes[s8DspModID].pVOIP != NULL)
					{
						memset(g_AllDeviceRes[s8DspModID].pVOIP, 0, 
							g_AllDeviceRes[s8DspModID].lVOIPNum*sizeof(VOIP_STRUCT));
					}
				}
				g_AllDeviceRes[s8DspModID].lVOIPNum = 0;
				g_AllDeviceRes[s8DspModID].lVOIPOpened = 0;
			}
			break;

		case XMS_EVT_OPEN_DEVICE:
			{
				OpenDeviceSuccess(&pAcsEvt->m_DeviceID);
				//g_pShowInfo->InitShowInfo();//LIST中初始化只显示VOIP 故此操作放入VOIP CH OPEN SUCCESS中
			}
			break;

		case XMS_EVT_CLOSE_DEVICE:
			// 在删除设备之前，先发给应用程序CloseDevice事件；调用函数XMS_ctsCloseDevicey也会产生本事件
			CloseDeviceSuccess(&pAcsEvt->m_DeviceID);
			break;

		case XMS_EVT_UNIFAILURE:
			// must handle this event in your real System
			iShowEvtInfo = g_cfgDebugOn;
			break;

		case XMS_EVT_DEVICESTATE:
			iShowEvtInfo = g_cfgDebugOn;
			AddDevLineState(pAcsEvt);
			break;

		default:
			iShowEvtInfo = g_cfgDebugOn;
			if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
			{
				ProcVOIPWork(&g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pVOIP[pAcsEvt->m_DeviceID.m_s16ChannelID], pAcsEvt);
			}
			if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH)
			{
				ProcInterfaceWork(&g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pInterf[pAcsEvt->m_DeviceID.m_s16ChannelID], pAcsEvt);
			}
			if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE)
			{
				VOICE_STRUCT *pOneVoice = &g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pVoice[pAcsEvt->m_DeviceID.m_s16ChannelID];
				if (pOneVoice->UsedDevID.m_s8ModuleID > 0)
				{
					if (pOneVoice->UsedDevID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
					{
						VOIP_STRUCT *pOneVOIP = &g_AllDeviceRes[pOneVoice->UsedDevID.m_s8ModuleID].pVOIP[pOneVoice->UsedDevID.m_s16ChannelID];

						if(pOneVOIP->LinkDevID.m_s8ModuleID > 0)
						{
							ProcInterfaceWork(&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID], pAcsEvt);
						}
					}
					else if(pOneVoice->UsedDevID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH)
					{
						ProcInterfaceWork(
							&g_AllDeviceRes[pOneVoice->UsedDevID.m_s8ModuleID].pInterf[pOneVoice->UsedDevID.m_s16ChannelID], pAcsEvt);
					}
				}
			}
			break;
	}

	if(iShowEvtInfo == 1)
	{
		ShowEventInfo(pAcsEvt);
	}

	return;
}

void ShowEventInfo(Acs_Evt_t *pAcsEvt)
{
	if(g_pShowInfo == NULL)
	{
		return;
	}

	char	chTemp256[256] = {0};
	char	chTemp128[128] = {0};
	Acs_Dev_List_Head_t *pAcsDevList = NULL;
	Acs_UniFailure_Data *pAcsUniFailure = NULL;

	sprintf(chTemp256, "EVT SIZE (%4d) : ", pAcsEvt->m_s32EvtSize);
	strcat(chTemp256, GetEventTypeString(pAcsEvt->m_s32EventType));

	switch (pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_OPEN_STREAM:
		break;

	case XMS_EVT_QUERY_DEVICE:
		pAcsDevList = (Acs_Dev_List_Head_t *)FetchEventData(pAcsEvt);
		sprintf(chTemp128, " (%s,%2d,%3d)", 
			GetDeviceMainString(pAcsDevList->m_s32DeviceMain),
			pAcsDevList->m_s32ModuleID,
			pAcsDevList->m_s32DeviceNum);
		strcat(chTemp256, chTemp128);
		break;

	case XMS_EVT_OPEN_DEVICE:
		break;

	case XMS_EVT_UNIFAILURE:
		pAcsUniFailure = (Acs_UniFailure_Data *)FetchEventData(pAcsEvt);
		sprintf( chTemp128, ": %s(0x%X) dev=(%s, %d, %d),  %d ?=? %d+%d", 
			GetErrorCodeString(pAcsUniFailure->m_s32AcsEvtErrCode), pAcsUniFailure->m_s32AcsEvtErrCode,
			GetDeviceMainString(pAcsEvt->m_DeviceID.m_s16DeviceMain), pAcsEvt->m_DeviceID.m_s8ModuleID, 
			pAcsEvt->m_DeviceID.m_s16ChannelID,
			pAcsEvt->m_s32EvtSize, sizeof(Acs_Evt_t), sizeof(Acs_UniFailure_Data) 
			);
		strcat(chTemp256, chTemp128);
		break;

	default:
		break;
	}

	g_pShowInfo->ShowInfo(chTemp256);

	return;
}

char* GetEventTypeString(EventType_t EvtType)
{
	switch(EvtType)
	{
	case XMS_EVT_OPEN_STREAM:			return "XMS_EVT_OPEN_STREAM";
	case XMS_EVT_QUERY_DEVICE:			return "XMS_EVT_QUERY_DEVICE";
	case XMS_EVT_QUERY_DEVICE_END:		return "XMS_EVT_QUERY_DEVICE_END";
	case XMS_EVT_OPEN_DEVICE:			return "XMS_EVT_OPEN_DEVICE";
	case XMS_EVT_CLOSE_DEVICE:			return "XMS_EVT_CLOSE_DEVICE";
	case XMS_EVT_RESET_DEVICE:			return "XMS_EVT_RESET_DEVICE";
	case XMS_EVT_DEVICESTATE:			return "XMS_EVT_DEVICESTATE";
	case XMS_EVT_SETDEV_GROUP:			return "XMS_EVT_SETDEV_GROUP";
	case XMS_EVT_SETPARAM:				return "XMS_EVT_SETPARAM";
	case XMS_EVT_GETPARAM:				return "XMS_EVT_GETPARAM";
	case XMS_EVT_QUERY_ONE_DSP_START:	return "XMS_EVT_QUERY_ONE_DSP_START";
	case XMS_EVT_QUERY_ONE_DSP_END:		return "XMS_EVT_QUERY_ONE_DSP_END";

	case XMS_EVT_CALLOUT:				return "XMS_EVT_CALLOUT";
	case XMS_EVT_CALLIN:				return "XMS_EVT_CALLIN";
	case XMS_EVT_ALERTCALL:				return "XMS_EVT_ALERTCALL";
	case XMS_EVT_ANSWERCALL:			return "XMS_EVT_ANSWERCALL";
	case XMS_EVT_LINKDEVICE:			return "XMS_EVT_LINKDEVICE";
	case XMS_EVT_UNLINKDEVICE:			return "XMS_EVT_UNLINKDEVICE";
	case XMS_EVT_CLEARCALL:				return "XMS_EVT_CLEARCALL";
	case XMS_EVT_JOINTOCONF:			return "XMS_EVT_JOINTOCONF";
	case XMS_EVT_LEAVEFROMCONF:			return "XMS_EVT_LEAVEFROMCONF";
	case XMS_EVT_CLEARCONF:				return "XMS_EVT_CLEARCONF";

	case XMS_EVT_PLAY:					return "XMS_EVT_PLAY";
	case XMS_EVT_INITINDEX:				return "XMS_EVT_INITINDEX";
	case XMS_EVT_BUILDINDEX:			return "XMS_EVT_BUILDINDEX";
	case XMS_EVT_CONTROLPLAY:			return "XMS_EVT_CONTROLPLAY";
	case XMS_EVT_RECORD:				return "XMS_EVT_RECORD";
	case XMS_EVT_CONTROLRECORD:			return "XMS_EVT_CONTROLRECORD";

	case XMS_EVT_SENDFAX:				return "XMS_EVT_SENDFAX";
	case XMS_EVT_RECVFAX:				return "XMS_EVT_RECVFAX";
	
	case XMS_EVT_SENDIODATA:			return "XMS_EVT_SENDIODATA";
	case XMS_EVT_RECVIODATA:			return "XMS_EVT_RECVIODATA";

	case XMS_EVT_CHGMONITORFILTER:		return "XMS_EVT_CHGMONITORFILTER";

	case XMS_EVT_UNIFAILURE:			return "XMS_EVT_UNIFAILURE";
	}

	return "UNKNOWN";
}

char* GetDeviceMainString(DJ_S32 s32DeviceMain)
{
	switch ( s32DeviceMain )
	{
	case XMS_DEVMAIN_VOICE:				return "VOC  ";
	case XMS_DEVMAIN_FAX:				return "FAX  ";
	case XMS_DEVMAIN_DIGITAL_PORT:		return "DIGIT";
	case XMS_DEVMAIN_INTERFACE_CH:		return "INTCH";
	case XMS_DEVMAIN_DSS1_LINK:			return "DSS1 ";
	case XMS_DEVMAIN_SS7_LINK:			return "SS7  ";
	case XMS_DEVMAIN_BOARD:				return "BRD  ";
	case XMS_DEVMAIN_CTBUS_TS:			return "CTBUS";
	case XMS_DEVMAIN_VOIP:				return "VOIP ";
	case XMS_DEVMAIN_CONFERENCE:		return "CONF ";
	case XMS_DEVMAIN_VIDEO:				return "VIDEO";
	}

	return "UNKNOWN";
}

char* GetErrorCodeString(DJ_S32 s32ErrorCode)
{
	switch ( s32ErrorCode )
	{
	case ACSERR_LOADLIBERR:					return "LOADLIBERR";
	case ACSERR_BADPARAMETER:				return "BADPARAMETER";
	case ACSERR_NOSERVER:					return "NOSERVER";
	case ACSERR_MAXCONN_EXCEED:				return "MAXCONN_EXCEED";
	case ACSERR_BADHDL:						return "ABADHDL";

	case ACSERR_FAILSEND:					return "FAILSEND";
	case ACSERR_LINKBROKEN:					return "LINKBROKEN";
	case ACSERR_NOMESSAGE:					return "NOMESSAGE";
	case ACSERR_GETRESFAIL:					return "GETRESFAIL";
	case ACSERR_NOSUPPORTCMD:				return "NOSUPPORTCMD";
	}

	return "UNKNOWN";
}

void AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32 s32Type = pAcsDevList->m_s32DeviceMain;
	DJ_S32 s32Num = pAcsDevList->m_s32DeviceNum;

	DJ_S8 s8DspModID = (DJ_S8)pAcsDevList->m_s32ModuleID;
	if( (s8DspModID < 0) || (s8DspModID >= MAX_DSP_SUM) )
	{
		return;
	}

	switch(s32Type)
	{
	case XMS_DEVMAIN_VOICE:
		AddDeviceVoiceRes(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_FAX:
		break;

	case XMS_DEVMAIN_DIGITAL_PORT:
		AddDevicePcmRes(s8DspModID, pAcsDevList);
		break;
	
	case XMS_DEVMAIN_INTERFACE_CH:
		AddDeviceInterfaceRes(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_DSS1_LINK:
		break;

	case XMS_DEVMAIN_SS7_LINK:
		break;

	case XMS_DEVMAIN_BOARD:
		AddDeviceBoardRes(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_CTBUS_TS:
		break;

	case XMS_DEVMAIN_VOIP:
		AddDeviceVOIPRes(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_CONFERENCE:
		break;

	case XMS_DEVMAIN_VIDEO:
		break;

	default:
		break;
	}
}

void AddDeviceInterfaceRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num = pAcsDevList->m_s32DeviceNum;
	char	chTemp256[256] = {0};

	if( (g_AllDeviceRes[s8DspModID].lInterfNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		if( g_AllDeviceRes[s8DspModID].pInterf != NULL )
		{
			delete []g_AllDeviceRes[s8DspModID].pInterf;
		}
		g_AllDeviceRes[s8DspModID].pInterf = new INTERFACE_STRUCT[s32Num];
		if( g_AllDeviceRes[s8DspModID].pInterf == NULL )
		{
			g_AllDeviceRes[s8DspModID].lInterfNum = 0;
			g_AllDeviceRes[s8DspModID].lInterfOpened = 0;

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Interface DSPID%d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, 
					theApp.m_clsLIni.GLoadString("Fail"));

				g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ERROR);
			}
		}
		else
		{
			g_AllDeviceRes[s8DspModID].lInterfNum = s32Num;
			g_AllDeviceRes[s8DspModID].lInterfOpened = 0;

			DeviceID_t *pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for(int i = 0; i < s32Num; ++i)
			{
				g_AllDeviceRes[s8DspModID].pInterf[i].deviceID = pDev[i];
				switch(g_AllDeviceRes[s8DspModID].pInterf[i].deviceID.m_s16DeviceSub)
				{
				case XMS_DEVSUB_ANALOG_USER:	// 内线
					{
					}
					break;
				case XMS_DEVSUB_ANALOG_TRUNK:	// 外线
					{
					}
					break;

				case XMS_DEVSUB_E1_SS7_TUP:
				case XMS_DEVSUB_E1_SS7_ISUP:
				case XMS_DEVSUB_E1_DSS1:
				case XMS_DEVSUB_E1_CAS:			// 数字
					{
						g_AllDeviceRes[s8DspModID].pInterf[i].iState = TRK_WAITOPEN;
					}
					break;
				}
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Interface DSPID%d Num = %d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, s32Num, 
					theApp.m_clsLIni.GLoadString("Success"));

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
	}
	else
	{
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Interface DSPID%d) %s", theApp.m_clsLIni.GLoadString("AddRes"), 
				s8DspModID, theApp.m_clsLIni.GLoadString("ResAlreadyExists"));

			g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
		}
	}
}

void AddDeviceVOIPRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num = pAcsDevList->m_s32DeviceNum;
	char	chTemp256[256] = {0};

	if( (g_AllDeviceRes[s8DspModID].lVOIPNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		if(g_AllDeviceRes[s8DspModID].pVOIP != NULL)
		{
			delete []g_AllDeviceRes[s8DspModID].pVOIP;
		}
		g_AllDeviceRes[s8DspModID].pVOIP = new VOIP_STRUCT[s32Num];
		if( g_AllDeviceRes[s8DspModID].pVOIP == NULL )
		{
			g_AllDeviceRes[s8DspModID].lVOIPNum = 0;
			g_AllDeviceRes[s8DspModID].lVOIPOpened = 0;

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (VOIP DSPID%d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, 
					theApp.m_clsLIni.GLoadString("Fail"));

				g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ERROR);
			}
		}
		else
		{
			g_AllDeviceRes[s8DspModID].lVOIPNum = s32Num;
			g_AllDeviceRes[s8DspModID].lVOIPOpened = 0;

			DeviceID_t *pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for(int i = 0; i < s32Num; ++i)
			{
				g_AllDeviceRes[s8DspModID].pVOIP[i].deviceID = pDev[i];
				g_AllDeviceRes[s8DspModID].pVOIP[i].iState = VOIP_WAITOPEN;
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (VOIP DSPID%d Num = %d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, s32Num, 
					theApp.m_clsLIni.GLoadString("Success"));

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
	}
	else
	{
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (VOIP DSPID%d) %s", theApp.m_clsLIni.GLoadString("AddRes"), 
				s8DspModID, theApp.m_clsLIni.GLoadString("ResAlreadyExists"));

			g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
		}
	}
}

void AddDeviceVoiceRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num = pAcsDevList->m_s32DeviceNum;
	char	chTemp256[256] = {0};

	if( (g_AllDeviceRes[s8DspModID].lVocNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		if(g_AllDeviceRes[s8DspModID].pVoice != NULL)
		{
			delete []g_AllDeviceRes[s8DspModID].pVoice;
		}
		g_AllDeviceRes[s8DspModID].pVoice = new VOICE_STRUCT[s32Num];
		if( g_AllDeviceRes[s8DspModID].pVoice == NULL )
		{
			g_AllDeviceRes[s8DspModID].lVocNum = 0;
			g_AllDeviceRes[s8DspModID].lVocOpened = 0;
			//g_AllDeviceRes[s8DspModID].lVocFreeNum = 0;

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Voice DSPID%d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, 
					theApp.m_clsLIni.GLoadString("Fail"));

				g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ERROR);
			}
		}
		else
		{
			g_AllDeviceRes[s8DspModID].lVocNum = s32Num;
			g_AllDeviceRes[s8DspModID].lVocOpened = 0;
			//g_AllDeviceRes[s8DspModID].lVocFreeNum = 0;

			DeviceID_t *pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for(int i = 0; i < s32Num; ++i)
			{
				g_AllDeviceRes[s8DspModID].pVoice[i].deviceID = pDev[i];
				g_AllDeviceRes[s8DspModID].pVoice[i].iState = VOC_WAITOPEN;
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Voice DSPID%d Num = %d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, s32Num, 
					theApp.m_clsLIni.GLoadString("Success"));

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
	}
	else
	{
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Voice DSPID%d) %s", theApp.m_clsLIni.GLoadString("AddRes"), 
				s8DspModID, theApp.m_clsLIni.GLoadString("ResAlreadyExists"));

			g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
		}
	}
}

void AddDevicePcmRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num = pAcsDevList->m_s32DeviceNum;
	char	chTemp256[256] = {0};

	if( (g_AllDeviceRes[s8DspModID].lPcmNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		if(g_AllDeviceRes[s8DspModID].pPcm != NULL)
		{
			delete []g_AllDeviceRes[s8DspModID].pPcm;
		}
		g_AllDeviceRes[s8DspModID].pPcm = new PCM_STRUCT[s32Num];
		if( g_AllDeviceRes[s8DspModID].pPcm == NULL )
		{
			g_AllDeviceRes[s8DspModID].lPcmNum = 0;
			g_AllDeviceRes[s8DspModID].lPcmOpened = 0;

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Pcm DSPID%d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, 
					theApp.m_clsLIni.GLoadString("Fail"));

				g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ERROR);
			}
		}
		else
		{
			g_AllDeviceRes[s8DspModID].lPcmNum = s32Num;
			g_AllDeviceRes[s8DspModID].lPcmOpened = 0;

			DeviceID_t *pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			for(int i = 0; i < s32Num; ++i)
			{
				g_AllDeviceRes[s8DspModID].pPcm[i].deviceID = pDev[i];
				g_AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag = false;
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Pcm DSPID%d Num = %d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, s32Num, 
					theApp.m_clsLIni.GLoadString("Success"));

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
	}
	else
	{
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Pcm DSPID%d) %s", theApp.m_clsLIni.GLoadString("AddRes"), 
				s8DspModID, theApp.m_clsLIni.GLoadString("ResAlreadyExists"));

			g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
		}
	}
}

void AddDeviceBoardRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num = pAcsDevList->m_s32DeviceNum;
	char	chTemp256[256] = {0};

	if( g_AllDeviceRes[s8DspModID].lFlag == 0 )		// 新增加的资源
	{
		g_AllDeviceRes[s8DspModID].lFlag = 1;
		DeviceID_t *pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

		g_AllDeviceRes[s8DspModID].deviceID = pDev[0];

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Board DSPID%d) %s", 
					theApp.m_clsLIni.GLoadString("AddRes"), s8DspModID, 
					theApp.m_clsLIni.GLoadString("Success"));

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}
	else
	{
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Board DSPID%d) %s", theApp.m_clsLIni.GLoadString("AddRes"), 
				s8DspModID, theApp.m_clsLIni.GLoadString("ResAlreadyExists"));

			g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
		}
	}
}

BOOL CfgIsUseDsp(DJ_S8 s8ModID)
{
	if( g_cfgiVoipDspID[s8ModID] > 0 )
	{
		return TRUE;
	}

	for(int iDspE1Index = 0; iDspE1Index < MAX_DSP_E1_SUM; ++iDspE1Index)
	{
		if( g_cfgiDigiDspID[s8ModID][iDspE1Index] > 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CfgIsUsePcm(DJ_S8 s8ModID, DJ_S8 sPcmID)
{
	if( g_cfgiDigiDspID[s8ModID][sPcmID] > 0 )
	{
			return TRUE;
	}

	return FALSE;
}

BOOL CfgIsUseDigi(DJ_S8 s8ModID, int iDspE1Index)
{
	if( g_cfgiDigiDspID[s8ModID][iDspE1Index] > 0 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CfgIsUseVOIP(DJ_S8 s8ModID)
{
	if( g_cfgiVoipDspID[s8ModID] > 0 )
	{
		return TRUE;
	}

	return FALSE;
}

void OpenOneDspAllDevice(DJ_S8 s8DspModID)
{
	char	chTemp256[256] = {0};

	// Open Board
    if( CfgIsUseDsp(s8DspModID) )
	{
		if( g_AllDeviceRes[s8DspModID].bOpenFlag == false )
		{
			XMS_ctsOpenDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].deviceID, NULL);
		}

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Board DSPID%d) ", theApp.m_clsLIni.GLoadString("BeginOpen"), s8DspModID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}
	else
	{
		return;
	}

	// Open Trunk
	for(int i = 0; i < g_AllDeviceRes[s8DspModID].lPcmNum; ++i)
	{
		if( CfgIsUsePcm(s8DspModID, i) )
		{
			// pPcm
			if(g_AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag == false)
			{
				XMS_ctsOpenDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pPcm[i].deviceID, NULL);
			}

			// pInterf
			for(int j = 32*i; j < g_AllDeviceRes[s8DspModID].lInterfNum && j < 32*(i+1); ++j)
			{
				XMS_ctsOpenDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pInterf[j].deviceID, NULL);
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s (Pcm&Interface DSPID%d %d)", 
					theApp.m_clsLIni.GLoadString("BeginOpen"), s8DspModID, i);

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
	}

	// Open VOIP & Voice
	if( CfgIsUseVOIP(s8DspModID) )
	{
		for(int i = 0; i < g_AllDeviceRes[s8DspModID].lVOIPNum; ++i)
		{
			XMS_ctsOpenDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pVOIP[i].deviceID, NULL );
			XMS_ctsOpenDevice(g_acsHandle, &g_AllDeviceRes[s8DspModID].pVoice[i].deviceID, NULL);

			memcpy(&g_AllDeviceRes[s8DspModID].pVOIP[i].VocDevID, 
				&g_AllDeviceRes[s8DspModID].pVoice[i].deviceID, sizeof(DeviceID_t));
			memcpy(&g_AllDeviceRes[s8DspModID].pVoice[i].UsedDevID, 
				&g_AllDeviceRes[s8DspModID].pVOIP[i].deviceID, sizeof(DeviceID_t));
		}

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (VOIP&Voice DSPID%d)", theApp.m_clsLIni.GLoadString("BeginOpen"), s8DspModID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}
}

void OpenDeviceSuccess(DeviceID_t *pDevice)
{
	VOIP_STRUCT	 *pOneVOIP = NULL;
	INTERFACE_STRUCT *pOneTrunk = NULL;
	PCM_STRUCT   *pOnePcm = NULL;
	VOICE_STRUCT *pOneVoice = NULL;
	char chTemp256[256] = {0};

	if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD)
	{
		g_AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = true;
		g_AllDeviceRes[pDevice->m_s8ModuleID].lUsed = 1;
		g_AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Board DSPID%d)", theApp.m_clsLIni.GLoadString("OpenSuccess"), pDevice->m_s8ModuleID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH)
	{
		pOneTrunk = &g_AllDeviceRes[pDevice->m_s8ModuleID].pInterf[pDevice->m_s16ChannelID];

		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;

		pOneTrunk->iState = TRK_FREE;

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);
		g_AllDeviceRes[pDevice->m_s8ModuleID].lInterfOpened++;

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Interface DSPID%d ChannelID%d)", 
				theApp.m_clsLIni.GLoadString("OpenSuccess"), pDevice->m_s8ModuleID, pDevice->m_s16ChannelID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP)
	{
		pOneVOIP = &g_AllDeviceRes[pDevice->m_s8ModuleID].pVOIP[pDevice->m_s16ChannelID];
		pOneVOIP->deviceID.m_CallID = pDevice->m_CallID;
		g_AllDeviceRes[pDevice->m_s8ModuleID].pVoice[pDevice->m_s16ChannelID].UsedDevID.m_CallID = pDevice->m_CallID;

		pOneVOIP->iState = VOIP_FREE;

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);	

		g_AllDeviceRes[pDevice->m_s8ModuleID].lVOIPOpened++;

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (VOIP DSPID%d ChannelID%d)", 
				theApp.m_clsLIni.GLoadString("OpenSuccess"), 
				pDevice->m_s8ModuleID, pDevice->m_s16ChannelID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}

		g_pShowInfo->InitShowInfo();	//...
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE)
	{
		pOneVoice = &g_AllDeviceRes[pDevice->m_s8ModuleID].pVoice[pDevice->m_s16ChannelID];
		pOneVoice->deviceID.m_CallID = pDevice->m_CallID;
		g_AllDeviceRes[pDevice->m_s8ModuleID].pVOIP[pDevice->m_s16ChannelID].VocDevID.m_CallID = pDevice->m_CallID;

		pOneVoice->iState = VOC_FREE;

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);

		g_AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened++;
		//g_AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum++;

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Voice DSPID%d ChannelID%d)", 
				theApp.m_clsLIni.GLoadString("OpenSuccess"), 
				pDevice->m_s8ModuleID, pDevice->m_s16ChannelID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT)
	{
		pOnePcm = &g_AllDeviceRes[pDevice->m_s8ModuleID].pPcm[pDevice->m_s16ChannelID];

		pOnePcm->deviceID.m_CallID = pDevice->m_CallID;
		pOnePcm->bOpenFlag = true;

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);

		g_AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened++;

		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s (Pcm DSPID%d PcmID%d)", 
				theApp.m_clsLIni.GLoadString("OpenSuccess"), 
				pDevice->m_s8ModuleID, pDevice->m_s16ChannelID);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}

	return;
}

void CloseDeviceSuccess(DeviceID_t* pDevice)
{
	INTERFACE_STRUCT	*pOneInterf = NULL;
	VOIP_STRUCT         *pOneVOIP = NULL;

	g_AllDeviceRes[pDevice->m_s8ModuleID].bErrFlag = true;

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		g_AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = false;
	}
	else if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneInterf = &g_AllDeviceRes[pDevice->m_s8ModuleID].pInterf[pDevice->m_s16ChannelID];

		switch(pOneInterf->deviceID.m_s16DeviceSub)
		{
		case XMS_DEVSUB_ANALOG_USER:
			break;

		case XMS_DEVSUB_ANALOG_TRUNK:
			break;

		default:
			pOneInterf->iState = TRK_WAITOPEN;
			break;
		}

		g_AllDeviceRes[pDevice->m_s8ModuleID].lInterfOpened--;
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP)
	{
		pOneVOIP = &g_AllDeviceRes[pDevice->m_s8ModuleID].pVOIP[pDevice->m_s16ChannelID];

		pOneVOIP->iState = VOIP_WAITOPEN;
		g_AllDeviceRes[pDevice->m_s8ModuleID].lVOIPOpened --;
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE)
	{
		g_AllDeviceRes[pDevice->m_s8ModuleID].pVoice[pDevice->m_s16ChannelID].iState = VOC_WAITOPEN;

		g_AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened--;
		//g_AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum--;
	}
	else if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT)
	{
		g_AllDeviceRes[pDevice->m_s8ModuleID].pPcm[pDevice->m_s16ChannelID].bOpenFlag = false;

		g_AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened--;
	}

	return;	
}

void AddDevLineState(Acs_Evt_t *pAcsEvt)
{
	VOIP_STRUCT	 *pOneVOIP;
	INTERFACE_STRUCT *pOneInterf;
	PCM_STRUCT   *pOnePcm;
	Acs_GeneralProc_Data *pGeneralData = NULL;

	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneInterf = &g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pInterf[pAcsEvt->m_DeviceID.m_s16ChannelID];
		pOneInterf->iLineState = pGeneralData->m_s32DeviceState;
	}
	else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pPcm[pAcsEvt->m_DeviceID.m_s16ChannelID];
		pOnePcm->u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		pOnePcm->s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);
	}
	else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		pOneVOIP = &g_AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pVOIP[pAcsEvt->m_DeviceID.m_s16ChannelID];
		pOneVOIP->iLineState = pGeneralData->m_s32DeviceState;
	}

	return;
}
