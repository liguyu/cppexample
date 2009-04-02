#include "stdafx.h"

#include "DJAcsDataDef.h"

#include "CAS_Common_Cfg.h"


char	CAS_Cfg_IniName[] = "C:\\DJXMS\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI";

/*************************************************************************************
return 
	0:	OK.
	-1:	Fail, m_u8CalledTableCount Invalid
	-2: Fail, m_u8CalledLen Invalid
	-3: Fail, m_u8CalledTimeOut Invalid
	-4: Fail, m_u8AreaCodeLen Invalid
	-5: Fail, m_CalledTable[x].m_u8NumLen Invalid
*************************************************************************************/
long	CAS_Common_Cfg_ReadCfg ( CmdParamData_CAS_t *pParam_CAS )
{
	int			iTmp;
	int			i;
	char		TmpStr[32], TmpName[32];

	// ------------------------ [Rule] ------------------------
	// m_u8CalledTableCount
	iTmp = GetPrivateProfileInt ( "Rule", "CalledTableCount", 0, CAS_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 16) )
		return -1;							// m_u8CalledTableCount Invalid
	pParam_CAS->m_u8CalledTableCount = (DJ_U8) iTmp;

	// m_u8CalledLen
	iTmp = GetPrivateProfileInt ( "Rule", "CalledLen", 5, CAS_Cfg_IniName);
	if ( (iTmp <= 0) || (iTmp > 32) )
		return -2;							// m_u8CalledLen Invalid
	pParam_CAS->m_u8CalledLen = (DJ_U8) iTmp;

	// m_u8CalledTimeOut
	iTmp = GetPrivateProfileInt ( "Rule", "CalledTimeOut", 0, CAS_Cfg_IniName);
	if ( (iTmp < 0) || (iTmp > 10) )
		return -3;							// m_u8CalledTimeOut Invalid
	pParam_CAS->m_u8CalledTimeOut = (DJ_U8) iTmp;

	// m_u8NeedCaller
	iTmp = GetPrivateProfileInt ( "Rule", "NeedCaller", 0, CAS_Cfg_IniName);
	if ( iTmp != 0 )
		iTmp = 1;
	pParam_CAS->m_u8NeedCaller = (DJ_U8) iTmp;

	// m_u8AreaCodeLen
	iTmp = GetPrivateProfileInt ( "Rule", "AreaCodeLen", 0, CAS_Cfg_IniName);
	if ( pParam_CAS->m_u8NeedCaller != 0 )		// need caller
	{
		if ( (iTmp <0) || (iTmp > 10) )
			return -4;						// m_u8AreaCodeLen Invalid
	}
	pParam_CAS->m_u8AreaCodeLen = (DJ_U8) iTmp;

	// m_u8ControlMode
	iTmp = GetPrivateProfileInt ( "Rule", "ControlMode", 0, CAS_Cfg_IniName);
	if ( (iTmp != 0) && (iTmp != 1) && (iTmp != 2) )
		iTmp = 0;
	pParam_CAS->m_u8ControlMode = (DJ_U8) iTmp;

	// ------------------------ [CalledTable] ------------------------
	memset ( pParam_CAS->m_CalledTable, 0, sizeof(CAS_CalledTableDesc_t) * 16 ); 
	for ( i = 0; i < pParam_CAS->m_u8CalledTableCount; i ++ )
	{
		// m_u8NumHeadLen & m_u8NumHead
		sprintf ( TmpName, "NumHead[%d]", i );
		GetPrivateProfileString ( "CalledTable", TmpName, "168", TmpStr, 14, CAS_Cfg_IniName);
		TmpStr[14] = 0;
		pParam_CAS->m_CalledTable[i].m_u8NumHeadLen = strlen(TmpStr); 
		memcpy( pParam_CAS->m_CalledTable[i].m_u8NumHead, TmpStr, pParam_CAS->m_CalledTable[i].m_u8NumHeadLen ); 

		// m_u8NumLen
		sprintf ( TmpName, "NumLen[%d]", i );
		iTmp = GetPrivateProfileInt ( "CalledTable", TmpName, 5, CAS_Cfg_IniName);
		if ( (iTmp < 0) || (iTmp > 16) )
			return -5;							// m_CalledTable[x].m_u8NumLen Invalid
		pParam_CAS->m_CalledTable[i].m_u8NumLen = (DJ_U8) iTmp;
	}

	// ------------------------ Other ------------------------
	// Call Out Parameter, set to default value directly.
	pParam_CAS->m_u8KA = 1;
	pParam_CAS->m_u8KD = 3;

	return	0;		// OK
}

