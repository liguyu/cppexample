#include "stdafx.h"
#include "globe.h"
#include "XMS_String.h"
#include "VOIP.H"
#include "globe.h"
//#include "Analog.h"
#include "DTrunk.h"
#include "XMS_Run.h"

extern FILE *g_logfp;
extern int  g_bSendFax;
extern int  g_bRecvFax;
extern unsigned int  cfg_FaxMode;

extern ACSHandle_t g_acsHandle;

extern  CRITICAL_SECTION cs;


extern TYPE_XMS_DSP_DEVICE_RES	AllDeviceRes[MAX_DSP_MODULE_NUM];

extern int						g_iTotalModule;
extern DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUM];

extern int						g_iTotalPcm;
extern int						g_iTotalPcmOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM];

extern int						g_iTotalDTrunk;
extern int						g_iTotalDTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_DTrunk[MAX_INTERFACE_NUM];

extern int						g_iTotalVOIP;
extern int						g_iTotalVOIPOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_VOIP[MAX_VOIP_NUM];

extern int						g_iTotalVoice;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_VOICE_NUM];

extern int						g_iTotalFax;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Fax[MAX_VOICE_NUM];


void DispEventInfo(Acs_Evt_t *pAcsEvt)
{
	char	TmpStr[256] = {0};
	char	TmpS[128] = {0};
	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_UniFailure_Data * pAcsUniFailure = NULL;

	sprintf(TmpStr, "EVT(%4d): ", pAcsEvt->m_s32EvtSize);
	strcat(TmpStr, GetString_EventType(pAcsEvt->m_s32EventType));

	switch (pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_OPEN_STREAM:
		break;

	case XMS_EVT_QUERY_DEVICE:
		pAcsDevList = (Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);
		sprintf ( TmpS, " (%s,%2d,%3d)", 
			GetString_DeviceMain(pAcsDevList->m_s32DeviceMain),
			pAcsDevList->m_s32ModuleID,
			pAcsDevList->m_s32DeviceNum );
		strcat ( TmpStr, TmpS );
		break;

	case XMS_EVT_OPEN_DEVICE:
		break;

	case XMS_EVT_UNIFAILURE:
		pAcsUniFailure = (Acs_UniFailure_Data *) FetchEventData(pAcsEvt);
		sprintf ( TmpS, ": %s(0x%X) dev=(%s, %d, %d),  %d ?=? %d+%d", 
			GetString_ErrorCode(pAcsUniFailure->m_s32AcsEvtErrCode), pAcsUniFailure->m_s32AcsEvtErrCode,
			GetString_DeviceMain(pAcsEvt->m_DeviceID.m_s16DeviceMain),  pAcsEvt->m_DeviceID.m_s8ModuleID, pAcsEvt->m_DeviceID.m_s16ChannelID,
			pAcsEvt->m_s32EvtSize, sizeof(Acs_Evt_t), sizeof(Acs_UniFailure_Data) 
			);
		strcat ( TmpStr, TmpS );		
		break;

	default:
		break;
	}
	
	return;
}

void RefreshMapTable(void)
{
	int		i, j;
	int		UserCount = 0;
	int     ATrkCount = 0;
	int     DTrkCount = 0;
	int     VocCount = 0;
	int     PcmCount = 0;
	int     VOIPCount = 0;
	int     ModuleCount = 0;
	int     FaxCount = 0;
	int		iModSeqID;

	// 将AllDeviceRes中的每个接口通道、语音通道、PCM通道所对应的（顺序号）记录下来
	
	for (i = 0; i < MAX_DSP_MODULE_NUM; i++)
	{
		 if (AllDeviceRes[i].lFlag == 1)
		 {
			// DSP 模块
			 if(IsDigitMod(i) )
			 {			 
				 AllDeviceRes[i].iSeqID = ModuleCount;
				 MapTable_Module[ModuleCount] = i;
				 ModuleCount ++;
			 }

			 // 语音通道

			if (IsVoipMod(i))
			{
				for ( j = 0; j < AllDeviceRes[i].lVOIPNum; j ++ )
				{
					AllDeviceRes[i].pVoice[j].iSeqID = VocCount;
					MapTable_Voice[VocCount].m_s8ModuleID = AllDeviceRes[i].pVoice[j].deviceID.m_s8ModuleID;
					MapTable_Voice[VocCount].m_s16ChannelID = AllDeviceRes[i].pVoice[j].deviceID.m_s16ChannelID;
					VocCount++;
				}
			
				
				//	VOIP通道			 
				for ( j = 0; j < AllDeviceRes[i].lVOIPNum; j ++ )
				{
					AllDeviceRes[i].pVOIP[j].iSeqID = VOIPCount;
					MapTable_VOIP[VOIPCount].m_s8ModuleID = AllDeviceRes[i].pVOIP[j].deviceID.m_s8ModuleID;
					MapTable_VOIP[VOIPCount].m_s16ChannelID = AllDeviceRes[i].pVOIP[j].deviceID.m_s16ChannelID;
					VOIPCount++;			
				}

				for (j = 0; j < AllDeviceRes[i].lFaxNum; j++)
				{
					AllDeviceRes[i].pFax[j].iSeqID = FaxCount;
					MapTable_Fax[FaxCount].m_s8ModuleID = AllDeviceRes[i].pFax[j].deviceID.m_s8ModuleID;
					MapTable_Fax[FaxCount].m_s16ChannelID = AllDeviceRes[i].pFax[j].deviceID.m_s16ChannelID;
					FaxCount++;
				}
			}
			 
			 // PCM通道
			 if(IsDigitMod(i))
			 {				 
				 for ( j = 0; j < AllDeviceRes[i].lPcmNum; j ++ )
				 {
					 AllDeviceRes[i].pPcm[j].iSeqID = PcmCount;
					 MapTable_Pcm[PcmCount].m_s8ModuleID = AllDeviceRes[i].pPcm[j].deviceID.m_s8ModuleID;
					 MapTable_Pcm[PcmCount].m_s16ChannelID = AllDeviceRes[i].pPcm[j].deviceID.m_s16ChannelID;
					 PcmCount++;
				 }
			 }

			 // 接口通道
			 iModSeqID = 0;
			 for (j = 0; j < AllDeviceRes[i].lInterfNum; j++ )
			 {				 
				 switch(AllDeviceRes[i].pInterf[j].deviceID.m_s16DeviceSub)
				 {					 
				  	
					  case XMS_DEVSUB_E1_CAS:
					  case XMS_DEVSUB_E1_DSS1:
					  case XMS_DEVSUB_E1_SS7_TUP:
					  case XMS_DEVSUB_E1_SS7_ISUP:  				
						 if (IsDigitMod(i))
						 {					  
							 AllDeviceRes[i].pInterf[j].iSeqID = DTrkCount;
							 MapTable_DTrunk[DTrkCount].m_s8ModuleID = AllDeviceRes[i].pInterf[j].deviceID.m_s8ModuleID;
							 MapTable_DTrunk[DTrkCount].m_s16ChannelID = AllDeviceRes[i].pInterf[j].deviceID.m_s16ChannelID;
							 DTrkCount++;
						 }
						 break;		
					  default:
						  break;
				 }

				if (AllDeviceRes[i].pInterf[j].deviceID.m_s16DeviceSub != XMS_DEVSUB_UNUSABLE)
				{
					// 可用的通道
					AllDeviceRes[i].pInterf[j].iModSeqID = iModSeqID;
					iModSeqID++;
				}			
			 }		 
		 }
	}

	g_iTotalModule = ModuleCount;
//	g_iTotalDTrunk = DTrkCount;
	g_iTotalVOIP = VOIPCount;
	g_iTotalVoice = VocCount;
	g_iTotalPcm = PcmCount;
	g_iTotalFax = FaxCount;

	return;
}

void AddDeviceRes_Interface(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256] = {0};

	s32Num = pAcsDevList->m_s32DeviceNum;

	if(IsDigitMod(s8DspModID))
	{	
		if ((AllDeviceRes[s8DspModID].lInterfNum == 0) && (s32Num > 0))		// 新增加的资源
		{
			// 
			delete [] AllDeviceRes[s8DspModID].pInterf;
			AllDeviceRes[s8DspModID].pInterf = NULL;

			AllDeviceRes[s8DspModID].pInterf = new INTERFACE_STRUCT[s32Num];
			if( !AllDeviceRes[s8DspModID].pInterf )
			{
				AllDeviceRes[s8DspModID].lInterfNum = 0;
				AllDeviceRes[s8DspModID].lInterfOpened = 0;
				
				// alloc fail, maybe disp this error in your log
				sprintf(TmpStr, "new INTERFACE_STRUCT[%d] fail in AddDeviceRes_Trunk()");			
			}
			else
			{
				memset(AllDeviceRes[s8DspModID].pInterf, 0, sizeof(INTERFACE_STRUCT)*s32Num );
				AllDeviceRes[s8DspModID].lInterfNum = s32Num;
				AllDeviceRes[s8DspModID].lInterfOpened = 0;
				
				DeviceID_t	 *pDev;
				
				pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				
				for (i = 0; i < s32Num; i++)
				{
					AllDeviceRes[s8DspModID].pInterf[i].deviceID = pDev[i];
					switch(AllDeviceRes[s8DspModID].pInterf[i].deviceID.m_s16DeviceSub)
					{
					case XMS_DEVSUB_E1_SS7_TUP:
					case XMS_DEVSUB_E1_SS7_ISUP:
					case XMS_DEVSUB_E1_DSS1:
					case XMS_DEVSUB_E1_CAS:
						if (IsDigitMod(s8DspModID))
						{
							g_iTotalDTrunk++;
							AllDeviceRes[s8DspModID].pInterf[i].DTrkState = TRK_WAITOPEN;
						}
						break;
					default:
						break;
					}					
				}
			}
		}
	}
}

void AddDeviceRes_VOIP(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256] = {0};

	s32Num = pAcsDevList->m_s32DeviceNum;

	if (IsVoipMod(s8DspModID))
	{	
		if (AllDeviceRes[s8DspModID].lVOIPNum == 0)		// 新增加的资源
		{
			// 
			delete [] AllDeviceRes[s8DspModID].pVOIP;
			AllDeviceRes[s8DspModID].pVOIP = NULL;

			AllDeviceRes[s8DspModID].pVOIP = new VOIP_STRUCT[s32Num];
			if( !AllDeviceRes[s8DspModID].pVOIP )
			{
				AllDeviceRes[s8DspModID].lVOIPNum = 0;
				AllDeviceRes[s8DspModID].lVOIPOpened = 0;
				
				// alloc fail, maybe disp this error in your log
				sprintf(TmpStr, "new VOIP_STRUCT[%d] fail in AddDeviceRes_VOIP()");			
			}
			else
			{
				AllDeviceRes[s8DspModID].lVOIPNum = s32Num;
				AllDeviceRes[s8DspModID].lVOIPOpened = 0;
				memset ( AllDeviceRes[s8DspModID].pVOIP, 0, sizeof(VOIP_STRUCT)*s32Num );
				
				DeviceID_t	 *pDev;
				
				pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				
				for (i = 0; i < s32Num; i ++)
				{
					AllDeviceRes[s8DspModID].pVOIP[i].deviceID = pDev[i];
					AllDeviceRes[s8DspModID].pVOIP[i].State = VOIP_WAITOPEN;
				}
			}
		}
	}
}

void AddDeviceRes_Voice(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256] = {0};

	s32Num = pAcsDevList->m_s32DeviceNum;

	if(IsVoipMod(s8DspModID))
	{		
		if ( AllDeviceRes[s8DspModID].lVocNum == 0 )		// 新增加的资源
		{
			delete [] AllDeviceRes[s8DspModID].pVoice;
			AllDeviceRes[s8DspModID].pVoice = NULL;

			AllDeviceRes[s8DspModID].pVoice = new VOICE_STRUCT[s32Num];
			if( !AllDeviceRes[s8DspModID].pVoice )
			{
				AllDeviceRes[s8DspModID].lVocNum = 0;
				AllDeviceRes[s8DspModID].lVocOpened = 0;
				AllDeviceRes[s8DspModID].lVocFreeNum = 0;
				
				// alloc fail, maybe disp this error in your log
				sprintf ( TmpStr, "new VOICE_STRUCT[%d] fail in AddDeviceRes_Voice()" );		
			}
			else
			{
				AllDeviceRes[s8DspModID].lVocNum = s32Num;
				AllDeviceRes[s8DspModID].lVocOpened = 0;
				AllDeviceRes[s8DspModID].lVocFreeNum = 0;
				
				DeviceID_t	 *pDev;
				pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				
				for ( i = 0; i < s32Num; i ++ )
				{
					AllDeviceRes[s8DspModID].pVoice[i].deviceID = pDev[i];
					AllDeviceRes[s8DspModID].pVoice[i].State = VOC_WAITOPEN;
				}
			}
		}
	}
}

void AddDeviceRes_Fax(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256] = {0};

	s32Num = pAcsDevList->m_s32DeviceNum;

	if(IsVoipMod(s8DspModID))
	{		
		if ( AllDeviceRes[s8DspModID].lFaxNum == 0 )		// 新增加的资源
		{
			delete [] AllDeviceRes[s8DspModID].pFax;
			AllDeviceRes[s8DspModID].pFax = NULL;

			AllDeviceRes[s8DspModID].pFax = new FAX_STRUCT[s32Num];
			if( !AllDeviceRes[s8DspModID].pFax )
			{
				AllDeviceRes[s8DspModID].lFaxNum = 0;
				AllDeviceRes[s8DspModID].lFaxOpened = 0;
				AllDeviceRes[s8DspModID].lFaxFreeNum = 0;
				
				// alloc fail, maybe disp this error in your log
				sprintf ( TmpStr, "new VOICE_STRUCT[%d] fail in AddDeviceRes_Voice()" );		
			}
			else
			{
				AllDeviceRes[s8DspModID].lFaxNum = s32Num;
				AllDeviceRes[s8DspModID].lFaxOpened = 0;
				AllDeviceRes[s8DspModID].lFaxFreeNum = 0;
				
				DeviceID_t	 *pDev;
				pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				
				for ( i = 0; i < s32Num; i ++ )
				{
					AllDeviceRes[s8DspModID].pFax[i].deviceID = pDev[i];
					AllDeviceRes[s8DspModID].pFax[i].State = FAX_WAITOPEN;
				}
			}
		}
	}
}

void AddDeviceRes_Pcm(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if(IsDigitMod(s8DspModID))
	{	
		if ( (AllDeviceRes[s8DspModID].lPcmNum == 0) && (s32Num > 0) )		// 新增加的资源
		{
			// 
			delete [] AllDeviceRes[s8DspModID].pPcm;
			AllDeviceRes[s8DspModID].pPcm = NULL;

			AllDeviceRes[s8DspModID].pPcm = new PCM_STRUCT[s32Num];
			if( !AllDeviceRes[s8DspModID].pPcm )
			{
				AllDeviceRes[s8DspModID].lPcmNum = 0;
				AllDeviceRes[s8DspModID].lPcmOpened = 0;
				
				// alloc fail, maybe disp this error in your log
				sprintf ( TmpStr, "new PCM_STRUCT[%d] fail in AddDeviceRes_Pcm()" );
				
			}
			else
			{
				memset ( AllDeviceRes[s8DspModID].pPcm, 0, sizeof(PCM_STRUCT)*s32Num );
				AllDeviceRes[s8DspModID].lPcmNum = s32Num;
				AllDeviceRes[s8DspModID].lPcmOpened = 0;
				
				DeviceID_t	 *pDev;
				pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				for ( i = 0; i < s32Num; i ++ )
				{
					AllDeviceRes[s8DspModID].pPcm[i].deviceID = pDev[i];
					AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag = false;
				}
			}
		}
	}
}

void AddDeviceRes_Board(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;

	s32Num = pAcsDevList->m_s32DeviceNum;

	if(IsDigitMod(s8DspModID) || IsVoipMod(s8DspModID))
	{	
		if ( AllDeviceRes[s8DspModID].lFlag == 0 )		// 新增加的资源
		{
			DeviceID_t	 *pDev;
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			
			AllDeviceRes[s8DspModID].deviceID = pDev[0];		
		}
	}
}

void AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Type, s32Num;
	DJ_S8	s8DspModID;

	s32Type = pAcsDevList->m_s32DeviceMain;
	s32Num = pAcsDevList->m_s32DeviceNum;

	s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;
	if ( (s8DspModID < 0) || (s8DspModID >= MAX_DSP_MODULE_NUM) )
		return;				// invalid ModuleID

	switch ( s32Type )
	{
	case XMS_DEVMAIN_VOICE:	
		AddDeviceRes_Voice(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_FAX:	
		AddDeviceRes_Fax(s8DspModID, pAcsDevList);
		break;
	
	case XMS_DEVMAIN_DIGITAL_PORT:
		AddDeviceRes_Pcm(s8DspModID, pAcsDevList);
		break;
	
	case XMS_DEVMAIN_INTERFACE_CH:
		AddDeviceRes_Interface(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_DSS1_LINK:		
		break;
	case XMS_DEVMAIN_SS7_LINK:		
		break;

	case XMS_DEVMAIN_BOARD:
		AddDeviceRes_Board(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_CTBUS_TS:		
		break;
	case XMS_DEVMAIN_VOIP:	
		AddDeviceRes_VOIP(s8DspModID, pAcsDevList);
		break;
	case XMS_DEVMAIN_CONFERENCE:		
		break;

	case XMS_DEVMAIN_VIDEO:			
		break;		
	}
}

void HandleDevState(Acs_Evt_t *pAcsEvt)
{
	VOIP_STRUCT	 *pOneVOIP = NULL;
	INTERFACE_STRUCT *pOneInterf = NULL;
	PCM_STRUCT   *pOnePcm = NULL;
	Acs_GeneralProc_Data *pGeneralData = NULL;

	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneInterf = &M_OneInterface(pAcsEvt->m_DeviceID);

		if (g_logfp != NULL)
		{
			char	mTemp[255]={0};
			char	mCurrTime[50]={0};
			char	pOldState[30]={0};
			char	pNewState[30]={0};

			GetString_LineState(pOldState,pOneInterf->iLineState);
			GetString_LineState(pNewState,pGeneralData->m_s32DeviceState);
			
			getCurrTime(mCurrTime);
			
			sprintf(mTemp,"%s, SS7 LineState Change:Dsp:%d Ch:%d %s-->%s \n\r",
				mCurrTime,
				pOneInterf->deviceID.m_s8ModuleID,
				pOneInterf->deviceID.m_s16ChannelID,
				pOldState,
				pNewState
				);
			fwrite(mTemp,strlen(mTemp),1,g_logfp);
			fflush(g_logfp);
		}
		pOneInterf->iLineState = pGeneralData->m_s32DeviceState;
		
		//线路状态不正常的时候的处理
		if(pOneInterf->iLineState == DCS_UNAVAILABLE || 
		   pOneInterf->iLineState == DCS_LOCAL_BLOCK || 	
		   pOneInterf->iLineState == DCS_REMOTE_BLOCK || 	
		   pOneInterf->iLineState == DCS_BLOCK || 	
		   pOneInterf->iLineState == DCS_UNAVAILABLE )
		   //|| pOneInterf->iLineState == DCS_WAIT_FREE)
		{
			_ASSERT(pOneInterf != NULL);
			Change_DTrunkState(pOneInterf, TRK_FREE);
			My_InitIntfDtmfBuf(pOneInterf);
			memset(&pOneInterf->LinkDevID, 0, sizeof(DeviceID_t));// 0表示还没有分配LinkDevID
			memset(&pOneInterf->VocDevID, 0, sizeof(DeviceID_t)); // 在中继中此语音为空
			DrawDTrunk_LinkDev(pOneInterf);
		}
		DrawInterf_LineState(pOneInterf);		
	}

	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(pAcsEvt->m_DeviceID);

		pOnePcm->u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		pOnePcm->s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);

//		DrawPcm_TypeAndAlarm ( pOnePcm );
	}

	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		pOneVOIP = &M_OneVOIP(pAcsEvt->m_DeviceID);

		if(pGeneralData->m_s32DeviceState == 66051)
		{
			return;
		}

		pOneVOIP->iLineState = pGeneralData->m_s32DeviceState;
		if(pGeneralData->m_s32DeviceState == DCS_FREE)
		{
			Change_VOIPState(pOneVOIP,VOIP_FREE);
		}
		DrawVOIP_LineState( pOneVOIP );			
	}
	return;
}

void  StartT38Fax(DeviceID_t *pDev)
{
//	VOIP_STRUCT *pOneVOIP = NULL;
	_ASSERT(pDev != NULL);
	RetCode_t ret = -1;

//	pOneVOIP = &M_OneVOIP(*pDev);
	
	//发送传真时，disable GTG
//	DisableGtg(&pOneVOIP->pVoiceDeviceID);

	if (pDev->m_s8ModuleID > 0)
	{
		ret = XMS_ctsSendIOData(g_acsHandle, pDev, 
			XMS_IO_TYPE_VOIP_T38START, 0, NULL);
		
		if(ret<=0)
		{
			if (g_logfp != NULL)
			{
				char	mTemp[255]={0};
				char	mCurrTime[50]={0};
				
				getCurrTime(mCurrTime);
				
				sprintf(mTemp,"%s, VOIP start T38 FAX Faile!!!!! m_s16DeviceMain=%d,m_s16DeviceSub=%d,m_s8ModuleID=%d,"\
					"m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,m_s32FlowChannel=%d\n\r",
					mCurrTime,
					pDev->m_s16DeviceMain,
					pDev->m_s16DeviceSub,
					pDev->m_s8ModuleID,
					pDev->m_s8MachineID,
					pDev->m_s16ChannelID,
					pDev->m_CallID.m_s32FlowType,
					pDev->m_CallID.m_s32FlowChannel);

				fwrite(mTemp,strlen(mTemp),1,g_logfp);
				fflush(g_logfp);
			}

		}
		else
		{
			if (g_logfp != NULL)
			{
				char	mTemp[255]={0};
				char	mCurrTime[50]={0};
				
				getCurrTime(mCurrTime);
				
				sprintf(mTemp,"%s, VOIP start T38 FAX! m_s16DeviceMain=%d,m_s16DeviceSub=%d,m_s8ModuleID=%d,"\
					"m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,m_s32FlowChannel=%d\n\r",
					mCurrTime,
					pDev->m_s16DeviceMain,
					pDev->m_s16DeviceSub,
					pDev->m_s8ModuleID,
					pDev->m_s8MachineID,
					pDev->m_s16ChannelID,
					pDev->m_CallID.m_s32FlowType,
					pDev->m_CallID.m_s32FlowChannel);

				fwrite(mTemp,strlen(mTemp),1,g_logfp);
				fflush(g_logfp);
			}
		}
	}
}

DJ_Void EvtHandler(DJ_U32 esrParam)
{
	Acs_Evt_t *			    pAcsEvt = NULL;
	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_GeneralProc_Data * pGenData = NULL;
	char tmpbuf[256] = {0};
	char tmp[128] = {0};
	VOICE_STRUCT *pOneVoice = NULL;
	VOIP_STRUCT  *pOneVOIP = NULL;
	FAX_STRUCT *pOneFax = NULL;
	char	TmpStr[256] = {0};
	char  TmpEventStr[512] = {0};
	Acs_FaxProc_Data *pFaxData  = NULL;


	pAcsEvt = (Acs_Evt_t *) esrParam;
	


/*	if (g_logfp != NULL)
	{
		char	mEventLog[512]={0};
		char	mCurrTime[50]={0};
		
		getCurrTime(mCurrTime);
		
		sprintf(mEventLog,"%s, EventType:%s !,m_s16DeviceMain=%d,m_s16DeviceSub=%d," \
			"m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d," \
			"m_s32FlowChannel=%d\n\r",
			mCurrTime,
			GetString_EventType(pAcsEvt->m_s32EventType),
			pAcsEvt->m_DeviceID.m_s16DeviceMain,
			pAcsEvt->m_DeviceID.m_s16DeviceSub,
			pAcsEvt->m_DeviceID.m_s8ModuleID,
			pAcsEvt->m_DeviceID.m_s8MachineID,
			pAcsEvt->m_DeviceID.m_s16ChannelID,
			pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType,
			pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel);
		fwrite(mEventLog,strlen(mEventLog),1,g_logfp);
		fflush(g_logfp);
	}
*/

	EnterCriticalSection (&cs);

	switch ( pAcsEvt->m_s32EventType )
	{
		case XMS_EVT_QUERY_DEVICE:			
				pAcsDevList = ( Acs_Dev_List_Head_t *)FetchEventData(pAcsEvt);				

				//收到设备变化的事件，增加/调整 该设备资源
				AddDeviceRes(pAcsDevList);
					
			break; 

		case XMS_EVT_QUERY_ONE_DSP_END:
		case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:	
			{				
				if ( pAcsEvt->m_s32EventType == XMS_EVT_QUERY_ONE_DSP_END )
				{
					AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 1;		// 该DSP可以用了
					OpenAllDevice_Dsp(pAcsEvt->m_DeviceID.m_s8ModuleID);
				}
				else
				{				
					AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 0;		// 该DSP已经Remove完毕
				}				

				RefreshMapTable();
			    ReDrawAll();
			}		
			
			break;

		case XMS_EVT_QUERY_DEVICE_END:	// 获取设备列表结束
			break;

		case XMS_EVT_OPEN_DEVICE:
			OpenDeviceOK(&pAcsEvt->m_DeviceID);
			break;

		case XMS_EVT_CLOSE_DEVICE:	
			// 在删除设备之前，先发给应用程序CloseDevice事件；调用函数XMS_ctsCloseDevicey也会产生本事件		
			CloseDeviceOK(&pAcsEvt->m_DeviceID);
			break;

		case XMS_EVT_UNIFAILURE:		
			// must handle this event in your real System
			break;	
	    case XMS_EVT_RECVIODATA:
			{
				Acs_IO_Data *pIOEvt = (Acs_IO_Data *)FetchEventData(pAcsEvt);
				if((pIOEvt->m_u16IoDataLen) > 0)
				{
					long mDspID = pAcsEvt->m_DeviceID.m_s8ModuleID;
					long mChannelID = pAcsEvt->m_DeviceID.m_s16ChannelID;

					if( pIOEvt->m_u16IoType ==XMS_IO_TYPE_DTMF && 
						pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
					{						
						if (g_logfp != NULL)
						{
							char	mTemp[255]={0};
							char	mCurrTime[50]={0};
							
							getCurrTime(mCurrTime);
							
							sprintf(mTemp,"%s, PSTN Recv DTMF!,m_s16DeviceMain=%d,m_s16DeviceSub=%d," \
								"m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d," \
								"m_s32FlowChannel=%d\n\r",
								mCurrTime,
								pAcsEvt->m_DeviceID.m_s16DeviceMain,
								pAcsEvt->m_DeviceID.m_s16DeviceSub,
								pAcsEvt->m_DeviceID.m_s8ModuleID,
								pAcsEvt->m_DeviceID.m_s8MachineID,
								pAcsEvt->m_DeviceID.m_s16ChannelID,
								pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType,
								pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel);
							fwrite(mTemp,strlen(mTemp),1,g_logfp);
							fflush(g_logfp);
						}

						InterfaceWork(&AllDeviceRes[mDspID].pInterf[AllDeviceRes[mDspID].pVoice[mChannelID].pTrunkDevice.m_s16ChannelID], pAcsEvt);
					}

					if((pIOEvt->m_u16IoType == XMS_IO_TYPE_VOIP_H245UII ||
						pIOEvt->m_u16IoType == XMS_IO_TYPE_VOIP_SIPINFO || 
						pIOEvt->m_u16IoType == XMS_IO_TYPE_VOIP_RTPDTMF ) &&
						pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
					{
						if (g_logfp != NULL)
						{
							char	mTemp[255]={0};
							char	mCurrTime[50]={0};

							getCurrTime(mCurrTime);
							
							sprintf(mTemp,"%s, VOIP RECV DTMF!,m_s16DeviceMain=%d,m_s16DeviceSub=%d,"\
								"m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,"\
								"m_s32FlowChannel=%d\n\r",
								mCurrTime,
								pAcsEvt->m_DeviceID.m_s16DeviceMain,
								pAcsEvt->m_DeviceID.m_s16DeviceSub,
								pAcsEvt->m_DeviceID.m_s8ModuleID,
								pAcsEvt->m_DeviceID.m_s8MachineID,
								pAcsEvt->m_DeviceID.m_s16ChannelID,
								pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType,
								pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel);
							
							fwrite(mTemp,strlen(mTemp),1,g_logfp);
							fflush(g_logfp);
						}

						VOIPWork(&AllDeviceRes[mDspID].pVOIP[mChannelID], pAcsEvt);
					}

					if ((pIOEvt->m_u16IoType == XMS_IO_TYPE_VOIP_T38START) &&
						(pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP))
					{						
						VOIPWork(&AllDeviceRes[mDspID].pVOIP[mChannelID], pAcsEvt);						
					}

					//fax tone
					if ((pIOEvt->m_u16IoType == XMS_IO_TYPE_GTG) && 
						(pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE))
					{						
						
						if (cfg_FaxMode == T38)
						{	
							DeviceID_t VoipDev = AllDeviceRes[mDspID].pVOIP[mChannelID].deviceID;
							char *pGtd = NULL;
							pGtd = (char *)FetchIOData(pAcsEvt);						
							
							if (g_logfp != NULL)
							{
								char	mTemp[255]={0};
								char	mCurrTime[50]={0};
								
								getCurrTime(mCurrTime);
								
								sprintf(mTemp,"%s, Fax Tone = %c! m_s16DeviceMain=%d,m_s16DeviceSub=%d,"\
									"m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,"\
									"m_s32FlowChannel=%d\n\r",
									mCurrTime,
									pGtd[0],
									pAcsEvt->m_DeviceID.m_s16DeviceMain,
									pAcsEvt->m_DeviceID.m_s16DeviceSub,
									pAcsEvt->m_DeviceID.m_s8ModuleID,
									pAcsEvt->m_DeviceID.m_s8MachineID,
									pAcsEvt->m_DeviceID.m_s16ChannelID,
									pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType,
									pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel);
							}
							
							if (pGtd[0] == 'L') //检测到1100 send fax
							{
								AllDeviceRes[mDspID].pVOIP[mChannelID].bSendFax = 1;								
							}
							else if (pGtd[0] == 'M') //检测到2100 recv fax
							{
								AllDeviceRes[mDspID].pVOIP[mChannelID].bRecvFax = 1;								
								
							}
							else
							{
								break;
							}						
							
							if (AllDeviceRes[mDspID].pVOIP[mChannelID].bStartFax == false )
							{	
								StartT38Fax(&VoipDev);	
								AllDeviceRes[mDspID].pVOIP[mChannelID].bStartFax = true;
							}						
							else
							{
								if (g_logfp != NULL)
								{
									char	mTemp[255]={0};
									char	mCurrTime[50]={0};
									
									getCurrTime(mCurrTime);
									
									sprintf(mTemp,"%s, VOIP RECV GTG but bStartFax is True !,m_s16DeviceMain=%d,m_s16DeviceSub=%d,"\
										"m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,"\
										"m_s32FlowChannel=%d\n\r",
										mCurrTime,
										pAcsEvt->m_DeviceID.m_s16DeviceMain,
										pAcsEvt->m_DeviceID.m_s16DeviceSub,
										pAcsEvt->m_DeviceID.m_s8ModuleID,
										pAcsEvt->m_DeviceID.m_s8MachineID,
										pAcsEvt->m_DeviceID.m_s16ChannelID,
										pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType,
										pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel);
									
									fwrite(mTemp,strlen(mTemp),1,g_logfp);
									fflush(g_logfp);
								}
								
							}
						}
					}
				}
				break;
			}	   
		case XMS_EVT_SENDFAX:
			{
				pOneFax = &M_OneFax(pAcsEvt->m_DeviceID);
				pOneVOIP = &M_OneVOIP(pOneFax->pVoipDevice);
				
				pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);	
				
				if (pOneVOIP->deviceID.m_s8ModuleID > 0)
				{
					VOIPWork(pOneVOIP, pAcsEvt);
				}
				
				if (g_logfp != NULL)
				{
					char	mTemp[512]={0};
					char	mCurrTime[50]={0};
					if ( pFaxData->m_s32AcsEvtState == 1)
					{
						sprintf(mTemp, "(Fax:%d,%d)(VOIP:%d,%d):send Successed\n\r" ,
							pAcsEvt->m_DeviceID.m_s8ModuleID, 
							pAcsEvt->m_DeviceID.m_s16ChannelID,
							pOneFax->pVoipDevice.m_s8ModuleID,
							pOneFax->pVoipDevice.m_s16ChannelID,
							pFaxData->m_s32AcsEvtState, pFaxData->m_s32AcsEvtErrCode, pFaxData->m_u8ErrorStep );
					}
					else
					{							
						sprintf(mTemp, "(Fax:%d,%d)(VOIP:%d,%d):send failed	St=%d, Err=%d, ErrStep=%d\n\r", 
							pAcsEvt->m_DeviceID.m_s8ModuleID, 
							pAcsEvt->m_DeviceID.m_s16ChannelID,
							pOneFax->pVoipDevice.m_s8ModuleID,
							pOneFax->pVoipDevice.m_s16ChannelID,
							pFaxData->m_s32AcsEvtState, 
							pFaxData->m_s32AcsEvtErrCode, 
							pFaxData->m_u8ErrorStep );				
						
					}					
					
					getCurrTime(mCurrTime);					
					fwrite(mTemp,strlen(mTemp),1,g_logfp);
					fflush(g_logfp);
				}				
				
			}
			break;
		case XMS_EVT_RECVFAX:
			{				
				pOneFax = &M_OneFax(pAcsEvt->m_DeviceID);
				pOneVOIP = &M_OneVOIP(pOneFax->pVoipDevice);
				
				pFaxData = (Acs_FaxProc_Data *)FetchEventData(pAcsEvt);	
				
				if (pOneVOIP->deviceID.m_s8ModuleID > 0)
				{
					VOIPWork(pOneVOIP, pAcsEvt);
				}
				
				if (g_logfp != NULL)
				{
					char	mTemp[512]={0};
					char	mCurrTime[50]={0};
					
					getCurrTime(mCurrTime);	
					sprintf(mTemp, "%s, (Fax:%d,%d)(VOIP:%d,%d):recieve Finish St=%d, Err=%d, ErrStep=%d SendState=%d TotalPages=%d \n\r", 
						mCurrTime,
						pAcsEvt->m_DeviceID.m_s8ModuleID, 
						pAcsEvt->m_DeviceID.m_s16ChannelID,
						pOneFax->pVoipDevice.m_s8ModuleID,
						pOneFax->pVoipDevice.m_s16ChannelID,
						pFaxData->m_s32AcsEvtState, 
						pFaxData->m_s32AcsEvtErrCode, 
						pFaxData->m_u8ErrorStep,
						pFaxData->m_u8T30SendState,
						pFaxData->m_u16TotalPages);		
					
									
					fwrite(mTemp,strlen(mTemp),1,g_logfp);
					fflush(g_logfp);
				}
			}
			break;
		case XMS_EVT_SENDIODATA:
			{
				long mDspID = pAcsEvt->m_DeviceID.m_s8ModuleID;
				long mChannelID = pAcsEvt->m_DeviceID.m_s16ChannelID;

				if(pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
				{
					if (g_logfp != NULL)
					{
						char	mTemp[255]={0};
						char	mCurrTime[50]={0};
						
						getCurrTime(mCurrTime);
						
						sprintf(mTemp,"%s, VOIP Recevice SNEDIODATA EVENT!,m_s16DeviceMain=%d,m_s16DeviceSub=%d," \
							"m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d," \
							"m_s32FlowChannel=%d\n\r",
							mCurrTime,
							pAcsEvt->m_DeviceID.m_s16DeviceMain,
							pAcsEvt->m_DeviceID.m_s16DeviceSub,
							pAcsEvt->m_DeviceID.m_s8ModuleID,
							pAcsEvt->m_DeviceID.m_s8MachineID,
							pAcsEvt->m_DeviceID.m_s16ChannelID,
							pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType,
							pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel);
						fwrite(mTemp,strlen(mTemp),1,g_logfp);
						fflush(g_logfp);
					}
				}
			}
			break;
		case XMS_EVT_DEVICESTATE:
			HandleDevState(pAcsEvt);
			break;
		default:
			if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
			{
				VOIPWork(&M_OneVOIP(pAcsEvt->m_DeviceID), pAcsEvt);
			}
			if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH)
			{
				InterfaceWork(&M_OneInterface(pAcsEvt->m_DeviceID), pAcsEvt);
			}

			break;
	}
	
	LeaveCriticalSection(&cs);


	return;
}

