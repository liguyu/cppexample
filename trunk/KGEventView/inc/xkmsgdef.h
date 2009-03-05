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
/* 网元消息 */
#define		XK_CMD_NETUNIT_LOGIN			0x20000001	// 网元登录消息
#define		XK_REP_NETUNIT_LOGIN			0x21000001	// 网元登录应答信息

#define		XK_KEY_MSGHEADER_ID				0x00000000		// 消息头->消息ID
#define		XK_KEY_MSGHEADER_SENDER			0x00000001		// 消息头->发送者
#define		XK_KEY_MSGHEADER_RECIEVER		0x00000002		// 消息头->接收者
#define		XK_KEY_MSGHEADER_STATUS			0x00000003		// 消息头->消息状态


#define		XK_KEY_NETUNIT_LOGIN_ID			(0x00010001)		// 网元ID
#define		XK_KEY_NETUNIT_LOGIN_TYPE		(0x00010002)		// 网元类型
#define		XK_KEY_NETUNIT_LOGIN_DESC		(0x00010003)		// 描述信息

/* 用户自定义消息 */
#define		XK_MSG_USER						(0x60000000)		// 用户自定义消息

/*
	+-----+------+------+--------+--------+------+
	+任选 + 必备 + 必备 + 消息类 + 电路识 + 路由 +
	+部分 + 可变 + 固定 + 型编码 +  别码  + 标记 + 
	+ --- + ---- + ---- + ------ + ------ +	---- +
	+  8n +  8n  +  8n  +   1    +   2    +  56  +
	+-----+-------------+------------------------+
	1、ISUP和TUP为MSU的一种，ISUP消息包含在MSU消息的SIF部分
	2、路由标记占56比特,OPC、DPC各占24比特,SLS占4比特,剩余4比特备用
	3、电路识别码(CIC)占16比特，12比特使用，4比特备用
		－2048kb/s的通道，CIC低5比特是通信通道所分配的时隙号码，其余7比特为PCM(E1)
		－8448kb/s的通道，CID低7比特是通信通道所分配的时隙号码，其余5比特为PCM(E1)
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
		TTC_TUP_AVIABLE		= 0x61,		//通知TUP链路启动
		TTC_TUP_UNAVIABLE	= 0x71,		//通知TUP链路停止

		TTC_GROUP_FAM		= 0x01,		//有关建立呼叫的前向地址消息
		TTC_IAM				= 0x11,		//起始地址消息
		TTC_IAI				= 0x21,		//带附加信息的起始地址消息
		TTC_SAM				= 0x31,		//后续地址消息
		TTC_SAO				= 0x41,		//带一个信号的后续地址消息
		TTC_TEST			= 0x51,

		TTC_GROUP_FSM		= 0x02,		//前向建立消息
		TTC_GSM				= 0x12,		//呼叫监视消息	
		TTC_COT				= 0x32,		//导通信号
		TTC_CCF				= 0x42,		//导通失败信号

		TTC_GROUP_BSM		= 0x03,		//后向建立消息
		TTC_GRQ				= 0x13,		//综合请求消息
		
		TTC_GROUP_SBM		= 0x04,		//后向建立成功消息
		TTC_ACM				= 0x14,		//地址收全消息
		TTC_CHG				= 0x24,		//计费消息

		TTC_GROUP_UBM		= 0x05,		//后向建立不成功消息
		TTC_SEC				= 0x15,		//交换设备拥塞信号
		TTC_CGC				= 0x25,		//电路群拥塞信号
		TTC_NNC				= 0x35,		//国家网拥塞信号
		TTC_ADI				= 0x45,		//地址不全信号
		TTC_CFL				= 0x55,		//呼叫失败信号
		TTC_SSB				= 0x65,		//用户忙信号
		TTC_UNN				= 0x75,		//未分配号码信号
		TTC_LOS				= 0x85,		//线路退出服务信号
		TTC_SST				= 0x95,		//发送特别信息音信号
		TTC_ACB				= 0xA5,		//禁止接入信号
		TTC_DPN				= 0xB5,		//数字通道部提供信号
		TTC_MPR				= 0xC5,		//误播中继前缀
		TTC_EUM				= 0xF5,		//扩充的后向建立不成功消息
		
		TTC_GROUP_CSM		= 0x06,		//呼叫监视消息
		TTC_ANU				= 0x06,		//应答信号,无任何说明
		TTC_ANC				= 0x16,		//应答信号,收费
		TTC_ANN				= 0x26,		//应答信号,不收费
		TTC_CBK				= 0x36,		//后向拆线信号
		TTC_CLF				= 0x46,		//前向拆线信号
		TTC_RAN				= 0x56,		//再应答信号
		TTC_FOT				= 0x66,		//前向传送信号
		TTC_CCL				= 0x76,		//主叫用户挂机信号

		TTC_TST				= 0x86,		//电路同步信号  yy add
		TTC_TSA				= 0x96,		//电路同步证实信号(已同步) yy add
		TTC_TAA				= 0xA6,		//电路同步证实信号(未同步) yy add

		TTC_GROUP_CCM		= 0x07,		//电路监视消息
		TTC_RLG				= 0x17,		//释放保护信号
		TTC_BLO				= 0x27,		//阻断信号
		TTC_BLA				= 0x37,		//阻断证实信号
		TTC_UBL				= 0x47,		//阻断解除信号
		TTC_UBA				= 0x57,		//阻断解除证实信号
		TTC_CCR				= 0x67,		//导通检验请求信号
		TTC_RSC				= 0x77,		//电路复原信号

		TTC_GROUP_GRM		= 0x08,		//电路群监视消息
		TTC_MGB				= 0x18,		//
		TTC_MBA				= 0x28,		//
		TTC_MGU				= 0x38,		//
		TTC_MUA				= 0x48,		//
		TTC_HGB				= 0x58,		//
		TTC_HBA				= 0x68,		//
		TTC_HGU				= 0x78,		//
		TTC_HUA				= 0x88,		//
		TTC_GRS				= 0x98,		//电路群复原消息
		TTC_GRA				= 0xA8,		//群复原证实消息
		TTC_SGB				= 0xB8,		//
		TTC_SBA				= 0xC8,		//
		TTC_SGU				= 0xD8,		//
		TTC_SUA				= 0xE8,		//

		TTC_GROUP_CNM		= 0x0A,		//电路网管理消息
		TTC_ACC				= 0x19,		//

		TTC_GROUP_NUB		= 0X0E,		//
		TTC_SLB				= 0x1e,		//被叫市忙
		TTC_STB				= 0X2e,		//被叫长忙
};

///////////////////////////////////////ISUP///////////////////////////////////////////////
#define PARAM_FIX	1
#define PARAM_VAR	2
#define PARAM_OPT	3
#define MAX_DATALEN	255

//消息格式
#define MSG_FORMAT_INC_SEC1		0x01  //包括有固定必备部分
#define MSG_FORMAT_INC_SEC2		0x02  //包括有可变必备部分
#define MSG_FORMAT_INC_SEC3		0x04  //包括有任选部分

enum MSG_TYPE_CODE
{
	//前向建立消息
	MTC_IAM					= 0X01,		//初始地址消息
	MTC_SAM					= 0X02,		//后续地址消息

	//一般建立消息
	MTC_INR					= 0X03,		//信息请求消息
	MTC_INF					= 0X04,		//信息消息
	MTC_COT					= 0X05,		//导通消息

	//后向建立消息
	MTC_ACM					= 0X06,		//地址全消息
	MTC_CON					= 0X07,		//连接消息
	MTC_CPG					= 0X2C,		//呼叫进展消息

	//呼叫监视消息
	MTC_ANM					= 0X09,		//应答消息
	MTC_FOT					= 0X08,		//前向传递消息
	MTC_REL					= 0X0C,		//释放消息

	//电路监视
//	MTC_DRS					= 			//延迟释放消息
	MTC_RLC					= 0X10,		//释放完全消息
	MTC_CCR					= 0X11,		//导通检验请求消息
	MTC_RSC					= 0X12,		//电路复原消息
	MTC_LPA					= 0X24,		//环路确认消息
	MTC_BLO					= 0X13,		//闭塞消息
	MTC_UBL					= 0X14,		//解除闭塞消息
	MTC_UCIC				= 0X2E,		//未装备的CIC
	MTC_BLA					= 0X15,		//闭塞证实消息
	MTC_UBA					= 0X16,		//解除闭塞证实消息
	MTC_OLM					= 0X30,		//过负荷消息
	MTC_SUS					= 0X0D,		//暂停消息
	MTC_RES					= 0X0E,		//恢复消息
	MTC_CFN					= 0X2F,		//混淆消息
	MTC_SGM					= 0x38,		//分段

	//电路群监视消息
	MTC_CGB					= 0X18,		//电路群闭塞消息
	MTC_CGU					= 0X19,		//电路群闭塞解除消息
	MTC_CGBA				= 0X1A,		//电路群闭塞证实消息
	MTC_CGUA				= 0X1B,		//电路群闭塞解除证实消息
	MTC_GRS					= 0X17,		//电路群复原消息
	MTC_GRA					= 0X29,		//电路群复原证实消息
	MTC_CQM					= 0X2A,		//电路群询问消息
	MTC_CQR					= 0X2B,		//电路群询问响应消息

	//呼叫中改变消息
//	MTC_CMR					=			//呼叫改变请求消息
//	MTC_CMC					=			//呼叫改变完全消息
//	MTC_CMRJ				= 			//呼叫改变拒绝消息
	MTC_FAR					= 0X1F,		//性能请求消息
	MTC_FAA					= 0X20,		//性能请求接受消息
	MTC_FRJ					= 0X21,		//性能请求拒绝消息

	//端到端消息
	MTC_PAM					= 0X28,		//传递消息
	MTC_USR					= 0X2D,		//用户至用户消息

	//用户到用户测试
	MTC_UPT					= 0x34,		//用户部分测试
	MTC_UPA					= 0x35,		//用户部分可利用

	MTC_TST					= 0x86,		//电路同步信号  yy add
	MTC_TSA					= 0x96,		//电路同步证实信号(已同步) yy add
	MTC_TAA					= 0xA6,		//电路同步证实信号(未同步) yy add

	MTC_FAC					= 0x33,		//电路同步信号  lyl add
	MTC_NRM					= 0x32,		//网络资源管理  lyl add
	MTC_IDR					= 0x36,		//识别请求 lyl add
	MTC_IRS					= 0x37,		//识别响应 lyl add
};


#endif // __XK_MSGDEF_H