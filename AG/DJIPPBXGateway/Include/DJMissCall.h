#ifndef _MISSCALL_H_
#define _MISSCALL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DJSSM_CALL_GENERATE			0x01	// ���з���
#define DJSSM_CALL_CONNECT			0x02	// ͨ������
#define DJSSM_CALL_DISCONNECT		0x03	// ͨ���Ͽ�
#define DJSSM_CALL_INFO				0x04	// ������Ϣ
#define DJSSM_CALL_INFO_ACM			0x05	// �յ�ACM��Ϣ
	
#define FORCE_MODE					0x01	// ǿ������

// TUP��æ�ź�
#define CALLEE_SLB					0x1E	// �����л�æ	
#define CALLEE_SSB					0x65	// ����æ
#define CALLEE_STB					0x2E	// ���г���æ

#define CALLEE_LOS					0x85	// ��·�˳�����
#define CALLEE_CGC					0x25	// ��·Ⱥӵ��
#define CALLEE_ADI					0x45	// ��ַ��ȫ
#define CALLEE_SEC					0x15	// �����豸ӵ��
#define CALLEE_UNN					0x75	// δ��������ź�
#define CALLEE_SST					0x95	// �ر��ź���
#define CALLEE_EUM					0xF5	// ���������ɹ������䣩

// TUP�����ź�
#define CALLINF_CLF					0x46	// ǰ�����
#define CALLINF_CBK					0x36	// �������

// ISUP�����ͷ�ԭ��ֵ
enum CAUSE_VALUE
{
	RSN_UNALLOCATED_NUMBER = 0x01,		//�պ�
	RSN_NORMAL_CALL_CLEARING = 0x10,	//�����ĺ������
	RSN_USER_BUSY = 0x11,				//�û�æ
	RSN_NO_USER_RESPONDING = 0x12,		//���û���Ӧ����ʱû���յ�ACM
	RSN_USER_ALERTING_NO_ANSWER = 0x13, //������Ӧ�𣬳�ʱδժ��
	RSN_CALL_REJECTED = 0x15,			//���оܾ�
	RSN_INCOMPLETE_NUMBER = 0x1c,		//�������ĺ���
	RSN_NORMAL_NO_SET = 0x1f,			//������δ�涨
	RSN_NO_CIRCUIT_CHANNELAVAILABLE = 0x22, //�޿��õĵ�·
	RSN_REQUESTEDCIRCUIT_CHANNEL_NOTAVAILABLE = 0x2c, //����ĵ�·������
	RSN_RECOVERY_ON_TIME_EXPIRY = 0x66,	//��ʱ����ʱ�ָ�

	RSN_LOCAL_RLC = 0x20,				//�����ͷŵ�·
	RSN_RECEIVE_RSC = 0x21,				//��·��ԭ
};

#define ERR_DMSU_SOCKFAIL			1000//-1		// ������DMSU����socketʧ��
#define ERR_DMSU_CONNECTFAIL		2000//-2		// ����DMSUʧ��
#define ERR_DMSU_INITRPARSERFAIL	-3		// ��ʼ����Ϣ��������ʧ��
#define ERR_DMSU_INITFRAGMENTFAIL	-4		// ��ʼ�����ݽ�������ʧ��
#define ERR_DMSU_INITRECVFAIL		-5		// ��ʼ�����ն���ʧ��
#define ERR_DMSU_STARTRECVTFAIL		-6		// ���������߳�ʧ��
#define ERR_DMSU_STARTHEARTTFAIL	-7		// ���������߳�ʧ��
#define ERR_DMSU_STARTPARSERTFAIL   -8		// ���������߳�ʧ��
#define ERR_DMSU_NOTEXISTS			-9		// ���Ӳ���
#define ERR_DMSU_CONNECTIONOK		-10		// ��ǰ��������
	
#define ERR_EVT_NODATA				-1		// û�н��յ�����
#define ERR_EVT_GETDATAFAIL			-2		// ��ȡ����ʧ��
#define ERR_EVT_DATANOTENOUGH		-3		// ���ݳ��Ȳ���
#define ERR_EVT_NOTEXISTS			-4		// ���Ӳ�����
#define ERR_EVT_DISCONNECT			-5		// ��DMSU�����ӶϿ�



typedef struct tagSockInfo
{
	char Addr[16];
	unsigned short Port;
	unsigned long BufSize;
	unsigned long RecvBufSize;
	unsigned char UnitID;
}tSockInfo, *ptSockInfo;

typedef struct tagMonEvt
{
	char caller_id[32];			// ���к���
	char callee_id[32];			// ���к���
	char original_callee_id[32];// ԭ���к���
	unsigned char Event;		// �¼�ָʾ��
	unsigned char LocalPcmID;	// ���ػ����������PCM��
	unsigned char pcm;			// �������������м̺�
	unsigned char chn;			// ������������ʱ϶��
	unsigned char Protocol;		// Э������
	unsigned char SubProtocol;	// Э�������� 
	unsigned int ToneID;		// ������
	unsigned char ReleaseReason;// ����ԭ��ֵ
	unsigned char MsgType;		// ��Ϣ����
	long DPC;
	long OPC; 	
	int DataLen;					 // ԭʼ������
	unsigned char OriginalData[300]; // ԭʼ�� CR20061116_01
}tMonEvt, *ptMonEvt;

/////////////////////////////////////////////////////////////////////////
// ϵͳ״̬��Ϣ CR20061116_02
//

/////////////////////////////////////////////////////////////////////////
// ��Ϣ����
#define SYS_INFO_BUFFER				00		// ��Ϣ������Ϣ

/////////////////////////////////////////////////////////////////////////
// ������ʾ
#define SYS_NOT_EXISTS				-1		// ָ����DMSU�豸������
#define SYS_MAX_INFO_LEN			128		// ��Ϣ������󳤶�

/////////////////////////////////////////////////////////////////////////
// �ṹ����
typedef struct tagSysBufInfo{
	unsigned char BuffID;
	int BufSize;
	int InUse;	
}tSysBufInfo, *ptSysBufInfo;

typedef struct tagSysState
{
	unsigned char sysID;						// ������Ϣ������Դ
	unsigned char infoLen;						// ��Ϣ����
	unsigned char info[SYS_MAX_INFO_LEN];		// ��Ϣ����
}tSysState, *ptSysState;

// end CR20061116_02

//////////////////////////////////////////////////////////////////////////// 
// ����ԭʼ��������ͷ�ʽ  CR20061116_01

// ��������
const unsigned char  PRO_TRANS		 = 0x00;	// ͸��
const unsigned char  PRO_PARSER		 = 0x01;	// ����
const unsigned char  PRO_BOTH		 = 0x02;	// ����������

// ��������
const unsigned char  PRO_ADD		 = 0x00;	// ���
const unsigned char  PRO_REMOVE		 = 0x01;	// �Ƴ�
const unsigned char  PRO_MODIFY		 = 0x02;	// �޸�

// Э�鹤��ģʽ
typedef struct tagProtocolMode
{
	unsigned char Protocol;
	unsigned char WorkMode;
}tProtocolMode, *ptProtocolMode;

// END CR20061116_01

// CR20070830_001
typedef struct tagDMSU_LOG
{
	int LogType;
	long Param1;
}DMSU_LOG, *PDMSU_LOG;

void DJSSM_ExitSystem();
int DJSSM_InitSystem();
int DJSSM_CreateDMSUConnection(const tSockInfo &_sockinfo);
int DJSSM_CreateDMSUConnectoin_A(ptSockInfo _pSockInfo);
int DJSSM_GetEvent(unsigned char _ClienIndex, tMonEvt * const _moniEvt);
int DJSSM_ReConnectDMSU(int _Index, int _Mode = 0);

int DJSSM_GetSystemState(unsigned char _ClientIndex, unsigned char _infoMask, tSysState * const _sysState);// CR20061116_02
int DJSSM_CfgProtocol(unsigned char _ClientIndex, tProtocolMode * const _WorkMode, unsigned char op);// CR20061116_01
int DJSSM_CfgDMSULog(unsigned char _ClientIndex, DMSU_LOG _DMSULog);// CR20061116_01
#ifdef __cplusplus
}
#endif

#endif // #ifndef _MISSCALL_H_