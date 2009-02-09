/*
*在程序界面显示内容方面的字符处理
*/
#ifndef _XMS_SWITCHDEMO_STRING_H_
#define _XMS_SWITCHDEMO_STRING_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
char * GetString_EventType ( EventType_t EvtType );
char * GetString_DeviceMain ( DJ_S32	s32DeviceMain );
char * GetString_ErrorCode ( DJ_S32	s32ErrorCode );
char * GetString_PcmType ( int iPcmType );
char * GetString_DeviceSub ( DJ_S32	s32DeviceSub );
void   GetString_LineState ( char *StateStr, int iLineState );

#endif