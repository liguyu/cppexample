#ifndef DJACSINTERDEF_H
#define DJACSINTERDEF_H

#include "ItpFlowChanDef.h"
#include "DJAcsDataDef.h"
#include "DJAcsCmdDef.h"
#include <io.h>
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

/*Acs_Fax_InterCmd*/
typedef struct
{
	PITP_RES_CHAN         m_ITPFaxRes;/*fax res*/
    PITP_RES_CHAN         m_ITPFaxVocRes;/*fax voice res*/
    SendFaxParm_t         m_ITPSendFaxParm;/*send fax parm*/
}Acs_Fax_InterCmd;/*Acs_Fax_InterCmd*/

/*Acs_FaxEnd_InterCmd*/
typedef struct
{
    PITP_RES_CHAN         m_ITPFaxVocRes;/*fax voice res*/
    DJ_S32                m_s32EvtState;
    DJ_S32                m_s32ErrCode;
    DJ_U8                 m_u8ErrStep;
    DJ_U8                 m_u8T30CmdState;
    DJ_U8                 m_u8T30RcvState;
    DJ_U16                m_u16TotalPages;
    DJ_S8                 m_s8RemoteID[20];
}Acs_FaxEnd_InterCmd;/*Acs_FaxEnd_InterCmd*/

typedef struct
{
	char szCalling[20];/**/
	char szCalled[20];/**/
	char szOrigCalled[20];/**/
	char szLostTime[40];/**/
}LostCallInfo;/*LostCallInfo*/

typedef struct
{
	DeviceID_t        m_FaxDevice;
	DJ_S8             m_s8T38Shake;
	DJ_S8             m_s8Rfu1[3];
}Fax_VoIP_InterCmd;/*Fax_VoIP_InterCmd*/

typedef struct 
{
	DeviceID_t    m_DeviceID;
	char          m_szCaller[32];
}CallUserInfor;/*CallUserInfor*/

typedef struct
{
	char          m_s8Buff[1024];
}InterCmdStru;/*InterCmdStru*/

#endif