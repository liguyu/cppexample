#ifndef _XMSAPI_TEST_STRING_H_
#define _XMSAPI_TEST_STRING_H_

char *GetString_EventType ( EventType_t EvtType );
char *GetString_DeviceMain ( DJ_S32	s32DeviceType );
char *GetString_DeviceSub (DJ_S32	s32DeviceMain, DJ_S32	s32DeviceSub );
char *GetString_DeviceAll ( DeviceID_t *pDevID );
char *GetString_ErrorCode ( DJ_S32	s32ErrorCode );
char *GetString_PlayControlType ( DJ_U16 u16ControlType );
char *GetString_RecordControlType ( DJ_U32 u32ControlType );
char *GetString_GtgType ( DJ_U16	u16GtgType );
char *GetString_DeviceState ( DJ_U32	s32DeviceState );
char *GetString_IOType ( DJ_U16 u16IOType );
char* GetString_AnalogInterfaceState(DJ_S16 u16DevSub,DJ_U8 u8Interface);
char* GetString_FaxState(DJ_U32 u32State);


#endif