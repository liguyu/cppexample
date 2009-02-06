#ifndef __XMS_ISUP_H__
#define __XMS_ISUP_H__


// ISUP������Ϣ����(������Ҫֱ�ӷ��Ͳ���) /////////////////////////////////////
enum XMS_ISUP_Signalling_Message     // ISUP������Ϣ
{	
	ISUP_SM_BLO = 0x13,              // ���
	
	ISUP_SM_CGB = 0x18,              // ��·Ⱥ���
	ISUP_SM_CGU = 0x19,              // ��·Ⱥ�������	
	ISUP_SM_CPG = 0x2C,              // ���н�չ
	ISUP_SM_CQM = 0x2A,              // ��·Ⱥ��Ѷ
	
	ISUP_SM_GRS = 0x17,              // ��·Ⱥ��ԭ
	
	ISUP_SM_INF = 0x04,              // ��Ϣ
	ISUP_SM_INR = 0x03,              // ��Ϣ����
	
	ISUP_SM_RES = 0x0E,              // �ָ�	
	ISUP_SM_RSC = 0x12,              // ��·��ԭ
	
	ISUP_SM_SAM = 0x02,              // ������ַ
	ISUP_SM_SGM = 0x38,              // �ֶ�
	ISUP_SM_SUS = 0x0D,              // ��ͣ
	
	ISUP_SM_UBL = 0x14,              // �������	
	ISUP_SM_USR = 0x2D               // �û����û���Ϣ
};


// ISUP����������� ///////////////////////////////////////////////////////////
enum XMS_ISUP_Signalling_Parameter                     // ISUP�������
{
	ISUP_SP_AccessDeliveryInformation          = 0x2E, // ���봫����Ϣ
	ISUP_SP_AccessTransport                    = 0x03, // ���봫��
	ISUP_SP_AutomaticCongestionLevel           = 0x27, // �Զ�ӵ����
	ISUP_SP_BackwardCallIndicator              = 0x11, // �������ָʾ��
	ISUP_SP_CallDiversionInformation           = 0x36, // ����ת����Ϣ
	ISUP_SP_CallHistoryInformation             = 0x2D, // ������ʷ��Ϣ
	ISUP_SP_CallReference                      = 0x01, // ���вο�
	ISUP_SP_CalledPartNumber                   = 0x04, // �����û�����
	ISUP_SP_CallingPartNumber                  = 0x0A, // �����û�����
	ISUP_SP_CallingPartCategory                = 0x09, // �����û����
	ISUP_SP_CauseIndicator                     = 0x12, // ԭ��ָʾ��
	ISUP_SP_CircuitGroupSupervisionIndicator   = 0x15, // ��·Ⱥ������Ϣ����ָʾ��
	ISUP_SP_CircuitStateIndicator              = 0x26, // ��·״ָ̬ʾ��
	ISUP_SP_CUGInterlockCode                   = 0x1A, // �պ��û�Ⱥ�����
	ISUP_SP_ConnectedNumber                    = 0x21, // �����ĺ���
	ISUP_SP_ConnectionRequest                  = 0x0D, // ��������
	ISUP_SP_ContitnuityIndicator               = 0x10, // ��ָͨʾ��
	ISUP_SP_EchoControlInformation             = 0x37, // ����������Ϣ
	ISUP_SP_EndOptionalParameter               = 0x00, // ��ѡ����βָʾ��
	ISUP_SP_EventInformation                   = 0x24, // �¼���Ϣ
	ISUP_SP_FacilityIndicator                  = 0x18, // ����ָʾ��
	ISUP_SP_ForwardCallIndicator               = 0x07, // ǰ�����ָʾ��
	ISUP_SP_GenericDigit                       = 0x47, // ��������
	ISUP_SP_GenericNotifyIndicator             = 0x2C, // ����ָ֪ͨʾ��
	ISUP_SP_GenericNumber                      = 0x46, // ��������
	ISUP_SP_GenericReference                   = 0x42, // �����ο�
	ISUP_SP_InformationIndicator               = 0x0F, // ��Ϣָʾ��
	ISUP_SP_InformationRequestIndicator        = 0x0E, // ��Ϣ����ָʾ��
	ISUP_SP_LocationNumber                     = 0x3F, // ��λ����
	ISUP_SP_MCIDRequestIndicator               = 0x3B, // MCID����ָʾ��
	ISUP_SP_MCIDResponseIndicator              = 0x3C, // MCID��Ӧָʾ��
	ISUP_SP_MsgCompatibilityInformation        = 0x38, // ��Ϣ��������Ϣ
	ISUP_SP_MlppPrecedence                     = 0x3A, // MLPP����
	ISUP_SP_NatureOfConnectionIndicator        = 0x06, // ��������ָʾ��
	ISUP_SP_NetworkSpecificFacility            = 0x2F, // �����ض�����
	ISUP_SP_OptBackwardCallIndicator           = 0x29, // ��ѡ�������ָʾ��
	ISUP_SP_OptForwardCallIndicator            = 0x08, // ��ѡǰ�����ָʾ��
	ISUP_SP_OriginalCalledNumber               = 0x28, // ԭ���ı��к���
	ISUP_SP_OriginationISCPointCode            = 0x2B, // ʼ�����ʽ�������(ISC)�ĵ���
	ISUP_SP_ParamCompatibilityInformation      = 0x39, // ������������Ϣ
	ISUP_SP_PropagationDelayCounter            = 0x31, // �����ӳټ�����
	ISUP_SP_RangeState                         = 0x16, // ��Χ��״̬
	ISUP_SP_RedirectingNumber                  = 0x0B, // �ķ�����
	ISUP_SP_RedirectionInformation             = 0x13, // �ķ���Ϣ
	ISUP_SP_RedirectionNumber                  = 0x0C, // �ķ�����
	ISUP_SP_RedirectionRestricition            = 0x40, // �ķ��������Ʋ���
	ISUP_SP_RemoteOperation                    = 0x32, // Զ�˲���
	ISUP_SP_ServiceActivation                  = 0x33, // ҵ�񼤻�
	ISUP_SP_SignalPointCode                    = 0x1E, // ��������
	ISUP_SP_SubsequentNumber                   = 0x05, // ��������
	ISUP_SP_SuspendResumeIndicator             = 0x22, // ��ͣ/�ָ�ָʾ��
	ISUP_SP_TransitNetworkSelection            = 0x23, // �������ѡ��
	ISUP_SP_TransmissionMediumRequirement      = 0x02, // ����ý��Ҫ��
	ISUP_SP_TransmissionMediumRequirementPrime = 0x3E, // ����ý��Ҫ��(������)
	ISUP_SP_TransmissionMediumUsed             = 0x35, // ʹ�õĴ���ý��
	ISUP_SP_UserServiceInformation             = 0x1D, // �û�ҵ����Ϣ
	ISUP_SP_UserServiceInformationPrime        = 0x30, // �û�ҵ����Ϣ(������)	
	ISUP_SP_UserTeleserviceInformation         = 0x34, // �û��ն�ҵ����Ϣ	
	ISUP_SP_UserToUserIndicator                = 0x2A, // �û����û�ָʾ��	
	ISUP_SP_UserToUserInformation              = 0x20  // �û����û���Ϣ
};

// ISUP����������ݽṹ���� ///////////////////////////////////////////////////
// ���봫����Ϣ: ISUP_SP_AccessDeliveryInformation ////////////////////////////
typedef struct
{
	DJ_U8 m_u8AccessDeliveryID;  // ���봫��ָʾ��	
	DJ_U8 m_u8Reserved[3];       // ����
} ISUP_spAccessDeliveryInformation, *PISUP_spAccessDeliveryInformation;


// ���봫��: ISUP_SP_AccessTransport //////////////////////////////////////////
typedef struct
{
	DJ_U8 m_u8AtpInformation[240];  // ��Ϣ��Ԫ��4.5/Q.931��������

	DJ_U8 m_u8AtpLen;               // ��������
	DJ_U8 m_u8Reserved[3];          // ����
} ISUP_spAccessTransport, *PISUP_spAccessTransport;


// �Զ�ӵ����: ISUP_SP_AutomaticCongestionLevel ///////////////////////////////
typedef struct
{	
	DJ_U8 m_u8CongestionLevelExceeded;     // ӵ��������ĳ������
	DJ_U8 m_u8Reserved[3];                 // ����
} ISUP_spAutomaticCongestionLevel, *PISUP_spAutomaticCongestionLevel;


// �������ָʾ��: ISUP_SP_BackwardCallIndicator //////////////////////////////
typedef struct
{	
	DJ_U8 m_u8ChargeIndicator;             // �Ʒ�ָʾ��
	DJ_U8 m_u8CalledStatusIndicator;       // �����û�״ָ̬ʾ��
	DJ_U8 m_u8CalledCategoryIndicator;     // �����û����ָʾ��
 	DJ_U8 m_u8EndToEndMethodIndicator;     // �˵��˷���ָʾ��
	
	DJ_U8 m_u8InterworkingIndicator;       // ��ָͨʾ��
	DJ_U8 m_u8EndToEndInformationIndicator;// �˵�����Ϣָʾ��
	DJ_U8 m_u8ISDNUserPartIndicator;       // ISDN�û�����ָʾ��
	DJ_U8 m_u8HoldingIndicator;            // ����ָʾ��
	DJ_U8 m_u8ISDNAccessIndicator;         // ISDN����ָʾ��
	DJ_U8 m_u8EchoControlDeviceIndicator;  // ������������ָʾ��
	DJ_U8 m_u8SCCPMethodIndicator;         // SCCP����ָʾ��
	DJ_U8 m_u8Reserved;                    // ����
} ISUP_spBackwardCallIndicator, *PISUP_spBackwardCallIndicator;


// ����ת����Ϣ: ISUP_SP_CallDiversionInformation /////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NotificationSubscriptionOptions; // ֪ͨ�Ǽ���ѡ
	DJ_U8 m_u8RedirectingReason;               // �ķ�ԭ��
	DJ_U8 m_u8Reserved[2];                     // ����
} ISUP_spCallDiversionInformation, *PISUP_spCallDiversionInformation;


// ������ʷ��Ϣ: ISUP_SP_CallHistoryInformation ///////////////////////////////
typedef struct
{	
	DJ_U16 m_u16PropagationDelayValue; // ����ʱ��ֵ
	DJ_U8  m_u8Reserved[2];            // ����
} ISUP_spCallHistoryInformation, *PISUP_spCallHistoryInformation;


// ���вο�: ISUP_SP_CallReference ////////////////////////////////////////////
typedef struct
{	
	DJ_U32  m_u32CallIdentity; // ����ʶ��

	DJ_U32  m_u32SPcode;       // ����
	DJ_U8   m_u8Reserved[4];   // ����
} ISUP_spCallReference, *PISUP_spCallReference;


// �����û�����: ISUP_SP_CalledPartNumber /////////////////////////////////////
typedef struct
{
	DJ_U8 m_u8NatureOfAddressIndicator;  // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;          // ��żָʾ��
	DJ_U8 m_u8NumberingPlanIndicator;    // ����ƻ�ָʾ
	DJ_U8 m_u8InternalNetworkNumberIndicator;// �ڲ��������ָʾ��

	DJ_U8 m_u8Reserved[4];               // ����
	
 	DJ_S8 m_s8AddressSignal[40];         // ��ַ�ź�	
} ISUP_spCalledPartNumber, *PISUP_spCalledPartNumber;


// �����û�����: ISUP_SP_CallingPartNumber ////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;         // ��żָʾ��
	
	DJ_U8 m_u8Screening;                // ����
	DJ_U8 m_u8AddressPresentationRestrictedIndicator;  // ��ַ��ʾָʾ
	DJ_U8 m_u8NumberingPlanIndicator;   // ����ƻ�ָʾ
	DJ_U8 m_u8NumberIncompleteIndicator;// ���к��벻ȫָʾ	
	
	DJ_S8 m_s8AddressSignal[40]; // ��ַ�ź�
	
	DJ_U8 m_u8Reserved[2];       // ����
} ISUP_spCallingPartNumber, *PISUP_spCallingPartNumber;


// �����û����: ISUP_SP_CallingPartCategory //////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8CallingPartyCategory; // �����û����
	DJ_U8 m_u8Reserved[3];          // ����
} ISUP_spCallingPartCategory, *PISUP_spCallingPartCategory;


// ԭ��ָʾ��: ISUP_SP_CauseIndicator /////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8Location;             // ��λ
	DJ_U8 m_u8CodindStandard;       // �����׼
	DJ_U8 m_u8ExtensionIndicator1;  // ����
	
	DJ_U8 m_u8CauseValue;           // ԭ��ֵ
	DJ_U8 m_u8ExtensionIndicator2;  // ����
	
	DJ_U8 m_u8Reserved[2];          // ����
	
	DJ_U8 m_u8Diagnose[32];         // ���	
	DJ_U8 m_u8DiagnoseLen;          // ��ϳ���
} ISUP_spCauseIndicator, *PISUP_spCauseIndicator;


// ��·Ⱥ������Ϣ����ָʾ��: ISUP_SP_CircuitGroupSupervisionIndicator /////////
typedef struct
{
	DJ_U8 m_u8CircuitGroupSupervisionIndicator; // ����ָʾ��
	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spCircuitGroupSupervisionIndicator, *PISUP_spCircuitGroupSupervisionIndicator;


// ��·״ָ̬ʾ��: ISUP_SP_CircuitStateIndicator //////////////////////////////
typedef struct
{	
	DJ_U8 m_u8CircuitStateIndicator[32];
} ISUP_spCircuitStateIndicator, *PISUP_spCircuitStateIndicator;


// �պ��û�Ⱥ�����: ISUP_SP_CUGInterlockCode /////////////////////////////////
typedef struct
{	
	DJ_U8  m_u8NIDigit1;  // ��1��NI����
	DJ_U8  m_u8NIDigit2;  // ��2��NI����
	DJ_U8  m_u8NIDigit3;  // ��3��NI����
	DJ_U8  m_u8NIDigit4;  // ��4��NI����	
	
	DJ_U16 m_u16BinaryCode; // ��������
	DJ_U8  m_u8Reserved[2]; // ����
} ISUP_spCUGInterlockCode, *PISUP_spCUGInterlockCode;


// �����ĺ���: ISUP_SP_ConnectedNumber ////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;         // ��żָʾ��
    
	DJ_U8 m_u8ScreeningIndicator;       // ����ָʾ��
	DJ_U8 m_u8AddressPresentationRestrictedIndicator; // ��ַ��ʾ����
	DJ_U8 m_u8NumberingPlanIndicator;   // ����ƻ�ָʾ	

	DJ_U8 m_u8Reserved[3];              // ����
	
	DJ_S8 m_s8AddressSignal[40];        // ��ַ�ź�	
} ISUP_spConnectedNumber, *PISUP_spConnectedNumber;


// ��������: ISUP_SP_ConnectionRequest ////////////////////////////////////////
typedef struct
{	
	DJ_U32 m_u32LocalReference; // ����ʶ��
	
	DJ_U32 m_u32SPcode;         // ����
	
	DJ_U8  m_u8ProtocolClass;   // Э�����
	DJ_U8  m_u8Credit;          // �ź���

	DJ_U8  m_u8Reserved[2];     // ����
} ISUP_spConnectionRequest, *PISUP_spConnectionRequest;


// ��ָͨʾ��: ISUP_SP_ContitnuityIndicator ///////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8ContinuityIndicator; // ��ָͨʾ��
	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spContitnuityIndicator, *PISUP_spContitnuityIndicator;


// ����������Ϣ: ISUP_SP_EchoControlInformation ///////////////////////////////
typedef struct
{	
	DJ_U8 m_u8OutEchoControlInformationIndicator; // ȥ�������������Ӧָʾ
	DJ_U8 m_u8InEchoControlInformationIndicator;  // ���������������Ӧָʾ
	DJ_U8 m_u8OutEchoControlRequestIndicator;     // ȥ���������������ָʾ
	DJ_U8 m_u8InEchoControlRequestIndicator;      // �����������������ָʾ
} ISUP_spEchoControlInformation, *PISUP_spEchoControlInformation;


// �¼���Ϣ: ISUP_SP_EventInformation /////////////////////////////////////////
typedef struct
{
	DJ_U8 m_u8EventIndicator; // �¼�ָʾ��
	DJ_U8 m_u8EventPresentationRestrictedIndicator;// �¼���ʾ����ָʾ��	
	DJ_U8 m_u8Reserved[2]; // ����
} ISUP_spEventInformation, *PISUP_spEventInformation;


// ����ָʾ��: ISUP_SP_FacilityIndicator //////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8FacilityIndicator; // ����ָʾ��
	DJ_U8 m_u8Reserved[3];
} ISUP_spFacilityIndicator, *PISUP_spFacilityIndicator;


// ǰ�����ָʾ��: ISUP_SP_ForwardCallIndicator ///////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NationalInternatoinalIndicator; // ����/���ʺ���ָʾ��
	DJ_U8 m_u8EndToEndMethodIndicator;        // �˵��˷���ָʾ��
	DJ_U8 m_u8InterworkingIndicator;          // ��ָͨʾ��
	DJ_U8 m_u8EndToEndInformationIndicator;   // �˵�����Ϣָʾ��
	DJ_U8 m_u8ISDNUserPartIndicator;          // ISDN�û�����ָʾ��
	DJ_U8 m_u8ISDNUserPartPreferenceIndicator;// ISDN�û���������ָʾ��
	
	DJ_U8 m_u8ISDNAccessIndicator; // ISDN����ָʾ��
	DJ_U8 m_u8SCCPMethodIndicator; // SCCP����ָʾ��
	
	DJ_U8 m_u8Reserved[4];         // ����
} ISUP_spForwardCallIndicator, *PISUP_spForwardCallIndicator;


// ��������: ISUP_SP_GenericDigit /////////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8TypeOfDigit;   // ��������
	DJ_U8 m_u8EncodingScheme;// ����ƻ�	
	
	DJ_U8 m_u8Digit[32];     // ����
	DJ_U8 m_u8DigitLen;      // ���ֳ���
	
	DJ_U8 m_u8Reserved;      // ����
} ISUP_spGenericDigit, *PISUP_spGenericDigit;


// ����ָ֪ͨʾ��: ISUP_SP_GenericNotifyIndicator /////////////////////////////
typedef struct
{
	DJ_U8 m_u8NotificationIndicator; // ָ֪ͨʾ��
	DJ_U8 m_u8ExtensionIndicator;    // ����ָʾ��
	DJ_U8 m_u8Reserved[2];           // ����
} ISUP_spGenericNotifyIndicator, *PISUP_spGenericNotifyIndicator;


// ��������: ISUP_SP_GenericNumber ////////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NumberQualifierIndicator;    // �����޶�ָʾ��
	
	DJ_U8 m_u8NatureOfAddressIndicator;// ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;  // ��żָʾ��	
	
	DJ_U8 m_u8ScreeningIndicator;// ����
	DJ_U8 m_u8AddressPresentationRestrictedIndicator; // ��ʾָʾ
	DJ_U8 m_u8NumberingPlanIndicator;    // ����ƻ�ָʾ
	DJ_U8 m_u8NumberIncompleteIndicator; //	���벻ȫָʾ	
	
	DJ_S8 m_s8AddressSignal[40]; // ��ַ�ź�
	
	DJ_U8 m_u8Reserved;          // ����
} ISUP_spGenericNumber, *PISUP_spGenericNumber;


// �����ο�: ISUP_SP_GenericReference /////////////////////////////////////////
typedef struct  
{
	DJ_U8 m_u8ReferenceRestrictedIndicator; // �ο��޶�ָʾ��
	DJ_U8 m_u8ReferenceLength;  // �ο�����	
	
	DJ_U8 m_u8ReferenceCode[4]; // �ο���λ����Ϣ

	DJ_U8 m_u8Reserved[2];      // ����
} ISUP_spGenericReference, *PISUP_spGenericReference;


// ��Ϣָʾ��: ISUP_SP_InformationIndicator ///////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8CallingPartyAddressResponseIndicator; // �����û���ַ��Ӧָʾ��
	DJ_U8 m_u8HoldProvidedIndicator; // �����ṩָʾ��
	
	DJ_U8 m_u8CallingPartyCategoryResponseIndicator; // �����û������Ӧָʾ��
	DJ_U8 m_u8ChargeInformationResponseIndicator;  // �Ʒ���Ϣ��Ӧָʾ��
	DJ_U8 m_u8SolicitedInformationIndicator;  // ������Ϣָʾ��
	
	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spInformationIndicator, *PISUP_spInformationIndicator;


// ��Ϣ��ʾָʾ��: ISUP_SP_InformationRequestIndicator ////////////////////////
typedef struct
{	
	DJ_U8 m_u8CallingPartyAddressRequestIndicator; // �����û���ַ����ָʾ��
	DJ_U8 m_u8HoldingIndicator; // ����ָʾ��
	
	DJ_U8 m_u8CallingPartyCategoryRequestIndicator;// �����û��������ָʾ��
	DJ_U8 m_u8ChargeInformationRequestIndicator;   // �Ʒ���Ϣ����ָʾ��
	
	DJ_U8 m_u8MaliciousCallIdentificationRequestIndicator; // �������ʶ��
	DJ_U8 m_u8Reserved[3];  // ����
} ISUP_spInformationRequestIndicator, *PISUP_spInformationRequestIndicator;


// ��λ����: ISUP_SP_LocationNumber ///////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;         // ��żָʾ��

	DJ_U8 m_u8ScreeningIndicator;       // ����ָʾ��
	DJ_U8 m_u8AddressPresentationRestrictedIndicator; // ��ַ��ʾ����ָʾ��
	DJ_U8 m_u8NumberingPlanIndicator;   // ����ƻ�ָʾ��
	DJ_U8 m_u8InternalNetworkNumberIndicator; // �ڲ��������ָʾ��	
	
	DJ_S8 m_s8AddressSignal[40]; // ��ַ�ź�

	DJ_U8 m_u8Reserved[2]; // ����
} ISUP_spLocationNumber, *PISUP_spLocationNumber;


// MCID����ָʾ��: ISUP_SP_MCIDRequestIndicator ///////////////////////////////
typedef struct
{	
	DJ_U8 m_u8MCIDRequestIndicator; // MCID����ָʾ��
	DJ_U8 m_u8HoldingIndicator;     // ����ָʾ��
	DJ_U8 m_u8Reserved[2]; // ����
} ISUP_spMCIDRequestIndicator, *PISUP_spMCIDRequestIndicator;


// MCID ��Ӧָʾ��: ISUP_SP_MCIDResponseIndicator /////////////////////////////
typedef struct
{	
	DJ_U8 m_u8MCIDResponseIndicator; // MCID��Ӧָʾ��
	DJ_U8 m_u8HoldProvidedIndicator; // �����ṩָʾ��
	DJ_U8 m_u8Reserved[2]; // ����
} ISUP_spMCIDResponseIndicator, *PISUP_spMCIDResponseIndicator;


// ��Ϣ��������Ϣ: ISUP_SP_MsgCompatibilityInformation ////////////////////////
typedef struct
{	
	DJ_U8 m_u8TransitAtIntermediateExchangeIndicator1;// ���м佻���ֻ��ָʾ��
	DJ_U8 m_u8ReleaseCallIndicator1;      // �ͷź���ָʾ��
	DJ_U8 m_u8SendNotificationIndicator1; // ����ָ֪ͨʾ��
	DJ_U8 m_u8DiscardMessageIndicator1;   // ������Ϣָʾ��
	DJ_U8 m_u8PassOnNotPossibleIndicator1;// ͨ��������ָʾ��
	DJ_U8 m_u8ExtensionIndicator1;        // ����ָʾ��
	DJ_U8 m_u8Reserved1[2];               // ����
	
	DJ_U8 m_u8TransitAtIntermediateExchangeIndicator2;// ���м佻���ֻ��ָʾ��
	DJ_U8 m_u8ReleaseCallIndicator2;      // �ͷź���ָʾ��
	DJ_U8 m_u8SendNotificationIndicator2; // ����ָ֪ͨʾ��
	DJ_U8 m_u8DiscardMessageIndicator2;   // ������Ϣָʾ��
	DJ_U8 m_u8PassOnNotPossibleIndicator2;// ͨ��������ָʾ��
	DJ_U8 m_u8ExtensionIndicator2;        // ����ָʾ��
	DJ_U8 m_u8Reserved2[2];               // ����
} ISUP_spMsgCompatibilityInformation, *PISUP_spMsgCompatibilityInformation;


// MLPP����: ISUP_SP_MlppPrecedence ///////////////////////////////////////////
typedef struct
{	
	DJ_U8  m_u8PrecedenceLevel;  // ���ȼ�	
	DJ_U8  m_u8LFB;        // LFB
	DJ_U8  m_u8Reserved[2];// ����
	
	DJ_U8  m_u8NIDigit1;  // ��1��NI����
	DJ_U8  m_u8NIDigit2;  // ��2��NI����
	DJ_U8  m_u8NIDigit3;  // ��3��NI����
	DJ_U8  m_u8NIDigit4;  // ��4��NI����	
	
	DJ_U32 m_u32MLPPServiceDomain;
} ISUP_spMlppPrecedence, *PISUP_spMlppPrecedence;


// ��������ָʾ��: ISUP_SP_NatureOfConnectionIndicator ////////////////////////
typedef struct
{	
	DJ_U8 m_u8SatelliteIndicator;        // ����ָʾ��
	DJ_U8 m_u8ContinuityCheckIndicator;  // ��ͨ����ָʾ��
	DJ_U8 m_u8EchoControlDeviceIndicator;// ������������ָʾ��
	DJ_U8 m_u8Reserved;
} ISUP_spNatureOfConnectionIndicator, *PISUP_spNatureOfConnectionIndicator;


// �����ض�����: ISUP_SP_NetworkSpecificFacility //////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NsfInformation[32];  // ������Ϣ
	
	DJ_U8 m_u8NsfLen;              // ��������
	DJ_U8 m_u8Reserved[3];         // ����	
} ISUP_spNetworkSpecificFacility, *PISUP_spNetworkSpecificFacility;


// ��ѡ�������ָʾ��: ISUP_SP_OptBackwardCallIndicator ///////////////////////
typedef struct
{	
	DJ_U8 m_u8InBandInformationIndicator;    // ������Ϣָʾ��
	DJ_U8 m_u8CallTransferPossibleIndicator; // ����ת�ƿ��ܷ���ָʾ��
	DJ_U8 m_u8SimpleSegmentationIndicator;   // �򵥷ֶ�ָʾ��
	DJ_U8 m_u8MLPPUserIndicator;             // MLPP�û�ָʾ��

	DJ_U8 m_u8Reserved[4];
} ISUP_spOptBackwardCallIndicator, *PISUP_spOptBackwardCallIndicator;


// ��ѡǰ�����ָʾ��: ISUP_SP_OptForwardCallIndicator ////////////////////////
typedef struct
{		
	DJ_U8 m_u8ClosedUserGroupCallIndicator; // CUG����ָʾ��
	DJ_U8 m_u8SimpleSegmentationIndicator;  // �򵥷ֶ�ָʾ��
	DJ_U8 m_u8UserIdentityRequestIndicator; // �������û�ʶ������ָʾ��
	DJ_U8 m_u8Reserved;                     // ����
} ISUP_spOptForwardCallIndicator, *PISUP_spOptForwardCallIndicator;


// ԭ���к���: ISUP_SP_OriginalCalledNumber ///////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;         // ��żָʾ��
	
	DJ_U8 m_u8AddressPresentationRestrictedIndicator; // ��ʾָʾ
	DJ_U8 m_u8NumberingPlanIndicator; // ����ƻ�ָʾ
	
	DJ_S8 m_s8AddressSignal[40]; // ��ַ�ź�
	
	DJ_U8 m_u8Reserved[4];  // ����
} ISUP_spOriginalCalledNumber, *PISUP_spOriginalCalledNumber;


// ʼ�����ʽ�������(ISC)�ĵ���: ISUP_SP_OriginationISCPointCode ///////////////
typedef struct
{
	DJ_U32 m_u32SPcode;     // ����
	DJ_U8  m_u8Reserved[4]; // ����
} ISUP_spOriginationISCPointCode, *PISUP_spOriginationISCPointCode;


// ������������Ϣ: ISUP_SP_ParamCompatibilityInformation //////////////////////
typedef struct
{
	DJ_U8 m_u8UpgradedParameter1;         // ��һ���������	
	DJ_U8 m_u8TransitAtIntermediateExchangeIndicator1;// ���м佻���ֻ��ָʾ��
	DJ_U8 m_u8ReleaseCallIndicator1;      // �ͷź���ָʾ��
	DJ_U8 m_u8SendNotificationIndicator1; // ����ָ֪ͨʾ��
	DJ_U8 m_u8DiscardMessageIndicator1;   // ������Ϣָʾ��
	DJ_U8 m_u8DiscardParameterIndicator1; // ��������ָʾ��
	DJ_U8 m_u8ExtensionIndicator1;        // ����ָʾ��
	DJ_U8 m_u8Reserved1;                  // ����
	
	DJ_U8 m_u8UpgradedParameter2;         // �ڶ����������	
	DJ_U8 m_u8TransitAtIntermediateExchangeIndicator2;// ���м佻���ֻ��ָʾ��
	DJ_U8 m_u8ReleaseCallIndicator2;      // �ͷź���ָʾ��
	DJ_U8 m_u8SendNotificationIndicator2; // ����ָ֪ͨʾ��
	DJ_U8 m_u8DiscardMessageIndicator2;   // ������Ϣָʾ��
	DJ_U8 m_u8DiscardParameterIndicator2; // ��������ָʾ��
	DJ_U8 m_u8ExtensionIndicator2;        // ����ָʾ��
	DJ_U8 m_u8Reserved2;                  // ����

	DJ_U8 m_u8UpgradedParameter3;         // �������������	
	DJ_U8 m_u8TransitAtIntermediateExchangeIndicator3;// ���м佻���ֻ��ָʾ��
	DJ_U8 m_u8ReleaseCallIndicator3;      // �ͷź���ָʾ��
	DJ_U8 m_u8SendNotificationIndicator3; // ����ָ֪ͨʾ��
	DJ_U8 m_u8DiscardMessageIndicator3;   // ������Ϣָʾ��
	DJ_U8 m_u8DiscardParameterIndicator3; // ��������ָʾ��
	DJ_U8 m_u8ExtensionIndicator3;        // ����ָʾ��
	DJ_U8 m_u8Reserved3;                  // ����
} ISUP_spParamCompatibilityInformation, *PISUP_spParamCompatibilityInformation;


// ������ʱ������: ISUP_SP_PropagationDelayCounter ////////////////////////////
typedef struct
{	
	DJ_U16 m_u16PropagationDelayValue;  // ����ʱ��ֵ
	DJ_U8  m_u8Reserved[2]; // ����
} ISUP_spPropagationDelayCounter, *PISUP_spPropagationDelayCounter;


// ��Χ��״̬: ISUP_SP_RangeState /////////////////////////////////////////////
typedef struct
{
	DJ_U8  m_u8Range;      // ��Χ
	DJ_U8  m_u8Reserved[3];// ����
	
	DJ_U32 m_u32Status;    // ״̬
} ISUP_spRangeState, *PISUP_spRangeState;


// �ķ�����: ISUP_SP_RedirectingNumber ////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator; // ��żָʾ��	
    
	DJ_U8 m_u8AddressPresentationRestrictedIndicator; // ��ʾ����ָʾ��
	DJ_U8 m_u8NumberingPlanIndicator; // ����ƻ�ָʾ��	
	
	DJ_S8 m_s8AddressSignal[40]; // ��ַ�ź�

	DJ_U8 m_u8Reserved[4];  // ����
} ISUP_spRedirectingNumber, *PISUP_spRedirectingNumber;


// �ķ���Ϣ: ISUP_SP_RedirectionInformation ///////////////////////////////////
typedef struct
{
	DJ_U8 m_u8RedirectingIndicator; // �ķ�ָʾ��
	DJ_U8 m_u8OriginalRedirectionReason; // ԭ��ĸķ�ԭ��
	DJ_U8 m_u8RedirectionCounter;   // �ķ�������
	DJ_U8 m_u8RedirectingReason;    // �ķ�ԭ��

	DJ_U8 m_u8Reserved[4]; // ����	
} ISUP_spRedirectionInformation, *PISUP_spRedirectionInformation;


// �ķ�����: ISUP_SP_RedirectionNumber ////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ����ָʾ��
	DJ_U8 m_u8OddEvenIndicator;         // ��żָʾ��
	DJ_U8 m_u8NumberingPlanIndicator;   // ����ƻ�ָʾ��
	DJ_U8 m_u8InternalNetworkNumberIndicator; // �ڲ��������ָʾ��
	
	DJ_S8 m_s8AddressSignal[40];            // ��ַ�ź�

	DJ_U8 m_u8Reserved[4]; // ����
} ISUP_spRedirectionNumber, *PISUP_spRedirectionNumber;


// �ķ��������Ʋ���: ISUP_SP_RedirectionRestricition //////////////////////////
typedef struct
{	
	DJ_U8 m_u8PresentationRestrictedIndicator; // ��ʾ����ָʾ��
	DJ_U8 m_u8Reserved[3];
} ISUP_spRedirectionRestricition, *PISUP_spRedirectionRestricition;


// Զ�˲���: ISUP_SP_RemoteOperation //////////////////////////////////////////
typedef struct
{	
	DJ_U8 m_u8ProtocolProfile;    // Э������    
	DJ_U8 m_u8ExtensionIndicator; // ����	
	
	DJ_U8 m_u8Component[240];
	
	DJ_U8 m_u8ComponentLen;
	DJ_U8 m_u8Reserved;          // ����
} ISUP_spRemoteOperation, *PISUP_spRemoteOperation;


// ҵ�񼤻�: ISUP_SP_ServiceActivation ////////////////////////////////////////
typedef struct
{
	DJ_U8 m_u32FeatureCode[32];  // ������
	
	DJ_U8 m_u8FCLen;             // �����볤��
	DJ_U8 m_u8Reserved[3];       // ����	
} ISUP_spServiceActivation, *PISUP_spServiceActivation;


// ��������: ISUP_SP_SignalPointCode ////////////////////////////////////////
typedef struct
{	
	DJ_U32 m_u32SPcode;     // ����	
	DJ_U8  m_u8Reserved[4]; // ����	
} ISUP_spSignalPointCode, *PISUP_spSignalPointCode;


// ��������: ISUP_SP_SubsequentNumber /////////////////////////////////////////
typedef struct
{		
	DJ_U8 m_u8OddEvenIndicator; // ��żָʾ��
	DJ_U8 m_u8Reserved[3];      // ����
	
	DJ_S8 m_s8AddressSignal[40];    // ��ַ�ź�
} ISUP_spSubsequentNumber, *PISUP_spSubsequentNumber;


// ��ͣ�ָ�ָʾ: ISUP_SP_SuspendResumeIndicator ///////////////////////////////
typedef struct
{	
	DJ_U8 m_u8SuspendResumeIndicator; // ��ͣ/�ָ�ָʾ��
	DJ_U8 m_u8Reserved[3]; // ����	
} ISUP_spSuspendResumeIndicator, *PISUP_spSuspendResumeIndicator;


// �������ѡ��: ISUP_SP_TransitNetworkSelection //////////////////////////////
typedef struct
{	
	DJ_U8 m_u8NetworkIdentificationPlan;   // ����ʶ��ƻ�
	DJ_U8 m_u8TypeOfNetworkIdentification; // ����ʶ������
	DJ_U8 m_u8OddEvenIndicator;            // ��żָʾ��
	DJ_U8 m_u8Reserved;                    // ����
	
	DJ_U8 m_u8NetworkIdentification[32];   // ����ʶ��
	
	DJ_U8 m_u8NetIdLen;                    // ����ʶ�𳤶�
	DJ_U8 m_u8Rev[3];                      // ����	
} ISUP_spTransitNetworkSelection, *PISUP_spTransitNetworkSelection;


// ����ý��Ҫ��: ISUP_SP_TransmissionMediumRequirement ////////////////////////
typedef struct
{	
	DJ_U8 m_u8TransmissionMediumRequirement; // ����ý��Ҫ��
	DJ_U8 m_u8Reserved[3]; // ����	
} ISUP_spTransmissionMediumRequirement, *PISUP_spTransmissionMediumRequirement;


// ����ý��Ҫ��(����): ISUP_SP_TransmissionMediumRequirementPrime /////////////
typedef struct
{	
	DJ_U8 m_u8TransmissionMediumRequirement; // ����ý��Ҫ��
	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spTransmissionMediumRequirementPrime, *PISUP_spTransmissionMediumRequirementPrime;


// ʹ���еĴ���ý��: ISUP_SP_TransmissionMediumUsed ///////////////////////////
typedef struct
{	
	DJ_U8 m_u8TransmissionMediumRequirement; // ����ý��Ҫ��
	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spTransmissionMediumUsed, *PISUP_spTransmissionMediumUsed;


// �û�ҵ����Ϣ: ISUP_SP_UserServiceInformation ///////////////////////////////
typedef struct
{
	DJ_U8 m_u8USInformation[32]; // �û�ҵ����Ϣ
	
	DJ_U8 m_u8USILen;            // ��Ϣ����
	DJ_U8 m_u8Reserved[3];       // ����	
} ISUP_spUserServiceInformation, *PISUP_spUserServiceInformation;


// �����û�ҵ����Ϣ: ISUP_SP_UserServiceInformationPrime //////////////////////
typedef struct
{
	DJ_U8 m_u8USInformation[32]; // �û�ҵ����Ϣ
	
	DJ_U8 m_u8USILen;            // ��Ϣ����
	DJ_U8 m_u8Reserved[3];       // ����	
} ISUP_spUserServiceInformationPrime, *PISUP_spUserServiceInformationPrime;


// �û�����ҵ����Ϣ: ISUP_SP_UserTeleserviceInformation ///////////////////////
typedef struct
{	
	DJ_U8 m_u8Presentation;   // ��ʾ
	DJ_U8 m_u8Interpretation; // ����
	DJ_U8 m_u8CodingStandard; // �����׼
	DJ_U8 m_u8ExtensionIndicator1; // ����
	
	DJ_U8 m_u8HighLayerCharacteristicsIdentification; // �߲�����ʶ��
	DJ_U8 m_u8ExtensionIndicator2; // ����

	DJ_U8 m_u8ExtendedHighLayerCharacteristicsIdentification; // ����ĸ߲�����ʶ��
	DJ_U8 m_u8ExtensionIndicator3;	// ����
} ISUP_spUserTeleserviceInformation, *PISUP_spUserTeleserviceInformation;


// �û����û�ָʾ��: ISUP_SP_UserToUserIndicator //////////////////////////////
typedef struct
{	
	DJ_U8 m_u8Type;     // ����
	DJ_U8 m_u8Service1; // ҵ��
	DJ_U8 m_u8Service2; // ҵ��
	DJ_U8 m_u8Service3; // ҵ��
	DJ_U8 m_u8NetworkDiscardIndicator; // ��������ָʾ��

	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spUserToUserIndicator, *PISUP_spUserToUserIndicator;


// �û����û���Ϣ: ISUP_SP_UserToUserInformation //////////////////////////////
typedef struct
{	
	DJ_U8 m_u8UserToUserInformation[240];
	
	DJ_U8 m_u8UifLen;      // ��Ϣ����
	DJ_U8 m_u8Reserved[3]; // ����
} ISUP_spUserToUserInformation, *PISUP_spUserToUserInformation;


#endif
