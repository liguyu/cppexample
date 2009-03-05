#include "stdafx.h"
#include "ReadPacket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern  CHECK_INFO  m_CheckInfo;
ReadPacket::ReadPacket(DJ_Void *pkt, int nNumRecv)
{
	   nmsgnum=nNumRecv;
       m_pkt=pkt;
	   EvtFlag=true;
       pAcsEvt=(Acs_Evt_t *)((DJ_S8 *)m_pkt+sizeof(DJ_S8));
	   pAcsCmd=(Acs_Cmd_t *)((DJ_S8 *)m_pkt+sizeof(DJ_S8));
	   f_head=0x00;
	   pkd_IDinfo.iAPI=0;
	   pkd_IDinfo.iEvt=0;
	   pkd_IDinfo.iDev=0;
	   pkd_IDinfo.iDsp=0;
	   pkd_IDinfo.iChnl=0;
}
ReadPacket::~ReadPacket()
{
       m_pkt=0;
	   nmsgnum=0;
       m_msg.Empty();
}
int   ReadPacket::GetPktType()
{
	char* pSrc  =(char *) m_pkt;

    f_head=(DJ_U8)(*pSrc);

    if (PKG_CMD==f_head)
    {
	   m_convert.msgType="API";
	   GetPktAPI();
       return APP_CMD;
    }
	if(PKG_EVT==f_head)
	{
		m_convert.msgType="EVT";
		GetPktEvt();
		return APP_EVT;
	}

	return 0;
}
int  ReadPacket::GetPktEvt()
{
	 m_convert.msg=GetString_EventType();
	 m_convert.dev=GetString_DeviceAll(&pAcsEvt->m_DeviceID);
	 pkd_IDinfo.iAPI=0;
	 pkd_IDinfo.iEvt=GetEvtID();
	 pkd_IDinfo.iDev=GetDevID(pAcsEvt->m_DeviceID.m_s16DeviceMain);
	 pkd_IDinfo.iDsp=pAcsEvt->m_DeviceID.m_s8ModuleID;
	 pkd_IDinfo.iChnl=pAcsEvt->m_DeviceID.m_s16ChannelID;
     return 0;
}

int  ReadPacket::GetPktAPI()
{
	 m_convert.msg=GetString_APIType();
	 m_convert.dev=GetString_DeviceAll(&pAcsCmd->m_ITPDeviceID);
	 pkd_IDinfo.iAPI=GetAPIID();
	 pkd_IDinfo.iEvt=0;
	 pkd_IDinfo.iDev=GetDevID(pAcsCmd->m_ITPDeviceID.m_s16DeviceMain);
	 pkd_IDinfo.iDsp=pAcsCmd->m_ITPDeviceID.m_s8ModuleID;
	 pkd_IDinfo.iChnl=pAcsCmd->m_ITPDeviceID.m_s16ChannelID;
     return 0;	 
}

//----------------------Evt Reader--------------------------------------//
CString ReadPacket::GetString_EventType ( )
{

	static char	TmpStr[40];

	switch ( pAcsEvt->m_s32EventType )
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
	case XMS_EVT_LICENSE_QUERY:     return "LICENSE_QUERY";
	case XMS_EVT_ACS_TIMER:         return "ACS_TIMER";

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
	case XMS_EVT_SENDSIGMSG:		return "SENDSIGMSG";		
	case XMS_EVT_SIGMON:       		return "SIGMON";
	case XMS_EVT_3G324M:       		return "3G324M";
	case XMS_EVT_CTX_REG:           return "CTX_REG";
	case XMS_EVT_CTX_LINK:          return "CTX_LINK";
	case XMS_EVT_CTX_APPDATA:       return "CTX_APPDATA";

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
	case XMS_EVT_3GPP_INIT_INDEX:   return "3GPP_INIT_INDEX";
	case XMS_EVT_3GPP_BUILD_INDEX:  return "3GPP_BUILD_INDEX";


	case XMS_EVT_SENDFAX:			return "SENDFAX";
	case XMS_EVT_RECVFAX:			return "RECVFAX";
	
	case XMS_EVT_SENDIODATA:		return "SENDIODATA";
	case XMS_EVT_RECVIODATA:		return "RECVIODATA";

	case XMS_EVT_CHGMONITORFILTER:	return "CHGMONITORFILTER";
	case XMS_EVT_DEV_TIMER:			return  "DEV_TIMER";

	case XMS_EVT_UNIFAILURE:		return "UNIFAILURE";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", pAcsEvt->m_s32EventType );
	return TmpStr;
}

CString ReadPacket::GetString_DeviceMain ( DJ_S32	s32DeviceMain )
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
	case XMS_DEVMAIN_CTX:               return "CTX";
	}

	sprintf ( TmpStr, "UNKNOWN(0x%X)", s32DeviceMain );
	return TmpStr;
}

CString ReadPacket::GetString_DeviceSub (DJ_S32	s32DeviceMain, DJ_S32	s32DeviceSub )
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
				case	XMS_DEVSUB_BASE              :         return"Base";  /*Base*/
				case	XMS_DEVSUB_ANALOG_TRUNK      :         return"Analog Trunk"; /*Analog Trunk*/
				case	XMS_DEVSUB_ANALOG_USER       :         return"Analog User"; /*Analog User*/
				case	XMS_DEVSUB_ANALOG_HIZ        :         return"Analog Hiz"; /*Analog Hiz*/
				case	XMS_DEVSUB_ANALOG_EMPTY      :         return"Analog Empty"; /*Analog Empty*/
				case	XMS_DEVSUB_E1_PCM            :         return"E1 PCM"; /*E1 PCM*/
				case	XMS_DEVSUB_E1_CAS            :         return"E1 CAS"; /*E1 CAS*/
				case	XMS_DEVSUB_E1_DSS1           :         return"E1 DSS1"; /*E1 DSS1*/
				case	XMS_DEVSUB_E1_SS7_TUP        :         return"E1 SS7 TUP"; /*E1 SS7 TUP*/
				case	XMS_DEVSUB_E1_SS7_ISUP       :         return"E1 SS7 ISUP"; /*E1 SS7 ISUP*/
				case	XMS_DEVSUB_ANALOG_VOC2W		 :         return"Analog voc 2 wire"; /*Voice only 2 wire*/
				case    XMS_DEVSUB_ANALOG_VOC4W		 :         return"Analog voc 4 wire"; /*Voice only 4 wire*/
				case    XMS_DEVSUB_ANALOG_EM		 :         return"Analog EM"; /*EM control module*/
				case    XMS_DEVSUB_ANALOG_MAG		 :         return"Analog magnetic"; /*magnetic module*/
				case    XMS_DEVSUB_E1_DCH			 :         return"Dchannel"; /*E1 6D25B's Dchannel*/
				case    XMS_DEVSUB_E1_BCH			 :         return"Bchannel"; /*E1 6D25B's Bchannel*/
				case    XMS_DEVSUB_UNUSABLE			 :         return"unusable"; /*unusable timeslot; such as ts0 of E1*/
				case	XMS_DEVSUB_HIZ_CAS			 :         return"Hiz Cas"; /*Hiz Cas*/
				case	XMS_DEVSUB_HIZ_PRI			 :         return"Hiz Pri"; /*Hiz Pri*/
				case	XMS_DEVSUB_HIZ_SS7			 :         return"Hiz ss7"; /*Hiz ss7*/
				case	XMS_DEVSUB_HIZ_PRI_LINK		 :         return"Hiz pri link"; /*Hiz pri link*/
				case	XMS_DEVSUB_HIZ_SS7_64K_LINK  :		   return"Hiz ss7 64link"; /*Hiz ss7 64k link*/
				case	XMS_DEVSUB_HIZ_SS7_2M_LINK   : 		   return"Hiz ss7 2Mlink"; /*Hiz ss7 2M link*/
				case	XMS_DEVSUB_SS7_LINK		     :         return"ss7 link"; /*ss7 link*/
				case	XMS_DEVSUB_LINESIDE		     :         return"LineSide"; /*LineSide E1*/
				case	XMS_DEVSUB_ANALOG_REC	     :         return"Analog Rec"; /*Analog phone Record*/
				case	XMS_DEVSUB_DIGITAL_REC	     :         return"Digital Rec"; /*Digital phone Record*/
				case	XMS_DEVSUB_HIZ_HDLC_N64K_LINK:         return"Hiz n*64K"; /*Hiz n*64K */
				case	XMS_DEVSUB_3G_324            :         return"3G324"; /*3G324*/
			}
		}
	}

	if( s32DeviceMain == XMS_DEVMAIN_VOIP )
		sprintf ( TmpStr, "Non-SubDev");
	else
		sprintf ( TmpStr, "UNKNOWN(0x%X)", s32DeviceSub );

	return TmpStr;
}

CString ReadPacket::GetString_DeviceAll (DeviceID_t *pDevID)
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

//----------------------Evt Reader End--------------------------------------//
//----------------------API Reader Start------------------------------------//
CString ReadPacket::GetString_APIType ()
{
	static char	TmpStr[40];

	switch ( pAcsCmd->m_s32CmdType )
	{
	case  ACS_CMD_TYPE_GETDEVLIST:               return "XMS_acsGetDeviceList";
	case  ACS_CMD_TYPE_OPENDEVICE:               return "XMS_ctsOpenDevice";
	case  ACS_CMD_TYPE_CLOSEDEVICE:              return "XMS_ctsCloseDevice";
	case  ACS_CMD_TYPE_RESETDEVICE:              return "XMS_ctsResetDevice";
	case  ACS_CMD_TYPE_GETDEVICESTATE:           return "XMS_ctsGetDevState";
	case  ACS_CMD_TYPE_SETDEVGROUP:	             return "XMS_ctsSetDevGroup";
	case  ACS_CMD_TYPE_SETDEVPARAM:	             return "XMS_ctsSetParam";
	case  ACS_CMD_TYPE_GETDEVPARAM:	             return "XMS_ctsGetParam";
	case  ACS_CMD_TYPE_MAKECALLOUT:              return "XMS_ctsMakeCallOut";
	case  ACS_CMD_TYPE_ALERTCALL:                return "XMS_ctsAlertCall";
	                                                                  
	case  ACS_CMD_TYPE_ANSWERCALLIN:	         return "XMS_ctsAnswerCallIn";
	case  ACS_CMD_TYPE_LINKDEV:                  return "XMS_ctsLinkDevice";
	case  ACS_CMD_TYPE_UNLINKDEV:	             return "XMS_ctsUnlinkDevice";
	case  ACS_CMD_TYPE_CLEARCALL:                return "XMS_ctsClearCall";
	case  ACS_CMD_TYPE_JOINTOCONF:               return "XMS_ctsJoinToConf";
	case  ACS_CMD_TYPE_LEAVEFROMCONF:            return "XMS_ctsLeaveFromConf";
	case  ACS_CMD_TYPE_CLEARCONF:                return "XMS_ctsClearConf";
	case  ACS_CMD_TYPE_PLAYFILE:                 return "XMS_ctsPlay";
	case  ACS_CMD_TYPE_INITPLAYINDEX:            return "XMS_ctsInitPlayIndex";
	case  ACS_CMD_TYPE_BUILDPLAYINDEX:           return "XMS_ctsBuildPlayIndex";
	                                                                  
	case  ACS_CMD_TYPE_CONTROLPLAY:              return "XMS_ctsControlPlay";
	case  ACS_CMD_TYPE_RECORDFILE:               return "XMS_ctsRecord";
	case  ACS_CMD_TYPE_CONTROLRECORD:            return "XMS_ctsControlRecord";
	case  ACS_CMD_TYPE_SENDFAX:                  return "XMS_ctsSendFax";
	case  ACS_CMD_TYPE_STOPSENDFAX:              return "XMS_ctsStopSendFax";
	case  ACS_CMD_TYPE_RECVFAX:                  return "XMS_ctsReceiveFax";
	case  ACS_CMD_TYPE_STOPRECVFAX:              return "XMS_ctsStopReceiveFax";
	case  ACS_CMD_TYPE_CHANGEMONITORFILTER:	     return "XMS_ctsChangeMonitorFilter";
	case  ACS_CMD_TYPE_SENDIO:	                 return "XMS_ctsSendIOData";
	case  ACS_CMD_TYPE_SENDSIGMSG:               return "XMS_ctsSendSignalMsg";
	                                                                  
	case  ACS_CMD_TYPE_RECORDCSP:	             return "XMS_ctsRecordCSP";
	case  ACS_CMD_TYPE_DBGON:                    return "OPEN_STREAM";
	case  ACS_CMD_TYPE_DBGOFF:                   return "OPEN_STREAM";
	case  ACS_CMD_TYPE_PLAY_3GPP:                return "XMS_cts3gppPlay";
	case  ACS_CMD_TYPE_CONTROL_3GPP_PLAY:        return "XMS_ctsControl3gppPlay";
	case  ACS_CMD_TYPE_RECORD_3GPP:              return "XMS_cts3gppRecord";
	case  ACS_CMD_TYPE_CONTROL_3GPP_RECORD:    	 return "XMS_ctsControl3gppRecord";
	case  ACS_CMD_TYPE_PLAYCSP:                  return "XMS_ctsPlayCSP";
	case  ACS_CMD_TYPE_SENDPLAYCSP:              return "XMS_ctsSendCSPData";
	case  ACS_CMD_TYPE_QUERYLIC:                 return "XMS_acsQueryLicense";
	                                                                  
	case  ACS_CMD_TYPE_CTXREG:                   return "XMS_ctxRegister";
	case  ACS_CMD_TYPE_CTXLINK:                  return "XMS_ctxLink";
	case  ACS_CMD_TYPE_CTXSENDAPPDATA:           return "XMS_ctxSendAppData";
	case  ACS_CMD_TYPE_INIT3GPPINDEX:            return "XMS_ctsInitPlay3gppIndex";
	case  ACS_CMD_TYPE_BUILD3GPPINDEX:           return "XMS_ctsBuildPlay3gppIndex";

	}
	sprintf ( TmpStr, "UNKNOWN(0x%X)", pAcsCmd->m_s32CmdType );
	return TmpStr;
}
int ReadPacket::GetAPIID()
{
    switch(pAcsCmd->m_s32CmdType)
    {
   	case  ACS_CMD_TYPE_GETDEVLIST:               return 1; 
	case  ACS_CMD_TYPE_OPENDEVICE:               return 2; 
	case  ACS_CMD_TYPE_CLOSEDEVICE:              return 3; 
	case  ACS_CMD_TYPE_RESETDEVICE:              return 4; 
	case  ACS_CMD_TYPE_GETDEVICESTATE:           return 5; 
	case  ACS_CMD_TYPE_SETDEVGROUP:	             return 6; 
	case  ACS_CMD_TYPE_SETDEVPARAM:	             return 7; 
	case  ACS_CMD_TYPE_GETDEVPARAM:	             return 8; 
	case  ACS_CMD_TYPE_MAKECALLOUT:              return 9; 
	case  ACS_CMD_TYPE_ALERTCALL:                return 10;
		
	case  ACS_CMD_TYPE_ANSWERCALLIN:	           return 11;
	case  ACS_CMD_TYPE_LINKDEV:                  return 12;
	case  ACS_CMD_TYPE_UNLINKDEV:	               return 13;
	case  ACS_CMD_TYPE_CLEARCALL:                return 14;
	case  ACS_CMD_TYPE_JOINTOCONF:               return 15;
	case  ACS_CMD_TYPE_LEAVEFROMCONF:            return 16;
	case  ACS_CMD_TYPE_CLEARCONF:                return 17;
	case  ACS_CMD_TYPE_PLAYFILE:                 return 18;
	case  ACS_CMD_TYPE_INITPLAYINDEX:            return 19;
	case  ACS_CMD_TYPE_BUILDPLAYINDEX:           return 20;
		
	case  ACS_CMD_TYPE_CONTROLPLAY:              return 21;
	case  ACS_CMD_TYPE_RECORDFILE:               return 22;
	case  ACS_CMD_TYPE_CONTROLRECORD:            return 23;
	case  ACS_CMD_TYPE_SENDFAX:                  return 24;
	case  ACS_CMD_TYPE_STOPSENDFAX:              return 25;
	case  ACS_CMD_TYPE_RECVFAX:                  return 26;
	case  ACS_CMD_TYPE_STOPRECVFAX:              return 27;
	case  ACS_CMD_TYPE_CHANGEMONITORFILTER:	     return 28;
	case  ACS_CMD_TYPE_SENDIO:	                 return 29;
	case  ACS_CMD_TYPE_SENDSIGMSG:               return 30;
		
	case  ACS_CMD_TYPE_RECORDCSP:	             return 31;
	case  ACS_CMD_TYPE_DBGON:                    return 32;
	case  ACS_CMD_TYPE_DBGOFF:                   return 33;
	case  ACS_CMD_TYPE_PLAY_3GPP:                return 34;
	case  ACS_CMD_TYPE_CONTROL_3GPP_PLAY:        return 35;
	case  ACS_CMD_TYPE_RECORD_3GPP:              return 36;
	case  ACS_CMD_TYPE_CONTROL_3GPP_RECORD:    	 return 37;
	case  ACS_CMD_TYPE_PLAYCSP:                  return 38;
	case  ACS_CMD_TYPE_SENDPLAYCSP:              return 39;
	case  ACS_CMD_TYPE_QUERYLIC:                 return 40;
		
	case  ACS_CMD_TYPE_CTXREG:                   return 41;
	case  ACS_CMD_TYPE_CTXLINK:                  return 42;
	case  ACS_CMD_TYPE_CTXSENDAPPDATA:           return 43;
	case  ACS_CMD_TYPE_INIT3GPPINDEX:            return 44;
	case  ACS_CMD_TYPE_BUILD3GPPINDEX:           return 45;
    }

   return 0;
}
int ReadPacket::GetEvtID()
{
    switch(pAcsEvt->m_s32EventType)
    {
   	case XMS_EVT_OPEN_STREAM:		      return 1;            
	case XMS_EVT_QUERY_DEVICE:		    return 2;            
	case XMS_EVT_QUERY_DEVICE_END:	  return 3;            
	case XMS_EVT_OPEN_DEVICE:		      return 4;            
	case XMS_EVT_CLOSE_DEVICE:		    return 5;            
	case XMS_EVT_RESET_DEVICE:		    return 6;            
	case XMS_EVT_DEVICESTATE:		      return 7;            
	case XMS_EVT_SETDEV_GROUP:		    return 8;            
	case XMS_EVT_SETPARAM:			      return 9;            
	case XMS_EVT_GETPARAM:			      return 10;           
	case XMS_EVT_QUERY_ONE_DSP_START:	return 11;           
	case XMS_EVT_QUERY_ONE_DSP_END:		return 12;           
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_START:	return 13;   
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:		return 14;   
	case XMS_EVT_OPEN_STREAM_EXT:	    return 15;           
	case XMS_EVT_LICENSE_QUERY:       return 16;           
	case XMS_EVT_ACS_TIMER:           return 17;           
		
	case XMS_EVT_CALLOUT:			        return 18;           
	case XMS_EVT_CALLIN:			        return 19;           
	case XMS_EVT_ALERTCALL:			      return 20;           
	case XMS_EVT_ANSWERCALL:		      return 21;           
	case XMS_EVT_LINKDEVICE:		      return 22;           
	case XMS_EVT_UNLINKDEVICE:		    return 23;           
	case XMS_EVT_CLEARCALL:			      return 24;           
	case XMS_EVT_ANALOG_INTERFACE:	  return 25;           
	case XMS_EVT_CAS_MFC_START:		    return 26;           
	case XMS_EVT_CAS_MFC_END:		      return 27;           
	case XMS_EVT_SENDSIGMSG:		      return 28;           
	case XMS_EVT_SIGMON:       		    return 29;           
	case XMS_EVT_3G324M:       		    return 30;           
	case XMS_EVT_CTX_REG:             return 31;           
	case XMS_EVT_CTX_LINK:            return 32;           
	case XMS_EVT_CTX_APPDATA:         return 33;           
		
	case XMS_EVT_JOINTOCONF:		      return 34;           
	case XMS_EVT_LEAVEFROMCONF:	      return 35;           
	case XMS_EVT_CLEARCONF:			      return 36;           
		
		
	case XMS_EVT_PLAY:				        return 37;           
	case XMS_EVT_INITINDEX:		        return 38;           
	case XMS_EVT_BUILDINDEX:	        return 39;           
	case XMS_EVT_CONTROLPLAY:	        return 40;           
	case XMS_EVT_RECORD:			        return 41;           
	case XMS_EVT_CONTROLRECORD:		    return 42;           
	case XMS_EVT_RECORDCSP:			      return 43;           
	case XMS_EVT_CONTROLRECORDCSP:    return 44;           
	case XMS_EVT_3GPP_PLAY:			      return 45;           
	case XMS_EVT_3GPP_CONTROLPLAY:	  return 46;           
	case XMS_EVT_3GPP_RECORD:		      return 47;           
	case XMS_EVT_3GPP_CONTROLRECORD:  return 48;           
	case XMS_EVT_PLAYCSPREQ:		      return 49;           
	case XMS_EVT_3GPP_INIT_INDEX:         return 50;           
	case XMS_EVT_3GPP_BUILD_INDEX:        return 51;           
		
	case XMS_EVT_SENDFAX:			        return 52;           
	case XMS_EVT_RECVFAX:			        return 53;           
		
	case XMS_EVT_SENDIODATA:		      return 54;           
	case XMS_EVT_RECVIODATA:		      return 55;           
		
	case XMS_EVT_CHGMONITORFILTER:	      return 56;           
	case XMS_EVT_DEV_TIMER:			      return 57;           
		
	case XMS_EVT_UNIFAILURE:		      return 58;           
    }

    return 0;
}
int ReadPacket::GetDevID(DJ_S32	s32DeviceMain)
{
	switch(s32DeviceMain)
	{
	case 	XMS_DEVMAIN_VOICE       :  return 1;
	case 	XMS_DEVMAIN_FAX         :  return 2;
	case 	XMS_DEVMAIN_INTERFACE_CH:  return 3;
	case 	XMS_DEVMAIN_BOARD       :  return 4;
	case 	XMS_DEVMAIN_VOIP        :  return 5;
	case 	XMS_DEVMAIN_CONFERENCE  :  return 6;
	case 	XMS_DEVMAIN_CTX         :  return 7;

	}
	return 0;
}
