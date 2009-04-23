#ifndef _XMS_DIAL_SUB_H_
#define _XMS_DIAL_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

enum SEARCH_RULE
{
	SEARCH_RULE_FIX_MODULE = 0,
	SEARCH_RULE_SAME_MODULE = 1,
	SEARCH_RULE_MAX_FREE_FIST = 2,
};

enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,			
	TRK_NOTHANDLE,
	TRK_CALLOUT,	
	TRK_FAX_SEND,
	TRK_FAX_RECEIVE,
	TRK_FAIL,
	TRK_WAITHANGUP,
	TRK_WAITSTOP,
};


typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DeviceID_t  VocDevID_ForRec;
	DeviceID_t  FaxDevID;

	DJ_U32		u32FaxCount;
	DJ_U32      u32FaxSendFail;
	DJ_U32      u32FaxRecvFail;

	// -----------------
	TRUNK_STATE	State;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

} TRUNK_STRUCT;

enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED
} ;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;

	DeviceID_t	UsedDevID;

	// ----------------
	VOICE_STATE	State;

} VOICE_STRUCT;

enum FAX_STATE
{
	FAX_WAITOPEN,
	FAX_FREE,
	FAX_USED
};

typedef struct
{
	//----------------
	DeviceID_t  deviceID;
	int         iSeqID;
	int			iLineState;

	DeviceID_t  UsedDevID;

	FAX_STATE   State;
}FAX_STRUCT;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

// ----- decalure function -----
bool	InitSystem(void);
void	ExitSystem(void); 

void	AddMsg ( char *str);

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );

void	HandleDevState ( Acs_Evt_t *pAcsEvt );

void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );

void	TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );

void    InitTrunkcounter(TRUNK_STRUCT *pOneTrunk);

void	SimulateCallOut(void);

void   WriteMsgLog(char *pMsg);

void   CreateFileDirector(DJ_S8 s8DspMod);


#endif