#if !defined(__XK_MSGDEF_H)
#define __XK_MSGDEF_H

#define XK_KEY_TYPE_UNKNOWN				0x00000000
#define XK_KEY_TYPE_BYTE				0x00000001
#define XK_KEY_TYPE_SHORT				0x00000002
#define XK_KEY_TYPE_INT					0x00000003
#define XK_KEY_TYPE_UINT				0x00000004
#define XK_KEY_TYPE_FLOAT				0x00000005
#define XK_KEY_TYPE_DOUBLE				0x00000006
#define XK_KEY_TYPE_STRING				0x00000007
#define XK_KEY_TYPE_BITARRAY			0x00000008
#define XK_KEY_TYPE_BOOL				0x00000009
/* ��Ԫ��Ϣ */
#define		XK_CMD_NETUNIT_LOGIN			0x20000001	// ��Ԫ��¼��Ϣ
#define		XK_REP_NETUNIT_LOGIN			0x21000001	// ��Ԫ��¼Ӧ����Ϣ

#define		XK_KEY_MSGHEADER_ID				0x00000000		// ��Ϣͷ->��ϢID
#define		XK_KEY_MSGHEADER_SENDER			0x00000001		// ��Ϣͷ->������
#define		XK_KEY_MSGHEADER_RECIEVER		0x00000002		// ��Ϣͷ->������
#define		XK_KEY_MSGHEADER_STATUS			0x00000003		// ��Ϣͷ->��Ϣ״̬


#define		XK_KEY_NETUNIT_LOGIN_ID			(0x00010001)		// ��ԪID
#define		XK_KEY_NETUNIT_LOGIN_TYPE		(0x00010002)		// ��Ԫ����
#define		XK_KEY_NETUNIT_LOGIN_DESC		(0x00010003)		// ������Ϣ

/* �û��Զ�����Ϣ */
#define		XK_MSG_USER						(0x60000000)		// �û��Զ�����Ϣ

/*
	+-----+------+------+--------+--------+------+
	+��ѡ + �ر� + �ر� + ��Ϣ�� + ��·ʶ + ·�� +
	+���� + �ɱ� + �̶� + �ͱ��� +  ����  + ��� + 
	+ --- + ---- + ---- + ------ + ------ +	---- +
	+  8n +  8n  +  8n  +   1    +   2    +  56  +
	+-----+-------------+------------------------+
	1��ISUP��TUPΪMSU��һ�֣�ISUP��Ϣ������MSU��Ϣ��SIF����
	2��·�ɱ��ռ56����,OPC��DPC��ռ24����,SLSռ4����,ʣ��4���ر���
	3����·ʶ����(CIC)ռ16���أ�12����ʹ�ã�4���ر���
		��2048kb/s��ͨ����CIC��5������ͨ��ͨ���������ʱ϶���룬����7����ΪPCM(E1)
		��8448kb/s��ͨ����CID��7������ͨ��ͨ���������ʱ϶���룬����5����ΪPCM(E1)
*/
///////////////////////////////////////TUP///////////////////////////////////////////////
#define TUP_LI_POSITION				1
#define TUP_SIO_POSITION			TUP_LI_POSITION+1
#define TUP_CIC_POSITION			TUP_SIO_POSITION+7  
#define TUP_INFO_POSITION			TUP_CIC_POSITION+2
#define TUP_INFOLEN					-(TUP_INFO_POSITION)

//define data msg byte position recv from L2
#define TUP_MSG_POSITION				9


#define TUP_SETUP				0
#define TUP_SETUP_ACK           1
#define TUP_INFO_REQUEST        2
#define TUP_MONITOR				3


enum TUP_TITLE_CODE 
{    //PAGE 22
		TTC_NULL = 0x00,
		TTC_TUP_AVIABLE		= 0x61,		//֪ͨTUP��·����
		TTC_TUP_UNAVIABLE	= 0x71,		//֪ͨTUP��·ֹͣ

		TTC_GROUP_FAM		= 0x01,		//�йؽ������е�ǰ���ַ��Ϣ
		TTC_IAM				= 0x11,		//��ʼ��ַ��Ϣ
		TTC_IAI				= 0x21,		//��������Ϣ����ʼ��ַ��Ϣ
		TTC_SAM				= 0x31,		//������ַ��Ϣ
		TTC_SAO				= 0x41,		//��һ���źŵĺ�����ַ��Ϣ
		TTC_TEST			= 0x51,

		TTC_GROUP_FSM		= 0x02,		//ǰ������Ϣ
		TTC_GSM				= 0x12,		//���м�����Ϣ	
		TTC_COT				= 0x32,		//��ͨ�ź�
		TTC_CCF				= 0x42,		//��ͨʧ���ź�

		TTC_GROUP_BSM		= 0x03,		//��������Ϣ
		TTC_GRQ				= 0x13,		//�ۺ�������Ϣ
		
		TTC_GROUP_SBM		= 0x04,		//�������ɹ���Ϣ
		TTC_ACM				= 0x14,		//��ַ��ȫ��Ϣ
		TTC_CHG				= 0x24,		//�Ʒ���Ϣ

		TTC_GROUP_UBM		= 0x05,		//���������ɹ���Ϣ
		TTC_SEC				= 0x15,		//�����豸ӵ���ź�
		TTC_CGC				= 0x25,		//��·Ⱥӵ���ź�
		TTC_NNC				= 0x35,		//������ӵ���ź�
		TTC_ADI				= 0x45,		//��ַ��ȫ�ź�
		TTC_CFL				= 0x55,		//����ʧ���ź�
		TTC_SSB				= 0x65,		//�û�æ�ź�
		TTC_UNN				= 0x75,		//δ��������ź�
		TTC_LOS				= 0x85,		//��·�˳������ź�
		TTC_SST				= 0x95,		//�����ر���Ϣ���ź�
		TTC_ACB				= 0xA5,		//��ֹ�����ź�
		TTC_DPN				= 0xB5,		//����ͨ�����ṩ�ź�
		TTC_MPR				= 0xC5,		//���м�ǰ׺
		TTC_EUM				= 0xF5,		//����ĺ��������ɹ���Ϣ
		
		TTC_GROUP_CSM		= 0x06,		//���м�����Ϣ
		TTC_ANU				= 0x06,		//Ӧ���ź�,���κ�˵��
		TTC_ANC				= 0x16,		//Ӧ���ź�,�շ�
		TTC_ANN				= 0x26,		//Ӧ���ź�,���շ�
		TTC_CBK				= 0x36,		//��������ź�
		TTC_CLF				= 0x46,		//ǰ������ź�
		TTC_RAN				= 0x56,		//��Ӧ���ź�
		TTC_FOT				= 0x66,		//ǰ�����ź�
		TTC_CCL				= 0x76,		//�����û��һ��ź�

		TTC_TST				= 0x86,		//��·ͬ���ź�  yy add
		TTC_TSA				= 0x96,		//��·ͬ��֤ʵ�ź�(��ͬ��) yy add
		TTC_TAA				= 0xA6,		//��·ͬ��֤ʵ�ź�(δͬ��) yy add

		TTC_GROUP_CCM		= 0x07,		//��·������Ϣ
		TTC_RLG				= 0x17,		//�ͷű����ź�
		TTC_BLO				= 0x27,		//����ź�
		TTC_BLA				= 0x37,		//���֤ʵ�ź�
		TTC_UBL				= 0x47,		//��Ͻ���ź�
		TTC_UBA				= 0x57,		//��Ͻ��֤ʵ�ź�
		TTC_CCR				= 0x67,		//��ͨ���������ź�
		TTC_RSC				= 0x77,		//��·��ԭ�ź�

		TTC_GROUP_GRM		= 0x08,		//��·Ⱥ������Ϣ
		TTC_MGB				= 0x18,		//
		TTC_MBA				= 0x28,		//
		TTC_MGU				= 0x38,		//
		TTC_MUA				= 0x48,		//
		TTC_HGB				= 0x58,		//
		TTC_HBA				= 0x68,		//
		TTC_HGU				= 0x78,		//
		TTC_HUA				= 0x88,		//
		TTC_GRS				= 0x98,		//��·Ⱥ��ԭ��Ϣ
		TTC_GRA				= 0xA8,		//Ⱥ��ԭ֤ʵ��Ϣ
		TTC_SGB				= 0xB8,		//
		TTC_SBA				= 0xC8,		//
		TTC_SGU				= 0xD8,		//
		TTC_SUA				= 0xE8,		//

		TTC_GROUP_CNM		= 0x0A,		//��·��������Ϣ
		TTC_ACC				= 0x19,		//

		TTC_GROUP_NUB		= 0X0E,		//
		TTC_SLB				= 0x1e,		//������æ
		TTC_STB				= 0X2e,		//���г�æ
};

///////////////////////////////////////ISUP///////////////////////////////////////////////
#define PARAM_FIX	1
#define PARAM_VAR	2
#define PARAM_OPT	3
#define MAX_DATALEN	255

//��Ϣ��ʽ
#define MSG_FORMAT_INC_SEC1		0x01  //�����й̶��ر�����
#define MSG_FORMAT_INC_SEC2		0x02  //�����пɱ�ر�����
#define MSG_FORMAT_INC_SEC3		0x04  //��������ѡ����

enum MSG_TYPE_CODE
{
	//ǰ������Ϣ
	MTC_IAM					= 0X01,		//��ʼ��ַ��Ϣ
	MTC_SAM					= 0X02,		//������ַ��Ϣ

	//һ�㽨����Ϣ
	MTC_INR					= 0X03,		//��Ϣ������Ϣ
	MTC_INF					= 0X04,		//��Ϣ��Ϣ
	MTC_COT					= 0X05,		//��ͨ��Ϣ

	//��������Ϣ
	MTC_ACM					= 0X06,		//��ַȫ��Ϣ
	MTC_CON					= 0X07,		//������Ϣ
	MTC_CPG					= 0X2C,		//���н�չ��Ϣ

	//���м�����Ϣ
	MTC_ANM					= 0X09,		//Ӧ����Ϣ
	MTC_FOT					= 0X08,		//ǰ�򴫵���Ϣ
	MTC_REL					= 0X0C,		//�ͷ���Ϣ

	//��·����
//	MTC_DRS					= 			//�ӳ��ͷ���Ϣ
	MTC_RLC					= 0X10,		//�ͷ���ȫ��Ϣ
	MTC_CCR					= 0X11,		//��ͨ����������Ϣ
	MTC_RSC					= 0X12,		//��·��ԭ��Ϣ
	MTC_LPA					= 0X24,		//��·ȷ����Ϣ
	MTC_BLO					= 0X13,		//������Ϣ
	MTC_UBL					= 0X14,		//���������Ϣ
	MTC_UCIC				= 0X2E,		//δװ����CIC
	MTC_BLA					= 0X15,		//����֤ʵ��Ϣ
	MTC_UBA					= 0X16,		//�������֤ʵ��Ϣ
	MTC_OLM					= 0X30,		//��������Ϣ
	MTC_SUS					= 0X0D,		//��ͣ��Ϣ
	MTC_RES					= 0X0E,		//�ָ���Ϣ
	MTC_CFN					= 0X2F,		//������Ϣ
	MTC_SGM					= 0x38,		//�ֶ�

	//��·Ⱥ������Ϣ
	MTC_CGB					= 0X18,		//��·Ⱥ������Ϣ
	MTC_CGU					= 0X19,		//��·Ⱥ���������Ϣ
	MTC_CGBA				= 0X1A,		//��·Ⱥ����֤ʵ��Ϣ
	MTC_CGUA				= 0X1B,		//��·Ⱥ�������֤ʵ��Ϣ
	MTC_GRS					= 0X17,		//��·Ⱥ��ԭ��Ϣ
	MTC_GRA					= 0X29,		//��·Ⱥ��ԭ֤ʵ��Ϣ
	MTC_CQM					= 0X2A,		//��·Ⱥѯ����Ϣ
	MTC_CQR					= 0X2B,		//��·Ⱥѯ����Ӧ��Ϣ

	//�����иı���Ϣ
//	MTC_CMR					=			//���иı�������Ϣ
//	MTC_CMC					=			//���иı���ȫ��Ϣ
//	MTC_CMRJ				= 			//���иı�ܾ���Ϣ
	MTC_FAR					= 0X1F,		//����������Ϣ
	MTC_FAA					= 0X20,		//�������������Ϣ
	MTC_FRJ					= 0X21,		//��������ܾ���Ϣ

	//�˵�����Ϣ
	MTC_PAM					= 0X28,		//������Ϣ
	MTC_USR					= 0X2D,		//�û����û���Ϣ

	//�û����û�����
	MTC_UPT					= 0x34,		//�û����ֲ���
	MTC_UPA					= 0x35,		//�û����ֿ�����

	MTC_TST					= 0x86,		//��·ͬ���ź�  yy add
	MTC_TSA					= 0x96,		//��·ͬ��֤ʵ�ź�(��ͬ��) yy add
	MTC_TAA					= 0xA6,		//��·ͬ��֤ʵ�ź�(δͬ��) yy add

	MTC_FAC					= 0x33,		//��·ͬ���ź�  lyl add
	MTC_NRM					= 0x32,		//������Դ����  lyl add
	MTC_IDR					= 0x36,		//ʶ������ lyl add
	MTC_IRS					= 0x37,		//ʶ����Ӧ lyl add
};


#endif // __XK_MSGDEF_H