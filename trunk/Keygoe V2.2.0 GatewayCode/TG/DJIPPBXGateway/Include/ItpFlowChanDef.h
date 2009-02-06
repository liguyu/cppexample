//*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ItpFlowChanDef.h
//Version       :  1.1                                                              
//Author        :  hewei                                                             
//DateTime      :  2005-07-20 15:00                                           
//Description  :   ITP����ִ��ģ��ͷ�ļ�
//*****************************************************************************

#ifndef _ITPFLOWCHANDEF_H
#define _ITPFLOWCHANDEF_H

#include "ITPMsgPublic.h"
#include "ITPMainModCallBack.h"
#include "DJAcsDataDef.h"

#define ITP_MAX_MOD_TYPE_NUM           128    // ���ģ����
#define ITP_MAX_RELATEMOD_NUM          64     //������ģ����
#define ITP_MAX_SYSFUNC_NUM            16     //����ģ��ϵͳ�����ӿڸ���
#define ITP_MAX_DLLFUNC_NUM            4      //���̶�̬��ӿں�������
#define ITP_FLOW_MAX_FLOWNAME_LEN      40     //��������󳤶�          
#define ITP_FLOW_MAX_FLOWNAME_LEN      40     //��������󳤶�          
#define ITP_FLOW_MAX_INTERNAME_LEN     120    //�û��Զ���ṹ����󳤶�


#ifndef MAX_PATH
#define MAX_PATH                       260
#endif

#define ITP_MAX_BINDRES_NUM            256     //����ͨ���󶨵������Դ����
#define ITP_MAX_BINDTIMER_NUM          64     //����ͨ���󶨵����ʱ������
#define ITP_FLOW_SHOW_INFO_SIZE        40     //�û���ʾ״̬���泤��
#define ITP_FLOW_SHOW_INFO_NUM         3      //�û���ʾ״̬�������

typedef struct _tagITP_FLOW_INFO ITP_FLOW_INFO,*PITP_FLOW_INFO;
typedef struct _tagITP_FLOW_CHAN ITP_FLOW_CHAN,*PITP_FLOW_CHAN;
typedef struct _tagITP_RES_CHAN  ITP_RES_CHAN, *PITP_RES_CHAN;
typedef struct _tagITP_FLOW_TIMER ITP_FLOW_TIMER, *PITP_FLOW_TIMER;

typedef DJ_S32 (*PTIMERFUNC)(DJ_S32 *);

enum ITP_RESOPEN_TYPE
{
    ITP_RESOPEN_FREE = 0,
    ITP_RESOPEN_LISTEN,
	ITP_RESOPEN_EXCLUDE,
};

//������Ϣ�ṹ
typedef struct _tagITP_FLOW_INFO
{
    DJ_U32     m_u32FlowIndex;                          //���̱��
    DJ_S8      m_s8FlowName[ITP_FLOW_MAX_FLOWNAME_LEN]; //������
    DJ_S8      m_s8DllName[MAX_PATH];                   //��̬����	
    DJ_U32     m_u32StartState;                         //����ʹ��״̬
    DJ_U32     m_u32LoadState;                          //���̼���״̬
    DJ_U32*    m_hdlDllHandle;                          //���̶�̬����
    DJ_U32*    m_hdlDllFunc[ITP_MAX_DLLFUNC_NUM];       //���̶�̬��ӿں���ָ��
    DJ_U16     m_u16FlowChanNum;                        //�ö�̬���Ӧ�ĸ�������
	DJ_U16     m_u16Resrv;
    PITP_FLOW_CHAN *m_PITPFlowChan;                     //�ö�̬���Ӧ�ĸ�����
}ITP_FLOW_INFO,*PITP_FLOW_INFO;

//����ͨ���ṹ
typedef struct _tagITP_FLOW_CHAN
{
    DJ_U16           m_u16FlowCurState;           //������ͨ����ǰ״̬
    DJ_U16           m_u16FlowInfoIndex;          //������ͨ����Ӧ�������ͱ��
    DJ_U16           m_u16ChanIndex;              //������ͨ����Ӧͨ�����
    DJ_U16           m_u16StartState;             //������־,0:��ֹ;1:ʹ��
	DJ_U16           m_u16BusyState;              //Busy��־,0:Free;1:Busy
    PITP_FLOW_INFO   m_PITPFlowInfo;              //������Ϣ
    DJ_U16           m_u16CurResNum;              //�������ͨ���󶨵ĵ�ǰ��Դ����
    DJ_S16           m_s16ChanRetCode;            //����ͨ������״ֵ̬
	PITP_RES_CHAN    m_PITPResList[ITP_MAX_BINDRES_NUM];//�������ͨ���󶨵���Դ�����
    DJ_U16           m_u16CurTimerNum;            //��ʱ������
	DJ_S16           m_s16Resrv2;
    PITP_FLOW_TIMER  m_PITPTimerChan[ITP_MAX_BINDTIMER_NUM];//��ʱ�����
    DJ_U32*          m_PITPSysFunc[ITP_MAX_SYSFUNC_NUM];    //ϵͳ����ָ��
    DJ_S8            m_s8InfoBuf[ITP_FLOW_SHOW_INFO_NUM][ITP_FLOW_SHOW_INFO_SIZE];//�û���ʾ״̬
	DJ_U8            m_u8RelateModInfo[ITP_MAX_MOD_TYPE_NUM]; //��ص�ģ��
    PPKG_HEAD_STRUCT m_PITPEvtPack;                 //�¼���ָ��
	PPKG_HEAD_STRUCT m_PITPInterEvt;                //�����ڲ��¼���ָ��
    DJ_U8            *m_PITPFlowExtData;            //������չ����
    DJ_U8            *m_PITPPublicBuf;              //����ʹ�û�����
}ITP_FLOW_CHAN,*PITP_FLOW_CHAN;

//��Դͨ���ṹ
typedef struct _tagITP_RES_CHAN
{
	DJ_U16	 m_u16ChanIndex;        //��ͨ����ͨ��������
    DJ_U8	 m_u8UnitIndex;         //��ͨ������ģ�鵥Ԫ������
	DJ_U8	 m_u8ModType;           //��ͨ������ģ���Module_Type
    DJ_U8	 m_u8FuncType;          //��ͨ������ģ���Func_Type        
    DJ_U8	 m_u8ChanType;          //��ͨ�������ͣ������ֻ��Interface_ch����
    DJ_S16	 m_s16ChanGroup;        //��ͨ�������
    DJ_U8	 m_u8BusyFlag;          //ͨ��æ��־,0:����;1:��ռ��;2:������
    PITP_FLOW_CHAN  m_PITPFlowChan; //����ͨ����Ϣ
	PITP_FLOW_CHAN  m_PITPListenFlowChan; //��������ͨ��
    DJ_S8    m_s8InfoBuf[ITP_FLOW_SHOW_INFO_NUM][ITP_FLOW_SHOW_INFO_SIZE];//�û���ʾ״̬
    DJ_U8    m_u8RelateModInfo[ITP_MAX_RELATEMOD_NUM];     //�����Դ��ص�ģ��
	DJ_S8    m_s8MachineId;                                //�����ID 
	DJ_S8    m_s8Resrv[ITP_MAX_RELATEMOD_NUM-1];
}ITP_RES_CHAN,*PITP_RES_CHAN;

//��ʱ���ṹ
typedef struct _tagITP_FLOW_TIMER
{	
    DJ_F64     m_u64Time;              //����ʱ��
    DJ_U32     m_u32Ticks;             //�������
    DJ_U8      m_u8StartFlag;          //������־
	DJ_U8      m_u8Caller;             //������:�û�DLL����ģ�鱾��
	DJ_U8      m_Resrv[2];
    PITP_FLOW_CHAN  m_PITPFlowChan;   //����ͨ��ָ��
    PTIMERFUNC m_PITPTimerFunc;       //��ʱ������
	DJ_U16	   m_u16ChanIndex;        //��������������Ϊ��������ת�����ֵ��¼�����Ϊ��Դ�����һ��
    DJ_U8	   m_u8UnitIndex;         //
	DJ_U8	   m_u8ModType;           //
}ITP_FLOW_TIMER,*PITP_FLOW_TIMER;

//�������Բ�ͨѶ�¼��ṹ
typedef struct
{
	PKG_HEAD_STRUCT  m_PkgHead;                                 //�¼���ͷ
	CallID_t         m_SrcCallID;                               //Դ����
	CallID_t         m_destCallID;                              //Ŀ������
	DJ_U32           m_u32CmdType;                              //�Զ�����������
	DJ_U32           m_u32InterNameLen;                         //�û��ṹ������
	DJ_S8            m_s8InterName[ITP_FLOW_MAX_INTERNAME_LEN]; //�û��ṹ��
	DJ_U32           m_u32InterDataSize;                        //�û��Զ���ṹ��С
}FLOW_INTERDATA_HEAD,*PFLOW_INTERDATA_HEAD;

#endif
