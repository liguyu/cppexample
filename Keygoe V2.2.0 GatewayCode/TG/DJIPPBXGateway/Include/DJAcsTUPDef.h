#ifndef __XMS_TUP_H__
#define __XMS_TUP_H__


// TUP������Ϣ����(������Ҫֱ�ӷ��Ͳ���) //////////////////////////////////////
enum XMS_TUP_Signalling_Message     // TUP������Ϣ
{	  
    TUP_SM_SAM = 0x31,              // ������ַ�ź�
    TUP_SM_SAO = 0x41,              // ����һ����ַ�ĺ�����ַ�ź�
    
    TUP_SM_GSM = 0x12,              // һ��ǰ������Ϣ�ź�
    TUP_SM_GRQ = 0x13,              // һ�������ź�
    
    TUP_SM_BLO = 0x27,              // �����ź�
    TUP_SM_UBL = 0x47,              // ��������ź�
    TUP_SM_RSC = 0x77,              // ��·��ԭ�ź�
    
    TUP_SM_MGB = 0x18,              // ����ά����Ⱥ�����ź�
    TUP_SM_MGU = 0x38,              // ����ά����Ⱥ��������ź�
    TUP_SM_HGB = 0x58,              // ����Ӳ�����ϵ�Ⱥ�����ź�
    TUP_SM_HGU = 0x78,              // ����Ӳ�����ϵ�Ⱥ��������ź�
    TUP_SM_GRS = 0x98,              // ��·Ⱥ��ԭ�ź�
    TUP_SM_SGB = 0xB8,              // ���������Ⱥ�����ź�
    TUP_SM_SGU = 0xD8,              // ���������Ⱥ����������ź�
};


// TUP����������� ////////////////////////////////////////////////////////////
enum XMS_TUP_Signalling_Parameter                   // TUP�������
{	
	TUP_SP_AccMessageIndicator        = 0x0C,       // ACC��Ϣָʾ��
	TUP_SP_AcmMessageIndicator        = 0x09,       // ACM��Ϣָʾ��
	TUP_SP_CalledPartAddress          = 0x0F,       // �����û�����
	TUP_SP_CallingLineAddress         = 0x02,       // �����û�����
	TUP_SP_CallingPartyCategory       = 0x01,       // �����û����
	TUP_SP_ConnectionNatureIndicator  = 0x0D,       // ��������ָʾ��
	TUP_SP_EumOctetIndicator          = 0x0A,       // EUM��Ϣ��λ��
	TUP_SP_EumSignallingPointCode     = 0x0B,       // EUM��Ϣ�����
	TUP_SP_ForwardCallInformation     = 0x0E,       // ǰ�������Ϣ
	TUP_SP_GrqRequestTypeIndicator    = 0x08,       // GRQ����ָʾ��
	TUP_SP_GsmResponseTypeIndicator   = 0x06,       // GSM��Ӧָʾ��
	TUP_SP_IaiFirstIndicator          = 0x05,       // IAI��һ��ʾ��
	TUP_SP_OriginalCalledAddress      = 0x03,       // ԭ�����û�����
	TUP_SP_RangeStatus                = 0x04,       // ��Χ��״̬
	TUP_SP_SubsequentAddress          = 0x10        // ������ַ
};


// TUP����������ݽṹ���� ////////////////////////////////////////////////////
// ACC��Ϣָʾ��: TUP_SP_AccMessageIndicator //////////////////////////////////
// ����: ACC
typedef struct
{
	DJ_U8 m_u8ACCInformation; // ACC��Ϣ
	DJ_U8 m_u8Reserved[3]; // ����
}TUP_spAccMessageIndicator, *PTUP_spAccMessageIndicator;


// ACM��Ϣָʾ��: TUP_SP_AcmMessageIndicator //////////////////////////////////
// ����: ACM
typedef struct
{
	DJ_U8 m_u8TypeOfACMIndicator; // ��ַȫ�ź����ͱ�ʾ��
	DJ_U8 m_u8SubscriberFreeIndicator; // �û��б�ʾ��
	DJ_U8 m_u8IncomingEchoSuppressorIndicator; // ����������������ʾ��
	DJ_U8 m_u8CallForwardingIndicator; // ����ת�Ʊ�ʾ��
	DJ_U8 m_u8SignallingPathIndicator; // �ź�ͨ����ʾ��
	DJ_U8 m_u8Reserved[3]; // ����
}TUP_spAcmMessageIndicator, *PTUP_spAcmMessageIndicator;


// �����û�����: TUP_SP_CalledPartAddress /////////////////////////////////////
// ����: IAM/IAI
typedef struct
{
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ���ʱ�ʾ��
	DJ_U8 m_u8NumberOfAddressSignal; // ��ַ�źŵ�����
	DJ_U8 m_u8Reserved[2]; // ����
	
	DJ_S8 m_s8AddressSignal[32]; // ��ַ�ź�
}TUP_spCalledPartAddress, *PTUP_spCalledPartAddress;


// �����û�����: TUP_SP_CallingLineAddress ////////////////////////////////////
// ����: IAI/GSM
typedef struct
{
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ���ʱ�ʾ��
	DJ_U8 m_u8PresentationIndicator; // �ṩ�����û��߱�ʶ��ʾ��
	DJ_U8 m_u8IncompleteIndicator; // �����û��߱�ʶ��ȫ��ʾ��
	DJ_U8 m_u8NumberOfAddressSignal; // ���е�ַ�źŵ�����
	
	DJ_S8 m_s8AddressSignal[32]; // ��ַ�ź�
}TUP_spCallingLineAddress, *PTUP_spCallingLineAddress;


// �����û����: TUP_SP_CallingPartyCategory //////////////////////////////////
// ����: IAM/IAI/GSM
typedef struct
{
	DJ_U8 m_u8CallingPartyCategory;  // �����û����
	DJ_U8 m_u8Reserved[3]; // ����
}TUP_spCallingPartyCategory, *PTUP_spCallingPartyCategory;


// ��������ָʾ��: TUP_SP_ConnectionNatureIndicator ///////////////////////////
// ����: IAM/IAI
typedef struct
{	
	DJ_U8 m_u8NatureOfCircuitIndicator; // ��·���ʱ�ʾ��
	DJ_U8 m_u8ContinuityCheckIndicator; // ��ͨ�����ʾ��
	DJ_U8 m_u8EchoSuppressorIndicator;  // ȥ��������������ʾ��
	DJ_U8 m_u8Reserved;
}TUP_spConnectionNatureIndicator, *PTUP_spConnectionNatureIndicator;


// EUM��Ϣ��λ��: TUP_SP_EumOctetIndicator ////////////////////////////////////
// ����: EUM
typedef struct
{
	DJ_U8 m_u8UnsuccessfulIndicator; // ���ɹ���ʾ��
	DJ_U8 m_u8Reserved[3]; // ����
}TUP_spEumOctetIndicator, *PTUP_spEumOctetIndicator;


// EUM��Ϣ�����: TUP_SP_EumSignallingPointCode ///////////////////////////////
// ����: EUM
typedef struct
{
	DJ_U32 m_u32SPC; // ��������
}TUP_spEumSignallingPointCode, *PTUP_spEumSignallingPointCode;


// ǰ�������Ϣ: TUP_SP_ForwardCallInformation ////////////////////////////////
// ����: IAM/IAI
typedef struct
{
	DJ_U8 m_u8InternationalCallIndicator; // �����������б�ʾ��
	
	DJ_U8 m_u8RedirectedCallIndicator; // �ķ����б�ʾ��
	DJ_U8 m_u8AllDigitalPathRequiredIndicator; // ��Ҫȫ��������ͨ·��ʾ��
	DJ_U8 m_u8SignallingPathIndicator; // �ź�ͨ����ʾ��
}TUP_spForwardCallInformation, *PTUP_spForwardCallInformation;


// GRQ����ָʾ��: TUP_SP_GrqRequestTypeIndicator //////////////////////////////
// ����: GRQ
typedef struct
{
	DJ_U8 m_u8CallingPartyCategoryRequestIndicator; // �����û���������ʾ��
	DJ_U8 m_u8CallingLineIdentityRequestIndicator; // �����û��߱�ʶ�����ʾ��
	DJ_U8 m_u8OriginalCalledAddressRequestIndicator; // ԭ���е�ַ�����ʾ��
	DJ_U8 m_u8MaliciousCallIdentificationIndicator; // �������ʶ�������ʾ��
	DJ_U8 m_u8HoldRequestIndicator; // ���������ʾ��
	DJ_U8 m_u8EchoSuppressorRequestIndicator; // ���������������ʾ��
	DJ_U8 m_u8Reserved[2]; // ����
}TUP_spGrqRequestTypeIndicator, *PTUP_spGrqRequestTypeIndicator;


// GSM��Ӧָʾ��: TUP_SP_GsmResponseTypeIndicator /////////////////////////////
// ����: GSM
typedef struct
{
	DJ_U8 m_u8CallingPartyCategoryIndicator; // �����û�����ʾ��
	DJ_U8 m_u8CallingLineIdentityIndicator; // �����û��߱�ʶ��ʾ��
	DJ_U8 m_u8IdentityIndicator; // �����м̺�ת�ӽ����ֱ�ʶ
	DJ_U8 m_u8OriginalCalledAddressIndicator; // ԭ���е�ַ��ʾ��
	DJ_U8 m_u8OutgoingEchoSuppressorIndicator; // ȥ��������������ʾ��
	DJ_U8 m_u8MaliciousCallIdentificationIndicator; // �������ʶ���ʾ��
	DJ_U8 m_u8HoldIndicator; // ���ֱ�ʾ��
	DJ_U8 m_u8Reserved; // ����
}TUP_spGsmResponseTypeIndicator, *PTUP_spGsmResponseTypeIndicator;


// IAI��һ��ʾ��: TUP_SP_IaiFirstIndicator ////////////////////////////////////
// ����: IAI
typedef struct
{
	DJ_U8 m_u8FacilityInformationIndicator; // �����������û�������Ϣ��ʾ��
	DJ_U8 m_u8CUGInformationIndicator; // �պ��û�Ⱥ��Ϣ��ʾ��
	DJ_U8 m_u8AdditionalCallingPartyIndicator; // ���������û���Ϣ��ʾ��
	DJ_U8 m_u8AdditionalRoutingIndicator; // ����·����Ϣ��ʾ��
	DJ_U8 m_u8CallingLineIdentityIndicator; // �����û��߱�ʶ��ʾ��
	DJ_U8 m_u8OriginalCalledAddressIndicator; // ԭ���е�ַ��ʾ��
	DJ_U8 m_u8ChargingInformationIndicator; // �Ʒ���Ϣ��ʾ��
	DJ_U8 m_u8Reserved; // ����
}TUP_spIaiFirstIndicator, *PTUP_spIaiFirstIndicator;


// ԭ�����û�����: TUP_SP_OriginalCalledAddress ///////////////////////////////
// ����: IAI/GSM
typedef struct
{
	DJ_U8 m_u8NatureOfAddressIndicator; // ��ַ���ʱ�ʾ��
	DJ_U8 m_u8NumberOfAddressSignal; // ��ַ�źŵ�����
	DJ_U8 m_u8Reserved[2]; // ����
	
	DJ_S8 m_s8AddressSignal[32]; // ��ַ�ź�
}TUP_spOriginalCalledAddress, *PTUP_spOriginalCalledAddress;


// ��Χ��״̬: TUP_SP_RangeStatus /////////////////////////////////////////////
// ����: GRM��Ϣ��
typedef struct
{
	DJ_U8  m_u8Range; // ��Χ
	DJ_U8  m_u8Reserved[3]; // ����
	
	DJ_U32 m_u32Status;    // ״̬
}TUP_spRangeStatus, *PTUP_spRangeStatus;


// ������ַ: TUP_SP_SubsequentAddress /////////////////////////////////////////
// ����: SAM/SAO
typedef struct
{
	DJ_S8 m_s8AddressSignal[32]; // ��ַ�ź�
}TUP_spSubsequentAddress, *PTUP_spSubsequentAddress;


#endif
