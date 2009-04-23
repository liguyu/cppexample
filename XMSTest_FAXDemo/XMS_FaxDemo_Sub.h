#ifndef _XMS_FAXDEMO_SUB_H_
#define _XMS_FAXDEMO_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

enum SEARCH_RULE
{
	SEARCH_RULE_FIX_MODULE = 0,
	SEARCH_RULE_SAME_MODULE = 1,
	SEARCH_RULE_MAX_FREE_FIST = 2,
};
/*
enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,
	TRK_NOTHANDLE,
	TRK_FAX_SEND,
	TRK_FAX_RECEIVE,	
	TRK_WAITHANGUP,

	// new add for XMS
	TRK_WAIT_ANSWERCALL	
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
	DeviceID_t	FaxDevID;
	DJ_U8		u8PlayTag;	

	// -----------------
	TRUNK_STATE	State;
	DJ_U32      u32FaxCount;
	DJ_U32      u32FaxSendFail;
	DJ_U32      u32FaxRecvFail;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

} TRUNK_STRUCT;
*/
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

// add by chenwy 2009-02-18
enum VOIP_STATE 
{
	VOIP_WAITOPEN,
	VOIP_FREE,
	VOIP_NOTHANDLE,
	VOIP_FAX_T38START,
	VOIP_FAX_SEND,
	VOIP_FAX_RECEIVE,
	VOIP_WAITHANGUP,

	// new add for XMS
	VOIP_WAIT_ANSWERCALL
};
// IP call party address struct
typedef struct
{
	TCHAR	tszAddr[20];
	USHORT	port;
	TCHAR	tszUserID[20];
	TCHAR	tszTelNo[20];
} VOIP_ADDR_STRUCT;

// IP Device struct
typedef struct
{
	DeviceID_t		deviceID;		// XMS device id
	int			iSeqID;
	DeviceID_t		VocDevID;
	DJ_U8			u8PlayTag;

	DeviceID_t		FaxDevID;
	int			DtmfCount;
	char			DtmfBuf[32];

	DJ_U32			u32FaxCount;
	DJ_U32			u32FaxSendFail;
	DJ_U32			u32FaxRecvFail;
	int			iLineState;

	int			iProtocol;		// ip protocol (h.323/sip)
	VOIP_STATE		iState;			// call state
	VOIP_ADDR_STRUCT	addrCaller;		// caller address
	VOIP_ADDR_STRUCT	addrCallee;		// callee address
	char			szDtmf[6];		// dtmf string
	int			iVoiceDevice;		// binded Voice Device index
} VOIP_STRUCT;
// add end
// ----- decalure function -----
bool	InitSystem(void);
void	ExitSystem(void); 

void	AddMsg ( char *str);

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );

void	HandleDevState ( Acs_Evt_t *pAcsEvt );

//void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );
void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );

void	My_BuildIndex ( DeviceID_t	*pVocDevID );
void	Change_State ( VOIP_STRUCT *pOneVoip, VOIP_STATE NewState );

DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue = false );
DJ_S32	 PlayIndex ( DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, bool bIsQueue );

//void	TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	VoipWork ( VOIP_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );// add by chenwy 2008-02-01

void   CreateFileDirector(DJ_S8 s8DspModID);

void    WriteMsgLog(char *pMsg);

DWORD WINAPI ThreadFunc(VOID);

#endif