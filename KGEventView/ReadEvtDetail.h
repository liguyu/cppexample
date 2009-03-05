#if !defined(AFX_READEVTDETAIL_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_)
#define AFX_READEVTDETAIL_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "ITPDataDefine.h"
#include "DJAcsCmdDef.h"
#include "commontool.h"

class ReadEvtDetail
{
//-------------def func-----------//
public:
	  
	ReadEvtDetail(DJ_Void *pkt, int nNumRecv);
	~ReadEvtDetail();
	int      GetString_EvtData();  

	int     GetString_OPEN_STREAM();               
	int     GetString_QUERY_DEVICE();              
	int     GetString_QUERY_DEVICE_END();
	int     GetString_OPEN_DEVICE();               
	int     GetString_CLOSE_DEVICE();              
	int     GetString_RESET_DEVICE();              
	int     GetString_DEVICESTATE();               
	int     GetString_SETDEV_GROUP();              
	int     GetString_SETPARAM();                  
	int     GetString_GETPARAM();                  
	int     GetString_QUERY_ONE_DSP_START();       
	int     GetString_QUERY_ONE_DSP_END();         
	int     GetString_QUERY_REMOVE_ONE_DSP_START();
	int     GetString_QUERY_REMOVE_ONE_DSP_END();  
	int     GetString_OPEN_STREAM_EXT();           
	int     GetString_LICENSE_QUERY();             
	int     GetString_ACS_TIMER();                 
													  
	int     GetString_CALLOUT();							      
	int     GetString_CALLIN();							      
	int     GetString_ALERTCALL();		              
	int     GetString_ANSWERCALL();		            
	int     GetString_LINKDEVICE();				        
	int     GetString_UNLINKDEVICE();				      
	int     GetString_CLEARCALL();						      
	int     GetString_ANALOG_INTERFACE();          
	int     GetString_CAS_MFC_START();				      
	int     GetString_CAS_MFC_END();					      
	int     GetString_SENDSIGMSG();					      
	int     GetString_SIGMON();       				      
	int     GetString_3G324M();       				      
	int     GetString_CTX_REG();                   
	int     GetString_CTX_LINK();                  
	int     GetString_CTX_APPDATA();               
													  
	int     GetString_JOINTOCONF();					      
	int     GetString_LEAVEFROMCONF();				      
	int     GetString_CLEARCONF();					        
													
	int     GetString_PLAY();			                
	int     GetString_INITINDEX();				          
	int     GetString_BUILDINDEX();					      
	int     GetString_CONTROLPLAY();				        
	int     GetString_RECORD();							      
	int     GetString_CONTROLRECORD();				      
	int     GetString_RECORDCSP();   				      
	int     GetString_CONTROLRECORDCSP();          
	int     GetString_3GPP_PLAY();			                  
	int     GetString_3GPP_CONTROLPLAY();		      
	int     GetString_3GPP_RECORD();	    		      
	int     GetString_3GPP_CONTROLRECORD();        
	int     GetString_PLAYCSPREQ(); 				        
	int     GetString_3GPP_INIT_INDEX();           
	int     GetString_3GPP_BUILD_INDEX();          
												  
	int     GetString_SENDFAX();			              
	int     GetString_RECVFAX();					          
													  
	int     GetString_SENDIODATA();					      
	int     GetString_RECVIODATA();					      
													
	int     GetString_CHGMONITORFILTER();		      
	int     GetString_DEV_TIMER();						      
												  
	int     GetString_UNIFAILURE();					      


protected:

	CString  GetString_ANALOG_INTERFACE_STATE(DeviceID_t *pDevID,DJ_U8 iState);
    CString  GetString_MonEvtType(DJ_U8 MonType);
	CString  GetString_SIGMON_ISDNMsgType(DJ_U8   MsgType);
	CString  GetString_SIGMON_TUPMsgType(DJ_U8   MsgType);
	CString  GetString_SIGMON_ISUPMsgType(DJ_U8   MsgType);
    CString  GetString_SIGMON_MsgType(DJ_U8   Protocol,DJ_U8   SI,DJ_U8   MsgType);
	CString  GetString_IOData(DeviceID_t *pDev, DJ_U16 IOType, DJ_U16 IOLen);
    CString  GetString_IOType(DeviceID_t *pDev,DJ_U16 IOType);
	CString  GetString_ErrorCode(DJ_S32	s32ErrorCode);

private:
	int     GetString_GeneralData();
    int     GetString_ParaData();
	int     GetString_CallControlData();
	int     GetString_LinkData();
	int     GetString_MFC_Data();
    int     GetString_ConfCtrl();
    int     GetString_MediaData();
	int     GetString_3GPPData();
//-------------def attrib---------//
public:
     CString     m_getmsg;
     MsgSub      m_Submsg;

protected:


private:
     DJ_Void *      m_pkt;
	 int            msgnum;
	 Acs_Evt_t *	pAcsEvt;
	 CString        m_msg;
	 CString        m_tmpmsg;
};


#endif