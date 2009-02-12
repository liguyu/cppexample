#include "stdafx.h"

#include "DJAcsDataDef.h"

#include "Analog_Common_Code.h"

char	Analog_Cfg_IniName[] = "..\\Analog_Common_Code\\XMS_Analog_Cfg.INI";


/*************************************************************************************
return 
	0:	OK.
	-1:	Fail, m_u8CalledTableCount Invalid
	-2: Fail, m_u8CalledLen Invalid
	-3: Fail, m_u8CalledTimeOut Invalid
	-4: Fail, m_u8AreaCodeLen Invalid
	-5: Fail, m_CalledTable[x].m_u8NumLen Invalid
*************************************************************************************/
long	Analog_Common_Cfg_ReadCfg ( TYPE_ANALOG_GTD_PARAM *pParam_Analog )
{
	int			iTmp;
//	int			i;
//	char		TmpStr[32], TmpName[32];

	// ------------------------ [Freq] ------------------------
	iTmp = GetPrivateProfileInt ( "Freq", "Freq0", 450, Analog_Cfg_IniName);
	if ( (iTmp < 300) || (iTmp > 3400) )
		return -1;							// m_u16Freq0 Invalid, should be 300-3400 Hz
	pParam_Analog->m_u16Freq0 = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "Freq", "Freq1", 500, Analog_Cfg_IniName);
	if ( (iTmp < 300) || (iTmp > 3400) )
		return -2;							// m_u16Freq1 Invalid, should be 300-3400 Hz
	pParam_Analog->m_u16Freq1 = (DJ_U16) iTmp;

	// ------------------------ [DialTone] ------------------------
	iTmp = GetPrivateProfileInt ( "DialTone", "FreqIndexMask", 3, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 0xFF) )
		return -3;							// m_u16DialTone_FreqIndexMask Invalid, should be 0x00-0xFF
	pParam_Analog->m_u16DialTone_FreqIndexMask = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "DialTone", "On_Time", 1000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -4;							// m_u16DialTone_On_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16DialTone_On_Time = (DJ_U16) iTmp;

	// ------------------------ [RingBackTone] ------------------------
	iTmp = GetPrivateProfileInt ( "RingBackTone", "FreqIndexMask", 3, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 0xFF) )
		return -5;							// m_u16RingBackTone_FreqIndexMask Invalid, should be 0x00-0xFF
	pParam_Analog->m_u16RingBackTone_FreqIndexMask = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "RingBackTone", "On_Time", 1000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -5;							// m_u16RingBackTone_On_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16RingBackTone_On_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "RingBackTone", "Off_Time", 4000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -6;							// m_u16RingBackTone_Off_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16RingBackTone_Off_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "RingBackTone", "TimeDeviation", 10, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 50) )
		return -7;							// m_u16RingBackTone_TimeDeviation Invalid, should be 0%-50%
	pParam_Analog->m_u16RingBackTone_TimeDeviation = (DJ_U16) iTmp;

	// ------------------------ [BusyTone0] ------------------------
	iTmp = GetPrivateProfileInt ( "BusyTone0", "FreqIndexMask", 3, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 0xFF) )
		return -8;							// m_u16BusyTone0_FreqIndexMask Invalid, should be 0x00-0xFF
	pParam_Analog->m_u16BusyTone0_FreqIndexMask = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone0", "On_Time", 1000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -8;							// m_u16BusyTone0_On_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16BusyTone0_On_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone0", "Off_Time", 4000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -10;							// m_u16BusyTone0_Off_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16BusyTone0_Off_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone0", "TimeDeviation", 10, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 50) )
		return -11;							// m_u16BusyTone0_TimeDeviation Invalid, should be 0%-50%
	pParam_Analog->m_u16BusyTone0_TimeDeviation = (DJ_U16) iTmp;

	// ------------------------ [BusyTone1] ------------------------
	iTmp = GetPrivateProfileInt ( "BusyTone1", "FreqIndexMask", 3, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 0xFF) )
		return -8;							// m_u16BusyTone1_FreqIndexMask Invalid, should be 0x00-0xFF
	pParam_Analog->m_u16BusyTone1_FreqIndexMask = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone1", "On_Time", 1000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -8;							// m_u16BusyTone1_On_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16BusyTone1_On_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone1", "Off_Time", 4000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -10;							// m_u16BusyTone1_Off_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16BusyTone1_Off_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone1", "TimeDeviation", 10, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 50) )
		return -11;							// m_u16BusyTone1_TimeDeviation Invalid, should be 0%-50%
	pParam_Analog->m_u16BusyTone1_TimeDeviation = (DJ_U16) iTmp;

	// ------------------------ [BusyTone2] ------------------------
	iTmp = GetPrivateProfileInt ( "BusyTone2", "FreqIndexMask", 3, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 0xFF) )
		return -8;							// m_u16BusyTone2_FreqIndexMask Invalid, should be 0x00-0xFF
	pParam_Analog->m_u16BusyTone2_FreqIndexMask = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone2", "On_Time", 1000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -8;							// m_u16BusyTone2_On_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16BusyTone2_On_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone2", "Off_Time", 4000, Analog_Cfg_IniName);
	if ( (iTmp < 100) || (iTmp > 5000) )
		return -10;							// m_u16BusyTone2_Off_Time Invalid, should be 100-5000 ms
	pParam_Analog->m_u16BusyTone2_Off_Time = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "BusyTone2", "TimeDeviation", 10, Analog_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 50) )
		return -11;							// m_u16BusyTone2_TimeDeviation Invalid, should be 0%-50%
	pParam_Analog->m_u16BusyTone2_TimeDeviation = (DJ_U16) iTmp;

	// ------------------------ [Other] ------------------------
	iTmp = GetPrivateProfileInt ( "Other", "NoDialToneAfterOffHook", 5000, Analog_Cfg_IniName);
	if ( (iTmp < 1000) || (iTmp > 10000) )
		return -11;							// m_u16NoDialToneAfterOffHook Invalid, should be 1-10 sec
	pParam_Analog->m_u16NoDialToneAfterOffHook = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "Other", "Ringback_NoAnswerTime", 40000, Analog_Cfg_IniName);
	if ( (iTmp < 10000) || (iTmp > 60000) )
		return -11;							// m_u16Ringback_NoAnswerTime Invalid, should be 10-60 sec
	pParam_Analog->m_u16Ringback_NoAnswerTime = (DJ_U16) iTmp;

	iTmp = GetPrivateProfileInt ( "Other", "SendFSKCallerIDTime", 1500, Analog_Cfg_IniName);
	if ( (iTmp < 1000) || (iTmp > 5000) )
		return -11;							// m_u16SendFSKCallerIDTime Invalid, should be 1000-5000 ms
	pParam_Analog->m_u16SendFSKCallerIDTime = (DJ_U16) iTmp;

	return 0;		// OK
}

// ---------------------------------------------------------------
// multiple format
void HandleMultiple ( char far *IDStr )
{
	int i;
	char TmpIDStr[80];
	char far *p;
	char c;

	p = IDStr;

	i = 0;
	while ( *p != 0 ) 
	{
		c = *p;
		if ( (c==1) || (c==2) || (c==4) || (c==7) || (c==8) ) 
		{
			p += 2;
		}
		else 
		{
			TmpIDStr[i] = *p;
			i ++;
			p ++;
		}
	}
	TmpIDStr[i] = 0;
	strcpy ( IDStr, TmpIDStr );
}
// end of multiple

int	ConvertRawFskToCallerID ( unsigned char *RawFSKStr, char *IDStr, int IDStrBufLen )
{
	WORD			count;
	WORD			i,j;
	WORD			pos,len;
	unsigned char	ChkSum;

	count	= strlen ( (char *)RawFSKStr );
	strcpy ( IDStr, "" );

	if ( count == 0 )
	{
		return -1;		// not a good Caller ID
	}

	// find the Format
	for ( i = 0; i < count; i ++ ) 
	{
		if ( RawFSKStr[i] == 0x04 )		// single format
		{
			break;
		}
		
		if ( RawFSKStr[i] == 0x80 ) {		// multiple format
			break;
		}
	}

	if ( (i == count) || (i==count-1) )
	{
		return -1;		// not a good Caller ID
	}

	// start fetch the Caller ID
	pos = i;
	len = RawFSKStr[pos+1];

	// "count-pos-2"    is the length of now received
	for ( i = pos+2,j = 0; (i<(count-1)) && (j<len); i ++,j++ )
	{
		IDStr[j] = RawFSKStr[i] & 0x7F;
		if ( j == IDStrBufLen - 1 )				// it will be overflow
			break;
	}
	IDStr[j] = 0;

	if ( (count-pos-3) < len  )
	{
		return -1;		// not a good Caller ID
	}

	ChkSum = 0;
	for ( i = pos; i < pos+len+3; i ++ )
			ChkSum += RawFSKStr[i];

// multiple format
	if ( RawFSKStr[pos] == 0x80 ) 
	{
		HandleMultiple ( IDStr );
	}
// end of multiple

	if ( ChkSum == 0 )
		return	0;		// OK
	else
		return -1;		// Fail
}

int	ConvertCallerIDToRawFsk (  char *IDStr, unsigned char *RawFSKStr, int FSKStrBufLen )
{
	int				pos, len;
	int				i;
	unsigned char	ChkSum;
	char			TmpDateTimeStr[16];

	// get the Tmp Data Time
	SYSTEMTIME		ST;
	GetLocalTime ( &ST );
	sprintf ( TmpDateTimeStr, "%02d%02d%02d%02d", ST.wMonth, ST.wDay, ST.wHour, ST.wMinute );

	pos = 0;
	len = strlen(TmpDateTimeStr) + strlen(IDStr);

	RawFSKStr[pos++] = 0x04;			// we use single format
	RawFSKStr[pos++] = len;	

	for ( i = 0; i < (int)strlen(TmpDateTimeStr); i ++ )
	{
		RawFSKStr[pos++] = TmpDateTimeStr[i];
		if ( pos >= (FSKStrBufLen-1) )		// overflow
			break;
	}

	for ( i = 0; i < (int)strlen(IDStr); i ++ )
	{
		RawFSKStr[pos++] = IDStr[i];
		if ( pos >= (FSKStrBufLen-1) )		// overflow
			break;
	}

	// calculate the CheckSum
	ChkSum = 0;
	for ( i = 0; i < pos; i ++ )
		ChkSum += RawFSKStr[i];

	ChkSum = 0 - ChkSum;

	RawFSKStr[pos++] = ChkSum;

	return pos;		// OK
}
