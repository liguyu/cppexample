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
#include "DJAcsDevState.h"

#include "XMSApi_Test_Sub.h"
#include "XMSApi_Test_Event.h"
#include "XMSApi_Test_String.h"

char * GetString_EventType ( EventType_t EvtType )
{
	static char	TmpStr[40];

	switch ( EvtType )
	{
	case XMS_EVT_OPEN_STREAM:		return "OPEN_STREAM";
	case XMS_EVT_QUERY_DEVICE:		return "QUERY_DEVICE";
	case XMS_EVT_QUERY_DEVICE_END:	return "QUERY_DEVICE_END";
	case XMS_EVT_OPEN_DEVICE:		return "OPEN_DEVICE";
	case XMS_EVT_CLOSE_DEVICE:		return "CLOSE_DEVICE";
	case XMS_EVT_RESET_DEVICE:		return "RESET_DEVICE";
	case XMS_EVT_DEVICESTATE:		return "DEVICESTATE";
	case XMS_EVT_SETDEV_GROUP:		return "SETDEV_GROUP";
	case XMS_EVT_SETPARAM:			return "SETPARAM";
	case XMS_EVT_GETPARAM:			return "GETPARAM";
	case XMS_EVT_QUERY_ONE_DSP_START:	return "QUERY_ONE_DSP_START";
	case XMS_EVT_QUERY_ONE_DSP_END:		return "QUERY_ONE_DSP_END";
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_START:	return "QUERY_REMOVE_ONE_DSP_START";
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:		return "QUERY_REMOVE_ONE_DSP_END";
	case XMS_EVT_OPEN_STREAM_EXT:	return "OPEN_STREAM_EXT";

	case XMS_EVT_CALLOUT:			return "CALLOUT";
	case XMS_EVT_CALLIN:			return "CALLIN";
	case XMS_EVT_ALERTCALL:			return "ALERTCALL";
	case XMS_EVT_ANSWERCALL:		return "ANSWERCALL";
	case XMS_EVT_LINKDEVICE:		return "LINKDEVICE";
	case XMS_EVT_UNLINKDEVICE:		return "UNLINKDEVICE";
	case XMS_EVT_CLEARCALL:			return "CLEARCALL";
	case XMS_EVT_ANALOG_INTERFACE:	return "ANALOG_INTERFACE";
	case XMS_EVT_CAS_MFC_START:		return "XMS_EVT_CAS_MFC_START";
	case XMS_EVT_CAS_MFC_END:		return "XMS_EVT_CAS_MFC_END";

	case XMS_EVT_JOINTOCONF:		return "JOINTOCONF";
	case XMS_EVT_LEAVEFROMCONF:		return "LEAVEFROMCONF";
	case XMS_EVT_CLEARCONF:			return "CLEARCONF";


	case XMS_EVT_PLAY:				return "PLAY";
	case XMS_EVT_INITINDEX:			return "INITINDEX";
	case XMS_EVT_BUILDINDEX:		return "BUILDINDEX";
	case XMS_EVT_CONTROLPLAY:		return "CONTROLPLAY";
	case XMS_EVT_RECORD:			return "RECORD";
	case XMS_EVT_CONTROLRECORD:		return "CONTROLRECORD";
	case XMS_EVT_RECORDCSP:			return "RECORDCSP";
	case XMS_EVT_CONTROLRECORDCSP:  return "CONTROLRECORDCSP";
	case XMS_EVT_3GPP_PLAY:			return "3GPPPlay";
	case XMS_EVT_3GPP_CONTROLPLAY:	return "3GPPControlPlay";
	case XMS_EVT_3GPP_RECORD:		return "3GPPRecord";
	case XMS_EVT_3GPP_CONTROLRECORD: return "3GPPControlRecord";
	case XMS_EVT_PLAYCSPREQ:		return "CSPPlayReq";

	case XMS_EVT_SENDFAX:			return "SENDFAX";
	case XMS_EVT_RECVFAX:			return "RECVFAX";
	
	case XMS_EVT_SENDIODATA:		return "SENDIODATA";
	case XMS_EVT_RECVIODATA:		return "RECVIODATA";

	case XMS_EVT_CHGMONITORFILTER:	return "CHGMONITORFILTER";
	case XMS_EVT_DEV_TIMER:			return  "DEV_TIMER";

	case XMS_EVT_UNIFAILURE:		return "UNIFAILURE";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", EvtType );
	return TmpStr;
}

char *GetString_DeviceMain ( DJ_S32	s32DeviceMain )
{
	static char	TmpStr[40];

	switch ( s32DeviceMain )
	{
	case XMS_DEVMAIN_VOICE:				return "VOC  ";
	case XMS_DEVMAIN_FAX:				return "FAX  ";
	case XMS_DEVMAIN_DIGITAL_PORT:		return "DIGIT";
	case XMS_DEVMAIN_INTERFACE_CH:		return "INTCH";
	case XMS_DEVMAIN_DSS1_LINK:			return "DSS1 ";
	case XMS_DEVMAIN_SS7_LINK:			return "SS7  ";
	case XMS_DEVMAIN_BOARD:				return "BRD  ";
	case XMS_DEVMAIN_CTBUS_TS:			return "CTBUS";
	case XMS_DEVMAIN_VOIP:				return "VOIP ";
	case XMS_DEVMAIN_CONFERENCE:		return "CONF ";
	case XMS_DEVMAIN_VIDEO:				return "VIDEO";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", s32DeviceMain );
	return TmpStr;
}

char *GetString_DeviceSub (DJ_S32	s32DeviceMain, DJ_S32	s32DeviceSub )
{
	static char	TmpStr[40];

	switch(s32DeviceMain)
	{
	case XMS_DEVMAIN_VOICE:
		{
			switch(s32DeviceSub)
			{
			case XMS_DEVSUB_MEDIA_VOC:			return "VOC";
			case XMS_DEVSUB_MEDIA_324:			return "324-Device";
			}
		}
		break;

	case XMS_DEVMAIN_INTERFACE_CH:
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
			case XMS_DEVSUB_HIZ_CAS:					return "Hiz-Cas";
			case XMS_DEVSUB_HIZ_PRI:					return "Hiz-Pri";
			case XMS_DEVSUB_HIZ_SS7:					return "Hiz-SS7";
			case XMS_DEVSUB_HIZ_PRI_LINK:				return "Hiz-Pri-Link";
			case XMS_DEVSUB_HIZ_SS7_64K_LINK:			return "Hiz-SS7-64KLink";
			case XMS_DEVSUB_HIZ_SS7_2M_LINK:			return "Hiz-SS7-2MLink";
			case XMS_DEVSUB_SS7_LINK:					return "SS7-Link";
			}
		}
	}

	if( s32DeviceMain == XMS_DEVMAIN_VOIP )
		sprintf ( TmpStr, "Non-SubDev");
	else
		sprintf ( TmpStr, "UNKNOWN(0x%X)", s32DeviceSub );

	return TmpStr;
}

char *GetString_DeviceAll ( DeviceID_t *pDevID )
{
	static char	TmpStr[80];
	char	TmpS[40]; 

	strcpy ( TmpStr, "" );

	// main type
	strcat ( TmpStr, GetString_DeviceMain(pDevID->m_s16DeviceMain) );
	strcat ( TmpStr, " " );

	// dsp module ID, channel ID
	sprintf ( TmpS, "%3d, %3d, ", pDevID->m_s8ModuleID, pDevID->m_s16ChannelID );
	strcat ( TmpStr, TmpS );
	
	// sub type
	if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
		strcat ( TmpStr, GetString_DeviceSub(pDevID->m_s16DeviceMain,pDevID->m_s16DeviceSub) );

	return TmpStr;
}

char *GetString_ErrorCode ( DJ_S32	s32ErrorCode )
{
	static char	TmpStr[40];

	switch ( s32ErrorCode )
	{
	case ACSERR_LOADLIBERR:					return "LOADLIBERR";
	case ACSERR_BADPARAMETER:				return "BADPARAMETER";
	case ACSERR_NOSERVER:					return "NOSERVER";
	case ACSERR_MAXCONN_EXCEED:				return "MAXCONN_EXCEED";
	case ACSERR_BADHDL:						return "ABADHDL";

	case ACSERR_FAILSEND:					return "FAILSEND";
	case ACSERR_LINKBROKEN:					return "LINKBROKEN";
	case ACSERR_NOMESSAGE:					return "NOMESSAGE";
	case ACSERR_GETRESFAIL:					return "GETRESFAIL";
	case ACSERR_NOSUPPORTCMD:				return "NOSUPPORTCMD";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", s32ErrorCode );
	return TmpStr;
}

char *GetString_PlayControlType ( DJ_U16 u16ControlType )
{
	static char	TmpStr[40];

	switch ( u16ControlType )
	{
	case XMS_STOP_PLAY:						return "STOP_PLAY";
	case XMS_PAUSE_PLAY:					return "PAUSE_PLAY";
	case XMS_RESUME_PLAY:					return "RESUME_PLAY";
	case XMS_STEP_BACKWARD_PLAY:			return "STEP_BACKWARD_PLAY";
	case XMS_STEP_FORWARD_PLAY:				return "STEP_FORWARD_PLAY";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", u16ControlType );
	return TmpStr;
}

char *GetString_RecordControlType ( DJ_U32 u32ControlType )
{
	static char	TmpStr[40];

	switch ( u32ControlType )
	{
	case XMS_STOP_RECORD:					return "STOP_RECORD";
	case XMS_PAUSE_RECORD:					return "PAUSE_RECORD";
	case XMS_RESUME_RECORD:					return "RESUME_RECORD";
	case XMS_STEP_BACKWARD_RECORD:			return "STEP_BACKWARD_RECORD";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", u32ControlType );
	return TmpStr;
}

char *GetString_DeviceState ( DJ_U32	s32DeviceState )
{
	static char	TmpStr[40];

	switch ( s32DeviceState )
	{
	case DEVICE_COMMON_STATE_START:			return "S_BASE";
	case DCS_FREE:							return "S_FREE";
	case DCS_LOCAL_BLOCK:					return "S_LOCAL_BLOCK";
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

	return GetString_FaxState(s32DeviceState);
	//sprintf ( TmpStr, "UNKNOWN(0x%X)", s32DeviceState );
	//return TmpStr;
}

char *GetString_IOType ( DJ_U16 u16IOType )
{
	static char	TmpStr[40];

	switch ( u16IOType )
	{
	case XMS_IO_TYPE_DTMF:							return "TYPE_DTMF";
	case XMS_IO_TYPE_MR2F:							return "TYPE_MR2F";
	case XMS_IO_TYPE_MR2B:							return "TYPE_MR2B";
	case XMS_IO_TYPE_GTG:							return "TYPE_GTG";
	case XMS_IO_TYPE_FSK:							return "TYPE_FSK";
	case XMS_IO_TYPE_DPD:							return "TYPE_DPD";
	case XMS_IO_TYPE_PVD:							return "TYPE_PVD";
		
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", u16IOType );
	return TmpStr;
}


char* GetString_AnalogInterfaceState(DJ_S16 u16DevSub,DJ_U8 u8Interface)
{
	static char	TmpStr[40];

	if(u16DevSub == XMS_DEVSUB_ANALOG_USER)
	{
		switch ( u8Interface )
		{
		case XMS_ANALOG_USER_CH_OFFHOOK:				return "AUSER_OFFHOOK";
		case XMS_ANALOG_USER_CH_ONHOOK:					return "AUSER_ONHOOK";
		case XMS_ANALOG_USER_CH_FLASH:					return "AUSER_FLASH";
		}
	}
	else if(u16DevSub == XMS_DEVSUB_ANALOG_TRUNK)
	{
		switch ( u8Interface )
		{
		case XMS_ANALOG_TRUNK_CH_RING:					return "ATRUNK_RING";
		case XMS_ANALOG_TRUNK_CH_POSITEX_POLARITY:		return "ATRUNK_POSITIVE";
		case XMS_ANALOG_TRUNK_CH_NEGATIVE_POLARITY:		return "ATRUNK_NEGATIVE";
		case XMS_ANALOG_TRUNK_CH_NOLOOP:				return "ATRUNK_NOLOOP";
		}
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", u8Interface );
	return TmpStr;

}

char* GetString_FaxState(DJ_U32 u32State)
{
	static char	TmpStr[40];

	switch ( u32State )
	{

	case DES_FAX_OPEN:  return "FAX_OPEN";
	case DES_FAX_CLOSE: return "FAX_CLOSE";
	case DES_FAX_RESET: return "FAX_RESET";

	case DES_FAX_SENDFAX_RECVCMD:			return "SENDFAX_RECVCMD";
	case DES_FAX_SENDFAX_START:				return "SENDFAX_START";
	case DES_FAX_SENDFAX_SETLOCAL:			return "SENDFAX_SETLOCAL";
	case DES_FAX_SENDFAX_RESOLVE_FISNIESH:	return "SENDFAX_RESOLVE_FISNIESH";
	case DES_FAX_SENDFAX_LOCAL_FINISH:		return "SENDFAX_LOCAL_FINISH";
	case DES_FAX_SENDFAX_SHAKEHAND_END:		return "SENDFAX_SHAKEHAND_END";
	case DES_FAX_SENDFAX_SENDDATA:			return "SENDFAX_SENDDATA";
	case DES_FAX_SENDFAX_SENDFINISH:		return "SENDFAX_SENDFINISH";
	case DES_FAX_SENDFAX_USERSTOP:			return "SENDFAX_USERSTOP";
	case DES_FAX_SENDFAX_TIFFFILE_NOTEXIST:	return "SENDFAX_TIFFFILE_NOTEXIST";
	case DES_FAX_SENDFAX_SENDEND:			return "SENDFAX_SENDEND";

	case DES_FAX_RECVFAX_RECVCMD:			return "RECVFAX_RECVCMD";
	case DES_FAX_RECVFAX_WAIT_HANDSHAKE:	return "RECVFAX_WAIT_HANDSHAKE";
	case DES_FAX_RECVFAX_HANDSHAKE_OK:		return "RECVFAX_HANDSHAKE_OK";
	case DES_FAX_RECVFAX_RECVDATA_START:	return "RECVFAX_RECVDATA_START";
	case DES_FAX_RECVFAX_BACKPAGESTATE_SUCC:return "RECVFAX_BACKPAGESTATE_SUCC";
	case DES_FAX_RECVFAX_RE_HANDSHAKE:		return "RECVFAX_RE_HANDSHAKE";
	case DES_FAX_RECVFAX_RECVDATA_END:		return "RECVFAX_RECVDATA_END";
	case DES_FAX_RECVFAX_USERSTOP:			return "RECVFAX_USERSTOP";
	case DES_FAX_RECVFAX_COMPOSE_START:		return "RECVFAX_COMPOSE_START";
	case DES_FAX_RECVFAX_COMPOSE_END:		return "RECVFAX_COMPOSE_END";
	case DES_FAX_RECVFAX_BACKPAGESTATE_FAIL:return "RECVFAX_BACKPAGESTATE_FAIL";
	case DES_FAX_RECVFAX_RECVEND:			return "RECVFAX_RECVEND";
	}

	sprintf ( TmpStr, "FaxState UNKNOWN(0x%X)", u32State );
	return TmpStr;
}