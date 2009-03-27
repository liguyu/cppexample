#include "stdafx.h"
#include "XMS_HizDemo.h"
#include "XMS_HizDemoDlg.h"


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMS_HizDemo_Sub.H"
#include "XMS_HizDemo_Event.H"
#include "XMS_HizDemo_String.H"

extern int				cfg_iPartWork;
extern int				cfg_iPartWorkModuleID;

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

int						g_iTotalVoice = 0;
int						g_iTotalVoiceOpened = 0;
int						g_iTotalVoiceFree = 0;
TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_TRUNK_NUM_IN_THIS_DEMO];
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
		break;

	default:
		break;

	}

	AddMsg ( TmpStr );
}

// --------------------------------------------------------------------------
void	InitAllDeviceRes (void)
{
	// clease the AllDeviceRes, include: lFlag, all the Total, the pointer clear to NULL
	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );

	g_iTotalModule = 0;

	g_iTotalPcm = 0;
	g_iTotalPcmOpened = 0;

	g_iTotalTrunk = 0;
	g_iTotalTrunkOpened = 0;

	g_iTotalVoice = 0;
	g_iTotalVoiceOpened = 0;
	g_iTotalVoiceFree = 0;

}

void	FreeOneDeviceRes ( int ID )
{
	if ( AllDeviceRes[ID].pVoice != NULL )
		delete [] AllDeviceRes[ID].pVoice;

	if ( AllDeviceRes[ID].pPcm != NULL )
		delete [] AllDeviceRes[ID].pPcm;

	if ( AllDeviceRes[ID].pTrunk != NULL )
		delete [] AllDeviceRes[ID].pTrunk;

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
	int		TrkCount, VocCount, PcmCount, ModuleCount;
	int		iModSeqID;

	// Remember the AllDeviceRes's Interface,Voice,PCM channel
	ModuleCount = TrkCount = VocCount = PcmCount = 0;
	for ( i = 0; i < MAX_DSP_MODULE_NUMBER_OF_XMS; i ++ )
	{
		 if ( AllDeviceRes[i].lFlag == 1 )
		 {
			// DSP Module
			AllDeviceRes[i].iSeqID = ModuleCount;
			MapTable_Module[ModuleCount] = i;
			ModuleCount ++;

			 // Voice Channel
			 for ( j = 0; j < AllDeviceRes[i].lVocNum; j ++ )
			 {
				AllDeviceRes[i].pVoice[j].iSeqID = VocCount;
				MapTable_Voice[VocCount].m_s8ModuleID = AllDeviceRes[i].pVoice[j].deviceID.m_s8ModuleID;
				MapTable_Voice[VocCount].m_s16ChannelID = AllDeviceRes[i].pVoice[j].deviceID.m_s16ChannelID;
				VocCount ++;
			 }

			 // PCM Channel
			 for ( j = 0; j < AllDeviceRes[i].lPcmNum; j ++ )
			 {
				AllDeviceRes[i].pPcm[j].iSeqID = PcmCount;
				MapTable_Pcm[PcmCount].m_s8ModuleID = AllDeviceRes[i].pPcm[j].deviceID.m_s8ModuleID;
				MapTable_Pcm[PcmCount].m_s16ChannelID = AllDeviceRes[i].pPcm[j].deviceID.m_s16ChannelID;
				PcmCount ++;
			 }

			 // Interface Channel
			 iModSeqID = 0;
			 for ( j = 0; j < AllDeviceRes[i].lTrunkNum; j ++ )
			 {
				AllDeviceRes[i].pTrunk[j].iSeqID = TrkCount;
				MapTable_Trunk[TrkCount].m_s8ModuleID = AllDeviceRes[i].pTrunk[j].deviceID.m_s8ModuleID;
				MapTable_Trunk[TrkCount].m_s16ChannelID = AllDeviceRes[i].pTrunk[j].deviceID.m_s16ChannelID;
				TrkCount ++;
				if ( AllDeviceRes[i].pTrunk[j].deviceID.m_s16DeviceSub != XMS_DEVSUB_UNUSABLE )
				{
					// Available Channel
					AllDeviceRes[i].pTrunk[j].iModSeqID = iModSeqID;
					iModSeqID ++;

					//////////////////////////////
					InitMGNode( &AllDeviceRes[i].pTrunk[j] );
					//////////////////////////////
				}
			 }
		 }
	}

	g_iTotalModule = ModuleCount;
	g_iTotalTrunk = TrkCount;
	g_iTotalVoice = VocCount;
	g_iTotalPcm = PcmCount;

}

void	AddDeviceRes_Trunk ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( (AllDeviceRes[s8DspModID].lTrunkNum == 0) && (s32Num > 0) )		// the resources new added
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
				AllDeviceRes[s8DspModID].pTrunk[i].State = TRK_WAITOPEN;
				AllDeviceRes[s8DspModID].pTrunk[i].u8IsBGFlag = FALSE;
				AllDeviceRes[s8DspModID].pTrunk[i].u8IsBGTxFlag = FALSE;
			}
		}
	}
	else if ( (AllDeviceRes[s8DspModID].lTrunkNum > 0) && (s32Num == 0) )		// delete this resource
	{
		// if some devices did not close, close them
		for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i ++ )
		{
			if ( AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAITOPEN )
				CloseDeviceOK ( &AllDeviceRes[s8DspModID].pTrunk[i].deviceID );
		}

		AllDeviceRes[s8DspModID].lTrunkNum = 0;
		AllDeviceRes[s8DspModID].lTrunkOpened = 0;

		delete [] AllDeviceRes[s8DspModID].pTrunk;
		AllDeviceRes[s8DspModID].pTrunk = NULL;
	}

}

void	AddDeviceRes_Voice ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( (AllDeviceRes[s8DspModID].lVocNum == 0) && (s32Num > 0) )		//  the resources new added
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
			memset ( AllDeviceRes[s8DspModID].pVoice, 0, sizeof(VOICE_STRUCT)*s32Num );
			
			DeviceID_t	 *pDev;
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pVoice[i].deviceID = pDev[i];
				AllDeviceRes[s8DspModID].pVoice[i].State = VOC_WAITOPEN;
			}
		}
	}
	else if ( (AllDeviceRes[s8DspModID].lVocNum > 0) && (s32Num == 0) )		// delete this resource
	{
		// if some devices did not close, close them
		for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i ++ )
		{
			if ( AllDeviceRes[s8DspModID].pVoice[i].State != VOC_WAITOPEN )
				CloseDeviceOK ( &AllDeviceRes[s8DspModID].pVoice[i].deviceID );
		}

		AllDeviceRes[s8DspModID].lVocNum = 0;
		AllDeviceRes[s8DspModID].lVocOpened = 0;
		AllDeviceRes[s8DspModID].lVocFreeNum = 0;

		delete [] AllDeviceRes[s8DspModID].pVoice;
		AllDeviceRes[s8DspModID].pVoice = NULL;
	}

}

void	AddDeviceRes_Pcm ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	char	TmpStr[256];

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( (AllDeviceRes[s8DspModID].lPcmNum == 0) && (s32Num > 0) )		// the resources new added
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
				AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag = false;
			}
		}
	}
	else if ( (AllDeviceRes[s8DspModID].lPcmNum > 0) && (s32Num == 0) )		// delete this resource
	{
		// if some devices did not close, close them
		for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i ++ )
		{
			if ( AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag != false )
				CloseDeviceOK ( &AllDeviceRes[s8DspModID].pPcm[i].deviceID );
		}

		AllDeviceRes[s8DspModID].lPcmNum = 0;
		AllDeviceRes[s8DspModID].lPcmOpened = 0;
	
		delete [] AllDeviceRes[s8DspModID].pPcm;
		AllDeviceRes[s8DspModID].pPcm = NULL;
	}

}

void	AddDeviceRes_Board ( DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;

	s32Num = pAcsDevList->m_s32DeviceNum;

	if ( (AllDeviceRes[s8DspModID].lFlag == 0) && (s32Num > 0) )		// the resources new added
	{
		DeviceID_t	 *pDev;
		pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));

		AllDeviceRes[s8DspModID].deviceID = pDev[0];
		AllDeviceRes[s8DspModID].bOpenFlag = false;
		AllDeviceRes[s8DspModID].bErrFlag = false;
		AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_NONE;
	}
	else if ( (AllDeviceRes[s8DspModID].lFlag == 1) && (s32Num == 0) )	// delete this resource
	{
		// if some devices did not close, close them
		if ( AllDeviceRes[s8DspModID].bOpenFlag != false )
			CloseDeviceOK ( &AllDeviceRes[s8DspModID].deviceID );

		memset ( &AllDeviceRes[s8DspModID].deviceID, 0, sizeof(DeviceID_t) );
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


DJ_Void EvtHandler(DJ_U32 esrParam)
{
	Acs_Evt_t *			    pAcsEvt = NULL;
	Acs_Dev_List_Head_t * pAcsDevList = NULL;


	pAcsEvt = (Acs_Evt_t *) esrParam;
	DispEventInfo ( pAcsEvt );

	switch ( pAcsEvt->m_s32EventType )
	{
		case XMS_EVT_QUERY_DEVICE:
			if ( ( cfg_iPartWork == 0 ) || (pAcsEvt->m_DeviceID.m_s8ModuleID == cfg_iPartWorkModuleID) )
			{
				pAcsDevList = ( Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);

				// receive the event of Device Resources changed, Add/Delete this Device Resources
				AddDeviceRes ( pAcsDevList );
			}
			break; 
		case XMS_EVT_QUERY_ONE_DSP_END:
		case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:
			if ( ( cfg_iPartWork == 0 ) || (pAcsEvt->m_DeviceID.m_s8ModuleID == cfg_iPartWorkModuleID) )
			{
				if ( pAcsEvt->m_s32EventType == XMS_EVT_QUERY_ONE_DSP_END )
				{
					AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 1;		// this DSP can use
					OpenAllDevice_Dsp ( pAcsEvt->m_DeviceID.m_s8ModuleID );
				}
				else
				{
					AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 0;		// Remove DSP Over
				}

				RefreshMapTable ( );
				ReDrawAll ();
			}
			break;

		case XMS_EVT_QUERY_DEVICE_END:	// Query Device List End
			break;

		case XMS_EVT_OPEN_DEVICE:
			OpenDeviceOK ( &pAcsEvt->m_DeviceID );
			break;

		case XMS_EVT_CLOSE_DEVICE:		// before Delete DSP, DSP send event CloseDevice to the APP; call XMS_ctsCloseDevicey() can generate this Event
			CloseDeviceOK ( &pAcsEvt->m_DeviceID );
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
				if ( XMS_DEVSUB_HIZ_SS7 == pAcsEvt->m_DeviceID.m_s16DeviceSub 
				  || XMS_DEVSUB_HIZ_SS7_64K_LINK == pAcsEvt->m_DeviceID.m_s16DeviceSub)
				{
					TRUNK_STRUCT *pOneTrunk = &M_OneTrunk(pAcsEvt->m_DeviceID);

					if ( NULL != pOneTrunk )
					{
// 						if ( TRUE == pOneTrunk->u8IsBGFlag ) //is BG channel
// 						{
// 							if ( TRUE == pOneTrunk->u8IsBGTxFlag )  //is the TX channel
// 							{
								TrunkWork ( pOneTrunk, pAcsEvt );	
// 							}
// 							else //is not the TX channel, and get the appointed TX channel
// 							{
// 								pOneTrunk = &M_OneTrunk(pOneTrunk->BGDevID);
// 								if ( NULL != pOneTrunk )
// 								{
// 									TrunkWork ( pOneTrunk, pAcsEvt );
// 								}
// 							}
// 						}
					}				
				}
				else
				{
					TrunkWork ( &M_OneTrunk(pAcsEvt->m_DeviceID), pAcsEvt );
				}

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
			break;
	}

}
