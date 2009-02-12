#ifndef __DJ_ACS_DEV_STATE__
#define __DJ_ACS_DEV_STATE__


enum DEVICE_CALL_STATE
{
DEVICE_COMMON_STATE_START   = 0x0000, //
DCS_FREE                    = 0x0001, //
DCS_LOCAL_BLOCK             = 0x0002, //
DCS_REMOTE_BLOCK            = 0x0003, //
DCS_BLOCK                   = 0x0004, //
DCS_UNAVAILABLE             = 0x0005, //

DCS_CALLOUT                 = 0x0006, //
DCS_CALLIN                  = 0x0007, //
DCS_ALERTING                = 0x0008, //

DCS_CONNECT                 = 0x0009, //

DCS_DISCONNECT              = 0x000A, //
DCS_WAIT_FREE               = 0x000B, //

DEVICE_ESPECIAL_STATE_START = 0x0100,
DES_ISUP_STATE_START        = (XMS_DEVSUB_E1_SS7_ISUP << 8), //
DES_ISUP_LOCAL_SUSPEND      = (DES_ISUP_STATE_START | 0x01), //
DES_ISUP_REMOTE_SUSPEND     = (DES_ISUP_STATE_START | 0x02), //
DES_ISUP_LOCAL_RESUME       = (DES_ISUP_STATE_START | 0x03), //
DES_ISUP_REMOTE_RESUME      = (DES_ISUP_STATE_START | 0x04), //
DES_ISUP_RECV_INR           = (DES_ISUP_STATE_START | 0x05), //
DES_ISUP_RECV_INF           = (DES_ISUP_STATE_START | 0x06), //
DES_ISUP_RECV_CQR           = (DES_ISUP_STATE_START | 0x07), //
DES_ISUP_RECV_USR           = (DES_ISUP_STATE_START | 0x08), //
DES_ISUP_RECV_CPG           = (DES_ISUP_STATE_START | 0x09), //

DES_TUP_STATE_START         = (XMS_DEVSUB_E1_SS7_TUP << 8),  // 
DES_TUP_RECV_CCL            = (DES_TUP_STATE_START | 0x01),  // 
DES_TUP_RECV_RAN            = (DES_TUP_STATE_START | 0x02),  // 
DES_TUP_RECV_GSM            = (DES_TUP_STATE_START | 0x03),  // 

DES_DSS1_STATE_START        = (XMS_DEVSUB_E1_DSS1 << 8),      // 
DES_DSS1_RESTRAT            = (DES_DSS1_STATE_START | 0x01),  // 
DES_DSS1_WAIT_CONNECT_ACK   = (DES_DSS1_STATE_START | 0x02),  // 
DES_DSS1_CALLER_RECV_ALERT  = (DES_DSS1_STATE_START | 0x03),  //
DES_DSS1_CALLER_RECV_CALLPROCESS = (DES_DSS1_STATE_START | 0x04),  //
DES_DSS1_CALLER_RECV_SETUP_ACK	 = (DES_DSS1_STATE_START | 0x05),  //
DES_DSS1_CALLER_WAIT_SETUP_ACK	 = (DES_DSS1_STATE_START | 0x06),  //

DES_H323_STATE_START        = 0x00010100,
DES_H323_PROCEEDING         = (DES_H323_STATE_START + 0x01), // proceeding
DES_H323_TRANSFERING        = (DES_H323_STATE_START + 0x02), // transfering
DES_H323_ACF                = (DES_H323_STATE_START + 0x03), // admission confirm (reserved)
DES_H323_ARJ                = (DES_H323_STATE_START + 0x04), // admission reject (reserved)
DES_H323_INCOMPADDR         = (DES_H323_STATE_START + 0x05), // incomplete address (reserved)
DES_H323_WAITACK            = (DES_H323_STATE_START + 0x06), // wait for address ack (reserved)

DES_SIP_STATE_START         = 0x00010200,
DES_SIP_REDIRECTED          = (DES_SIP_STATE_START + 0x01),  // call redirected
DES_SIP_UNAUTH              = (DES_SIP_STATE_START + 0x02),  // unauthenticated
DES_SIP_ACCEPTED            = (DES_SIP_STATE_START + 0x03),  // accepted (200 ok received)
DES_SIP_REMOTEACCEPT        = (DES_SIP_STATE_START + 0x04),  // remote party accepted (reserved)
DES_SIP_DISCONNECTING       = (DES_SIP_STATE_START + 0x05),  // BYE sent
DES_SIP_TERMINATED          = (DES_SIP_STATE_START + 0x06),  // call finished
DES_SIP_CANCELLED           = (DES_SIP_STATE_START + 0x07),  // call cancelled
DES_SIP_CANCELLING          = (DES_SIP_STATE_START + 0x08),  // CANCEL sent
DES_SIP_TIMEOUT             = (DES_SIP_STATE_START + 0x09),  // timeout after 1xx response received (reserved)
DES_SIP_MSGSENDFAIL         = (DES_SIP_STATE_START + 0x0A),  // message send failed

DES_ANALOG_TRUNK_STATE_START     = (XMS_DEVSUB_ANALOG_TRUNK << 8),         // 
DES_ANALOG_TRUNK_OFFHOOK         = (DES_ANALOG_TRUNK_STATE_START | 0x01),  // 
DES_ANALOG_TRUNK_CALLOUT_FAILURE = (DES_ANALOG_TRUNK_STATE_START | 0x02),  // 

DES_ANALOG_USER_STATE_START      = (XMS_DEVSUB_ANALOG_USER << 8),         // 
DES_ANALOG_USER_RING             = (DES_ANALOG_USER_STATE_START | 0x01),  // 
DES_ANALOG_USER_OFFHOOK          = (DES_ANALOG_USER_STATE_START | 0x02),  // 

DES_FAX_START                      = (XMS_DEVMAIN_FAX<<16),  // 
DES_FAX_OPEN                       = (DES_FAX_START ) ,      //  
DES_FAX_CLOSE                      = (DES_FAX_START | 0x01), //  
DES_FAX_RESET                      = (DES_FAX_START | 0x02), //

DES_FAX_SENDFAX_RECVCMD            = (DES_FAX_START | 0x10), //
DES_FAX_SENDFAX_START              = (DES_FAX_START | 0x11), //
DES_FAX_SENDFAX_SETLOCAL           = (DES_FAX_START | 0x12), //
DES_FAX_SENDFAX_RESOLVE_FISNIESH   = (DES_FAX_START | 0x13), //
DES_FAX_SENDFAX_LOCAL_FINISH       = (DES_FAX_START | 0x14), //
DES_FAX_SENDFAX_SHAKEHAND_END      = (DES_FAX_START | 0x15), //
DES_FAX_SENDFAX_SENDDATA           = (DES_FAX_START | 0x16), //
DES_FAX_SENDFAX_SENDFINISH         = (DES_FAX_START | 0x17), //
DES_FAX_SENDFAX_USERSTOP           = (DES_FAX_START | 0x18), //
DES_FAX_SENDFAX_TIFFFILE_NOTEXIST  = (DES_FAX_START | 0x19), //
DES_FAX_SENDFAX_SENDEND            = (DES_FAX_START | 0x20), //
DES_FAX_SENDFAX_SENDSTOP           = (DES_FAX_START | 0x21), //

DES_FAX_RECVFAX_RECVCMD            = (DES_FAX_START | 0x30), //
DES_FAX_RECVFAX_WAIT_HANDSHAKE     = (DES_FAX_START | 0x31), //
DES_FAX_RECVFAX_HANDSHAKE_OK       = (DES_FAX_START | 0x32), //
DES_FAX_RECVFAX_RECVDATA_START     = (DES_FAX_START | 0x33), //
DES_FAX_RECVFAX_BACKPAGESTATE_SUCC = (DES_FAX_START | 0x34), //
DES_FAX_RECVFAX_RE_HANDSHAKE       = (DES_FAX_START | 0x35), //
DES_FAX_RECVFAX_RECVDATA_END       = (DES_FAX_START | 0x36), //
DES_FAX_RECVFAX_USERSTOP           = (DES_FAX_START | 0x37), //
DES_FAX_RECVFAX_COMPOSE_START      = (DES_FAX_START | 0x38), //
DES_FAX_RECVFAX_COMPOSE_END        = (DES_FAX_START | 0x39), //
DES_FAX_RECVFAX_BACKPAGESTATE_FAIL = (DES_FAX_START | 0x40), //
DES_FAX_RECVFAX_RECVEND            = (DES_FAX_START | 0x41), //
};

enum XMS_DEVICE_ATRUNK_CALLOUT_REASON
{
	XMS_ATRUNK_CALLOUT_SUCC                = 1, //
	XMS_ATRUNK_CALLOUT_WAIDAILTONE_TIMEOUT = 2, //
	XMS_ATRUNK_CALLOUT_RING_TIMEOUT        = 3, //
	XMS_ATRUNK_CALLOUT_BUSY                = 4, //
};

enum XMS_DEVICE_CLEAR_CALL_CAUSE                         //
{
	XMS_CC_ReasonUnspecified                        = 0,   //
	XMS_CC_UnallocatedNumber                        = 1,   //
	XMS_CC_NoRouteToSpecifiedTransitNetwork         = 2,   //
	XMS_CC_NoRouteToDestination                     = 3,   //
	XMS_CC_SendSpecialInformationTone               = 4,   //
	XMS_CC_MisdialledTrunkPrefix                    = 5,   //
	XMS_CC_Preemption                               = 8,   //
	XMS_CC_PreemptionCircuitReservedForReuse        = 9,   //
	XMS_CC_NormalCallClearing                       = 16,  //
	XMS_CC_UserBusy                                 = 17,  //
	XMS_CC_NoUserResponding                         = 18,  //
	XMS_CC_UserAlertedNoAnswer                      = 19,  //
	XMS_CC_SubscriberAbsent                         = 20,  //
	XMS_CC_CallRejected                             = 21,  //
	XMS_CC_NumberChanged                            = 22,  //
	XMS_CC_DestinationOutOfOrder                    = 27,  //
	XMS_CC_InvalidNumberFormat                      = 28,  //
	XMS_CC_FacilityRejected                         = 29,  //
	XMS_CC_NormalUnspecified                        = 31,  //
	XMS_CC_NoCircuitChannelAvailable                = 34,  //
	XMS_CC_NetworkOutOfOrder                        = 38,  //
	XMS_CC_TemporaryFailure                         = 41,  //
	XMS_CC_SwitchingEquipmentCongestion             = 42,  //
	XMS_CC_AccessInformationDiscarded               = 43,  //
	XMS_CC_RequestedCircuitChannelNotAvailable      = 44,  //
	XMS_CC_PrecedenceCallBlocked                    = 46,  //
	XMS_CC_ResourceUnavailableUnspecified           = 47,  //
	XMS_CC_RequestedFacilityNotSubscribed           = 50,  //
	XMS_CC_OutgoingCallsBarredWithinCUG             = 53,  //
	XMS_CC_IncomingCallsBarredWithinCUG             = 55,  //
	XMS_CC_BearerCapabilityNotAuthorized            = 57,  //
	XMS_CC_BearerCapabilityNotPresentlyAvailable    = 58,  //
	XMS_CC_ServiceOrOptionNotAvailableUnspecified   = 63,  //
	XMS_CC_BearerCapabilityNotImplemented           = 65,  //
	XMS_CC_RequestedFacilityNotImplemented          = 69,  //
	XMS_CC_RestrictedDigitalInfoBearerIsAvailable   = 70,  //
	XMS_CC_ServiceOrOptionNotImplementedUnspecified = 79,  //
	XMS_CC_UserNotMemberOfCUG                       = 87,  //
	XMS_CC_IncompatibleDestination                  = 88,  //
	XMS_CC_NonExistentCUG                           = 90,  //
	XMS_CC_InvalidTransitNetworkSelection           = 91,  //
	XMS_CC_InvalidMessageUnspecified                = 95,  //
	XMS_CC_MessageTypeNonExistenOrNotImplemented    = 97,  //
	XMS_CC_InfElementNonExistentOrNotImplemented    = 99,  //
	XMS_CC_RecoveryOnTimerExpiry                    = 102, //
	XMS_CC_ParameterNonExistentOrNotImplement       = 103, //
	XMS_CC_MessageWithUnrecognizedParameter         = 110, //
	XMS_CC_ProtocolErrorUnspecified                 = 111, //
	XMS_CC_InterworkingUnspecified                  = 127, //
	
	//
	XMS_CC_DualSeizure                              = 129, //
	XMS_CC_RemoteBlock                              = 130, //
	XMS_CC_RemoteUnblock                            = 131, //
	XMS_CC_ResetCircuit                             = 132, //
	XMS_CC_LocalBusy                                = 133, //
};

//////////////////////////////////////////////////////////////////////////
enum XMS_VOIP_ERRCODE
{
	XMS_VOIP_ERR_OK							= 0x00,	// command succeed with no error
	XMS_VOIP_ERR_INTERNAL					= 0x01,	// internal error
	XMS_VOIP_ERR_SERVICE_DISABLED			= 0x02,	// service disabled
	XMS_VOIP_ERR_INVALID_CALLID				= 0x03,	// call id invalid
	XMS_VOIP_ERR_UNKNOWN_PROTOCOL			= 0x04,	// call protocol unknown
	XMS_VOIP_ERR_CALL_ACTIVE				= 0x05,	// call is active
	XMS_VOIP_ERR_CALL_FREE					= 0x06,	// call is free
	XMS_VOIP_ERR_SLOT_NOACCESS				= 0x07	// call slot no access
};

enum XMS_VOIP_DISCONNECT_REASON
{
	XMS_VOIP_DISCONNECT_UNKNOWN				= 0x00,	// unknown reason
	XMS_VOIP_DISCONNECT_LOCAL				= 0x01,	// local disconnect
	XMS_VOIP_DISCONNECT_REMOTE				= 0x02,	// remote disconnect

	// H.323
	XMS_VOIP_DISCONNECT_H323_BUSY			= 0x03,	// busy
	XMS_VOIP_DISCONNECT_H323_REJECT			= 0x04,	// remote reject
	XMS_VOIP_DISCONNECT_H323_UNREACHABLE	= 0x05,	// not reachable

	// SIP
	XMS_VOIP_DISCONNECT_SIP_LOCAL_REJECT	= 0x06,	// local reject
	XMS_VOIP_DISCONNECT_SIP_REQUEST_FAILURE	= 0x07,	// 4xx response rcvd (not 401 or 407)
	XMS_VOIP_DISCONNECT_SIP_SERVER_FAILURE	= 0x08,	// 5xx response rcvd
	XMS_VOIP_DISCONNECT_SIP_GLOBAL_FAILURE	= 0x09,	// 6xx response rcvd
	XMS_VOIP_DISCONNECT_SIP_LOCAL_FAILURE	= 0x0A,	// local network failure
	XMS_VOIP_DISCONNECT_SIP_TIMEOUT			= 0x0B	// time out
};


#endif
