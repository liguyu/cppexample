#include "stdafx.h"
#include "XMSTest_FaxDemo.h"
#include "XMSTest_FaxDemoDlg.h"


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMS_FaxDemo_Sub.H"
#include "XMS_FaxDemo_Event.H"
#include "XMS_FaxDemo_String.H"

extern int				cfg_iPartWorkModuleID[256];
extern CRITICAL_SECTION CriticalSection;

// ----------------------------------------------------------------------------------------------------
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

int						g_iTotalModule = 0;
DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

int						g_iTotalPcm = 0;
int						g_iTotalPcmOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM_IN_THIS_DEMO];
/*
int						g_iTotalTrunk = 0;
int						g_iTotalTrunkOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];
*/
int						g_iTotalUser = 0;
int						g_iTotalUserOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_User[MAX_TRUNK_NUM_IN_THIS_DEMO];

int						g_iTotalVoice = 0;
int						g_iTotalVoiceOpened = 0;
int						g_iTotalVoiceFree = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_TRUNK_NUM_IN_THIS_DEMO];

int                     g_iTotalFax = 0;
int                     g_iTotalFaxOpened = 0;
int                     g_iTotalFaxFree = 0;
TYPE_CHANNEL_MAP_TABLE  MapTable_Fax[MAX_TRUNK_NUM_IN_THIS_DEMO];
// add by chenwy 2009-02-18
int                     g_iTotalVoip	    = 0;
int                     g_iTotalVoipOpened  = 0;
int                     g_iTotalVoipFree    = 0;
TYPE_CHANNEL_MAP_TABLE  MapTable_Voip[MAX_TRUNK_NUM_IN_THIS_DEMO];
// add end
// ----------------------------------------------------------------------------------------------------
void	DispEventInfo ( Acs_Evt_t *pAcsEvt )
{
	char	TmpStr[256];
	char	TmpS[128];
	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_UniFailure_Data * pAcsUniFailure = NULL;

	sprintf ( TmpStr, "EVT(%4d) : ", pAcsEvt->m_s32EvtSize );
	strcat ( TmpStr, GetString_EventType ( pAcsEvt->m_s32EventType ) );

	switch ( pAcsEvt->m_s32EventType )
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
		AddMsg ( TmpStr );
//		WriteMsgLog(TmpStr);
		break;

	default:
		break;

	}
}

// --------------------------------------------------------------------------
void	InitAllDeviceRes (void)
{
	// 将AllDeviceRes所有清为0，包括：lFlag, 各个数量，以及各个指针清为NULL
	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );

	g_iTotalModule = 0;

	g_iTotalPcm = 0;
	g_iTotalPcmOpened = 0;

//	g_iTotalTrunk = 0;
//	g_iTotalTrunkOpened = 0;

	g_iTotalUser = 0;
	g_iTotalUserOpened = 0;

	g_iTotalVoice = 0;
	g_iTotalVoiceOpened = 0;
	g_iTotalVoiceFree = 0;

	g_iTotalFax = 0;
	g_iTotalFaxOpened = 0;
	g_iTotalFaxFree = 0;
	// add by chenwy 2009-02-18
	g_iTotalVoip = 0;
	g_iTotalVoipOpened = 0;
	g_iTotalVoipFree = 0;
	// add end
}

void	FreeOneDeviceRes ( int ID )
{
	if ( AllDeviceRes[ID].pVoice != NULL )
		delete [] AllDeviceRes[ID].pVoice;

	if (AllDeviceRes[ID].pFax != NULL)
		delete [] AllDeviceRes[ID].pFax;

	if ( AllDeviceRes[ID].pPcm != NULL )
		delete [] AllDeviceRes[ID].pPcm;

/*	if ( AllDeviceRes[ID].pTrunk != NULL )
		delete [] AllDeviceRes[ID].pTrunk;
*/
	// add by chenwy 2009-02-18
	if ( AllDeviceRes[ID].pVoip != NULL )
	{
	    delete [] AllDeviceRes[ID].pVoip;
	    AllDeviceRes[ID].pVoip  = NULL;
	}
	// add end

	memset ( &AllDeviceRes[ID], 0, sizeof (TYPE_XMS_DSP_DEVICE_RES_DEMO) );
}

void	FreeAllDeviceRes (void)
{
	int		i;

	for ( i = 0; i < MAX_DSP_MODULE_NUMBER_OF_XMS; i ++ )
	{
		FreeOneDeviceRes ( i );
	}

	InitAllDeviceRes ();
}

// --------------------------------------------------------------------------
void	RefreshMapTable ( void )
{
	int		i, j;
	int		TrkCount, VocCount, PcmCount, ModuleCount, UserCount, FaxCount, VoipCount;
//	int		iModSeqID;

	// 将AllDeviceRes中的每个接口通道、语音通道、PCM通道所对应的（顺序号）记录下来
	ModuleCount = TrkCount = VocCount = PcmCount = UserCount = FaxCount = VoipCount = 0;
	for ( i = 0; i < MAX_DSP_MODULE_NUMBER_OF_XMS; i ++ )
	{
		 if ( AllDeviceRes[i].lFlag == 1 )
		 {
			// DSP 模块
			AllDeviceRes[i].iSeqID = ModuleCount;
			MapTable_Module[ModuleCount] = i;
			ModuleCount ++;

			 // 语音通道
			 for ( j = 0; j < AllDeviceRes[i].lVocNum; j ++ )
			 {
				AllDeviceRes[i].pVoice[j].iSeqID = VocCount;
				MapTable_Voice[VocCount].m_s8ModuleID = AllDeviceRes[i].pVoice[j].deviceID.m_s8ModuleID;
				MapTable_Voice[VocCount].m_s16ChannelID = AllDeviceRes[i].pVoice[j].deviceID.m_s16ChannelID;
				VocCount ++;
			 }

			 // Fax通道
			 for (j=0; j<AllDeviceRes[i].lFaxNum; j++)
			 {
				 AllDeviceRes[i].pFax[j].iSeqID = FaxCount;
				 MapTable_Fax[FaxCount].m_s8ModuleID = AllDeviceRes[i].pFax[j].deviceID.m_s8ModuleID;
				 MapTable_Fax[FaxCount].m_s16ChannelID = AllDeviceRes[i].pFax[j].deviceID.m_s16ChannelID;
				 FaxCount++;
			 }

			 // PCM通道
			 for ( j = 0; j < AllDeviceRes[i].lPcmNum; j ++ )
			 {
				AllDeviceRes[i].pPcm[j].iSeqID = PcmCount;
				MapTable_Pcm[PcmCount].m_s8ModuleID = AllDeviceRes[i].pPcm[j].deviceID.m_s8ModuleID;
				MapTable_Pcm[PcmCount].m_s16ChannelID = AllDeviceRes[i].pPcm[j].deviceID.m_s16ChannelID;
				PcmCount ++;
			 }

			 // 接口通道
/*			 iModSeqID = 0;
			 for ( j = 0; j < AllDeviceRes[i].lTrunkNum; j ++ )
			 {
				AllDeviceRes[i].pTrunk[j].iSeqID = TrkCount;
				MapTable_Trunk[TrkCount].m_s8ModuleID = AllDeviceRes[i].pTrunk[j].deviceID.m_s8ModuleID;
				MapTable_Trunk[TrkCount].m_s16ChannelID = AllDeviceRes[i].pTrunk[j].deviceID.m_s16ChannelID;
				TrkCount ++;
				if ( AllDeviceRes[i].pTrunk[j].deviceID.m_s16DeviceSub != XMS_DEVSUB_UNUSABLE )
				{
					// 可用的通道
					AllDeviceRes[i].pTrunk[j].iModSeqID = iModSeqID;
					iModSeqID ++;
				}	
			 }*/
			 // add by chenwy 2009-02-18
			 // VOIP通道
			 for (j=0; j<AllDeviceRes[i].lVoipNum; j++)
			 {
				 AllDeviceRes[i].pVoip[j].iSeqID = VoipCount;
				 MapTable_Voip[VoipCount].m_s8ModuleID = AllDeviceRes[i].pVoip[j].deviceID.m_s8ModuleID;
				 MapTable_Voip[VoipCount].m_s16ChannelID = AllDeviceRes[i].pVoip[j].deviceID.m_s16ChannelID;
				 VoipCount++;
			 }
			 // add end
		 }
	}

	g_iTotalModule = ModuleCount;
//	g_iTotalTrunk = TrkCount;
	g_iTotalVoice = VocCount;
	g_iTotalPcm = PcmCount;
	g_iTotalFax = FaxCount;
	g_iTotalVoip	= VoipCount;	// add by chenwy 2009-02-18
}
/*
void	AddDeviceRes_Trunk ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( AllDeviceRes[s8DspModID].lTrunkNum == 0 )		// 新增加的资源
	{
		// 
		AllDeviceRes[s8DspModID].pTrunk = new TRUNK_STRUCT[s32Num];
		if( !AllDeviceRes[s8DspModID].pTrunk )
		{
			AllDeviceRes[s8DspModID].lTrunkNum = 0;
			AllDeviceRes[s8DspModID].lTrunkOpened = 0;

			// alloc fail, maybe disp this error in your log
			sprintf ( TmpStr, "new TRUNK_STRUCT[%d] fail in AddDeviceRes_Trunk()" );
			AddMsg ( TmpStr );						
		}
		else
		{
			AllDeviceRes[s8DspModID].lTrunkNum = s32Num;
			AllDeviceRes[s8DspModID].lTrunkOpened = 0;
			memset ( AllDeviceRes[s8DspModID].pTrunk, 0, sizeof(TRUNK_STRUCT)*s32Num );
			
			DeviceID_t	 *pDev;

			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pTrunk[i].deviceID = pDev[i];
			}
		}
	}
}
*/
void	AddDeviceRes_Voice ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( AllDeviceRes[s8DspModID].lVocNum == 0 )		// 新增加的资源
	{
		AllDeviceRes[s8DspModID].pVoice = new VOICE_STRUCT[s32Num];
		if( !AllDeviceRes[s8DspModID].pVoice )
		{
			AllDeviceRes[s8DspModID].lVocNum = 0;
			AllDeviceRes[s8DspModID].lVocOpened = 0;
			AllDeviceRes[s8DspModID].lVocFreeNum = 0;

			// alloc fail, maybe disp this error in your log
			sprintf ( TmpStr, "new VOICE_STRUCT[%d] fail in AddDeviceRes_Voice()" );
			AddMsg ( TmpStr );			
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
			}
		}
	}
}

void    AddDeviceRes_Fax(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32   s32Num;
	int      i;
	char     TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if (AllDeviceRes[s8DspModID].lFaxNum == 0)     // 新增加的资源
	{
		AllDeviceRes[s8DspModID].pFax = new FAX_STRUCT[s32Num];
		if (AllDeviceRes[s8DspModID].pFax == NULL)
		{
			AllDeviceRes[s8DspModID].lFaxNum = 0;
			AllDeviceRes[s8DspModID].lFaxOpened = 0;
			AllDeviceRes[s8DspModID].lFaxFreeNum = 0;

			// alloc fail, maybe disp error in your log
			sprintf(TmpStr, "new FAX_STRUCT[%d] fail in AddDeviceRes_Fax()");
			AddMsg(TmpStr);			
		}
		else
		{
			memset(AllDeviceRes[s8DspModID].pFax, 0, sizeof(FAX_STRUCT) * s32Num);
			AllDeviceRes[s8DspModID].lFaxNum = s32Num;
			AllDeviceRes[s8DspModID].lFaxOpened = 0;
			AllDeviceRes[s8DspModID].lFaxFreeNum = 0;

			DeviceID_t   *pDev;
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			for (i=0; i<s32Num; i++)
			{
				AllDeviceRes[s8DspModID].pFax[i].deviceID = pDev[i];
			}
		}
	}
}

// add by chenwy 2009-02-18
void    AddDeviceRes_Voip(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32   s32Num;
	int      i;
	char     TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if (AllDeviceRes[s8DspModID].lVoipNum == 0)     // 新增加的资源
	{
		AllDeviceRes[s8DspModID].pVoip = new VOIP_STRUCT[s32Num];
		if (AllDeviceRes[s8DspModID].pVoip == NULL)
		{
			AllDeviceRes[s8DspModID].lVoipNum = 0;
			AllDeviceRes[s8DspModID].lVoipOpened = 0;
			AllDeviceRes[s8DspModID].lVoipFreeNum = 0;

			// alloc fail, maybe disp error in your log
			sprintf(TmpStr, "new VOIP_STRUCT[%d] fail in AddDeviceRes_Voip()");
			AddMsg(TmpStr);			
		}
		else
		{
			memset(AllDeviceRes[s8DspModID].pVoip, 0, sizeof(VOIP_STRUCT) * s32Num);
			AllDeviceRes[s8DspModID].lVoipNum = s32Num;
			AllDeviceRes[s8DspModID].lVoipOpened = 0;
			AllDeviceRes[s8DspModID].lVoipFreeNum = 0;

			DeviceID_t   *pDev;
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			for (i=0; i<s32Num; i++)
			{
				AllDeviceRes[s8DspModID].pVoip[i].deviceID = pDev[i];
			}
		}
	}
}
// add end

void	AddDeviceRes_Pcm ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( AllDeviceRes[s8DspModID].lPcmNum == 0 )		// 新增加的资源
	{
		// 
		AllDeviceRes[s8DspModID].pPcm = new PCM_STRUCT[s32Num];
		if( !AllDeviceRes[s8DspModID].pPcm )
		{
			AllDeviceRes[s8DspModID].lPcmNum = 0;
			AllDeviceRes[s8DspModID].lPcmOpened = 0;

			// alloc fail, maybe disp this error in your log
			sprintf ( TmpStr, "new PCM_STRUCT[%d] fail in AddDeviceRes_Pcm()" );
			AddMsg ( TmpStr );
			
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
			}
		}
	}
}

void	AddDeviceRes_Board ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	if ( AllDeviceRes[s8DspModID].lFlag == 0 )		// 新增加的资源
	{
		DeviceID_t	 *pDev;
		pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

		AllDeviceRes[s8DspModID].deviceID = pDev[0];
		
		CreateFileDirector(s8DspModID);  //创建录音文件路径
	}
}

void	AddDeviceRes ( Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Type, s32Num;
	DJ_S8	s8DspModID;

	s32Type = pAcsDevList->m_s32DeviceMain;
	s32Num = pAcsDevList->m_s32DeviceNum;

	s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;
	if ( (s8DspModID < 0) || (s8DspModID >= MAX_DSP_MODULE_NUMBER_OF_XMS) )
		return;				// invalid ModuleID

	switch ( s32Type )
	{
	case XMS_DEVMAIN_VOICE:	
		AddDeviceRes_Voice ( s8DspModID, pAcsDevList );
		break;

	case XMS_DEVMAIN_FAX:
		AddDeviceRes_Fax(s8DspModID, pAcsDevList);
		break;

	case XMS_DEVMAIN_DIGITAL_PORT:
		AddDeviceRes_Pcm ( s8DspModID, pAcsDevList );
		break;
	
	case XMS_DEVMAIN_INTERFACE_CH:
//		AddDeviceRes_Trunk ( s8DspModID, pAcsDevList );
		break;

	case XMS_DEVMAIN_DSS1_LINK:			break;
	case XMS_DEVMAIN_SS7_LINK:			break;

	case XMS_DEVMAIN_BOARD:
		AddDeviceRes_Board ( s8DspModID, pAcsDevList );
		break;

	case XMS_DEVMAIN_CTBUS_TS:			break;
	case XMS_DEVMAIN_VOIP:			
	    AddDeviceRes_Voip(s8DspModID, pAcsDevList);
	    break;
	case XMS_DEVMAIN_CONFERENCE:		break;

	case XMS_DEVMAIN_VIDEO:				break;
		break;
	}
}

BOOL IsSysMod(DJ_S8 s8ModuleID)
{
	int   i;
	BOOL  ret = FALSE;

	for (i=0; i<256; i++)
	{
		if (cfg_iPartWorkModuleID[i]==0)
		{
			ret = FALSE;
			break;
		}

		if (cfg_iPartWorkModuleID[i] == s8ModuleID)
		{
			ret = TRUE;
			break;
		}
	}

	return ret;
}

DJ_Void EvtHandler(DJ_U32 esrParam)
{
	Acs_Evt_t *			    pAcsEvt = NULL;
	Acs_Dev_List_Head_t * pAcsDevList = NULL;

	pAcsEvt = (Acs_Evt_t *) esrParam;
	DispEventInfo ( pAcsEvt );

	EnterCriticalSection(&CriticalSection);

	switch ( pAcsEvt->m_s32EventType )
	{
		case XMS_EVT_QUERY_DEVICE:
			if ( IsSysMod(pAcsEvt->m_DeviceID.m_s8ModuleID) )
			{
				pAcsDevList = ( Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);

				//收到设备变化的事件，增加/调整 该设备资源
				AddDeviceRes ( pAcsDevList );
			}		
			break; 

		case XMS_EVT_QUERY_ONE_DSP_END:
			if (  IsSysMod(pAcsEvt->m_DeviceID.m_s8ModuleID) )
			{
				AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 1;		// 该DSP可以用了

				OpenAllDevice_Dsp ( pAcsEvt->m_DeviceID.m_s8ModuleID );

				RefreshMapTable ( );

				ReDrawAll ();
			}	
			break;

		case XMS_EVT_QUERY_DEVICE_END:	// 获取设备列表结束
			break;

		case XMS_EVT_OPEN_DEVICE:
			OpenDeviceOK ( &pAcsEvt->m_DeviceID );
			break;

		case XMS_EVT_DEVICESTATE:
			HandleDevState ( pAcsEvt );
			break;

		case XMS_EVT_UNIFAILURE:
			// must handle this event in your real System
			break;

		default:
/*			if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH  )
			{
				TrunkWork ( &M_OneTrunk(pAcsEvt->m_DeviceID), pAcsEvt );
			}
			else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
			{
				DeviceID_t	*pDevID;

				pDevID = &M_OneVoice(pAcsEvt->m_DeviceID).UsedDevID;

				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
				{					
					TrunkWork ( &M_OneTrunk(*pDevID), pAcsEvt );				
				}
			}
			else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_FAX )
			{
				DeviceID_t	*pDevID;

				pDevID = &M_OneFax(pAcsEvt->m_DeviceID).UsedDevID;

				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
				{
					TrunkWork ( &M_OneTrunk(*pDevID), pAcsEvt );
				}
			}*/
			if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP  )
			{
				VoipWork ( &M_OneVoip(pAcsEvt->m_DeviceID), pAcsEvt );
			}
			else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
			{
				DeviceID_t	*pDevID;

				pDevID = &M_OneVoice(pAcsEvt->m_DeviceID).UsedDevID;

				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_VOIP )
				{					
					VoipWork ( &M_OneVoip(*pDevID), pAcsEvt );				
				}
			}
			else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_FAX )
			{
				DeviceID_t	*pDevID;

				pDevID = &M_OneFax(pAcsEvt->m_DeviceID).UsedDevID;

				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_VOIP )
				{
					VoipWork ( &M_OneVoip(*pDevID), pAcsEvt );
				}
			}
			break;
	}

	LeaveCriticalSection(&CriticalSection);
}