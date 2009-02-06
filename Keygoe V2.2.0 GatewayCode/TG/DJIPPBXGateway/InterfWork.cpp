#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "GatewayWork.h"

void ProcInterfaceWork(INTERFACE_STRUCT* pOneInterface, Acs_Evt_t* pAcsEvt)
{
	_ASSERT(pOneInterface != NULL);
	_ASSERT(pAcsEvt != NULL);

	switch(pOneInterface->deviceID.m_s16DeviceSub)
	{
		case XMS_DEVSUB_ANALOG_TRUNK:
			// ProcATrunkWork(pOneInterface, pAcsEvt);
			break;

		case XMS_DEVSUB_ANALOG_USER:
			// ProcUserWork(pOneInterface, pAcsEvt);
			break;

		case XMS_DEVSUB_ANALOG_EMPTY:
			break;

		default:
			ProcDTrunkWork(pOneInterface, pAcsEvt);
			break;
	}
}

void ProcDTrunkWork(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt)
{
	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pAcsEvt != NULL);

	char chTemp256[256] = {0};

	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_CLEARCALL:		/*拆线事件*/
		{
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d PSTN%d) %s", 
					pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CLEARCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}

			if(pOneTrunk->LinkDevID.m_s16DeviceMain > 0)
			{
				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];

				if(pOneVOIP->iLineState != DCS_FREE)
				{
					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);

					// free the link dev
					if (pOneVOIP->VocDevID.m_s8ModuleID > 0)
					{
						XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
						XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);
					}

					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
					pOneVOIP->iState = VOIP_HANGUP;
				}

				if( g_pShowInfo != NULL )
				{
					g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
						pOneVOIP->deviceID.m_s16ChannelID);
				}
			}

			FreeInterfDev(pOneTrunk);
			pOneTrunk->iState = TRK_FREE;
		}
		break;

	case XMS_EVT_CALLIN:
		{
			if(pOneTrunk->iState == TRK_FREE)
			{
				Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
				DJ_S8 *pCallerNum = pCallControl->m_s8CallingNum;
				DJ_S8 *pCalledNum = pCallControl->m_s8CalledNum;

				// 20080723 首先给PSTN端回地址全消息 以确保对端挂机本端可收到拆线信号
				XMS_ctsAlertCall(g_acsHandle, &pOneTrunk->deviceID, NULL);

				DeviceID_t FreeVoipDev;
				if( SearchOneFreeVOIP(FreeVoipDev) )
				{
					VOIP_STRUCT *pOneVOIP = 
						&g_AllDeviceRes[FreeVoipDev.m_s8ModuleID].pVOIP[FreeVoipDev.m_s16ChannelID];

					if(CalloutPSTNToVOIP(pCallerNum, pCalledNum, &FreeVoipDev, pOneTrunk) < 0)
					{
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d PSTN%d) %s (DSP%d VOIP%d) %s", 
								pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CalloutFail"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}

						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
						pOneTrunk->iState = TRK_HANGUP;
						break;
					}
					else
					{
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d PSTN%d) %s (DSP%d VOIP%d) %s", 
								pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"),
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("BeginCallout"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}
					}

					pOneVOIP->iState = VOIP_CALLOUT;
					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}

					//strncpy(pOneTrunk->CalleeNum, pCalledNum, sizeof(pOneTrunk->CalleeNum));
					//strncpy(pOneTrunk->CallerNum, pCallerNum, sizeof(pOneTrunk->CallerNum));
					pOneVOIP->iCallDre = CALLDRE_CALLIN;
					pOneTrunk->iState = TRK_WAIT_ANSERCALL;
				}
				else
				{
					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d PSTN%d) %s %s", 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLIN"), theApp.m_clsLIni.GLoadString("NoVoipCallout"));
						g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
					}

					XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
					pOneTrunk->iState = TRK_HANGUP;
					break;
				}
			}
			else
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d PSTN%d) %s %s (%s)", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLIN"), 
						theApp.m_clsLIni.GLoadString("PSTNStateError"), 
						DTRKSTATE_STRING[pOneTrunk->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
			}
		}
		break;

	case XMS_EVT_ANSWERCALL:
		{
			if(pOneTrunk->iState == TRK_WAIT_ANSERCALL)
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d PSTN%d) %s", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("ANSWERCALL"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
				pOneTrunk->iState = TRK_CONNECT;
			}
			else
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d PSTN%d) %s %s (%s)", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("ANSWERCALL"), 
						theApp.m_clsLIni.GLoadString("PSTNStateError"), 
						DTRKSTATE_STRING[pOneTrunk->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
			}
		}
		break;

	case XMS_EVT_CALLOUT:	/*呼出连通结果 如果SS7 如果ISDN失败则是XMS_EVT_CLEARCALL*/
		{
			if(pOneTrunk->iState == TRK_CALLOUT)
			{
				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
				Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
				if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
				{
					pOneTrunk->iState = TRK_CONNECT;
					pOneVOIP->iState = VOIP_WAIT_ANSWERCALL;
					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}

					XMS_ctsAlertCall(g_acsHandle, &pOneVOIP->deviceID, NULL);
					XMS_ctsAnswerCallIn(g_acsHandle, &pOneVOIP->deviceID, NULL);

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d PSTN%d) %s %s", 
							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLOUT"), 
							theApp.m_clsLIni.GLoadString("CalloutConnect"));
						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
				else		// 呼出失败
				{
					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d PSTN%d) %s %s EvtErrCode%d CallClearCause%d", 
							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLOUT"), 
							theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
							pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
						g_pLogInfo->WriteLogInfo(chTemp256);
					}

					XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
					XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);
					memset(&pOneTrunk->LinkDevID, 0, sizeof(DeviceID_t));
					pOneTrunk->iState = TRK_FREE;

					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
					pOneVOIP->iState = VOIP_HANGUP;
					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}
				}
			}
			else
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d PSTN%d) %s %s (%s)", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), theApp.m_clsLIni.GLoadString("PSTNStateError"), 
						DTRKSTATE_STRING[pOneTrunk->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
			}
		}
		break;

	default:
		{
			if(pOneTrunk->iState == TRK_CONNECT)
			{
				char chTempDtmf = GetIntfDtmfCode(pAcsEvt);				//DTMF按键
				if( chTempDtmf != -1 )
				{
					// 20080715 Mantis0001190
					if(pOneTrunk->LinkDevID.m_s16DeviceMain > 0)
					{
						VOIP_STRUCT *pOneVOIP = 
							&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
						if(g_cfgVOIPDTMFType == VDT_RTPDTMF)
						{
							DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_RTPDTMF;
							Acs_VoIP_RTPDTMF rtpDtmf;
							memset(&rtpDtmf, 0, sizeof(Acs_VoIP_RTPDTMF));
							rtpDtmf.m_s8EventChar	= chTempDtmf;		// rtp dtmf event char
							rtpDtmf.m_s8Volume		= -32;				// volume (in dbm0 from -63 to 0)
							rtpDtmf.m_u16Duration	= 200;				// duration (in timestamp units

							RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->deviceID, 
								u16IoType, sizeof(rtpDtmf), &rtpDtmf);
						}
						else if(g_cfgVOIPDTMFType == VDT_SIPINFO)
						{
							DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_SIPINFO;
							Acs_VoIP_SIPINFO sipInfo;
							memset(&sipInfo, 0, sizeof(Acs_VoIP_SIPINFO));
							strcpy(sipInfo.m_s8ContentType, "application/dtmf");
							sprintf(sipInfo.m_s8Body, "dtmf=%c\nduration=%d", chTempDtmf, 200);

							RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->deviceID, 
								u16IoType, sizeof(sipInfo), &sipInfo);
						}
						else if(g_cfgVOIPDTMFType == VDT_H245UII)
						{
							DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_H245UII;
							Acs_VoIP_H245UII h245Uii;
							memset(&h245Uii, 0, sizeof(Acs_VoIP_H245UII));
							h245Uii.m_s8Signal = chTempDtmf;	// dtmf char in "0123456789#*ABCD!"
							h245Uii.m_u32Duration = 200;		// milliseconds

							RetCode_t s32Result = XMS_ctsSendIOData (g_acsHandle, &pOneVOIP->deviceID, 
								u16IoType, sizeof(h245Uii), &h245Uii);
						}
						else
						{
							DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_RTPDTMF;
							Acs_VoIP_RTPDTMF rtpDtmf;
							memset(&rtpDtmf, 0, sizeof(Acs_VoIP_RTPDTMF));
							rtpDtmf.m_s8EventChar	= chTempDtmf;		// rtp dtmf event char
							rtpDtmf.m_s8Volume		= -32;				// volume (in dbm0 from -63 to 0)
							rtpDtmf.m_u16Duration	= 200;				// duration (in timestamp units

							RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->deviceID, 
								u16IoType, sizeof(rtpDtmf), &rtpDtmf);
						}
					}

					if( pOneTrunk->DtmfCount < sizeof(pOneTrunk->DtmfCount) - 1 )
					{
						pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount++] = chTempDtmf;
						pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount] = 0;
					}

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d PSTN%d) %s (%c)", 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("GetDtmfCode"), chTempDtmf);
						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}

				if(pOneTrunk->iLineState == DCS_FREE)
				{
					if(pOneTrunk->LinkDevID.m_s16DeviceMain > 0)
					{
						VOIP_STRUCT *pOneVOIP = 
							&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
						if(pOneVOIP->iLineState != DCS_FREE)
						{
							XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);

							if (pOneVOIP->VocDevID.m_s8ModuleID > 0)
							{
								XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
								XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);
							}

							memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
							pOneVOIP->iState = VOIP_HANGUP;
						}

						if( g_pShowInfo != NULL )
						{
							g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
								pOneVOIP->deviceID.m_s16ChannelID);
						}
					}

					FreeInterfDev(pOneTrunk);
					pOneTrunk->iState = TRK_FREE;
				}
			}
		}
		break;
	}

	return;
}

int SearchOneFreeVOIP(DeviceID_t &FreeVOIPDeviceID)
{
	if( g_cfgVOIPCallOutRuleType == VCT_DEFAULT )
	{
		for(int i = 1; i < MAX_DSP_SUM; ++i)
		{
			if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
			{
				int j = g_AllDeviceRes[i].lVOIPNum;

				for(int k = 0; k < j; ++k)
				{
					if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_FREE && g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
					{
						memcpy(&FreeVOIPDeviceID, &g_AllDeviceRes[i].pVOIP[k].deviceID, sizeof(DeviceID_t));
						return 1;
					}
				}
			}
		}
	}
	else if( g_cfgVOIPCallOutRuleType == VCT_CIRCULATION )
	{
		static int iPreValue = 1;
		static int kPreValue = -1;

		int i = iPreValue;	// 初始DSP
		if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)	// 后续通道
		{
			int j = g_AllDeviceRes[i].lVOIPNum;

			for(int k = kPreValue+1; k < j; ++k)
			{
				if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_FREE && g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
				{
					memcpy(&FreeVOIPDeviceID, &g_AllDeviceRes[i].pVOIP[k].deviceID, sizeof(DeviceID_t));
					iPreValue = i;
					kPreValue = k;
					return 1;
				}
			}
		}

		for(i = iPreValue+1; i < MAX_DSP_SUM; ++i)	// 后续DSP
		{
			if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
			{
				int j = g_AllDeviceRes[i].lVOIPNum;

				for(int k = 0; k < j; ++k)
				{
					if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_FREE && g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
					{
						memcpy(&FreeVOIPDeviceID, &g_AllDeviceRes[i].pVOIP[k].deviceID, sizeof(DeviceID_t));
						iPreValue = i;
						kPreValue = k;
						return 1;
					}
				}
			}
		}

		for(i = 1; i < iPreValue; ++i)	// 前DSP
		{
			if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
			{
				int j = g_AllDeviceRes[i].lVOIPNum;

				for(int k = 0; k < j; ++k)
				{
					if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_FREE && g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
					{
						memcpy(&FreeVOIPDeviceID, &g_AllDeviceRes[i].pVOIP[k].deviceID, sizeof(DeviceID_t));
						iPreValue = i;
						kPreValue = k;
						return 1;
					}
				}
			}
		}

		i = iPreValue;	// 初始DSP
		if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)	// 前通道
		{
			int j = g_AllDeviceRes[i].lVOIPNum;

			for(int k = 0; k <= kPreValue; ++k)
			{
				if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_FREE && g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
				{
					memcpy(&FreeVOIPDeviceID, &g_AllDeviceRes[i].pVOIP[k].deviceID, sizeof(DeviceID_t));
					iPreValue = i;
					kPreValue = k;
					return 1;
				}
			}
		}
	}
	else
	{
		for(int i = 1; i < MAX_DSP_SUM; ++i)
		{
			if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
			{
				int j = g_AllDeviceRes[i].lVOIPNum;

				for(int k = 0; k < j; ++k)
				{
					if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_FREE && g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
					{
						memcpy(&FreeVOIPDeviceID, &g_AllDeviceRes[i].pVOIP[k].deviceID, sizeof(DeviceID_t));
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

void ChangeCallerNumEx(VOIP_STRUCT *pOneVOIP)
{
	BOOL bIsModify = FALSE;
	char chTemp[CALL_NUMBER_MAX_LENGTH] = {0};
	CString strPreStr, strTemp;
	strPreStr.Empty();
	int iPos1 = 0, iPos2 = 0, iPos3 = 0, iStrLen = 0;
	BOOL bIsRouting = FALSE;
	for(int iRuleIndex = 1; iRuleIndex <= g_cfgCCRuleStringEx.iRuleSum; ++iRuleIndex)
	{
		if(g_cfgCCRuleStringEx.pStrRuleString[iRuleIndex].Find('+') != -1)
		{
			AfxExtractSubString(strPreStr, g_cfgCCRuleStringEx.pStrRuleString[iRuleIndex], 0, '+');
			AfxExtractSubString(strTemp, g_cfgCCRuleStringEx.pStrRuleString[iRuleIndex], 1, '+');
		}
		else
		{
			strTemp = g_cfgCCRuleStringEx.pStrRuleString[iRuleIndex];
		}

		bIsModify = TRUE;
		memset(chTemp, 0, sizeof(chTemp));
		iStrLen = strTemp.GetLength();
		for(iPos1 = 0, iPos2 = 0, iPos3 = 0; iPos1 < iStrLen; ++iPos1)
		{
			if(strTemp[iPos1] >= '0' && strTemp[iPos1] <= '9')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsRouting = FALSE;
					break;
				}

				if(pOneVOIP->CallerNum[iPos2] == strTemp[iPos1])
				{
					chTemp[iPos3++] = pOneVOIP->CallerNum[iPos2++];
					continue;
				}
				else
				{
					bIsRouting = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == 'X')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsModify = FALSE;
					break;
				}

				if( pOneVOIP->CallerNum[iPos2] >= '0' && pOneVOIP->CallerNum[iPos2] <= '9' )
				{
					chTemp[iPos3++] = pOneVOIP->CallerNum[iPos2++];
					continue;
				}
				else
				{
					bIsModify = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == 'Z')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsModify = FALSE;
					break;
				}

				if( pOneVOIP->CallerNum[iPos2] >= '1' && pOneVOIP->CallerNum[iPos2] <= '9' )
				{
					chTemp[iPos3++] = pOneVOIP->CallerNum[iPos2++];
					continue;
				}
				else
				{
					bIsModify = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == 'N')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsModify = FALSE;
					break;
				}

				if( pOneVOIP->CallerNum[iPos2] >= '2' && pOneVOIP->CallerNum[iPos2] <= '9' )
				{
					chTemp[iPos3++] = pOneVOIP->CallerNum[iPos2++];
					continue;
				}
				else
				{
					bIsModify = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == '[')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsModify = FALSE;
					break;
				}

				int iLen = 0;
				while( strTemp[iPos1+(++iLen)] != ']' && (iPos1+iLen) < strTemp.GetLength() );
				if(strTemp[iPos1+iLen] == ']')
				{
					BOOL bIsOK = FALSE;
					for(int iPos =1; iPos < iLen; ++iPos)
					{
						if(strTemp[iPos1+iPos+1] == '-')
						{
							if( pOneVOIP->CallerNum[iPos2] >= strTemp[iPos1+iPos] 
								&& pOneVOIP->CallerNum[iPos2] <= strTemp[iPos1+iPos+2] )
							{
								chTemp[iPos3++] = pOneVOIP->CallerNum[iPos2++];
								bIsOK = TRUE;
								break;
							}
							else
							{
								iPos += 2;
								continue;
							}
						}
						else if(strTemp[iPos1+iPos] == pOneVOIP->CallerNum[iPos2])
						{
							chTemp[iPos3++] = pOneVOIP->CallerNum[iPos2++];
							bIsOK = TRUE;
							break;
						}
						else
						{
							continue;
						}
					}

					if(!bIsOK)
					{
						bIsModify = FALSE;
						break;
					}
					else
					{
						iPos1 += iLen;
						continue;
					}
				}
				else
				{
					bIsModify = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == '|')
			{
				iPos3 = 0;
				continue;
			}
			else if(strTemp[iPos1] == '.')
			{
				if( iPos3 + strlen(&pOneVOIP->CallerNum[iPos2]) > CALL_NUMBER_MAX_LENGTH )
				{
					bIsModify = FALSE;
					break;
				}

				strncpy(&chTemp[iPos3], &pOneVOIP->CallerNum[iPos2], CALL_NUMBER_MAX_LENGTH - iPos2);
				break;
			}
			else
			{
				bIsModify = FALSE;
				break;
			}
		}

		if(bIsModify)
		{
			char chCallerTemp[256] = {0};
			strcpy(chCallerTemp, strPreStr);
			strcat(chCallerTemp, chTemp);
			strncpy(pOneVOIP->CallerNumNow, chCallerTemp, CALL_NUMBER_MAX_LENGTH);
			break;
		}
	}

	if(!bIsModify)
	{
		strncpy(pOneVOIP->CallerNumNow, pOneVOIP->CallerNum, CALL_NUMBER_MAX_LENGTH);
	}
}

int GetVOIPRoutingAndChangeCalleeNum(VOIP_STRUCT *pOneVOIP)
{
	int iRoutingNum = USE_ROUTING_NORMAL;
	char chTemp[CALL_NUMBER_MAX_LENGTH] = {0};
	CString strPreStr, strTemp;
	strPreStr.Empty();
	int iPos1 = 0, iPos2 = 0, iPos3 = 0, iStrLen = 0;
	BOOL bIsRouting = FALSE;
	for(int iRoutingIndex = 1; iRoutingIndex <= g_cfgVOIPRouting.iRoutingSum; ++iRoutingIndex)
	{
		if(g_cfgVOIPRouting.pStrRoutingString[iRoutingIndex].Find('+') != -1)
		{
			AfxExtractSubString(strPreStr, g_cfgVOIPRouting.pStrRoutingString[iRoutingIndex], 0, '+');
			AfxExtractSubString(strTemp, g_cfgVOIPRouting.pStrRoutingString[iRoutingIndex], 1, '+');
		}
		else
		{
			strTemp = g_cfgVOIPRouting.pStrRoutingString[iRoutingIndex];
		}

		bIsRouting = TRUE;
		memset(chTemp, 0, sizeof(chTemp));
		iStrLen = strTemp.GetLength();
		for(iPos1 = 0, iPos2 = 0, iPos3 = 0; iPos1 < iStrLen; ++iPos1)
		{
			if(strTemp[iPos1] >= '0' && strTemp[iPos1] <= '9')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsRouting = FALSE;
					break;
				}

				if(pOneVOIP->CalleeNum[iPos2] == strTemp[iPos1])
				{
					chTemp[iPos3++] = pOneVOIP->CalleeNum[iPos2++];
					continue;
				}
				else
				{
					bIsRouting = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == 'X')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsRouting = FALSE;
					break;
				}

				if( pOneVOIP->CalleeNum[iPos2] >= '0' && pOneVOIP->CalleeNum[iPos2] <= '9' )
				{
					chTemp[iPos3++] = pOneVOIP->CalleeNum[iPos2++];
					continue;
				}
				else
				{
					bIsRouting = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == 'Z')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsRouting = FALSE;
					break;
				}

				if( pOneVOIP->CalleeNum[iPos2] >= '1' && pOneVOIP->CalleeNum[iPos2] <= '9' )
				{
					chTemp[iPos3++] = pOneVOIP->CalleeNum[iPos2++];
					continue;
				}
				else
				{
					bIsRouting = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == 'N')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsRouting = FALSE;
					break;
				}

				if( pOneVOIP->CalleeNum[iPos2] >= '2' && pOneVOIP->CalleeNum[iPos2] <= '9' )
				{
					chTemp[iPos3++] = pOneVOIP->CalleeNum[iPos2++];
					continue;
				}
				else
				{
					bIsRouting = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == '[')
			{
				if(iPos2 >= CALL_NUMBER_MAX_LENGTH)
				{
					bIsRouting = FALSE;
					break;
				}

				int iLen = 0;
				while( strTemp[iPos1+(++iLen)] != ']' && (iPos1+iLen) < strTemp.GetLength() );
				if(strTemp[iPos1+iLen] == ']')
				{
					BOOL bIsOK = FALSE;
					for(int iPos =1; iPos < iLen; ++iPos)
					{
						if(strTemp[iPos1+iPos+1] == '-')
						{
							if( pOneVOIP->CalleeNum[iPos2] >= strTemp[iPos1+iPos] 
								&& pOneVOIP->CalleeNum[iPos2] <= strTemp[iPos1+iPos+2] )
							{
								chTemp[iPos3++] = pOneVOIP->CalleeNum[iPos2++];
								bIsOK = TRUE;
								break;
							}
							else
							{
								iPos += 2;
								continue;
							}
						}
						else if(strTemp[iPos1+iPos] == pOneVOIP->CalleeNum[iPos2])
						{
							chTemp[iPos3++] = pOneVOIP->CalleeNum[iPos2++];
							bIsOK = TRUE;
							break;
						}
						else
						{
							continue;
						}
					}

					if(!bIsOK)
					{
						bIsRouting = FALSE;
						break;
					}
					else
					{
						iPos1 += iLen;
						continue;
					}
				}
				else
				{
					bIsRouting = FALSE;
					break;
				}
			}
			else if(strTemp[iPos1] == '|')
			{
				iPos3 = 0;
				continue;
			}
			else if(strTemp[iPos1] == '.')
			{
				if( iPos3 + strlen(&pOneVOIP->CalleeNum[iPos2]) > CALL_NUMBER_MAX_LENGTH )
				{
					bIsRouting = FALSE;
					break;
				}

				strncpy(&chTemp[iPos3], &pOneVOIP->CalleeNum[iPos2], CALL_NUMBER_MAX_LENGTH - iPos2);
				break;
			}
			else
			{
				bIsRouting = FALSE;
				break;
			}
		}

		if(bIsRouting)
		{
			iRoutingNum = iRoutingIndex;
			char chCalleeTemp[256] = {0};
			strcpy(chCalleeTemp, strPreStr);
			strcat(chCalleeTemp, chTemp);
			strncpy(pOneVOIP->CalleeNumNow, chCalleeTemp, CALL_NUMBER_MAX_LENGTH);
			break;
		}
	}

	if( iRoutingNum == USE_ROUTING_NORMAL )
	{
		strncpy(pOneVOIP->CalleeNumNow, pOneVOIP->CalleeNum, CALL_NUMBER_MAX_LENGTH);
		iRoutingNum = 0;	// 默认VOIP路由
	}

	return iRoutingNum;
}

int  CalloutPSTNToVOIP(char* pCallerNum, char* pCalledNum,  DeviceID_t* pVoipDev, INTERFACE_STRUCT* pOneInterface)
{
	_ASSERT(pCallerNum != NULL);
	_ASSERT(pCalledNum != NULL);
	_ASSERT(pVoipDev != NULL);
	_ASSERT(pOneInterface != NULL);

	RetCode_t iResult = -1;
	VOIP_STRUCT *pOneVOIP = &g_AllDeviceRes[pVoipDev->m_s8ModuleID].pVOIP[pVoipDev->m_s16ChannelID];
	strncpy(pOneVOIP->CalleeNum, pCalledNum, CALL_NUMBER_MAX_LENGTH);
	strncpy(pOneVOIP->CallerNum, pCallerNum, CALL_NUMBER_MAX_LENGTH);
	//strncpy(pOneVOIP->CallerNumNow, pCallerNum, CALL_NUMBER_MAX_LENGTH);
	ChangeCallerNumEx(pOneVOIP);									// VOIP主叫转换
	int iRoutingIndex = GetVOIPRoutingAndChangeCalleeNum(pOneVOIP);	// VOIP路由以及VOIP被叫转换

	VoIPCallPrivate_t voipCallParam = {0};
	voipCallParam.m_PrivateData.m_u32DataSize = sizeof(VoIPCallPrivate_t);
	if(iRoutingIndex > 0)
	{
		voipCallParam.m_s32Protocol = g_cfgVOIPRouting.pAddress[iRoutingIndex].m_ushProtocol;
		strncpy(voipCallParam.m_s8CalleeAddress, g_cfgVOIPRouting.pAddress[iRoutingIndex].m_chIPAddr, 
			sizeof(voipCallParam.m_s8CalleeAddress));
		voipCallParam.m_s32CalleePort = g_cfgVOIPRouting.pAddress[iRoutingIndex].m_uiPort;
	}
	else
	{
		voipCallParam.m_s32Protocol = g_cfgPBXADDRESS.m_ushProtocol;
		strncpy(voipCallParam.m_s8CalleeAddress, g_cfgPBXADDRESS.m_chIPAddr, sizeof(voipCallParam.m_s8CalleeAddress));
		voipCallParam.m_s32CalleePort = g_cfgPBXADDRESS.m_uiPort;
	}

	iResult = XMS_ctsMakeCallOut(g_acsHandle, pVoipDev, pOneVOIP->CallerNumNow, pOneVOIP->CalleeNumNow, 
		(PrivateData_t *)&voipCallParam);
	if(iResult > 0)
	{
		pOneInterface->LinkDevID = *pVoipDev;
		pOneVOIP->LinkDevID = pOneInterface->deviceID;

		int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
		int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

		MixerControlParam_t VocCmdP;
		memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
		VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
		VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
		int iResult3 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER/*VOIP_PARAM_MIXER*/, 
			sizeof(VocCmdP), &VocCmdP);

		CmdParamData_Voice_t VocMixParam;
		memset(&VocMixParam, 0, sizeof(CmdParamData_Voice_t));
		VocMixParam.m_u8OutputCtrlValid = 1;
		VocMixParam.m_VocOutputControl.m_u8OutputType = XMS_VOC_OUTPUT_FROM_MIXER;
		VocMixParam.m_VocOutputControl.m_u16FixGain = g_cfgVOIPVOCVALUE;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_IP;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID2 = pOneVOIP->deviceID.m_s16ChannelID;
		int iResult4 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->VocDevID, VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);
	}

	return iResult;
}

char GetIntfDtmfCode(Acs_Evt_t* pAcsEvt)
{
	Acs_IO_Data *pIOData = NULL;
	char		chDtmf = -1;

	if( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data事件*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if( (pIOData->m_u16IoType == XMS_IO_TYPE_DTMF) 
			&& (pIOData->m_u16IoDataLen > 0) )
		{
			chDtmf = *((char *)FetchIOData(pAcsEvt));

			return chDtmf;
		}
	}

	return chDtmf;
}

