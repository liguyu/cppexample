#include "stdafx.h"
#include "XMS_VOIPTestDemo.h"
#include "XMS_VOIPTestDemoDlg.h"


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMS_VOIPDemo_Sub.H"
#include "XMS_VOIPDemo_Event.H"
#include "XMS_VOIPDemo_String.H"

extern int				cfg_iPartWorkModuleID[256];

// ----------------------------------------------------------------------------------------------------
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

int						g_iTotalModule = 0;
DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

int						g_iTotalVOIP = 0;
int						g_iTotalVOIPOpened = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_VOIP[MAX_VOIP_NUM_IN_THIS_DEMO];

int						g_iTotalVoice = 0;
int						g_iTotalVoiceOpened = 0;
int						g_iTotalVoiceFree = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_VOIP_NUM_IN_THIS_DEMO];
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
		WriteMsgLog(TmpStr);
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

	g_iTotalVOIP = 0;
	g_iTotalVOIPOpened = 0;

	g_iTotalVoice = 0;
	g_iTotalVoiceOpened = 0;
	g_iTotalVoiceFree = 0;

}

void	FreeOneDeviceRes ( int ID )
{
	if ( AllDeviceRes[ID].pVoice != NULL )
		delete [] AllDeviceRes[ID].pVoice;


	if ( AllDeviceRes[ID].pVOIP != NULL )
		delete [] AllDeviceRes[ID].pVOIP;

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
	int		VOIPCount, VocCount, ModuleCount;
	int		iModSeqID;

	// 将AllDeviceRes中的每个接口通道、语音通道、PCM通道所对应的（顺序号）记录下来
	ModuleCount = VOIPCount = VocCount =  0;
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

			 //	VOIP通道
			 iModSeqID = 0;
			 for ( j = 0; j < AllDeviceRes[i].lVOIPNum; j ++ )
			 {
				AllDeviceRes[i].pVOIP[j].iSeqID = VOIPCount;
				MapTable_VOIP[VOIPCount].m_s8ModuleID = AllDeviceRes[i].pVOIP[j].deviceID.m_s8ModuleID;
				MapTable_VOIP[VOIPCount].m_s16ChannelID = AllDeviceRes[i].pVOIP[j].deviceID.m_s16ChannelID;
				VOIPCount ++;			
			 }
		 }
	}

	g_iTotalModule = ModuleCount;
	g_iTotalVOIP = VOIPCount;
	g_iTotalVoice = VocCount;
}

void	AddDeviceRes_VOIP ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( AllDeviceRes[s8DspModID].lVOIPNum == 0 )		// 新增加的资源
	{
		// 
		AllDeviceRes[s8DspModID].pVOIP = new VOIP_STRUCT[s32Num];
		if( !AllDeviceRes[s8DspModID].pVOIP )
		{
			AllDeviceRes[s8DspModID].lVOIPNum = 0;
			AllDeviceRes[s8DspModID].lVOIPOpened = 0;

			// alloc fail, maybe disp this error in your log
			sprintf ( TmpStr, "new TRUNK_STRUCT[%d] fail in AddDeviceRes_Trunk()" );
			AddMsg ( TmpStr );
			WriteMsgLog(TmpStr);
		}
		else
		{
			AllDeviceRes[s8DspModID].lVOIPNum = s32Num;
			AllDeviceRes[s8DspModID].lVOIPOpened = 0;
			memset ( AllDeviceRes[s8DspModID].pVOIP, 0, sizeof(VOIP_STRUCT)*s32Num );
			
			DeviceID_t	 *pDev;

			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pVOIP[i].deviceID = pDev[i];
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
			WriteMsgLog(TmpStr);
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
		break;

	case XMS_DEVMAIN_DIGITAL_PORT:		
		break;
	
	case XMS_DEVMAIN_INTERFACE_CH:		
		break;

	case XMS_DEVMAIN_DSS1_LINK:			break;
	case XMS_DEVMAIN_SS7_LINK:			break;

	case XMS_DEVMAIN_BOARD:
		AddDeviceRes_Board ( s8DspModID, pAcsDevList );
		break;

	case XMS_DEVMAIN_CTBUS_TS:			break;
	case XMS_DEVMAIN_VOIP:	
		AddDeviceRes_VOIP ( s8DspModID, pAcsDevList );
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
			if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP  )
			{
				VOIPWork ( &M_OneVOIP(pAcsEvt->m_DeviceID), pAcsEvt );
			}
			else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
			{
				DeviceID_t	*pDevID;

				pDevID = &M_OneVoice(pAcsEvt->m_DeviceID).UsedDevID;

				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_VOIP )
				{					
					VOIPWork ( &M_OneVOIP(*pDevID), pAcsEvt );				
				}
			}
			break;
	}

}
