//*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ITPGUID.h                                                              
//Version       :  1.1                                                              
//Author        :  兰骏                                                             
//DateTime      :  2005-07-14 15:00                                           
//Description   :  ITP系统GUID定义                                                                
//*****************************************************************************

#define  MODULE_TYPE_DSP    	0x01           //DSP功能模块
#define  MODULE_TYPE_MEDIA	    0x02           //媒体处理模块
#define  MODULE_TYPE_SS7        0x03           //七号处理模块	
#define  MODULE_TYPE_CFG 	    0x04           //配置模块
#define  MODULE_TYPE_MONITOR 	0x05           //监控模块
#define  MODULE_TYPE_FLOW       0x06	       //流程模块
#define  MODULE_TYPE_PRI        0x07	       //ISDN信令模块
#define  MODULE_TYPE_USER       0x08           //用户模块
#define  MODULE_TYPE_INTERFACE  0x09           //界面模块
#define  MODULE_TYPE_VOIP       0x0A           //VoIP模块
#define  MODULE_TYPE_3G324M     0x0B           //3G-324M模块
#define  MODULE_TYPE_FAXTIFF    0x0E           //FAXTIFF模块
#define  MODULE_TYPE_ACS        0x0F           //API模块
#define  MODULE_TYPE_SIGMON     0x10           //信令监控模块
#define  MODULE_TYPE_CTXM       0x11           //交换矩阵模块
	
//DSP功能模块功能ID定义***************************************
#define  DSP_MAIN_FUNCTION_CONFIG      0x01           //全局设备管理
#define  DSP_MAIN_FUNCTION_SPEECH      0x02           //语音功能
#define  DSP_MAIN_FUNCTION_FAX         0x03           //传真功能
#define  DSP_MAIN_FUNCTION_DIGITAL     0x04           //数字中继功能
#define  DSP_MAIN_FUNCTION_INTERFACE   0x05           //坐席功能模块
#define  DSP_MAIN_FUNCTION_PRILINK     0x06           //会议功能模块
#define  DSP_MAIN_FUNCTION_SS7LINK     0x07           //数字中继功能模块
#define  DSP_MAIN_FUNCTION_CTCLK       0x08           //CT_BUS时钟
#define  DSP_MAIN_FUNCTION_CTTS        0x09           //CT_BUS资源
#define  DSP_MAIN_FUNCTION_CONNECTTS   0x0A           //时隙连接
#define  DSP_MAIN_FUNCTION_FIRMWARE    0x0B           //固件操作
#define  DSP_MAIN_FUNCTION_VOIP        0x0C           //VoIP功能
#define  DSP_MAIN_FUNCTION_3G324M      0x0D           //3G-324M功能
#define  DSP_MAIN_FUNCTION_LICENSE     0x0E           //license alarm


#define  SUB_FUNCTION_ALL                0xFF        //所有子功能

#define  DSP_SUB_FUNCTION_SPEECH_INPUT   0x01        //输入功能(EC,AGC等)
#define  DSP_SUB_FUNCTION_SPEECH_OUTPUT  0x02        //输出功能(混音,AGC等)
#define  DSP_SUB_FUNCTION_SPEECH_PLAY    0x03        //放音功能
#define  DSP_SUB_FUNCTION_SPEECH_RECORD  0x04        //录音功能
#define  DSP_SUB_FUNCTION_SPEECH_GTD     0x05        //各种音频处理功能
#define  DSP_SUB_FUNCTION_SPEECH_CONF    0x06        //会议处理功能


//DSP DSS1子功能定义
#define  DSP_SUB_FUNCTION_DSS1_CFG_CMD_TO_DSP        0x01      //DSS1配置命令，        Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_GET_CFG_TO_DSP        0x02      //获取DSS1配置信息命令，Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_CFG_INFO_TO_Q931      0x02      //DSS1配置信息，        DSP->Q931   
#define  DSP_SUB_FUNCTION_DSS1_CONTROL_CMD_TO_DSP    0x03      //DSS1控制命令          Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_STATE_EVENT_TO_Q931   0x03      //DSS1状态事件          DSP->Q931 
#define  DSP_SUB_FUNCTION_DSS1_SIGNAL_DATA_TO_DSP    0x04      //DSS1信令数据          Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_SIGNAL_DATA_TO_Q931   0x04      //DSS1信令数据          DSP->Q931
#define  DSP_SUB_FUNCTION_DSS1_DATA_REQ_TO_Q931      0x05      //DSS1数据请求包        DSP->Q931
#define  DSP_SUB_FUNCTION_DSS1_DEBUG_DATA_TO_Q931    0x06      //DSS1调试信息包        DSP->Q931
//DSP DSS1子功能定义 end

//DSP 固件子功能定义
#define  DSP_SUB_FUNCTION_FIRMWARE_READ_FLASH        0x01      //读FLASH命令
#define  DSP_SUB_FUNCTION_FIRMWARE_WRITE_FLASH       0x02      //写FLASH命令
#define  DSP_SUB_FUNCTION_FIRMWARE_ERASE_FLASH       0x03      //擦除FLASH命令
#define  DSP_SUB_FUNCTION_FIRMWARE_FINISH_FLASH      0x04      //更新FLASH完成命令
#define  DSP_SUB_FUNCTION_FIRMWARE_READ_SDRAM        0x06      //读SDRAM命令
#define  DSP_SUB_FUNCTION_REBOOT                     0x07      //DSP重启命令
//DSP 固件子功能定义

//******************************************************************************

#define  MEDIA_MAIN_FUNCTION_STREAMPLAY    0x01           //流放音
//媒体功能模块功能ID定义***************************************
#define  MEDIA_MAIN_FUNCTION_STREAMRECORD  0x02           //流录音

//SS7信令功能模块功能ID定义***************************************
#define  SS7_MAIN_FUNCTION_ISUP      0x01           //七号信令
#define  SS7_MAIN_FUNCTION_TUP       0x02

//PRI信令功能模块功能ID定义***************************************
#define  PRI_MAIN_FUNCTION_Q931      0x01           //Q.931信令


//配置模块功能ID定义***************************************

#define  CONFIG_MAIN_FUNCTION_INIT        0x01           //初始化模块
#define  CONFIG_MAIN_FUNCTION_START       0x02           //启动模块
#define  CONFIG_MAIN_FUNCTION_STOP        0x03           //停止模块
#define  CONFIG_MAIN_FUNCTION_RELATE      0x04           //关联模块
#define  CONFIG_MAIN_FUNCTION_UNRELATE    0x05           //停止关联模块
#define  CONFIG_MAIN_FUNCTION_MONCONFIG   0x06           //监视配置
#define  CONFIG_MAIN_FUNCTION_MONSTART    0x07           //监视启动
#define  CONFIG_MAIN_FUNCTION_MONSTOP     0x08           //监视停止
#define  CONFIG_MAIN_FUNCTION_HEART       0x09           //心跳包
#define  CONFIG_MAIN_FUNCTION_VALIDITY    0x0A           //系统连接验证包
#define  CONFIG_MAIN_FUNCTION_RELEAT      0x0B
#define  CONFIG_MAIN_FUNCTION_MODSTATE_REPORT 0x0C

#define  CONFIG_MAIN_FUNCTION_INTERFACE   0x10           //界面模块发送过来的数据


#define CONFIG_SUB_FUNCTION_INTERFACE_REQALL      0x01    //请求所有模块数据
#define CONFIG_SUB_FUNCTION_INTERFACE_REQSINGAL   0x02    //请求某个模块数据
#define CONFIG_SUB_FUNCTION_INTERFACE_REQINIT     0x03    //请求某个模块初始化包数据
#define CONFIG_SUB_FUNCTION_INTERFACE_SETPARM     0x04    //设置某个模块参数(例如IP,是否使能等)
#define CONFIG_SUB_FUNCTION_INTERFACE_SETINIT     0x05    //设置模块初始化包
#define CONFIG_SUB_FUNCTION_INTERFACE_START       0x06    //模块初始化和启动
#define CONFIG_SUB_FUNCTION_INTERFACE_STOP        0x07    //模块停止
#define CONFIG_SUB_FUNCTION_INTERFACE_REQRELATE   0x08    //请求模块关联信息
#define CONFIG_SUB_FUNCTION_INTERFACE_TRANRELATE  0x09    //模块关联信息
#define CONFIG_SUB_FUNCTION_INTERFACE_ADD         0x0a    //增加一个模块
#define CONFIG_SUB_FUNCTION_INTERFACE_SUB         0x0b    //删除一个模块

#define CONFIG_SUB_FUNCTION_INTERFACE_HEART       0x10   //心跳包


//界面模块功能ID定义***************************************
#define FACE_MAIN_FUNCTION_REQNODE                0x01    //请求节点定义
#define FACE_MAIN_FUNCTION_READNODE               0x02    //读节点数据
#define FACE_MAIN_FUNCTION_WRITENODE              0x03    //写节点数据
#define FACE_MAIN_FUNCTION_SAVECONFIG             0x04    //保存节点配置
#define FACE_MAIN_FUNCTION_LOADCONFIG             0x05    //转载节点配置

#define FACE_MAIN_FUNCTION_HEART                  0x09    //界面的心跳包


//流程执行模块功能ID定义*************************************
#define  CTX_MAIN_FUNCTION_CTXCMD                0xFA	   //交换中心命令
//流程执行模块子功能ID定义*************************************
#define  CTX_SUB_FUNCTION_CTXREG                 0x01     //ACS发送交换中心注册命令
#define  CTX_SUB_FUNCTION_CTXLINK                0x02     //ACS发送交换中心Link命令
#define  CTX_SUB_FUNCTION_CTXAPPDATA             0x03     //ACS发送注册交换中AppData心命令

//流程执行模块功能ID定义*************************************
#define  FLOW_MAIN_FUNCTION_SYNCMD                0xFB	   //流程同步命令

#define  FLOW_MAIN_FUNCTION_ACSEVT                0xFC	   //流程API事件

#define  FLOW_MAIN_FUNCTION_ACSCMD                0xFD	   //流程命令
//流程执行模块子功能ID定义*************************************
#define  FLOW_SUB_FUNCTION_INTERCMD               0x01     //流程内部命令
#define  FLOW_SUB_FUNCTION_REQDEVICE              0x02     //ACS请求资源列表
#define  FLOW_SUB_FUNCTION_OPENDEVICE             0x03     //ACS打开设备
#define  FLOW_SUB_FUNCTION_CLOSEDEVICE            0x04     //ACS关闭设备
#define  FLOW_SUB_FUNCTION_RESETDEVICE            0x05     //ACS复位设备
#define  FLOW_SUB_FUNCTION_GETDEVSTATE            0x06     //ACS获取设备状态
#define  FLOW_SUB_FUNCTION_SETDEVGROUP            0x07     //ACS设置资源组号
#define  FLOW_SUB_FUNCTION_SETPARAM               0x08     //ACS设置参数
#define  FLOW_SUB_FUNCTION_GETPARAM               0x09     //ACS设置参数 ...
#define  FLOW_SUB_FUNCTION_MAKECALLOUT            0x0A     //ACS发起呼叫
#define  FLOW_SUB_FUNCTION_ALERTCALL              0x0B     //ACS Alert呼叫
#define  FLOW_SUB_FUNCTION_ANSWERCALLIN           0x0C     //ACS应答呼入
#define  FLOW_SUB_FUNCTION_LINKDEV                0x0D     //ACS连接设备
#define  FLOW_SUB_FUNCTION_UNLINKDEV              0x0E     //ACS拆除设备连接
#define  FLOW_SUB_FUNCTION_CLEARCALL              0x0F     //ACS清除呼叫
#define  FLOW_SUB_FUNCTION_JOINTOCONF             0x10     //ACS加入会议
#define  FLOW_SUB_FUNCTION_LEAVEFROMCONF          0x11     //ACS离开会议
#define  FLOW_SUB_FUNCTION_CLEARCONF              0x12     //ACS删除会议
#define  FLOW_SUB_FUNCTION_PLAYFILE               0x13     //ACS放音
#define  FLOW_SUB_FUNCTION_INITPLAYINDEX          0x14     //ACS初始化索引
#define  FLOW_SUB_FUNCTION_BUILDINDEX             0x15     //ACS建立放音索引
#define  FLOW_SUB_FUNCTION_CONTROLPLAY            0x16     //ACS控制放音
#define  FLOW_SUB_FUNCTION_RECORDFILE             0x17     //ACS录音
#define  FLOW_SUB_FUNCTION_CONTROLRECORD          0x18     //ACS控制录音
#define  FLOW_SUB_FUNCTION_SENDFAX                0x19     //ACS发送传真
#define  FLOW_SUB_FUNCTION_STOPSENDFAX            0x1A     //ACS停止发送传真
#define  FLOW_SUB_FUNCTION_RECVFAX                0x1B     //ACS接收传真
#define  FLOW_SUB_FUNCTION_STOPRECVFAX            0x1C     //ACS停止接收传真
#define  FLOW_SUB_FUNCTION_CHANGEMONITORFILTER    0x1D     //ACS改变事件filter
#define  FLOW_SUB_FUNCTION_SENDIODATA             0x1E     //ACS发送IO数据
#define  FLOW_SUB_FUNCTION_SENDSIGMSG             0x1F     //ACS发送信令消息
#define  FLOW_SUB_FUNCTION_RECORDCSP              0x20     //ACS内存录音
#define  ACS_SUB_FUNCTION_DBGON                   0x22     //调试使能
#define  ACS_SUB_FUNCTION_DBGOFF                  0x23     //调试禁止
#define  FLOW_SUB_FUNCTION_PLAY3GPP               0x24     //播放3gpp
#define  FLOW_SUB_FUNCTION_CONTROLPLAY3GPP        0x25     //控制播放3gpp
#define  FLOW_SUB_FUNCTION_RECORD3GPP             0x26     //播放3gpp
#define  FLOW_SUB_FUNCTION_CONTROLRECORD3GPP      0x27     //控制播放3gpp
#define  FLOW_SUB_FUNCTION_PLAYCSP                0x28     //ACS启动CSP放音
#define  FLOW_SUB_FUNCTION_SENDPLAYCSP            0x29     //ACS发送CSP放音数据
#define  FLOW_SUB_FUNCTION_CTXREG                 0x2A     //ACS发送Ctx Reg事件
#define  FLOW_SUB_FUNCTION_CTXLINK                0x2B     //ACS发送CTX Link事件
#define  FLOW_SUB_FUNCTION_CTXAPPDATA             0x2C     //ACS发送CTX AppData事件
#define  FLOW_SUB_FUNCTION_LICQUERY               0x2E     //ACS发送License query
#define  FLOW_SUB_FUNCTION_LINKQUERY              0x2F     //ACS发送Link query
#define  FLOW_SUB_FUNCTION_INIT3GPPINDEX          0x30     //ACS初始化3gpp索引
#define  FLOW_SUB_FUNCTION_BUILD3GPPINDEX         0x31     //ACS建立3gpp索引
#define  FLOW_SUB_FUNCTION_RESETCHAN              0xFF     //发送复位通道事件


//流程执行模块功能ID定义*************************************
#define  FLOW_MAIN_FUNCTION_TIMEREVT              0xFE	   //定时器事件
#define  MOD_MAIN_FUNCTION_MODHEART               0xFF	   //各模块之间的心跳包


//IP信令模块功能ID定义*******************************************

//faxTiff模块主功能
#define FAXTIFF_MAIN_FUNCTION_CHANGE  0x01

//faxTiff模块子功能
//命令包定义
#define FAXTIFF_SUB_FUNCTION_CHANGE_DECOMPOSE  0x01  //分解
#define FAXTIFF_SUB_FUNCTION_CHANGE_COMPOSE  0x02    //合成

//事件包定义
#define FAXTIFF_SUB_FUNCTION_DECOMPOSE_RESULT  0x01  //分解结果
#define FAXTIFF_SUB_FUNCTION_COMPOSE_RESULT    0x02  //合成结果

//****************************************************************
//#endif

