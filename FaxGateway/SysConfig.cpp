#include "StdAfx.h"
#include "SysConfig.h"
#include "PBXGetWay.h"
#include "PBXGetWayDlg.h"
#include "resource.h"
#include <winsock2.h>
#include "globe.h"

#pragma comment( lib, "WS2_32.lib" )

extern CPBXGetWayDlg  *pdlg;

// var about XMS_Demo.INI
char		cfg_IniName[MAX_FILE_NAME_LEN] = {0};
char		cfg_IniShortName[] = "\\PBXGetWay.ini";

ServerID_t	cfg_ServerID;
int         cfg_iVoipWorkMod[256] = {0};
int         cfg_iDTrkWorkMod[256] = {0};
char        cfg_chPartWorkModuleID[256] = {0};

unsigned int         cfg_FaxMode = 1; //T.30透传

int			pubSendRTPDTMFType = 1;
int			pubDebug = 0 ;
char		pubOutVoipAddress[255]={0};
int			pubOutVoipPort=0;
int			pubVoipRule = 0 ;			//呼出规则

long		pubCallInCount = 0;
long		pubNormalClearCallCount = 0;
long		pubErrorClearCallCount = 0;
long		pubIpSendClearCallCount = 0;
long		pubIpSS7ConnectCount = 0;
long		pubSS7IpConnectCount = 0;
long		pubIpSS7ErrorCount = 0;
long		pubSS7IpErrorCount = 0;

long		pubGloalIndex = 1;
BOOL        bLog = 0;

extern      DJ_U8  g_u8UnitID;

int    cfg_IpPool = 0;
char   **pIpPool = NULL; // IP ADDR
char   **pIpPoolItem = NULL; //IP KEY

//char CodeRule[256] = {0};
PtrRule pDailRuleHead = NULL;

int cfg_routCnt = 0;
OutBoundRout *outRout = NULL;

int My_IpPoolItem(int iItemCnt)
{	
	int i;
	pIpPoolItem = new char* [iItemCnt];
	if (pIpPoolItem == NULL)
		return -1;

	for (i = 0; i < iItemCnt; i++)
	{
		pIpPoolItem[i] = new char [256];	
		
		if (pIpPoolItem[i] != NULL)
			sprintf(pIpPoolItem[i], "IP[%d]", i+1);			
	}

	return 0;
}

void My_DeleteIpPoolItem(int iItemCnt)
{
	int i;

	for (i = 0; i < iItemCnt; i++)
	{
		delete [] pIpPoolItem[i];
		pIpPoolItem[i] = NULL;
	}

	delete [] pIpPoolItem;
	pIpPoolItem = NULL;
}

int My_IpPool(int iItemCnt)
{
	pIpPool = new char * [iItemCnt];
	if (pIpPool == NULL)
		return -1;

	for (int i = 0; i < iItemCnt; i++)
	{
		pIpPool[i] = new char [20];
	}

	return 0;
}

void My_DeleteIpPool(int iItemCnt)
{
	int i;

	for (i = 0; i < iItemCnt; i++)
	{
		delete [] pIpPool[i];
		pIpPool[i] = NULL;
	}

	delete [] pIpPool;
	pIpPool = NULL;
}


void My_DeleteAllMem()
{
	My_DeleteIpPoolItem(cfg_IpPool);
	My_DeleteIpPool(cfg_IpPool);
}

int SplitStr2Int(DJ_S8* str, DJ_S8* sep, int buf[])
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

void My_DeleteRout()
{
	if (outRout != NULL)
		delete [] outRout;
	outRout = NULL;
}

void  ReadVOIPConfig(void)
{
	int   i;
	DJ_S8 sep[] = {","};
	char  buf[MAX_PREFIX][256] = {(0,0)};
	char  *p = NULL;
	char  *pBuf = NULL;
	char  chTmp[MAX_SEC]={0};
	char  strTmp[256]={0};
	
	/*read sysconfig from PBXGetWay.ini*/
	
	GetCurrentDirectory(MAX_FILE_NAME_LEN-32, cfg_IniName);
	strcat(cfg_IniName, cfg_IniShortName);

	GetPrivateProfileString("XMSCONFIG", "XMSIP", "192.168.10.132", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);

	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt("XMSCONFIG", "XMSPORT", 9000, cfg_IniName);

	GetPrivateProfileString("XMSCONFIG","XMSUSER","",cfg_ServerID.m_s8UserName,sizeof(cfg_ServerID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("XMSCONFIG","XMSPASSWORD","",cfg_ServerID.m_s8UserPwd,sizeof(cfg_ServerID.m_s8UserPwd),cfg_IniName);
	g_u8UnitID = GetPrivateProfileInt("XMSCONFIG", "UNITID", 40, cfg_IniName);
	/*read work moduleID form PBXGetWay.ini*/
	GetPrivateProfileString("XMSCONFIG", "VOIPWORK","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(strTmp, cfg_chPartWorkModuleID, sizeof(strTmp));	
	SplitStr2Int(strTmp, ",", cfg_iVoipWorkMod);	

	GetPrivateProfileString("XMSCONFIG", "DIGTRKWORK","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(strTmp, cfg_chPartWorkModuleID, sizeof(strTmp));	
	SplitStr2Int(strTmp, ",", cfg_iDTrkWorkMod);
	
	
	/*read ip pool from PBXGetWay.ini*/
	cfg_IpPool = GetPrivateProfileInt("IPPOOL", "ITEMCNT", 1, cfg_IniName);
	My_IpPoolItem(cfg_IpPool);
	My_IpPool(cfg_IpPool);		

	for (i = 0; i < cfg_IpPool; i++)
	{
		if (pIpPool[i] != NULL)
			GetPrivateProfileString("IPPOOL",pIpPoolItem[i] , "", (LPTSTR )pIpPool[i], 20, cfg_IniName);
	}

	pubSendRTPDTMFType = GetPrivateProfileInt("RTPDTMF","SENDTYPE",1,cfg_IniName);
	pubDebug = GetPrivateProfileInt("XMSCONFIG","DEBUGON",0,cfg_IniName);

	GetPrivateProfileString("OUTBONDVOIP","VOIP_ADDRESS","",pubOutVoipAddress,sizeof(pubOutVoipAddress),cfg_IniName);
	pubOutVoipPort=GetPrivateProfileInt("OUTBONDVOIP","VOIP_PORT",5060,cfg_IniName);

	//VOIP输出规则
	pubVoipRule=GetPrivateProfileInt("VOIPRULE","OUTRULE",0,cfg_IniName);

	//日志开关
	bLog = GetPrivateProfileInt("LOG", "LOG", 0, cfg_IniName);
	
//	GetPrivateProfileString("DIALRULE", "DIALRULE", "", CodeRule, sizeof(CodeRule), cfg_IniName);    

	cfg_routCnt = GetPrivateProfileInt("OUTBOUNDROUT", "ROUTCNT", 1, cfg_IniName);
	if (cfg_routCnt > 0)
	{
		
		outRout = new OutBoundRout[cfg_routCnt+1];
		memset(outRout, 0, sizeof(OutBoundRout) * (cfg_routCnt+1));
		if (outRout != NULL)
		{
			char routStr[10] = {0};
			char outRoutStr[512] = {0};
			for (i = 1; i <= cfg_routCnt; i++)
			{
				sprintf(routStr, "ROUT[%d]", i);
				GetPrivateProfileString("OUTBOUNDROUT", routStr ,"",outRoutStr,sizeof(OutBoundRout),cfg_IniName);
    			assignmentOutBoundRout(outRoutStr, &outRout[i]);	
			}
		}
	}

	cfg_FaxMode = GetPrivateProfileInt("FAXMODE", "FaxMode", 0, cfg_IniName);

	CreateDailRule(&pDailRuleHead);	


	return ;
}

void assignmentOutBoundRout(char *outBoundRoutStr, OutBoundRout *outRout)
{	
	char delim [] = {':', ',', '\0'};

	char *p = NULL;

	int strCnt = 0;
	int preCnt = 0;
	
//	memset(outRout, 0, sizeof(OutBoundRout));
	p = strtok(outBoundRoutStr, delim);
	while(p != NULL)
	{
		if (strCnt == 0)
		{
			strncpy(outRout->outBAddr.proxyAddr, p, sizeof(outRout->outBAddr.proxyAddr));
		}
		else if (strCnt == 1)
		{
			outRout->outBAddr.proxyPort = atol(p);
		}
		else
		{
			if (preCnt < 10)
			{
				if (strlen(p) < 10)
				{
					strncpy(outRout->phonePreNum[preCnt], p, sizeof(outRout->phonePreNum[preCnt]));
					preCnt++;		
				}
			}
			else
				break;
		}
		
		p = strtok(NULL, delim);
		strCnt++;
	}

	p = NULL;
	
}

void InitTextBox(void)
{
	char	TmpStr[256] = {0};

	pdlg->GetDlgItem(IDC_EDIT_IPADDR)->SetWindowText(cfg_ServerID.m_s8ServerIp);

	sprintf(TmpStr, "%d", cfg_ServerID.m_u32ServerPort);
	pdlg->GetDlgItem(IDC_EDIT_PORT)->SetWindowText(TmpStr);
}

BOOL IsValidIP(char* IPAddr)
{
	return true;

	bool  bUnValideIP = FALSE;
	unsigned int   iIpAddr = 0;

	iIpAddr = inet_addr(IPAddr);

	for (int i = 0; i < cfg_IpPool; i++)
	{
		if (iIpAddr == inet_addr(pIpPool[i]))
			return bUnValideIP = TRUE;	
	}

	return bUnValideIP;
}


bool	JudgeDspIsValid(DJ_S8	 mDspID)
{
	bool	mResult=0;
	
	return 1;
}


int CreateDailRule(PtrRule *ppDailRule)				 
{
	char CodeRule[256] = {0};
	DWORD dwSize = 0;
	int i = 0;

	char chTmp[] = "DIALRULE";
	char chDialList[12] = {0};	
	PtrRule pDialRule = NULL;
	

	pDialRule = new ListRule;
	memset(pDialRule, 0, sizeof(ListRule));
	
	if (pDialRule == NULL)
	{
		printf("Out of space\n");
		return 0;
	}

	*ppDailRule = pDialRule;

	do {
		i++;
		sprintf(chDialList, "%s[%d]", chTmp, i);
		dwSize = GetPrivateProfileString("DIALRULE", chDialList, "", CodeRule, sizeof(CodeRule), cfg_IniName);
		
		if (dwSize > 0)
		{
			Insert(CodeRule, pDialRule, pDialRule);
		}
		
	} while(dwSize > 0);



	return 0;
}