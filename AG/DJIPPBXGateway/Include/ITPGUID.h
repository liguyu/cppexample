//*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ITPGUID.h                                                              
//Version       :  1.1                                                              
//Author        :  ����                                                             
//DateTime      :  2005-07-14 15:00                                           
//Description   :  ITPϵͳGUID����                                                                
//*****************************************************************************

#define  MODULE_TYPE_DSP    	0x01           //DSP����ģ��
#define  MODULE_TYPE_MEDIA	    0x02           //ý�崦��ģ��
#define  MODULE_TYPE_SS7        0x03           //�ߺŴ���ģ��	
#define  MODULE_TYPE_CFG 	    0x04           //����ģ��
#define  MODULE_TYPE_MONITOR 	0x05           //���ģ��
#define  MODULE_TYPE_FLOW       0x06	       //����ģ��
#define  MODULE_TYPE_PRI        0x07	       //ISDN����ģ��
#define  MODULE_TYPE_USER       0x08           //�û�ģ��
#define  MODULE_TYPE_INTERFACE  0x09           //����ģ��
#define  MODULE_TYPE_VOIP       0x0A           //VoIPģ��
#define  MODULE_TYPE_3G324M     0x0B           //3G-324Mģ��
#define  MODULE_TYPE_FAXTIFF    0x0E           //FAXTIFFģ��
#define  MODULE_TYPE_ACS        0x0F           //APIģ��
#define  MODULE_TYPE_SIGMON     0x10           //������ģ��
#define  MODULE_TYPE_CTXM       0x11           //��������ģ��
	
//DSP����ģ�鹦��ID����***************************************
#define  DSP_MAIN_FUNCTION_CONFIG      0x01           //ȫ���豸����
#define  DSP_MAIN_FUNCTION_SPEECH      0x02           //��������
#define  DSP_MAIN_FUNCTION_FAX         0x03           //���湦��
#define  DSP_MAIN_FUNCTION_DIGITAL     0x04           //�����м̹���
#define  DSP_MAIN_FUNCTION_INTERFACE   0x05           //��ϯ����ģ��
#define  DSP_MAIN_FUNCTION_PRILINK     0x06           //���鹦��ģ��
#define  DSP_MAIN_FUNCTION_SS7LINK     0x07           //�����м̹���ģ��
#define  DSP_MAIN_FUNCTION_CTCLK       0x08           //CT_BUSʱ��
#define  DSP_MAIN_FUNCTION_CTTS        0x09           //CT_BUS��Դ
#define  DSP_MAIN_FUNCTION_CONNECTTS   0x0A           //ʱ϶����
#define  DSP_MAIN_FUNCTION_FIRMWARE    0x0B           //�̼�����
#define  DSP_MAIN_FUNCTION_VOIP        0x0C           //VoIP����
#define  DSP_MAIN_FUNCTION_3G324M      0x0D           //3G-324M����
#define  DSP_MAIN_FUNCTION_LICENSE     0x0E           //license alarm


#define  SUB_FUNCTION_ALL                0xFF        //�����ӹ���

#define  DSP_SUB_FUNCTION_SPEECH_INPUT   0x01        //���빦��(EC,AGC��)
#define  DSP_SUB_FUNCTION_SPEECH_OUTPUT  0x02        //�������(����,AGC��)
#define  DSP_SUB_FUNCTION_SPEECH_PLAY    0x03        //��������
#define  DSP_SUB_FUNCTION_SPEECH_RECORD  0x04        //¼������
#define  DSP_SUB_FUNCTION_SPEECH_GTD     0x05        //������Ƶ������
#define  DSP_SUB_FUNCTION_SPEECH_CONF    0x06        //���鴦����


//DSP DSS1�ӹ��ܶ���
#define  DSP_SUB_FUNCTION_DSS1_CFG_CMD_TO_DSP        0x01      //DSS1�������        Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_GET_CFG_TO_DSP        0x02      //��ȡDSS1������Ϣ���Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_CFG_INFO_TO_Q931      0x02      //DSS1������Ϣ��        DSP->Q931   
#define  DSP_SUB_FUNCTION_DSS1_CONTROL_CMD_TO_DSP    0x03      //DSS1��������          Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_STATE_EVENT_TO_Q931   0x03      //DSS1״̬�¼�          DSP->Q931 
#define  DSP_SUB_FUNCTION_DSS1_SIGNAL_DATA_TO_DSP    0x04      //DSS1��������          Q931->DSP
#define  DSP_SUB_FUNCTION_DSS1_SIGNAL_DATA_TO_Q931   0x04      //DSS1��������          DSP->Q931
#define  DSP_SUB_FUNCTION_DSS1_DATA_REQ_TO_Q931      0x05      //DSS1���������        DSP->Q931
#define  DSP_SUB_FUNCTION_DSS1_DEBUG_DATA_TO_Q931    0x06      //DSS1������Ϣ��        DSP->Q931
//DSP DSS1�ӹ��ܶ��� end

//DSP �̼��ӹ��ܶ���
#define  DSP_SUB_FUNCTION_FIRMWARE_READ_FLASH        0x01      //��FLASH����
#define  DSP_SUB_FUNCTION_FIRMWARE_WRITE_FLASH       0x02      //дFLASH����
#define  DSP_SUB_FUNCTION_FIRMWARE_ERASE_FLASH       0x03      //����FLASH����
#define  DSP_SUB_FUNCTION_FIRMWARE_FINISH_FLASH      0x04      //����FLASH�������
#define  DSP_SUB_FUNCTION_FIRMWARE_READ_SDRAM        0x06      //��SDRAM����
#define  DSP_SUB_FUNCTION_REBOOT                     0x07      //DSP��������
//DSP �̼��ӹ��ܶ���

//******************************************************************************

#define  MEDIA_MAIN_FUNCTION_STREAMPLAY    0x01           //������
//ý�幦��ģ�鹦��ID����***************************************
#define  MEDIA_MAIN_FUNCTION_STREAMRECORD  0x02           //��¼��

//SS7�����ģ�鹦��ID����***************************************
#define  SS7_MAIN_FUNCTION_ISUP      0x01           //�ߺ�����
#define  SS7_MAIN_FUNCTION_TUP       0x02

//PRI�����ģ�鹦��ID����***************************************
#define  PRI_MAIN_FUNCTION_Q931      0x01           //Q.931����


//����ģ�鹦��ID����***************************************

#define  CONFIG_MAIN_FUNCTION_INIT        0x01           //��ʼ��ģ��
#define  CONFIG_MAIN_FUNCTION_START       0x02           //����ģ��
#define  CONFIG_MAIN_FUNCTION_STOP        0x03           //ֹͣģ��
#define  CONFIG_MAIN_FUNCTION_RELATE      0x04           //����ģ��
#define  CONFIG_MAIN_FUNCTION_UNRELATE    0x05           //ֹͣ����ģ��
#define  CONFIG_MAIN_FUNCTION_MONCONFIG   0x06           //��������
#define  CONFIG_MAIN_FUNCTION_MONSTART    0x07           //��������
#define  CONFIG_MAIN_FUNCTION_MONSTOP     0x08           //����ֹͣ
#define  CONFIG_MAIN_FUNCTION_HEART       0x09           //������
#define  CONFIG_MAIN_FUNCTION_VALIDITY    0x0A           //ϵͳ������֤��
#define  CONFIG_MAIN_FUNCTION_RELEAT      0x0B
#define  CONFIG_MAIN_FUNCTION_MODSTATE_REPORT 0x0C

#define  CONFIG_MAIN_FUNCTION_INTERFACE   0x10           //����ģ�鷢�͹���������


#define CONFIG_SUB_FUNCTION_INTERFACE_REQALL      0x01    //��������ģ������
#define CONFIG_SUB_FUNCTION_INTERFACE_REQSINGAL   0x02    //����ĳ��ģ������
#define CONFIG_SUB_FUNCTION_INTERFACE_REQINIT     0x03    //����ĳ��ģ���ʼ��������
#define CONFIG_SUB_FUNCTION_INTERFACE_SETPARM     0x04    //����ĳ��ģ�����(����IP,�Ƿ�ʹ�ܵ�)
#define CONFIG_SUB_FUNCTION_INTERFACE_SETINIT     0x05    //����ģ���ʼ����
#define CONFIG_SUB_FUNCTION_INTERFACE_START       0x06    //ģ���ʼ��������
#define CONFIG_SUB_FUNCTION_INTERFACE_STOP        0x07    //ģ��ֹͣ
#define CONFIG_SUB_FUNCTION_INTERFACE_REQRELATE   0x08    //����ģ�������Ϣ
#define CONFIG_SUB_FUNCTION_INTERFACE_TRANRELATE  0x09    //ģ�������Ϣ
#define CONFIG_SUB_FUNCTION_INTERFACE_ADD         0x0a    //����һ��ģ��
#define CONFIG_SUB_FUNCTION_INTERFACE_SUB         0x0b    //ɾ��һ��ģ��

#define CONFIG_SUB_FUNCTION_INTERFACE_HEART       0x10   //������


//����ģ�鹦��ID����***************************************
#define FACE_MAIN_FUNCTION_REQNODE                0x01    //����ڵ㶨��
#define FACE_MAIN_FUNCTION_READNODE               0x02    //���ڵ�����
#define FACE_MAIN_FUNCTION_WRITENODE              0x03    //д�ڵ�����
#define FACE_MAIN_FUNCTION_SAVECONFIG             0x04    //����ڵ�����
#define FACE_MAIN_FUNCTION_LOADCONFIG             0x05    //ת�ؽڵ�����

#define FACE_MAIN_FUNCTION_HEART                  0x09    //�����������


//����ִ��ģ�鹦��ID����*************************************
#define  CTX_MAIN_FUNCTION_CTXCMD                0xFA	   //������������
//����ִ��ģ���ӹ���ID����*************************************
#define  CTX_SUB_FUNCTION_CTXREG                 0x01     //ACS���ͽ�������ע������
#define  CTX_SUB_FUNCTION_CTXLINK                0x02     //ACS���ͽ�������Link����
#define  CTX_SUB_FUNCTION_CTXAPPDATA             0x03     //ACS����ע�ύ����AppData������

//����ִ��ģ�鹦��ID����*************************************
#define  FLOW_MAIN_FUNCTION_SYNCMD                0xFB	   //����ͬ������

#define  FLOW_MAIN_FUNCTION_ACSEVT                0xFC	   //����API�¼�

#define  FLOW_MAIN_FUNCTION_ACSCMD                0xFD	   //��������
//����ִ��ģ���ӹ���ID����*************************************
#define  FLOW_SUB_FUNCTION_INTERCMD               0x01     //�����ڲ�����
#define  FLOW_SUB_FUNCTION_REQDEVICE              0x02     //ACS������Դ�б�
#define  FLOW_SUB_FUNCTION_OPENDEVICE             0x03     //ACS���豸
#define  FLOW_SUB_FUNCTION_CLOSEDEVICE            0x04     //ACS�ر��豸
#define  FLOW_SUB_FUNCTION_RESETDEVICE            0x05     //ACS��λ�豸
#define  FLOW_SUB_FUNCTION_GETDEVSTATE            0x06     //ACS��ȡ�豸״̬
#define  FLOW_SUB_FUNCTION_SETDEVGROUP            0x07     //ACS������Դ���
#define  FLOW_SUB_FUNCTION_SETPARAM               0x08     //ACS���ò���
#define  FLOW_SUB_FUNCTION_GETPARAM               0x09     //ACS���ò��� ...
#define  FLOW_SUB_FUNCTION_MAKECALLOUT            0x0A     //ACS�������
#define  FLOW_SUB_FUNCTION_ALERTCALL              0x0B     //ACS Alert����
#define  FLOW_SUB_FUNCTION_ANSWERCALLIN           0x0C     //ACSӦ�����
#define  FLOW_SUB_FUNCTION_LINKDEV                0x0D     //ACS�����豸
#define  FLOW_SUB_FUNCTION_UNLINKDEV              0x0E     //ACS����豸����
#define  FLOW_SUB_FUNCTION_CLEARCALL              0x0F     //ACS�������
#define  FLOW_SUB_FUNCTION_JOINTOCONF             0x10     //ACS�������
#define  FLOW_SUB_FUNCTION_LEAVEFROMCONF          0x11     //ACS�뿪����
#define  FLOW_SUB_FUNCTION_CLEARCONF              0x12     //ACSɾ������
#define  FLOW_SUB_FUNCTION_PLAYFILE               0x13     //ACS����
#define  FLOW_SUB_FUNCTION_INITPLAYINDEX          0x14     //ACS��ʼ������
#define  FLOW_SUB_FUNCTION_BUILDINDEX             0x15     //ACS������������
#define  FLOW_SUB_FUNCTION_CONTROLPLAY            0x16     //ACS���Ʒ���
#define  FLOW_SUB_FUNCTION_RECORDFILE             0x17     //ACS¼��
#define  FLOW_SUB_FUNCTION_CONTROLRECORD          0x18     //ACS����¼��
#define  FLOW_SUB_FUNCTION_SENDFAX                0x19     //ACS���ʹ���
#define  FLOW_SUB_FUNCTION_STOPSENDFAX            0x1A     //ACSֹͣ���ʹ���
#define  FLOW_SUB_FUNCTION_RECVFAX                0x1B     //ACS���մ���
#define  FLOW_SUB_FUNCTION_STOPRECVFAX            0x1C     //ACSֹͣ���մ���
#define  FLOW_SUB_FUNCTION_CHANGEMONITORFILTER    0x1D     //ACS�ı��¼�filter
#define  FLOW_SUB_FUNCTION_SENDIODATA             0x1E     //ACS����IO����
#define  FLOW_SUB_FUNCTION_SENDSIGMSG             0x1F     //ACS����������Ϣ
#define  FLOW_SUB_FUNCTION_RECORDCSP              0x20     //ACS�ڴ�¼��
#define  ACS_SUB_FUNCTION_DBGON                   0x22     //����ʹ��
#define  ACS_SUB_FUNCTION_DBGOFF                  0x23     //���Խ�ֹ
#define  FLOW_SUB_FUNCTION_PLAY3GPP               0x24     //����3gpp
#define  FLOW_SUB_FUNCTION_CONTROLPLAY3GPP        0x25     //���Ʋ���3gpp
#define  FLOW_SUB_FUNCTION_RECORD3GPP             0x26     //����3gpp
#define  FLOW_SUB_FUNCTION_CONTROLRECORD3GPP      0x27     //���Ʋ���3gpp
#define  FLOW_SUB_FUNCTION_PLAYCSP                0x28     //ACS����CSP����
#define  FLOW_SUB_FUNCTION_SENDPLAYCSP            0x29     //ACS����CSP��������
#define  FLOW_SUB_FUNCTION_CTXREG                 0x2A     //ACS����Ctx Reg�¼�
#define  FLOW_SUB_FUNCTION_CTXLINK                0x2B     //ACS����CTX Link�¼�
#define  FLOW_SUB_FUNCTION_CTXAPPDATA             0x2C     //ACS����CTX AppData�¼�
#define  FLOW_SUB_FUNCTION_LICQUERY               0x2E     //ACS����License query
#define  FLOW_SUB_FUNCTION_LINKQUERY              0x2F     //ACS����Link query
#define  FLOW_SUB_FUNCTION_INIT3GPPINDEX          0x30     //ACS��ʼ��3gpp����
#define  FLOW_SUB_FUNCTION_BUILD3GPPINDEX         0x31     //ACS����3gpp����
#define  FLOW_SUB_FUNCTION_RESETCHAN              0xFF     //���͸�λͨ���¼�


//����ִ��ģ�鹦��ID����*************************************
#define  FLOW_MAIN_FUNCTION_TIMEREVT              0xFE	   //��ʱ���¼�
#define  MOD_MAIN_FUNCTION_MODHEART               0xFF	   //��ģ��֮���������


//IP����ģ�鹦��ID����*******************************************

//faxTiffģ��������
#define FAXTIFF_MAIN_FUNCTION_CHANGE  0x01

//faxTiffģ���ӹ���
//���������
#define FAXTIFF_SUB_FUNCTION_CHANGE_DECOMPOSE  0x01  //�ֽ�
#define FAXTIFF_SUB_FUNCTION_CHANGE_COMPOSE  0x02    //�ϳ�

//�¼�������
#define FAXTIFF_SUB_FUNCTION_DECOMPOSE_RESULT  0x01  //�ֽ���
#define FAXTIFF_SUB_FUNCTION_COMPOSE_RESULT    0x02  //�ϳɽ��

//****************************************************************
//#endif

