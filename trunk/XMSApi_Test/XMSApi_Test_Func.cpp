#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "XMSApi_PlaySet.h"
#include "XMSApi_RecordSet.h"
#include "XMSApi_ControlPlay.h"
#include "XMSApi_ControlRecord.h"
#include "XMSApi_ConfParam.h"
#include "XMSApi_Test_ParamVoice.h"
#include "XMSApi_BuildIndex.h"
#include "XMSApi_SendIOData.h"
#include "XMSApi_ParamCAS.h"
#include "DialogAnaTrunk.h"
#include "DialogAnalogUser.h"

#include "DialogFskSet.h"
#include "DialogFlashSet.h"
#include "DialogGtdSet.h"
#include "DialogATrunkFlash.h"
#include "Dialog3GFaxTone.h"
#include "RecCSPDlg.h"
#include "CSPPlayDlg.h"

#include "DialogFaxHeader.h"

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_Sub.h"
#include "XMSApi_Test_Event.h"
#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Func.h"

#include "XMS_Test_ParamVoip.h"

#include "..\CAS_Common_Code\CAS_Common_Cfg.h"

// ----------------------------------------------------------
extern ACSHandle_t		g_acsHandle;
extern bool				bStartWorkFlag;

extern	int		iNowOpIndex;
extern	int		iOpenedIndex1;
extern	int		iOpenedIndex2;
extern	int		iOpenedIndex3;

extern	int		TotalDevRes;
extern	int		TotalDevOpened;
extern	TYPE_MY_DEVICE_STRUCT	*pDevRes;
extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern	CXMSApi_TestDlg	*pdlg;

extern int		iComboParam;
extern char		ParamTypeStr[MAX_PARAMETER_TYPE][32];
extern char		cfg_IniName[MAX_FILE_NAME_LEN];
extern FILE *   g_cspFp;
extern char     g_szCSPFile[MAX_PATH];
extern int		g_iProtocol;
extern int		g_Select;
// ----------------------------------------------------------
PlayProperty_t		g_PlayProperty;

RecordProperty_t	g_RecordProperty;

ControlPlay_t		g_ControlPlay;

ControlRecord_t		g_ControlRecord;

CmdParamData_Conf_t		g_Conf_Param;

CmdParamData_Voice_t	g_Voice_Param;

PlayProperty_t		g_BuildIndexProperty;

CmdParamData_CAS_t	g_CAS_Param;

CmdParamData_AnalogTrunk_t g_AnalogTrunk;
RecordCSPProperty_t        g_RecCSP;
CSPPlayProperty_t          g_PlayCSP;
CmdParamData_AnalogUser_t  g_AnalogUser;

MixerControlParam_t  g_VoIP_Param;

CmdParamData_FskParam_t    g_FskSet;
CmdParamData_FlashParam_t  g_FlashSet;
CmdParamData_GtdFreq_t     g_GtdFreq;
CmdParamData_GtdProtoType_t g_GtdProto;
CmdParamData_AnalogTrunkFlash_t g_ATrunkFlash;
ITP_3G_FaxToneStruct_t    g_3GFaxTone;
CmdParamData_FaxSetHeader_t g_FaxHeader;

DeviceID_t DevRecordCsp;
DeviceID_t DevPlayCsp;
// ----------------------------------------------------------
void	InitVar_Func(void)
{
	memset ( &g_PlayProperty, 0, sizeof(PlayProperty_t) );

	memset ( &g_RecordProperty, 0, sizeof(RecordProperty_t) );

	memset ( &g_ControlPlay, 0, sizeof(ControlPlay_t) );

	memset ( &g_ControlRecord, 0, sizeof(ControlRecord_t) );

	memset ( &g_Conf_Param, 0, sizeof (CmdParamData_Conf_t) );
	g_Conf_Param.m_u8InputOpt = g_Conf_Param.m_u8OutputOpt = 1;

	memset ( &g_Voice_Param, 0, sizeof (CmdParamData_Voice_t) );

	memset ( &g_BuildIndexProperty, 0, sizeof (PlayProperty_t) );

	memset(&g_AnalogTrunk,0,sizeof(g_AnalogTrunk));
	memset(&g_AnalogUser,0,sizeof(g_AnalogUser));

	// read g_CAS_Param from "c:\\DJKeygoe\\XMS_CAS_Cfg.INI"
	long	r;
	char	TmpStr[64];

	r = CAS_Common_Cfg_ReadCfg ( &g_CAS_Param );
	if ( r != 0 )
	{
		sprintf ( TmpStr, "Error in Read CAS Cfg! return = %d", r );
		AfxMessageBox ( TmpStr );
	}
}

bool	CheckOpenDevice ( int iIndex, bool	bIsDev1 )
{
	char		MsgStr[160];

	if ( iIndex == -1 )
	{
		if ( bIsDev1 )
			sprintf ( MsgStr, "Select an Opened Dev1 First!" );
		else
			sprintf ( MsgStr, "Select an Opened Dev2 First!" );

		AfxMessageBox ( MsgStr );
		AddMsg ( MSG_TYPE_OTHER, MsgStr );
		return false;
	}

	if ( (iIndex < 0) || (iIndex >= TotalDevOpened) )
	{
		sprintf ( MsgStr, "Selected Opened Device overflow. Index = %d.", iIndex );
		AfxMessageBox ( MsgStr );
		AddMsg ( MSG_TYPE_OTHER, MsgStr );
		return false;
	}

	return true;
}

bool	CheckOpenDeviceForVoIP (int iIndex )
{
	char		MsgStr[160];

	if ( iIndex == -1)
	//( DevOpened[iIndex].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOICE )
	{
		
		sprintf ( MsgStr, "Select an Opened Dev3 First!" );
		
		AfxMessageBox ( MsgStr );
		AddMsg ( MSG_TYPE_OTHER, MsgStr );
		return false;
	}

	return true;
}



// --------------------------------------------------------------------
// normal 
// --------------------------------------------------------------------
void	DoFunc_GetDeviceList (void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	r = XMS_acsGetDeviceList ( g_acsHandle, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_acsGetDeviceList() FAIL!", r );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_acsGetDeviceList() OK!" );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_OpenDevice(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( iNowOpIndex == -1 )
	{
		sprintf ( MsgStr, "Select a Device First!" );
		AfxMessageBox ( MsgStr );
		AddMsg ( MSG_TYPE_OTHER, MsgStr );
		return;
	}

	if ( (iNowOpIndex < 0) || (iNowOpIndex >= TotalDevRes) )
	{
		sprintf ( MsgStr, "Selected Device overflow. iIndex = %d.", iNowOpIndex );
		AfxMessageBox ( MsgStr );
		AddMsg ( MSG_TYPE_OTHER, MsgStr );
		return;
	}

	r = XMS_ctsOpenDevice ( g_acsHandle, &pDevRes[iNowOpIndex].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsOpenDevice() FAIL! (%s)", r, GetString_DeviceAll (&pDevRes[iNowOpIndex].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsOpenDevice() OK! (%s)", GetString_DeviceAll (&pDevRes[iNowOpIndex].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

#ifdef	DEBUG_IN_HOME_NOTEBOOK
	// for-test
	OpenDeviceOK ( &pDevRes[iNowOpIndex].DevID );
	//
#endif
}

void	DoFunc_CloseDevice(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if(g_Select == 0)
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;

		r = XMS_ctsCloseDevice ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsCloseDevice() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsCloseDevice() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		if ( !CheckOpenDeviceForVoIP (iOpenedIndex3) )
			return;

		r = XMS_ctsCloseDevice ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsCloseDevice() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsCloseDevice() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}

	/*int iRow = pdlg->m_ListDevOpened.GetCurSel ();
		
	if(DevOpened[iRow].DevID.m_s16DeviceMain == XMS_DEVMAIN_CTBUS_TS
		|| DevOpened[iRow].DevID.m_s16DeviceMain == XMS_DEVMAIN_VOIP)
	{		
		pdlg->GetDlgItem ( IDC_EDIT_DEVICE3 )->SetWindowText ("");
	}*/

	
#ifdef	DEBUG_IN_HOME_NOTEBOOK
	// for-test
//	CloseDeviceOK ( &DevOpened[iRow].DevID );
	//
#endif
}

void	DoFunc_ResetDevice(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if(g_Select == 0 )
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;
		r = XMS_ctsResetDevice ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
			if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsResetDevice() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsResetDevice() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;
		r = XMS_ctsResetDevice ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsResetDevice() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsResetDevice() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}

	//if ( !CheckOpenDevice (iOpenedIndex1) )
	//	return;

	
	
}

void	DoFunc_GetDeviceState(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	r = XMS_ctsGetDevState ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsGetDevState() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsGetDevState() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_GetDeviceStateVoIP(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex3) )
		return;

	r = XMS_ctsGetDevState ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsGetDevState() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsGetDevState() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}
// --------------------------------------------------------------------
//  end of normal 
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// call control
// --------------------------------------------------------------------
void	DoFunc_AlertCall(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if( g_Select == 0 )
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;
		r = XMS_ctsAlertCall ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
			if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsAlertCall() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsAlertCall() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;
		r = XMS_ctsAlertCall ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, NULL );
			if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsAlertCall() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsAlertCall() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	

}

void	DoFunc_AnswerCallIn(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if(g_Select == 0 )
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;

		r = XMS_ctsAnswerCallIn ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsAnswerCallIn() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsAnswerCallIn() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;

		r = XMS_ctsAnswerCallIn ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsAnswerCallIn() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsAnswerCallIn() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
}

void	DoFunc_ClearCall(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if(g_Select == 0)
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;

		r = XMS_ctsClearCall ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 0, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsClearCall() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsClearCall() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;

		r = XMS_ctsClearCall ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, 0, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsClearCall() FAIL! (%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsClearCall() OK! (%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
}

void	DoFunc_MakeCallOut(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		Str_callingNum[ACS_MAX_CALL_NUM]; 
	char		Str_calledNum[ACS_MAX_CALL_NUM];
	char		m_strCalleeAddr[ACS_MAX_CALL_NUM];
	char		m_strCalleePort[ACS_MAX_CALL_NUM];
	if ( !bStartWorkFlag )	return;



	pdlg->GetDlgItem ( IDC_EDIT_CALLINGNUM )->GetWindowText ( Str_callingNum, ACS_MAX_CALL_NUM-1 );
	pdlg->GetDlgItem ( IDC_EDIT_CALLEDNUM )->GetWindowText ( Str_calledNum, ACS_MAX_CALL_NUM-1 );
	
	pdlg->GetDlgItem ( IDC_EDIT_CALLEE_ADDR )->GetWindowText ( m_strCalleeAddr, ACS_MAX_CALL_NUM-1 );
	pdlg->GetDlgItem ( IDC_EDIT_CALLEE_PORT )->GetWindowText ( m_strCalleePort, ACS_MAX_CALL_NUM-1 );

	if( g_Select == 0)
	//if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;

		if (DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOIP )
		{
			r = XMS_ctsMakeCallOut ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
										Str_callingNum,	Str_calledNum,
										NULL );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsMakeCallOut(%s,%s) FAIL! (%s)",
					r, Str_callingNum, Str_calledNum, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsMakeCallOut(%s,%s) OK! (%s)",
					Str_callingNum, Str_calledNum, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );

			if(DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH 
			&& DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  == XMS_DEVSUB_ANALOG_USER)
			{
				char szDev2[128];
				pdlg->GetDlgItemText(IDC_EDIT_DEVICE2,szDev2,sizeof(szDev2));

				//if ( !CheckOpenDevice (iOpenedIndex2) )
				if( strlen(szDev2) == 0 )
				{
					sprintf ( MsgStr, "XMS_ctsMakeCallOut SendCallerSetTimer(%s) Failed! Voc is NULL",
					Str_callingNum );
					AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
					return;
				}
				else
				{
					XMS_ctsSetDevTimer(g_acsHandle,&DevOpened[iOpenedIndex2].DevID,2000);
				}
			}

			}
		}
		else
		{
			// fill call parameters
			VoIPCallPrivate_t voipCallParam = {0};
			memset(&voipCallParam,0,sizeof(voipCallParam));
			voipCallParam.m_PrivateData.m_u32DataSize = sizeof(VoIPCallPrivate_t);
			voipCallParam.m_s32Protocol = g_iProtocol == 0 ? XMS_VOIP_PROTOCOL_H323 : XMS_VOIP_PROTOCOL_SIP;

			strcpy(voipCallParam.m_s8CalleeAddress, m_strCalleeAddr);
			if (strlen(m_strCalleeAddr) >0)
				voipCallParam.m_s32CalleePort = atol(m_strCalleePort);
			strcpy(voipCallParam.m_s8CalleeUserID, "");
			
			r = XMS_ctsMakeCallOut(g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
				"", "", (PrivateData_t *)&voipCallParam);

			//--------------add end
			//r = XMS_ctsMakeCallOut ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
			//							Str_callingNum,	Str_calledNum,
			//							NULL );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsMakeCallOut(%s,%s:%s) FAIL! (%s)",
					r, Str_callingNum, m_strCalleeAddr,m_strCalleePort, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsMakeCallOut(%s,%s:%s) OK! (%s)",
					Str_callingNum, m_strCalleeAddr,m_strCalleePort, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
		}
	}
	else //if (DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain == XMS_DEVMAIN_VOIP )
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;

		//--------------add begin

		// fill call parameters
		VoIPCallPrivate_t voipCallParam = {0};
		memset(&voipCallParam,0,sizeof(voipCallParam));
		voipCallParam.m_PrivateData.m_u32DataSize = sizeof(VoIPCallPrivate_t);
		voipCallParam.m_s32Protocol = g_iProtocol == 0 ? XMS_VOIP_PROTOCOL_H323 : XMS_VOIP_PROTOCOL_SIP;

		strcpy(voipCallParam.m_s8CalleeAddress, m_strCalleeAddr);
		if (strlen(m_strCalleeAddr) >0)
			voipCallParam.m_s32CalleePort = atol(m_strCalleePort);
		strcpy(voipCallParam.m_s8CalleeUserID, "");
		
		r = XMS_ctsMakeCallOut(g_acsHandle, &DevOpened[iOpenedIndex3].DevID, 
			"", "", (PrivateData_t *)&voipCallParam);

		//--------------add end
		//r = XMS_ctsMakeCallOut ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
		//							Str_callingNum,	Str_calledNum,
		//							NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsMakeCallOut(%s,%s) FAIL! (%s)",
				r, Str_callingNum, Str_calledNum, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID) );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsMakeCallOut(%s,%s) OK! (%s)",
				Str_callingNum, Str_calledNum, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID) );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
}

void	DoFunc_LinkDevice(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		DevStr1[64], DevStr2[64],DevStr3[64];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));
	sprintf ( DevStr3, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID));

	if( g_Select == 0)
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;

		r = XMS_ctsLinkDevice ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &DevOpened[iOpenedIndex2].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsLinkDevice() FAIL! src(%s) -> dst(%s)", 
				r, DevStr1, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsLinkDevice() OK! src(%s) -> dst(%s)", 
				DevStr1, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;

		r = XMS_ctsLinkDevice ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, &DevOpened[iOpenedIndex2].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsLinkDevice() FAIL! src(%s) -> dst(%s)", 
				r, DevStr3, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsLinkDevice() OK! src(%s) -> dst(%s)", 
				DevStr3, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
}

// --------------------------------------------------------------------
// end of call control
// --------------------------------------------------------------------


// --------------------------------------------------------------------
// link/play/record
// --------------------------------------------------------------------
void	DoFunc_UnlinkDevice(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		DevStr1[64], DevStr2[64],DevStr3[64];

	if ( !bStartWorkFlag )	return;



	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));
	sprintf ( DevStr3, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID));

	if( g_Select == 0)
	{
		if ( !CheckOpenDevice (iOpenedIndex1) )
			return;

		r = XMS_ctsUnlinkDevice ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &DevOpened[iOpenedIndex2].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsUnlinkDevice() FAIL! src(%s) -> dst(%s)", 
				r, DevStr1, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsUnlinkDevice() OK! src(%s) -> dst(%s)", 
				DevStr1, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	/*else
	{
		if ( !CheckOpenDevice (iOpenedIndex3) )
			return;

		r = XMS_ctsUnlinkDevice ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &DevOpened[iOpenedIndex2].DevID, NULL );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsUnlinkDevice() FAIL! src(%s) -> dst(%s)", 
				r, DevStr1, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsUnlinkDevice() OK! src(%s) -> dst(%s)", 
				DevStr1, DevStr2  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}*/
}

void	DoFunc_Play(bool bIsDetailPlay)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		TmpStr1[32], TmpStr2[ACS_MAX_FILE_NAME_LEN];
	PlayProperty_t		tmp_PlayProperty;

	CXMSApi_PlaySet	MyPlaySet;

	if ( !bStartWorkFlag )	return;

#ifndef	DEBUG_IN_HOME_NOTEBOOK
	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;
#endif

	if ( bIsDetailPlay )
	{
		g_PlayProperty.m_u8TaskID = (DJ_S8)GetTickCount();
		pdlg->GetDlgItem ( IDC_EDIT_PLAY )->GetWindowText ( g_PlayProperty.m_s8PlayContent, ACS_MAX_FILE_NAME_LEN-1 );

		int	nRes = MyPlaySet.DoModal ();
		if ( nRes != IDOK )
		{
			return;
		}

		memcpy ( &tmp_PlayProperty, &g_PlayProperty, sizeof(PlayProperty_t) );
	}
	else
	{
		memset ( &tmp_PlayProperty, 0, sizeof(PlayProperty_t) );

		tmp_PlayProperty.m_u8TaskID = (DJ_S8)GetTickCount();
		pdlg->GetDlgItem ( IDC_EDIT_PLAY )->GetWindowText ( tmp_PlayProperty.m_s8PlayContent, ACS_MAX_FILE_NAME_LEN-1 );
	}

	{
		strcpy ( TmpStr1, "PLAY_FILE" );

		if ( tmp_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_INDEX )
			sprintf ( TmpStr2, "<index=%d>", tmp_PlayProperty.m_u16PlayIndex );
		else if ( tmp_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_INDEX_QUEUE )
			sprintf ( TmpStr2, "<index Q=%d>", tmp_PlayProperty.m_u16PlayIndex );
		else if ( tmp_PlayProperty.m_u16PlayType == XMS_PLAY_TYPE_FILE )
			sprintf ( TmpStr2, "<\"%s\">", tmp_PlayProperty.m_s8PlayContent );
		else
			sprintf ( TmpStr2, "<File Q\"%s\">", tmp_PlayProperty.m_s8PlayContent );
	}

	r = XMS_ctsPlay ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &tmp_PlayProperty, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsPlay(%s, %s) FAIL! (%s)", 
			r, TmpStr1, TmpStr2,
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsPlay(%s, %s) OK! (%s)", 
			TmpStr1, TmpStr2, 
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_MyStopPlay(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	ControlPlay_t	controlPlay;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	controlPlay.m_u16ControlType = XMS_STOP_PLAY;

	r = XMS_ctsControlPlay ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &controlPlay, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) MyStopPlay() FAIL! (%s)", 
			r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "MyStopPlay() OK! (%s)", 
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_ControlPlay_Real(ControlPlay_t *pControlPlay)
{
	RetCode_t	r;
	char		MsgStr[160];

	r = XMS_ctsControlPlay ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, pControlPlay, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsControlPlay(%s, %d) FAIL! (%s)", 
			r, 
			GetString_PlayControlType(pControlPlay->m_u16ControlType),pControlPlay->m_u16StepSize,
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsControlPlay(%s, %d) OK! (%s)", 
			GetString_PlayControlType(pControlPlay->m_u16ControlType),pControlPlay->m_u16StepSize,
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_ControlPlay(void)
{
	CXMSApi_ControlPlay	MyControl;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	MyControl.DoModal ();

}

void	DoFunc_Record ( bool bIsDetailRecord )
{
	RetCode_t			r;
	char				MsgStr[160];
	RecordProperty_t	recordProperty;
	CXMSApi_RecordSet	MyRecordSet;
	
	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	pdlg->GetDlgItem ( IDC_EDIT1_RECORD )->GetWindowText ( g_RecordProperty.m_s8FileName, ACS_MAX_FILE_NAME_LEN-1 );
	
	if ( bIsDetailRecord )
	{
		int	nRes = MyRecordSet.DoModal ();
		if ( nRes != IDOK )
		{
			return;
		}
	}

	memcpy ( &recordProperty, &g_RecordProperty, sizeof(RecordProperty_t) );

	r = XMS_ctsRecord ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &recordProperty, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsRecord(%s) FAIL! (%s)", 
			r, recordProperty.m_s8FileName, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsRecord(%s) OK! (%s)", 
			recordProperty.m_s8FileName, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_MyStopRecord(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	ControlRecord_t	controlRecord;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	controlRecord.m_u32ControlType = XMS_STOP_RECORD;

	r = XMS_ctsControlRecord ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &controlRecord, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) MyStopRecord() FAIL! (%s)", 
			r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "MyStopRecord() OK! (%s)", 
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_ControlRecord_Real(ControlRecord_t *pControlRecord)
{
	RetCode_t	r;
	char		MsgStr[160];

	r = XMS_ctsControlRecord ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, pControlRecord, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsControlRecord(%s, %d) FAIL! (%s)", 
			r, 
			GetString_RecordControlType(pControlRecord->m_u32ControlType),pControlRecord->m_u32StepSize,
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsControlRecord(%s, %d) OK! (%s)", 
			GetString_RecordControlType(pControlRecord->m_u32ControlType),pControlRecord->m_u32StepSize,
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_ControlRecord(void)
{
	CXMSApi_ControlRecord	MyControl;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	MyControl.DoModal ();

}
// --------------------------------------------------------------------
// end of link/play/record
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// Fax
// --------------------------------------------------------------------
void	DoFunc_SendFax(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		TiffFileName[ACS_MAX_FILE_NAME_LEN];
	char		s8LocalID[20];
	char		DevStr1[64], DevStr2[64];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	pdlg->GetDlgItem ( IDC_EDIT_SENDFAX )->GetWindowText ( TiffFileName, ACS_MAX_FILE_NAME_LEN-1 );
	sprintf ( s8LocalID, "DJ:83636988" );

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));

	r = XMS_ctsSendFax ( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// FaxDevice
					&DevOpened[iOpenedIndex2].DevID,			// mediaDevice
					TiffFileName,
					s8LocalID,
					NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsSendFax(%s, %s) FAIL! fax(%s) media(%s)", 
			r, TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsSendFax(%s, %s) OK! fax(%s) media(%s)", 
			TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

}

void	DoFunc_StopSendFax(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	r = XMS_ctsStopSendFax ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsStopSendFax() FAIL! fax(%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsStopSendFax() OK! fax(%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_ReceiveFax(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		TiffFileName[ACS_MAX_FILE_NAME_LEN];
	char		s8LocalID[20];
	char		DevStr1[64], DevStr2[64];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	pdlg->GetDlgItem ( IDC_EDIT_RECEIVEFAX )->GetWindowText ( TiffFileName, ACS_MAX_FILE_NAME_LEN-1 );
	sprintf ( s8LocalID, "DJ:83636988" );

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));

	r = XMS_ctsReceiveFax ( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// FaxDevice
					&DevOpened[iOpenedIndex2].DevID,			// mediaDevice
					TiffFileName,
					s8LocalID,
					NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsReceiveFax(%s, %s) FAIL! media(%s) fax(%s)", 
			r, TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsReceiveFax(%s, %s) OK! fax(%s) media(%s)", 
			TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_StopReceiveFax(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	r = XMS_ctsStopReceiveFax ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsStopReceiveFax() FAIL! fax(%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsStopReceiveFax() OK! fax(%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}
// --------------------------------------------------------------------
// end Fax
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// conference 
// --------------------------------------------------------------------
int		g_IsLeaveConf = false;

void 	DoFunc_JoinToConf(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		DevStr1[64], DevStr2[64];
	CXMSApi_ConfParam	MyConfParam;
	CmdParamData_Conf_t	confParam;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	int	nRes;

	g_IsLeaveConf = false;
	nRes = MyConfParam.DoModal ();
	if ( nRes != IDOK )
		return;

	confParam = g_Conf_Param;

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));

	r = XMS_ctsJoinToConf ( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// confDevice
					&DevOpened[iOpenedIndex2].DevID,			// MediaDevice
					&confParam,
					NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsJoinToConf(InOpt=%d,OutOpt=%d) FAIL! conf(%s) media(%s)", 
			r, confParam.m_u8InputOpt, confParam.m_u8OutputOpt,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsJoinToConf(InOpt=%d,OutOpt=%d) OK! conf(%s) media(%s)", 
			confParam.m_u8InputOpt, confParam.m_u8OutputOpt,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

}

void	DoFunc_LeaveFromConf(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	char		DevStr1[64], DevStr2[64];
	CXMSApi_ConfParam	MyConfParam;
	CmdParamData_Conf_t	confParam;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	int	nRes;

	g_IsLeaveConf = true;
	nRes = MyConfParam.DoModal ();
	if ( nRes != IDOK )
		return;

	confParam = g_Conf_Param;

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));

	r = XMS_ctsLeaveFromConf ( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// confDevice
					&DevOpened[iOpenedIndex2].DevID,			// MediaDevice
					&confParam,
					NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsLeaveFromConf() FAIL! conf(%s) media(%s)", 
			r, DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsLeaveFromConf() OK! conf(%s) media(%s)", 
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void 	DoFunc_ClearConf(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	r = XMS_ctsClearConf ( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// confDevice
					NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsClearConf() FAIL! conf(%s)", 
			r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsClearConf() OK! conf(%s)", 
			 GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

// --------------------------------------------------------------------
// end of conference 
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// other
// --------------------------------------------------------------------
void	DoFunc_SetParam(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	void		* p;
	int			nRes;

	// parameter
	DJ_U16		u16ParamType, u16ParamSize;
	CXMSApi_Test_ParamVoice	My_P_Voice;
	CXMSApi_ParamCAS		My_P_CAS;
	CDialogAnaTrunk         My_P_Trunk;
	CDialogAnalogUser       My_P_User;
	CXMS_Test_ParamVoIP     My_P_VoIP;
	CDialogFskSet			My_P_Fsk;
	CDialogFlashSet			My_P_Flash;
	CDialogGTDSet			My_P_GtdSet;
	DialogAtrunkFlash		My_P_ATrunkFlash;
	CDialog3GFaxTone        My_P_3GFaxTone;
	CDialogFaxHeader		My_P_FaxHeader;
	
#ifndef	DEBUG_IN_HOME_NOTEBOOK
	if ( !bStartWorkFlag )	return;

if ( iComboParam != 5 )
{
	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;
}

#endif

	switch (iComboParam)
	{
	case 0:		// Voice
 		nRes = My_P_Voice.DoModal ();

		if ( nRes != IDOK )
		{
			return ;
		}

		u16ParamType = VOC_PARAM_UNIPARAM;
		u16ParamSize = sizeof(CmdParamData_Voice_t);
		p = (void *)&g_Voice_Param;
		break;

	case 1:		// CAS
		// parameter CAS


		if ( !CheckOpenDevice (iOpenedIndex2) )
			return;
		
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_E1_CAS )
		{
			AfxMessageBox("Dev1 is NOT a CAS Device!");
			return;
		}

		nRes = My_P_CAS.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		g_CAS_Param.m_VocDevID = DevOpened[iOpenedIndex2].DevID;

		u16ParamType = CAS_PARAM_UNIPARAM;
		u16ParamSize = sizeof(CmdParamData_CAS_t);
		p = (void *)&g_CAS_Param;
		//
		break;

	case 2: //Analog User

		//if ( !CheckOpenDevice (iOpenedIndex2) )
		//	return;
		
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_ANALOG_USER )
		{
			AfxMessageBox("Dev1 is NOT an AnalogUser Device!");
			return;
		}
		
		nRes = My_P_User.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		//g_AnalogUser.m_VocDevID = DevOpened[iOpenedIndex2].DevID;

		u16ParamType = ANALOGUSER_PARAM_UNIPARAM;
		u16ParamSize = sizeof(CmdParamData_AnalogUser_t);
		p = (void *)&g_AnalogUser;

		break;

	case 3: //Analog Trunk
		//if ( !CheckOpenDevice (iOpenedIndex2) )
		//	return;

		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_ANALOG_TRUNK )
		{
			AfxMessageBox("Dev1 is NOT An AnalogTrunk Device!");
			return;
		}

		nRes = My_P_Trunk.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		//g_AnalogTrunk.m_VocDevID = DevOpened[iOpenedIndex2].DevID;

		u16ParamType = ANALOGTRUNK_PARAM_UNIPARAM;
		u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);
		p = (void *)&g_AnalogTrunk;
		break;

	case 4://VoIP
		//if( !CheckOpenDevice(iOpenedIndex3) )
		//	return;

		//if ( !CheckOpenDeviceForVoIP (&DevOpened[iOpenedIndex1].DevID) )
		//	return;

		if( !CheckOpenDevice(iOpenedIndex2,false) )
			return;

		if( !CheckOpenDeviceForVoIP(iOpenedIndex1 ) )
			return;

		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOIP)
		{
			AfxMessageBox("Dev1 is NOT A Voip Device!");
			return;
		}

		nRes = My_P_VoIP.DoModal();
		if( nRes != IDOK)
		{
			return;
		}

		//g_VoIP.m_u16SRC_ChID1 = g_deviceVoc[iDeviceVoc].device.m_s16ChannelID;

		g_VoIP_Param.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
		g_VoIP_Param.m_u16SRC_ChID1 = DevOpened[iOpenedIndex2].DevID.m_s16ChannelID;
		u16ParamType = VOIP_PARAM_RTPMIXER;
		u16ParamSize = sizeof(MixerControlParam_t);
		p = (void *)&g_VoIP_Param;

		break;

	case 5: //fsk set
		//if ( !CheckOpenDevice (iOpenedIndex2) )
		//	return;

		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_BOARD )
		{
			AfxMessageBox("Dev1 is NOT A Board Device!");
			return;
		}

		nRes = My_P_Fsk.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		u16ParamType = BOARD_PARAM_SETFSK;
		u16ParamSize = sizeof(CmdParamData_FskParam_t);
		p = (void *)&g_FskSet;
		break;
	case 6: //flash Time  set
			//if ( !CheckOpenDevice (iOpenedIndex2) )
		//	return;

		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_BOARD )
		{
			AfxMessageBox("Dev1 is NOT A Board Device!");
			return;
		}

		nRes = My_P_Flash.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		u16ParamType = BOARD_PARAM_SETFLASH;
		u16ParamSize = sizeof(CmdParamData_FlashParam_t);
		p = (void *)&g_FlashSet;
		break;
	case 7://setGTD
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_BOARD )
		{
			AfxMessageBox("Dev1 Is NOT A Board Device!");
			return;
		}

		nRes = My_P_GtdSet.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		//*** Set GTD_Freq
		u16ParamType = 	BOARD_PARAM_SETGTDFREQ ;
		u16ParamSize = sizeof(CmdParamData_GtdFreq_t);
		p = (void *)&g_GtdFreq;

		r = XMS_ctsSetParam( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, u16ParamType, u16ParamSize, (DJ_Void *)p );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsSetParam( ParamType = %s GTD_FREQ) FAIL! (%s)", 
				r, ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsSetParam( ParamType = %s GTD_FREQ) OK! (%s)", 
				ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		
		//Set GTD ProtoType(GTD_Template)
		u16ParamType = 	BOARD_PARAM_SETGTDTONE ;
		u16ParamSize = sizeof(CmdParamData_GtdProtoType_t);
		p = (void *)&g_GtdProto;

		r = XMS_ctsSetParam( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, u16ParamType, u16ParamSize, (DJ_Void *)p );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsSetParam( ParamType = %s GTD_PROTO) FAIL! (%s)", 
				r, ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsSetParam( ParamType = %s GTD_PROTO) OK! (%s)", 
				ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}

		return;

		//break;
	case 8: //Analog Trunk Set FlashEvent
		
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_ANALOG_TRUNK )
		{
			AfxMessageBox("Dev1 is NOT An AnalogTrunk Device!");
			return;
		}

		nRes = My_P_ATrunkFlash.DoModal ();
		if ( nRes != IDOK )
		{
			return ;
		}

		//g_ATrunkFlash.m_VocDevID = DevOpened[iOpenedIndex2].DevID;

		u16ParamType = ANALOGTRUNK_PARAM_SETFLASH;
		u16ParamSize = sizeof(CmdParamData_AnalogTrunkFlash_t);
		p = (void *)&g_ATrunkFlash;
		break;

	/*case 9:
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_ANALOG_TRUNK )
		{
			AfxMessageBox("Dev1 is NOT An AnalogTrunk Device!");
			return;
		}
		u16ParamType = ANALOGTRUNK_PARAM_UNSETUNIPARAM;
		u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);
		memset(&g_AnalogTrunk,-1,sizeof(g_AnalogTrunk));
		p = (void *)&g_AnalogTrunk;
		break;*/
	case 9: //key interrupt
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOICE )
		{
			AfxMessageBox("Dev1 Is NOT A Voice Device!");
			return;
		}
		return;
		break;
	case 10://3G FaxTone
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOICE )
		{
			AfxMessageBox("Dev1 Is NOT A Voice Device!");
			return;
		}
		nRes = My_P_3GFaxTone.DoModal();
		if ( nRes != IDOK )
		{
			return ;
		}

		u16ParamType = VOC_PARAM_3GFAXTONE;
		u16ParamSize = sizeof(ITP_3G_FaxToneStruct_t);
		p = (void *)&g_3GFaxTone;

		break;
	/*case 12://analogtrunk unbind
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_ANALOG_TRUNK )
		{
			AfxMessageBox("Dev1 is NOT An AnalogTrunk Device!");
			return;
		}

		u16ParamType = ANALOGTRUNK_PARAM_UNBINDVOC;
		u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);
		p=(void*)&g_AnalogTrunk;
		break;
	case 13://analogUSER unbind
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_INTERFACE_CH || 
			 DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub  != XMS_DEVSUB_ANALOG_USER )
		{
			AfxMessageBox("Dev1 is NOT An AnalogUser Device!");
			return;
		}

		u16ParamType = ANALOGUSER_PARAM_UNBINDVOC;
		u16ParamSize = sizeof(CmdParamData_AnalogUser_t);
		p=(void*)&g_AnalogUser;
		break;*/
	case 11://fax setheader
		if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_FAX )
		{
			AfxMessageBox("Dev1 Is NOT A Fax Device!");
			return;
		}
		nRes = My_P_FaxHeader.DoModal();
		if ( nRes != IDOK )
		{
			return ;
		}

		u16ParamType = FAX_PARAM_FAXHEADER;
		u16ParamSize = sizeof(CmdParamData_FaxSetHeader_t);
		p = (void *)&g_FaxHeader;
		break;
	default:
		sprintf ( MsgStr, "Parameter Type[%s] Not Support!", ParamTypeStr[iComboParam] );
		AfxMessageBox ( MsgStr );
		AddMsg ( MSG_TYPE_OTHER, MsgStr );
		return;
	}

	
	if( g_Select == 0)
	{
		r = XMS_ctsSetParam( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, u16ParamType, u16ParamSize, (DJ_Void *)p );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsSetParam( ParamType = %s ) FAIL! (%s)", 
				r, ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsSetParam( ParamType = %s ) OK! (%s)", 
				ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	else
	{
		r = XMS_ctsSetParam( g_acsHandle, &DevOpened[iOpenedIndex3].DevID, u16ParamType, u16ParamSize, (DJ_Void *)p );
		if ( r < 0 )
		{
			sprintf ( MsgStr, "X(%d) XMS_ctsSetParam( ParamType = %s ) FAIL! (%s)", 
				r, ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
		else
		{
			sprintf ( MsgStr, "XMS_ctsSetParam( ParamType = %s ) OK! (%s)", 
				ParamTypeStr[iComboParam], GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID)  );
			AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
		}
	}
	

}

void	DoFunc_InitPlayIndex(void)
{
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	r = XMS_ctsInitPlayIndex( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );

	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsInitPlayIndex( ) FAIL! (%s)", 
			r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsInitPlayIndex( ) OK! (%s)", 
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

	pdlg->m_ComboIndexList.ResetContent();
}

void	DoFunc_BuildPlayIndex_Real(void)
{
	RetCode_t	r;
	char		MsgStr[160], TmpS[32];

	if ( g_BuildIndexProperty.m_u16PlayType == XMS_BUILD_INDEX_FILE )
		strcpy ( TmpS, "(FILE)" );
	else
		strcpy ( TmpS, "(RAM )" );

	//g_BuildIndexProperty.m_u16PlayIndex = 0 ;
	r = XMS_ctsBuildPlayIndex ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &g_BuildIndexProperty, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsBuildPlayIndex( %s %s ) FAIL! (%s)", 
			r, TmpS, g_BuildIndexProperty.m_s8PlayContent, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsInitPlayIndex( %s %s ) OK! (%s)", 
			TmpS, g_BuildIndexProperty.m_s8PlayContent, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

	// add to the Combo
	sprintf ( MsgStr, "%2d - %s %s", pdlg->m_ComboIndexList.GetCount(), TmpS, g_BuildIndexProperty.m_s8PlayContent );
	pdlg->m_ComboIndexList.AddString (MsgStr);
	pdlg->m_ComboIndexList.SetCurSel (pdlg->m_ComboIndexList.GetCount()-1);
}

void	DoFunc_BuildPlayIndex(void)
{
	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	CXMSApi_BuildIndex	MyBuildIndex;

	MyBuildIndex.DoModal ();
}

void	DoFunc_SendIOData(void)
{
	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	CXMSApi_SendIOData	MySendIOData;

	MySendIOData.DoModal ();
}

void	DoFunc_ConnDsp(void)
{
	ServerID_t serverID={0};
	char szBuf[40]={0};

	if ( !bStartWorkFlag )	return;
	
	pdlg->GetDlgItem ( IDC_EDIT_DSPPORT )->GetWindowText (szBuf , 40);
	serverID.m_u32ServerPort = atoi(szBuf);
	pdlg->GetDlgItem ( IDC_EDIT_DSPIP )->GetWindowText (serverID.m_s8ServerIp,sizeof(serverID.m_s8ServerIp));
	GetPrivateProfileString("ConfigInfo","UserName","",serverID.m_s8UserName,sizeof(serverID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",serverID.m_s8UserPwd,sizeof(serverID.m_s8UserPwd),cfg_IniName);

	int r = XMS_acsOpenStreamExt(g_acsHandle,&serverID,NULL);
}

void DoFunc_DisConnDsp()
{
	ServerID_t serverID={0};
	char szBuf[40]={0};

	if ( !bStartWorkFlag )	return;
	
	pdlg->GetDlgItem ( IDC_EDIT_DSPPORT )->GetWindowText (szBuf , 40);
	serverID.m_u32ServerPort = atoi(szBuf);
	pdlg->GetDlgItem ( IDC_EDIT_DSPIP )->GetWindowText (serverID.m_s8ServerIp,sizeof(serverID.m_s8ServerIp));
	GetPrivateProfileString("ConfigInfo","UserName","",serverID.m_s8UserName,sizeof(serverID.m_s8UserName),cfg_IniName);
	GetPrivateProfileString("ConfigInfo","PassWord","",serverID.m_s8UserPwd,sizeof(serverID.m_s8UserPwd),cfg_IniName);

	int r = XMS_acsCloseStreamExt(g_acsHandle,&serverID,NULL);	
}

void DoFunc_RecCSP()
{
	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;
	
	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	DevRecordCsp  = DevOpened[iOpenedIndex1].DevID;

	CRecCSPDlg RecCSPDlg;

	RecCSPDlg.DoModal();
}

void DoFunc_CtrlRecCSP()
{
	int  r = 0;
	char MsgStr[100]={0};
	ControlRecord_t ctrlRecCSP;

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	memset(&ctrlRecCSP,0,sizeof(ctrlRecCSP));
	ctrlRecCSP.m_u32ControlType = XMS_CSP_STOP_RECORD;

	r = XMS_ctsControlRecordCSP(g_acsHandle, &DevOpened[iOpenedIndex1].DevID,&ctrlRecCSP,NULL);
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsControlRecordCSP() FAIL! (%s)", 
			r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsControlRecordCSP() OK! (%s)", 
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

	fclose(g_cspFp);
}

void DoFunc_CTXLink()
{
#if 0
	RetCode_t	r;
	char		MsgStr[160];
	char		DevStr1[64], DevStr2[64], DevStr3[64];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex3, false) )
		return;

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));
	sprintf ( DevStr3, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID));

	r = XMS_ctsCTXLinkDevice( g_acsHandle, &DevOpened[iOpenedIndex3].DevID,&DevOpened[iOpenedIndex1].DevID, &DevOpened[iOpenedIndex2].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsCTXLinkDevice() FAIL! src(%s) -> dst(%s)", 
			r, DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsCTXLinkDevice() OK! src(%s) -> dst(%s)", 
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}	
#endif
}

void DoFunc_CTXUnLink()
{
#if 0
	RetCode_t	r;
	char		MsgStr[160];
	char		DevStr1[64], DevStr2[64], DevStr3[64];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex3, false) )
		return;

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));
	sprintf ( DevStr3, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID));

	r = XMS_ctsCTXUnlinkDevice ( g_acsHandle, &DevOpened[iOpenedIndex3].DevID,&DevOpened[iOpenedIndex1].DevID, &DevOpened[iOpenedIndex2].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsCTXUnlinkDevice() FAIL! src(%s) -> dst(%s)", 
			r, DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsCTXUnlinkDevice() OK! src(%s) -> dst(%s)", 
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
#endif
}

void	DoFunc_StartFaxT38(void)
{
	char		  DevStr1[64], DevStr2[64],DevStr3[64];

	char		  MsgStr[160];

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));
	//sprintf ( DevStr3, GetString_DeviceAll (&DevOpened[iOpenedIndex3].DevID));

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1, false) )
		return;

	RetCode_t r = XMS_ctsSendIOData(g_acsHandle, &DevOpened[iOpenedIndex1].DevID,
			XMS_IO_TYPE_VOIP_T38START, 0, NULL);

	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData(%s) FAIL! ", 
						r,DevStr1  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData(%s) OK! ", 
						r,DevStr3  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_SendFaxT38(void)
{
	RetCode_t	  r;
	char		  MsgStr[160];
	char		  TiffFileName[ACS_MAX_FILE_NAME_LEN];
	char		  s8LocalID[20];
	char		  DevStr1[64], DevStr2[64];
	FaxPrivate_t  faxPriv={0};

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;
		
	//if ( !CheckOpenDevice (iOpenedIndex3, false) )
	//	return;


	pdlg->GetDlgItem ( IDC_EDIT_SENDFAX )->GetWindowText ( TiffFileName, ACS_MAX_FILE_NAME_LEN-1 );
	sprintf ( s8LocalID, "DJ:83636988" );

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));

	faxPriv.m_PrivateData.m_u32DataSize = sizeof(faxPriv);
	faxPriv.m_VoIPDevice = DevOpened[iOpenedIndex3].DevID;	
	faxPriv.m_u8WorkMode = pdlg->m_FaxType.GetCurSel();
			
	r = XMS_ctsSendFax( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// FaxDevice
					&DevOpened[iOpenedIndex2].DevID,			// mediaDevice					
					TiffFileName,
					s8LocalID,
					(PrivateData_t *)(&faxPriv) );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsSendFaxT38(%s, %s) FAIL! fax(%s) media(%s)", 
			r, TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsSendFaxT38(%s, %s) OK! fax(%s) media(%s)", 
			TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_StopSendFaxT38(void)
{
	RetCode_t	 r;
	char		 MsgStr[160];
	FaxPrivate_t faxPriv={0};

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	faxPriv.m_PrivateData.m_u32DataSize = sizeof(faxPriv);
	faxPriv.m_VoIPDevice = DevOpened[iOpenedIndex3].DevID;
	faxPriv.m_u8WorkMode = pdlg->m_FaxType.GetCurSel();

	r = XMS_ctsStopSendFax( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsStopSendFaxT38() FAIL! fax(%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsStopSendFaxT38() OK! fax(%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_ReceiveFaxT38(void)
{
	RetCode_t	 r;
	char		 MsgStr[160];
	char		 TiffFileName[ACS_MAX_FILE_NAME_LEN];
	char		 s8LocalID[20];
	char		 DevStr1[64], DevStr2[64];
	FaxPrivate_t faxPriv={0};

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex2, false) )
		return;

	if ( !CheckOpenDevice (iOpenedIndex3, false) )
		return;

	pdlg->GetDlgItem ( IDC_EDIT_RECEIVEFAX )->GetWindowText ( TiffFileName, ACS_MAX_FILE_NAME_LEN-1 );
	sprintf ( s8LocalID, "DJ:83636988" );

	sprintf ( DevStr1, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID));
	sprintf ( DevStr2, GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID));

	faxPriv.m_PrivateData.m_u32DataSize = sizeof(faxPriv);
	faxPriv.m_VoIPDevice = DevOpened[iOpenedIndex3].DevID;
	faxPriv.m_u8WorkMode = XMS_FAX_WORKMODE_SERVER;

	r = XMS_ctsReceiveFax( g_acsHandle, 
					&DevOpened[iOpenedIndex1].DevID,			// FaxDevice
					&DevOpened[iOpenedIndex2].DevID,			// mediaDevice					
					TiffFileName,
					s8LocalID,
					NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsReceiveFaxT38(%s, %s) FAIL! media(%s) fax(%s)", 
			r, TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsReceiveFaxT38(%s, %s) OK! fax(%s) media(%s)", 
			TiffFileName, s8LocalID,
			DevStr1, DevStr2  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void	DoFunc_StopReceiveFaxT38(void)
{
	RetCode_t	  r;
	char		  MsgStr[160];
	FaxPrivate_t  faxPriv={0};

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	faxPriv.m_PrivateData.m_u32DataSize = sizeof(faxPriv);
	faxPriv.m_VoIPDevice = DevOpened[iOpenedIndex3].DevID;
	faxPriv.m_u8WorkMode = XMS_FAX_WORKMODE_SERVER;

	r = XMS_ctsStopReceiveFax( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsStopReceiveFaxT38() FAIL! fax(%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsStopReceiveFaxT38() OK! fax(%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void DoFunc_PlayCSP()
{
	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	g_cspFp = fopen(g_szCSPFile,"rb");	
	
	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;
	
	DevPlayCsp  = DevOpened[iOpenedIndex1].DevID;

	CCSPPlayDlg playCSPDlg;

	playCSPDlg.DoModal();
}

void	DoFunc_ControlPlayCSP(void)
{
	RetCode_t	r;
	char		MsgStr[160];
	ControlPlay_t	controlPlay;

	if ( !bStartWorkFlag )	return;

	//if ( !CheckOpenDevice (iOpenedIndex1) )
	//	return;

	controlPlay.m_u16ControlType = XMS_STOP_PLAY;

	r = XMS_ctsControlPlay ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, &controlPlay, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) ControlPlayCSP() FAIL! (%s)", 
			r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "ControlPlayCSP() OK! (%s)", 
			GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
}

void DoFunc_GetBoardInfo()
{
	
	RetCode_t	r;
	char		MsgStr[160];

	if ( !bStartWorkFlag )	return;

	if ( !CheckOpenDevice (iOpenedIndex1) )
		return;

	int size = sizeof(Acs_ParamData_UserReadXmsBoardInfo);
	r = XMS_ctsGetParam ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, BOARD_PARAM_GETBOARDINFO, size,NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsGetParam () FAIL! GetBoardInfo(%s)", r, GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsGetParam() OK! GetBoardInfo(%s)", GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}
	
}

// --------------------------------------------------------------------
// end of other 
// --------------------------------------------------------------------
