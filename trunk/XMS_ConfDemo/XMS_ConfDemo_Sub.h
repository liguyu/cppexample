#ifndef _XMS_CONFDEMO_SUB_H_
#define _XMS_CONFDEMO_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

#define		MAX_MEMBER_PER_CONF_GROUP		128

#define		CONF_MODE_NONE			0
#define		CONF_MODE_ADD			1
#define		CONF_MODE_LISTEN		2
#define		CONF_MODE_SPEAKONLY		3
#define		CONF_MODE_PLAY			4

// -----------------------------------------------------------------------------
enum SEARCH_RULE
{
	SEARCH_RULE_SAME_MODULE = 1,
	SEARCH_RULE_MAX_FREE_FIST = 2,
};


enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,			
	TRK_SELECT,		
	TRK_CONF_INPUTGROUP,		
	TRK_CONF_CONFING,
	TRK_CONF_KARAOK,
	TRK_CONF_PLAYBACK,
	TRK_CONF_ERROR,		

	// new add for XMS
	TRK_WAIT_ANSWERCALL,
	TRK_WAIT_LINKOK,

	TRK_WAIT_REMOVE,
};


enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED,
	
	VOC_WAIT_REMOVE,
} ;

enum VOIP_STATE {
	VOIP_WAITOPEN,
	VOIP_FREE,
	VOIP_SELECT,		
	VOIP_CONF_INPUTGROUP,		
	VOIP_CONF_CONFING,
	VOIP_CONF_KARAOK,
	VOIP_CONF_PLAYBACK,
	VOIP_CONF_ERROR,		
	
	// new add for XMS
	VOIP_WAIT_ANSWERCALL,
	VOIP_WAIT_LINKOK,
	
	VOIP_WAIT_REMOVE,
} ;

enum CONF_STATE {
	CONF_WAITOPEN,
	CONF_FREE,
	CONF_USED,
	CONF_WAIT_REMOVE,
};

// IP call party address struct
typedef struct
{
	TCHAR	tszAddr[20];
	USHORT	port;
	TCHAR	tszUserID[20];
	TCHAR	tszTelNo[20];

} VOIP_ADDR;

// ----------------------------------------------------------
typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;

	DeviceID_t	ConfDevID;
	// -----------------
	TRUNK_STATE	State;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

	int		iConfSelect;
} TRUNK_STRUCT;

// IP Device struct
typedef struct
{
	BOOL			bOpened;		// if opened
	DeviceID_t		deviceID;			// XMS device id
	int				iSeqID;
	int				iProtocol;		// ip protocol (h.323/sip)
	VOIP_ADDR		addrCaller;		// caller address
	VOIP_ADDR		addrCallee;		// callee address
	int				DtmfCount;
	char			DtmfBuf[32];
	VOIP_STATE		State;
	
	DeviceID_t		VocDevID;
	DJ_U8			u8PlayTag;

	DeviceID_t		ConfDevID;
	int				iConfSelect;

} VOIP_STRUCT;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;

	DeviceID_t	UsedDevID;
	// ----------------
	VOICE_STATE	State;

} VOICE_STRUCT;

typedef struct
{
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// flag of OpenDevice OK
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

typedef struct
{
	long		lMode;
	DeviceID_t	DevID;
	int			devType;  //1:trunk device 2:VOIP device
} MEMBER_STRUCT;

typedef struct
{	
	int				iSeqID;
	DeviceID_t		deviceID;
	
	DeviceID_t		UsedDevID;
	CONF_STATE		State;

	DeviceID_t		VocDevID;
	long			lMemberNum;
	long			lListenNum;
	MEMBER_STRUCT	Member[MAX_MEMBER_PER_CONF_GROUP];

} CONF_STRUCT;

// ----- declare function -----
bool	InitSystem(void);
void	ExitSystem(void); 

void	AddMsg ( char *str);

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );
void	ReOpen_AllDevice (void);

void	HandleDevState ( Acs_Evt_t *pAcsEvt );

void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );
void	CloseDeviceOK ( DeviceID_t *pDevice );

void	Remove_OneDsp(void);

DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue = false );

void	TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	TrunkWork_VOIP ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt );

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );

void	InitConfChannel ( CONF_STRUCT *pOneConf );
void	InitVOIPChannel ( VOIP_STRUCT *pOneVOIP );
int		SearchOneFreeVoice (  DeviceID_t *pFreeVocDeviceID, SEARCH_RULE SearchRule, DJ_S8 s8RefModID );
void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );
void	Change_Voc_State ( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState );
void	Change_Conf_State ( CONF_STRUCT *pOneConf, CONF_STATE NewState );

#endif