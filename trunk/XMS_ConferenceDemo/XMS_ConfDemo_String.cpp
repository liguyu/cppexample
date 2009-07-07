
#include "stdafx.h"
#include "XMS_ConfDemo.h"
#include "XMS_ConfDemoDlg.h"

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_ConfDemo_String.H"

char * GetString_EventType ( EventType_t EvtType )
{
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
	case XMS_EVT_QUERY_ONE_DSP_START:				return "QUERY_ONE_DSP_START";
	case XMS_EVT_QUERY_ONE_DSP_END:					return "QUERY_ONE_DSP_END";
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_START:		return "QUERY_REMOVE_ONE_DSP_START";
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:			return "QUERY_REMOVE_ONE_DSP_END";

	case XMS_EVT_CALLOUT:			return "CALLOUT";
	case XMS_EVT_CALLIN:			return "CALLIN";
	case XMS_EVT_ALERTCALL:			return "ALERTCALL";
	case XMS_EVT_ANSWERCALL:		return "ANSWERCALL";
	case XMS_EVT_LINKDEVICE:		return "LINKDEVICE";
	case XMS_EVT_UNLINKDEVICE:		return "UNLINKDEVICE";
	case XMS_EVT_CLEARCALL:			return "CLEARCALL";
	case XMS_EVT_ANALOG_INTERFACE:	return "ANALOG_INTERFACE";
	case XMS_EVT_CAS_MFC_START:		return "CAS_MFC_START";
	case XMS_EVT_CAS_MFC_END:		return "CAS_MFC_END";

	case XMS_EVT_JOINTOCONF:		return "JOINTOCONF";
	case XMS_EVT_LEAVEFROMCONF:		return "LEAVEFROMCONF";
	case XMS_EVT_CLEARCONF:			return "CLEARCONF";


	case XMS_EVT_PLAY:				return "PLAY";
	case XMS_EVT_INITINDEX:			return "INITINDEX";
	case XMS_EVT_BUILDINDEX:		return "BUILDINDEX";
	case XMS_EVT_CONTROLPLAY:		return "CONTROLPLAY";
	case XMS_EVT_RECORD:			return "RECORD";
	case XMS_EVT_CONTROLRECORD:		return "CONTROLRECORD";

	case XMS_EVT_SENDFAX:			return "SENDFAX";
	case XMS_EVT_RECVFAX:			return "RECVFAX";
	
	case XMS_EVT_SENDIODATA:		return "SENDIODATA";
	case XMS_EVT_RECVIODATA:		return "RECVIODATA";

	case XMS_EVT_UNIFAILURE:		return "UNIFAILURE";
	}

	return "UNKNOWN";
}

char *GetString_DeviceMain ( DJ_S32	s32DeviceMain )
{
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

char *GetString_ErrorCode ( DJ_S32	s32ErrorCode )
{
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

	return "UNKNOWN";
}


void	GetString_LineState ( char *StateStr, int iLineState )
{
	switch ( iLineState )
	{
	case DCS_FREE:							strcpy ( StateStr, "S_FREE");			break;
	case DCS_LOCAL_BLOCK:					strcpy ( StateStr, "S_LOCAL_BLOCK");	break;
	case DCS_REMOTE_BLOCK:					strcpy ( StateStr, "S_REMOTE_BLOCK");	break;
	case DCS_BLOCK:							strcpy ( StateStr, "S_BLOCK");			break;
	case DCS_UNAVAILABLE:					strcpy ( StateStr, "S_UNAVAILABLE");	break;

	case DCS_CALLOUT:						strcpy ( StateStr, "S_CALLOUT");		break;
	case DCS_CALLIN:						strcpy ( StateStr, "S_CALLIN");			break;
	case DCS_ALERTING:						strcpy ( StateStr, "S_ALERTING");		break;

	case DCS_CONNECT:						strcpy ( StateStr, "S_CONNECT");		break;
	case DCS_DISCONNECT:					strcpy ( StateStr, "S_DISCONNECT");		break;
	case DCS_WAIT_FREE:						strcpy ( StateStr, "S_WAIT_FREE");		break;
	default:
		sprintf ( StateStr, "0x%X", iLineState );
		break;
	}
}

char	*GetString_PcmType ( int iPcmType )
{
	switch ( iPcmType )
	{
	case 0:		return "Disable";

	case XMS_E1_TYPE_Analog30:			return "T_Analog30";
	case XMS_E1_TYPE_PCM31:				return "T_PCM31";
	case XMS_E1_TYPE_PCM30:				return "T_PCM30";
	case XMS_E1_TYPE_CAS:				return "T_CAS";
	case XMS_E1_TYPE_PRI:				return "T_PRI";
	case XMS_E1_TYPE_SS7_TUP_0_Link:	return "T_TUP_0_LK";
	case XMS_E1_TYPE_SS7_TUP_1_Link:	return "T_TUP_1_LK";
	case XMS_E1_TYPE_SS7_TUP_2_Link:	return "T_TUP_2_L";
	case XMS_T1_TYPE_D4:				return "T_D4";
	case XMS_T1_TYPE_ESF:				return "T_ESF";
	case XMS_J1_TYPE_D4:				return "T_D4";
	case XMS_J1_TYPE_ESF:				return "T_ESF";
	case XMS_SLC_TYPE_96:				return "SLC_TYPE_96";
	case XMS_E1_TYPE_SS7_ISUP_0_Link:	return "T_ISUP_0_LK";
	case XMS_E1_TYPE_SS7_ISUP_1_Link:	return "T_ISUP_1_LK";
	case XMS_E1_TYPESS7_ISUP_2_Link:	return "T_ISUP_2_LK";
	case XMS_E1_TYPE_6D25B:				return "T_6D25B";

	}

	return "UNKNOWN";
}

