//*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ITPUserInterface.h
//Version       :  1.1                                                              
//Author        :  hewei                                                             
//DateTime      :  2006-05-08 15:00                                           
//Description  :   ITP用户接口定义文件
//*****************************************************************************

#ifndef _ITPUSERINTERFACE_H
#define _ITPUSERINTERFACE_H

#include "ITPDataDefine.h"
#include "ITPMsgPublic.h"

#define ITP_MAX_MOD_TYPE_NUM                        128    // 最大模块数
#define ITP_MAX_UNIT_NUM                            128    // 最大UNIT数目
#define ITP_MAX_CH_NUM                              128    // 最大通道数目

/************************************************************************/
/*功能描述:    获取本模块的UnitID                                       */
/*输入参数:    无                                                       */
/*输出参数:    无                                                       */
/*返回值  :    模块UnitID                                               */
typedef DJ_U8     (*PFUNC_GETSELFUNITID)();
/************************************************************************/

/************************************************************************/
/*功能描述:    获取本模块的运行状态                                     */
/*输入参数:    无                                                       */
/*输出参数:    无                                                       */
/*返回值  :                                                             */
/*             0x01    //模块已初始化                                   */ 
/*             0x02    //模块已启动                                     */
/*             0x04    //模块已全部被其上级模块关联                     */ 
/*             0x08    //模块已被监视                                   */
/*             0x10    //模块已连接                                     */
/*             0x20    //模块正在停止                                   */ 
/*             0x40    //正在运行模块                                   */
typedef DJ_U32    (*PFUNC_GETMODSTATE)();
/************************************************************************/

/************************************************************************/
/*功能描述:    向其他模块发送数据包                                     */
/*输入参数:                                                             */
/*             u8ModType: 目的模块类型                                  */
/*             u8UnitId : 目的模块UnitID                                */
/*             pData    : 发送的ITP数据,包含ITP包头                     */
/*输出参数:    无                                                       */
/*返回值  :    0:成功; -1:失败                                          */
typedef DJ_S32    (*PFUNC_SENDPACK)(DJ_U8 u8ModType,DJ_U8 u8UnitId,PPKG_HEAD_STRUCT pData);
/************************************************************************/

/************************************************************************/
/*功能描述:    向其他模块重发送数据包                                   */
/*输入参数:                                                             */
/*             u8ModType: 目的模块类型                                  */
/*             u8UnitId : 目的模块UnitID                                */
/*             pData    : 发送的ITP数据,包含ITP包头                     */
/*输出参数:    无                                                       */
/*返回值  :    0:成功; -1:失败                                          */
typedef DJ_S32    (*PFUNC_RESENDPACK)(DJ_U8 u8ModType,DJ_U8 u8UnitId,PPKG_HEAD_STRUCT pData);
/************************************************************************/

/************************************************************************/
/*功能描述:    获取指定模块的IP和port                                   */
/*输入参数:                                                             */
/*             u8ModType: 模块类型                                      */
/*             u8UnitId:  模块的UnitID                                  */
/*输出参数:    s8IPAddr:  返回的模块IP                                  */
/*返回值:      -1:失败; >0: 模块port                                    */
typedef DJ_S32    (*PFUNC_GETMODIPPORT)(DJ_U8 u8ModType,DJ_U8 u8UnitId,DJ_S8 * s8IPAddr);
/************************************************************************/

/************************************************************************/
/*功能描述:    获取本模块的ini文件名                                    */
/*输入参数:    无                                                       */
/*输出参数:    s8IniFile: 返回的ini文件名                               */
/*返回值:      0:成功; -1:失败                                          */
typedef DJ_S32    (*PFUNC_GETMODINIFILE)(DJ_S8 * s8IniFile);
/************************************************************************/

/************************************************************************/
/*功能描述:    获取其他模块的UnitID                                     */
/*输入参数:    无                                                       */
/*输出参数:    无                                                       */
/*返回值  :    模块UnitID                                               */
typedef DJ_U8     (*PFUNC_GETMODUNITID)(DJ_SOCKET);
/************************************************************************/

/************************************************************************/
/*功能描述:    获取指定模块的所有UnitID                                     */
/*输入参数:    无                                                       */
/*输出参数:    无                                                       */
/*返回值  :    模块UnitID                                               */
typedef DJ_U8     (*PFUNC_GETMODALLUNITID)(DJ_U8 u8ModType,DJ_U8 u8UnitID[]);
/************************************************************************/

//回调接口函数指针
typedef struct
{
	PFUNC_GETSELFUNITID    m_pGetSelfUnitId;
	PFUNC_GETMODSTATE      m_pGetModState;
	PFUNC_SENDPACK         m_pSendPack;
	PFUNC_RESENDPACK       m_pReSendPack;
	PFUNC_GETMODIPPORT     m_pGetModIpPort;
	PFUNC_GETMODINIFILE    m_pGetModIniFile;
	PFUNC_GETMODUNITID     m_pGetModUnitID;
	PFUNC_GETMODALLUNITID  m_pGetModAllUnitID;
}ITP_MOD_CALLBACK,*PITP_MOD_CALLBACK;

extern ITP_MOD_CALLBACK     g_ITPCallBack;

#define   GetSelfUnitId    g_ITPCallBack.m_pGetSelfUnitId
#define   GetModState      g_ITPCallBack.m_pGetModState
#define   SendPack         g_ITPCallBack.m_pSendPack
#define   ReSendPack       g_ITPCallBack.m_pReSendPack
#define   GetModIpPort     g_ITPCallBack.m_pGetModIpPort
#define   GetModIniFile    g_ITPCallBack.m_pGetModIniFile
#define   GetModUnitID     g_ITPCallBack.m_pGetModUnitID
#define   GetModAllUnitID  g_ITPCallBack.m_pGetModAllUnitID

#endif
