; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDialogAnalogUser
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XMSApi_Test.h"

ClassCount=24
Class1=CXMSApi_TestApp
Class2=CXMSApi_TestDlg
Class3=CAboutDlg

ResourceCount=23
Resource1=IDD_DIALOG_PARAM_ATRUNKFLASH
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_PARAM_Flash
Class4=CXMSApi_PlaySet
Resource4=IDD_DIALOG_ANAUSER
Class5=CXMSApi_ControlPlay
Resource5=IDD_ABOUTBOX
Class6=CXMSApi_RecordSet
Resource6=IDD_DIALOG_PLAYCSP
Class7=CXMSApi_ControlRecord
Resource7=IDD_DIALOG_ANATRUNK
Class8=CXMSApi_ConfParam
Resource8=IDD_DIALOG_GTDSET
Class9=CXMSApi_Test_ParamVoice
Resource9=IDD_DIALOG_PARAM_3GFaxTone
Class10=CXMSApi_BuildIndex
Resource10=IDD_DIALOG_RECORD
Class11=CXMSApi_SendIOData
Resource11=IDD_XMSAPI_TEST_DIALOG
Class12=CXMSApi_ParamCAS
Resource12=IDD_DIALOG_CONTROLRECORD
Class13=CDialogAnaTrunk
Resource13=IDD_DIALOG_PARAM_VOIP
Class14=CDialogAnalogUser
Resource14=IDD_DIALOG1
Class15=CRecCSPDlg
Resource15=IDD_DIALOG_BuildPlayIndex
Class16=CCSPPlayDlg
Resource16=IDD_DIALOG_CONF
Class17=CXMS_Test_ParamVoIP
Resource17=IDD_DIALOG_CONTROLPLAY
Class18=CDialogFskSet
Resource18=IDD_DIALOG_PARAM_VOICE
Class19=CDialogFlashSet
Resource19=IDD_DIALOG_RECCSP
Class20=CDialogGTD
Resource20=IDD_DIALOG_PARAM_CAS
Class21=CDialogGTDSet
Resource21=IDD_DIALOG_PLAY
Class22=DialogAtrunkFlash
Resource22=IDD_DIALOG_PARAM_FSK
Class23=CDialog3GFaxTone
Class24=CDialogFaxHeader
Resource23=IDD_DIALOG_PARAM_FAXHEADER

[CLS:CXMSApi_TestApp]
Type=0
HeaderFile=XMSApi_Test.h
ImplementationFile=XMSApi_Test.cpp
Filter=N

[CLS:CXMSApi_TestDlg]
Type=0
HeaderFile=XMSApi_TestDlg.h
ImplementationFile=XMSApi_TestDlg.cpp
Filter=D
LastObject=IDC_LIST_DEVICE_INFO
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=XMSApi_TestDlg.h
ImplementationFile=XMSApi_TestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_XMSAPI_TEST_DIALOG]
Type=1
Class=CXMSApi_TestDlg
ControlCount=104
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_LIST_DEVICE_INFO,listbox,1352728833
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_BUTTON_OpenDevice,button,1208025088
Control7=IDC_EDIT_NOW_OPEN_DEVICE,edit,1216413824
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,button,1342177287
Control10=IDC_LIST_MSG,listbox,1353777409
Control11=IDC_EDIT_SERVER_IPADDR,edit,1350631552
Control12=IDC_EDIT_SERVER_IPPORT,edit,1350631552
Control13=IDC_BUTTON_Exchange,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_DEVICE1,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_BUTTON_CloseDevice,button,1342242816
Control18=IDC_BUTTON_ResetDevice,button,1342242816
Control19=IDC_BUTTON_GetDeviceList,button,1342242816
Control20=IDC_BUTTON_GetDeviceState,button,1342242816
Control21=IDC_BUTTON_SetDeviceGroup,button,1476460544
Control22=IDC_BUTTON_MakeCallOut,button,1342242816
Control23=IDC_STATIC,static,1342308352
Control24=IDC_EDIT_CALLINGNUM,edit,1350631552
Control25=IDC_STATIC,static,1342308352
Control26=IDC_EDIT_CALLEDNUM,edit,1350631552
Control27=IDC_BUTTON_SetParam,button,1342242816
Control28=IDC_BUTTON_GetParam,button,1476460544
Control29=IDC_BUTTON_AlertCall,button,1342242816
Control30=IDC_BUTTON_AnswerCallIn,button,1342242816
Control31=IDC_BUTTON_LinkDevice,button,1342242816
Control32=IDC_BUTTON_UnlinkDevice,button,1342242816
Control33=IDC_BUTTON_ClearCall,button,1342242816
Control34=IDC_BUTTON_JoinConf,button,1342242816
Control35=IDC_BUTTON_LeaveFromConf,button,1342242816
Control36=IDC_BUTTON_ClearConf,button,1342242816
Control37=IDC_BUTTON_Play,button,1342242816
Control38=IDC_BUTTON_My_StopPlay,button,1342242816
Control39=IDC_BUTTON_Record,button,1342242816
Control40=IDC_BUTTON_My_StopRecord,button,1342242816
Control41=IDC_BUTTON_SendFax,button,1342242816
Control42=IDC_BUTTON_StopSendFax,button,1342242816
Control43=IDC_BUTTON_SendIOData,button,1342242816
Control44=IDC_BUTTON_ControlPlay,button,1342242816
Control45=IDC_BUTTON_ControlRecord,button,1342242816
Control46=IDC_COMBO_DEVICE,combobox,1344339970
Control47=IDC_STATIC,static,1342308352
Control48=IDC_LIST_MSG_HIDDEN,listbox,1084293377
Control49=IDC_STATIC,static,1342308352
Control50=IDC_COMBO_MSG,combobox,1478557698
Control51=IDC_EDIT_SENDFAX,edit,1350631552
Control52=IDC_EDIT_RECEIVEFAX,edit,1350631552
Control53=IDC_BUTTON_SelectSendFaxFile,button,1342242816
Control54=IDC_BUTTON_SelectReceiveFaxFile,button,1342242816
Control55=IDC_EDIT_PLAY,edit,1350631552
Control56=IDC_BUTTON_SelectPlayFile,button,1342242816
Control57=IDC_EDIT1_RECORD,edit,1350631552
Control58=IDC_BUTTON_SelectRecordFile,button,1342242816
Control59=IDC_LIST_DEVICE_OPENED,listbox,1352728833
Control60=IDC_EDIT_DEVICE2,edit,1350631552
Control61=IDC_CHECK_AutoScroll,button,1342242819
Control62=IDC_EDIT_MsgDetail,edit,1350633600
Control63=IDC_BUTTON_PlayDetail,button,1342242816
Control64=IDC_BUTTON_RecordDetail,button,1342242816
Control65=IDC_COMBO_ParamType,combobox,1344340226
Control66=IDC_BUTTON_InitPlayIndex,button,1342242816
Control67=IDC_BUTTON_BuildPlayIndex,button,1342242816
Control68=IDC_COMBO_NowIndexList,combobox,1344339970
Control69=IDC_CHECK_DEBUG,button,1342242819
Control70=IDC_EDIT_DSPIP,edit,1350631552
Control71=IDC_EDIT_DSPPORT,edit,1350631552
Control72=IDC_BUTTON_CONN,button,1342242816
Control73=IDC_STATIC,static,1342308352
Control74=IDC_STATIC,static,1342308352
Control75=IDC_BUTTON_RECCSP,button,1342242816
Control76=IDC_BUTTON_CTRLRECCSP,button,1342242816
Control77=IDC_EDIT_RECCSP_FILE,edit,1350631552
Control78=IDC_BUTTON_SelectRecordFile2,button,1342242816
Control79=IDC_BUTTON_CTXLINK,button,1073807360
Control80=IDC_BUTTON_CTXUNLINK,button,1073807360
Control81=IDC_EDIT_DEVICE3,edit,1082196096
Control82=IDC_STATIC,static,1073872896
Control83=IDC_BUTTON_DISCONN,button,1342242816
Control84=IDC_BUTTON_SendFaxT38,button,1342242816
Control85=IDC_BUTTON_StopSendFaxT38,button,1073807360
Control86=IDC_BUTTON_ReceiveFax,button,1342242816
Control87=IDC_BUTTON_StopReceiveFax,button,1342242816
Control88=IDC_BUTTON_ReceiveFaxT38,button,1073807360
Control89=IDC_BUTTON_StopReceiveFaxT38,button,1073807360
Control90=IDC_BUTTON_PLAYCSP,button,1342242816
Control91=IDC_BUTTON_CTRLPLAYCSP,button,1342242816
Control92=IDC_BUTTON_GetBoardInfo,button,1342242816
Control93=IDC_COMBO_FAXTYPE,combobox,1344339970
Control94=IDC_EDIT_CALLEE_ADDR,edit,1350631552
Control95=IDC_EDIT_CALLEE_PORT,edit,1350631552
Control96=IDC_STATIC,static,1342308352
Control97=IDC_RADIO_H323,button,1342308361
Control98=IDC_RADIO_SIP,button,1342177289
Control99=IDC_STATIC,button,1342177287
Control100=IDC_RADIO_OTHER,button,1073872905
Control101=IDC_RADIO_VOIP,button,1073741833
Control102=IDC_STATIC,button,1342177287
Control103=IDC_RADIO_SEND,button,1342308361
Control104=IDC_RADIO_RECV,button,1342177289

[DLG:IDD_DIALOG_PLAY]
Type=1
Class=CXMSApi_PlaySet
ControlCount=40
Control1=IDC_RADIO_PLAY_TYPE_FILE,button,1342308361
Control2=IDC_RADIO_PLAY_TYPE_INDEX,button,1342177289
Control3=IDC_RADIO_PLAY_TYPE_FILE_QUEUE,button,1342177289
Control4=IDC_RADIO_PLAY_TYPE_INDEX_QUEUE,button,1342177289
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_Play_u32TaskID,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_Play_u32MaxSize,edit,1350631552
Control22=IDC_EDIT_Play_u32MaxTime,edit,1350631552
Control23=IDC_EDIT_Play_u32StopCode,edit,1350631552
Control24=IDC_RADIO_SRC_8K,button,1342308361
Control25=IDC_RADIO_SRC_6K,button,1342177289
Control26=IDC_RADIO_SRC_GTG,button,1342177289
Control27=IDC_RADIO_SRC_FSK,button,1342177289
Control28=IDC_RADIO_SRC_FAX,button,1342177289
Control29=IDC_RADIO_Alaw_type,button,1342308361
Control30=IDC_RADIO_Ulaw_type,button,1342177289
Control31=IDC_RADIO_Vox_type,button,1342177289
Control32=IDC_RADIO_Linear_8bit,button,1342177289
Control33=IDC_RADIO_Normal_Stop,button,1342308361
Control34=IDC_RADIO_Single_Code,button,1342177289
Control35=IDC_RADIO_Any_Code,button,1342177289
Control36=IDC_EDIT_Play_u16FileOffset,edit,1350631552
Control37=IDC_EDIT_Play_u16PlayIndex,edit,1350631552
Control38=IDC_EDIT_Play_s8PlayContent,edit,1484849280
Control39=IDC_EDIT_Play_DevcieID,edit,1484849280
Control40=IDC_RADIO_Play_Linear_16bit,button,1342177289

[CLS:CXMSApi_PlaySet]
Type=0
HeaderFile=XMSApi_PlaySet.h
ImplementationFile=XMSApi_PlaySet.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_Play_u32MaxTime
VirtualFilter=dWC

[DLG:IDD_DIALOG_CONTROLPLAY]
Type=1
Class=CXMSApi_ControlPlay
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_u16StepSize,edit,1350631552
Control7=IDC_EDIT2,edit,1484849280
Control8=IDC_EDIT3,edit,1484849280
Control9=IDC_BUTTON_Stop,button,1342242816
Control10=IDC_BUTTON_Pause,button,1342242816
Control11=IDC_BUTTON_Resume,button,1342242816
Control12=IDC_BUTTON_StepBack,button,1342242816
Control13=IDC_BUTTON_Forward,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_ControlPlay_DeviceID,edit,1484849280

[CLS:CXMSApi_ControlPlay]
Type=0
HeaderFile=XMSApi_ControlPlay.h
ImplementationFile=XMSApi_ControlPlay.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CXMSApi_ControlPlay

[DLG:IDD_DIALOG_RECORD]
Type=1
Class=CXMSApi_RecordSet
ControlCount=49
Control1=IDC_EDIT_Record_u32MaxSize,edit,1350631552
Control2=IDC_EDIT_Record_u32MaxTime,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_RADIO_Alaw_type,button,1342308361
Control5=IDC_RADIO_Ulaw_type,button,1342177289
Control6=IDC_RADIO_Vox_type,button,1342177289
Control7=IDC_RADIO_Linear_8bit,button,1342177289
Control8=IDC_RADIO_Linear_16bit,button,1342177289
Control9=IDC_STATIC,static,1342308352
Control10=IDC_RADIO_SRC_8K,button,1342308361
Control11=IDC_RADIO_SRC_6K,button,1342177289
Control12=IDC_RADIO_SRC_GTG,button,1342177289
Control13=IDC_RADIO_SRC_FSK,button,1342177289
Control14=IDC_RADIO_SRC_FAX,button,1342177289
Control15=IDC_STATIC,static,1342308352
Control16=IDC_RADIO_Normal_Stop,button,1342308361
Control17=IDC_RADIO_Single_Code,button,1342177289
Control18=IDC_RADIO_Any_Code,button,1342177289
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT_Record_u8StopCode,edit,1350631552
Control21=IDC_STATIC,static,1342308352
Control22=IDC_CHECK_s8MixEnable,button,1342242819
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,static,1342308352
Control25=IDC_RADIO_MIXER_FROM_NULL,button,1342308361
Control26=IDC_RADIO_MIXER_FROM_INPUT,button,1342177289
Control27=IDC_RADIO_MIXER_FROM_PLAY,button,1342177289
Control28=IDC_RADIO_MIXER_FROM_CONF,button,1342177289
Control29=IDC_RADIO_MIXER_FROM_RTP,button,1342177289
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_Record_u16SRC_ChID1,edit,1350631552
Control32=IDC_STATIC,static,1342308352
Control33=IDC_RADIO_MIXER_FROM_NULL2,button,1342308361
Control34=IDC_RADIO_MIXER_FROM_INPUT2,button,1342177289
Control35=IDC_RADIO_MIXER_FROM_PLAY2,button,1342177289
Control36=IDC_RADIO_MIXER_FROM_CONF2,button,1342177289
Control37=IDC_RADIO_MIXER_FROM_RTP2,button,1342177289
Control38=IDC_STATIC,static,1342308352
Control39=IDC_EDIT_Record_u16SRC_ChID2,edit,1350631552
Control40=IDC_STATIC,static,1342308352
Control41=IDC_CHECK_s8IsAppend,button,1342242819
Control42=IDC_STATIC,static,1342308352
Control43=IDC_EDIT_Record_s8FileName,edit,1484849280
Control44=IDOK,button,1342242817
Control45=IDCANCEL,button,1342242816
Control46=IDC_STATIC,static,1342308352
Control47=IDC_STATIC,static,1342308352
Control48=IDC_STATIC,static,1342308352
Control49=IDC_EDIT_Record_DevcieID,edit,1484849280

[CLS:CXMSApi_RecordSet]
Type=0
HeaderFile=XMSApi_RecordSet.h
ImplementationFile=XMSApi_RecordSet.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK_s8MixEnable

[DLG:IDD_DIALOG_CONTROLRECORD]
Type=1
Class=CXMSApi_ControlRecord
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_ControlRecord_DeviceID,edit,1484849280
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_u32StepSize,edit,1350631552
Control8=IDC_EDIT_u32SpeedSize,edit,1484849280
Control9=IDC_BUTTON_Stop,button,1342242816
Control10=IDC_BUTTON_Pause,button,1342242816
Control11=IDC_BUTTON_Resume,button,1342242816
Control12=IDC_BUTTON_StepBack,button,1342242816

[CLS:CXMSApi_ControlRecord]
Type=0
HeaderFile=XMSApi_ControlRecord.h
ImplementationFile=XMSApi_ControlRecord.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CXMSApi_ControlRecord

[DLG:IDD_DIALOG_CONF]
Type=1
Class=CXMSApi_ConfParam
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_RADIO_InputOpt_0,button,1342308361
Control6=IDC_RADIO_InputOpt_1,button,1342177289
Control7=IDC_RADIO_InputOpt_2,button,1342177289
Control8=IDC_RADIO_OutputOpt_0,button,1342308361
Control9=IDC_RADIO_OutputOpt_1,button,1342177289
Control10=IDC_RADIO_OutputOpt_2,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_Conf_DeviceID,edit,1484849280
Control14=IDC_EDIT_Media_DeviceID,edit,1484849280

[CLS:CXMSApi_ConfParam]
Type=0
HeaderFile=XMSApi_ConfParam.h
ImplementationFile=XMSApi_ConfParam.cpp
BaseClass=CDialog
Filter=D
LastObject=CXMSApi_ConfParam
VirtualFilter=dWC

[DLG:IDD_DIALOG_PARAM_VOICE]
Type=1
Class=CXMSApi_Test_ParamVoice
ControlCount=77
Control1=IDC_CHECK_Gtd_u8ChannelEnable,button,1342242819
Control2=IDC_CHECK_Gtd_u8DTMFEnable,button,1342242819
Control3=IDC_CHECK_Gtd_u8MR2FEnable,button,1073807363
Control4=IDC_CHECK_Gtd_u8MR2BEnable,button,1342242819
Control5=IDC_CHECK_Gtd_u8DPDEnable,button,1342242819
Control6=IDC_EDIT_Gtd_u8GTDID,edit,1350631552
Control7=IDC_CHECK_u8PlayGainMode,button,1342242819
Control8=IDC_EDIT_Play_u16FixGain,edit,1350631552
Control9=IDC_CHECK_Gtd_u8PVDEnable,button,1342242819
Control10=IDC_CHECK_Gtd_u8GTDEnable,button,1342242819
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816
Control13=IDC_CHECK_In_u8AgcEnable,button,1342242819
Control14=IDC_RADIO_In_ALS_Mode,button,1342308361
Control15=IDC_RADIO_In_AGC_Mode,button,1342177289
Control16=IDC_CHECK_In_u8EcEnable,button,1342242819
Control17=IDC_RADIO_In_Ref_Input,button,1342308361
Control18=IDC_RADIO_In_Ref_Output,button,1342177289
Control19=IDC_EDIT_In_u16EcRefCh,edit,1350631552
Control20=IDC_CHECK_In_u8VadEnable,button,1342242819
Control21=IDC_CHECK_In_u8TadEnable,button,1342242819
Control22=IDC_EDIT_In_u16FixGain,edit,1350631552
Control23=IDC_CHECK_Out_u8AgcEnable,button,1342242819
Control24=IDC_RADIO_Out_ALS_Mode,button,1342308361
Control25=IDC_RADIO_Out_AGC_Mode,button,1342177289
Control26=IDC_RADIO_Out_Silence,button,1342308361
Control27=IDC_RADIO_Out_Input,button,1342177289
Control28=IDC_RADIO_Out_Play,button,1342177289
Control29=IDC_RADIO_Out_Mixer,button,1342177289
Control30=IDC_EDIT_Out_u16FixGain,edit,1350631552
Control31=IDC_RADIO_MIXER_FROM_NULL,button,1342308361
Control32=IDC_RADIO_MIXER_FROM_INPUT,button,1342177289
Control33=IDC_RADIO_MIXER_FROM_PLAY,button,1342177289
Control34=IDC_RADIO_MIXER_FROM_CONF,button,1342177289
Control35=IDC_RADIO_MIXER_FROM_RTP,button,1342177289
Control36=IDC_EDIT_Out_u16SRC_ChID1,edit,1350631552
Control37=IDC_RADIO_MIXER_FROM_NULL2,button,1342308361
Control38=IDC_RADIO_MIXER_FROM_INPUT2,button,1342177289
Control39=IDC_RADIO_MIXER_FROM_PLAY2,button,1342177289
Control40=IDC_RADIO_MIXER_FROM_CONF2,button,1342177289
Control41=IDC_RADIO_MIXER_FROM_RTP2,button,1342177289
Control42=IDC_EDIT_Out_u16SRC_ChID2,edit,1350631552
Control43=IDC_STATIC,static,1342308352
Control44=IDC_EDIT_ParamVoice_DeviceID,edit,1484849280
Control45=IDC_STATIC,button,1342177287
Control46=IDC_STATIC,button,1342177287
Control47=IDC_STATIC,button,1342177287
Control48=IDC_STATIC,static,1342308352
Control49=IDC_STATIC,static,1342308352
Control50=IDC_STATIC,static,1342308352
Control51=IDC_STATIC,static,1342308352
Control52=IDC_STATIC,static,1342308352
Control53=IDC_STATIC,static,1342308352
Control54=IDC_STATIC,static,1342308352
Control55=IDC_STATIC,static,1342308352
Control56=IDC_STATIC,static,1342308352
Control57=IDC_STATIC,static,1342308352
Control58=IDC_STATIC,static,1342308352
Control59=IDC_STATIC,static,1342308352
Control60=IDC_STATIC,static,1342308352
Control61=IDC_STATIC,static,1073872896
Control62=IDC_STATIC,static,1342308352
Control63=IDC_STATIC,static,1342308352
Control64=IDC_STATIC,static,1342308352
Control65=IDC_STATIC,button,1342177287
Control66=IDC_STATIC,static,1342308352
Control67=IDC_CHECK_u8InputCtrlValid,button,1342242819
Control68=IDC_CHECK_u8OutputCtrlValid,button,1342242819
Control69=IDC_CHECK_u8GtdCtrlValid,button,1342242819
Control70=IDC_STATIC,static,1342308352
Control71=IDC_STATIC,static,1342308352
Control72=IDC_STATIC,static,1342308352
Control73=IDC_STATIC,static,1342308352
Control74=IDC_STATIC,static,1342308352
Control75=IDC_STATIC,button,1342177287
Control76=IDC_STATIC,static,1342308352
Control77=IDC_STATIC,static,1342308352

[CLS:CXMSApi_Test_ParamVoice]
Type=0
HeaderFile=xmsapi_test_paramvoice.h
ImplementationFile=xmsapi_test_paramvoice.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CXMSApi_Test_ParamVoice

[DLG:IDD_DIALOG_BuildPlayIndex]
Type=1
Class=CXMSApi_BuildIndex
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_BuildIndex_DeviceID,edit,1484849280
Control4=IDC_RADIO_INDEX_FILE,button,1342177289
Control5=IDC_RADIO_INDEX_RAM,button,1342177289
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BUTTON_SelectBuildIndex,button,1342242816
Control9=IDC_EDIT_BuildPlayIndex,edit,1350631552
Control10=IDC_BUTTON_Build,button,1342242816

[CLS:CXMSApi_BuildIndex]
Type=0
HeaderFile=XMSApi_BuildIndex.h
ImplementationFile=XMSApi_BuildIndex.cpp
BaseClass=CDialog
Filter=D
LastObject=CXMSApi_BuildIndex
VirtualFilter=dWC

[DLG:IDD_DIALOG1]
Type=1
Class=CXMSApi_SendIOData
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_u16IoLen,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_DataContent,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_DevcieID,edit,1484849280
Control9=IDC_BUTTON1,button,1342242816
Control10=IDC_COMBO1,combobox,1344339970
Control11=IDC_BUTTON2,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT1,edit,1350631552

[CLS:CXMSApi_SendIOData]
Type=0
HeaderFile=XMSApi_SendIOData.h
ImplementationFile=XMSApi_SendIOData.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CXMSApi_SendIOData

[DLG:IDD_DIALOG_PARAM_CAS]
Type=1
Class=CXMSApi_ParamCAS
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_ParamVoice_DeviceID,edit,1484849280
Control6=IDC_EDIT_ParamCAS_DeviceID,edit,1484849280
Control7=IDC_STATIC,static,1342308352

[CLS:CXMSApi_ParamCAS]
Type=0
HeaderFile=XMSApi_ParamCAS.h
ImplementationFile=XMSApi_ParamCAS.cpp
BaseClass=CDialog
Filter=D
LastObject=CXMSApi_ParamCAS
VirtualFilter=dWC

[DLG:IDD_DIALOG_ANATRUNK]
Type=1
Class=CDialogAnaTrunk
ControlCount=11
Control1=IDC_EDIT_TRUNK_DEV,edit,1484849280
Control2=IDC_STATIC,static,1342308354
Control3=IDC_STATIC,static,1073872898
Control4=IDC_EDIT_VOC_DEV,edit,1216413824
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_CALLINRINGCOUNT,edit,1350631552
Control7=IDCANCEL,button,1342242816
Control8=IDOK,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_ANSWERCALLINTIMEOUT,edit,1350631552

[CLS:CDialogAnaTrunk]
Type=0
HeaderFile=DialogAnaTrunk.h
ImplementationFile=DialogAnaTrunk.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogAnaTrunk
VirtualFilter=dWC

[DLG:IDD_DIALOG_ANAUSER]
Type=1
Class=CDialogAnalogUser
ControlCount=18
Control1=IDC_STATIC,static,1342308354
Control2=IDC_STATIC,static,1073872898
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_USER_DEV,edit,1484849280
Control6=IDC_EDIT_VOC_DEV,edit,1216413824
Control7=IDC_RADIO_CALLERID_DISABLE,button,1342177289
Control8=IDC_RADIO_CALLERID_FSK,button,1342177289
Control9=IDC_RADIO_CALLERID_DTMF,button,1342177289
Control10=IDC_RADIO_SINGLEPULSE,button,1342308361
Control11=IDC_RADIO_DOUBLEPULSE,button,1342177289
Control12=IDC_RADIO_TRIPPULSE,button,1342177289
Control13=IDC_BUTTON1,button,1342242816
Control14=IDCANCEL,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_CALLOUTRINGTIMETOU,edit,1350631552
Control18=IDC_STATIC,static,1342308352

[CLS:CDialogAnalogUser]
Type=0
HeaderFile=DialogAnalogUser.h
ImplementationFile=DialogAnalogUser.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogAnalogUser

[DLG:IDD_DIALOG_RECCSP]
Type=1
Class=CRecCSPDlg
ControlCount=11
Control1=IDC_COMBO_ENCODETYPE,combobox,1344339971
Control2=IDC_COMBO_SRCMODE,combobox,1344339971
Control3=IDC_EDIT_PACKLEN,edit,1350631552
Control4=IDC_RADIO_BARGE_IN,button,1342177289
Control5=IDC_RADIO_NO_BARGE_IN,button,1342177289
Control6=IDC_BUTTON1,button,1342242816
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352

[CLS:CRecCSPDlg]
Type=0
HeaderFile=RecCSPDlg.h
ImplementationFile=RecCSPDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON2
VirtualFilter=dWC

[DLG:IDD_DIALOG_PLAYCSP]
Type=1
Class=CCSPPlayDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_DATALEN,edit,1484849280
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO_DATATYPE,combobox,1478557698
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_TASKID,edit,1484849280
Control7=IDC_BUTTON1,button,1342242816
Control8=IDCANCEL,button,1342242816

[CLS:CCSPPlayDlg]
Type=0
HeaderFile=CSPPlayDlg.h
ImplementationFile=CSPPlayDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_DATALEN
VirtualFilter=dWC

[DLG:IDD_DIALOG_PARAM_VOIP]
Type=1
Class=CXMS_Test_ParamVoIP
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_IPDEV,edit,1484849280
Control4=IDC_EDIT_VOCDEV,edit,1484849280
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342178055
Control8=IDC_RADIO_MIXER_FROM_NULL,button,1342177289
Control9=IDC_RADIO_MIXER_FROM_INPUT,button,1342177289
Control10=IDC_RADIO_MIXER_FROM_PLAY,button,1342177289
Control11=IDC_RADIO_MIXER_FROM_CONF,button,1342177289
Control12=IDC_RADIO_MIXER_FROM_IP,button,1342177289

[CLS:CXMS_Test_ParamVoIP]
Type=0
HeaderFile=XMS_Test_ParamVoIP.h
ImplementationFile=XMS_Test_ParamVoIP.cpp
BaseClass=CDialog
Filter=D
LastObject=CXMS_Test_ParamVoIP
VirtualFilter=dWC

[DLG:IDD_DIALOG_PARAM_FSK]
Type=1
Class=CDialogFskSet
ControlCount=21
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_rxFilterWindow,edit,1484849280
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_rxPeriodTheshold,edit,1484849280
Control7=IDC_EDIT_rxAllOneSamples,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_rxDataTimeOut,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_txBit1Freq,edit,1350631552
Control13=IDC_EDIT_txBit0Freq,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,button,1342178055
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_tx55Count,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT_txAllOneSamples,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT_rx55Count,edit,1350631552

[CLS:CDialogFskSet]
Type=0
HeaderFile=DialogFskSet.h
ImplementationFile=DialogFskSet.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogFskSet
VirtualFilter=dWC

[DLG:IDD_DIALOG_PARAM_Flash]
Type=1
Class=CDialogFlashSet
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDD_ABOUTBOX,button,1342178055
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_m_u8MinHookFlashTime,edit,1350631552
Control7=IDC_EDIT_m_u8MaxHookFlashTime,edit,1350631552

[CLS:CDialogFlashSet]
Type=0
HeaderFile=DialogFlashSet.h
ImplementationFile=DialogFlashSet.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogFlashSet

[DLG:IDD_DIALOG_GTDSET]
Type=1
Class=?
ControlCount=36
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TAB_GTD_FREQ,SysTabControl32,1342177344
Control4=IDC_STATIC,button,1342178055
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_FREQ_INDEX,edit,1484849280
Control8=IDC_EDIT_FREQ_COEF,edit,1350631552
Control9=IDC_TAB_GTD_TONE,SysTabControl32,1350565888
Control10=IDC_STATIC,button,1342178055
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_EDIT_GTDID,edit,1484849280
Control25=IDC_EDIT_MINONTIME1,edit,1350631552
Control26=IDC_EDIT_THRESHOLD,edit,1350631552
Control27=IDC_EDIT_FREQ_MASK,edit,1350631552
Control28=IDC_EDIT_REPEATCOUNT,edit,1350631552
Control29=IDC_EDIT_MAXONTIME1,edit,1350631552
Control30=IDC_EDIT_MINOFFTIME1,edit,1350631552
Control31=IDC_EDIT_MAXOFFTIME1,edit,1350631552
Control32=IDC_EDIT_MINONTIME2,edit,1350631552
Control33=IDC_EDIT_MAXONTIME2,edit,1350631552
Control34=IDC_EDIT_MINOFFTIME2,edit,1350631552
Control35=IDC_EDIT_MAXOFFTIME2,edit,1350631552
Control36=IDC_COMBO_ENVELOP,combobox,1344340227

[DLG:IDD_DIALOG_PARAM_ATRUNKFLASH]
Type=1
Class=?
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342178055
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_AT_TYPE,edit,1484849280
Control8=IDC_EDIT_AT_TXSTATE,edit,1484849280
Control9=IDC_CHECK_AT_CHANNEL_ENABLED,button,1476460547
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_AT_RXSTATE,edit,1484849280

[DLG:IDD_DIALOG_PARAM_3GFaxTone]
Type=1
Class=CDialog3GFaxTone
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342178055
Control4=IDC_CHECK_3GFAXTONE,button,1342242819

[CLS:CDialog3GFaxTone]
Type=0
HeaderFile=Dialog3GFaxTone.h
ImplementationFile=Dialog3GFaxTone.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialog3GFaxTone
VirtualFilter=dWC

[DLG:IDD_DIALOG_PARAM_FAXHEADER]
Type=1
Class=CDialogFaxHeader
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_WITHHEADER,edit,1350631552
Control8=IDC_EDIT_HEADERUSER,edit,1350631552
Control9=IDC_EDIT_HEADERFROM,edit,1350631552
Control10=IDC_EDIT_HEADERTO,edit,1350631552

[CLS:CDialogFaxHeader]
Type=0
HeaderFile=DialogFaxHeader.h
ImplementationFile=DialogFaxHeader.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogFaxHeader
VirtualFilter=dWC

