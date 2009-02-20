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
	case XMS_EVT_CLEARCALL:		/*拆线事件*/
		{
			if(pOneVOIP->LinkDevID.m_s16DeviceMain > 0)	// 判断是否已经拆线
			{
				INTERFACE_STRUCT *pOneInterface = 
					&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID];

				if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)	// User
				{
					if( pOneInterface->iState == USER_CALLOUT )	// 底层线路状态不会复位 王占康反应底层问题 ...
					{
						int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
						int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

						XMS_ctsResetDevice(g_acsHandle, &pOneInterface->deviceID, NULL);	// 为了避免底层线路状态问题
						XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);
						memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
						pOneInterface->iState = USER_WAITHANGUP;

						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
								theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
								pOneInterface->deviceID.m_s8ModuleID, 
								pOneInterface->deviceID.m_s16ChannelID, 
								pOneVOIP->VocDevID.m_s8ModuleID, 
								pOneVOIP->VocDevID.m_s16ChannelID, 
								iResult1, iResult2);

							g_pLogInfo->WriteLogInfo(chTemp256);
						}
					}
					else
					{
						int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
						int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);
						int iResult3 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->VocDevID, &pOneInterface->deviceID, NULL);
						int iResult4 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneInterface->VocDevID, NULL);
						PlayTone(&pOneInterface->VocDevID, PLAYTONE_BUSY);		// busy tone
						memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
						pOneInterface->iState = USER_WAITHANGUP;
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d), %s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
								theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
								pOneInterface->deviceID.m_s8ModuleID, 
								pOneInterface->deviceID.m_s16ChannelID, 
								pOneVOIP->VocDevID.m_s8ModuleID, 
								pOneVOIP->VocDevID.m_s16ChannelID, 
								iResult1, iResult2, 
								theApp.m_clsLIni.GLoadString("DLinkDevice"), 
								pOneInterface->deviceID.m_s8ModuleID, 
								pOneInterface->deviceID.m_s16ChannelID, 
								pOneInterface->VocDevID.m_s8ModuleID, 
								pOneInterface->VocDevID.m_s16ChannelID, 
								iResult3, iResult4);

							g_pLogInfo->WriteLogInfo(chTemp256);
						}
					}
				}
				else if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)	// ATrunk
				{
					int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
					int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

					XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);
					memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
					pOneInterface->iState = ATRK_HANGUP;

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
							theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
							pOneInterface->deviceID.m_s8ModuleID, 
							pOneInterface->deviceID.m_s16ChannelID, 
							pOneVOIP->VocDevID.m_s8ModuleID, 
							pOneVOIP->VocDevID.m_s16ChannelID, 
							iResult1, iResult2);

						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
				else	// DTrunk
				{
					int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->LinkDevID, &pOneVOIP->VocDevID, NULL);
					int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneVOIP->LinkDevID, NULL);

					XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);
					memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
					pOneInterface->iState = TRK_HANGUP;

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "%s DTRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
							theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
							pOneInterface->deviceID.m_s8ModuleID, 
							pOneInterface->deviceID.m_s16ChannelID, 
							pOneVOIP->VocDevID.m_s8ModuleID, 
							pOneVOIP->VocDevID.m_s16ChannelID, 
							iResult1, iResult2);

						g_pLogInfo->WriteLogInfo(chTemp256);
					}
				}
			}

			FreeVOIPDev(pOneVOIP);
			pOneVOIP->iState = VOIP_FREE;
			pOneVOIP->iCallDre = CALLDRE_NOTCALL;
			if( g_pShowInfo != NULL )
			{
				g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID);
			}

			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d VOIP%d) %s", 
					pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("CLEARCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
		break;

	case XMS_EVT_CALLIN:		/*呼入事件*/
		{
			g_pShowInfo->ShowInfo("ProcVOIPWork()===>VOIP呼入事件。。。");

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
					g_pShowInfo->ShowInfo(chTemp256);
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

			pOneVOIP->iCallDre = CALLDRE_CALLIN;

			{
				DeviceID_t FreeInterfaceDev;
				int iUserOrAtrunk = SearchOneFreeIntfUserOrAtrunk(pOneVOIP, FreeInterfaceDev);
				if( iUserOrAtrunk == 0 )
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
				else
				{
					ChangeCallerNum(pOneVOIP);

					INTERFACE_STRUCT *pOneInterface = 
						&g_AllDeviceRes[FreeInterfaceDev.m_s8ModuleID].pInterf[FreeInterfaceDev.m_s16ChannelID];

					CString strTrunkType;
					if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)
					{
						strTrunkType = "ATRUNK";
					}
					else if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
					{
						strTrunkType = "USER";
					}

					if( CallOutVOIPToPSTN(pOneVOIP->CallerNumNow, pOneVOIP->CalleeNumNow, 
						&FreeInterfaceDev, pOneVOIP) )
					{
						if( iUserOrAtrunk == 1 )
						{
							if( pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
							{
								pOneInterface->iState = USER_CALLOUT;
							}
							else
							{
								pOneInterface->iState = ATRK_CALLOUT;
							}
						}
						else if( iUserOrAtrunk == 2 )
						{
							pOneInterface->iState = ATRK_CALLOUT;
						}
						
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d VOIP%d) %s (DSP%d %s%d) %s", 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneInterface->deviceID.m_s8ModuleID, 
								strTrunkType, pOneInterface->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("BeginCallout"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}
					}
					else
					{
						if(g_cfgAppIsLog)
						{
							sprintf(chTemp256, "(DSP%d VOIP%d) %s (DSP%d %s%d) %s", 
								pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CALLIN"), 
								pOneInterface->deviceID.m_s8ModuleID, 
								strTrunkType, pOneInterface->deviceID.m_s16ChannelID, 
								theApp.m_clsLIni.GLoadString("CalloutFail"));
							g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
						}

						memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
						XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
						pOneVOIP->iState = VOIP_HANGUP;
					}
				}

				if( g_pShowInfo != NULL )
				{
					g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
						pOneVOIP->deviceID.m_s16ChannelID);
				}
			}
		}
		break;

	case XMS_EVT_ANSWERCALL:		/*呼入应答完成事件*/
		{
			if(pOneVOIP->iState != VOIP_WAIT_ANSWERCALL)
			{
				/*if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) %s %s (%s)", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("ANSWERCALL"), 
						theApp.m_clsLIni.GLoadString("VOIPStateError"), 
						VOIPSTATE_STRING[pOneVOIP->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}*/
				break;
			}

			pOneVOIP->iState = VOIP_CONNECT;
			
			INTERFACE_STRUCT *pOneInterface = 
					&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID];

			if( pOneVOIP->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
			{
				// 20080506 将VOIP方面的操作统一放入VOIP处理函数
				PlayTone(&pOneVOIP->VocDevID, PLAYTONE_STOP);	// Play Stop
				MixerControlParam_t VocCmdP;
				memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
				VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
				VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
				int iResult = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
					sizeof(VocCmdP), &VocCmdP);
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "%s VOIP(%d.%d)  VOIP_PARAM_RTPMIXER XMS_MIXER_FROM_INPUT VOC(%d.%d) Ret(%d)", 
						theApp.m_clsLIni.GLoadString("SetParam"), 
						pOneVOIP->deviceID.m_s8ModuleID, 
						pOneVOIP->deviceID.m_s16ChannelID, 
						pOneVOIP->VocDevID.m_s8ModuleID, 
						pOneVOIP->VocDevID.m_s16ChannelID, 
						iResult);

					g_pLogInfo->WriteLogInfo(chTemp256);
				}

				// 20080512 将USER自己的语音连接去掉 以免发生串音
				int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->VocDevID, &pOneInterface->deviceID, NULL);
				int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneInterface->VocDevID, NULL);
				int iResult3 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);
				int iResult4 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d), %s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
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
			else if( pOneVOIP->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
			{
				// 20080506 将VOIP方面的操作统一放入VOIP处理函数
				PlayTone(&pOneVOIP->VocDevID, PLAYTONE_STOP);	// Play Stop
				MixerControlParam_t VocCmdP;
				memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
				VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
				VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
				int iResult = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
					sizeof(VocCmdP), &VocCmdP);
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "%s VOIP(%d.%d)  VOIP_PARAM_RTPMIXER XMS_MIXER_FROM_INPUT VOC(%d.%d) Ret(%d)", 
						theApp.m_clsLIni.GLoadString("SetParam"), 
						pOneVOIP->deviceID.m_s8ModuleID, 
						pOneVOIP->deviceID.m_s16ChannelID, 
						pOneVOIP->VocDevID.m_s8ModuleID, 
						pOneVOIP->VocDevID.m_s16ChannelID, 
						iResult);

					g_pLogInfo->WriteLogInfo(chTemp256);
				}

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
				// 数字信令线路在呼叫时已经Link
			}

			if( g_pShowInfo != NULL )
			{
				g_pShowInfo->ShowInfo(pOneVOIP->iSeqID, pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID);
			}
			
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "(DSP%d VOIP%d) %s", 
					pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
					theApp.m_clsLIni.GLoadString("ANSWERCALL"));
				g_pLogInfo->WriteLogInfo(chTemp256);
			}
		}
		break;

	case XMS_EVT_CALLOUT:			/*呼出完成事件*/
		{
			if(pOneVOIP->iState != VOIP_CALLOUT)
			{
				/*if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d VOIP%d) %s %s (%s)", 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("VOIPStateError"), 
						VOIPSTATE_STRING[pOneVOIP->iState]);
					g_pLogInfo->WriteLogInfo(chTemp256, LOGTYPE_ALTER);
				}*/
				break;
			}

			Acs_CallControl_Data *pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			INTERFACE_STRUCT *pOneInterface = 
				&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID];

			if(pCallControl->m_s32AcsEvtState == 1)	// 呼出成功，对方摘机
			{
				pOneVOIP->iState = VOIP_CONNECT;

				CString strTrunkType;
				if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)	// User
				{
					PlayTone(&pOneInterface->VocDevID, PLAYTONE_STOP);		// Stop Play Ring Back
					// 20080512 将USER自己的语音连接去掉 以免发生串音
					int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneInterface->VocDevID, NULL);
					int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->VocDevID, &pOneInterface->deviceID, NULL);
					int iResult3 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
					int iResult4 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "%s USER(%d.%d) VOC(%d.%d) Ret(%d,%d), %s USER(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
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

					strTrunkType = "USER";
				}
				else if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)	// ATrunk
				{
					XMS_ctsAlertCall(g_acsHandle, &pOneInterface->deviceID, NULL);
					XMS_ctsAnswerCallIn(g_acsHandle, &pOneInterface->deviceID, NULL);
					pOneInterface->iState = ATRK_WAIT_ANSERCALL;
					strTrunkType = "ATRUNK";
				}
				else	// DTrunk
				{
					XMS_ctsAlertCall(g_acsHandle, &pOneInterface->deviceID, NULL);
					XMS_ctsAnswerCallIn(g_acsHandle, &pOneInterface->deviceID, NULL);
					pOneInterface->iState = TRK_WAIT_ANSERCALL;
					strTrunkType = "DTRUNK";
				}

				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d %s%d) -> (DSP%d VOIP%d) %s %s", 
						pOneInterface->deviceID.m_s8ModuleID, 
						strTrunkType, pOneInterface->deviceID.m_s16ChannelID, 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutConnect"));
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
			}
			else		// 呼出失败
			{
				CString strTrunkType;
				if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)	// User
				{
					PlayTone(&pOneInterface->VocDevID, PLAYTONE_BUSY);		// busy tone
					memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
					pOneInterface->iState = USER_WAITHANGUP;
					strTrunkType = "USER";
				}
				else if(pOneInterface->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)	// ATrunk
				{
					XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);

					int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);
					int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);

					memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
					pOneInterface->iState = ATRK_HANGUP;

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
							theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
							pOneInterface->deviceID.m_s8ModuleID, 
							pOneInterface->deviceID.m_s16ChannelID, 
							pOneVOIP->VocDevID.m_s8ModuleID, 
							pOneVOIP->VocDevID.m_s16ChannelID, 
							iResult1, iResult2);

						g_pLogInfo->WriteLogInfo(chTemp256);
					}

					strTrunkType = "ATRUNK";
				}
				else	// DTrunk
				{
					XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);
					int iResult1 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->LinkDevID, &pOneVOIP->VocDevID, NULL);
					int iResult2 = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneVOIP->LinkDevID, NULL);
					memset(&pOneInterface->LinkDevID, 0, sizeof(DeviceID_t));
					pOneInterface->iState = TRK_HANGUP;

					if(g_cfgAppIsLog)
					{
						sprintf(chTemp256, "%s DTRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
							theApp.m_clsLIni.GLoadString("DUnlinkDevice"), 
							pOneInterface->deviceID.m_s8ModuleID, 
							pOneInterface->deviceID.m_s16ChannelID, 
							pOneVOIP->VocDevID.m_s8ModuleID, 
							pOneVOIP->VocDevID.m_s16ChannelID, 
							iResult1, iResult2);

						g_pLogInfo->WriteLogInfo(chTemp256);
					}

					strTrunkType = "DTRUNK";
				}

				memset(&pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t));
				pOneVOIP->iState = VOIP_FREE;
				pOneVOIP->iCallDre = CALLDRE_NOTCALL;
				
				if(g_cfgAppIsLog)
				{
					sprintf(chTemp256, "(DSP%d %s%d) -> (DSP%d VOIP%d) %s %s EvtErrCode%d CallClearCause%d", 
						pOneInterface->deviceID.m_s8ModuleID, 
						strTrunkType, pOneInterface->deviceID.m_s16ChannelID, 
						pOneVOIP->deviceID.m_s8ModuleID, pOneVOIP->deviceID.m_s16ChannelID, 
						theApp.m_clsLIni.GLoadString("CALLOUT"), 
						theApp.m_clsLIni.GLoadString("CalloutDisConnect"), 
						pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
					g_pLogInfo->WriteLogInfo(chTemp256);
				}
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
				if(chTempDtmf != -1 )								//DTMF按键
				{
					if( (chTempDtmf >= '0' && chTempDtmf <= '9') || chTempDtmf == '*' || chTempDtmf == '#' )
					{
						// 20080512 DTMF透传
						INTERFACE_STRUCT *pOneInterface = 
							&g_AllDeviceRes[pOneVOIP->LinkDevID.m_s8ModuleID].pInterf[pOneVOIP->LinkDevID.m_s16ChannelID];
						DJ_U16 u16IoType = XMS_IO_TYPE_DTMF;
						char chIoDataBuf[2] = {chTempDtmf, 0};
						RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneInterface->deviceID, 
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

	// 20080506 底层修改以后 模拟线路必须时刻有语音与其连接以便于收号
	pOneInterf->iIsMakeCallOut = 0;
	if(pOneInterf->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK 
		/*|| pOneInterf->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER*/)
	{
		int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterf->deviceID, &pOneInterf->VocDevID, NULL);
		int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterf->VocDevID, &pOneInterf->deviceID, NULL);

		if(g_cfgAppIsLog)
		{
			char chTemp256[256];
			sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
				theApp.m_clsLIni.GLoadString("DLinkDevice"), 
				pOneInterf->deviceID.m_s8ModuleID, 
				pOneInterf->deviceID.m_s16ChannelID, 
				pOneInterf->VocDevID.m_s8ModuleID, 
				pOneInterf->VocDevID.m_s16ChannelID, 
				iResult1, iResult2);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}

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

int SearchOneFreeIntfDTrunk(VOIP_STRUCT *pOneVOIP, DeviceID_t &pFreeIntfDeviceID)
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
					k = (k/32+1)*32-1;	// 移到下一个E1
					continue;
				}
				if(g_AllDeviceRes[i].pInterf[k].iState == TRK_FREE && g_AllDeviceRes[i].pInterf[k].iLineState == DCS_FREE )
				{
					if( g_AllDeviceRes[i].pInterf[k].deviceID.m_s16ChannelID == 0 
						|| g_AllDeviceRes[i].pInterf[k].deviceID.m_s16ChannelID == 16 )
						// 0 16 两个时隙不走话路
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

int SearchOneFreeIntfUserOrAtrunk(VOIP_STRUCT *pOneVOIP, DeviceID_t &pFreeIntfDeviceID)
{
	int iRoutingNum = GetRoutingNumAndChangeCalleeNum(pOneVOIP);

	int i = pOneVOIP->deviceID.m_s8ModuleID;	// 限定USER&ATRUNK和VOIP在同一个DSP
	if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
	{
		int j = g_AllDeviceRes[i].lInterfNum;

		for(int k = 0; k < j; ++k)
		{
			if( g_cfgiDigiDspID[i][k/32] != iRoutingNum )
			{
				k = (k/32+1)*32-1;	// 移到下一个E1
				continue;
			}
			if( g_AllDeviceRes[i].pInterf[k].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
			{
				if( strcmp(g_AllDeviceRes[i].pInterf[k].SelfCallNum, pOneVOIP->CalleeNumNow) == 0 )
				{
					if(g_AllDeviceRes[i].pInterf[k].iState == USER_FREE && g_AllDeviceRes[i].pInterf[k].iLineState == DCS_FREE )
					{
						memcpy(&pFreeIntfDeviceID, &g_AllDeviceRes[i].pInterf[k].deviceID, sizeof(DeviceID_t));
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
		}

		for(k = 0; k < j; ++k)
		{
			if( g_cfgiDigiDspID[i][k/32] != iRoutingNum )
			{
				k = (k/32+1)*32-1;	// 移到下一个E1
				continue;
			}
			if( g_AllDeviceRes[i].pInterf[k].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
			{
				if(g_AllDeviceRes[i].pInterf[k].iState == ATRK_FREE && g_AllDeviceRes[i].pInterf[k].iLineState == DCS_FREE )
				{
					memcpy(&pFreeIntfDeviceID, &g_AllDeviceRes[i].pInterf[k].deviceID, sizeof(DeviceID_t));
					return 2;
				}
			}
		}
	}

	return 0;
}

int CallOutVOIPToPSTN(char* callerNum, char* calledNum, DeviceID_t* pInterfaceDev, VOIP_STRUCT* pOneVOIP)
{
	RetCode_t iResult = -1;
	char chTemp256[256];
	INTERFACE_STRUCT *pOneInterface = 
		&g_AllDeviceRes[pInterfaceDev->m_s8ModuleID].pInterf[pInterfaceDev->m_s16ChannelID];

	if( pInterfaceDev->m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )// 补救底层问题 底层修改好以后可删除
	{
		// 20080422 MODIFY BY WHA FOR WZK 修改后每次呼出重新设置参数
		//int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
		//int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

		// 20080506 USER底层修改后 初始化ATRUNK
		/*CmdParamData_AnalogTrunk_t cmdAnalogTrunk;
		memset(&cmdAnalogTrunk, 0, sizeof(cmdAnalogTrunk));
		// 20080411 MODIFY BY WHA FOR WZK
		cmdAnalogTrunk.m_u8CallOutMode = 1;
		//cmdAnalogTrunk.m_u8CallerIdOption = XMS_ANALOG_TRUNK_CALLERID_FSKORDTMF;;
		//cmdAnalogTrunk.m_u8CallInRingCount = 3;
		//cmdAnalogTrunk.m_u8DialtoneDetect = XMS_ANALOG_TRUNK_DIALTONE_DETECT;
		//cmdAnalogTrunk.m_u8CallOutMethod = XMS_ANALOG_TRUNK_CALLOUT_NORMAL_PROGRESS;
		cmdAnalogTrunk.m_VocDevID = pOneInterface->VocDevID;

		DJ_U16 u16ParamType = ANALOGTRUNK_PARAM_UNIPARAM;
		DJ_U16 u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);

		RetCode_t iResult = XMS_ctsSetParam(g_acsHandle, &pOneInterface->deviceID, 
			u16ParamType, u16ParamSize, &cmdAnalogTrunk);*/
		int iResult3 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneInterface->VocDevID, NULL);
		int iResult4 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->VocDevID, &pOneInterface->deviceID, NULL);
		if(g_cfgAppIsLog)
		{
			sprintf(chTemp256, "%s ATRUNK(%d.%d) VOC(%d.%d) Ret(%d,%d)", 
				theApp.m_clsLIni.GLoadString("DLinkDevice"), 
				pOneInterface->deviceID.m_s8ModuleID, 
				pOneInterface->deviceID.m_s16ChannelID, 
				pOneInterface->VocDevID.m_s8ModuleID, 
				pOneInterface->VocDevID.m_s16ChannelID, 
				iResult3, iResult4);

			g_pLogInfo->WriteLogInfo(chTemp256);
		}
	}

	iResult = XMS_ctsMakeCallOut(g_acsHandle, pInterfaceDev, callerNum, calledNum,  NULL);
	g_pShowInfo->ShowInfo("XMS_ctsMakeCallOut()................................");

	if(g_cfgAppIsLog)
	{
		sprintf(chTemp256, "%s TRUNK(%d.%d) Caller(%s) Callee(%s) Retrun(%d)", 
			theApp.m_clsLIni.GLoadString("MakeCallOut"), 
			pInterfaceDev->m_s8ModuleID, 
			pInterfaceDev->m_s16ChannelID, 
			callerNum, calledNum, iResult);

		g_pLogInfo->WriteLogInfo(chTemp256);
		g_pShowInfo->ShowInfo(chTemp256);
	}
	if(iResult > 0 )
	{
		pOneVOIP->LinkDevID = *pInterfaceDev;
		pOneInterface->LinkDevID = pOneVOIP->deviceID;

		if( pInterfaceDev->m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			// 20080506 USER底层修改后 送主叫并设置MakeCallOut标志
// 			DJ_U16 u16IoType = XMS_IO_TYPE_DTMF;
// 			RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle, &pOneVOIP->VocDevID, 
// 				u16IoType, strlen(callerNum), callerNum);
			pOneInterface->iIsMakeCallOut = 1;
			XMS_ctsSetDevTimer(g_acsHandle, &pOneInterface->deviceID, 60000);// 设置定时器 超时则自动清楚此呼叫

			// User呼叫时为了避免线路杂音让VOIP听到 语音通道呼叫成功后连接
			MixerControlParam_t VocCmdP;
			memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
			VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
			VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
			int iResult1 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
				sizeof(VocCmdP), &VocCmdP);
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s VOIP(%d.%d)  VOIP_PARAM_RTPMIXER XMS_MIXER_FROM_PLAY VOC(%d.%d) Ret(%d)", 
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
			int iResult2 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->VocDevID, 
				VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);
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

			int iResult3 = PlayTone(&pOneVOIP->VocDevID, PLAYTONE_RINGBACK);	// Ring Back Tone
		}
		else if( pInterfaceDev->m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
		{
			// 20080506 ATRUNK底层修改后 设置MakeCallOut标志
			//暂时修改。。。
			char tempstr[10];
			sprintf(tempstr,",,,,%s",calledNum);
			DJ_U16 u16IoType = XMS_IO_TYPE_DTMF;
			RetCode_t s32Result = XMS_ctsSendIOData(g_acsHandle,&pOneInterface->VocDevID, 
				u16IoType, 10, tempstr);
			
			Sleep (10000);

			lstrcpy(pOneInterface->CalleeNum, calledNum);
			lstrcpy(pOneInterface->CallerNum, callerNum);
			pOneInterface->iIsMakeCallOut = 1;

			XMS_ctsSetDevTimer(g_acsHandle, &pOneInterface->deviceID, 65000);// 设置定时器 超时则自动清除此呼叫

			// ATrunk内部有Link 必须在CallOut发送呼叫码后连接
			MixerControlParam_t VocCmdP;
			memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
			VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
			VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
			int iResult1 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
				sizeof(VocCmdP), &VocCmdP);
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s VOIP(%d.%d)  VOIP_PARAM_RTPMIXER XMS_MIXER_FROM_PLAY VOC(%d.%d) Ret(%d)", 
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
			int iResult2 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->VocDevID, 
				VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);
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

		//	RetCode_t r = XMS_ctsSendIOData(g_acsHandle,&pOneVOIP->VocDevID,XMS_IO_TYPE_DTMF,20,"6346");

			// 20080422 在呼叫连通前VOIP语音与ATRUNK语音不互联 故需给VOIP放回铃音
//			int iResult3 = PlayTone(&pOneVOIP->VocDevID, PLAYTONE_RINGBACK);	// Play RingBack for VOIP
		}
		else
		{
			int iResult1 = XMS_ctsLinkDevice(g_acsHandle, &pOneInterface->deviceID, &pOneVOIP->VocDevID, NULL);
			int iResult2 = XMS_ctsLinkDevice(g_acsHandle, &pOneVOIP->VocDevID, &pOneInterface->deviceID, NULL);

			MixerControlParam_t VocCmdP;
			memset(&VocCmdP, 0, sizeof(MixerControlParam_t));
			VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;
			VocCmdP.m_u16SRC_ChID1 = pOneVOIP->VocDevID.m_s16ChannelID;
			int iResult3 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID, VOIP_PARAM_RTPMIXER, //VOIP_PARAM_MIXER, 
				sizeof(VocCmdP), &VocCmdP);
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s VOIP(%d.%d)  VOIP_PARAM_RTPMIXER XMS_MIXER_FROM_INPUT VOC(%d.%d) Ret(%d)", 
					theApp.m_clsLIni.GLoadString("SetParam"), 
					pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID, 
					pOneVOIP->VocDevID.m_s8ModuleID, 
					pOneVOIP->VocDevID.m_s16ChannelID, 
					iResult3);

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
			int iResult4 = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->VocDevID, 
				VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);
			if(g_cfgAppIsLog)
			{
				sprintf(chTemp256, "%s VOC(%d.%d)  VOC_PARAM_UNIPARAM XMS_MIXER_FROM_PLAY(self) XMS_MIXER_FROM_IP VOIP(%d.%d) Ret(%d)", 
					theApp.m_clsLIni.GLoadString("SetParam"), 
					pOneVOIP->VocDevID.m_s8ModuleID, 
					pOneVOIP->VocDevID.m_s16ChannelID, 
					pOneVOIP->deviceID.m_s8ModuleID, 
					pOneVOIP->deviceID.m_s16ChannelID, 
					iResult4);

				g_pLogInfo->WriteLogInfo(chTemp256);
			}

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
