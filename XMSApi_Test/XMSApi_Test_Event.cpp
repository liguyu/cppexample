#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_Sub.h"
#include "XMSApi_Test_Event.h"
#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Event.h"

// -------------------------------------------------------
extern	CXMSApi_TestDlg	*pdlg;

int		TotalDevRes = 0;
TYPE_MY_DEVICE_STRUCT	*pDevRes = NULL;

int		TotalDevOpened = 0;
TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

FILE * g_cspFp = NULL;
char   g_szCSPFile[MAX_PATH]={0};
char   g_szDataBuf[4000] = {0};
int    g_cspPlay = 0;

int			iNowOpIndex = -1;
int			iOpenedIndex1 = -1;
int			iOpenedIndex2 = -1;
int         iOpenedIndex3 = -1;

extern FILE * g_cspFp;
extern char   g_szCSPFile[MAX_PATH];

extern bool				bStartWorkFlag;
extern ACSHandle_t		g_acsHandle;
extern CXMSApi_TestDlg	*pdlg;
extern int g_iSendRecv;



void EncodeFskCaller(char* ioDataBuf,char* ioFskBuf,int* iFskLen)
{
	time_t tm1;
	char szSysTime[30];
	struct tm* newtime;

	int iTotal = 0;
	int ioDataLen =strlen(ioDataBuf);

	time(&tm1);
	newtime = localtime(&tm1);
	sprintf(szSysTime,"%02d%02d%02d%02d",
		newtime->tm_mon+1,
		newtime->tm_mday,
		newtime->tm_hour,
		newtime->tm_min);
	
	

	memset(ioFskBuf,0,sizeof(ioFskBuf));

	{
  
		int callerLen = strlen(ioDataBuf);
    
		ioFskBuf[0] = 4 ;
		ioFskBuf[1] = callerLen + 8;
    
		memcpy(ioFskBuf+2,szSysTime,8);
		memcpy(ioFskBuf+10,ioDataBuf,callerLen);
    
		int iTotal = 0 ;
    
		for( int iLoop = 0 ; iLoop< (2+8+ callerLen); iLoop++)
		{
			iTotal += ioFskBuf[iLoop];  
		}
 
		//printf("---AnalogUser Cksum orginal=[%d]---\n",iTotal);
		iTotal %= 256;
		unsigned char cCompliment = iTotal;

		ioFskBuf[10+callerLen] = (~((unsigned char)cCompliment)) + 1;
		ioDataLen = callerLen + 11; 
		*iFskLen = ioDataLen;

	}
}

extern int g_iLen;
extern char g_TmpStr[1024];

// -------------------------------------------------------
void	DispEventInfo ( Acs_Evt_t *pAcsEvt )
{
	char	TmpStr[512] = {0};
	char	TmpS[256] = {0};

	char		  SendTiffFileName[ACS_MAX_FILE_NAME_LEN];
	char		  RecvTiffFileName[ACS_MAX_FILE_NAME_LEN];
	
	char dtmf='?';

	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_UniFailure_Data * pAcsUniFailure = NULL;
	Acs_GeneralProc_Data *pGeneralData = NULL;
	Acs_CallControl_Data *pCallControl = NULL;
	Acs_IO_Data			 *pIOData = NULL;
	Acs_CAS_ExtraInfo_t  *pCAS_ExtraInfo = NULL;
	Acs_AnalogInterface_Data* pAnalogInterface=NULL;
	Acs_CSPDataReq_Data* pCspReqData=NULL;
	Acs_ParamProc_Data*  pSetParam=NULL;
	Acs_ParamProc_Data*  pGetParam=NULL;
	Acs_MediaProc_Data* pMedia=NULL;
	Acs_ParamData_UserReadXmsBoardInfo* pBoardInfor=NULL;

	sprintf ( TmpStr, "" );
	strcat ( TmpStr, GetString_EventType ( pAcsEvt->m_s32EventType ) );

	Acs_MediaProc_Data * pMediaData = (Acs_MediaProc_Data *)FetchEventData(pAcsEvt);

	switch ( pAcsEvt->m_s32EventType )
	{
	case XMS_EVT_GETPARAM:
		{
			pGetParam = (Acs_ParamProc_Data*)FetchEventData(pAcsEvt);
			pBoardInfor = (Acs_ParamData_UserReadXmsBoardInfo*)((char*)pGetParam+sizeof(Acs_ParamProc_Data));
		}
		break;
	case XMS_EVT_DEV_TIMER:
		{

			char ioFskBuf[512];
			int iFskLen;
			char MsgStr[512];

			memset(ioFskBuf,0,sizeof(ioFskBuf));
			pdlg->GetDlgItemText(IDC_EDIT_CALLINGNUM,MsgStr,sizeof(MsgStr));
			EncodeFskCaller(MsgStr,ioFskBuf,&iFskLen);

		    int r = XMS_ctsSendIOData ( g_acsHandle, &pAcsEvt->m_DeviceID, 
			XMS_IO_TYPE_FSK, (DJ_U16)iFskLen, (DJ_Void *) ioFskBuf );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData( %s, %d, [%s] ) FAIL! (%s)", 
					r, "GTG ", g_iLen, TmpStr, 
					GetString_DeviceAll (&pAcsEvt->m_DeviceID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%s] ) OK! (%s)", 
					"GTG ", g_iLen, TmpStr,  
					GetString_DeviceAll (&pAcsEvt->m_DeviceID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
		}
		break;
	case XMS_EVT_BUILDINDEX:
		pMedia=(Acs_MediaProc_Data*)FetchEventData(pAcsEvt);
		sprintf(TmpS," EvtState=%d taskID=%d fileIndex=%u,%x",pMedia->m_s32AcsEvtState,pMedia->m_u8TaskID,pMedia->m_u32CurFileIndex,pMedia->m_u32CurFileIndex);
		strcat(TmpStr,TmpS);
		break;
	case XMS_EVT_PLAY:
		pMedia=(Acs_MediaProc_Data*)FetchEventData(pAcsEvt);
		sprintf(TmpS," EvtState=%d Errcode=%d taskID=%d fileIndex=%d",pMedia->m_s32AcsEvtState,pMedia->m_s32AcsEvtErrCode,pMedia->m_u8TaskID,pMedia->m_u32CurFileIndex);
		strcat(TmpStr,TmpS);
		break;
	case XMS_EVT_SETPARAM:
		pSetParam = (Acs_ParamProc_Data*)FetchEventData(pAcsEvt);
		sprintf(TmpS," CmdType=[%d]",pSetParam->m_u16ParamCmdType);
		strcat(TmpStr,TmpS);
		break;
	case XMS_EVT_UNLINKDEVICE:
		pGeneralData = (Acs_GeneralProc_Data*) FetchEventData(pAcsEvt);
		break;
	case XMS_EVT_PLAYCSPREQ:
		//
		pCspReqData = (Acs_CSPDataReq_Data*)FetchEventData(pAcsEvt);
		sprintf(TmpS,"CSPPLAYREQ datalen =%d",pCspReqData->m_u16ReqCspDataLen);
		strcat(TmpStr,TmpS);
		break;

	case XMS_EVT_ANALOG_INTERFACE:
		pAnalogInterface = (Acs_AnalogInterface_Data*)FetchEventData(pAcsEvt);
		sprintf(TmpS,"(%d,%s)",pAnalogInterface->m_u8AnalogInterfaceState,GetString_AnalogInterfaceState(pAcsEvt->m_DeviceID.m_s16DeviceSub,pAnalogInterface->m_u8AnalogInterfaceState));
		strcat(TmpStr,TmpS);
		break;
	case XMS_EVT_SENDIODATA:
		sprintf ( TmpS, " (%s,%s,MachineID:%2d,ModuleID:%3d,Ch:%d)", 
			GetString_DeviceMain(pAcsEvt->m_DeviceID.m_s16DeviceMain),
			GetString_DeviceSub(pAcsEvt->m_DeviceID.m_s16DeviceMain,pAcsEvt->m_DeviceID.m_s16DeviceSub),
			pAcsEvt->m_DeviceID.m_s8MachineID,
			pAcsEvt->m_DeviceID.m_s8ModuleID,
			pAcsEvt->m_DeviceID.m_s16ChannelID );
		/*strcat ( TmpStr, TmpS );
		{
			CmdParamData_AnalogTrunk_t cmdAnalogTrunk;
			void*p =(void*)&cmdAnalogTrunk; ;
			XMS_ctsSetParam(g_acsHandle,&pAcsEvt->m_DeviceID,ANALOGTRUNK_PARAM_UNBINDVOC,sizeof(CmdParamData_AnalogTrunk_t),p);
			////
		}*/
		strcat(TmpStr,TmpS);
		break;
	case XMS_EVT_LINKDEVICE:
	case XMS_EVT_CLEARCALL:
		sprintf ( TmpS, " (%s,%s,MachineID:%2d,ModuleID:%3d,Ch:%d)", 
			GetString_DeviceMain(pAcsEvt->m_DeviceID.m_s16DeviceMain),
			GetString_DeviceSub(pAcsEvt->m_DeviceID.m_s16DeviceMain,pAcsEvt->m_DeviceID.m_s16DeviceSub),
			pAcsEvt->m_DeviceID.m_s8MachineID,
			pAcsEvt->m_DeviceID.m_s8ModuleID,
			pAcsEvt->m_DeviceID.m_s16ChannelID );
		strcat ( TmpStr, TmpS );
		break;
	case XMS_EVT_OPEN_STREAM:
		break;

	case XMS_EVT_QUERY_DEVICE:
		pAcsDevList = (Acs_Dev_List_Head_t *)FetchEventData(pAcsEvt);
		sprintf ( TmpS, " (%s,%2d,%3d)", 
			GetString_DeviceMain(pAcsDevList->m_s32DeviceMain),
			pAcsDevList->m_s32ModuleID,
			pAcsDevList->m_s32DeviceNum );
		strcat ( TmpStr, TmpS );
		break;

	case XMS_EVT_QUERY_ONE_DSP_START:
	case XMS_EVT_QUERY_ONE_DSP_END:
		sprintf ( TmpS, " (%d)", pAcsEvt->m_DeviceID.m_s8ModuleID );
		strcat ( TmpStr, TmpS );
		break;

	case XMS_EVT_OPEN_DEVICE:
		{
			pGeneralData = (Acs_GeneralProc_Data*)FetchEventData(pAcsEvt);
			int isize = sizeof(Acs_GeneralProc_Evt);
			 isize = sizeof(Acs_GeneralProc_Data);
			
			sprintf ( TmpS, "(%s) CallID(%d,%d)",  GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
				pAcsEvt->m_DeviceID.m_CallID.m_s32FlowType, pAcsEvt->m_DeviceID.m_CallID.m_s32FlowChannel );
			strcat ( TmpStr, TmpS );
		}
		break;

	case XMS_EVT_DEVICESTATE:
		pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);

		sprintf ( TmpS, "(%s) S=0x%X(%s)", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
			pGeneralData->m_s32DeviceState, GetString_DeviceState(pGeneralData->m_s32DeviceState) );
		strcat ( TmpStr, TmpS );
		break;

	case XMS_EVT_CALLIN:
		pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

		sprintf ( TmpS, "(%s) (%s,%s)", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
			 pCallControl->m_s8CallingNum, pCallControl->m_s8CalledNum); 
		strcat ( TmpStr, TmpS );
		break;

	case XMS_EVT_CALLOUT:
		pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);

		sprintf ( TmpS, "(%s) EvtState=%d, ErrCode=%d,ClearCause=%d) calling=%s called=%s", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
			pCallControl->m_s32AcsEvtState, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause,pCallControl->m_s8CallingNum,pCallControl->m_s8CalledNum );  
		strcat ( TmpStr, TmpS );
		break;

	case XMS_EVT_RECVIODATA:
		{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);


		//strcat ( TmpStr, TmpS );
		//AddMsg ( MSG_TYPE_EVENT, TmpStr );
		

		pdlg->GetDlgItem ( IDC_EDIT_SENDFAX )->GetWindowText ( SendTiffFileName, ACS_MAX_FILE_NAME_LEN-1 );
		pdlg->GetDlgItem ( IDC_EDIT_RECEIVEFAX )->GetWindowText ( RecvTiffFileName, ACS_MAX_FILE_NAME_LEN-1 );
		//sprintf ( s8LocalID, "DJ:83636988" );

		if( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
		{
			//Fax Process....begin
			switch (pIOData->m_u16IoType)
			{
				case XMS_IO_TYPE_VOIP_H245UII:
					if (pIOData->m_u16IoDataLen == sizeof(Acs_VoIP_H245UII))
					{
						Acs_VoIP_H245UII *h245Uii = (Acs_VoIP_H245UII *)FetchIOData(pAcsEvt);
						dtmf = h245Uii->m_s8Signal;
					}
					sprintf ( TmpS, "(%s) (%s,%d) [%c]", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
					GetString_IOType(pIOData->m_u16IoType),  pIOData->m_u16IoDataLen, dtmf ); 
					break;
				case XMS_IO_TYPE_VOIP_SIPINFO:
					if (pIOData->m_u16IoDataLen == sizeof(Acs_VoIP_SIPINFO))
					{
						Acs_VoIP_SIPINFO *sipInfo = (Acs_VoIP_SIPINFO *)FetchIOData(pAcsEvt);
						if (strcmp(sipInfo->m_s8ContentType, "application/dtmf") == 0)
							sscanf(sipInfo->m_s8Body, "dtmf=%c", &dtmf);
					}
					sprintf ( TmpS, "(%s) (%s,%d) [%c]", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
					GetString_IOType(pIOData->m_u16IoType),  pIOData->m_u16IoDataLen, dtmf ); 
					break;
				case XMS_IO_TYPE_VOIP_RTPDTMF:
					if (pIOData->m_u16IoDataLen == sizeof(Acs_VoIP_RTPDTMF))
					{
						Acs_VoIP_RTPDTMF *rtpDtmf = (Acs_VoIP_RTPDTMF *)FetchIOData(pAcsEvt);
						dtmf = rtpDtmf->m_s8EventChar;
					}
					sprintf ( TmpS, "(%s) (%s,%d) [%c]", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
					GetString_IOType(pIOData->m_u16IoType),  pIOData->m_u16IoDataLen, dtmf ); 
					break;
				case XMS_IO_TYPE_VOIP_T38START:
					if (pIOData->m_u16IoDataLen == sizeof(DJ_S32))
					{
						DJ_S32 *ret = (DJ_S32 *)FetchIOData(pAcsEvt);
						if (*ret == 0)
						{
							

							pdlg->UpdateData();

							if (g_iSendRecv == 0)//SendFax
							{
								//pdlg->m_lstIP.SetItemText(iDevice, 3, _T("Send Fax"));

								XMS_ctsSendFax(g_acsHandle,
									&DevOpened[iOpenedIndex1].DevID,
									&DevOpened[iOpenedIndex2].DevID,
									//&g_deviceVoIP[iDevice].device,
									(DJ_S8 *)(LPCTSTR)SendTiffFileName, "1234", NULL);

								sprintf ( TmpS, "(%s) (%s,%d) T38START SENDFAX", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
								GetString_IOType(pIOData->m_u16IoType),  pIOData->m_u16IoDataLen  ); 
							}
							else
							{
								//pdlg->m_lstIP.SetItemText(iDevice, 3, _T("Recv Fax"));
								XMS_ctsReceiveFax(g_acsHandle,
									&DevOpened[iOpenedIndex1].DevID,
									&DevOpened[iOpenedIndex2].DevID,
									//&g_deviceVoIP[iDevice].device,
									(DJ_S8 *)(LPCTSTR)RecvTiffFileName, "1234", NULL);

								sprintf ( TmpS, "(%s) (%s,%d) T38START RECVFAX", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
								GetString_IOType(pIOData->m_u16IoType),  pIOData->m_u16IoDataLen  ); 
							}
						}
					}
			
					
					break;
				//default:
				//	sprintf ( TmpS, "(%s) (%s,%d) INVALID IOTYPE FOR VOIP", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
				//				GetString_IOType(pIOData->m_u16IoType),  pIOData->m_u16IoDataLen  ); 
				//	break;

				
			}
		//Fax Process....end

		}
		else //NO IP Device
		{
			char *p;
			p = (char *)(pIOData) + sizeof(Acs_IO_Data);
			switch( pIOData->m_u16IoType)
			{
			case XMS_IO_TYPE_DTMF:
				sprintf ( TmpS, "(%s) (%s,%d) [%s]", 
				GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
				GetString_IOType(pIOData->m_u16IoType),  
				pIOData->m_u16IoDataLen, 
				p ); 
				break;
			case XMS_IO_TYPE_GTG:
				sprintf ( TmpS, "(%s) (%s,%d) [%c]", 
				GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
				GetString_IOType(pIOData->m_u16IoType),  
				pIOData->m_u16IoDataLen, 
				*p ); 
				break;
			case XMS_IO_TYPE_FSK:
				sprintf ( TmpS, "(%s) (%s,Len:%d) [%d][%d][%d][%s]", 
				GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
				GetString_IOType(pIOData->m_u16IoType),  
				pIOData->m_u16IoDataLen, 
				p[0],p[1],p[2],p ); 
				break;
			
			case XMS_IO_TYPE_PVD:
				sprintf ( TmpS, "(%s) (%s,%d) [%c]", 
				GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
				GetString_IOType(pIOData->m_u16IoType),  
				pIOData->m_u16IoDataLen, 
				*p ); 
				break;
			default:
				break;
			}
			
		}

		}
		strcat ( TmpStr, TmpS );
		//AddMsg ( MSG_TYPE_EVENT, TmpStr );
		break;

	case XMS_EVT_CAS_MFC_START:
	case XMS_EVT_CAS_MFC_END:
		pCAS_ExtraInfo = (Acs_CAS_ExtraInfo_t *)FetchEventData(pAcsEvt);

		sprintf ( TmpS, "(%s) KA=%d, KD=%d)", GetString_DeviceAll(&pAcsEvt->m_DeviceID), 
			pCAS_ExtraInfo->m_u8KA, pCAS_ExtraInfo->m_u8KD );
		strcat ( TmpStr, TmpS );
		break;


	case XMS_EVT_UNIFAILURE:
		pAcsUniFailure = (Acs_UniFailure_Data *)FetchEventData(pAcsEvt);

		sprintf ( TmpS, ": %s(0x%X)", GetString_ErrorCode(pAcsUniFailure->m_s32AcsEvtErrCode), pAcsUniFailure->m_s32AcsEvtErrCode );
		strcat ( TmpStr, TmpS );

		if ( pAcsEvt->m_DeviceID.m_s16DeviceMain != 0 )
		{
			//Unifailure event that some device link each other
			sprintf ( TmpS, " (%s)", GetString_DeviceAll(&pAcsEvt->m_DeviceID) );
			strcat ( TmpStr, TmpS );
		}
		break;
	default:
		break;

	}

	AddMsg ( MSG_TYPE_EVENT, TmpStr );
}

void	InitDeviceRes (void)
{
	TotalDevRes = 0;

	TotalDevOpened = 0;
}

void	FreeDeviceRes (void)
{
	if ( pDevRes != NULL )
	{
		free ( pDevRes );
		pDevRes = NULL;
	}

	TotalDevRes = 0;
	TotalDevOpened = 0;
}

void	AddDeviceRes ( Acs_Dev_List_Head_t *pAcsDevList )
{
	char	TmpStr[256];
	void	*p;
	int		i;
	DeviceID_t	*pDev;


	if ( pDevRes == NULL )
	{
		pDevRes = (TYPE_MY_DEVICE_STRUCT *) malloc ( pAcsDevList->m_s32DeviceNum * sizeof(TYPE_MY_DEVICE_STRUCT) );
		if ( pDevRes == NULL )
		{
			AddMsg ( MSG_TYPE_OTHER, "malloc() Fail in My Function AddDeviceRes()!" );
			return;
		}
	}
	else
	{
		p = realloc ( pDevRes, (TotalDevRes + pAcsDevList->m_s32DeviceNum) * sizeof(TYPE_MY_DEVICE_STRUCT) );
		if ( p == NULL )
		{
			sprintf ( TmpStr, "realloc() from %d to %d Fail in My Function AddDeviceRes()!", 
				TotalDevRes,  TotalDevRes + pAcsDevList->m_s32DeviceNum );
			AddMsg ( MSG_TYPE_OTHER, TmpStr );
			return;
		}

		pDevRes = (TYPE_MY_DEVICE_STRUCT *)p;
	}
	
	pDev = (DeviceID_t *) ((DJ_S8 *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
	for ( i = 0; i < pAcsDevList->m_s32DeviceNum; i ++ )
	{
		pDevRes[TotalDevRes + i].DevID = pDev[i];
	}

	TotalDevRes += pAcsDevList->m_s32DeviceNum;

}

void	OpenDeviceOK ( DeviceID_t	*pDev )
{
	char		TmpStr[256];

	if ( TotalDevOpened >= MAX_DEVICE_ALLOW_OPEN )
	{
		AddMsg ( MSG_TYPE_OTHER, "Add OpenDevice Fail. overflow." );
	}
	else
	{
		DevOpened[TotalDevOpened].DevID = *pDev;
		DevOpened[TotalDevOpened].bOpenFlag = true;

		sprintf ( TmpStr, "%d-(%s) (%d,%d)", TotalDevOpened, GetString_DeviceAll(pDev),
			pDev->m_CallID.m_s32FlowType, pDev->m_CallID.m_s32FlowChannel );
		pdlg->m_ListDevOpened.AddString ( TmpStr );

		if ( TotalDevOpened == 0 )
		{
			//it is fill into IDC_EDIT_DEVICE1 block automatic if opened the first deviceID
			//if ( pDev->m_s16DeviceMain != XMS_DEVMAIN_VOIP)
				 iOpenedIndex1 = 0;
			//else
			//	 iOpenedIndex3 = 0;

			//pdlg->GetDlgItem ( IDC_EDIT_DEVICE1 )->SetWindowText ( GetString_DeviceAll(&DevOpened[iOpenedIndex1].DevID) );
		}
/*		if ( iOpenedIndex1 == -1)
		{
			pdlg->m_ListDevOpened.SetCurSel (TotalDevOpened);
		}
*/
		TotalDevOpened ++;
	}
}

void	CloseDeviceOK ( DeviceID_t	*pDev )
{
	char		TmpStr[256];
	int			i, j, iRemoveIndex;
	bool		bFindFlag = false;

	for ( i = 0; i < TotalDevOpened; i ++ )
	{
		if ( ( DevOpened[i].DevID.m_s16DeviceMain == pDev->m_s16DeviceMain )
			&& ( DevOpened[i].DevID.m_s16DeviceSub == pDev->m_s16DeviceSub )
			&& ( DevOpened[i].DevID.m_s8ModuleID == pDev->m_s8ModuleID )
			&& ( DevOpened[i].DevID.m_s16ChannelID == pDev->m_s16ChannelID ) )
		{
			bFindFlag = true;
			iRemoveIndex = i;

			if ( i < TotalDevOpened -1 )
			{
				for ( j = i; j < TotalDevOpened-1 ; j ++ )
				{
					DevOpened[j] = DevOpened[j+1];
				}
				break;
			}
		}
	}

	if ( !bFindFlag )
	{
		sprintf ( TmpStr, "Fail to Remove from list in CloseDevice()! (%s)", GetString_DeviceAll(pDev) );
		AddMsg ( MSG_TYPE_OTHER, TmpStr );
	}
	else
	{
		TotalDevOpened --;

		if ( iOpenedIndex1 == iRemoveIndex)
		{
			iOpenedIndex1 = -1;
			pdlg->GetDlgItem ( IDC_EDIT_DEVICE1 )->SetWindowText ( "" );
		}
		
		/*if ( iOpenedIndex3 == iRemoveIndex)
		{
			iOpenedIndex3 = -1;
			pdlg->GetDlgItem ( IDC_EDIT_DEVICE3 )->SetWindowText ( "" );
		}*/
	}

	// redraw list
	pdlg->m_ListDevOpened.ResetContent ();
	for ( i = 0; i < TotalDevOpened; i ++ )
	{
		sprintf ( TmpStr, "%d-(%s) (%d,%d)", i, GetString_DeviceAll(&DevOpened[i].DevID),
			DevOpened[i].DevID.m_CallID.m_s32FlowType, DevOpened[i].DevID.m_CallID.m_s32FlowChannel );
		pdlg->m_ListDevOpened.AddString ( TmpStr );
	}

}

// -------------------------------------------------------
DJ_Void EvtHandler(DJ_U32 esrParam)
{
	Acs_Evt_t *			    pAcsEvt = NULL;

	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_GeneralProc_Data* pGeneral=NULL;

	char		TmpStr[256];
	char *      pStr = NULL;

	pAcsEvt = (Acs_Evt_t *) esrParam;

	DispEventInfo ( pAcsEvt );

	switch ( pAcsEvt->m_s32EventType )
	{
		case XMS_EVT_OPEN_STREAM:
			printf("");
			break;

		case XMS_EVT_QUERY_DEVICE:
			if ( !bStartWorkFlag )
			{
				pAcsDevList = ( Acs_Dev_List_Head_t *) ( (DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t) );

				//Add or modify device resource to pool when receive the device's event of the state changing
				AddDeviceRes ( pAcsDevList );
			}
			break; 

		case XMS_EVT_QUERY_DEVICE_END:	//get device list completely
			if ( !bStartWorkFlag )
			{
				pGeneral = (Acs_GeneralProc_Data*)FetchEventData(pAcsEvt);
				sprintf ( TmpStr, "Start work. DeviceNum = %d\n", TotalDevRes );
				AddMsg ( MSG_TYPE_OTHER, TmpStr );

				InitDeviceInfo();

				bStartWorkFlag = true;
			}
			break;

		case XMS_EVT_OPEN_DEVICE:
			OpenDeviceOK ( &pAcsEvt->m_DeviceID );						
			break;

		case XMS_EVT_CLOSE_DEVICE:
			CloseDeviceOK ( &pAcsEvt->m_DeviceID );

			pStr = GetString_DeviceAll(&pAcsEvt->m_DeviceID);
			sprintf(TmpStr,"Close devcie evt: %s",pStr);
			AfxMessageBox(TmpStr,NULL,MB_OK);
			break;

		case XMS_EVT_PLAYCSPREQ:
			{
				DJ_S32  r = 0;
				char MsgStr[100]={0};
				CSPPlayDataInfo_t     playCSP={0};
				Acs_CSPDataReq_Data * pCspReq = NULL;
				
				if(g_cspPlay == 0)
				{
					break;
				}

				pCspReq = (Acs_CSPDataReq_Data *)FetchEventData(pAcsEvt);
								
				if(g_cspFp == NULL)
				{
					g_cspFp = fopen(g_szCSPFile,"rb");
				}

				if(feof(g_cspFp))
				{
					fseek(g_cspFp,SEEK_SET,0);
					TRACE("Seek to Set\n");
				}
				
				playCSP.m_u16DataLen = pCspReq->m_u16ReqCspDataLen;
				playCSP.m_u8DataType = XMS_CSPPLAY_DATA_VOC;
				playCSP.m_u8TaskID = (DJ_U8)(GetTickCount() % 128);	
				
				long rr = fread(g_szDataBuf,sizeof(char),playCSP.m_u16DataLen,g_cspFp);
				TRACE("ReqDataLen = %d,sentData=%d\n",pCspReq->m_u16ReqCspDataLen,rr);
				
				r = XMS_ctsSendCSPData(g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &playCSP,g_szDataBuf,NULL);
				if ( r < 0 )
				{
					sprintf ( MsgStr, "X(%d) XMS_ctsPlayCSP() FAIL! (%s)", 
						r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
					AddMsg ( MSG_TYPE_FUNCTION, MsgStr);
				}
				else
				{
					sprintf ( MsgStr, "XMS_ctsPlayCSP() OK! (%s)", 
						GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
					AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
				}									
			}
			break;
		case XMS_EVT_CONTROLPLAY:
			g_cspPlay = 0;
			break;
		case XMS_EVT_RECORDCSP:
			{
				Acs_MediaCSPProc_Data * pCSPData = NULL;
								
				pCSPData = (Acs_MediaCSPProc_Data *)FetchEventData(pAcsEvt);				
				fwrite(pCSPData->m_u8StreamData,sizeof(char),pCSPData->m_u16DataLen,g_cspFp);				
			}
			break;
		default:
			break;
	}

}
