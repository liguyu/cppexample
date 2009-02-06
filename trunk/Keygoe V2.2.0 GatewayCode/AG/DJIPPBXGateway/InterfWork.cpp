#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "GatewayWork.h"
#include "StringManage.h"

void ProcInterfaceWork(INTERFACE_STRUCT* pOneInterface, Acs_Evt_t* pAcsEvt)
{
	_ASSERT(pOneInterface != NULL);
	_ASSERT(pAcsEvt != NULL);

	switch(pOneInterface->deviceID.m_s16DeviceSub)
	{
		case XMS_DEVSUB_ANALOG_TRUNK:
			ProcATrunkWork(pOneInterface, pAcsEvt);
			break;

		case XMS_DEVSUB_ANALOG_USER:
			ProcUserWork(pOneInterface, pAcsEvt);
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
						int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
						int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "%s DTRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
								theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
								pOneTrunk->deviceID.m_s8ModuleID, 
								pOneTrunk->deviceID.m_s16ChannelID, 
								pOneVOIP->VocDevID.m_s8ModuleID, 
								pOneVOIP->VocDevID.m_s16ChannelID, 
								iResult1, iResult2);

							g_pLogInfo->WriteLogInfo(chTemp256);
						}
					}

					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
					pOneVOIP->iState = VOIP_HANGUP;

					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}
				}
			}

			FreeInterfDev(pOneTrunk);
			pOneTrunk->iState = TRK_FREE;
			
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d DTRUNK%d) %s", 
					pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CLEARCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
		break;

	case XMS_EVT_CALLIN:
		{
			if(pOneTrunk->iState == TRK_FREE)
			{
				Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
				DJ_S8 *pCallerNum = pCallControl->m_s8CallingNum;
				DJ_S8 *pCalledNum = pCallControl->m_s8CalledNum;

				DeviceID_t FreeVoipDev;
				if( SearchOneFreeVOIP(FreeVoipDev) )
				{
					VOIP_STRUCT *pOneVOIP = 
						&g_AllDeviceRes[FreeVoipDev.m_s8ModuleID].pVOIP[FreeVoipDev.m_s16ChannelID];

					if(CalloutPSTNToVOIP(pCallerNum, pCalledNum, &FreeVoipDev, pOneTrunk) < 0)
					{
						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
						pOneTrunk->iState = TRK_HANGUP;
						
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d DTRUNK%d) %s (DSP%d VOIP%d) %s", 
								pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CalloutFail"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}
						break;
					}
					else
					{
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d DTRUNK%d) %s (DSP%d VOIP%d) %s", 
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
					pOneVOIP->iCallDre = CALLDRE_CALLOUT;
					pOneTrunk->iState = TRK_WAIT_ANSERCALL;
				}
				else
				{
					XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
					pOneTrunk->iState = TRK_HANGUP;
					
					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d DTRUNK%d) %s %s", 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLIN"), theApp.m_clsLIni.GLoadString("NoVoipCallout"));
						g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
					}
					break;
				}
			}
			else
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d DTRUNK%d) %s %s (%s)", 
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
					sprintf(chTemp256, "(DSP%d DTRUNK%d) %s", 
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
					sprintf(chTemp256, "(DSP%d DTRUNK%d) %s %s (%s)", 
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
						sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d DTRUNK%d) %s %s", 
							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLOUT"), 
							theApp.m_clsLIni.GLoadString("CalloutConnect"));
						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
				else		// 呼出失败
				{
					int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
					int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);
					memset(&pOneTrunk->LinkDevID, 0, sizeof(DeviceID_t));
					pOneTrunk->iState = TRK_FREE;
					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "%s DTRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
							theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
							pOneTrunk->deviceID.m_s8ModuleID, 
							pOneTrunk->deviceID.m_s16ChannelID, 
							pOneVOIP->VocDevID.m_s8ModuleID, 
							pOneVOIP->VocDevID.m_s16ChannelID, 
							iResult1, iResult2);

						g_pLogInfo->WriteLogInfo(chTemp256);
					}

					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
					pOneVOIP->iState = VOIP_HANGUP;
					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}
					
					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d DTRUNK%d) %s %s EvtErrCode%d CallClearCause%d", 
							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLOUT"), 
							theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
							pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
			}
			else
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d DTRUNK%d) %s %s (%s)", 
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
					if( pOneTrunk->DtmfCount < sizeof(pOneTrunk->DtmfBuf) - 1 )
					{
						pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount++] = chTempDtmf;
						pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount] = 0;
					}

					// 20080512 DTMF透传
					if(pOneTrunk->LinkDevID.m_s16DeviceMain > 0)
					{
						VOIP_STRUCT *pOneVOIP = 
							&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
						DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_RTPDTMF;
						char chIoDataBuf[2] = {chTempDtmf, 0};
						RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->deviceID, 
							u16IoType, strlen(chIoDataBuf), chIoDataBuf);
					}

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d DTRUNK%d) %s (%c)", 
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
								int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
								int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);

								if(g_cfgAppIsLog)
								{
									sprintf(chTemp256, "%s DTRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
										theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
										pOneTrunk->deviceID.m_s8ModuleID, 
										pOneTrunk->deviceID.m_s16ChannelID, 
										pOneVOIP->VocDevID.m_s8ModuleID, 
										pOneVOIP->VocDevID.m_s16ChannelID, 
										iResult1, iResult2);

									g_pLogInfo->WriteLogInfo(chTemp256);
								}
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
	char chTemp256[256];
	_ASSERT(pCallerNum != NULL);
	_ASSERT(pCalledNum != NULL);
	_ASSERT(pVoipDev != NULL);
	_ASSERT(pOneInterface != NULL);

	RetCode_t iResult = -1;
	VOIP_STRUCT *pOneVOIP = &g_AllDeviceRes[pVoipDev->m_s8ModuleID].pVOIP[pVoipDev->m_s16ChannelID];
	strncpy(pOneVOIP->CalleeNum, pCalledNum, CALL_NUMBER_MAX_LENGTH);
	strncpy(pOneVOIP->CallerNum, pCallerNum, CALL_NUMBER_MAX_LENGTH);
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
	if(g_cfgAppIsLog)
	{
		sprintf(chTemp256, "%s VOIP(%d.%d) Caller(%s) Callee(%s) IP(%s) Port(%d) Retrun(%d)", 
			theApp.m_clsLIni.GLoadString("MakeCallOut"), 
			pVoipDev->m_s8ModuleID, 
			pVoipDev->m_s16ChannelID, 
			pOneVOIP->CallerNumNow, pOneVOIP->CalleeNumNow, voipCallParam.m_s8CalleeAddress, 
			voipCallParam.m_s32CalleePort, iResult);

		g_pLogInfo->WriteLogInfo(chTemp256);
	}
	if(iResult > 0)
	{
		pOneInterface->LinkDevID = *pVoipDev;
		pOneVOIP->LinkDevID = pOneInterface->deviceID;

		MixerControlParam_t VocCmdP;
		memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
		VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
		VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
		int iResult1 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER/*VOIP_PARAM_MIXER*/, 
			sizeof(VocCmdP), &VocCmdP);
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s VOIP(%d.%d)  VOIP_PARAM_RTPMIXER XMS_MIXER_FROM_INPUT VOC(%d.%d) Ret(%d)", 
				theApp.m_clsLIni.GLoadString("SetParam"), 
				pOneVOIP->deviceID.m_s8ModuleID, 
				pOneVOIP->deviceID.m_s16ChannelID, 
				pOneVOIP->VocDevID.m_s8ModuleID, 
				pOneVOIP->VocDevID.m_s16ChannelID, 
				iResult1);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}

		CmdParamData_Voice_t VocMixParam;
		memset(&VocMixParam, 0, sizeof(CmdParamData_Voice_t));
		VocMixParam.m_u8OutputCtrlValid = 1;
		VocMixParam.m_VocOutputControl.m_u8OutputType = XMS_VOC_OUTPUT_FROM_MIXER;
		VocMixParam.m_VocOutputControl.m_u16FixGain = 1024;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_IP;
		VocMixParam.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID2 = pOneVOIP->deviceID.m_s16ChannelID;
		int iResult2 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->VocDevID, VOC_PARAM_UNIPARAM, 
			sizeof(VocMixParam), &VocMixParam);
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s VOC(%d.%d)  VOC_PARAM_UNIPARAM XMS_MIXER_FROM_PLAY(self) XMS_MIXER_FROM_IP VOIP(%d.%d) Ret(%d)", 
				theApp.m_clsLIni.GLoadString("SetParam"), 
				pOneVOIP->VocDevID.m_s8ModuleID, 
				pOneVOIP->VocDevID.m_s16ChannelID, 
				pOneVOIP->deviceID.m_s8ModuleID, 
				pOneVOIP->deviceID.m_s16ChannelID, 
				iResult2);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}

		if( pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			// 放回铃音 在VOIP应答后进行连接的操作
		}
		else if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)
		{
			// 20080512 将ATRUNK自己的语音连接去掉 以免发生串音
			int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->VocDevID, &pOneInterface->deviceID, NULL);
			int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneInterface->VocDevID, NULL);
			int iResult3 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
			int iResult4 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d), %s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
					theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
					pOneInterface->deviceID.m_s8ModuleID, 
					pOneInterface->deviceID.m_s16ChannelID, 
					pOneInterface->VocDevID.m_s8ModuleID, 
					pOneInterface->VocDevID.m_s16ChannelID, 
					iResult1, iResult2, 
					theApp.m_clsLIni.GLoadString("DLinkDevice"), 
					pOneInterface->deviceID.m_s8ModuleID, 
					pOneInterface->deviceID.m_s16ChannelID, 
					pOneVOIP->VocDevID.m_s8ModuleID, 
					pOneVOIP->VocDevID.m_s16ChannelID, 
					iResult3, iResult4);

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
		else
		{
			int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
			int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s DTRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
					theApp.m_clsLIni.GLoadString("DLinkDevice"), 
					pOneInterface->deviceID.m_s8ModuleID, 
					pOneInterface->deviceID.m_s16ChannelID, 
					pOneVOIP->VocDevID.m_s8ModuleID, 
					pOneVOIP->VocDevID.m_s16ChannelID, 
					iResult1, iResult2);

				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
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

// VD&VM Flag
/********************************************************************************************/
void ProcATrunkWork(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt)
{
	char chTemp256[256] = {0};

	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_CLEARCALL:	/*拆线事件*/
		{
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
						int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
						int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
								theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
								pOneTrunk->deviceID.m_s8ModuleID, 
								pOneTrunk->deviceID.m_s16ChannelID, 
								pOneVOIP->VocDevID.m_s8ModuleID, 
								pOneVOIP->VocDevID.m_s16ChannelID, 
								iResult1, iResult2);

							g_pLogInfo->WriteLogInfo(chTemp256);
						}
					}

					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
					pOneVOIP->iState = VOIP_HANGUP;

					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}
				}
			}

			FreeInterfDev(pOneTrunk);
			pOneTrunk->iState = ATRK_FREE;
			
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d ATRUNK%d) %s", 
					pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CLEARCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
		break;

	case XMS_EVT_RECVIODATA:	/*收到数据事件*/
		{
			Acs_IO_Data *pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

			if( pIOData->m_u16IoType == XMS_IO_TYPE_GTG ) 
			{
				if( pIOData->m_u16IoDataLen > 0 )
				{
					char *p = (char *)FetchIOData(pAcsEvt);

					if( *p == 'I' || *p == 'J' || *p == 'K' )	// 忙音
					{
						XMS_ctsResetDevice(g_acsHandle, &pOneTrunk->deviceID, NULL);
						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d ATRUNK%d) %s", 
								pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("RcvBusyTone"));
							g_pLogInfo->WriteLogInfo(chTemp256);
						}

						break;
					}
					// 20080506 底层修改
					else if( *p == 'h'/* || *p == 'b'*/ )			// 连通提示音
					{
						if( pOneTrunk->iState == ATRK_CALLOUT && pOneTrunk->iIsMakeCallOut > 0 )	// 呼出成功
						{
							XMS_ctsSetDevTimer(g_acsHandle, &pOneTrunk->deviceID, 0);

							VOIP_STRUCT *pOneVOIP = 
								&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
							pOneTrunk->iState = ATRK_CONNECT;
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
								sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d ATRUNK%d) %s %s", 
									pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
									pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
									theApp.m_clsLIni.GLoadString("CALLOUT"), 
									theApp.m_clsLIni.GLoadString("CalloutConnect"));
								g_pLogInfo->WriteLogInfo(chTemp256);
							}
						}
						break;
					}
					else if( *p == 'G' )						// GTG拨号提示音
					{
						if( pOneTrunk->iState == ATRK_CALLOUT && pOneTrunk->iIsMakeCallOut == 1 )
						{
							DJ_U16 u16IoType = XMS_IO_TYPE_DTMF;
							RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneTrunk->VocDevID, 
								u16IoType, lstrlen(pOneTrunk->CalleeNum), pOneTrunk->CalleeNum);
							pOneTrunk->iIsMakeCallOut += 1;	// 避免重复发号

							if(g_cfgAppIsLog)
							{
								sprintf(chTemp256, "(DSP%d ATRUNK%d) %s %s", 
									pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
									theApp.m_clsLIni.GLoadString("SndCallee"), 
									pOneTrunk->CalleeNum);
								g_pLogInfo->WriteLogInfo(chTemp256);
							}
						}
						break;
					}
				}
			}
			else if( pIOData->m_u16IoType == XMS_IO_TYPE_DTMF )
			{
				if( pOneTrunk->iState == ATRK_CONNECT )
				{
					char chTempDtmf = GetIntfDtmfCode(pAcsEvt);				//DTMF按键
					if( chTempDtmf != -1 )
					{
						if( pOneTrunk->DtmfCount < sizeof(pOneTrunk->DtmfBuf) - 1 )
						{
							pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount++] = chTempDtmf;
							pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount] = 0;
						}

						// 20080512 DTMF透传
						if(pOneTrunk->LinkDevID.m_s16DeviceMain > 0)
						{
							VOIP_STRUCT *pOneVOIP = 
								&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
							DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_RTPDTMF;
							char chIoDataBuf[2] = {chTempDtmf, 0};
							RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->deviceID, 
								u16IoType, strlen(chIoDataBuf), chIoDataBuf);
						}

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d ATRUNK%d) %s (%c)", 
								pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("GetDtmfCode"), chTempDtmf);
							g_pLogInfo->WriteLogInfo(chTemp256);
						}
					}
				}
				// 20080506 底层修改
				else if( pOneTrunk->iState == ATRK_FREE )	// 呼入收主叫
				{
					int iDataLen = pIOData->m_u16IoDataLen;
					if(iDataLen > 0)	// DTMF主叫一次性收齐
					{
						char *p = (char *)FetchIOData(pAcsEvt);
						if(iDataLen >= CALL_NUMBER_MAX_LENGTH)
						{
							memcpy(pOneTrunk->CallerNum, p, CALL_NUMBER_MAX_LENGTH-1);
							pOneTrunk->CallerNum[CALL_NUMBER_MAX_LENGTH-1] = 0;
						}
						else
						{
							memcpy(pOneTrunk->CallerNum, p, iDataLen);
							pOneTrunk->CallerNum[iDataLen] = 0;
						}

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d ATRUNK%d) %s %s", 
								pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("RcvDTMFCaller"), 
								pOneTrunk->CalleeNum);
							g_pLogInfo->WriteLogInfo(chTemp256);
						}

						// 收完主叫以后 若是已经经过了CALLIN事件 则开始用VOIP呼出
						if( lstrlen(pOneTrunk->CalleeNum) > 0 )
						{
							ATrunkCallInProc(pOneTrunk);
						}
					}
				}
			}
			else if( pIOData->m_u16IoType == XMS_IO_TYPE_FSK )
			{
				// 20080506 底层修改
				if( pOneTrunk->iState == ATRK_FREE )		// 呼入收主叫
				{
					int iDataLen = pIOData->m_u16IoDataLen;
					if(iDataLen > 10)
					{
						char *p = (char *)FetchIOData(pAcsEvt);
						if(p[0] == 4)
						{
							int iCallerNumLen = p[1] - 8;
							if(iCallerNumLen > 0 && iDataLen >= 10+iCallerNumLen)
							{
								if(iCallerNumLen >= CALL_NUMBER_MAX_LENGTH)
								{
									memcpy(pOneTrunk->CallerNum, &p[10], CALL_NUMBER_MAX_LENGTH-1);
									pOneTrunk->CallerNum[CALL_NUMBER_MAX_LENGTH-1] = 0;
								}
								else
								{
									memcpy(pOneTrunk->CallerNum, &p[10], iCallerNumLen);
									pOneTrunk->CallerNum[iCallerNumLen] = 0;
								}

								if(g_cfgAppIsLog)
								{
									sprintf(chTemp256, "(DSP%d ATRUNK%d) %s %s", 
										pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
										theApp.m_clsLIni.GLoadString("RcvFSKCaller"), 
										pOneTrunk->CalleeNum);
									g_pLogInfo->WriteLogInfo(chTemp256);
								}

								// 收完主叫以后 若是已经经过了CALLIN事件 则开始用VOIP呼出
								if( lstrlen(pOneTrunk->CalleeNum) > 0 )
								{
									ATrunkCallInProc(pOneTrunk);
								}
							}
						}
					}
				}
			}
			// 20080625 V2.2.0
			else if( pIOData->m_u16IoType == XMS_IO_TYPE_PVD )
			{
				if( pIOData->m_u16IoDataLen > 0 )
				{
					char *p = (char *)FetchIOData(pAcsEvt);
					if( *p == 'b' )			// 连通提示音
					{
						if( pOneTrunk->iState == ATRK_CALLOUT && pOneTrunk->iIsMakeCallOut > 0 )	// 呼出成功
						{
							XMS_ctsSetDevTimer(g_acsHandle, &pOneTrunk->deviceID, 0);

							VOIP_STRUCT *pOneVOIP = 
								&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
							pOneTrunk->iState = ATRK_CONNECT;
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
								sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d ATRUNK%d) %s %s", 
									pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
									pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
									theApp.m_clsLIni.GLoadString("CALLOUT"), 
									theApp.m_clsLIni.GLoadString("CalloutConnect"));
								g_pLogInfo->WriteLogInfo(chTemp256);
							}
						}
						break;
					}
				}
			}
		}
		break;

	case XMS_EVT_DEV_TIMER:
		{
			if( pOneTrunk->iState == ATRK_CALLOUT && pOneTrunk->iIsMakeCallOut == 1 )	// 呼出失败
			{
				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d ATRUNK%d) %s %s %s", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
						theApp.m_clsLIni.GLoadString("TimerIsOver"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}

				memset(&pOneTrunk->LinkDevID, 0, sizeof(DeviceID_t));
				FreeInterfDev(pOneTrunk);
				pOneTrunk->iState = ATRK_FREE;

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
		break;

	case XMS_EVT_CALLIN:	/*呼入事件*/
		{
			if(pOneTrunk->iState != ATRK_FREE)
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d ATRUNK%d) %s %s (%s)", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLIN"), 
						theApp.m_clsLIni.GLoadString("PSTNStateError"), 
						DTRKSTATE_STRING[pOneTrunk->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}

				break;
			}

			// 20080506 ATRUNK底层修改后的变更
			lstrcpy(pOneTrunk->CalleeNum, pOneTrunk->SelfCallNum);
			if( lstrlen(pOneTrunk->CallerNum) > 0 )	// 已接收主叫 开始用VOIP呼出
			{
				ATrunkCallInProc(pOneTrunk);
			}
			else	// 未接收主叫 等到接收到主叫以后再用VOIP呼出
			{
				NULL;
			}

			/*
			Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
			DJ_S8 *pCallerNum = pCallControl->m_s8CallingNum;
			DJ_S8 *pCalledNum = pCallControl->m_s8CalledNum;
			if( strlen(pCalledNum) == 0 )	// 没有传送被叫号码 则将被叫号码设置为本线路号码
			{
				pCalledNum = pOneTrunk->SelfCallNum;
			}

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
					pOneTrunk->iState = ATRK_HANGUP;
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

				pOneVOIP->iCallDre = CALLDRE_CALLOUT;
				pOneTrunk->iState = ATRK_WAIT_ANSERCALL;
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
				pOneTrunk->iState = ATRK_HANGUP;
				break;
			}*/
		}
		break;

	case XMS_EVT_ANSWERCALL:
		{
			if(pOneTrunk->iState == ATRK_WAIT_ANSERCALL)
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d ATRUNK%d) %s", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("ANSWERCALL"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
				
				pOneTrunk->iState = ATRK_CONNECT;
			}
			else
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d ATRUNK%d) %s %s (%s)", 
						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("ANSWERCALL"), 
						theApp.m_clsLIni.GLoadString("PSTNStateError"), 
						DTRKSTATE_STRING[pOneTrunk->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
			}
		}
		break;

	case XMS_EVT_SENDIODATA:
		{
			// 20080506 ATRUNK底层修改后的变更
			/*// 如果拨号音发送完成则就可以开始连接VOIP语音通道
			if(pOneTrunk->iState == ATRK_CALLOUT)
			{
				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
			}*/
		}
		break;

//	case XMS_EVT_CALLOUT:	/*呼出完成事件*/
//		{
//			if(pOneTrunk->iState == ATRK_CALLOUT)
//			{
//				VOIP_STRUCT *pOneVOIP = 
//					&g_AllDeviceRes[pOneTrunk->LinkDevID.m_s8ModuleID].pVOIP[pOneTrunk->LinkDevID.m_s16ChannelID];
//				Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
//
//				/* 20080506 将VOIP方面的操作统一放入VOIP处理函数
//				// 20080422 在呼叫连通前VOIP语音与ATRUNK语音不互联 给VOIP放回铃音 停止回铃音
//				PlayTone(&pOneVOIP->VocDevID, PLAYTONE_STOP);	// Play Stop
//				MixerControlParam_t VocCmdP;
//				memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
//				VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
//				VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
//				int iResult1 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
//					sizeof(VocCmdP), &VocCmdP);*/
//
//				if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
//				{
//					pOneTrunk->iState = ATRK_CONNECT;
//					pOneVOIP->iState = VOIP_WAIT_ANSWERCALL;
//					if( g_pShowInfo != NULL )
//					{
//						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
//							pOneVOIP->deviceID.m_s16ChannelID);
//					}
//
//					XMS_ctsAlertCall(g_acsHandle, &pOneVOIP->deviceID, NULL);
//					XMS_ctsAnswerCallIn(g_acsHandle, &pOneVOIP->deviceID, NULL);
//
//					if(g_cfgAppIsLog)
//					{
//						sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d PSTN%d) %s %s", 
//							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
//							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
//							theApp.m_clsLIni.GLoadString("CALLOUT"), 
//							theApp.m_clsLIni.GLoadString("CalloutConnect"));
//						g_pLogInfo->WriteLogInfo(chTemp256);
//					}
//				}
//				else		// 呼出失败
//				{
//					if(g_cfgAppIsLog)
//					{
//						sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d PSTN%d) %s %s EvtErrCode%d CallClearCause%d", 
//							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
//							pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
//							theApp.m_clsLIni.GLoadString("CALLOUT"), 
//							theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
//							pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
//						g_pLogInfo->WriteLogInfo(chTemp256);
//					}
//
//					memset(&pOneTrunk->LinkDevID, 0, sizeof(DeviceID_t));
//					FreeInterfDev(pOneTrunk);
//					pOneTrunk->iState = ATRK_FREE;
//
//					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
//					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
//					pOneVOIP->iState = VOIP_HANGUP;
//					if( g_pShowInfo != NULL )
//					{
//						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
//							pOneVOIP->deviceID.m_s16ChannelID);
//					}
//				}
//			}
//			else
//			{
//				if(g_cfgAppIsLog)
//				{
//					sprintf(chTemp256, "(DSP%d PSTN%d) %s %s (%s)", 
//						pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
//						theApp.m_clsLIni.GLoadString("CALLOUT"), theApp.m_clsLIni.GLoadString("PSTNStateError"), 
//						DTRKSTATE_STRING[pOneTrunk->iState]);
//					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
//				}
//			}
//		}
//		break;

	// 20080506 ATRUNK底层修改后 MakeCallOut后处理
	case XMS_EVT_DEVICESTATE:	/*当MakeCallOut后 会上报状态改变为DCS_CONNECT事件*/
		{
			if(pOneTrunk->iLineState == DCS_CONNECT 
				&& pOneTrunk->iIsMakeCallOut == 1 
				&& pOneTrunk->iState == ATRK_CALLOUT )
			{
				// 为MakeCallOut以后 此处不做处理 在收到GTG拨号音时再发送被叫号码...
			}
		}
		break;

	default:
		{
			if(pOneTrunk->iState == ATRK_CONNECT)
			{
				if(pOneTrunk->iLineState == DCS_FREE)
				{
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
								int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneTrunk->deviceID, &pOneVOIP->VocDevID, NULL);
								int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneTrunk->deviceID, NULL);

								if(g_cfgAppIsLog)
								{
									sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
										theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
										pOneTrunk->deviceID.m_s8ModuleID, 
										pOneTrunk->deviceID.m_s16ChannelID, 
										pOneVOIP->VocDevID.m_s8ModuleID, 
										pOneVOIP->VocDevID.m_s16ChannelID, 
										iResult1, iResult2);

									g_pLogInfo->WriteLogInfo(chTemp256);
								}
							}

							memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
							pOneVOIP->iState = VOIP_HANGUP;

							if( g_pShowInfo != NULL )
							{
								g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
									pOneVOIP->deviceID.m_s16ChannelID);
							}
						}
					}

					FreeInterfDev(pOneTrunk);
					pOneTrunk->iState = ATRK_FREE;
				}
			}
		}
		break;
	}
}

int PlayTone(DeviceID_t	*pVocDevID, int iPlayType)
{
	DJ_U16           u16IoLen = 0;
	DJ_U16           u16IoType = 0;
	char		 	 IoDataBuf[MAX_SEND_IODATA_DTMF_LEN] = {0};
	
	_ASSERT(pVocDevID != NULL);

	switch(iPlayType)
	{
	case PLAYTONE_STOP:
		{
			u16IoLen = 0;
			u16IoType = XMS_IO_TYPE_GTG;
		}
		break;

	case PLAYTONE_DAIL:
		{
			u16IoType = XMS_IO_TYPE_GTG;
			u16IoLen = 1;
			IoDataBuf[0] = 'G';
		}
		break;

	case PLAYTONE_RINGBACK:
		{
			u16IoType = XMS_IO_TYPE_GTG;
			u16IoLen = 1;
			IoDataBuf[0] = 'H';
		}
		break;

	case PLAYTONE_BUSY:
		{
			u16IoType = XMS_IO_TYPE_GTG;
			u16IoLen = 1;
			IoDataBuf[0] = 'I';
		}
		break;

	default:
		{
			return 0;
		}
		break;
	}

	return XMS_ctsSendIOData(g_acsHandle, pVocDevID, u16IoType, u16IoLen, IoDataBuf);
}

void ProcUserWork(INTERFACE_STRUCT* pOneUser, Acs_Evt_t* pAcsEvt)
{
	char chTemp256[256] = {0};
	
	char TmpStr[100];
	sprintf( TmpStr, "坐席事件处理===ProcUserWork()", pAcsEvt->m_s32EvtSize );
	g_pShowInfo->ShowInfo(TmpStr);

	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_CLEARCALL:	/*拆线事件*/
		{
			if(pOneUser->LinkDevID.m_s16DeviceMain > 0)
			{
				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneUser->LinkDevID.m_s8ModuleID].pVOIP[pOneUser->LinkDevID.m_s16ChannelID];
				if(pOneVOIP->iLineState != DCS_FREE)
				{
					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);

					// free the link dev
					if (pOneVOIP->VocDevID.m_s8ModuleID > 0)
					{
						int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneUser->deviceID, &pOneVOIP->VocDevID, NULL);
						int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneUser->deviceID, NULL);

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
								theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
								pOneUser->deviceID.m_s8ModuleID, 
								pOneUser->deviceID.m_s16ChannelID, 
								pOneVOIP->VocDevID.m_s8ModuleID, 
								pOneVOIP->VocDevID.m_s16ChannelID, 
								iResult1, iResult2);

							g_pLogInfo->WriteLogInfo(chTemp256);
						}
					}

					memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
					pOneVOIP->iState = VOIP_HANGUP;

					if( g_pShowInfo != NULL )
					{
						g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
							pOneVOIP->deviceID.m_s16ChannelID);
					}
				}
			}

			FreeInterfDev(pOneUser);
			pOneUser->iState = USER_FREE;
			
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d USER%d) %s", 
					pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CLEARCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
				g_pShowInfo->ShowInfo(chTemp256);
			}
		}
		break;

	case XMS_EVT_CALLIN:	/*用户通道摘机事件*/// 20080506 USER底层修改后 无此事件
		{
			
			if(pOneUser->iState != USER_FREE)
			{
				break;
			}
			//双向连接
			int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneUser->deviceID, &pOneUser->VocDevID, NULL);
			int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneUser->VocDevID, &pOneUser->deviceID, NULL);
			
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
					theApp.m_clsLIni.GLoadString("DLinkDevice"), 
					pOneUser->deviceID.m_s8ModuleID, 
					pOneUser->deviceID.m_s16ChannelID, 
					pOneUser->VocDevID.m_s8ModuleID, 
					pOneUser->VocDevID.m_s16ChannelID, 
					iResult1, iResult2);

				g_pLogInfo->WriteLogInfo(chTemp256);
			}

			pOneUser->iState = USER_OFFHOOK;
			pOneUser->dwTime = 0;
			XMS_ctsSetDevTimer(g_acsHandle, &pOneUser->deviceID, 30000);// 每次用户摘机都将设置30秒定时器

			PlayTone(&pOneUser->VocDevID, PLAYTONE_DAIL);		// dial tone
		}
		break;

	case XMS_EVT_DEV_TIMER:
		{
			// 20080506 USER底层修改后 MakeCallOut后用户未摘机超时处理
			if(pOneUser->iState == USER_CALLOUT && pOneUser->iIsMakeCallOut == 1)
			{
				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneUser->LinkDevID.m_s8ModuleID].pVOIP[pOneUser->LinkDevID.m_s16ChannelID];

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d USER%d) %s %s %s", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
						theApp.m_clsLIni.GLoadString("TimerIsOver"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}

				XMS_ctsClearCall(g_acsHandle, &pOneUser->deviceID, 0, NULL);
				pOneUser->iState = USER_FREE;
				memset(&pOneUser->LinkDevID, 0, sizeof(DeviceID_t));

				pOneVOIP->iState = VOIP_HANGUP;
				XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
				memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));

				break;
			}

			if(pOneUser->iState != USER_OFFHOOK)
			{
				break;
			}

			if( pOneUser->DtmfCount >= g_cfgUserCalledMinLen )	// 收号完成
			{
				char pCalledNum[CALL_NUMBER_MAX_LENGTH] = {0};
				strncpy(pCalledNum, pOneUser->DtmfBuf, sizeof(pCalledNum)-1);
				pOneUser->DtmfCount = 0;
				memset(pOneUser->DtmfBuf, 0, sizeof(pOneUser->DtmfBuf));

				DeviceID_t FreeVoipDev;
				if( SearchOneFreeVOIP(FreeVoipDev) )
				{
					VOIP_STRUCT *pOneVOIP = 
						&g_AllDeviceRes[FreeVoipDev.m_s8ModuleID].pVOIP[FreeVoipDev.m_s16ChannelID];

					if(CalloutPSTNToVOIP(pOneUser->SelfCallNum, pCalledNum, &FreeVoipDev, pOneUser) < 0)
					{
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d USER%d) %s (DSP%d VOIP%d) %s", 
								pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CalloutFail"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}

						PlayTone(&pOneUser->VocDevID, PLAYTONE_BUSY);		// busy tone
						pOneUser->iState = USER_WAITHANGUP;
					}
					else
					{
						PlayTone(&pOneUser->VocDevID, PLAYTONE_RINGBACK);		// Ring back tone

						pOneVOIP->iState = VOIP_CALLOUT;
						pOneVOIP->iCallDre = CALLDRE_CALLOUT;

						pOneUser->iState = USER_CONNECT;

						if( g_pShowInfo != NULL )
						{
							g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
								pOneVOIP->deviceID.m_s16ChannelID);
						}

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d USER%d) %s (DSP%d VOIP%d) %s", 
								pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"),
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("BeginCallout"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}
					}
				}
				else
				{
					PlayTone(&pOneUser->VocDevID, PLAYTONE_BUSY);		// busy tone
					pOneUser->iState = USER_WAITHANGUP;
				}
			}
			else
			{
				PlayTone(&pOneUser->VocDevID, PLAYTONE_BUSY);		// busy tone
				pOneUser->iState = USER_WAITHANGUP;
			}
		}
		break;

	case XMS_EVT_CALLOUT:	/*用户通道呼叫结果事件*/
	{
			if(pOneUser->iState != USER_CALLOUT)
			{
				break;
			}

			Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));

			VOIP_STRUCT *pOneVOIP = 
				&g_AllDeviceRes[pOneUser->LinkDevID.m_s8ModuleID].pVOIP[pOneUser->LinkDevID.m_s16ChannelID];

			/* 20080506 将VOIP方面的操作统一放入VOIP处理函数
			PlayTone(&pOneVOIP->VocDevID, PLAYTONE_STOP);	// Play Stop
			MixerControlParam_t VocCmdP;
			memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
			VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
			VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
			int iResult1 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
				sizeof(VocCmdP), &VocCmdP);*/

			if( pCallControl->m_s32AcsEvtState == 1 )	// 呼叫成功摘机
			{
				pOneUser->iState = USER_CONNECT;
				pOneVOIP->iState = VOIP_WAIT_ANSWERCALL;

				XMS_ctsAlertCall(g_acsHandle, &pOneVOIP->deviceID, NULL);
				XMS_ctsAnswerCallIn(g_acsHandle, &pOneVOIP->deviceID, NULL);

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d PSTN%d) %s %s", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutConnect"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
			}
			else		// 呼叫失败
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d PSTN%d) %s %s EvtErrCode%d CallClearCause%d", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
						pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
					g_pLogInfo->WriteLogInfo(chTemp256);
				}

				pOneUser->iState = USER_FREE;
				memset(&pOneUser->LinkDevID, 0, sizeof(DeviceID_t));

				pOneVOIP->iState = VOIP_HANGUP;
				XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
				memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
			}
		}
		break;

	// 20080506 USER底层修改后 MakeCallOut后用户摘机处理
	case XMS_EVT_DEVICESTATE:	/*当MakeCallOut后 用户摘机会上报状态改变为DCS_CONNECT事件*/
		{
			if(pOneUser->iLineState == DCS_CONNECT 
				&& pOneUser->iIsMakeCallOut == 1 
				&& pOneUser->iState == USER_CALLOUT )	// 有来话状态用户摘机
			{
				XMS_ctsSetDevTimer(g_acsHandle,&pOneUser->deviceID, 0);	// 停止定时器

				VOIP_STRUCT *pOneVOIP = 
					&g_AllDeviceRes[pOneUser->LinkDevID.m_s8ModuleID].pVOIP[pOneUser->LinkDevID.m_s16ChannelID];

				pOneUser->iState = USER_CONNECT;
				pOneVOIP->iState = VOIP_WAIT_ANSWERCALL;

				XMS_ctsAlertCall(g_acsHandle, &pOneVOIP->deviceID, NULL);
				XMS_ctsAnswerCallIn(g_acsHandle, &pOneVOIP->deviceID, NULL);

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) -> (DSP%d USER%d) %s %s", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutConnect"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
			}
			else if( pOneUser->iState == USER_FREE && pOneUser->iLineState == DCS_CONNECT )	// 空闲状态 用户摘机
			{
				int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneUser->deviceID, &pOneUser->VocDevID, NULL);
				int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneUser->VocDevID, &pOneUser->deviceID, NULL);
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
						theApp.m_clsLIni.GLoadString("DLinkDevice"), 
						pOneUser->deviceID.m_s8ModuleID, 
						pOneUser->deviceID.m_s16ChannelID, 
						pOneUser->VocDevID.m_s8ModuleID, 
						pOneUser->VocDevID.m_s16ChannelID, 
						iResult1, iResult2);

					g_pLogInfo->WriteLogInfo(chTemp256);
				}

				pOneUser->iState = USER_OFFHOOK;
				pOneUser->dwTime = 0;
				XMS_ctsSetDevTimer(g_acsHandle, &pOneUser->deviceID, 30000);// 每次用户摘机都将设置30秒定时器

				PlayTone(&pOneUser->VocDevID, PLAYTONE_DAIL);		// dial tone
			}
		}
		break;

	default:
		{
			if(pOneUser->iState == USER_OFFHOOK)
			{
				char chTempDtmf = GetIntfDtmfCode(pAcsEvt);
				if ( chTempDtmf != -1 )								//DTMF按键
				{
					if( pOneUser->DtmfCount < sizeof(pOneUser->DtmfBuf) - 1 )
					{
						pOneUser->DtmfBuf[pOneUser->DtmfCount++] = chTempDtmf;
						pOneUser->DtmfBuf[pOneUser->DtmfCount] = 0;
					}

					if( pOneUser->dwTime == 0 )
					{
						PlayTone(&pOneUser->VocDevID, PLAYTONE_STOP);	// stop play tone
						pOneUser->dwTime = GetTickCount();
						break;
					}

					if( pOneUser->DtmfCount >= g_cfgUserCalledMaxLen || pOneUser->DtmfBuf[pOneUser->DtmfCount-1] == '#' )
					{
						XMS_ctsSetDevTimer(g_acsHandle,&pOneUser->deviceID, 0);	// 停止定时器

						if(pOneUser->DtmfBuf[pOneUser->DtmfCount-1] == '#')
						{
							pOneUser->DtmfBuf[pOneUser->DtmfCount-1] = 0;
						}

						char pCalledNum[CALL_NUMBER_MAX_LENGTH] = {0};
						strncpy(pCalledNum, pOneUser->DtmfBuf, sizeof(pCalledNum)-1);
						pOneUser->DtmfCount = 0;
						memset(pOneUser->DtmfBuf, 0, sizeof(pOneUser->DtmfBuf));

						DeviceID_t FreeVoipDev;
						if( SearchOneFreeVOIP(FreeVoipDev) )
						{
							VOIP_STRUCT *pOneVOIP = 
								&g_AllDeviceRes[FreeVoipDev.m_s8ModuleID].pVOIP[FreeVoipDev.m_s16ChannelID];

							if(CalloutPSTNToVOIP(pOneUser->SelfCallNum, pCalledNum, &FreeVoipDev, pOneUser) < 0)
							{
								if(g_cfgAppIsLog)
								{
									sprintf(chTemp256, "(DSP%d USER%d) %s (DSP%d VOIP%d) %s", 
										pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
										theApp.m_clsLIni.GLoadString("CALLIN"), 
										pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
										theApp.m_clsLIni.GLoadString("CalloutFail"));
									g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
								}

								PlayTone(&pOneUser->VocDevID, PLAYTONE_BUSY);		// busy tone
								pOneUser->iState = USER_WAITHANGUP;
							}
							else
							{
								PlayTone(&pOneUser->VocDevID, PLAYTONE_RINGBACK);		// Ring back tone

								pOneVOIP->iState = VOIP_CALLOUT;
								pOneVOIP->iCallDre = CALLDRE_CALLOUT;

								pOneUser->iState = USER_CONNECT;

								if( g_pShowInfo != NULL )
								{
									g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
										pOneVOIP->deviceID.m_s16ChannelID);
								}

								if(g_cfgAppIsLog)
								{
									sprintf(chTemp256, "(DSP%d USER%d) %s (DSP%d VOIP%d) %s", 
										pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
										theApp.m_clsLIni.GLoadString("CALLIN"),
										pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
										theApp.m_clsLIni.GLoadString("BeginCallout"));
									g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
								}
							}
						}
						else
						{
							PlayTone(&pOneUser->VocDevID, PLAYTONE_BUSY);		// busy tone
							pOneUser->iState = USER_WAITHANGUP;
						}
					}
				}
			}
			else if(pOneUser->iState == USER_CONNECT)
			{
				char chTempDtmf = GetIntfDtmfCode(pAcsEvt);
				if ( chTempDtmf != -1 )								//DTMF按键
				{
					if( pOneUser->DtmfCount < sizeof(pOneUser->DtmfBuf) - 1 )
					{
						pOneUser->DtmfBuf[pOneUser->DtmfCount++] = chTempDtmf;
						pOneUser->DtmfBuf[pOneUser->DtmfCount] = 0;
					}

					// 20080512 DTMF透传
					if(pOneUser->LinkDevID.m_s16DeviceMain > 0)
					{
						VOIP_STRUCT *pOneVOIP = 
							&g_AllDeviceRes[pOneUser->LinkDevID.m_s8ModuleID].pVOIP[pOneUser->LinkDevID.m_s16ChannelID];
						DJ_U16 u16IoType = XMS_IO_TYPE_VOIP_RTPDTMF;
						char chIoDataBuf[2] = {chTempDtmf, 0};
						RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->deviceID, 
							u16IoType, strlen(chIoDataBuf), chIoDataBuf);
					}

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d USER%d) %s (%c)", 
							pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("GetDtmfCode"), chTempDtmf);
						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
			}
		}
		break;
	}
}

int InitATrunk(INTERFACE_STRUCT* pOneATrunk)
{
	int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneATrunk->deviceID, &pOneATrunk->VocDevID, NULL);
	int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneATrunk->VocDevID, &pOneATrunk->deviceID, NULL);
	if(g_cfgAppIsLog)
	{
		char chTemp256[256];
		sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
			theApp.m_clsLIni.GLoadString("DLinkDevice"), 
			pOneATrunk->deviceID.m_s8ModuleID, 
			pOneATrunk->deviceID.m_s16ChannelID, 
			pOneATrunk->VocDevID.m_s8ModuleID, 
			pOneATrunk->VocDevID.m_s16ChannelID, 
			iResult1, iResult2);

		g_pLogInfo->WriteLogInfo(chTemp256);
	}

	CmdParamData_AnalogTrunk_t anatrkparam;
	anatrkparam.m_u16CallInRingCount = 1;		// 几次振铃后产生CAllIN事件
	anatrkparam.m_u16CallInRingTimeOut = 6000;	// 振铃应答超时时间 超过此时间未应答将恢复到挂机状态（DCS_FREE）

	return XMS_ctsSetParam(g_acsHandle, &pOneATrunk->deviceID, ANALOGTRUNK_PARAM_UNIPARAM, 
		sizeof(anatrkparam), (void*)&anatrkparam);;
}

int ATrunkCallInProc(INTERFACE_STRUCT* pOneTrunk)
{
	char chTemp256[256];
	DeviceID_t FreeVoipDev;

	if( SearchOneFreeVOIP(FreeVoipDev) )
	{
		VOIP_STRUCT *pOneVOIP = 
			&g_AllDeviceRes[FreeVoipDev.m_s8ModuleID].pVOIP[FreeVoipDev.m_s16ChannelID];

		if(CalloutPSTNToVOIP(pOneTrunk->CallerNum, pOneTrunk->CalleeNum, &FreeVoipDev, pOneTrunk) < 0)
		{
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d ATRUNK%d) %s (DSP%d VOIP%d) %s", 
					pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CALLIN"), 
					pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CalloutFail"));
				g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
			}

			XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
			pOneTrunk->iState = ATRK_HANGUP;
		}
		else
		{
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d ATRUNK%d) %s (DSP%d VOIP%d) %s", 
					pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CALLIN"),
					pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("BeginCallout"));
				g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
			}

			pOneVOIP->iState = VOIP_CALLOUT;
			if( g_pShowInfo != NULL )
			{
				g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID);
			}

			pOneVOIP->iCallDre = CALLDRE_CALLOUT;
			pOneTrunk->iState = ATRK_WAIT_ANSERCALL;
		}
	}
	else
	{
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "(DSP%d ATRUNK%d) %s %s", 
				pOneTrunk->deviceID.m_s8ModuleID, pOneTrunk->deviceID.m_s16ChannelID, 
				theApp.m_clsLIni.GLoadString("CALLIN"), theApp.m_clsLIni.GLoadString("NoVoipCallout"));
			g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
		}

		XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
		pOneTrunk->iState = ATRK_HANGUP;
	}

	return 0;
}

