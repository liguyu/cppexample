//*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ITPUserInterface.h
//Version       :  1.1                                                              
//Author        :  hewei                                                             
//DateTime      :  2006-05-08 15:00                                           
//Description  :   ITP�û��ӿڶ����ļ�
//*****************************************************************************

#ifndef _ITPUSERINTERFACE_H
#define _ITPUSERINTERFACE_H

#include "ITPDataDefine.h"
#include "ITPMsgPublic.h"

#define ITP_MAX_MOD_TYPE_NUM                        128    // ���ģ����
#define ITP_MAX_UNIT_NUM                            128    // ���UNIT��Ŀ
#define ITP_MAX_CH_NUM                              128    // ���ͨ����Ŀ

/************************************************************************/
/*��������:    ��ȡ��ģ���UnitID                                       */
/*�������:    ��                                                       */
/*�������:    ��                                                       */
/*����ֵ  :    ģ��UnitID                                               */
typedef DJ_U8     (*PFUNC_GETSELFUNITID)();
/************************************************************************/

/************************************************************************/
/*��������:    ��ȡ��ģ�������״̬                                     */
/*�������:    ��                                                       */
/*�������:    ��                                                       */
/*����ֵ  :                                                             */
/*             0x01    //ģ���ѳ�ʼ��                                   */ 
/*             0x02    //ģ��������                                     */
/*             0x04    //ģ����ȫ�������ϼ�ģ�����                     */ 
/*             0x08    //ģ���ѱ�����                                   */
/*             0x10    //ģ��������                                     */
/*             0x20    //ģ������ֹͣ                                   */ 
/*             0x40    //��������ģ��                                   */
typedef DJ_U32    (*PFUNC_GETMODSTATE)();
/************************************************************************/

/************************************************************************/
/*��������:    ������ģ�鷢�����ݰ�                                     */
/*�������:                                                             */
/*             u8ModType: Ŀ��ģ������                                  */
/*             u8UnitId : Ŀ��ģ��UnitID                                */
/*             pData    : ���͵�ITP����,����ITP��ͷ                     */
/*�������:    ��                                                       */
/*����ֵ  :    0:�ɹ�; -1:ʧ��                                          */
typedef DJ_S32    (*PFUNC_SENDPACK)(DJ_U8 u8ModType,DJ_U8 u8UnitId,PPKG_HEAD_STRUCT pData);
/************************************************************************/

/************************************************************************/
/*��������:    ������ģ���ط������ݰ�                                   */
/*�������:                                                             */
/*             u8ModType: Ŀ��ģ������                                  */
/*             u8UnitId : Ŀ��ģ��UnitID                                */
/*             pData    : ���͵�ITP����,����ITP��ͷ                     */
/*�������:    ��                                                       */
/*����ֵ  :    0:�ɹ�; -1:ʧ��                                          */
typedef DJ_S32    (*PFUNC_RESENDPACK)(DJ_U8 u8ModType,DJ_U8 u8UnitId,PPKG_HEAD_STRUCT pData);
/************************************************************************/

/************************************************************************/
/*��������:    ��ȡָ��ģ���IP��port                                   */
/*�������:                                                             */
/*             u8ModType: ģ������                                      */
/*             u8UnitId:  ģ���UnitID                                  */
/*�������:    s8IPAddr:  ���ص�ģ��IP                                  */
/*����ֵ:      -1:ʧ��; >0: ģ��port                                    */
typedef DJ_S32    (*PFUNC_GETMODIPPORT)(DJ_U8 u8ModType,DJ_U8 u8UnitId,DJ_S8 * s8IPAddr);
/************************************************************************/

/************************************************************************/
/*��������:    ��ȡ��ģ���ini�ļ���                                    */
/*�������:    ��                                                       */
/*�������:    s8IniFile: ���ص�ini�ļ���                               */
/*����ֵ:      0:�ɹ�; -1:ʧ��                                          */
typedef DJ_S32    (*PFUNC_GETMODINIFILE)(DJ_S8 * s8IniFile);
/************************************************************************/

/************************************************************************/
/*��������:    ��ȡ����ģ���UnitID                                     */
/*�������:    ��                                                       */
/*�������:    ��                                                       */
/*����ֵ  :    ģ��UnitID                                               */
typedef DJ_U8     (*PFUNC_GETMODUNITID)(DJ_SOCKET);
/************************************************************************/

/************************************************************************/
/*��������:    ��ȡָ��ģ�������UnitID                                     */
/*�������:    ��                                                       */
/*�������:    ��                                                       */
/*����ֵ  :    ģ��UnitID                                               */
typedef DJ_U8     (*PFUNC_GETMODALLUNITID)(DJ_U8 u8ModType,DJ_U8 u8UnitID[]);
/************************************************************************/

//�ص��ӿں���ָ��
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
