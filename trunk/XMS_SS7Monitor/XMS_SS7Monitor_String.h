#ifndef _XMS_SS7MONITOR_STRING_H_
#define _XMS_SS7MONITOR_STRING_H_


char	*GetString_EventType ( EventType_t EvtType );
char	*GetString_DeviceMain ( DJ_S32	s32DeviceType );
char	*GetString_DeviceSub ( DJ_S32	s32DeviceSub );
char	*GetString_ErrorCode ( DJ_S32	s32ErrorCode );
char	*GetString_PcmType ( int iPcmType );
void     GetString_LineState ( char *StateStr, int iLineState );

#endif
