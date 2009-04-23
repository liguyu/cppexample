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

#include "XMS_FaxDial_Sub.H"
#include "XMS_FaxDial_Event.H"
#include "XMS_FaxDial_String.H"

extern int				cfg_iPartWorkModuleID[256];
extern CRITICAL_SECTION CriticalSection; 

// ----------------------------------------------------------------------------------------------------
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

int						g_iTotalModule = 0;
DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

int						g_iTotalPcm = 0;
int						g_iTotalPcmOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM_IN_THIS_DEMO];

int						g_iTotalTrunk = 0;
int						g_iTotalTrunkOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];

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
//		WriteMsgLog(TmpStr);
		break;

	default:
		break;

	}

//	AddMsg ( TmpStr );
}

// --------------------------------------------------------------------------
void	InitAllDeviceRes (void)
{
	// 将AllDeviceRes所有清为0，包括：lFlag, 各个数量，以及各个指针清为NULL
	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );

	g_iTotalModule = 0;

	g_iTotalPcm = 0;
	g_iTotalPcmOpened = 0;

	g_iTotalTrunk = 0;
	g_iTotalTrunkOpened = 0;

	g_iTotalUser = 0;
	g_iTotalUserOpened = 0;

	g_iTotalVoice = 0;
	g_iTotalVoiceOpened = 0;
	g_iTotalVoiceFree = 0;
}

void	FreeOneDeviceRes ( int ID )
{
	if ( AllDeviceRes[ID].pVoice != NULL )
	{
		delete [] AllDeviceRes[ID].pVoice;
		AllDeviceRes[ID].pVoice = NULL;
	}

	if ( AllDeviceRes[ID].pPcm != NULL )
	{
		delete [] AllDeviceRes[ID].pPcm;
		AllDeviceRes[ID].pPcm = NULL;
	}

	if ( AllDeviceRes[ID].pTrunk != NULL )
	{
		delete [] AllDeviceRes[ID].pTrunk;
		AllDeviceRes[ID].pTrunk = NULL;
	}

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
	int		TrkCount, VocCount, PcmCount, ModuleCount, UserCount, FaxCount;
	int		iModSeqID;

	// 将AllDeviceRes中的每个接口通道、语音通道、PCM通道所对应的（顺序号）记录下来
	ModuleCount = TrkCount = VocCount = PcmCount = UserCount = FaxCount = 0;
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

			 // PCM通道
			 for ( j = 0; j < AllDeviceRes[i].lPcmNum; j ++ )
			 {
				AllDeviceRes[i].pPcm[j].iSeqID = PcmCount;
				MapTable_Pcm[PcmCount].m_s8ModuleID = AllDeviceRes[i].pPcm[j].deviceID.m_s8ModuleID;
				MapTable_Pcm[PcmCount].m_s16ChannelID = AllDeviceRes[i].pPcm[j].deviceID.m_s16ChannelID;
				PcmCount ++;
			 }

			 // 接口通道
			 iModSeqID = 0;
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
			 }

			 // 传真通道
			 for (j = 0; j < AllDeviceRes[i].lFaxNum; j++)
			 {
				 AllDeviceRes[i].pFax[j].iSeqID = FaxCount;
				 MapTable_Fax[FaxCount].m_s8ModuleID = AllDeviceRes[i].pFax[j].deviceID.m_s8ModuleID;
				 MapTable_Fax[FaxCount].m_s16ChannelID = AllDeviceRes[i].pFax[j].deviceID.m_s16ChannelID;
				 FaxCount++;				
			 }
		 }
	}

	g_iTotalModule = ModuleCount;
	g_iTotalTrunk = TrkCount;
	g_iTotalUser = UserCount;
	g_iTotalFax = FaxCount;
	g_iTotalVoice = VocCount;
	g_iTotalPcm = PcmCount;
}

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
			memset ( AllDeviceRes[s8DspModID].pTrunk, 0, sizeof(TRUNK_STRUCT)*s32Num );
			AllDeviceRes[s8DspModID].lTrunkNum = s32Num;
			AllDeviceRes[s8DspModID].lTrunkOpened = 0;
			
			DeviceID_t	 *pDev;

			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pTrunk[i].deviceID = pDev[i];
			}
		}
	}
}

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

void   AddDeviceRes_Fax(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32   s32Num;
	int      i;
	char     TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if (AllDeviceRes[s8DspModID].lFaxNum == 0)
	{
		AllDeviceRes[s8DspModID].pFax = new FAX_STRUCT[s32Num];
		if (AllDeviceRes[s8DspModID].pFax == NULL)
		{
			AllDeviceRes[s8DspModID].lFaxNum = 0;
			AllDeviceRes[s8DspModID].lFaxOpened = 0;

			sprintf(TmpStr, "new FAX_STRUCT[%d] fail in AddDeviceRes_Fax()");
			AddMsg(TmpStr);
		}
		else
		{
			memset(AllDeviceRes[s8DspModID].pFax, 0, sizeof(FAX_STRUCT)*s32Num);
			AllDeviceRes[s8DspModID].lFaxNum = s32Num;
			AllDeviceRes[s8DspModID].lFaxOpened = 0;
			AllDeviceRes[s8DspModID].lFaxFreeNum = 0;

			DeviceID_t *pDev;
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));	
			for (i = 0; i < s32Num; i++)
			{
				AllDeviceRes[s8DspModID].pFax[i].deviceID = pDev[i];
			}
		}
	}
}

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
		
		CreateFileDirector(s8DspModID);       // 创建新目录,保存录音文件
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
		AddDeviceRes_Trunk ( s8DspModID, pAcsDevList );
		break;

	case XMS_DEVMAIN_DSS1_LINK:			break;
	case XMS_DEVMAIN_SS7_LINK:			break;

	case XMS_DEVMAIN_BOARD:
		AddDeviceRes_Board ( s8DspModID, pAcsDevList );
		break;

	case XMS_DEVMAIN_CTBUS_TS:			break;
	case XMS_DEVMAIN_VOIP:				break;
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
			if ( IsSysMod(pAcsEvt->m_DeviceID.m_s8ModuleID) )
			{
				AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 1;		// 该DSP可以用了

				OpenAllDevice_Dsp ( pAcsEvt->m_DeviceID.m_s8ModuleID );

				RefreshMapTable ( );

				ReDrawAll ();
			}
			break;

		case XMS_EVT_QUERY_DEVICE_END:	// 获取设备列表结束
			{

			}
			
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
			if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH  )
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
			else if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_FAX)
			{
				DeviceID_t	*pDevID;

				pDevID = &M_OneFax(pAcsEvt->m_DeviceID).UsedDevID;

				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
				{
					TrunkWork ( &M_OneTrunk(*pDevID), pAcsEvt );
				}
			}
			break;
	}
	LeaveCriticalSection(&CriticalSection);
}