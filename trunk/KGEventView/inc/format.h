typedef struct
{
	SYSTEMTIME m_SystemTime; // 接收或发送数据的时间
	
	DJ_U8      m_u8DataType; // 1－信令数据, 2-调试信息, 3-状态信息
	DJ_U8      m_u8Txrx;     // 1－接收数据, 2-发送数据
	DJ_U8      m_u8LinkId;   // 链路编号
	DJ_U8      m_u8LinkState;// 链路状态
	DJ_U8      m_u8Rfu[8];   // 保留
	
	DJ_S32     m_s32DataLen; // 数据长度
	DJ_U8*     m_pData;      // 实际数据
} SS7_Monitor;


// 信令数据: 从SIO开始的所有信令数据
//  sio & 0x0f = 0 (信令网管理)
//  sio & 0x0f = 1 (信令测试)
//  sio & 0x0f = 3 (SCCP)
//  sio & 0x0f = 4 (TUP)
//  sio & 0x0f = 5 (ISUP)


// 调试信息: 
typedef	struct	
{
	DJ_U16	m_u16data_length;	//有效数据长度
	DJ_U16	m_u16data_type;	    //数据类型
	DJ_U8  	m_u8debug_data[64];	//调试数据
} SS7_link_DebugData;


// 状态信息: 提供的数据，不一定需要全部显示出来 (;//部分需要显示)
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
