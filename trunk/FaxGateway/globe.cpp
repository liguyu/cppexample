#include "StdAfx.h"
#include "globe.h"
#include "DTrunk.h"
#include "VOIP.H"
#include "XMS_Run.h"
#include "PBXGetWay.h"
#include "PBXGetWayDlg.h"
#include "XMS_String.h"
#include "SysConfig.h"
#include <time.h>
#include <stdlib.h>

FILE * g_logfp = NULL;
extern BOOL bLog;

extern CPBXGetWayApp theApp;
CPBXGetWayDlg  *pdlg = NULL;
extern int pubDebug;
extern	int pubVoipRule;
extern char		cfg_IniName[MAX_FILE_NAME_LEN] ;

ACSHandle_t g_acsHandle;

extern ServerID_t	cfg_ServerID;
DJ_U8  g_u8UnitID = 40;

extern int         cfg_iVoipWorkMod[256];
extern int         cfg_iDTrkWorkMod[256];
extern char        cfg_chPartWorkModuleID[256];
extern long	 pubGloalIndex;

extern PtrRule pDailRuleHead;

TYPE_XMS_DSP_DEVICE_RES	AllDeviceRes[MAX_DSP_MODULE_NUM];

int						g_iTotalModule = 0;
DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUM];

int						g_iTotalPcm = 0;
int						g_iTotalPcmOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM];

int						g_iTotalDTrunk = 0;
int						g_iTotalDTrunkOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_DTrunk[MAX_INTERFACE_NUM];

int						g_iTotalVOIP = 0;
int						g_iTotalVOIPOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_VOIP[MAX_VOIP_NUM];

int						g_iTotalVoice = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_VOICE_NUM];

int						g_iTotalFax = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Fax[MAX_VOICE_NUM];

CRITICAL_SECTION cs;

BOOL IsDigitMod(DJ_S8 s8ModID)
{
	BOOL ret = FALSE;
	int i;

	for (i=0; i<256; i++)
	{
		if (cfg_iDTrkWorkMod[i]==0) 
		{		
			break;
		}

		if (cfg_iDTrkWorkMod[i] == s8ModID) 			
		{
			ret = TRUE;
			break;
		}
	}

	return ret;
}

BOOL IsVoipMod(DJ_S8 s8ModID)
{
	BOOL ret = FALSE;
	int i;
	
	for (i=0; i<256; i++)
	{
		if (cfg_iVoipWorkMod[i] == 0)
		{		
			break;
		}

		if (cfg_iVoipWorkMod[i] == s8ModID)		
		{
			ret = TRUE;
			break;
		}
	}
	
	return ret;
}

void ReDrawList(int row, CListCtrl& listCtrl)
{
	int itemCount = 0;
	int i, iDispRow;
	char TmpStr[10];

	itemCount = listCtrl.GetItemCount();
	if (itemCount <= row)
	{
		for(i = 0; i < row-itemCount; i++)
		{
			iDispRow = CalDispRow(i); 
			sprintf(TmpStr, "%3d", iDispRow);
			listCtrl.InsertItem(iDispRow, TmpStr);
		}
	}
	else
	{
		for(i = itemCount; i > itemCount - row; i--)
		{
			listCtrl.DeleteItem(i);
		}
	}
}

void ReDrawAll(void)
{
	char	TmpStr[256];
	int		i, iDispRow, j;
	int     ItemCount = 0;
	VOIP_STRUCT	*pOneVOIP = NULL;
	INTERFACE_STRUCT *pOneInterf = NULL;

	ReDrawList(g_iTotalDTrunk, pdlg->m_ListDTrunk);
	for (i = 0; i < g_iTotalDTrunk; i++)
	{
		for (j = 1; j < 9; j++)
		{
			pdlg->m_ListDTrunk.SetItemText(i, j, "");
		}
		
		iDispRow = CalDispRow(i); 
		if (iDispRow < 0)	
			continue;		

		sprintf(TmpStr, "%d,%d", MapTable_DTrunk[i].m_s8ModuleID, MapTable_DTrunk[i].m_s16ChannelID);
		pdlg->m_ListDTrunk.SetItemText(iDispRow, 1, TmpStr);

		pOneInterf = &M_OneInterface(MapTable_DTrunk[i]);
		pdlg->m_ListDTrunk.SetItemText(iDispRow, 2, GetString_DeviceSub(pOneInterf->deviceID.m_s16DeviceSub));

		// 可能改变的内容
		DrawDTrunk_LineState(pOneInterf);
		DrawDTrunk_State(pOneInterf);
		DrawDTrunk_CallInfo(pOneInterf);
	}
		
	ReDrawList(g_iTotalVOIP, pdlg->m_ListVOIP);	
	for ( i = 0; i < g_iTotalVOIP; i ++ )
	{
		for (j = 1; j < 13; j++)
		pdlg->m_ListVOIP.SetItemText(i, j, "");

		iDispRow =  i; 	
		sprintf(TmpStr, "(%d,%d)", MapTable_VOIP[i].m_s8ModuleID, MapTable_VOIP[i].m_s16ChannelID);
		pdlg->m_ListVOIP.SetItemText(iDispRow, 1, TmpStr);

		pOneVOIP = &M_OneVOIP(MapTable_VOIP[i]);
		pdlg->m_ListVOIP.SetItemText(iDispRow, 2, GetString_DeviceMain(pOneVOIP->deviceID.m_s16DeviceMain));

		// 可能改变的内容
		DrawVOIP_LineState(pOneVOIP);
		DrawVOIP_State(pOneVOIP);
		DrawVOIP_CallInfo(pOneVOIP);		
	}	

	return;
}

BOOL InitSystem(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CPBXGetWayDlg *)theApp.m_pMainWnd;

	InitializeCriticalSection(&cs); 

	ReadVOIPConfig();
	if (bLog == 1)
	{
		g_logfp = fopen("c:\\DonJinIpGateWay_1.log","w+");		
	}

	InitListVOIP();	
	InitListDTrunk();	

	InitTextBox();

//	fp = fopen("c:\\Event.log", "a+");
	// now open ACS Server
	/*调用acsOpenStream 建立与ACS服务器的连接*/
	r = XMS_acsOpenStream ( &g_acsHandle, 
						&cfg_ServerID,
						g_u8UnitID,		// App Unit ID 
						64,				// sendQSize, in K Bytes
						64,				// recvQSize, in K Bytes
						pubDebug,
						NULL);

	if (r < 0)
	{
		sprintf (MsgStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );

		MessageBox(NULL, MsgStr, "Init System", MB_OK);
		
		return FALSE;
	}
	else
	{
		sprintf(MsgStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort);		
	}

	r = XMS_acsSetESR(g_acsHandle, (EsrFunc)EvtHandler, 0, 1);
	if (r < 0)
	{
		sprintf(MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r);	
		return FALSE;
	}
	else
	{
		sprintf(MsgStr, "XMS_acsSetESR() OK!");		
	}

	InitAllDeviceRes();

	XMS_acsGetDeviceList(g_acsHandle, NULL);	

	return TRUE;
}

void ExitSystem(void)
{
	XMS_acsCloseStream(g_acsHandle, NULL);

	DeledtTab(&pDailRuleHead);

	My_DeleteAllMem();
	
	FreeAllDeviceRes();

	My_DeleteRout();

	if (g_logfp != NULL)
	{
		fclose(g_logfp);		
	}

	DeleteCriticalSection (&cs);

	pdlg = NULL;
	
	return;
}

void InitAllDeviceRes(void)
{
	// 将AllDeviceRes所有清为0，包括：lFlag, 各个数量，以及各个指针清为NULL
	memset(AllDeviceRes, 0, sizeof(AllDeviceRes));

	g_iTotalModule = 0;

	g_iTotalPcm = 0;
	g_iTotalPcmOpened = 0;

	g_iTotalDTrunk = 0;
	g_iTotalDTrunkOpened = 0;

	g_iTotalVOIP = 0;
	g_iTotalVOIPOpened = 0;

	g_iTotalVoice = 0;

	g_iTotalFax = 0;

	return;
}

void FreeOneDeviceRes(int ID)
{
	if(AllDeviceRes[ID].pVoice != NULL)
	{
		delete [] AllDeviceRes[ID].pVoice;	
		AllDeviceRes[ID].pVoice = NULL;
	}
	
	if (AllDeviceRes[ID].pPcm != NULL)
	{
		delete [] AllDeviceRes[ID].pPcm;
		AllDeviceRes[ID].pPcm = NULL;
	}

	if (AllDeviceRes[ID].pInterf != NULL)
	{
		delete [] AllDeviceRes[ID].pInterf;	
		AllDeviceRes[ID].pInterf = NULL;
	}

	if (AllDeviceRes[ID].pVOIP != NULL)
	{
		delete [] AllDeviceRes[ID].pVOIP;
		 AllDeviceRes[ID].pVOIP = NULL;
	}

	memset(&AllDeviceRes[ID], 0, sizeof(TYPE_XMS_DSP_DEVICE_RES));

	return;
}

void FreeAllDeviceRes(void)
{
	int		i;

	for (i = 0; i < MAX_DSP_MODULE_NUM; i ++)
	{
		FreeOneDeviceRes(i);
	}

	InitAllDeviceRes();

	return;
}

void OpenVoiceDevice(VOICE_STRUCT *pOneVoice)
{	
	char        TmpStr[500] = {0};
	
	pOneVoice->State= VOC_WAITOPEN;

   XMS_ctsOpenDevice(g_acsHandle, &pOneVoice->deviceID, NULL);

}

void CloseVoiceDevice(VOICE_STRUCT *pOneVoice)
{	
	
	XMS_ctsCloseDevice(g_acsHandle, &pOneVoice->deviceID, NULL);
}

void OpenBoardDevice(DeviceID_t* pBoardDevID)
{
	char        TmpStr[500] = {0};

    XMS_ctsOpenDevice(g_acsHandle, pBoardDevID, NULL);

	return;
}

void CloseBoardDevice(DeviceID_t* pBoardDevID)
{
	XMS_ctsCloseDevice ( g_acsHandle, pBoardDevID, NULL );

	return;
}

int	JudgeEmptyDsp(DJ_S8 mStartDspID)
{
	int j=1;
	int mEmptyCount = 0;
	while(j<=mStartDspID)
	{
		if(AllDeviceRes[j].lFlag == 0 ) mEmptyCount++;
		j++;
	}
	return mEmptyCount;
}

void OpenAllDevice_Dsp(DJ_S8 s8DspModID)
{
	int	 i;
	DeviceID_t pdev;	
	
	if(!IsVoipMod(s8DspModID))	return;
	// Open Board
    if (IsDigitMod(s8DspModID) || IsVoipMod(s8DspModID))
	{
		OpenBoardDevice(&AllDeviceRes[s8DspModID].deviceID);
	}	

	// pTrunk
	if(IsDigitMod(s8DspModID))
	{
	// pPcm
		for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
		{
			OpenPcmDevice(&AllDeviceRes[s8DspModID].pPcm[i]);
		}
		
	}


//		for (i = 0; i < AllDeviceRes[s8DspModID].lInterfNum; i++ )
		int mIndex=0;

		
		DeviceID_t	*mVoipDevice = NULL;
		DeviceID_t	*mVoiceDevice = NULL;
		DeviceID_t	*mTrunkDevice = NULL;
		DeviceID_t *mFaxDevice = NULL;
		
		INTERFACE_STRUCT	*pTrunkDevice = NULL;
		VOICE_STRUCT		*pVoiceDevice = NULL;
		VOIP_STRUCT			*pVoipDevice = NULL;
		FAX_STRUCT          *pFaxDevice = NULL;
				
		for (i = 0; i < 32; i++ )
		{
			if(i!=0 && i!=16)
			{
				pdev = AllDeviceRes[s8DspModID].pInterf[i].deviceID;
				


				int mEmptyCount = JudgeEmptyDsp(s8DspModID);				
				
				int mOrderIndex = CalDispRow(i);

				pubGloalIndex++;

				AllDeviceRes[s8DspModID].pVOIP[mIndex].iGlobeID = mOrderIndex;

				if (g_logfp != NULL) 
				{					
					char	mInfo[512]={0};
					sprintf(mInfo,"Trunk:Dsp:%d Ch:%d -->Voip:Dsp:%d Ch:%d GlobalID:%d\n\r",
						s8DspModID,
						i,
						s8DspModID,
						mIndex,
						mOrderIndex);
					fwrite(mInfo,strlen(mInfo),1,g_logfp);
					fflush(g_logfp);
				}
					
				pVoipDevice = &AllDeviceRes[s8DspModID].pVOIP[mIndex];
				pVoiceDevice = &AllDeviceRes[s8DspModID].pVoice[mIndex];
				pTrunkDevice = &AllDeviceRes[s8DspModID].pInterf[i];
				pFaxDevice = &AllDeviceRes[s8DspModID].pFax[mIndex];			

				mVoipDevice = &pVoipDevice->deviceID;
				mVoiceDevice = &pVoiceDevice->deviceID;
				mTrunkDevice = &pTrunkDevice->deviceID;
				mFaxDevice = &pFaxDevice->deviceID;

				pTrunkDevice->pVoiceDeviceID = *mVoiceDevice;
				pTrunkDevice->pVoipDeviceID = *mVoipDevice;

				pVoipDevice->pTrunkDeviceID = *mTrunkDevice;
				pVoipDevice->pVoiceDeviceID =* mVoiceDevice;
				pVoipDevice->pFaxDeviceID = *mFaxDevice;

				pVoiceDevice->pTrunkDevice= *mTrunkDevice;
				pVoiceDevice->pVoipDevice= *mVoipDevice;

				pFaxDevice->pVoipDevice = *mVoipDevice;


				OpenVOIPDevice(pVoipDevice);
				OpenVoiceDevice(pVoiceDevice);
				OpenFaxDevice(pFaxDevice);
				OpenInterfaceDevice(pTrunkDevice);
			//	OpenInterfaceDevice(&AllDeviceRes[s8DspModID].pInterf[i]);
				mIndex++;
			}
		}

		pVoiceDevice = &AllDeviceRes[s8DspModID].pVoice[mIndex];
		OpenVoiceDevice(pVoiceDevice);

 }

void OpenFaxDevice(FAX_STRUCT *pOneFax)
{
	RetCode_t	r;
	
//	if (pOneFax->bOpenFlag == false)		// 还没有Open
	{
		r = XMS_ctsOpenDevice(g_acsHandle, &pOneFax->deviceID, NULL);	
	}
}

void CloseFaxDevice(FAX_STRUCT *pOneFax)
{
	RetCode_t r;

	r = XMS_ctsOpenDevice(g_acsHandle, &pOneFax->deviceID, NULL);
}


void OpenPcmDevice(PCM_STRUCT *pOnePcm)
{
	RetCode_t	r;
	
	if (pOnePcm->bOpenFlag == false)		// 还没有Open
	{
		r = XMS_ctsOpenDevice(g_acsHandle, &pOnePcm->deviceID, NULL);	
	}
}

void ClosePcmDevice(PCM_STRUCT *pOnePcm)
{
	RetCode_t	r;
	
	r = XMS_ctsCloseDevice(g_acsHandle, &pOnePcm->deviceID, NULL);
}

void CloseAllDevice_Dsp(DJ_S8 s8DspModID)
{
	int	i;

	// close Board
	CloseBoardDevice(&AllDeviceRes[s8DspModID].deviceID);

	
	for (i = 0; i < AllDeviceRes[s8DspModID].lInterfNum; i++)
	{
		CloseInterfaceDevice(&AllDeviceRes[s8DspModID].pInterf[i]);
	}
	
	for (i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++)
	{
		ClosePcmDevice(&AllDeviceRes[s8DspModID].pPcm[i]);
	}
	
	for (i = 0; i < AllDeviceRes[s8DspModID].lVOIPNum; i++ )
	{
		CloseVOIPDevice ( &AllDeviceRes[s8DspModID].pVOIP[i] );
	}

	// pVoice
	for (i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++)
	{
		CloseVoiceDevice(&AllDeviceRes[s8DspModID].pVoice[i]);
	}	

	for (i = 0; i < AllDeviceRes[s8DspModID].lFaxNum; i++)
	{
		CloseFaxDevice(&AllDeviceRes[s8DspModID].pFax[i]);
	}
}

void OpenDeviceOK(DeviceID_t *pDevice)
{
	VOIP_STRUCT	 *pOneVOIP = NULL;
	INTERFACE_STRUCT *pOneTrunk = NULL;
	PCM_STRUCT   *pOnePcm = NULL;
	VOICE_STRUCT *pOneVoice = NULL;
	FAX_STRUCT   *pOneFax = NULL;
	CmdParamData_Voice_t vocGtdParam = {0};	

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		
		setFax1100Gtd_Template(pDevice);
		setFax2100Gtd_Template(pDevice);
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneInterface(*pDevice);

		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		pOneTrunk->mExchangeType = EXCHANGE_FREE;

		DeviceID_t	*pVoiceDevice=&pOneTrunk->pVoiceDeviceID;
		DeviceID_t	*pVoipDevice=&pOneTrunk->pVoipDeviceID;

		//回写关联该设备的语音通道数据
		AllDeviceRes[pOneTrunk->pVoiceDeviceID.m_s8ModuleID].pVoice[pOneTrunk->pVoiceDeviceID.m_s16ChannelID].pTrunkDevice.m_CallID = pDevice->m_CallID;
		AllDeviceRes[pOneTrunk->pVoipDeviceID.m_s8ModuleID].pVOIP[pOneTrunk->pVoipDeviceID.m_s16ChannelID].pTrunkDeviceID.m_CallID = pDevice->m_CallID;

	    if (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_CAS ||
			pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_DSS1 ||
			pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_TUP ||
			pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_ISUP )
		{
			InitDTrunkChannel(pOneTrunk);	
			
			// modify the count
			g_iTotalDTrunkOpened++;	
		}
				
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		AllDeviceRes[pDevice->m_s8ModuleID].lInterfOpened++;
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		pOneVOIP = &M_OneVOIP(*pDevice);
		pOneVOIP->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		pOneVOIP->mExchangeType = EXCHANGE_FREE;

		DeviceID_t	*pVoiceDevice=&pOneVOIP->pVoiceDeviceID;
		DeviceID_t	*pTrunkDeviceID=&pOneVOIP->pTrunkDeviceID;

		AllDeviceRes[pOneVOIP->pTrunkDeviceID.m_s8ModuleID].pInterf[pOneVOIP->pTrunkDeviceID.m_s16ChannelID].pVoipDeviceID.m_CallID = pDevice->m_CallID;
		AllDeviceRes[pOneVOIP->pVoiceDeviceID.m_s8ModuleID].pVoice[pOneVOIP->pVoiceDeviceID.m_s16ChannelID].pVoipDevice.m_CallID = pDevice->m_CallID;
		AllDeviceRes[pOneVOIP->pFaxDeviceID.m_s8ModuleID].pFax[pOneVOIP->pFaxDeviceID.m_s16ChannelID].pVoipDevice.m_CallID = pDevice->m_CallID;


		//加入对奇偶通道的呼入呼出处理
		char	mInfo[255]={0};
		int mCallType;
		if(pubVoipRule == 1 )
		{
			//奇数通道用于呼出	
			int mTempChannelID = pOneVOIP->iGlobeID;		//全局的编号

			if((mTempChannelID % 2) != 0  )
			{
				//是奇数
				mCallType = XMS_VOIP_ACCESS_OUTGOING;
				mCallType |= XMS_VOIP_ACCESS_SIP;
				XMS_ctsSetParam(g_acsHandle,&pOneVOIP->deviceID,VOIP_PARAM_ACCESS,sizeof(mCallType),(void *)&mCallType);

				if (g_logfp != NULL)
				{
					sprintf(mInfo,"SetVoipOutGoing Dsp:%d Ch:%d GlobalID:%d \n\r",
						pOneVOIP->deviceID.m_s8ModuleID,
						pOneVOIP->deviceID.m_s16ChannelID,
						pOneVOIP->iGlobeID);
					fwrite(mInfo,strlen(mInfo),1,g_logfp);
					fflush(g_logfp);
				}
			}
			else 
			{
				//是偶数
				mCallType = XMS_VOIP_ACCESS_INCOMING;
				mCallType |= XMS_VOIP_ACCESS_OUTGOING;
			}

		}
		else if (pubVoipRule == 0)
		{
			//偶数通道用于呼出
			int mTempChannelID = pOneVOIP->iGlobeID;		//全局的编号
			if((mTempChannelID % 2) == 0  )
			{
				//是偶数
				mCallType = XMS_VOIP_ACCESS_OUTGOING;
				mCallType |= XMS_VOIP_ACCESS_SIP;
				XMS_ctsSetParam(g_acsHandle,&pOneVOIP->deviceID,VOIP_PARAM_ACCESS,sizeof(mCallType),(void *)&mCallType);
				if (g_logfp != NULL)
				{
					sprintf(mInfo,"SetVoipOutGoing Dsp:%d Ch:%d GlobalID:%d \n\r",
						pOneVOIP->deviceID.m_s8ModuleID,
						pOneVOIP->deviceID.m_s16ChannelID,
						pOneVOIP->iGlobeID);
					fwrite(mInfo,strlen(mInfo),1,g_logfp);
					fflush(g_logfp);
				}
			}
			else
			{
				//是奇数
				mCallType = XMS_VOIP_ACCESS_INCOMING;
				mCallType |= XMS_VOIP_ACCESS_OUTGOING;
			}
		}
		else 
		{
			
		}

		// init this Device: VOIP
		InitVOIPChannel(pOneVOIP);

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);	

		// modify the count
		g_iTotalVOIPOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVOIPOpened++;
	}

	vocGtdParam.m_u8GtdCtrlValid = 1;
	vocGtdParam.m_VocGtdControl.m_u8ChannelEnable = 1;
	vocGtdParam.m_VocGtdControl.m_u8GTDEnable = 1;
	unsigned char gtdID[] = {"GHIJKLM"};
	memcpy(vocGtdParam.m_VocGtdControl.m_u8GTDID, gtdID, sizeof(vocGtdParam.m_VocGtdControl.m_u8GTDID));
	

	if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE)
	{
		pOneVoice = &M_OneVoice(*pDevice);
		M_OneVoice(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important

		DeviceID_t	*pVoipDeviceID=&pOneVoice->pVoipDevice;
		DeviceID_t	*pTrunkDeviceID=&pOneVoice->pTrunkDevice;

		if (pOneVoice->deviceID.m_s16ChannelID < 30)
		{
			
			AllDeviceRes[pOneVoice->pTrunkDevice.m_s8ModuleID].pInterf[pOneVoice->pTrunkDevice.m_s16ChannelID].pVoiceDeviceID.m_CallID = pDevice->m_CallID;
			AllDeviceRes[pOneVoice->pVoipDevice.m_s8ModuleID].pVOIP[pOneVoice->pVoipDevice.m_s16ChannelID].pVoiceDeviceID.m_CallID = pDevice->m_CallID;			
			//使能GTG			
			XMS_ctsSetParam(g_acsHandle, pDevice, VOC_PARAM_UNIPARAM, sizeof(CmdParamData_Voice_t), (DJ_Void *)&vocGtdParam );
		}
		else
		{
			char TempStr[50] = {0};
			sprintf(TempStr, "%d,%d", pDevice->m_s8ModuleID, pDevice->m_s16ChannelID);
			((CListBox *)pdlg->GetDlgItem(IDC_EDITRecordDev))->AddString(TempStr);
		}
		
		// 记录录音的语音资源的源语音设备
		M_OneVoice(*pDevice).State = VOC_FREE;
//		memset(&pOneVoice->UsedDevID, 0, sizeof(DeviceID_t)); 

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);		

		// modify the count
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum++;
	}

	if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_FAX)
	{
		pOneFax = &M_OneFax(*pDevice);
		pOneFax->deviceID.m_CallID = pDevice->m_CallID;

		AllDeviceRes[pOneFax->pVoipDevice.m_s8ModuleID].pVOIP[pOneFax->pVoipDevice.m_s16ChannelID].pFaxDeviceID.m_CallID = pDevice->m_CallID;

		pOneFax->State = FAX_FREE;
//		memset(pOneFax->UsedDevID, 0, sizeof(DeviceID_t));

		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);
		AllDeviceRes[pDevice->m_s8ModuleID].lFaxOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lFaxFreeNum++;
	}

	if(pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT)
	{
		pOnePcm = &M_OnePcm(*pDevice);

		pOnePcm->deviceID.m_CallID = pDevice->m_CallID;		// this is very important

		// init the Device: Pcm
		XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
		XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);

		// moidfy the count
		g_iTotalPcmOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened++;

	}
	
	return;
}

void CloseDeviceOK(DeviceID_t* pDevice)
{
	INTERFACE_STRUCT	*pOneInterf = NULL;
	VOIP_STRUCT         *pOneVOIP = NULL;

	AllDeviceRes[pDevice->m_s8ModuleID].bErrFlag = true;
//	DrawCount_ErrFlag (pDevice->m_s8ModuleID);

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = false;
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneInterf = &M_OneInterface(*pDevice);

		switch(pOneInterf->deviceID.m_s16DeviceSub)
		{
		case XMS_DEVSUB_ANALOG_USER:
		//	Changle_UserState(pOneInterf, USER_WAITOPEN);
		//	g_iTotalUserOpened--;
			break;
		case XMS_DEVSUB_ANALOG_TRUNK:
		//	Change_ATrunkState(pOneInterf, ATRK_WAITOPEN);
		//	g_iTotalATrunkOpened--;
			break;
		default:
			Change_DTrunkState(pOneInterf, TRK_WAITOPEN);
			g_iTotalDTrunkOpened--;
			break;
		}

		AllDeviceRes[pDevice->m_s8ModuleID].lInterfOpened--;

//		DrawCount_Trunk(pDevice->m_s8ModuleID );
	}

	if (pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOIP)
	{
		pOneVOIP = &M_OneVOIP(*pDevice);

		Change_VOIPState( pOneVOIP, VOIP_WAITOPEN );

		// modify the count
		g_iTotalVOIPOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lVOIPOpened--;

//		DrawCount_VOIP(pDevice->m_s8ModuleID);
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		M_OneVoice(*pDevice).State = VOC_WAITOPEN;

		// modify the count
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened--;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum--;

//		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}

	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).bOpenFlag = false;

		// moidfy the count
		g_iTotalPcmOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened--;

//		DrawCount_Pcm  ( pDevice->m_s8ModuleID );
	}

	if (pDevice->m_s16DeviceMain == XMS_DEVMAIN_FAX)
	{
//		M_OneFax(*pDevice).bOpenFlag = false;

		// moidfy the count
	//	g_iTotalFaxOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lFaxOpened--;
	}

	return;	
}

void My_DualLink(DeviceID_t *pDev1, DeviceID_t *pDev2)
{
	XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 

	return;
}

void My_DualUnlink(DeviceID_t *pDev1, DeviceID_t *pDev2)
{
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 

	return;
}




int setFax1100Gtd_Template(DeviceID_t *pDev)
{
	int ret = -1;
	CmdParamData_GtdProtoType_t g_GtdProto;	
	void *p = NULL;

    char cfg_GtdID[20] = {0};
	char cfg_Freq_Mask[20] = {0};
	char cfg_Amp_Threshold[20] = {0};
	char cfg_Envelope_Mode[20] = {0};
	char cfg_Repeat_Count[20] = {0};
	char cfg_Min_On_Time1[20] = {0};
	char cfg_Max_On_Time1[20] = {0};
	char cfg_Min_Off_Time1[20] = {0};
	char cfg_Max_Off_Time1[20] = {0};
	char cfg_Min_On_Time2[20] = {0};
	char cfg_Max_On_Time2[20] = {0};
	char cfg_Min_Off_Time2[20] = {0};
	char cfg_Max_Off_Time2[20] = {0};

	memset(&g_GtdProto, 0, sizeof(CmdParamData_GtdProtoType_t));
	//Set GTD ProtoType(GTD_Template)
	DJ_U16 u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	DJ_U16 u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);

	//set 1100  'L'
	GetPrivateProfileString("GTD", "gtdid6", "", cfg_GtdID, sizeof(cfg_GtdID), cfg_IniName);
	GetPrivateProfileString("GTD", "freqmask6", "", cfg_Freq_Mask, sizeof(cfg_Freq_Mask), cfg_IniName);
	GetPrivateProfileString("GTD", "threshold6", "", cfg_Amp_Threshold, sizeof(cfg_Amp_Threshold), cfg_IniName);
	GetPrivateProfileString("GTD", "envelop6", "", cfg_Envelope_Mode, sizeof(cfg_Envelope_Mode), cfg_IniName);
	GetPrivateProfileString("GTD", "repeatcount6", "", cfg_Repeat_Count, sizeof(cfg_Repeat_Count), cfg_IniName);
	GetPrivateProfileString("GTD", "minontime16", "", cfg_Min_On_Time1, sizeof(cfg_Min_On_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "maxontime16", "", cfg_Max_On_Time1, sizeof(cfg_Max_On_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "minofftime16", "", cfg_Min_Off_Time1, sizeof(cfg_Min_Off_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "maxofftime16", "", cfg_Max_Off_Time1, sizeof(cfg_Max_Off_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "minontime26", "", cfg_Min_On_Time2, sizeof(cfg_Min_On_Time2), cfg_IniName);
	GetPrivateProfileString("GTD", "maxontime26", "", cfg_Max_On_Time2, sizeof(cfg_Max_On_Time2), cfg_IniName);
	GetPrivateProfileString("GTD", "minofftime26", "", cfg_Min_Off_Time2, sizeof(cfg_Min_Off_Time2), cfg_IniName);
	GetPrivateProfileString("GTD", "maxofftime26", "", cfg_Max_Off_Time2, sizeof(cfg_Max_Off_Time2), cfg_IniName);

	sscanf(cfg_GtdID, "%d", &g_GtdProto.m_u16GtdID);
	sscanf(cfg_Freq_Mask, "%d", &g_GtdProto.m_u16Freq_Mask);;
	sscanf(cfg_Amp_Threshold, "%d", &g_GtdProto.m_u16Amp_Threshold);
	sscanf(cfg_Envelope_Mode, "%d", &g_GtdProto.m_u16Envelope_Mode);
	sscanf(cfg_Repeat_Count, "%d", &g_GtdProto.m_u16Repeat_Count);
	sscanf(cfg_Min_On_Time1, "%d", &g_GtdProto.m_u16Min_On_Time1);
	sscanf(cfg_Max_On_Time1, "%d", &g_GtdProto.m_u16Max_On_Time1);
	sscanf(cfg_Min_Off_Time1, "%d", &g_GtdProto.m_u16Min_Off_Time1);
	sscanf(cfg_Max_Off_Time1, "%d", &g_GtdProto.m_u16Max_Off_Time1);
	sscanf(cfg_Min_On_Time2, "%d", &g_GtdProto.m_u16Min_On_Time2);
	sscanf(cfg_Max_On_Time2, "%d", &g_GtdProto.m_u16Max_On_Time2);
	sscanf(cfg_Min_Off_Time2, "%d", &g_GtdProto.m_u16Min_Off_Time2);
	sscanf(cfg_Max_Off_Time2, "%d", &g_GtdProto.m_u16Max_Off_Time2);
	
	p = (void *)&g_GtdProto;	
	ret = XMS_ctsSetParam( g_acsHandle, pDev, u16ParamType, u16ParamSize, (DJ_Void *)p);	

	return ret;
}

int setFax2100Gtd_Template(DeviceID_t *pDev)
{
	int ret = -1;
	CmdParamData_GtdProtoType_t g_GtdProto;	
	void *p = NULL;

    char cfg_GtdID[20] = {0};
	char cfg_Freq_Mask[20] = {0};
	char cfg_Amp_Threshold[20] = {0};
	char cfg_Envelope_Mode[20] = {0};
	char cfg_Repeat_Count[20] = {0};
	char cfg_Min_On_Time1[20] = {0};
	char cfg_Max_On_Time1[20] = {0};
	char cfg_Min_Off_Time1[20] = {0};
	char cfg_Max_Off_Time1[20] = {0};
	char cfg_Min_On_Time2[20] = {0};
	char cfg_Max_On_Time2[20] = {0};
	char cfg_Min_Off_Time2[20] = {0};
	char cfg_Max_Off_Time2[20] = {0};

	memset(&g_GtdProto, 0, sizeof(CmdParamData_GtdProtoType_t));
	//Set GTD ProtoType(GTD_Template)
	DJ_U16 u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
	DJ_U16 u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);

	//set 2100  'M'
	GetPrivateProfileString("GTD", "gtdid7", "", cfg_GtdID, sizeof(cfg_GtdID), cfg_IniName);
	GetPrivateProfileString("GTD", "freqmask7", "", cfg_Freq_Mask, sizeof(cfg_Freq_Mask), cfg_IniName);
	GetPrivateProfileString("GTD", "threshold7", "", cfg_Amp_Threshold, sizeof(cfg_Amp_Threshold), cfg_IniName);
	GetPrivateProfileString("GTD", "envelop7", "", cfg_Envelope_Mode, sizeof(cfg_Envelope_Mode), cfg_IniName);
	GetPrivateProfileString("GTD", "repeatcount7", "", cfg_Repeat_Count, sizeof(cfg_Repeat_Count), cfg_IniName);
	GetPrivateProfileString("GTD", "minontime17", "", cfg_Min_On_Time1, sizeof(cfg_Min_On_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "maxontime17", "", cfg_Max_On_Time1, sizeof(cfg_Max_On_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "minofftime17", "", cfg_Min_Off_Time1, sizeof(cfg_Min_Off_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "maxofftime17", "", cfg_Max_Off_Time1, sizeof(cfg_Max_Off_Time1), cfg_IniName);
	GetPrivateProfileString("GTD", "minontime27", "", cfg_Min_On_Time2, sizeof(cfg_Min_On_Time2), cfg_IniName);
	GetPrivateProfileString("GTD", "maxontime27", "", cfg_Max_On_Time2, sizeof(cfg_Max_On_Time2), cfg_IniName);
	GetPrivateProfileString("GTD", "minofftime27", "", cfg_Min_Off_Time2, sizeof(cfg_Min_Off_Time2), cfg_IniName);
	GetPrivateProfileString("GTD", "maxofftime27", "", cfg_Max_Off_Time2, sizeof(cfg_Max_Off_Time2), cfg_IniName);

	sscanf(cfg_GtdID, "%d", &g_GtdProto.m_u16GtdID);
	sscanf(cfg_Freq_Mask, "%d", &g_GtdProto.m_u16Freq_Mask);;
	sscanf(cfg_Amp_Threshold, "%d", &g_GtdProto.m_u16Amp_Threshold);
	sscanf(cfg_Envelope_Mode, "%d", &g_GtdProto.m_u16Envelope_Mode);
	sscanf(cfg_Repeat_Count, "%d", &g_GtdProto.m_u16Repeat_Count);
	sscanf(cfg_Min_On_Time1, "%d", &g_GtdProto.m_u16Min_On_Time1);
	sscanf(cfg_Max_On_Time1, "%d", &g_GtdProto.m_u16Max_On_Time1);
	sscanf(cfg_Min_Off_Time1, "%d", &g_GtdProto.m_u16Min_Off_Time1);
	sscanf(cfg_Max_Off_Time1, "%d", &g_GtdProto.m_u16Max_Off_Time1);
	sscanf(cfg_Min_On_Time2, "%d", &g_GtdProto.m_u16Min_On_Time2);
	sscanf(cfg_Max_On_Time2, "%d", &g_GtdProto.m_u16Max_On_Time2);
	sscanf(cfg_Min_Off_Time2, "%d", &g_GtdProto.m_u16Min_Off_Time2);
	sscanf(cfg_Max_Off_Time2, "%d", &g_GtdProto.m_u16Max_Off_Time2);
	
	p = (void *)&g_GtdProto;	
	ret = XMS_ctsSetParam( g_acsHandle, pDev, u16ParamType, u16ParamSize, (DJ_Void *)p );	

	return ret;
}

void RecordFile(int iChType, int iDspID, int iChnID, int vocDspID, int vocChnID)
{
	INTERFACE_STRUCT *pOneIntf = NULL;
	VOIP_STRUCT *pOneVOIP = NULL;
	VOICE_STRUCT *pOneVoice = NULL;
	RecordProperty_t	recordProperty;
	RetCode_t			ret = -1;	
	char s8FilePath[128] = {0};
	char s8FileName[256] = {0};
	char chTemp[512] = {0};
	SYSTEMTIME st;

	EnterCriticalSection (&cs);
	pOneVoice = &AllDeviceRes[vocDspID].pVoice[vocChnID];
	ASSERT(pOneVoice != NULL);

	TRACE("Record voc %d,%d\n", pOneVoice->deviceID.m_s8ModuleID, pOneVoice->deviceID.m_s16ChannelID);

	if (pOneVoice->State == VOC_USED)
	{
		//增加提示，语音已经被使用
		return;
	}
	else
	{
		pOneVoice->State = VOC_USED;
	}
	
	if (iChType == 0) //选择trunk对应得VOIP得语音
	{
		pOneIntf = &AllDeviceRes[iDspID].pInterf[iChnID];
		pOneVOIP = &M_OneVOIP(pOneIntf->pVoipDeviceID);

		ASSERT(pOneIntf != NULL);
		ASSERT(pOneVOIP != NULL);
		TRACE("Record trunk %d,%d\n", pOneIntf->deviceID.m_s8ModuleID, pOneIntf->deviceID.m_s16ChannelID);

	/*	if (pOneIntf->iLineState != DCS_CONNECT)
		{	
			//增加提示，7号线路未接通
			return;
		}*/
		
	}
	else if(iChType == 1) //选择VOIP
	{
		pOneVOIP = &AllDeviceRes[iDspID].pVOIP[iChnID];
		ASSERT(pOneVOIP != NULL);

		TRACE("Record voip %d,%d\n", pOneIntf->deviceID.m_s8ModuleID, pOneIntf->deviceID.m_s16ChannelID);
	/*	if (pOneVOIP->iLineState != DCS_CONNECT)
		{
			//增加提示，voip 线路未接通
			return;
		}*/
	}
	else
	{
		// 
		return;
	}

	pOneVOIP->pVoiceDevRecord = pOneVoice->deviceID;

	ret = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->pVoiceDeviceID, &pOneVOIP->pVoiceDevRecord, NULL);

	memset(&recordProperty,0,sizeof(RecordProperty_t));		
	recordProperty.m_u32MaxSize = 0;
	recordProperty.m_s8IsMixEnable = 1;
	recordProperty.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
	recordProperty.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
	recordProperty.m_MixerControl.m_u16SRC_ChID1 = pOneVOIP->pVoiceDeviceID.m_s16ChannelID;
	recordProperty.m_MixerControl.m_u16SRC_ChID2 = pOneVOIP->pVoiceDevRecord.m_s16ChannelID;
	recordProperty.m_s8IsAppend = 0;

	GetPrivateProfileString("RECORD", "FILEPATH", "", s8FilePath, sizeof(s8FilePath), cfg_IniName);
	if (strlen(s8FilePath) > 0)
	{
		GetLocalTime(&st);
		sprintf(s8FileName, "%sRec%d%d%d%d-%d-%d.pcm",
			    s8FilePath,
			    st.wDay,
			    st.wHour,
			    st.wMinute,
			    st.wSecond,
				pOneVOIP->deviceID.m_s8ModuleID,
				pOneVOIP->deviceID.m_s16ChannelID);		
	}
	else
	{
		GetLocalTime(&st);
		sprintf(s8FileName, "Rec%d%d%d%d-%d-%d.pcm",			    
			    st.wDay,
			    st.wHour,
			    st.wMinute,
			    st.wSecond,
				pOneVOIP->deviceID.m_s8ModuleID,
				pOneVOIP->deviceID.m_s16ChannelID);	
	}
	strcpy(recordProperty.m_s8FileName, s8FileName);

	ret = XMS_ctsRecord (g_acsHandle, &pOneVOIP->pVoiceDevRecord, &recordProperty, NULL );
	//记录日志，说明调用了录音

	LeaveCriticalSection(&cs);

	return ;
}

void getCurrTime(char* mTime)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(mTime,"%04d-%02d-%02d %02d:%02d:%02d",
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
}

void EnableGtg(DeviceID_t *pVocDev)
{
	RetCode_t ret = -1;
	ASSERT(pVocDev != NULL);

	CmdParamData_Voice_t    VocMixParam = {0};
	
	VocMixParam.m_u8GtdCtrlValid = 1;
	VocMixParam.m_VocGtdControl.m_u8DTMFEnable = 1;
	VocMixParam.m_VocGtdControl.m_u8FSKEnable = 1;
	VocMixParam.m_VocGtdControl.m_u8ChannelEnable = 1;
	VocMixParam.m_VocGtdControl.m_u8GTDEnable = 1;
	unsigned char gtdID[] = {"GHIJKLM"};
	memcpy(VocMixParam.m_VocGtdControl.m_u8GTDID, gtdID, sizeof(VocMixParam.m_VocGtdControl.m_u8GTDID));	
	
	ret = XMS_ctsSetParam(g_acsHandle, pVocDev,VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);			
	
	if (g_logfp != NULL)
	{
		char	mTemp[512]={0};
		char	mCurrTime[50]={0};
		getCurrTime(mCurrTime);
		
		sprintf(mTemp,"%s enable voice GTD, Voice:(%d, %d) CallID(%d,%d)<------>Voip:(%d,%d) RetCode=%d.\n\r",
			mCurrTime,
			pVocDev->m_s8ModuleID,
			pVocDev->m_s16ChannelID,
			pVocDev->m_CallID.m_s32FlowType,
			pVocDev->m_CallID.m_s32FlowChannel,
			pVocDev->m_s8ModuleID,
			pVocDev->m_s16ChannelID,
			ret);

		fwrite(mTemp,strlen(mTemp),1,g_logfp);
		fflush(g_logfp);
	}	
}

void DisableGtg(DeviceID_t *pVocDev)
{
	RetCode_t ret = -1;
	ASSERT(pVocDev != NULL);

	CmdParamData_Voice_t    VocMixParam = {0} ;	
	VocMixParam.m_u16PlayFixGain = 0;
	VocMixParam.m_u8PlayGainMode = 0;
	VocMixParam.m_u8GtdCtrlValid = 1;
	memset(&VocMixParam.m_VocGtdControl, 0, sizeof(VocGtdControlParam_t));
	
	ret = XMS_ctsSetParam(g_acsHandle, pVocDev,VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);			
	
	if (g_logfp != NULL)
	{
		char	mTemp[512]={0};
		char	mCurrTime[50]={0};
		getCurrTime(mCurrTime);
		
		sprintf(mTemp,"%s disable voice GTD, Voice:(%d, %d) CallID(%d,%d)<------>Voip:(%d,%d) RetCode=%d.\n\r",
			mCurrTime,
			pVocDev->m_s8ModuleID,
			pVocDev->m_s16ChannelID,
			pVocDev->m_CallID.m_s32FlowType,
			pVocDev->m_CallID.m_s32FlowChannel,
			pVocDev->m_s8ModuleID,
			pVocDev->m_s16ChannelID,
			ret);

		fwrite(mTemp,strlen(mTemp),1,g_logfp);
		fflush(g_logfp);
	}
}

//创建号码规则表
int IsLast(PtrRule rule)
{
	return rule->Next == NULL;
}

void Insert(char *chElement, PtrRule rule, Position p)
{
	Position TempCell = NULL;

	ASSERT(p != NULL);
	ASSERT(rule != NULL);
	ASSERT(chElement != NULL);

	if (strlen(chElement) == 0)
		return;

	TempCell = new ListRule; 
    if (TempCell == NULL)
	{
		printf("Out of space!!!");
		return;
	}

	memset(TempCell, 0, sizeof(ListRule));
	strncpy(TempCell->CodeRule, chElement, sizeof(TempCell->CodeRule));
	TempCell->Next = p->Next;
	p->Next = TempCell;
}

void DeleteList( PtrRule rule)
{
	Position p = NULL;
	Position Tmp = NULL;
	
	ASSERT(rule != NULL);

	p = rule->Next;
	rule->Next = NULL;
	while(p != NULL)
	{
		Tmp = p->Next;
		delete p;
		p = Tmp;
	}
}

int IsEmpty(PtrRule rule)
{
	return rule->Next == NULL;
}

void DeledtTab(PtrRule *pDailRule)
{
	DeleteList(*pDailRule);
	delete *pDailRule;
	*pDailRule = NULL;	
}


//号码规则表end

