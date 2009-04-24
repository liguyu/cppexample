#include "stdafx.h"
#include "ReadEvtDetail.h"
#include "ReadPacket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool	isEnglish = TRUE;

ReadEvtDetail::ReadEvtDetail(DJ_Void *pkt, int nNumRecv)
{
      m_pkt=pkt;
	  msgnum=nNumRecv;
      pAcsEvt=(Acs_Evt_t *)((DJ_S8 *)m_pkt+sizeof(DJ_S8));
	  m_Submsg.nNUm=0;
      for (int i=0;i<_MUX_MSG_DETAIL;i++)
      {
       m_Submsg.n_MSgSrc[i].Empty();
       m_Submsg.n_MsgAnly[i].Empty();
       m_Submsg.n_Msg[i].Empty();
      }
}
ReadEvtDetail::~ReadEvtDetail()
{
       m_pkt=0;
       m_msg.Empty();
}
int ReadEvtDetail::GetString_EvtData()
{
     
	 char   tempstr[__MAX_PACKET__DATA];

	 ReadPacket  rpkt(m_pkt,msgnum);
//	 m_Submsg.n_MSgSrc[0].Format("   ");

	 rpkt.GetPktType();
	 m_Submsg.nNUm++;
	 memset(tempstr,0,__MAX_PACKET__DATA);
	 HextoStr((DJ_U8*)m_pkt,tempstr,sizeof(DJ_S8));
	 m_Submsg.n_MSgSrc[m_Submsg.nNUm].Format(tempstr);
     if (isEnglish)
     {
		 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("EVT FLAG");
     }else{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("EVT标志");
	 }
     
     m_Submsg.n_Msg[m_Submsg.nNUm].Format("PKG_EVT");

	 m_Submsg.nNUm++;
	 m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsEvt->m_s32EventType),sizeof(EventType_t));
	 if (isEnglish)
	 {
		 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("EVT TYPE");
	 }else{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件类型");
	 }
	 
     m_Submsg.n_Msg[m_Submsg.nNUm].Format(rpkt.m_convert.msg);

	 m_Submsg.nNUm++;
	 m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsEvt->m_DeviceID),sizeof(DeviceID_t));
     if (isEnglish)
     {
		 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Device Resource");
     }else{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("设备资源");
	 }
	 
     m_Submsg.n_Msg[m_Submsg.nNUm].Format(rpkt.m_convert.dev);

	 m_Submsg.nNUm++;
	 m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsEvt->m_s32EvtSize),sizeof(DJ_S32));
     if (isEnglish)
     {
		 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("EVT Package Length");

     }else{
		 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件包长度");
	 }
     m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAcsEvt->m_s32EvtSize);

	 switch(pAcsEvt->m_s32EventType)
	 {
		case	XMS_EVT_OPEN_STREAM:
			    GetString_OPEN_STREAM();
				break;
		case	XMS_EVT_QUERY_DEVICE:
				GetString_QUERY_DEVICE();
				break;
		case	XMS_EVT_QUERY_DEVICE_END:
				// m_getmsg+=GetString_QUERY_DEVICE_END();
				break;			
		case	XMS_EVT_OPEN_DEVICE:
				GetString_OPEN_DEVICE();
				break;			
		case	XMS_EVT_CLOSE_DEVICE: 
				GetString_CLOSE_DEVICE();
				break;
		case	XMS_EVT_RESET_DEVICE: 
				GetString_RESET_DEVICE();
				break;
		case	XMS_EVT_DEVICESTATE:
				GetString_DEVICESTATE();
				break;			
		case	XMS_EVT_SETDEV_GROUP: 
				GetString_SETDEV_GROUP();
				break;
		case	XMS_EVT_SETPARAM: 
				GetString_SETPARAM();
				break;
		case	XMS_EVT_GETPARAM: 
				GetString_GETPARAM();
				break;
		case	XMS_EVT_QUERY_ONE_DSP_START: 
				// m_getmsg+=GetString_QUERY_ONE_DSP_START();
				break;
		case	XMS_EVT_QUERY_ONE_DSP_END: 
				// m_getmsg+=GetString_QUERY_ONE_DSP_END();
				break;
		case	XMS_EVT_QUERY_REMOVE_ONE_DSP_START: 
				// m_getmsg+=GetString_QUERY_REMOVE_ONE_DSP_START();
				break;
		case	XMS_EVT_QUERY_REMOVE_ONE_DSP_END: 
				// m_getmsg+=GetString_QUERY_REMOVE_ONE_DSP_END();
				break;
		case	XMS_EVT_OPEN_STREAM_EXT: 
				GetString_OPEN_STREAM_EXT();
				break;
		case	XMS_EVT_LICENSE_QUERY: 
				GetString_LICENSE_QUERY();
				break;
		case	XMS_EVT_ACS_TIMER: 
				// m_getmsg+=GetString_ACS_TIMER();
				break;
                                               
		case	XMS_EVT_CALLOUT: 
				GetString_CALLOUT();
				break;
		case	XMS_EVT_CALLIN:
				GetString_CALLIN();
				break;			
		case	XMS_EVT_ALERTCALL:
				GetString_ALERTCALL();
				break;				
		case	XMS_EVT_ANSWERCALL: 
				GetString_ANSWERCALL();
				break;	
		case	XMS_EVT_LINKDEVICE:
				GetString_LINKDEVICE();
				break;				
		case	XMS_EVT_UNLINKDEVICE: 
				GetString_UNLINKDEVICE();
				break;	
		case	XMS_EVT_CLEARCALL: 
				GetString_CLEARCALL();
				break;	
		case	XMS_EVT_ANALOG_INTERFACE: 
				GetString_ANALOG_INTERFACE();
				break;	
		case	XMS_EVT_CAS_MFC_START: 
				GetString_CAS_MFC_START();
				break;	
		case	XMS_EVT_CAS_MFC_END: 
				GetString_CAS_MFC_END();
				break;	
		case	XMS_EVT_SENDSIGMSG: 
				GetString_SENDSIGMSG();
				break;	
		case	XMS_EVT_SIGMON: 
				GetString_SIGMON();
				break;	
		case	XMS_EVT_3G324M: 
				GetString_3G324M();
				break;	
		case	XMS_EVT_CTX_REG: 
				GetString_CTX_REG();
				break;	
		case	XMS_EVT_CTX_LINK: 
				GetString_CTX_LINK();
				break;	
		case	XMS_EVT_CTX_APPDATA: 
				GetString_CTX_APPDATA();
				break;	
                                               
		case	XMS_EVT_JOINTOCONF: 
				GetString_JOINTOCONF();
				break;	
		case	XMS_EVT_LEAVEFROMCONF: 
				GetString_LEAVEFROMCONF();
				break;	
		case	XMS_EVT_CLEARCONF: 
				GetString_CLEARCONF();
				break;	
                                               
		case	XMS_EVT_PLAY: 
				GetString_PLAY();
				break;	
		case	XMS_EVT_INITINDEX: 
				GetString_INITINDEX();
				break;	
		case	XMS_EVT_BUILDINDEX: 
				GetString_BUILDINDEX();
				break;	
		case	XMS_EVT_CONTROLPLAY: 
				GetString_CONTROLPLAY();
				break;	
		case	XMS_EVT_RECORD: 
				GetString_RECORD();
				break;	
		case	XMS_EVT_CONTROLRECORD: 
				GetString_CONTROLRECORD();
				break;	
		case	XMS_EVT_RECORDCSP: 
				GetString_RECORDCSP();
				break;	
		case	XMS_EVT_CONTROLRECORDCSP: 
				GetString_CONTROLRECORDCSP();
				break;	
		case    XMS_EVT_3GPP_PLAY: 
				GetString_3GPP_PLAY();
				break;	
		case	XMS_EVT_3GPP_CONTROLPLAY: 
				GetString_3GPP_CONTROLPLAY();
				break;	
		case	XMS_EVT_3GPP_RECORD: 
				GetString_3GPP_RECORD();
				break;	
		case	XMS_EVT_3GPP_CONTROLRECORD: 
				GetString_3GPP_CONTROLRECORD();
				break;	
		case	XMS_EVT_PLAYCSPREQ: 
				GetString_PLAYCSPREQ();
				break;	
		case	XMS_EVT_3GPP_INIT_INDEX: 
				GetString_3GPP_INIT_INDEX();
				break;	
		case	XMS_EVT_3GPP_BUILD_INDEX: 
				GetString_3GPP_BUILD_INDEX();
				break;	
                                               
		case	XMS_EVT_SENDFAX: 
				GetString_SENDFAX();
				break;	
		case	XMS_EVT_RECVFAX: 
				GetString_RECVFAX();
				break;	
                                               
		case	XMS_EVT_SENDIODATA: 
				GetString_SENDIODATA();
				break;	
		case	XMS_EVT_RECVIODATA: 
				GetString_RECVIODATA();
				break;	
                                               
		case	XMS_EVT_CHGMONITORFILTER: 
				GetString_CHGMONITORFILTER();
				break;	
		case	XMS_EVT_DEV_TIMER: 
				// m_getmsg+=GetString_DEV_TIMER();
				break;	
                                               
		case	XMS_EVT_UNIFAILURE: 
				GetString_UNIFAILURE();
				break;	
	 }

     return  m_Submsg.nNUm;
}
//==================================Class  General=================================//
int     ReadEvtDetail::GetString_GeneralData()
{
	CString         cs;
    Acs_GeneralProc_Data  *pGenalData;

	m_tmpmsg.Empty();
	cs.Empty();
	pGenalData=(Acs_GeneralProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pGenalData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("EVT Err Code");
	}else{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件错误码");
	}
	
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pGenalData->m_s32AcsEvtErrCode);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pGenalData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("EVT State Code(1: EVT Success 0: EVT Fail)");
	}else{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态码(1: 事件成功  0: 事件失败)");
	}

    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pGenalData->m_s32AcsEvtState);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pGenalData->m_s32DeviceState),sizeof(DJ_S32));
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dev State");
	} 
	else
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("设备状态");
	}
	
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pGenalData->m_s32DeviceState);
	
	return  m_Submsg.nNUm;
}
int     ReadEvtDetail::GetString_OPEN_STREAM()
{
	int   i;
    i=GetString_GeneralData();
	return  i;
}                            	           
int     ReadEvtDetail::GetString_QUERY_DEVICE()
{
	int   i=0;

	return  i;	
}                           	           
// CString ReadEvtDetail::GetString_QUERY_DEVICE_END() // Acs_Evt_t
// {
// 	m_msg=GetString_GeneralData();
// 	return  m_msg;
// }                                   
int     ReadEvtDetail::GetString_OPEN_DEVICE()
{
	int  i;
    i=GetString_GeneralData();
	return  i;
}                                        
int     ReadEvtDetail::GetString_CLOSE_DEVICE()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}                                       
int     ReadEvtDetail::GetString_RESET_DEVICE()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}                                       
int     ReadEvtDetail::GetString_DEVICESTATE()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}                                        
int     ReadEvtDetail::GetString_SETDEV_GROUP()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}                                       
   
//---------------------Para data-----------------------//
int     ReadEvtDetail::GetString_ParaData()
{
	CString             cs;
    Acs_ParamProc_Data  *pParaData;
	m_tmpmsg.Empty();
	cs.Empty();

	pParaData=(Acs_ParamProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pParaData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pParaData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pParaData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pParaData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pParaData->m_u16ParamCmdType),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("cmd 类型");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_ParaType(&pAcsEvt->m_DeviceID,pParaData->m_u16ParamCmdType));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pParaData->m_u16ParamDataSize),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据大小");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pParaData->m_u16ParamDataSize);

	return m_Submsg.nNUm;
}
int     ReadEvtDetail::GetString_SETPARAM()
{
	int  i;
    i=GetString_ParaData();
	return  i;	
}
int     ReadEvtDetail::GetString_GETPARAM()
{
	int  i;
    i=GetString_ParaData();
	return  i;	
}
//-----------------------------------                                           
int     ReadEvtDetail::GetString_QUERY_ONE_DSP_START()
{
	int  i;
    i=GetString_GeneralData();
	return  i;
}                                
int     ReadEvtDetail::GetString_QUERY_ONE_DSP_END()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}                                  
int     ReadEvtDetail::GetString_QUERY_REMOVE_ONE_DSP_START()
{
 	int  i;
    i=GetString_GeneralData();
	return  i;	
}                         
int     ReadEvtDetail::GetString_QUERY_REMOVE_ONE_DSP_END()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}  
//---------------------------------                         
int     ReadEvtDetail::GetString_OPEN_STREAM_EXT()
{
	
	CString                 cs;
    Acs_OpenStreamExt_Data  *pOpenStrmExtData;
	m_msg.Empty();
	cs.Empty();
	pOpenStrmExtData=(Acs_OpenStreamExt_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pOpenStrmExtData->m_ServerID.m_s8ServerIp),sizeof(DJ_S8)*32);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IP");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pOpenStrmExtData->m_ServerID.m_s8ServerIp);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pOpenStrmExtData->m_ServerID.m_u32ServerPort),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("PORT");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pOpenStrmExtData->m_ServerID.m_u32ServerPort);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pOpenStrmExtData->m_ServerID.m_s8UserName),sizeof(DJ_S8)*32);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("USER NAME");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pOpenStrmExtData->m_ServerID.m_s8UserName);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pOpenStrmExtData->m_ServerID.m_s8UserPwd),sizeof(DJ_S8)*32);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Password");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pOpenStrmExtData->m_ServerID.m_s8UserPwd);
	
	return  m_Submsg.nNUm;	
}                        	           
int     ReadEvtDetail::GetString_LICENSE_QUERY()
{
	CString                cs;
	Acs_LicenseQuery_Data  *pLicenQueryData;
    cs.Empty();
    m_msg.Empty();
	
	pLicenQueryData=(Acs_LicenseQuery_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLicenQueryData->m_u8ModuleID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("DSP ID");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pLicenQueryData->m_u8ModuleID);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLicenQueryData->m_u8ModuleID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("剩余天数(0:授权过期 100:永不过期 else:剩余天数)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pLicenQueryData->m_u16LicLeftDay);

	return  m_Submsg.nNUm;
}                          	           
int     ReadEvtDetail::GetString_ACS_TIMER()
{
	int  i;
    i=GetString_GeneralData();
	return  i;	
}                                          
//============================CLASS_CALLCONTROL===========================//  
int     ReadEvtDetail::GetString_CallControlData()                             
{
      Acs_CallControl_Data  *pCallCtrlData;
	  CString               cs;

	  cs.Empty();
	  m_tmpmsg.Empty();

	  //pCallCtrlData=(Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));
	  pCallCtrlData=(Acs_CallControl_Data *)(FetchEventData(pAcsEvt));

	  m_Submsg.nNUm++;
	  m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCallCtrlData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	  m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("呼叫状态(1:成功 0:失败)");
      m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCallCtrlData->m_s32AcsEvtState);

	  m_Submsg.nNUm++;
	  m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCallCtrlData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	  m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
      m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCallCtrlData->m_s32AcsEvtErrCode);

	  m_Submsg.nNUm++;
	  m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCallCtrlData->m_s32CallClearCause),sizeof(DJ_S32));
	  m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("拆线原因");
      m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCallCtrlData->m_s32CallClearCause);

	  m_Submsg.nNUm++;
	  m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCallCtrlData->m_s8CalledNum),sizeof(DJ_S8)*ACS_MAX_CALL_NUM);
	  m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("被叫号码");
      m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCallCtrlData->m_s8CalledNum);

	  m_Submsg.nNUm++;
	  m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCallCtrlData->m_s8CallingNum),sizeof(DJ_S8)*ACS_MAX_CALL_NUM);
	  m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("主叫号码");
      m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCallCtrlData->m_s8CallingNum);

	  return m_Submsg.nNUm;
}
int     ReadEvtDetail::GetString_CALLOUT()
{
    int    i;
    i=GetString_CallControlData();
	return  i;	
}							             
int     ReadEvtDetail::GetString_CALLIN()
{
    int    i;
    i=GetString_CallControlData();
	return  i;	
}							               
int     ReadEvtDetail::GetString_ALERTCALL()
{
    int    i;
    i=GetString_CallControlData();
	return  i;
}		                     
int     ReadEvtDetail::GetString_ANSWERCALL()
{
    int    i;
    i=GetString_CallControlData();
	return  i;	
}
//---------------------	Link Data---------------------
int     ReadEvtDetail::GetString_LinkData()
{
	Acs_LinkDev_Data     *pLinkDevData;
	CString               cs;
	
	cs.Empty();
	m_tmpmsg.Empty();
	
	pLinkDevData=(Acs_LinkDev_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLinkDevData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("连接/断开 状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pLinkDevData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLinkDevData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pLinkDevData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLinkDevData->m_SrcDevice),sizeof(DeviceID_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("源地址");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pLinkDevData->m_SrcDevice));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLinkDevData->m_DestDevice),sizeof(DeviceID_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("目的地址");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pLinkDevData->m_DestDevice));

	return m_Submsg.nNUm;
}
int     ReadEvtDetail::GetString_LINKDEVICE()
{
	int  i;
    i=GetString_LinkData();
	return  i;	
}				                 
int     ReadEvtDetail::GetString_UNLINKDEVICE()
{
	int  i;
    i=GetString_LinkData();
	return  i;
}				               
int     ReadEvtDetail::GetString_CLEARCALL()
{
	int  i;
    i=GetString_CallControlData();
	return  i;
}
CString     ReadEvtDetail::GetString_ANALOG_INTERFACE_STATE(DeviceID_t *pDevID,DJ_U8 iState)
{
	m_tmpmsg.Empty();
	
	if (XMS_DEVMAIN_INTERFACE_CH!=pDevID->m_s16DeviceMain) m_tmpmsg="UnKnown";

	if(XMS_DEVSUB_ANALOG_TRUNK==pDevID->m_s16DeviceSub)
	{
        switch(iState)
		{
		case	XMS_ANALOG_TRUNK_CH_RING:
			m_tmpmsg="Trunk Ring";
			break;
		case	XMS_ANALOG_TRUNK_CH_POSITEX_POLARITY:
			m_tmpmsg="Trunk Positex polarity";
			break;
		case	XMS_ANALOG_TRUNK_CH_NEGATIVE_POLARITY:
			m_tmpmsg="Trunk Negative polarity";
			break;
		case	XMS_ANALOG_TRUNK_CH_LOOPDROP:
			m_tmpmsg="Trunk Loop Drop";
			break;
		case	XMS_ANALOG_TRUNK_CH_NOLOOP:
			m_tmpmsg="Trunk No loop";
			break;			
		case    XMS_ANALOG_TRUNK_CH_FLASH:
			m_tmpmsg="Trunk Flash";
			break;
		default:
            m_tmpmsg="UnKnown";
		}
		return m_tmpmsg;
	}
    if(XMS_DEVSUB_ANALOG_USER==pDevID->m_s16DeviceSub)
	{
		switch(iState)
		{
		case XMS_ANALOG_USER_CH_OFFHOOK:
            m_tmpmsg="User OFF Hook";
			break;
        case XMS_ANALOG_USER_CH_ONHOOK:
			m_tmpmsg="User ON Hook";
			break;
		case XMS_ANALOG_USER_CH_FLASH:
            m_tmpmsg="User Flash";
			break;
		default:
            m_tmpmsg="UnKnown";
		}
		return m_tmpmsg;       
	}
	else m_tmpmsg="UnKnown";
	return m_tmpmsg;
}						             
int     ReadEvtDetail::GetString_ANALOG_INTERFACE()
{
	Acs_AnalogInterface_Data     *pAnalogData;
	CString                      cs;
	
	cs.Empty();
	m_msg.Empty();
	
	pAnalogData=(Acs_AnalogInterface_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAnalogData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAnalogData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAnalogData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAnalogData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAnalogData->m_u8AnalogInterfaceState),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("通道状态");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_ANALOG_INTERFACE_STATE(&pAcsEvt->m_DeviceID,pAnalogData->m_u8AnalogInterfaceState));
	
	return m_Submsg.nNUm;	
}
//------------------------------------
int     ReadEvtDetail::GetString_MFC_Data()
{
	Acs_CAS_ExtraInfo_t     *pCASData;
	CString                 cs;
	
	cs.Empty();
	m_tmpmsg.Empty();
	
	pCASData=(Acs_CAS_ExtraInfo_t *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCASData->m_u8KA),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Caller's KA");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCASData->m_u8KA);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCASData->m_u8KD),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Caller's KD");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCASData->m_u8KD);
	
	return m_Submsg.nNUm;   
}
int     ReadEvtDetail::GetString_CAS_MFC_START()
{
	int  i;
	i=GetString_MFC_Data();
	return  i;	
}				             
int     ReadEvtDetail::GetString_CAS_MFC_END()
{
	int  i;
	i=GetString_MFC_Data();
	return  i;	
}
//--------------------------------------------------------					             
int     ReadEvtDetail::GetString_SENDSIGMSG()
{
	Acs_SendSigMsg_Data   *pSendSigMsgData;
	CString                      cs;
	
	cs.Empty();
	m_msg.Empty();
	
	pSendSigMsgData=(Acs_SendSigMsg_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendSigMsgData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSendSigMsgData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendSigMsgData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSendSigMsgData->m_s32AcsEvtErrCode);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendSigMsgData->m_s16SignalMsgType),sizeof(DJ_S16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("消息类型(1:Msg Data 2:Msg cmd 3:Msg status)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSendSigMsgData->m_s16SignalMsgType);

	return  m_Submsg.nNUm;
}
//--------------------------------------------------
CString     ReadEvtDetail::GetString_SIGMON_ISDNMsgType(DJ_U8   MsgType)
{
	char  Temp[50];

	switch(MsgType)
	{
	case	SMON_ISDN_Alerting        : return"ISDN Alerting";//
	case	SMON_ISDN_Call_Proceeding : return"ISDN Call Proceeding";//
	case	SMON_ISDN_Connect         : return"ISDN Connect";//
	case	SMON_ISDN_Connect_Ack     : return"ISDN Connect Ack";//
	case	SMON_ISDN_Progress        : return"ISDN Progress";//
	case	SMON_ISDN_Setup           : return"ISDN Setup";//
	case	SMON_ISDN_Setup_Ack       : return"ISDN Setup Ack";//
		
	case	SMON_ISDN_Resume          : return"ISDN Resume";//
	case	SMON_ISDN_Resume_Ack      : return"ISDN Resume Ack";//
	case	SMON_ISDN_Resume_Reject   : return"ISDN Resume Reject";//
	case	SMON_ISDN_Suspend         : return"ISDN Suspend";//
	case	SMON_ISDN_Suspend_Ack     : return"ISDN Suspend Ack";//
	case	SMON_ISDN_Suspend_Reject  : return"ISDN Suspend Reject";//
	case	SMON_ISDN_User_Information: return"ISDN User Information";//
		                                                                
	case	SMON_ISDN_Disconnect      : return"ISDN Disconnect";//
	case	SMON_ISDN_Release         : return"ISDN Release";//
	case	SMON_ISDN_Release_Complete: return"ISDN Release Complete";//
	case	SMON_ISDN_Restart         : return"ISDN Restart";//
	case	SMON_ISDN_Restart_Ack     : return"ISDN Restart Ack";//
		                                                                
	case	SMON_ISDN_Segment         : return"ISDN Segment";//
	case	SMON_ISDN_Congestion_Ctrl : return"ISDN Congestion Ctrl";//
	case	SMON_ISDN_Information     : return"ISDN Information";//
	case	SMON_ISDN_Notify          : return"ISDN Notify";//
	case	SMON_ISDN_Status          : return"ISDN Status";//
	case	SMON_ISDN_Status_Enquiry  : return"ISDN Status Enquiry";//
	}

	sprintf ( Temp, "UNKNOWN(0x%X)", MsgType );
	return Temp;
}
CString     ReadEvtDetail::GetString_SIGMON_TUPMsgType(DJ_U8   MsgType)
{
	char  Temp[50];
	
	switch(MsgType)
	{
		case	SMON_TUP_IAM: return "TUP IAM";             //       
		case	SMON_TUP_IAI: return "TUP IAI";             //       
		case	SMON_TUP_SAM: return "TUP SAM";             //       
		case	SMON_TUP_SAO: return "TUP SAO";             //       
			
		case	SMON_TUP_ACM: return "TUP ACM";             //       
			
		case	SMON_TUP_SEC: return "TUP SEC";             //       
		case	SMON_TUP_CGC: return "TUP CGC";             //       
		case	SMON_TUP_NNC: return "TUP NNC";             //       
		case	SMON_TUP_ADI: return "TUP ADI";             //       
		case	SMON_TUP_CFL: return "TUP CFL";             //       
		case	SMON_TUP_SSB: return "TUP SSB";             //       
		case	SMON_TUP_UNN: return "TUP UNN";             //       
		case	SMON_TUP_LOS: return "TUP LOS";             //       
		case	SMON_TUP_SST: return "TUP SST";             //       
		case	SMON_TUP_ACB: return "TUP ACB";             //       
		case	SMON_TUP_DPN: return "TUP DPN";             //       
		case	SMON_TUP_MPR: return "TUP MPR";             //       
			
		case	SMON_TUP_SLB: return "TUP SLB";             //       
		case	SMON_TUP_STB: return "TUP STB";             //       
			
		case	SMON_TUP_ANU: return "TUP ANU";             //       
		case	SMON_TUP_ANC: return "TUP ANC";             //       
		case	SMON_TUP_ANN: return "TUP ANN";             //       
		case	SMON_TUP_CBK: return "TUP CBK";             //       
		case	SMON_TUP_CLF: return "TUP CLF";             //       
			
		case	SMON_TUP_RLG: return "TUP RLG";             //       
			
		case	SMON_TUP_GSM: return "TUP GSM";             //       
			
			
		case	SMON_TUP_COT: return "TUP COT";             //       
		case	SMON_TUP_CCF: return "TUP CCF";             //       
			
		case	SMON_TUP_GRQ: return "TUP GRQ";             //       
			
		case	SMON_TUP_CHG: return "TUP CHG";             //       
			
		case	SMON_TUP_EUM: return "TUP EUM";             //       
			
		case	SMON_TUP_RAN: return "TUP RAN";             //       
		case	SMON_TUP_FOT: return "TUP FOT";             //       
		case	SMON_TUP_CCL: return "TUP CCL";             //       
			
		case	SMON_TUP_BLO: return "TUP BLO";             //       
		case	SMON_TUP_BLA: return "TUP BLA";             //       
		case	SMON_TUP_UBL: return "TUP UBL";             //       
		case	SMON_TUP_UBA: return "TUP UBA";             //       
		case	SMON_TUP_CCR: return "TUP CCR";             //       
		case	SMON_TUP_RSC: return "TUP RSC";             //       
			
		case	SMON_TUP_MGB: return "TUP MGB";             //       
		case	SMON_TUP_MBA: return "TUP MBA";             //       
		case	SMON_TUP_MGU: return "TUP MGU";             //       
		case	SMON_TUP_MUA: return "TUP MUA";             //       
		case	SMON_TUP_HGB: return "TUP HGB";             //       
		case	SMON_TUP_HBA: return "TUP HBA";             //       
		case	SMON_TUP_HGU: return "TUP HGU";             //       
		case	SMON_TUP_HUA: return "TUP HUA";             //       
		case	SMON_TUP_GRS: return "TUP GRS";             //       
		case	SMON_TUP_GRA: return "TUP GRA";             //       
		case	SMON_TUP_SGB: return "TUP SGB";             //       
		case	SMON_TUP_SBA: return "TUP SBA";             //       
		case	SMON_TUP_SGU: return "TUP SGU";             //       
		case	SMON_TUP_SUA: return "TUP SUA";             //       
			
		case	SMON_TUP_ACC: return "TUP ACC";             //       
			
		case	SMON_TUP_MPM: return "TUP MPM";             //       
			
		case	SMON_TUP_OPR: return "TUP OPR";             //       
			
		case	SMON_TUP_MAL: return "TUP MAL";             //       
	}
	
	sprintf ( Temp, "UNKNOWN(0x%X)", MsgType );
	return Temp;
}
CString     ReadEvtDetail::GetString_SIGMON_ISUPMsgType(DJ_U8   MsgType)
{
	char  Temp[50];
	
	switch(MsgType)
	{
		case	SMON_ISUP_ACM : return"ISUP ACM";		     //    
		case	SMON_ISUP_ANM : return"ISUP ANM";		     //    
                                                     
		case	SMON_ISUP_CON : return"ISUP CON";		     //    
                                                     
		case	SMON_ISUP_INF : return"ISUP INF";		     //    
		case	SMON_ISUP_IAM : return"ISUP IAM";		     //    
                                                     
		case	SMON_ISUP_REL : return"ISUP REL";		     //    
		case	SMON_ISUP_RLC : return"ISUP RLC";		     //    
                                                     
		case	SMON_ISUP_SAM : return"ISUP SAM";		     //    
                                                     
                                                     
		case	SMON_ISUP_CPG : return"ISUP CPG";		     //    
		case	SMON_ISUP_CQR : return"ISUP CQR";		     //    
		case	SMON_ISUP_GRA : return"ISUP GRA";		     //    
		case	SMON_ISUP_CFN : return"ISUP CFN";		     //    
                                                     
		case	SMON_ISUP_COT : return"ISUP COT";		     //    
		case	SMON_ISUP_FRJ : return"ISUP FRJ";		     //    
                                                     
		case	SMON_ISUP_INR : return"ISUP INR";		     //    
                                                     
		case	SMON_ISUP_USR : return"ISUP USR";		     //    
		case	SMON_ISUP_FOT : return"ISUP FOT";		     //    
                                                     
		case	SMON_ISUP_RES : return"ISUP RES";		     //    
		case	SMON_ISUP_SUS : return"ISUP SUS";		     //    
                                                     
		case	SMON_ISUP_BLO : return"ISUP BLO";		     //    
		case	SMON_ISUP_BLA : return"ISUP BLA";		     //    
		case	SMON_ISUP_CCR : return"ISUP CCR";		     //    
		case	SMON_ISUP_LPA : return"ISUP LPA";		     //    
		case	SMON_ISUP_OLM : return"ISUP OLM";		     //    
		case	SMON_ISUP_RSC : return"ISUP RSC";		     //    
		case	SMON_ISUP_UBL : return"ISUP UBL";		     //    
		case	SMON_ISUP_UBA : return"ISUP UBA";		     //    
		case	SMON_ISUP_UCIC: return"ISUP UCIC";		     //  
                                                     
		case	SMON_ISUP_CGB : return"ISUP CGB";		     //    
		case	SMON_ISUP_CGBA: return"ISUP CGBA";		     //  
		case	SMON_ISUP_CGU : return"ISUP CGU";		     //    
		case	SMON_ISUP_CGUA: return"ISUP CGUA";		     //  
                                                     
		case	SMON_ISUP_GRS : return"ISUP GRS";		     //    
		case	SMON_ISUP_CQM : return"ISUP CQM";		     //    
                                                     
		case	SMON_ISUP_FAA : return"ISUP FAA";		     //    
		case	SMON_ISUP_FAR : return"ISUP FAR";		     //    
                                                     
		case	SMON_ISUP_PAM : return"ISUP PAM";		     //    
                                                     
		case	SMON_ISUP_UPT : return"ISUP UPT";		     //    
		case	SMON_ISUP_UPA : return"ISUP UPA";		     //    
                                                     
		case	SMON_ISUP_FAC : return"ISUP FAC";		     //    
		case	SMON_ISUP_NRM : return"ISUP NRM";		     //    
		case	SMON_ISUP_IDR : return"ISUP IDR";		     //    
		case	SMON_ISUP_IRS : return"ISUP IRS";		     //    
		case	SMON_ISUP_SGM : return"ISUP SGM";		     //    
                                                     
		case	SMON_ISUP_CRG : return"ISUP CRG";		     //    
		case	SMON_ISUP_OPR : return"ISUP OPR";		     //    
		case	SMON_ISUP_MPM : return"ISUP MPM";		     //    
		case	SMON_ISUP_CCL : return"ISUP CCL";		     //    
 
	}
	
	sprintf ( Temp, "UNKNOWN(0x%X)", MsgType );
	return Temp;
}

CString     ReadEvtDetail::GetString_SIGMON_MsgType(DJ_U8   Protocol,DJ_U8   SI,DJ_U8   MsgType)
{
    m_tmpmsg.Empty();
    if(SMON_Protocol_ISDN==Protocol)
		m_tmpmsg=GetString_SIGMON_ISDNMsgType(MsgType);
    if(SMON_Protocol_SS7==Protocol)
	{
		if(SMON_SI_TUP==SI) m_tmpmsg=GetString_SIGMON_TUPMsgType(MsgType);
        if(SMON_SI_ISUP==SI) m_tmpmsg=GetString_SIGMON_ISUPMsgType(MsgType);
	}
	else
		m_tmpmsg.Format("Unknown msg 0x%X",MsgType);

	return m_tmpmsg;

}
CString ReadEvtDetail::GetString_MonEvtType(DJ_U8 MonType)
{
    char   temp[50];
	switch(MonType)
	{
	case	SMON_EVT_Call_Generate  :  return"Call Generat"; //   
	case    SMON_EVT_Call_Connect   :  return"Call Connect"; //   
	case	SMON_EVT_Call_Disconnect:  return"Call Disconnect"; //
	case	SMON_EVT_Call_Info      :  return"Call Info"; //      
	case	SMON_Evt_Gen_Signal_Info:  return"Gen Signal Info"; //
	case	SMON_Evt_Raw_Signal_Info:  return"Raw Signal Info"; //
	}
	sprintf ( temp, "unknown Mon evt type(0x%X)", MonType );
	return temp;
}					               
int     ReadEvtDetail::GetString_SIGMON()
{
	Acs_SigMon_Data   *pSigMonData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	
	pSigMonData=(Acs_SigMon_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSigMonData->m_SigMon.EventType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Monitor 事件类型");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MonEvtType(pSigMonData->m_SigMon.EventType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSigMonData->m_SigMon.Protocol),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Mon Protocol(1:ISDN 2:SS7 3:N64K)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSigMonData->m_SigMon.Protocol);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSigMonData->m_SigMon.SI),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Monitor SI(3:SCCP 4:TUP 5:ISUP)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSigMonData->m_SigMon.SI);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSigMonData->m_SigMon.MsgType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Monitor 消息类型");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_SIGMON_MsgType(\
		   pSigMonData->m_SigMon.Protocol,pSigMonData->m_SigMon.SI,pSigMonData->m_SigMon.MsgType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSigMonData->m_SigMon.Caller_ID),sizeof(DJ_S8)*32);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Monitor Caller ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSigMonData->m_SigMon.Caller_ID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSigMonData->m_SigMon.Called_ID),sizeof(DJ_S8)*32);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Monitor Called ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSigMonData->m_SigMon.Called_ID);
	
	return  m_Submsg.nNUm;	
}
//-------------------------------------------------       				             
int     ReadEvtDetail::GetString_3G324M()
{
	Acs_3G324_Data     *p3GData;
	CString           cs;

	cs.Empty();
	m_msg.Empty();
	
	p3GData=(Acs_3G324_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GData->m_u8DataType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据类型(1:START 2:STOP 3:SIG PDU 4:UII)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GData->m_u8DataType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GData->m_u16DataLen),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据长度");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GData->m_u16DataLen);

	return  m_Submsg.nNUm;	
}       				             
int     ReadEvtDetail::GetString_CTX_REG()
{
	Acs_CtxReg_Data     *pCtxRegData;
	CString           cs;

	cs.Empty();
	m_msg.Empty();
	pCtxRegData=(Acs_CtxReg_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxRegData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxRegData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxRegData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxRegData->m_s32AcsEvtErrCode);

	return  m_Submsg.nNUm;	
}                           
int     ReadEvtDetail::GetString_CTX_LINK()
{
	Acs_CtxLink_Data   *pCtxLinkData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pCtxLinkData=(Acs_CtxLink_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxLinkData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxLinkData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxLinkData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxLinkData->m_s32AcsEvtErrCode);

	return  m_Submsg.nNUm;
}                          
int     ReadEvtDetail::GetString_CTX_APPDATA()
{
    Acs_CtxAppData_Info  *pCtxAppData;	
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pCtxAppData=(Acs_CtxAppData_Info *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxAppData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxAppData->m_s32AcsEvtErrCode);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_u8AcsUnitID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("APP ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxAppData->m_AppDataInfo.m_u8AcsUnitID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_u8SrcAppUnitID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("源APP ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxAppData->m_AppDataInfo.m_u8SrcAppUnitID);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_u8DstAppUnitID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("目的APP ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxAppData->m_AppDataInfo.m_u8DstAppUnitID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_s8SrcRegName),sizeof(DJ_S8)*XMS_MAX_REGNAME_LEN);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("源APP 名字");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCtxAppData->m_AppDataInfo.m_s8SrcRegName);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_s8DestRegName),sizeof(DJ_S8)*XMS_MAX_REGNAME_LEN);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("目的APP 名字");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCtxAppData->m_AppDataInfo.m_s8DestRegName);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_srcDevice),sizeof(DeviceID_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("源APP 设备");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pCtxAppData->m_AppDataInfo.m_srcDevice));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxAppData->m_AppDataInfo.m_u32AppDataSize),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("APP 数据大小");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxAppData->m_AppDataInfo.m_u32AppDataSize);

	return  m_Submsg.nNUm;	
}                       
//=======================================CLASS_CONFPROC==============================//	
int     ReadEvtDetail::GetString_ConfCtrl()
{
    Acs_ConfControl_Data  *pConfControlData;	
	CString           cs;
	
	cs.Empty();
	m_tmpmsg.Empty();
	pConfControlData=(Acs_ConfControl_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pConfControlData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pConfControlData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pConfControlData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pConfControlData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pConfControlData->m_MediaDevice),sizeof(DeviceID_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("媒体设备");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pConfControlData->m_MediaDevice));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pConfControlData->m_s32ConfMembers),sizeof(DJ_S32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("会议成员数");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pConfControlData->m_s32ConfMembers);

	return m_Submsg.nNUm;
}											                              
int     ReadEvtDetail::GetString_JOINTOCONF()
{
	int   i;
	i=GetString_ConfCtrl();
	return  i;	
}					               
int     ReadEvtDetail::GetString_LEAVEFROMCONF()
{
	int   i;
	i=GetString_ConfCtrl();
	return  i;	
}				             
int     ReadEvtDetail::GetString_CLEARCONF()
{
	int   i;
	i=GetString_ConfCtrl();
	return  i;			
}					               
//=======================================CLASS_MEDIAPROC============================//
int     ReadEvtDetail::GetString_MediaData()												                              
{
	Acs_MediaProc_Data     *pMediaData;
	CString           cs;
	
	cs.Empty();
	m_tmpmsg.Empty();
	pMediaData=(Acs_MediaProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaData->m_u8TaskID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaData->m_u8TaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaData->m_u8GTD),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("GTG ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaData->m_u8GTD);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaData->m_u32CurFileIndex),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("文件索引号");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaData->m_u32CurFileIndex);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaData->m_u32CurFilePos),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("文件位置");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaData->m_u32CurFilePos);

	return m_Submsg.nNUm;
}
int     ReadEvtDetail::GetString_PLAY()
{
    int	 i;
	i=GetString_MediaData();
	return i;
}			                        
int     ReadEvtDetail::GetString_INITINDEX()
{
    int	 i;
	i=GetString_MediaData();
	return i;
}				                
int     ReadEvtDetail::GetString_BUILDINDEX()
{
    int	 i;
	i=GetString_MediaData();
	return i;	
}					              
int     ReadEvtDetail::GetString_CONTROLPLAY()
{
    int	 i;
	i=GetString_MediaData();
	return i;	
}				              
int     ReadEvtDetail::GetString_RECORD()
{
    int	 i;
	i=GetString_MediaData();
	return i;	
}							              
int     ReadEvtDetail::GetString_CONTROLRECORD()
{
    int	 i;
	i=GetString_MediaData();
	return i;
}	
//------------------------------------			            
int     ReadEvtDetail::GetString_RECORDCSP()
{
	Acs_MediaCSPProc_Data    *pMediaCSPData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pMediaCSPData=(Acs_MediaCSPProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaCSPData->m_u16DataLen),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据长度");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaCSPData->m_u16DataLen);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaCSPData->m_u8DataType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据类型(用户自定义)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaCSPData->m_u8DataType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaCSPData->m_u8StreamData),sizeof(DJ_U8)*ITP_MAX_STREAM_DATA_LENGTH);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("原始数据(用户数据)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaCSPData->m_u8StreamData);
	
	return  m_Submsg.nNUm;	
}   				              
int     ReadEvtDetail::GetString_CONTROLRECORDCSP()
{
	Acs_MediaCSPProc_Data    *pMediaCSPData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pMediaCSPData=(Acs_MediaCSPProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaCSPData->m_u16DataLen),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据长度");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaCSPData->m_u16DataLen);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaCSPData->m_u8DataType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据类型(用户自定义)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaCSPData->m_u8DataType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMediaCSPData->m_u8StreamData),sizeof(DJ_U8)*ITP_MAX_STREAM_DATA_LENGTH);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("原始数据(用户数据)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pMediaCSPData->m_u8StreamData);
	
	return  m_Submsg.nNUm;	
}     
//----------------------------------------  
int     ReadEvtDetail::GetString_3GPPData()
{
    Acs_Media3GPPProc_Data *p3GPData;
	CString           cs;
	
	cs.Empty();
	m_tmpmsg.Empty();
	p3GPData=(Acs_Media3GPPProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u8AudioTaskID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("音频任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u8AudioTaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u8VideoTaskID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("视频任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u8VideoTaskID);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u8MediaType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("媒体类型(0:音频 1:视频 4:音频和视频)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u8MediaType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u32AudioCurFileIndex),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("音频文件索引号");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u32AudioCurFileIndex);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u32AudioCurFilePos),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("音频文件位置");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u32AudioCurFilePos);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u32VideoCurFileIndex),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("视频文件索引号");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u32VideoCurFileIndex);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(p3GPData->m_u32VideoCurFilePos),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("视频文件位置");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",p3GPData->m_u32VideoCurFilePos);
	
	return m_Submsg.nNUm;
	
}
int     ReadEvtDetail::GetString_3GPP_PLAY()
{
     int   i;
	 i=GetString_3GPPData();
	 return i;
}			                  
int     ReadEvtDetail::GetString_3GPP_CONTROLPLAY()
{
     int   i;
	 i=GetString_3GPPData();
	 return i;
}		              
int     ReadEvtDetail::GetString_3GPP_RECORD()
{
     int   i;
	 i=GetString_3GPPData();
	 return i;	
}	    		            
int     ReadEvtDetail::GetString_3GPP_CONTROLRECORD()
{
     int   i;
	 i=GetString_3GPPData();
	 return i;
}
//--------------------------------------------------------               
int     ReadEvtDetail::GetString_PLAYCSPREQ()
{
	Acs_CSPDataReq_Data  *pCSPReqData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pCSPReqData=(Acs_CSPDataReq_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPReqData->m_u16ReqCspDataLen),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据长度");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPReqData->m_u16ReqCspDataLen);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPReqData->m_u8DataType),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据类型(0:音频数据 1:视频数据)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPReqData->m_u8DataType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPReqData->m_u8TaskID),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("任务 ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPReqData->m_u8TaskID);

	return  m_Submsg.nNUm;	
} 				              
int     ReadEvtDetail::GetString_3GPP_INIT_INDEX()//----------
{
    int	 i;
	i=GetString_MediaData();
	return i;
}                  
int     ReadEvtDetail::GetString_3GPP_BUILD_INDEX()
{
     int   i;
	 i=GetString_3GPPData();
	 return i;		
}                 
//================================================CLASS_FAXPROC====================//											                                                                               
int     ReadEvtDetail::GetString_SENDFAX()
{
	Acs_FaxProc_Data *pFaxData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pFaxData=(Acs_FaxProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u8ErrorStep),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Error Step");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u8ErrorStep);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u8T30SendState),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("T30 Send State");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u8T30SendState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u8RecvT30Cmd),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("T30 rcv cmd");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u8RecvT30Cmd);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u16TotalPages),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("TotalPage");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u16TotalPages);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_s8RemoteID),sizeof(DJ_S8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("RemoteID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_s8RemoteID);

	return  m_Submsg.nNUm;	
}			                             
int     ReadEvtDetail::GetString_RECVFAX()
{
	Acs_FaxProc_Data *pFaxData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pFaxData=(Acs_FaxProc_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u8ErrorStep),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Error Step");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u8ErrorStep);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u8T30SendState),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("T30 Send State");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u8T30SendState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u8RecvT30Cmd),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("T30 rcv cmd");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u8RecvT30Cmd);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_u16TotalPages),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("TotalPage");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_u16TotalPages);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pFaxData->m_s8RemoteID),sizeof(DJ_S8)*ACS_REMOTEID_MAX_LEN);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("RemoteID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pFaxData->m_s8RemoteID);

	return  m_Submsg.nNUm;
}					                         
//==============================================CLASS_IOPROC=======================//
CString     ReadEvtDetail::GetString_IOType(DeviceID_t *pDev,DJ_U16 IOType)
{
	char   tmpstr[50];
	if(XMS_DEVMAIN_VOIP==pDev->m_s16DeviceMain)
	{
	   switch(IOType)
	   {
		   case	XMS_IO_TYPE_VOIP_H245UII    :    return"VoIP H.245 UserInputIndication Event"; /* VoIP H.245 UserInputIndication Event */ 
		   case	XMS_IO_TYPE_VOIP_SIPINFO    :    return"VoIP SIP INFO Event";                  /* VoIP SIP INFO Event */                  
		   case	XMS_IO_TYPE_VOIP_RTPDTMF    :    return"VoIP RTP DTMF Event";                  /* VoIP RTP DTMF Event */                  
		   case	XMS_IO_TYPE_VOIP_RTPTONE    :    return"VoIP RTP TONE";                        /* VoIP RTP TONE */                        
		   case	XMS_IO_TYPE_VOIP_T38START   :    return"VoIP T.38 fax start";                  /* VoIP T.38 fax start */                  
		   case	XMS_IO_TYPE_VOIP_T38STOP    :    return"VoIP T.38 fax stop";                   /* VoIP T.38 fax stop */                   
		   case	XMS_IO_TYPE_VOIP_REGSTATE   :    return"VoIP Registration State";              /* VoIP Registration State */              
		   case	XMS_IO_TYPE_VOIP_REGREQUEST :    return"VoIP Registration Request";            /* VoIP Registration Request */            
		   case	XMS_IO_TYPE_VOIP_REGRESPONSE:    return"VoIP Registration Response";           /* VoIP Registration Response */           
		   case	XMS_IO_TYPE_VOIP_TRANSFER   :    return"VoIP Device Transfer";                 /* VoIP Device Transfer */                 
		   default:
              sprintf(tmpstr,"Unknown 0x%X",IOType);	
	   }
	}
	if(XMS_DEVMAIN_VOICE==pDev->m_s16DeviceMain)
	{
		switch(IOType)
	   {	
	        case	XMS_IO_TYPE_DTMF:     return"DTMF";
			case	XMS_IO_TYPE_MR2F:     return"MR2F";
			case	XMS_IO_TYPE_MR2B:     return"MR2B";
			case	XMS_IO_TYPE_GTG :     return"GTG"; 
			case	XMS_IO_TYPE_FSK :     return"FSK"; 
			case	XMS_IO_TYPE_DPD :     return"DPD"; 
			case	XMS_IO_TYPE_PVD :     return"PVD";
			default:
              sprintf(tmpstr,"Unknown 0x%X",IOType);
		}
	}
   else 
	   sprintf(tmpstr,"Unknown 0x%X",IOType);
   return tmpstr;
}

CString     ReadEvtDetail::GetString_IOData(DeviceID_t *pDev, DJ_U16 IOType, DJ_U16 IOLen)
{
	char  dtmf='?';
	char  tmpstr[200]="0";
    Acs_IO_Data *pIOData;
	pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

	memset(tmpstr,0,sizeof(tmpstr));
	if(XMS_DEVMAIN_VOIP==pDev->m_s16DeviceMain)
	{
    switch (IOType)
	{
		case XMS_IO_TYPE_VOIP_H245UII:
			if (IOLen== sizeof(Acs_VoIP_H245UII))
			{
				Acs_VoIP_H245UII *h245Uii = (Acs_VoIP_H245UII *)FetchIOData(pAcsEvt);
				dtmf = h245Uii->m_s8Signal;
			}
     		break;
		case XMS_IO_TYPE_VOIP_SIPINFO:
			if (IOLen== sizeof(Acs_VoIP_SIPINFO))
			{
				Acs_VoIP_SIPINFO *sipInfo = (Acs_VoIP_SIPINFO *)FetchIOData(pAcsEvt);
				if (strcmp(sipInfo->m_s8ContentType, "application/dtmf") == 0)
					sscanf(sipInfo->m_s8Body, "%c", &dtmf);
			}
			break;
			case XMS_IO_TYPE_VOIP_RTPDTMF:
				if (IOLen== sizeof(Acs_VoIP_RTPDTMF))
				{
					Acs_VoIP_RTPDTMF *rtpDtmf = (Acs_VoIP_RTPDTMF *)FetchIOData(pAcsEvt);
					dtmf = rtpDtmf->m_s8EventChar;
				}
				break;
			case XMS_IO_TYPE_VOIP_T38START:
            case XMS_IO_TYPE_VOIP_T38STOP:			
				dtmf=0;
				break;

			default:
				dtmf=0;
				break;
	}
	return dtmf;
	}

	else
	{
	    char *p = (char *)(pIOData)+ sizeof(Acs_IO_Data);
			sprintf(tmpstr,"%s",p);
			return tmpstr;
	}

}												                               
int     ReadEvtDetail::GetString_SENDIODATA()
{
	Acs_IO_Data   *pIOData;
	CString           cs;
	
	cs.Empty();
	m_msg.Empty();
	pIOData=(Acs_IO_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pIOData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pIOData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_u16IoType),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IO TYPE");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_IOType(&pAcsEvt->m_DeviceID, pIOData->m_u16IoType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_u16IoDataLen),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IO LEN");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pIOData->m_u16IoDataLen);

	return  m_Submsg.nNUm;	
}					                       
int     ReadEvtDetail::GetString_RECVIODATA()
{
	Acs_IO_Data   *pIOData;
	CString           cs;
    DJ_S8         *pIOD=NULL;
	int           IoLen=0;
	int           k=0;
	
	cs.Empty();
	m_msg.Empty();
	pIOData=(Acs_IO_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));
	k=sizeof(Acs_IO_Data);
	pIOD=(DJ_S8 *)pIOData+sizeof(Acs_IO_Data);
	//FetchIOData

	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态(1:成功 0:失败)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pIOData->m_s32AcsEvtState);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pIOData->m_s32AcsEvtErrCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_u16IoType),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IO TYPE");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_IOType(&pAcsEvt->m_DeviceID, pIOData->m_u16IoType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pIOData->m_u16IoDataLen),sizeof(DJ_U16));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IOLEN");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pIOData->m_u16IoDataLen);
	IoLen=atoi(m_Submsg.n_Msg[m_Submsg.nNUm]);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextoChars(pIOD,IoLen);
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IO Data");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_IOData(&pAcsEvt->m_DeviceID, pIOData->m_u16IoType,pIOData->m_u16IoDataLen));

	return  m_Submsg.nNUm;
}					                       
//==============================================CLASS_SYSMONITOR===================//												                               
int     ReadEvtDetail::GetString_CHGMONITORFILTER()
{
	int i;
    i=GetString_GeneralData();
	return  i;
}		                       
int     ReadEvtDetail::GetString_DEV_TIMER()   //Acs_Evt_t
{
	int i;
    i=GetString_GeneralData();
	return  i;
}						                     
//====================================CLASS_UNIFAILURE=============================//											                                 
int     ReadEvtDetail::GetString_UNIFAILURE()
{
	Acs_UniFailure_Data  *pUnFailure;
	CString              cs;

	cs.Empty();
	m_msg.Empty();
	pUnFailure=(Acs_UniFailure_Data *)((DJ_S8 *)pAcsEvt+sizeof(Acs_Evt_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pUnFailure->m_s32AcsEvtState),sizeof(Acs_Evt_State_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("事件状态");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pUnFailure->m_s32AcsEvtState);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pUnFailure->m_s32AcsEvtErrCode),sizeof(Acs_Evt_ErrCode_t));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("ErrorCode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_ErrorCode(pUnFailure->m_s32AcsEvtErrCode));

	return  m_Submsg.nNUm;	
}	   	            

CString ReadEvtDetail::GetString_ErrorCode(DJ_S32	s32ErrorCode)
{
	static char	TmpStr[40];

	switch ( s32ErrorCode )
	{
//		case  ACSPOSITIVE_ACK      :            return"Positive Ack";
		case  ACSERR_LOADLIBERR    :            return"Load lib error";
		case  ACSERR_BADPARAMETER  :            return"Bad parameter";
		case  ACSERR_NOSERVER      :            return"No server";
		case  ACSERR_MAXCONN_EXCEED:            return"Max conn exceed";
		case  ACSERR_BADHDL        :            return"Bad handle";
		case  ACSERR_FAILSEND      :            return"Fail send";
		case  ACSERR_LINKBROKEN    :            return"Link broken";
		case  ACSERR_NOMESSAGE     :            return"No message";
		case  ACSERR_GETRESFAIL    :            return"Get res fail";
                                                  
		case  ACSERR_NOSUPPORTCMD  :            return"No support command";
		case  ACSERR_DEVICEERR     :            return"Device error";
		case  ACSERR_CTX_REGOVERRUN:            return"Ctx reg over run";
		case  ACSERR_CTX_REG_NAMEEXIS:          return"Ctx reg name exist";
		case  ACSERR_CTX_UNREG_NOREG:           return"Ctx unreg";
		case  ACSERR_CTX_TRAN_NODEST:           return"Ctx trans no dest";
		case  ACSERR_CTX_TRAN_NOSRC:            return"Ctx trans no src";
		case  ACSERR_CTX_LINK_FAIL:             return"Ctx link fail";
		case  ACSERR_CTX_APPDATA_NODEST:        return"Ctx appdata no dest";
		case  ACSERR_CTX_APPDATA_NOSRC:         return"Ctx appdata no src";
	}
	sprintf ( TmpStr, "UNKNOWN(0x%X)", s32ErrorCode);
	return TmpStr;
}
////////////////////////////////////////