typedef struct
{
	SYSTEMTIME m_SystemTime; // ���ջ������ݵ�ʱ��
	
	DJ_U8      m_u8DataType; // 1����������, 2-������Ϣ, 3-״̬��Ϣ
	DJ_U8      m_u8Txrx;     // 1����������, 2-��������
	DJ_U8      m_u8LinkId;   // ��·���
	DJ_U8      m_u8LinkState;// ��·״̬
	DJ_U8      m_u8Rfu[8];   // ����
	
	DJ_S32     m_s32DataLen; // ���ݳ���
	DJ_U8*     m_pData;      // ʵ������
} SS7_Monitor;


// ��������: ��SIO��ʼ��������������
//  sio & 0x0f = 0 (����������)
//  sio & 0x0f = 1 (�������)
//  sio & 0x0f = 3 (SCCP)
//  sio & 0x0f = 4 (TUP)
//  sio & 0x0f = 5 (ISUP)


// ������Ϣ: 
typedef	struct	
{
	DJ_U16	m_u16data_length;	//��Ч���ݳ���
	DJ_U16	m_u16data_type;	    //��������
	DJ_U8  	m_u8debug_data[64];	//��������
} SS7_link_DebugData;


// ״̬��Ϣ: �ṩ�����ݣ���һ����Ҫȫ����ʾ���� (;//������Ҫ��ʾ)
typedef struct
{
	DJ_U32 durLnkUnav;//            /* duration of link unavailability */
	DJ_U32 durLnkCong;//           /* duration of link congestion */
	DJ_S32 lnkErrPduRx;//            /* number of errored messages received */
	DJ_S32 lnkConOrdTx;            /* link connection order transmitted */
	DJ_S32 lnkConOrdRx;            /* link connection order received */
	DJ_S32 lnkConAckTx;            /* link connection ack transmitted */
	DJ_S32 lnkConAckRx;            /* link connection ack received */
	DJ_S32 lnkTstRx;//               /* link test received */
	DJ_S32 lnkTstTx;//               /* link test transmitted */
	DJ_S32 lnkTstAckRx;//            /* link test ack received */
	DJ_S32 lnkTstAckTx;//            /* link test ack transmitted */
	DJ_S32 txDrop;//                 /* transmit messages dropped */
	DJ_S32 txCongDrop;//             /* MSUs dropped due to link congestion */
	DJ_S32 sifOctTx;               /* number of SIF octets transmitted */
	DJ_S32 sifOctRx;               /* number of SIF octets received */
	DJ_S32 sioOctTx;               /* number of SIO octets transmitted */
	DJ_S32 sioOctRx;               /* number of SIO octets received */
	DJ_S32 msuTx;//                  /* number of MSU transmitted */
	DJ_S32 msuRx;//                  /* number of MSU received */
	
	DJ_U32 L3L2Cong;			   /*L3->L2 queue congestion*/
	DJ_U32 L3L4Cong;			   /*L3->L4 queue congestion*/
}   SS7_link_StsDlSapInf;
