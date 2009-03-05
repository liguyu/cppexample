#ifndef COMMONTOOL_H
#define COMMONTOOL_H

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "ITPDataDefine.h"
#include "DJAcsCmdDef.h"
#include "DJAcsTUPDef.h"
#include "DJAcsISUPDef.h"

#define   KEY_LEN              3000
#define   _MUX_MSG_DETAIL      30
#define   _MUX_CHECK_ITEM      150

CString    GetString_ParaType(DeviceID_t *pDev, DJ_U16 Type);
CString    GetString_DeviceMain (DJ_S32	s32DeviceMain);
CString    GetString_DeviceSub (DJ_S32	s32DeviceMain, DJ_S32	s32DeviceSub );
CString    GetString_DeviceAll (DeviceID_t *pDevID);
CString    GetString_MediaSrcMod (DJ_U8 SrcMod);
CString    GetString_MediaDecodeType(DJ_U8 DecType);
CString    GetString_PlayCtrlType(DJ_U16 playType);
CString    GetString_MixerType(DJ_U8 Type);
CString    GetString_MediaStopMod(DJ_U32 Type);
CString    GetString_RecordCtrlType(DJ_U32 RecordType);
CString    GetString_IOEvtMask(DJ_U32 EvtType);
CString    GetString_IOType(DeviceID_t *pDev, DJ_U16 IOType);
CString    GetString_CtxLinkType(DJ_U32 CtxLinkType);
int        HextoStr(DJ_U8 *msgbuf,DJ_S8 * Transbuf,int nNumRecv);
void       StrtoHex(unsigned   char   *lpBuffer,   int   *Total);
BOOL       TransData(CString   InStr,   DJ_U8*   OutStr);
CString    HextotoStr(DJ_Void *msgbuf,int nNumRecv);
CString    HextoChars(DJ_S8 *msgbuf,int nNumRecv);

typedef struct
{
   int      nNUm; 
   CString  n_MSgSrc[_MUX_MSG_DETAIL];
   CString  n_Msg[_MUX_MSG_DETAIL];
   CString  n_MsgAnly[_MUX_MSG_DETAIL];
}MsgSub;
typedef struct
{
   BOOL   IsCheck;
}IF_IS_CHECK;
typedef struct  
{
	int         iAPIcheckNum;
	int         iEVTcheckNum;
	int         iDEVcheckNum;
	int         iDSPcheckNum;
	int         iCHNLcheckNum;
	IF_IS_CHECK API[_MUX_CHECK_ITEM];
	IF_IS_CHECK EVT[_MUX_CHECK_ITEM];
	IF_IS_CHECK DEV[_MUX_CHECK_ITEM];
	IF_IS_CHECK DSP[_MUX_CHECK_ITEM];     
	IF_IS_CHECK CHNL[_MUX_CHECK_ITEM];
}CHECK_INFO;
#endif