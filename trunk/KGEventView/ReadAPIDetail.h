#if !defined(AFX_READAPIDETAIL_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_)
#define AFX_READAPIDETAIL_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReadPacket.h"
#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "ITPDataDefine.h"
#include "DJAcsCmdDef.h"
#include "commontool.h"

class ReadAPIDetail
{
//-------------def func-----------//
public:
	ReadAPIDetail(DJ_Void *pkt, int nNumRecv);
	~ReadAPIDetail();
	CString  GetString_APIData(); 

	int    	GetString_CMD_GETDEVLIST();              
	int    	GetString_CMD_OPENDEVICE();              
	int    	GetString_CMD_CLOSEDEVICE();             
	int    	GetString_CMD_RESETDEVICE();             
	int    	GetString_CMD_GETDEVICESTATE();          
	int    	GetString_CMD_SETDEVGROUP();	           
	int    	GetString_CMD_SETDEVPARAM();	           
	int    	GetString_CMD_GETDEVPARAM();	           
	int    	GetString_CMD_MAKECALLOUT();             
	int    	GetString_CMD_ALERTCALL();               
	int    	GetString_CMD_ANSWERCALLIN();	           
	int    	GetString_CMD_LINKDEV();                 
	int    	GetString_CMD_UNLINKDEV();	             
	int    	GetString_CMD_CLEARCALL();               
	int    	GetString_CMD_JOINTOCONF();              
	int    	GetString_CMD_LEAVEFROMCONF();           
	int    	GetString_CMD_CLEARCONF();               
	int    	GetString_CMD_PLAYFILE();                
	int    	GetString_CMD_INITPLAYINDEX();           
	int    	GetString_CMD_BUILDPLAYINDEX();          
	int    	GetString_CMD_CONTROLPLAY();             
	int    	GetString_CMD_RECORDFILE();              
	int    	GetString_CMD_CONTROLRECORD();           
	int    	GetString_CMD_SENDFAX();                 
	int    	GetString_CMD_STOPSENDFAX();             
	int    	GetString_CMD_RECVFAX();                 
	int    	GetString_CMD_STOPRECVFAX();             
	int    	GetString_CMD_CHANGEMONITORFILTER();	   
	int    	GetString_CMD_SENDIO();	                 
	int    	GetString_CMD_SENDSIGMSG();              
	int    	GetString_CMD_RECORDCSP();	             
	int    	GetString_CMD_DBGON();                   
	int    	GetString_CMD_DBGOFF();                  
	int     GetString_CMD_PLAY_3GPP();              
	int     GetString_CMD_CONTROL_3GPP_PLAY();      
	int     GetString_CMD_RECORD_3GPP();            
	int     GetString_CMD_CONTROL_3GPP_RECORD();    
	int    	GetString_CMD_PLAYCSP();                 
	int    	GetString_CMD_SENDPLAYCSP();             
	int    	GetString_CMD_QUERYLIC();                
	int    	GetString_CMD_CTXREG();                  
	int    	GetString_CMD_CTXLINK();                 
	int    	GetString_CMD_CTXSENDAPPDATA();          
	int    	GetString_CMD_INIT3GPPINDEX();           
	int    	GetString_CMD_BUILD3GPPINDEX();          


protected:


private:
    int  	GetString_PlayProperty(PlayProperty_t *pPlayProperty);
	int     GetString_3GpPlayProperty(Play3gppProperty_t *pPlayProperty);
//-------------def attrib---------//
public:
     CString      m_getmsg;
     MsgSub       m_Submsg;

protected:


private:
	DJ_Void*       m_pkt;
	int            m_msgnum;
	Acs_Cmd_t *	   pAcsCmd;
	CString        m_msg;
	CString        m_tmpmsg;
};



#endif