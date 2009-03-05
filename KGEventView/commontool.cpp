#include "StdAfx.h"
#include "commontool.h"

CString    m_CStrreturn;

CString  GetString_ParaType(DeviceID_t *pDev, DJ_U16 Type)
{
	char  tempstr[100];
    if (XMS_DEVMAIN_BOARD==pDev->m_s16DeviceMain)
    {
		switch(Type)
		{
		case  BOARD_PARAM_GETBOARDINFO:  return"PARAM GETBOARD INFO";
		case  BOARD_PARAM_SETFSK:        return"PARAM SET FSK";
		case  BOARD_PARAM_SETFLASH:      return"PARAM SET FLASH";
		case  BOARD_PARAM_SETGTDFREQ:    return"PARAM SET GTD FREQ";
		case  BOARD_PARAM_SETGTDTONE:    return"PARAM SET GTD TONE";
		default:
			sprintf(tempstr,"Unknown 0x%X",Type);
		}
    }
    	
	if (XMS_DEVMAIN_VOICE==pDev->m_s16DeviceMain)
	{
      if (XMS_DEVSUB_MEDIA_VOC==pDev->m_s16DeviceSub)
      {
		switch(Type)
		{
		case	VOC_PARAM_UNIPARAM    : return"set universal vocice parameters"; // set universal vocice parameters. 
		case	VOC_PARAM_BREAKEN_CALL: return"set breaken call";                // for keyInterrupet                
		case	VOC_PARAM_3GFAXTONE   : return"set 3G faxtone";                  // for 3G FaxTone Eanble or Disable 
		default:
			sprintf(tempstr,"Unknown 0x%X",Type);
		}
      }
	  if (XMS_DEVSUB_MEDIA_324==pDev->m_s16DeviceSub)
	  {
        switch(Type)
        {
		case		H324_PARAM_UNIPARAM:    return"set universal 324 parameters";   
		case		H324_PARAM_MEDIA:       return"set 324 Media";                  
		case		H324_PARAM_CH:          return"set 324 Chnl";                   
		default:
			sprintf(tempstr,"Unknown 0x%X",Type);
        }
	  }
	}
	
	if (XMS_DEVMAIN_CONFERENCE==pDev->m_s16DeviceMain)
	{
        if(0==Type) return"set universal parameters";
        else 	sprintf(tempstr,"Unknown 0x%X",Type);
	}
	
	if (XMS_DEVMAIN_VOIP==pDev->m_s16DeviceMain)
	{
       switch(Type)
       {
	   case	VOIP_PARAM_ACCESS    :    return"PARAM ACCESS";    
	   case	VOIP_PARAM_VOICE     :    return"PARAM VOICE";     
	   case	VOIP_PARAM_RTPSESSION:    return"PARAM RTPSESSION";
	   case	VOIP_PARAM_RTPADDR   :    return"PARAM RTPADDR";   
	   case	VOIP_PARAM_RTCPADDR  :    return"PARAM RTCPADDR";  
	   case	VOIP_PARAM_RTPCODEC  :    return"PARAM RTPCODEC";  
	   case	VOIP_PARAM_RTPMIXER  :    return"PARAM RTPMIXER";  
	   default:
			sprintf(tempstr,"Unknown 0x%X",Type);
       }

	}
	
	if (XMS_DEVMAIN_INTERFACE_CH==pDev->m_s16DeviceMain)
	{
		if (XMS_DEVSUB_E1_CAS==pDev->m_s16DeviceSub)
		{
			switch(Type)
			{
			case  CAS_PARAM_UNIPARAM:  return"PARAM UNIPARAM";
			case  CAS_PARAM_MFCTYPE:   return"PARAM MFCTYPE";
			default:
				sprintf(tempstr,"Unknown 0x%X",Type);
			}
		}
		if (XMS_DEVSUB_E1_SS7_TUP==pDev->m_s16DeviceSub)
		{
			switch(Type)
			{
			case  TUP_SP_AccMessageIndicator      :     return"ACC消息指示码";      // ACC消息指示码 
			case  TUP_SP_AcmMessageIndicator      :     return"ACM消息指示码";      // ACM消息指示码 
			case  TUP_SP_CalledPartAddress        :     return"被叫用户号码";       // 被叫用户号码  
			case  TUP_SP_CallingLineAddress       :     return"主叫用户号码";       // 主叫用户号码  
			case  TUP_SP_CallingPartyCategory     :     return"主叫用户类别";       // 主叫用户类别  
			case  TUP_SP_ConnectionNatureIndicator:     return"接续性质指示码";     // 接续性质指示码
			case  TUP_SP_EumOctetIndicator        :     return"EUM消息八位组";      // EUM消息八位组 
			case  TUP_SP_EumSignallingPointCode   :     return"EUM消息信令点";      // EUM消息信令点 
			case  TUP_SP_ForwardCallInformation   :     return"前向呼叫信息";       // 前向呼叫信息  
			case  TUP_SP_GrqRequestTypeIndicator  :     return"GRQ请求指示码";      // GRQ请求指示码 
			case  TUP_SP_GsmResponseTypeIndicator :     return"GSM响应指示码";      // GSM响应指示码 
			case  TUP_SP_IaiFirstIndicator        :     return"IAI第一表示语";      // IAI第一表示语 
			case  TUP_SP_OriginalCalledAddress    :     return"原被叫用户号码";     // 原被叫用户号码
			case  TUP_SP_RangeStatus              :     return"范围与状态";         // 范围与状态    
			case  TUP_SP_SubsequentAddress        :     return"后续地址";           // 后续地址      
			default:
				sprintf(tempstr,"Unknown 0x%X",Type);
		}
		if (XMS_DEVSUB_E1_SS7_ISUP==pDev->m_s16DeviceSub)
		{
			switch(Type)
			{
			case	ISUP_SP_AccessDeliveryInformation         :return" 接入传递信息";                        // 接入传递信息                       
			case	ISUP_SP_AccessTransport                   :return" 接入传送";                            // 接入传送                           
			case	ISUP_SP_AutomaticCongestionLevel          :return" 自动拥塞级";                          // 自动拥塞级                         
			case	ISUP_SP_BackwardCallIndicator             :return" 后向呼叫指示码";                      // 后向呼叫指示码                     
			case	ISUP_SP_CallDiversionInformation          :return" 呼叫转移信息";                        // 呼叫转移信息                       
			case	ISUP_SP_CallHistoryInformation            :return" 呼叫历史信息";                        // 呼叫历史信息                       
			case	ISUP_SP_CallReference                     :return" 呼叫参考";                            // 呼叫参考                           
			case	ISUP_SP_CalledPartNumber                  :return" 被叫用户号码";                        // 被叫用户号码                       
			case	ISUP_SP_CallingPartNumber                 :return" 主叫用户号码";                        // 主叫用户号码                       
			case	ISUP_SP_CallingPartCategory               :return" 主叫用户类别";                        // 主叫用户类别                       
			case	ISUP_SP_CauseIndicator                    :return" 原因指示码";                          // 原因指示码                         
			case	ISUP_SP_CircuitGroupSupervisionIndicator  :return" 电路群监视消息类型指示码";            // 电路群监视消息类型指示码           
			case	ISUP_SP_CircuitStateIndicator             :return" 电路状态指示码";                      // 电路状态指示码                     
			case	ISUP_SP_CUGInterlockCode                  :return" 闭合用户群相关码";                    // 闭合用户群相关码                   
			case	ISUP_SP_ConnectedNumber                   :return" 接续的号码";                          // 接续的号码                         
			case	ISUP_SP_ConnectionRequest                 :return" 接续请求";                            // 接续请求                           
			case	ISUP_SP_ContitnuityIndicator              :return" 导通指示码";                          // 导通指示码                         
			case	ISUP_SP_EchoControlInformation            :return" 回声控制信息";                        // 回声控制信息                       
			case	ISUP_SP_EndOptionalParameter              :return" 任选参数尾指示码";                    // 任选参数尾指示码                   
			case	ISUP_SP_EventInformation                  :return" 事件信息";                            // 事件信息                           
			case	ISUP_SP_FacilityIndicator                 :return" 功能指示码";                          // 功能指示码                         
			case	ISUP_SP_ForwardCallIndicator              :return" 前向呼叫指示码";                      // 前向呼叫指示码                     
			case	ISUP_SP_GenericDigit                      :return" 类属数字";                            // 类属数字                           
			case	ISUP_SP_GenericNotifyIndicator            :return" 类属通知指示码";                      // 类属通知指示码                     
			case	ISUP_SP_GenericNumber                     :return" 类属号码";                            // 类属号码                           
			case	ISUP_SP_GenericReference                  :return" 类属参考";                            // 类属参考                           
			case	ISUP_SP_InformationIndicator              :return" 信息指示码";                          // 信息指示码                         
			case	ISUP_SP_InformationRequestIndicator       :return" 信息请求指示码";                      // 信息请求指示码                     
			case	ISUP_SP_LocationNumber                    :return" 定位号码";                            // 定位号码                           
			case	ISUP_SP_MCIDRequestIndicator              :return" MCID请求指示码";                      // MCID请求指示码                     
			case	ISUP_SP_MCIDResponseIndicator             :return" MCID响应指示码";                      // MCID响应指示码                     
			case	ISUP_SP_MsgCompatibilityInformation       :return" 消息兼容性信息";                      // 消息兼容性信息                     
			case	ISUP_SP_MlppPrecedence                    :return" MLPP优先";                            // MLPP优先                           
			case	ISUP_SP_NatureOfConnectionIndicator       :return" 接续性质指示码";                      // 接续性质指示码                     
			case	ISUP_SP_NetworkSpecificFacility           :return" 网络特定功能";                        // 网络特定功能                       
			case	ISUP_SP_OptBackwardCallIndicator          :return" 任选后向呼叫指示码";                  // 任选后向呼叫指示码                 
			case	ISUP_SP_OptForwardCallIndicator           :return" 任选前向呼叫指示码";                  // 任选前向呼叫指示码                 
			case	ISUP_SP_OriginalCalledNumber              :return" 原来的被叫号码";                      // 原来的被叫号码                     
			case	ISUP_SP_OriginationISCPointCode           :return" 始发国际交换中心(ISC)的点码";         // 始发国际交换中心(ISC)的点码        
			case	ISUP_SP_ParamCompatibilityInformation     :return" 参数兼容性信息";                      // 参数兼容性信息                     
			case	ISUP_SP_PropagationDelayCounter           :return" 传播延迟计数器";                      // 传播延迟计数器                     
			case	ISUP_SP_RangeState                        :return" 范围和状态";                          // 范围和状态                         
			case	ISUP_SP_RedirectingNumber                 :return" 改发号码";                            // 改发号码                           
			case	ISUP_SP_RedirectionInformation            :return" 改发信息";                            // 改发信息                           
			case	ISUP_SP_RedirectionNumber                 :return" 改发号码";                            // 改发号码                           
			case	ISUP_SP_RedirectionRestricition           :return" 改发号码限制参数";                    // 改发号码限制参数                   
			case	ISUP_SP_RemoteOperation                   :return" 远端操作";                            // 远端操作                           
			case	ISUP_SP_ServiceActivation                 :return" 业务激活";                            // 业务激活                           
			case	ISUP_SP_SignalPointCode                   :return" 信令点编码";                          // 信令点编码                         
			case	ISUP_SP_SubsequentNumber                  :return" 后续号码";                            // 后续号码                           
			case	ISUP_SP_SuspendResumeIndicator            :return" 暂停/恢复指示码";                     // 暂停/恢复指示码                    
			case	ISUP_SP_TransitNetworkSelection           :return" 汇接网络选择";                        // 汇接网络选择                       
			case	ISUP_SP_TransmissionMediumRequirement     :return" 传输媒介要求";                        // 传输媒介要求                       
			case	ISUP_SP_TransmissionMediumRequirementPrime:return" 传输媒介要求(基本的)";                // 传输媒介要求(基本的)               
			case	ISUP_SP_TransmissionMediumUsed            :return" 使用的传输媒介";                      // 使用的传输媒介                     
			case	ISUP_SP_UserServiceInformation            :return" 用户业务信息";                        // 用户业务信息                       
			case	ISUP_SP_UserServiceInformationPrime       :return" 用户业务信息(基本的)";                // 用户业务信息(基本的)               
			case	ISUP_SP_UserTeleserviceInformation        :return" 用户终端业务信息";                    // 用户终端业务信息                   
			case	ISUP_SP_UserToUserIndicator               :return" 用户至用户指示码";                    // 用户至用户指示码                   
			case	ISUP_SP_UserToUserInformation             :return" 用户至用户信息";                      // 用户至用户信息                     
			default:
				sprintf(tempstr,"Unknown 0x%X",Type);
			}
		}
		if (XMS_DEVSUB_ANALOG_USER==pDev->m_s16DeviceSub)
		{
			switch(Type)
			{
			case ANALOGUSER_PARAM_UNIPARAM:    return"PARAM UNIPARAM";
			case ANALOGUSER_PARAM_UNBINDVOC:   return"PARAM UNBINDVOC";
			default:
			sprintf(tempstr,"Unknown 0x%X",Type);
			}
		}
        if (XMS_DEVSUB_ANALOG_TRUNK==pDev->m_s16DeviceSub)
        {
			switch(Type)
			{
			case	ANALOGTRUNK_PARAM_UNIPARAM     : return"PARAM UNIPARAM";      
			case	ANALOGTRUNK_PARAM_SETFLASH     : return"PARAM SETFLASH";      
			case	ANALOGTRUNK_PARAM_UNSETUNIPARAM: return"PARAM UNSETUNIPARAM"; 
			case	ANALOGTRUNK_PARAM_UNBINDVOC    : return"PARAM UNBINDVOC";  
			default:
			sprintf(tempstr,"Unknown 0x%X",Type);
			}

        }

		else
            sprintf(tempstr,"Unknown 0x%X",Type);
	}
	}
         return tempstr;	
}
CString GetString_DeviceMain ( DJ_S32	s32DeviceMain )
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

CString GetString_DeviceSub (DJ_S32	s32DeviceMain, DJ_S32	s32DeviceSub )
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

CString GetString_DeviceAll (DeviceID_t *pDevID)
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
CString    GetString_MediaSrcMod (DJ_U8 SrcMod)
{
	static char	TmpStr[40];

	switch(SrcMod)
	{
	case	XMS_SRC_8K     :              return"8K";       
	case	XMS_SRC_6K     :              return"6K";       
	case	XMS_SRC_GTG    :              return"GTG";      
	case	XMS_SRC_FSK    :              return"FSK";      
	case	XMS_SRC_RTP    :              return"RTP";      
	case	XMS_SRC_FAX    :              return"FAX";      
	case	XMS_SRC_3GVIDEO:              return"3GVIDEO";  
	case	XMS_SRC_11K    :              return"11K";      
	}
	sprintf(TmpStr,"Unknown Type0x%X",SrcMod);

	return TmpStr;
}
CString    GetString_MediaDecodeType(DJ_U8 DecType)
{
	static char	TmpStr[40];
	
	switch(DecType)
	{
		case		XMS_Alaw_type   :             return"Alaw type";   
		case		XMS_Ulaw_type   :             return"Ulaw type";   
		case		XMS_Vox_type    :             return"Vox type";    
		case		XMS_Linear_8bit :             return"Linear 8bit"; 
		case		XMS_Linear_16bit:             return"Linear 16bit";
		case		XMS_Amr_type    :             return"Amr type";    
		case		XMS_G723_type   :             return"G723 type";   
		case		XMS_CODE_STREAM :             return"CODE STREAM"; 
	}
	sprintf(TmpStr,"Unknown Type0x%X",DecType);
	
	return TmpStr;
}

CString    GetString_PlayCtrlType(DJ_U16 playType)
{
	static char	TmpStr[40];
	
	switch(playType)
	{
		case	XMS_STOP_PLAY             :       return"STOP PLAY";               
		case	XMS_PAUSE_PLAY            :       return"PAUSE PLAY";              
		case	XMS_RESUME_PLAY           :       return"RESUME PLAY";             
		case	XMS_STEP_BACKWARD_PLAY    :       return"STEP BACKWARD PLAY";      
		case	XMS_STEP_FORWARD_PLAY     :       return"STEP FORWARD PLAY";       
		case	XMS_CSP_STOP_PLAY         :       return"CSP STOP PLAY";           
		case	XMS_CSP_PAUSE_PLAY        :       return"CSP PAUSE PLAY";          
		case	XMS_CSP_RESUME_PLAY       :       return"CSP RESUME PLAY";         
		case	XMS_CSP_STEP_BACKWARD_PLAY:       return"CSP STEP BACKWARD PLAY";  
		case	XMS_CSP_STEP_FORWARD_PLAY :       return"CSP STEP FORWARD PLAY";  
	}
	sprintf(TmpStr,"Unknown Type0x%X",playType);
	
	return TmpStr;
}
CString    GetString_MixerType(DJ_U8 Type)
{
	static char	TmpStr[40];
	
	switch(Type)
	{
		case	XMS_MIXER_FROM_NULL	:              return"FROM NULL";		
		case	XMS_MIXER_FROM_INPUT:              return"FROM INPUT";	
		case	XMS_MIXER_FROM_PLAY	:              return"FROM PLAY";		
		case	XMS_MIXER_FROM_CONF :              return"FROM CONF";		
		case	XMS_MIXER_FROM_IP	:              return"FROM IP";	    
		case	XMS_MIXER_FROM_324	:              return"FROM 324";		
	}
	sprintf(TmpStr,"Unknown Type0x%X",Type);
	
	return TmpStr;
}
CString    GetString_MediaStopMod(DJ_U32 Type)
{
	static char	TmpStr[40];
	
	switch(Type)
	{
	case	XMS_Normal_Stop:                  return"Normal Stop"; 
	case	XMS_Single_Code:                  return"Single Code";	
	case	XMS_Any_Code   :                  return"Any Code";  
	}
	sprintf(TmpStr,"Unknown Type0x%X",Type);
	
	return TmpStr;
}
CString    GetString_RecordCtrlType(DJ_U32 RecordType)
{
	static char	TmpStr[40];
	
	switch(RecordType)
	{
	case	XMS_STOP_RECORD             :        return"STOP RECORD";              
	case	XMS_PAUSE_RECORD            :        return"PAUSE RECORD";             
	case	XMS_RESUME_RECORD           :        return"RESUME RECORD";            
	case	XMS_STEP_BACKWARD_RECORD    :        return"STEP BACKWARD RECORD";     
	case	XMS_CSP_STOP_RECORD         :        return"CSP STOP RECORD";          
	case	XMS_CSP_PAUSE_RECORD        :        return"CSP PAUSE RECORD";         
	case	XMS_CSP_RESUME_RECORD       :        return"CSP RESUME RECORD";        
	case	XMS_CSP_STEP_BACKWARD_RECORD:        return"CSP STEP BACKWARD RECORD"; 

	}
	sprintf(TmpStr,"Unknown Type0x%X",RecordType);
	
	return TmpStr;
}
CString    GetString_IOEvtMask(DJ_U32 EvtType)
{
	static char	TmpStr[40];
	
	switch(EvtType)
	{
		case	XMS_IOEVT_MASK_GTD :            return"MASK GTD";  
		case	XMS_IOEVT_MASK_FSK :            return"MASK FSK";  
		case	XMS_IOEVT_MASK_MR2F:            return"MASK MR2F"; 
		case	XMS_IOEVT_MASK_MR2B:            return"MASK MR2B"; 
	}
	sprintf(TmpStr,"Unknown Type0x%X",EvtType);
	
	return TmpStr;
}
CString    GetString_IOType(DeviceID_t *pDev, DJ_U16 IOType)
{
	static char	TmpStr[40];

	if(XMS_DEVMAIN_VOICE==pDev->m_s16DeviceMain)
	switch(IOType)
	{
		case	XMS_IO_TYPE_DTMF:                return"DTMF";  
		case	XMS_IO_TYPE_MR2F:                return"MR2F";  
		case	XMS_IO_TYPE_MR2B:                return"MR2B";  
		case	XMS_IO_TYPE_GTG :                return"GTG";   
		case	XMS_IO_TYPE_FSK :                return"FSK";   
		case	XMS_IO_TYPE_DPD :                return"DPD";   
		case	XMS_IO_TYPE_PVD :                return"PVD";   	
	}
	
	if (XMS_DEVMAIN_VOIP==pDev->m_s16DeviceMain)
	switch(IOType)
	{
		case	XMS_IO_TYPE_VOIP_H245UII    :   return"H245UII";        /* VoIP H.245 UserInputIndication Event */       
		case	XMS_IO_TYPE_VOIP_SIPINFO    :   return"SIPINFO";        /* VoIP SIP INFO Event */                        
		case	XMS_IO_TYPE_VOIP_RTPDTMF    :   return"RTPDTMF";        /* VoIP RTP DTMF Event */                        
		case	XMS_IO_TYPE_VOIP_RTPTONE    :   return"RTPTONE";        /* VoIP RTP TONE */                              
		case	XMS_IO_TYPE_VOIP_T38START   :   return"T38START";       /* VoIP T.38 fax start */                        
		case	XMS_IO_TYPE_VOIP_T38STOP    :   return"T38STOP";        /* VoIP T.38 fax stop */                         
		case	XMS_IO_TYPE_VOIP_REGSTATE   :   return"REGSTATE";       /* VoIP Registration State */                    
		case	XMS_IO_TYPE_VOIP_REGREQUEST :   return"REGREQUEST";     /* VoIP Registration Request */                  
		case	XMS_IO_TYPE_VOIP_REGRESPONSE:   return"REGRESPONSE";    /* VoIP Registration Response */                 
		case	XMS_IO_TYPE_VOIP_TRANSFER   :   return"TRANSFER";       /* VoIP Device Transfer */                       
	}
	
	sprintf(TmpStr,"Unknown Type0x%X",IOType);
	
	return TmpStr;
}
CString    GetString_CtxLinkType(DJ_U32 CtxLinkType)
{
	static char	TmpStr[40];
	
	switch(CtxLinkType)
	{
		case	CTX_LINK_OPTION_RESET		:           return"reset all link";       /*reset all link*/     
		case	CTX_LINK_OPTION_LINK		:           return"single link";          /*single link*/        
		case	CTX_LINK_OPTION_UNLINK		:           return"single unlink";        /*single unlink*/      
		case	CTX_LINK_OPTION_DUALLINK	:           return"dual link";            /*dual link*/          
		case	CTX_LINK_OPTION_DUALUNLINK  :           return"dual unlink";          /*dual unlink*/        
	}
	
	sprintf(TmpStr,"Unknown Type0x%X",CtxLinkType);
	
	return TmpStr;
}
CString HextoChars(DJ_S8 *msgbuf,int nNumRecv)
{
	DJ_S8 Temp[KEY_LEN]={0};
    DJ_S8* pTemp=Temp;

	for (int i = 0; i < nNumRecv; i++)
	{
		sprintf(pTemp, "%02X ", *msgbuf);
		msgbuf ++;
		pTemp += 3;
	}
	
	return Temp;
}
CString HextotoStr(DJ_Void *msgbuf,int nNumRecv)
{
	DJ_S8 Temp[KEY_LEN]={0};
    DJ_S8* pTemp=Temp;
	DJ_U8 * p = NULL;
    
	p = (DJ_U8 *)msgbuf;
	for (int i = 0; i < nNumRecv; i++)
	{
		sprintf(pTemp, "%02X ", *p);
		p ++;
		pTemp += 3;
	}

	return Temp;
}
int   HextoStr(DJ_U8 *msgbuf,DJ_S8 *Transbuf ,int nNumRecv )
{

    memset(Transbuf,0,KEY_LEN);
	for (int i = 0; i < nNumRecv; i++)
	{
		sprintf(Transbuf, "%02X ", (DJ_U8)(*msgbuf));
		msgbuf ++;
		Transbuf += 3;
	}
     return 3*nNumRecv;
}
void  StrtoHex(unsigned   char   *lpBuffer,   int   *Total)
{
	//不为偶数则要加一个   
	if(((*Total)   %   2)   !=   0)   
	{   
		strcat((char   *)lpBuffer,"0");   
		(*Total)   ++;   
	}   
    
	unsigned   char   HexBuffer[1024];   
    
	memset(HexBuffer,'\0',1024);   
    
	int   i   ;   
	for(   i   =   0   ;   i   <   (*Total)   ;   i++)   
	{   
		
		if(lpBuffer[i]   >   0x39)   
			lpBuffer[i]   -=   0x37   ;   
		else   
			lpBuffer[i]   -=   0x30   ;   
	}   
    
	for(i=0   ;   i   <   (*Total)   ;   i+=2)   
	{   
		HexBuffer[i/2]   =   lpBuffer[i]   ;   
		HexBuffer[i/2]   <<=   4;   
		
		HexBuffer[i/2]   |=   lpBuffer[i+1];   
	}   
    
	(*Total)   /=   2   ;   
	memcpy(lpBuffer,HexBuffer,(*Total));   
    
  }
BOOL   TransData(CString   InStr,   DJ_U8*   OutStr)   
{   
	UINT                 i   =   0,   j   =   0;   
	UINT       len   =   0;   
	UCHAR               Str[KEY_LEN   *   2];   
    
	len   =   InStr.GetLength();   
    
	if   ((len   %   2)   !=   0)   
	{   
         InStr+='0';
	}   
    
	memset(OutStr,   0,   sizeof(UCHAR[KEY_LEN]));   
	memset(Str,   0,   sizeof(UCHAR[KEY_LEN   *   2]));   
	strcpy((char*)(Str),   InStr.GetBuffer(0));   
	InStr.ReleaseBuffer();   
    
	for(i   =   0;   i   <   len;   i++)   
	{   
		if   ((Str[i]   >   47)   &&   (Str[i]   <   58))   
		{   
			Str[i]   -=   48;   
		}   
		else   if   ((Str[i]   >   64)   &&   (Str[i]   <   71))   
		{   
			Str[i]   -=   55;   
		}   
		else   if   ((Str[i]   >   96)   &&   (Str[i]   <   103))   
		{   
			Str[i]   -=   87;   
		}   
		else   
		{   
			AfxMessageBox("数据格式错误！");   
			return   FALSE;   
		}   
	}   
    
	i   =   0;   
	while(i   <   len)   
	{   //   转换   
		Str[j]   =   Str[i]   *   16   +   Str[i   +   1];   
		i   +=   2;   
		j++;   
	} 
	return true;
}