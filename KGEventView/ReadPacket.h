#if !defined(AFX_READPACKET_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_)
#define AFX_READPACKET_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "ITPDataDefine.h"
#include "DJAcsCmdDef.h"
#include "ITPGUID.h"

#define  __MAX_PACKET__DATA		3000

#define  FetchApiData(pAcsCmd)  ((DJ_S8 *)pAcsCmd + sizeof(Acs_Cmd_t))
#define  FetchApiDeviceList(pAcsCmd) ((DJ_S8 *)FetchEventData(pAcsCmd) + sizeof(Acs_Dev_List_Head_t))
#define  FetchApiIOData(pAcsCmd)     ((DJ_S8 *)FetchEventData(pAcsCmd) + sizeof(Acs_IO_Data))
#define  FetchApiParamData(pAcsCmd)  ((DJ_S8 *)FetchEventData(pAcsCmd) + sizeof(Acs_ParamProc_Data))

enum   PKT_TYPE
{
     APP_CMD =1,
     APP_EVT =2,
};
class ReadPacket
{
public:

      ReadPacket(DJ_Void *pkt, int nNumRecv);
	  ~ReadPacket();
      int      GetPktType();
	  int      GetPktEvt();
	  int      GetPktAPI();

	  CString  GetString_EventType ();
	  CString  GetString_DeviceMain (DJ_S32	s32DeviceMain);
	  CString  GetString_DeviceSub (DJ_S32	s32DeviceMain, DJ_S32	s32DeviceSub );
	  CString  GetString_DeviceAll (DeviceID_t *pDevID);
	  CString  GetString_ErrorCode ( DJ_S32	s32ErrorCode );
	  CString  GetString_PlayControlType ( DJ_U16 u16ControlType );
	  CString  GetString_RecordControlType ( DJ_U32 u32ControlType );
	  CString  GetString_GtgType ( DJ_U16	u16GtgType );
	  CString  GetString_DeviceState ( DJ_U32	s32DeviceState );
	  CString  GetString_IOType ( DJ_U16 u16IOType );
	  CString  GetString_AnalogInterfaceState(DJ_S16 u16DevSub,DJ_U8 u8Interface);
	  CString  GetString_FaxState(DJ_U32 u32State);
	  CString  GetString_APIType ();

	  typedef struct
	  {
       CString   msgType;
	   CString   msg;
	   CString   dev;
	  }pkt_cvt;
	  pkt_cvt          m_convert;

	  typedef struct  
	  {
		int    iAPI;
		int    iEvt;
		int    iDev;
		int    iDsp;
		int    iChnl;
	  }id_num;
	  int      GetAPIID();
	  int      GetEvtID();
	  int      GetDevID(DJ_S32	s32DeviceMain);
      id_num   pkd_IDinfo;
      int              f_head;
protected:

private:
     DJ_Void  *       m_pkt;
	 int              nmsgnum;
	 bool             EvtFlag;
	 Acs_Evt_t *	  pAcsEvt;
	 CString          m_msg;
	 Acs_Cmd_t *      pAcsCmd;
	 PKG_HEAD_STRUCT  *pktHead;
};




#endif