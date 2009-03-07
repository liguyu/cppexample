#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "GatewayWork.h"

void ProcVOIPWork(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{
	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pAcsEvt != NULL);

	char chTemp256[256] = {0};

	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_CLEARCALL:		/*�����¼�*/
		{
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d VOIP%d) %s", 
					pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CLEARCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}

			if(pOneVOIP->LinkDevID.m_s16DeviceMain > 0)	// �ж��Ƿ��Ѿ�����
			{
				INTERFACE_STRUCT *pOneInterface = 
					&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID];

				RetCode_t s32Result = XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);

				if(pOneVOIP->VocDevID.m_s8ModuleID > 0)
				{
					XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->LinkDevID, &pOneVOIP->VocDevID, NULL);
					XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneVOIP->LinkDevID, NULL);
				}

				FreeInterfDev(pOneInterface);
				pOneInterface->iState = TRK_HANGUP;
			}

			FreeVOIPDev(pOneVOIP);
			pOneVOIP->iState = VOIP_FREE;
			pOneVOIP->iCallDre = CALLDRE_NOTCALL;
			if( g_pShowInfo != NULL )
			{
				g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID);
			}
		}
		break;

	case XMS_EVT_CALLIN:		/*�����¼�*/
		{
			if(pOneVOIP->iState != VOIP_FREE)
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) %s %s (%s)", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLIN"), 
						theApp.m_clsLIni.GLoadString("VOIPStateError"), 
						VOIPSTATE_STRING[pOneVOIP->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
				break;
			}

			Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			if( pCallControl->m_PrivData.m_u32DataSize != sizeof(VoIPCallPrivate_t) )
			{
				break;
			}
			VoIPCallPrivate_t *voipCallPrivate = (VoIPCallPrivate_t *)&(pCallControl->m_PrivData);

			pOneVOIP->iProtocol = voipCallPrivate->m_s32Protocol;

			//Caller Info
			strncpy(pOneVOIP->CallerAddr, voipCallPrivate->m_s8CallerAddress, sizeof(pOneVOIP->CallerAddr));			
			strncpy(pOneVOIP->CallerUserID, voipCallPrivate->m_s8CallerUserID, sizeof(pOneVOIP->CallerUserID));
			strncpy ( pOneVOIP->CallerNum, pCallControl->m_s8CallingNum, sizeof(pOneVOIP->CallerNum));
			pOneVOIP->iCallerPort = (unsigned short)voipCallPrivate->m_s32CallerPort;

			//Callee Info
			strncpy(pOneVOIP->CalleeAddr, voipCallPrivate->m_s8CalleeAddress, sizeof(pOneVOIP->CalleeAddr));			
			strncpy(pOneVOIP->CalleeUserID, voipCallPrivate->m_s8CalleeUserID, sizeof(pOneVOIP->CalleeUserID));
			strncpy(pOneVOIP->CalleeNum, pCallControl->m_s8CalledNum, sizeof(pOneVOIP->CalleeNum));			
			pOneVOIP->iCalleePort = (unsigned short)voipCallPrivate->m_s32CalleePort;

			pOneVOIP->iCallDre = CALLDRE_CALLOUT;

			// ������������������
			// if(IsValidIP(voipCallPrivate->m_s8CallerAddress))
			{
				DeviceID_t FreeInterfaceDev;

				if( SearchOneFreeIntf(pOneVOIP, FreeInterfaceDev) )
				{
					ChangeCallerNum(pOneVOIP);

					char callerNum[CALL_NUMBER_MAX_LENGTH];
					char calledNum[CALL_NUMBER_MAX_LENGTH];
					strncpy(callerNum, pOneVOIP->CallerNumNow, sizeof(callerNum));
					strncpy(calledNum, pOneVOIP->CalleeNumNow, sizeof(calledNum));
					INTERFACE_STRUCT *pOneInterface = 
						&g_AllDeviceRes[FreeInterfaceDev.m_s8ModuleID].pInterf[FreeInterfaceDev.m_s16ChannelID];

					if( CallOutVOIPToPSTN(callerNum, calledNum, &FreeInterfaceDev, pOneVOIP) )
					{
						strcpy(pOneInterface->CallerNum, pCallControl->m_s8CallingNum);
						strcpy(pOneInterface->CalleeNum, pCallControl->m_s8CalledNum);

						pOneInterface->iState = TRK_CALLOUT;

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d VOIP%d) %s (DSP%d PSTN%d) %s", 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneInterface->deviceID.m_s8ModuleID, pOneInterface->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("BeginCallout"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}
					}
					else
					{
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d VOIP%d) %s (DSP%d PSTN%d) %s", 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneInterface->deviceID.m_s8ModuleID, pOneInterface->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CalloutFail"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}

						memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
						XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
						pOneVOIP->iState = VOIP_HANGUP;
					}
				}
				else
				{
					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d VOIP%d) %s %s", 
							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("CALLIN"), 
							theApp.m_clsLIni.GLoadString("NoPstnCallout"));
						g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
					}

					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
					pOneVOIP->iState = VOIP_HANGUP;
				}
				
				if( g_pShowInfo != NULL )
				{
					g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
						pOneVOIP->deviceID.m_s16ChannelID);
				}
			}
			/*else
			{
				XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
				pOneVOIP->iState = VOIP_HANGUP;
			}*/
		}
		break;

	case XMS_EVT_ANSWERCALL:		/*����Ӧ������¼�*/
		{
			if(pOneVOIP->iState != VOIP_WAIT_ANSWERCALL)
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) %s %s (%s)", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("ANSWERCALL"), 
						theApp.m_clsLIni.GLoadString("VOIPStateError"), 
						VOIPSTATE_STRING[pOneVOIP->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
				break;
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d VOIP%d) %s", 
					pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("ANSWERCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}

			pOneVOIP->iState = VOIP_CONNECT;
			if( g_pShowInfo != NULL )
			{
				g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID);
			}
		}
		break;

	case XMS_EVT_CALLOUT:			/*��������¼�*/
		{
			if(pOneVOIP->iState != VOIP_CALLOUT)
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) %s %s (%s)", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("VOIPStateError"), 
						VOIPSTATE_STRING[pOneVOIP->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}
				break;
			}

			Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			INTERFACE_STRUCT *pOneInterface = 
				&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID];

			if(pCallControl->m_s32AcsEvtState == 1)	// �����ɹ����Է�ժ��
			{
				pOneVOIP->iState = VOIP_CONNECT;
				pOneInterface->iState = TRK_WAIT_ANSERCALL;

				/*Trunk*/
				if(pOneInterface->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_USER)
				{
					XMS_ctsAlertCall(g_acsHandle, &pOneInterface->deviceID, NULL);
					XMS_ctsAnswerCallIn(g_acsHandle, &pOneInterface->deviceID, NULL);
				}

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d PSTN%d) -> (DSP%d VOIP%d) %s %s", 
						pOneInterface->deviceID.m_s8ModuleID, pOneInterface->deviceID.m_s16ChannelID, 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutConnect"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
			}
			else		// ����ʧ��
			{
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d PSTN%d) -> (DSP%d VOIP%d) %s %s EvtErrCode%d CallClearCause%d", 
						pOneInterface->deviceID.m_s8ModuleID, pOneInterface->deviceID.m_s16ChannelID, 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
						pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
					g_pLogInfo->WriteLogInfo(chTemp256);
				}

				XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);
				XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->LinkDevID, &pOneVOIP->VocDevID, NULL);
				XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneVOIP->LinkDevID, NULL);
				memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
				pOneInterface->iState = TRK_HANGUP;

				memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
				pOneVOIP->iState = VOIP_FREE;
				pOneVOIP->iCallDre = CALLDRE_NOTCALL;
			}

			if( g_pShowInfo != NULL )
			{
				g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID);
			}
		}
		break;

	default:
		{
			if(pOneVOIP->iState == VOIP_CONNECT)
			{
				char chTempDtmf = GetVoipDtmfCode(pAcsEvt);
				if(chTempDtmf != -1 )								//DTMF����
				{
					// 20080715 Mantis0001190
					DJ_U16 u16IoType = XMS_IO_TYPE_DTMF;
					char chIoDataBuf[2] = {chTempDtmf, 0};
					RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->VocDevID, 
						u16IoType, strlen(chIoDataBuf), chIoDataBuf);

					if( pOneVOIP->DtmfCount < sizeof(pOneVOIP->DtmfBuf)-1 )
					{
						pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount++] = chTempDtmf;
						pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount] = 0;
					}

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "(DSP%d VOIP%d) %s (%c)", 
							pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
							theApp.m_clsLIni.GLoadString("GetDtmfCode"), 
							chTempDtmf);
						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
			}
		}
		break;
	}

	return;
}

void FreeVOIPDev(VOIP_STRUCT* pOneVOIP)
{
	pOneVOIP->DtmfCount = 0;
	memset(pOneVOIP->DtmfBuf, 0, sizeof(pOneVOIP->DtmfBuf));

	pOneVOIP->iCalleePort = 0;
	pOneVOIP->iCallerPort = 0;
	pOneVOIP->iProtocol = 0;

	memset(pOneVOIP->CallerAddr, 0, sizeof(pOneVOIP->CallerAddr));
	memset(pOneVOIP->CallerUserID, 0, sizeof(pOneVOIP->CallerUserID));
	
	memset(pOneVOIP->CalleeAddr, 0, sizeof(pOneVOIP->CalleeAddr));
	memset(pOneVOIP->CalleeUserID, 0, sizeof(pOneVOIP->CalleeUserID));

	memset(pOneVOIP->CallerNum, 0, sizeof(pOneVOIP->CallerNum));
	memset(pOneVOIP->CalleeNum, 0, sizeof(pOneVOIP->CalleeNum));

	memset(pOneVOIP->CallerNumNow, 0, sizeof(pOneVOIP->CallerNumNow));
	memset(pOneVOIP->CalleeNumNow, 0, sizeof(pOneVOIP->CalleeNumNow));

	memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t) );

	return;
}

void FreeInterfDev(INTERFACE_STRUCT* pOneInterf)
{
	pOneInterf->DtmfCount = 0;
	memset(pOneInterf->DtmfBuf, 0, sizeof(pOneInterf->DtmfBuf));

	memset(pOneInterf->CallerNum, 0, sizeof(pOneInterf->CallerNum));
	memset(pOneInterf->CalleeNum, 0, sizeof(pOneInterf->CalleeNum));

	memset(&pOneInterf->LinkDevID, 0, sizeof(DeviceID_t) );

	return;
}

char GetVoipDtmfCode(Acs_Evt_t *pAcsEvt)
{
	Acs_VoIP_H245UII *h245Uii = NULL;
	Acs_VoIP_SIPINFO *sipInfo = NULL;
	Acs_VoIP_RTPDTMF *rtpDtmf = NULL;
	char			 chDtmf = -1;

	Acs_IO_Data *pIOEvt = (Acs_IO_Data *)FetchEventData(pAcsEvt);
	
	switch (pIOEvt->m_u16IoType)
	{
	case XMS_IO_TYPE_VOIP_H245UII:
		if(pIOEvt->m_u16IoDataLen == sizeof(Acs_VoIP_H245UII))
		{
			h245Uii = (Acs_VoIP_H245UII *)FetchIOData(pAcsEvt);

			return  chDtmf = strlen(h245Uii->m_s8String) == 0 ? h245Uii->m_s8Signal : h245Uii->m_s8String[0];
		}
		break;

	case XMS_IO_TYPE_VOIP_SIPINFO:
		if (pIOEvt->m_u16IoDataLen == sizeof(Acs_VoIP_SIPINFO))
		{
			sipInfo = (Acs_VoIP_SIPINFO *)FetchIOData(pAcsEvt);
			if(strcmp(sipInfo->m_s8ContentType, "application/dtmf") == 0)
			{
				return chDtmf = sipInfo->m_s8Body[0];
			}
		}
		break;

	case XMS_IO_TYPE_VOIP_RTPDTMF:
		if (pIOEvt->m_u16IoDataLen == sizeof(Acs_VoIP_RTPDTMF))
		{
			rtpDtmf = (Acs_VoIP_RTPDTMF *)FetchIOData(pAcsEvt);

			return chDtmf = rtpDtmf->m_s8EventChar;
		}
		break;
	}

	return chDtmf;
}

int GetRoutingNumAndChangeCalleeNum(VOIP_STRUCT *pOneVOIP)
{
	int iRoutingNum = USE_ROUTING_NORMAL;
	char chTemp[CALL_NUMBER_MAX_LENGTH] = {0};
	CString strPreStr, strTemp;
	strPreStr.Empty();
	int iPos1 = 0, iPos2 = 0, iPos3 = 0, iStrLen = 0;
	BOOL bIsRouting = FALSE;
	for(int iRoutingIndex = 1; iRoutingIndex <= g_cfgOutRoutingString.iRoutingSum; ++iRoutingIndex)
	{
		if(g_cfgOutRoutingString.pStrRoutingString[iRoutingIndex].Find('+') != -1)
		{
			AfxExtractSubString(strPreStr, g_cfgOutRoutingString.pStrRoutingString[iRoutingIndex], 0, '+');
			AfxExtractSubString(strTemp, g_cfgOutRoutingString.pStrRoutingString[iRoutingIndex], 1, '+');
		}
		else
		{
			strTemp = g_cfgOutRoutingString.pStrRoutingString[iRoutingIndex];
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
	}

	return iRoutingNum;
}

void ChangeCallerNum(VOIP_STRUCT *pOneVOIP)
{
	BOOL bIsModify = FALSE;
	char chTemp[CALL_NUMBER_MAX_LENGTH] = {0};
	CString strPreStr, strTemp;
	strPreStr.Empty();
	int iPos1 = 0, iPos2 = 0, iPos3 = 0, iStrLen = 0;
	BOOL bIsRouting = FALSE;
	for(int iRuleIndex = 1; iRuleIndex <= g_cfgCCRuleString.iRuleSum; ++iRuleIndex)
	{
		if(g_cfgCCRuleString.pStrRuleString[iRuleIndex].Find('+') != -1)
		{
			AfxExtractSubString(strPreStr, g_cfgCCRuleString.pStrRuleString[iRuleIndex], 0, '+');
			AfxExtractSubString(strTemp, g_cfgCCRuleString.pStrRuleString[iRuleIndex], 1, '+');
		}
		else
		{
			strTemp = g_cfgCCRuleString.pStrRuleString[iRuleIndex];
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

int SearchOneFreeIntf(VOIP_STRUCT *pOneVOIP, DeviceID_t &pFreeIntfDeviceID)
{
	int iRoutingNum = GetRoutingNumAndChangeCalleeNum(pOneVOIP);

	for(int i = 1; i < MAX_DSP_SUM; ++i)
	{
		if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
		{
			int j = g_AllDeviceRes[i].lInterfNum;

			for(int k = 0; k < j; ++k)
			{
				if( g_cfgiDigiDspID[i][k/32] != iRoutingNum )
				{
					k = (k/32+1)*32-1;	// �Ƶ���һ��E1
					continue;
				}
				if(g_AllDeviceRes[i].pInterf[k].iState == TRK_FREE && g_AllDeviceRes[i].pInterf[k].iLineState == DCS_FREE )
				{
					if( g_AllDeviceRes[i].pInterf[k].deviceID.m_s16ChannelID == 0 
						|| g_AllDeviceRes[i].pInterf[k].deviceID.m_s16ChannelID == 16 )
						// 0 16 ����ʱ϶���߻�·
					{
						continue;
					}
					memcpy(&pFreeIntfDeviceID, &g_AllDeviceRes[i].pInterf[k].deviceID, sizeof(DeviceID_t));
					return 1;
				}
			}
		}
	}

	return 0;
}

int CallOutVOIPToPSTN(char* callerNum, char* calledNum, DeviceID_t* pInterfaceDev, VOIP_STRUCT* pOneVOIP)
{
	RetCode_t iResult = -1;

	iResult = XMS_ctsMakeCallOut(g_acsHandle, pInterfaceDev, callerNum, calledNum,  NULL);
	if(iResult > 0)
	{
		INTERFACE_STRUCT *pOneInterface = 
			&g_AllDeviceRes[pInterfaceDev->m_s8ModuleID].pInterf[pInterfaceDev->m_s16ChannelID];
		pOneVOIP->LinkDevID = *pInterfaceDev;
		pOneInterface->LinkDevID = pOneVOIP->deviceID;

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
