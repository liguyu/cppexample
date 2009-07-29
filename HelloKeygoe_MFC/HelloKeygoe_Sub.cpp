#include "stdafx.h"
#include "HelloKeygoe.h"
#include "HelloKeygoeDlg.h"

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "HelloKeygoe_Sub.h"

extern CHelloKeygoeApp theApp;

CHelloKeygoeDlg		*pdlg;
ACSHandle_t			g_acsHandle = -1;
ServerID_t	cfg_ServerID;
int					cfg_iPartWorkModuleID = 1;
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];


char *GetString_TrunkState(TRUNK_STRUCT *pOneTrunk){
	switch( pOneTrunk->State ) 
	{
	case TRK_WAITOPEN:				return "Wait Open"; 		
	case TRK_FREE:					return "Free"; 
	case TRK_WELCOME:				return "Welcome to DonJin Keygo Bank Demo";
	case USER_OFFHOOK:				return "OFFHOOK";
	}
	return "UNKNOWN";
}

char *GetString_LineState ( int iLineState )
{
	switch ( iLineState )
	{
	case DCS_FREE:							return "S_FREE";
	case DCS_LOCAL_BLOCK:				    return "S_LOCAL_BLOCK";
	case DCS_REMOTE_BLOCK:					return "S_REMOTE_BLOCK";
	case DCS_BLOCK:							return "S_BLOCK";
	case DCS_UNAVAILABLE:					return "S_UNAVAILABLE";
		
	case DCS_CALLOUT:						return "S_CALLOUT";
	case DCS_CALLIN:						return "S_CALLIN";	
	case DCS_ALERTING:						return "S_ALERTING";
		
	case DCS_CONNECT:						return "S_CONNECT";
	case DCS_DISCONNECT:					return "S_DISCONNECT";	
	case DCS_WAIT_FREE:						return "S_WAIT_FREE";	
	
	}
    return "UNKNOWN";
}

char *GetString_DeviceSub ( DJ_S32	s32DeviceSub )
{
	switch ( s32DeviceSub )
	{
	case XMS_DEVSUB_BASE:						return "BASE   ";
	case XMS_DEVSUB_ANALOG_TRUNK:				return "A_TRUNK";
	case XMS_DEVSUB_ANALOG_USER:				return "A_USER ";
	case XMS_DEVSUB_ANALOG_HIZ:					return "A_HIZ  ";
	case XMS_DEVSUB_ANALOG_EMPTY:				return "A_EMPTY";
	case XMS_DEVSUB_E1_PCM:						return "E1_PCM ";
	case XMS_DEVSUB_E1_CAS:						return "E1_CAS ";
	case XMS_DEVSUB_E1_DSS1:					return "E1_DSS1";
	case XMS_DEVSUB_E1_SS7_TUP:					return "E1_TUP ";
	case XMS_DEVSUB_E1_SS7_ISUP:				return "E1_ISUP";
	case XMS_DEVSUB_ANALOG_VOC2W:				return "A_VOC2W";
	case XMS_DEVSUB_ANALOG_VOC4W:				return "A_VOC4W";
	case XMS_DEVSUB_ANALOG_EM:					return "A_EM   ";
	case XMS_DEVSUB_E1_DCH:						return "E1_DCH ";
	case XMS_DEVSUB_E1_BCH:						return "E1_BCH ";
	case XMS_DEVSUB_UNUSABLE:					return "UNUSE  ";
		
	// new add next types from V2.2.0
	case XMS_DEVSUB_HIZ_CAS:					return "HIZ_CAS";
	case XMS_DEVSUB_HIZ_PRI:					return "HIZ_PRI";
	case XMS_DEVSUB_HIZ_SS7:					return "HIZ_SS7";
	case XMS_DEVSUB_HIZ_PRI_LINK:				return "HIZ_PRI_LINK";
	case XMS_DEVSUB_HIZ_SS7_64K_LINK:			return "HIZ_SS7_64K_LINK";
	case XMS_DEVSUB_HIZ_SS7_2M_LINK:			return "HIZ_SS7_2M_LINK";
	case XMS_DEVSUB_SS7_LINK:					return "SS7_LINK";
	case XMS_DEVSUB_LINESIDE:					return "LINESIDE";
	case XMS_DEVSUB_ANALOG_REC:					return "A_REC";
	case XMS_DEVSUB_DIGITAL_REC:				return "D_REC";
	case XMS_DEVSUB_HIZ_HDLC_N64K_LINK:			return "HDLC_N64_LINK";	
	}
	
	return "UNKNOWN";
}


void	InitListTrunk(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
		LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListTrunk.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	
	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListTrunk.InsertColumn ( 0, &lvc ); 
	
	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListTrunk.InsertColumn ( 1, &lvc ); 
	
	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListTrunk.InsertColumn ( 2, &lvc ); 
	
	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListTrunk.InsertColumn ( 3, &lvc ); 
	
	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListTrunk.InsertColumn ( 4, &lvc ); 

}

bool	InitSystem()
{
	RetCode_t	r;
	char		MsgStr[160];

	pdlg = (CHelloKeygoeDlg	*)theApp.m_pMainWnd;
	
	strcpy(cfg_ServerID.m_s8ServerIp,"192.168.11.111");
	cfg_ServerID.m_u32ServerPort = 9000;

	InitListTrunk();

	r = XMS_acsOpenStream( &g_acsHandle, 
		&cfg_ServerID,
		2,				// App Unit ID 
		32,				// sendQSize, in K Bytes
		32,				// recvQSize, in K Bytes
		0,				// Debug On/Off
		NULL);
	
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );
		
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
		return false;
	}else
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
		//MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
	}
	
	r = XMS_acsSetESR(g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
		return false;
	}else
	{
		sprintf ( MsgStr, "XMS_acsSetESR() OK!" );
		//MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
	}
	
	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );
	
	XMS_acsGetDeviceList ( g_acsHandle, NULL );
	
	return true;
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT *	pOneTrunk;
	VOICE_STRUCT *	pOneVoice;	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &AllDeviceRes[(*pDevice).m_s8ModuleID].pTrunk[(*pDevice).m_s16ChannelID];
		
		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		
		// init this Device: Trunk
		pOneTrunk->State = TRK_FREE;
		pOneTrunk->CallerCode[0]=0;
		pOneTrunk->CalledCode[0]=0;	
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
			
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		pdlg->m_ListTrunk.SetItemText ( pOneTrunk->iSeqID, 3, GetString_LineState(pOneTrunk->iLineState) ); 
		
		if (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
		{
			pdlg->m_ListTrunk.SetItemText ( pOneTrunk->iSeqID, 4, GetString_TrunkState(pOneTrunk) ); 
		}

		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;		
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		pOneVoice = &AllDeviceRes[(*pDevice).m_s8ModuleID].pVoice[(*pDevice).m_s16ChannelID];
		pOneVoice->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation

		// init this Device: Voice
		pOneVoice->State = VOC_FREE;
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );	
	
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;	
	}

}

DJ_Void EvtHandler(DJ_U32 esrParam){
	Acs_Evt_t *			    pAcsEvt = NULL;
	Acs_Dev_List_Head_t *	pAcsDevList = NULL;
	DJ_S8					s8DspModID;
	DJ_S32					s32Num;
	pAcsEvt = (Acs_Evt_t *) esrParam;
	VOICE_STRUCT *			pOneVoice;
	TRUNK_STRUCT *			pOneTrunk;
	char					TmpStr[256];
	Acs_GeneralProc_Data *pGeneralData = NULL;	

	switch ( pAcsEvt->m_s32EventType )
	{
	case XMS_EVT_QUERY_DEVICE:		
		{
			pAcsDevList = ( Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);	
			s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;
			s32Num =  pAcsDevList->m_s32DeviceNum;
			if (s8DspModID == cfg_iPartWorkModuleID)
			{
				switch (pAcsDevList->m_s32DeviceMain)
				{
				case XMS_DEVMAIN_VOICE:	
					if ( (AllDeviceRes[s8DspModID].lVocNum == 0) && (s32Num > 0) )		//  the resources new added
					{
						AllDeviceRes[s8DspModID].pVoice = new VOICE_STRUCT[s32Num];
						if( AllDeviceRes[s8DspModID].pVoice )
						{
							AllDeviceRes[s8DspModID].lVocNum = s32Num;
							AllDeviceRes[s8DspModID].lVocOpened = 0;
							AllDeviceRes[s8DspModID].lVocFreeNum = 0;
							memset ( AllDeviceRes[s8DspModID].pVoice, 0, sizeof(VOICE_STRUCT)*s32Num );
							
							DeviceID_t	 *pDev;
							pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
							
							for ( int i = 0; i < s32Num; i ++ )
							{
								AllDeviceRes[s8DspModID].pVoice[i].deviceID = pDev[i];
								AllDeviceRes[s8DspModID].pVoice[i].State = VOC_WAITOPEN;
							}
						}
					}
					break;
				case XMS_DEVMAIN_INTERFACE_CH:
					if ( (AllDeviceRes[s8DspModID].lTrunkNum == 0) && (s32Num > 0) )		//  the resources new added
					{
						AllDeviceRes[s8DspModID].pTrunk = new TRUNK_STRUCT[s32Num];
						if( AllDeviceRes[s8DspModID].pTrunk )
						{
							AllDeviceRes[s8DspModID].lTrunkNum = s32Num;
							AllDeviceRes[s8DspModID].lTrunkOpened = 0;
							memset ( AllDeviceRes[s8DspModID].pTrunk, 0, sizeof(TRUNK_STRUCT)*s32Num );
							
							DeviceID_t	 *pDev;
							
							pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
							
							for (int i = 0; i < s32Num; i ++ )
							{
								AllDeviceRes[s8DspModID].pTrunk[i].deviceID = pDev[i];
								AllDeviceRes[s8DspModID].pTrunk[i].State = TRK_WAITOPEN;
							}
						}
					}					
					break;
				}

			}
			
		}
		break; 
	case XMS_EVT_QUERY_ONE_DSP_END:	//一个DSP上的资源搜索完毕，打开应用需要的设备
		if (pAcsEvt->m_DeviceID.m_s8ModuleID == cfg_iPartWorkModuleID)
		{
			AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 1;		// this DSP can use
			s8DspModID = (DJ_S8) pAcsEvt->m_DeviceID.m_s8ModuleID;
			// pVoice
			for ( int i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
			{
				pOneVoice = &AllDeviceRes[s8DspModID].pVoice[i];			
				if ( pOneVoice->State == VOC_WAITOPEN )		// not Open yet
				{
					if ( XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL ) < 0 )
					{
						MessageBox(NULL, "Voice open fail!", "XMS_ctsOpenDevice", MB_OK ) ;
					}
				}
			}
						
			// pTrunk
			pdlg->m_ListTrunk.DeleteAllItems ();
			for ( int j = 0; j < AllDeviceRes[s8DspModID].lTrunkNum; j++ )
			{
				pOneTrunk = &AllDeviceRes[s8DspModID].pTrunk[j];
				pOneTrunk->iSeqID = j;
				if ( pOneTrunk->State == TRK_WAITOPEN )		// not Open yet
				{
					if ( XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL ) < 0 )
					{
						MessageBox(NULL, "Trunk open fail!", "XMS_ctsOpenDevice", MB_OK ) ;
					}
				}
			
				//界面显示中继信息
				sprintf ( TmpStr, "%3d", j );
				pdlg->m_ListTrunk.InsertItem ( j, TmpStr );
				
				sprintf ( TmpStr, "%d,%d", pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID );
				pdlg->m_ListTrunk.SetItemText ( j, 1, TmpStr );	
				
				pdlg->m_ListTrunk.SetItemText ( j, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );
				
				pdlg->m_ListTrunk.SetItemText ( j, 3, GetString_LineState(pOneTrunk->iLineState) ); 
			}
			
		}
		break;
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:		
		break;		
	case XMS_EVT_QUERY_DEVICE_END:	// Query Device List End
		break;		
	case XMS_EVT_OPEN_DEVICE:
		OpenDeviceOK(&pAcsEvt->m_DeviceID );
		break;		
	case XMS_EVT_CLOSE_DEVICE:		// before Delete DSP, DSP send event CloseDevice to the APP; call XMS_ctsCloseDevicey() can generate this Event
		break;		
	case XMS_EVT_DEVICESTATE:
			
		pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
		
		if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
		{
			pOneTrunk = &AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pTrunk[pAcsEvt->m_DeviceID.m_s16ChannelID];
			pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
			pdlg->m_ListTrunk.SetItemText ( pOneTrunk->iSeqID, 3, GetString_LineState(pOneTrunk->iLineState) ); 
		}
		break;		
	case XMS_EVT_UNIFAILURE:
		break;		
	default:
		if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			pAcsEvt->m_DeviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
		{
			pOneTrunk = &AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pTrunk[pAcsEvt->m_DeviceID.m_s16ChannelID];
			UserWork( pOneTrunk, pAcsEvt );
		}else if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE)
		{	
			DeviceID_t	*pDevID;
			pOneVoice = &AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pVoice[pAcsEvt->m_DeviceID.m_s16ChannelID];
			pDevID = &pOneVoice->UsedDevID;
			
			if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH && pDevID->m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
			{
				pOneTrunk = &AllDeviceRes[pDevID->m_s8ModuleID].pTrunk[pDevID->m_s16ChannelID];
				UserWork ( pOneTrunk, pAcsEvt );
			}

		}
		break;
	}
}
void ResetUser ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	VOICE_STRUCT * pOneVoice;
	if ( pOneUser->VocDevID.m_s16DeviceMain != 0 )//有语音设备与此中继连接
	{
		XMS_ctsUnlinkDevice ( g_acsHandle, &pOneUser->VocDevID, &pOneUser->deviceID, NULL ); 
		XMS_ctsUnlinkDevice ( g_acsHandle, &pOneUser->deviceID, &pOneUser->VocDevID, NULL ); 
		pOneVoice = &AllDeviceRes[pOneUser->VocDevID.m_s8ModuleID].pVoice[pOneUser->VocDevID.m_s16ChannelID];
		pOneVoice->State = VOC_FREE;
		AllDeviceRes[pOneVoice->deviceID.m_s8ModuleID].lVocFreeNum++;
	}
	
	pOneUser->State = TRK_FREE;
	memset( &pOneUser->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
	pdlg->m_ListTrunk.SetItemText ( pOneUser->iSeqID, 3, GetString_LineState(pOneUser->iLineState) ); 					
	pdlg->m_ListTrunk.SetItemText ( pOneUser->iSeqID, 4, GetString_TrunkState(pOneUser)); 					

}

void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	VOICE_STRUCT *			pOneVoice;
	RetCode_t				r;
	Acs_GeneralProc_Data	*pGeneral = NULL;
	Acs_CallControl_Data	*pCallControl = NULL;
	PlayProperty_t  playProperty;


	if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ){	
		if(pOneUser->State != TRK_FREE)
		{
			ResetUser(pOneUser,pAcsEvt);
			return;
		}
	}

	switch(pOneUser->State)
	{
	case TRK_FREE:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN ) /*当前状态USER_FREE,坐席摘机事件User Offhook Event*/
		{
			pOneVoice = &AllDeviceRes[pOneUser->deviceID.m_s8ModuleID].pVoice[pOneUser->iSeqID];
			if ( pOneVoice->State == VOC_FREE ){
				pOneVoice->State = VOC_USED;
				AllDeviceRes[pOneUser->deviceID.m_s8ModuleID].lVocFreeNum--;
				
				pOneUser->VocDevID = pOneVoice->deviceID;
				pOneVoice->UsedDevID = pOneUser->deviceID;

				XMS_ctsLinkDevice ( g_acsHandle, &pOneUser->deviceID, &pOneVoice->deviceID, NULL ); 
				XMS_ctsLinkDevice ( g_acsHandle, &pOneVoice->deviceID, &pOneUser->deviceID, NULL ); 
				
				memset(&playProperty,0,sizeof(playProperty));
				playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;	
				strcpy ( playProperty.m_s8PlayContent, "C:\\DJKeygoe\\Samples\\Voc\\Bank.001");
				r = XMS_ctsPlay ( g_acsHandle, &pOneVoice->deviceID, &playProperty, NULL );
				if (r < 0)
				{
					MessageBox(NULL, "XMS_ctsPlay fail!", "XMS_ctsPlay", MB_OK ) ;
				}else{
					pOneUser->State = USER_OFFHOOK;
					pdlg->m_ListTrunk.SetItemText ( pOneUser->iSeqID, 4, GetString_TrunkState(pOneUser)); 					

				}

			}

		}
		break;
		
	case USER_OFFHOOK:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_PLAY ){
			ResetUser(pOneUser,pAcsEvt);
		}	
		break;

	}
}
	
void	ExitSystem() 
{
	int			i;
	RetCode_t	r;
	// 关闭中继设备
	for ( i = 0; i < AllDeviceRes[cfg_iPartWorkModuleID].lTrunkNum; i++ )
	{
		XMS_ctsCloseDevice ( g_acsHandle, &AllDeviceRes[cfg_iPartWorkModuleID].pTrunk[i].deviceID, NULL );		
	}	
	// 关闭语音设备
	for ( i = 0; i < AllDeviceRes[cfg_iPartWorkModuleID].lVocNum; i++ )
	{
		XMS_ctsCloseDevice ( g_acsHandle, &AllDeviceRes[cfg_iPartWorkModuleID].pVoice[i].deviceID, NULL );		
	}	
	
	r = XMS_acsCloseStream ( g_acsHandle, NULL );

	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );
}
