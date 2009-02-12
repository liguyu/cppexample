#ifndef _DJ_XMS_ANALOG_COMMON_CODE_H
#define _DJ_XMS_ANALOG_COMMON_CODE_H


typedef struct 
{
	// Freq
	DJ_U16		m_u16Freq0;
	DJ_U16		m_u16Freq1;

	// DailTone
	DJ_U16		m_u16DialTone_FreqIndexMask;
	DJ_U16		m_u16DialTone_On_Time;

	// RingBackTone
	DJ_U16		m_u16RingBackTone_FreqIndexMask;
	DJ_U16		m_u16RingBackTone_On_Time;
	DJ_U16		m_u16RingBackTone_Off_Time;
	DJ_U16		m_u16RingBackTone_TimeDeviation;		// in percentage

	// BusyTone0
	DJ_U16		m_u16BusyTone0_FreqIndexMask;
	DJ_U16		m_u16BusyTone0_On_Time;
	DJ_U16		m_u16BusyTone0_Off_Time;
	DJ_U16		m_u16BusyTone0_TimeDeviation;				// in percentage

	// BusyTone1
	DJ_U16		m_u16BusyTone1_FreqIndexMask;
	DJ_U16		m_u16BusyTone1_On_Time;
	DJ_U16		m_u16BusyTone1_Off_Time;
	DJ_U16		m_u16BusyTone1_TimeDeviation;				// in percentage

	// BusyTone2
	DJ_U16		m_u16BusyTone2_FreqIndexMask;
	DJ_U16		m_u16BusyTone2_On_Time;
	DJ_U16		m_u16BusyTone2_Off_Time;
	DJ_U16		m_u16BusyTone2_TimeDeviation;				// in percentage

	// other 
	DJ_U16		m_u16NoDialToneAfterOffHook;
	DJ_U16		m_u16Ringback_NoAnswerTime;

	DJ_U16		m_u16SendFSKCallerIDTime;					// for Analog_User Channel

} TYPE_ANALOG_GTD_PARAM;

long	Analog_Common_Cfg_ReadCfg ( TYPE_ANALOG_GTD_PARAM *pParam_Analog );
void	SetGTD_ToneParam ( ACSHandle_t acsHandle, DeviceID_t *pBoardDevice );

int		ConvertRawFskToCallerID ( unsigned char *RawFSKStr, char *IDStr, int IDStrBufLen );
int		ConvertCallerIDToRawFsk (  char *IDStr, unsigned char *RawFSKStr, int FSKStrBufLen );

#endif