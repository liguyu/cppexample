#include "stdafx.h"
#include "ReadAPIDetail.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool isEnglish;

ReadAPIDetail::ReadAPIDetail(DJ_Void *pkt, int nNumRecv)
{
	m_pkt=pkt;
	m_msgnum=nNumRecv;
    pAcsCmd=(Acs_Cmd_t *)((DJ_S8 *)m_pkt+sizeof(DJ_U8));
	m_Submsg.nNUm=0;
	for (int i=0;i<_MUX_MSG_DETAIL;i++)
	{
	   m_Submsg.n_MSgSrc[i].Empty();
	   m_Submsg.n_MsgAnly[i].Empty();
	   m_Submsg.n_Msg[i].Empty();
    }
}
ReadAPIDetail::~ReadAPIDetail()
{
	m_pkt=0;
    m_msg.Empty();
	m_Submsg.nNUm=0;
	for (int i=0;i<_MUX_MSG_DETAIL;i++)
	{
		m_Submsg.n_MSgSrc[i].Empty();
		m_Submsg.n_MsgAnly[i].Empty();
		m_Submsg.n_Msg[i].Empty();
    }
}
CString  ReadAPIDetail::GetString_APIData()
{
	char   tempstr[__MAX_PACKET__DATA];
	
	ReadPacket  rpkt(m_pkt,m_msgnum);
	//	 m_Submsg.n_MSgSrc[0].Format("   ");
	
	rpkt.GetPktType();
	m_Submsg.nNUm++;
	memset(tempstr,0,__MAX_PACKET__DATA);
	HextoStr((DJ_U8*)m_pkt,tempstr,sizeof(DJ_U8));
	m_Submsg.n_MSgSrc[m_Submsg.nNUm].Format(tempstr);
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("API Flag");
	} 
	else
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("API标志");
	}
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("PKG_API");
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsCmd->m_s32CmdType),sizeof(AcsCmdTye_t));     
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("CMD TYPE");
	} 
	else
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("命令类型");
	}
	m_Submsg.n_Msg[m_Submsg.nNUm].Format(rpkt.m_convert.msg);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsCmd->m_ITPDeviceID),sizeof(DeviceID_t));    
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Device Resource");
	} 
	else
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("设备资源");
	}
	
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pAcsCmd->m_ITPDeviceID));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsCmd->m_s32PackSize),sizeof(ParmSize_t));
	if (isEnglish)
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("CMD Package Length");
	} 
	else
	{
		m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("命令包长度");
	}
	
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAcsCmd->m_s32PackSize);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAcsCmd->m_u8AcsId),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Acs ServerID");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAcsCmd->m_u8AcsId);

	switch(pAcsCmd->m_s32CmdType)
	{
		case	ACS_CMD_TYPE_GETDEVLIST:                     
			//  GetString_CMD_GETDEVLIST();                  
			  break;                                       
                                                   
		case    ACS_CMD_TYPE_OPENDEVICE:                     
			//  GetString_CMD_OPENDEVICE();                  
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CLOSEDEVICE:                    
		//	  GetString_CMD_CLOSEDEVICE();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_RESETDEVICE:                    
			//  GetString_CMD_RESETDEVICE();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_GETDEVICESTATE:                 
			//  GetString_CMD_GETDEVICESTATE();              
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_SETDEVGROUP:                    
			//  GetString_CMD_SETDEVGROUP();                 
			  break;                                       
               											   
		case	ACS_CMD_TYPE_SETDEVPARAM:                    
			  GetString_CMD_SETDEVPARAM();                 
			  break;                                       
               											   
		case	ACS_CMD_TYPE_GETDEVPARAM:                    
			  GetString_CMD_GETDEVPARAM();                 
			  break;                                       
               											   
		case	ACS_CMD_TYPE_MAKECALLOUT:                    
			  GetString_CMD_MAKECALLOUT();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_ALERTCALL:                      
			//  GetString_CMD_ALERTCALL();                   
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_ANSWERCALLIN:                   
			//  GetString_CMD_ANSWERCALLIN();                
			  break;                                       
               											   
		case	ACS_CMD_TYPE_LINKDEV:                        
			  GetString_CMD_LINKDEV();                     
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_UNLINKDEV:                      
			  GetString_CMD_UNLINKDEV();                   
			  break;                                       
               											   
		case	ACS_CMD_TYPE_CLEARCALL:                      
			  GetString_CMD_CLEARCALL();                   
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_JOINTOCONF:                     
			  GetString_CMD_JOINTOCONF();                  
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_LEAVEFROMCONF:                  
			  GetString_CMD_LEAVEFROMCONF();               
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CLEARCONF:                      
			//  GetString_CMD_CLEARCONF();                   
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_PLAYFILE:                       
			  GetString_CMD_PLAYFILE();                    
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_INITPLAYINDEX:                  
			//  GetString_CMD_INITPLAYINDEX();               
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_BUILDPLAYINDEX:                 
			  GetString_CMD_BUILDPLAYINDEX();              
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CONTROLPLAY:                    
			  GetString_CMD_CONTROLPLAY();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_RECORDFILE:                     
			  GetString_CMD_RECORDFILE();                  
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CONTROLRECORD:                  
			  GetString_CMD_CONTROLRECORD();               
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_SENDFAX:                        
			  GetString_CMD_SENDFAX();                     
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_STOPSENDFAX:                    
			 // m_getmsg+=GetString_CMD_STOPSENDFAX();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_RECVFAX:                        
			  GetString_CMD_RECVFAX();                     
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_STOPRECVFAX:                    
			//  m_getmsg+=GetString_CMD_STOPRECVFAX();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CHANGEMONITORFILTER:            
			  GetString_CMD_CHANGEMONITORFILTER();         
			  break;                                       
               											   
		case	ACS_CMD_TYPE_SENDIO:                         
			  GetString_CMD_SENDIO();                      
			  break;                                       
               											   
		case	ACS_CMD_TYPE_SENDSIGMSG:                     
			  GetString_CMD_SENDSIGMSG();                  
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_RECORDCSP:                      
			  GetString_CMD_RECORDCSP();                   
			  break;                                       
               											   
		case	ACS_CMD_TYPE_DBGON:                          
			//  GetString_CMD_DBGON();                       
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_DBGOFF:                         
			//  GetString_CMD_DBGOFF();                      
			  break;                                       
                                                   
		case  ACS_CMD_TYPE_PLAY_3GPP:                      
			  GetString_CMD_PLAY_3GPP();                   
			  break;                                       
                                                   
		case  ACS_CMD_TYPE_CONTROL_3GPP_PLAY:              
			  GetString_CMD_CONTROL_3GPP_PLAY();           
			  break;                                       
                                                   
		case  ACS_CMD_TYPE_RECORD_3GPP:                    
			  GetString_CMD_RECORD_3GPP();                 
			  break;                                       
                                                   
		case  ACS_CMD_TYPE_CONTROL_3GPP_RECORD:            
			  GetString_CMD_CONTROL_3GPP_RECORD();         
			  break;                                       
                   											 
		case	ACS_CMD_TYPE_PLAYCSP:                        
			  GetString_CMD_PLAYCSP();                     
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_SENDPLAYCSP:                    
			  GetString_CMD_SENDPLAYCSP();                 
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_QUERYLIC:                       
			  GetString_CMD_QUERYLIC();                    
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CTXREG:                         
			  GetString_CMD_CTXREG();                      
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CTXLINK:                        
			  GetString_CMD_CTXLINK();                     
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_CTXSENDAPPDATA:                 
			  GetString_CMD_CTXSENDAPPDATA();              
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_INIT3GPPINDEX:                  
			//  GetString_CMD_INIT3GPPINDEX();               
			  break;                                       
                                                   
		case	ACS_CMD_TYPE_BUILD3GPPINDEX:                 
			  GetString_CMD_BUILD3GPPINDEX();              
			  break;         
	}

	return m_getmsg;
}

int    	ReadAPIDetail::GetString_CMD_GETDEVLIST()                  
{                                                                  
          
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_OPENDEVICE()                  
{                                                                  
      
	return 0;	
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CLOSEDEVICE()                 
{                                                                  
                
	return 0;	
}                                                                  
int    	ReadAPIDetail::GetString_CMD_RESETDEVICE()                 
{                                                                  
                                                                   
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_GETDEVICESTATE()              
{                                                                  
                                                                   
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_SETDEVGROUP()                 
{                                                                  
                                                                   
	return 0;
}
//----------------------------------	                                                                 
int    	ReadAPIDetail::GetString_CMD_SETDEVPARAM()                 
{                                                                  
    SetParm_t     *pSetParm;
    CString       cs;

	pSetParm=(SetParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSetParm->m_u16ParamCmdType),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("设置类型");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_ParaType(&pAcsCmd->m_ITPDeviceID,pSetParm->m_u16ParamCmdType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSetParm->m_u16ParamDataSize),sizeof(DJ_U16));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据大小");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSetParm->m_u16ParamDataSize);

	return m_Submsg.nNUm;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_GETDEVPARAM()                 
{                                                                  
     GetParm_t    *pGetParm;
	 CString       cs;
	 
	 pGetParm=(GetParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	 m_Submsg.nNUm++;
	 m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pGetParm->m_u16ParamCmdType),sizeof(DJ_U16));     
	 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("设置类型");
	 m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_ParaType(&pAcsCmd->m_ITPDeviceID,pGetParm->m_u16ParamCmdType));
	 
	 m_Submsg.nNUm++;
	 m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pGetParm->m_u16ParamDataSize),sizeof(DJ_U16));    
	 m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("数据大小");
	 m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pGetParm->m_u16ParamDataSize);
		 
	return 0;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_MAKECALLOUT()                 
{                                                                  
	CalloutParm_t    *pCalloutParm;
	CString           cs;
	
	pCalloutParm=(CalloutParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCalloutParm->m_s8CallingNum),sizeof(DJ_S8)*ACS_MAX_CALL_NUM);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("主叫号码");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCalloutParm->m_s8CallingNum);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCalloutParm->m_s8CalledNum),sizeof(DJ_S8)*ACS_MAX_CALL_NUM);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("被叫号码");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCalloutParm->m_s8CalledNum);
	 
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_ALERTCALL()                   
{                                                                  
                                                                   
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_ANSWERCALLIN()                
{                                                                  
                                                                   
	return 0;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_LINKDEV()                     
{                                                                  
	LinkDevParm_t    *pLinkDevParm;
	CString           cs;
	
	pLinkDevParm=(LinkDevParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLinkDevParm->m_ITPDestDeviceID),sizeof(DeviceID_t));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("目的设备");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pLinkDevParm->m_ITPDestDeviceID));
                                                                  
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_UNLINKDEV()                   
{                                                                  
	UnlinkDevParm_t    *pUnLinkDevParm;
	CString           cs;
	
	pUnLinkDevParm=(UnlinkDevParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pUnLinkDevParm->m_ITPDestDeviceID),sizeof(DeviceID_t));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("目的设备");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pUnLinkDevParm->m_ITPDestDeviceID));
	
	return m_Submsg.nNUm;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_CLEARCALL()                   
{                                                                  
    ClearCallParm_t  *pClearCallparm;
	CString           cs;
	
	pClearCallparm=(ClearCallParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pClearCallparm->m_s32ClearCause),sizeof(DJ_S32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("拆线原因");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pClearCallparm->m_s32ClearCause);
	
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_JOINTOCONF()                  
{                                                                  
    JoinToConfParm_t  *pJoinConfp;
	CString           cs;

	pJoinConfp=(JoinToConfParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pJoinConfp->m_ITPMediaDevice),sizeof(DeviceID_t));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("媒体设备");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pJoinConfp->m_ITPMediaDevice));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pJoinConfp->m_ITPConfParam.m_u8InputOpt),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("会议输入设置(0:无声 1:正常 2:放音)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pJoinConfp->m_ITPConfParam.m_u8InputOpt);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pJoinConfp->m_ITPConfParam.m_u8OutputOpt),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("会议输出设置(0:无声 1:正常 2:叠加)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pJoinConfp->m_ITPConfParam.m_u8OutputOpt);

	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_LEAVEFROMCONF()               
{                                                                  
   LeaveFromConfParm_t   *pLeavConfp;
   CString           cs;
   

   pLeavConfp=(LeaveFromConfParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));
   
   m_Submsg.nNUm++;
   m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLeavConfp->m_ITPMediaDevice),sizeof(DeviceID_t));     
   m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("媒体设备");
   m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pLeavConfp->m_ITPMediaDevice));
   
   m_Submsg.nNUm++;
   m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLeavConfp->m_ITPConfParam.m_u8InputOpt),sizeof(DJ_U8));    
   m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("会议输入设置(0:无声 1:正常 2:放音)");
   m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pLeavConfp->m_ITPConfParam.m_u8InputOpt);
   
   m_Submsg.nNUm++;
   m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pLeavConfp->m_ITPConfParam.m_u8OutputOpt),sizeof(DJ_U8));     
   m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("会议输出设置(0:无声 1:正常 2:叠加)");
   m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pLeavConfp->m_ITPConfParam.m_u8OutputOpt);
   
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CLEARCONF()                   
{                                                                  
                                                                   
	return 0;
}
int      ReadAPIDetail::GetString_PlayProperty(PlayProperty_t *pPlayProperty)
{
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8TaskID),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8TaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8SrcMode),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码格式");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaSrcMod(pPlayProperty->m_u8SrcMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8DecodeType),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码类型");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaDecodeType(pPlayProperty->m_u8DecodeType));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32MaxSize),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Size(0:real size >0:play size)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u32MaxSize);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32MaxTime),sizeof(DJ_U32));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Tmie(0:real time >0:play time)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u32MaxTime);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32StopMode),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop Mod");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaStopMod(pPlayProperty->m_u32StopMode));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32StopCode),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop code");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%c",pPlayProperty->m_u32StopCode);
	

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16FileOffset),sizeof(DJ_U16));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("File Offset");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16FileOffset);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16PlayType),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Play Type(0:file 1:index 2:queue 3:index queue)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16PlayType);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16PlayIndex),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Play index");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16PlayIndex);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_s8PlayContent),sizeof(DJ_S8)*ACS_MAX_FILE_NAME_LEN);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("File name");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pPlayProperty->m_s8PlayContent);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16FileSubOffset),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Sub Offset");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16FileSubOffset);
	
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_PLAYFILE()                    
{                                                                  
    PlayParm_t    *pPlayp;
	int           i;
	
	pPlayp=(PlayParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));
    i=GetString_PlayProperty(&pPlayp->m_PlayProperty);
	return i;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_INITPLAYINDEX()               
{                                                                  

	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_BUILDPLAYINDEX()              
{                                                                  
    BuildPlayIndexParm_t    *pBuildPlayp;
	int           i;
	
	pBuildPlayp=(BuildPlayIndexParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

    i=GetString_PlayProperty(&pBuildPlayp->m_ITPPlayProperty);
	return i;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CONTROLPLAY()                 
{                                                                  
    ControlPlayParm_t      *pCtrlPlayp;

	pCtrlPlayp=(ControlPlayParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtrlPlayp->m_ITPControlPlay.m_u16ControlType),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Ctrl Type");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_PlayCtrlType(pCtrlPlayp->m_ITPControlPlay.m_u16ControlType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtrlPlayp->m_ITPControlPlay.m_u16StepSize),sizeof(DJ_U16));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("step size");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtrlPlayp->m_ITPControlPlay.m_u16StepSize);


	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_RECORDFILE()                  
{                                                                  
    RecordParm_t * pRecordp;
	CString cs;
	pRecordp=(RecordParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_u8SRCMode),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码格式");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaSrcMod(pRecordp->m_ITPRecordProperty.m_u8SRCMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_u8EncodeType),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码类型");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaDecodeType(pRecordp->m_ITPRecordProperty.m_u8EncodeType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_u32MaxSize),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Size(0:real size >0:record size)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordp->m_ITPRecordProperty.m_u32MaxSize);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_u32MaxTime),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Tmie(0:real time >0:record time)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordp->m_ITPRecordProperty.m_u32MaxTime);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_u8StopMode),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop Mod");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaStopMod(pRecordp->m_ITPRecordProperty.m_u8StopMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_u8StopCode),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop code");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%c",pRecordp->m_ITPRecordProperty.m_u8StopCode);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_MixerControl.m_u8SRC1_Ctrl),sizeof(DJ_U8));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Chnl 1 Mix Ctrl Type");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MixerType(pRecordp->m_ITPRecordProperty.m_MixerControl.m_u8SRC1_Ctrl));
	
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_MixerControl.m_u8SRC2_Ctrl),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Chnl 2 Mix Ctrl Type");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MixerType(pRecordp->m_ITPRecordProperty.m_MixerControl.m_u8SRC2_Ctrl));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_MixerControl.m_u16SRC_ChID1),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Chnl 1 Mix Ctrl ID");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordp->m_ITPRecordProperty.m_MixerControl.m_u16SRC_ChID1);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_MixerControl.m_u16SRC_ChID2),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Chnl 2 Mix Ctrl ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordp->m_ITPRecordProperty.m_MixerControl.m_u16SRC_ChID2);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_s8FileName),sizeof(DJ_S8)*MAX_PATH);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("File path");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pRecordp->m_ITPRecordProperty.m_s8FileName);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordp->m_ITPRecordProperty.m_s8IsAppend),sizeof(DJ_S16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Is Append(0:no 1:append)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordp->m_ITPRecordProperty.m_s8IsAppend);
	
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CONTROLRECORD()               
{                                                                  
    ControlRecordParm_t   *pCtrlRepara;
	CString cs;
	pCtrlRepara=(ControlRecordParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtrlRepara->m_ITPControlRecord.m_u32ControlType),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("控制类型");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_RecordCtrlType(pCtrlRepara->m_ITPControlRecord.m_u32ControlType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtrlRepara->m_ITPControlRecord.m_u32StepSize),sizeof(DJ_U32));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("step size");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtrlRepara->m_ITPControlRecord.m_u32StepSize);

	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_SENDFAX()                     
{                                                                  
    SendFaxParm_t     *pSendFaxp; 
	CString cs;
	pSendFaxp=(SendFaxParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendFaxp->m_s8LocalID),sizeof(DJ_S8)*ACS_LOCALID_MAX_LEN);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Local ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSendFaxp->m_s8LocalID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendFaxp->m_s8FaxTiffFile),sizeof(DJ_S8)*MAX_PATH);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("File Path");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pSendFaxp->m_s8FaxTiffFile);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendFaxp->m_ITPMediaDeviceID),sizeof(DeviceID_t));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dev ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pSendFaxp->m_ITPMediaDeviceID));
	
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_STOPSENDFAX()                 
{                                                                  
	
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_RECVFAX()                     
{                                                                  
    RecvFaxParm_t    *pRecvFaxp;
	CString cs;
	pRecvFaxp=(RecvFaxParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecvFaxp->m_s8LocalID),sizeof(DJ_S8)*ACS_LOCALID_MAX_LEN);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Local ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecvFaxp->m_s8LocalID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecvFaxp->m_s8FaxTiffFile),sizeof(DJ_S8)*MAX_PATH);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("File Path");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pRecvFaxp->m_s8FaxTiffFile);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecvFaxp->m_ITPMediaDeviceID),sizeof(DeviceID_t));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dev ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pRecvFaxp->m_ITPMediaDeviceID));
	
	return m_Submsg.nNUm;

}                                                                  
int    	ReadAPIDetail::GetString_CMD_STOPRECVFAX()                 
{                                                                  
                                                                   
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CHANGEMONITORFILTER()         
{                                                                  
    MonitorFilterParm_t *pMonitorp;
	CString cs;
	pMonitorp=(MonitorFilterParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pMonitorp->m_u32MonitorFilter),sizeof(DJ_U32));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dev ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_IOEvtMask(pMonitorp->m_u32MonitorFilter));
	
	return m_Submsg.nNUm;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_SENDIO()                      
{                                                                  
    SendIOParm_t     *pSendIop;
	CString cs;	

	pSendIop=(SendIOParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendIop->m_u16IoType),sizeof(DJ_U16));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("IO Type");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_IOType(&pAcsCmd->m_ITPDeviceID,pSendIop->m_u16IoType));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pSendIop->m_u16IoDataLen),sizeof(DJ_U16));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Data Len");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pSendIop->m_u16IoDataLen);
	
	return m_Submsg.nNUm;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_SENDSIGMSG()                  
{                                                                  
                                                               
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_RECORDCSP()                   
{                                                                  
    RecordCSPParm_t  *pRecordCSPp;
	CString cs;	
	
	pRecordCSPp=(RecordCSPParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordCSPp->m_ITPRecordCSPProperty.m_u8SRCMode),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("CSP src Mode");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaSrcMod(pRecordCSPp->m_ITPRecordCSPProperty.m_u8SRCMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordCSPp->m_ITPRecordCSPProperty.m_u8EncodeType),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("CSP encode type");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaDecodeType(pRecordCSPp->m_ITPRecordCSPProperty.m_u8EncodeType));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordCSPp->m_ITPRecordCSPProperty.m_u8CSPRecType),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("CSP Rec Type(1: barge-in, 2: non barge-in)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordCSPp->m_ITPRecordCSPProperty.m_u8CSPRecType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pRecordCSPp->m_ITPRecordCSPProperty.m_u16PacketLen),sizeof(DJ_U16));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("CSP packt len(1: barge-in, 2: non barge-in)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pRecordCSPp->m_ITPRecordCSPProperty.m_u16PacketLen);
	
	return m_Submsg.nNUm;
}	                                                                 
int    	ReadAPIDetail::GetString_CMD_DBGON()                       
{                                                                  
                                                                   
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_DBGOFF()                      
{                                                                  
                                                                   
	return 0;
}                                                                  
int     ReadAPIDetail::GetString_CMD_PLAY_3GPP()                   
{                                                                  
//    Play3gppParm_t    *pPlay3gppP;

	return 0;
} 
int      ReadAPIDetail::GetString_3GpPlayProperty(Play3gppProperty_t *pPlayProperty)
{
	CString cs;
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8TaskID),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8TaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8SrcMode),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码格式");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaSrcMod(pPlayProperty->m_u8SrcMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8DecodeType),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码类型");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaDecodeType(pPlayProperty->m_u8DecodeType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8Audio_tracks),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Audio tracks)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8Audio_tracks);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8AmrFrameType),sizeof(DJ_S8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("AMR Frame type)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8AmrFrameType);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8AudioNoDecode),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Audio No Decode)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8AudioNoDecode);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32MaxTime),sizeof(DJ_U32));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Tmie(0:real time >0:play time)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u32MaxTime);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32StopMode),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop Mod");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaStopMod(pPlayProperty->m_u32StopMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32StopCode),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop code");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%c",pPlayProperty->m_u32StopCode);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16TimeOffset),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Time Offset");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16TimeOffset);	

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16PlayType),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Play Type(0:file 1:index 2:queue 3:index queue)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16PlayType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16PlayIndex),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Play index");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16PlayIndex);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_s8PlayContent),sizeof(DJ_S8)*ACS_MAX_FILE_NAME_LEN);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("File name");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pPlayProperty->m_s8PlayContent);

	///////////////////////////////////////////////////////////////////////////////////////
    //Video
	///////////////////////////////////////////////////////////////////////////////////////////
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8VideoFrameRate),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video frame rate");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8VideoFrameRate);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8VideoDecode_Type),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video decode type");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8VideoDecode_Type);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8VideoHigh),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video high");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8VideoHigh);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8VideoWidth),sizeof(DJ_S8)*ACS_MAX_FILE_NAME_LEN);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video width");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pPlayProperty->m_u8VideoWidth);



	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u32VideoMaxTime),sizeof(DJ_U32));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video Max Time");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u32VideoMaxTime);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16VideoTimeOffset),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video Time Offset");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16VideoTimeOffset);	
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16VideoPlayType),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video Play Type(0:file 1:index 2:queue 3:index queue)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16VideoPlayType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u16VideoPlayIndex),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video Play index");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u16VideoPlayIndex);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8VideoTaskID),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video task ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8VideoTaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_u8MediaType),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Media type(0:audio 1:video 4:video and audio)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pPlayProperty->m_u8MediaType);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pPlayProperty->m_s8VideoPlayContent),sizeof(DJ_S8)*ACS_MAX_FILE_NAME_LEN);    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Video File Path");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pPlayProperty->m_s8VideoPlayContent);

	return m_Submsg.nNUm;
}                                                                  
int     ReadAPIDetail::GetString_CMD_CONTROL_3GPP_PLAY()           
{                                                                  
 //   Play3gppParm_t                                                               
	return 0;
}                                                                  
int     ReadAPIDetail::GetString_CMD_RECORD_3GPP()                 
{                                                                  
                                                                   
	return 0;
}                                                                  
int     ReadAPIDetail::GetString_CMD_CONTROL_3GPP_RECORD()         
{                                                                  
                                                                   
	return 0;
}    		                                                           
int    	ReadAPIDetail::GetString_CMD_PLAYCSP()                     
{                                                                  
    CSPPlayParm_t     *pCSPPlayp;
	CString cs;	
	
	pCSPPlayp=(CSPPlayParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u8TaskID),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlayp->m_ITPCSPPlayProperty.m_u8TaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u8SrcMode),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码格式");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaSrcMod(pCSPPlayp->m_ITPCSPPlayProperty.m_u8SrcMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u8DecodeType),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("编码类型");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaDecodeType(pCSPPlayp->m_ITPCSPPlayProperty.m_u8DecodeType));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u32MaxSize),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Size(0:real size >0:play size))");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlayp->m_ITPCSPPlayProperty.m_u32MaxSize);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u32MaxTime),sizeof(DJ_U32));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Max Tmie(0:real time >0:play time)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlayp->m_ITPCSPPlayProperty.m_u32MaxTime);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u32StopMode),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop Mod");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_MediaStopMod(pCSPPlayp->m_ITPCSPPlayProperty.m_u32StopMode));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlayp->m_ITPCSPPlayProperty.m_u32StopCode),sizeof(DJ_U32));
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Stop code");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%c",pCSPPlayp->m_ITPCSPPlayProperty.m_u32StopCode);	
	
    return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_SENDPLAYCSP()                 
{                                                                  
    CSPPlaySendDataParm_t   *pCSPPlaySendDatap;
    int             Len=0;

	pCSPPlaySendDatap=(CSPPlaySendDataParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlaySendDatap->m_CspPlayDataInfo.m_u8TaskID),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("任务ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlaySendDatap->m_CspPlayDataInfo.m_u8TaskID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlaySendDatap->m_CspPlayDataInfo.m_u8DataType),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Data Type(0:voc 1:video)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlaySendDatap->m_CspPlayDataInfo.m_u8DataType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlaySendDatap->m_CspPlayDataInfo.m_u16DataLen),sizeof(DJ_U16));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Data Len");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlaySendDatap->m_CspPlayDataInfo.m_u16DataLen);
	Len=atoi(m_Submsg.n_Msg[m_Submsg.nNUm]);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCSPPlaySendDatap->m_u8StreamData),sizeof(DJ_U8)*Len);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Data");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCSPPlaySendDatap->m_u8StreamData);

	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_QUERYLIC()                    
{                                                                  
    QueryLicenseParm_t  *pQLicenp;
    CString          cs;
	
	pQLicenp=(QueryLicenseParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pQLicenp->m_u8ModuleID),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Module ID)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pQLicenp->m_u8ModuleID);

	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CTXREG()                      
{                                                                  
    XMSCtxRegParm_t *pCtxRegp;
    CString          cs;
	
	pCtxRegp=(XMSCtxRegParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxRegp->m_u8AcsUnitID),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("AcsUnit ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxRegp->m_u8AcsUnitID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxRegp->m_s8RegName),sizeof(DJ_S8)*XMS_MAX_REGNAME_LEN);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Reg Name");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pCtxRegp->m_s8RegName);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxRegp->m_u8RegType),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Reg Type(1:reg 2:unreg)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxRegp->m_u8RegType);
	
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CTXLINK()                     
{                                                                  
    XMSCtxLinkParm_t  *pCtxlinkp;
    CString          cs;
	
	pCtxlinkp=(XMSCtxLinkParm_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxlinkp->m_u8AcsUnitID),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("AcsUnit ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pCtxlinkp->m_u8AcsUnitID);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxlinkp->m_srcDev),sizeof(DeviceID_t));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Src Dev");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pCtxlinkp->m_srcDev));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxlinkp->m_destDev),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Reg Type(1:reg 2:unreg)");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pCtxlinkp->m_destDev));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pCtxlinkp->m_u8CtxLinkOpt),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Link opt");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",GetString_CtxLinkType(pCtxlinkp->m_u8CtxLinkOpt));
	
	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_CTXSENDAPPDATA()              
{                                                                  
    XMS_AppData_Info_t   *pAppDatap;
    CString          cs;
	
	pAppDatap=(XMS_AppData_Info_t *)((DJ_S8 *)pAcsCmd+sizeof(Acs_Cmd_t));

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_u8AcsUnitID),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("AcsUnit ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAppDatap->m_u8AcsUnitID);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_u8SrcAppUnitID),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Src AcsUnit ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAppDatap->m_u8SrcAppUnitID);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_u8DstAppUnitID),sizeof(DJ_U8));    
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dest AcsUnit ID");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAppDatap->m_u8DstAppUnitID);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_s8SrcRegName),sizeof(DJ_S8)*XMS_MAX_REGNAME_LEN);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Src app name");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pAppDatap->m_s8SrcRegName);

	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_s8DestRegName),sizeof(DJ_S8)*XMS_MAX_REGNAME_LEN);     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dest app name");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",pAppDatap->m_s8DestRegName);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_u8AppReqType),sizeof(DJ_U8));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format(",App req Type(user self define)");
    m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAppDatap->m_u8AppReqType);
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_srcDevice),sizeof(DeviceID_t));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Dev");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%s",GetString_DeviceAll(&pAppDatap->m_srcDevice));
	
	m_Submsg.nNUm++;
	m_Submsg.n_MSgSrc[m_Submsg.nNUm]=HextotoStr((DJ_Void *)&(pAppDatap->m_u32AppDataSize),sizeof(DJ_U32));     
	m_Submsg.n_MsgAnly[m_Submsg.nNUm].Format("Data size");
	m_Submsg.n_Msg[m_Submsg.nNUm].Format("%d",pAppDatap->m_u32AppDataSize);

	return m_Submsg.nNUm;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_INIT3GPPINDEX()               
{                                                                  
                                                                   
	return 0;
}                                                                  
int    	ReadAPIDetail::GetString_CMD_BUILD3GPPINDEX()              
{                                                                  
                                                             
	return 0;
}                                                                  
