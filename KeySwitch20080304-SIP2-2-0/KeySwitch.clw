; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInLineSetDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "KeySwitch.h"

ClassCount=6
Class1=CKeySwitchApp
Class2=CKeySwitchDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_OUT_LINE_SET
Resource2=IDR_MAINFRAME
Resource3=IDD_KEYSWITCH_DIALOG
Resource4=IDD_VOIP_SET
Resource5=IDD_ABOUTBOX
Class4=CInLineSetDlg
Class5=COutLineSetDlg
Resource6=IDD_IN_LINE_SET
Class6=CVoipSetDlg
Resource7=IDR_MENU1

[CLS:CKeySwitchApp]
Type=0
HeaderFile=KeySwitch.h
ImplementationFile=KeySwitch.cpp
Filter=N
LastObject=CKeySwitchApp

[CLS:CKeySwitchDlg]
Type=0
HeaderFile=KeySwitchDlg.h
ImplementationFile=KeySwitchDlg.cpp
Filter=D
LastObject=IDR_EXIT_SYS
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=KeySwitchDlg.h
ImplementationFile=KeySwitchDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_KEYSWITCH_DIALOG]
Type=1
Class=CKeySwitchDlg
ControlCount=14
Control1=IDC_LIST_STATUS,SysListView32,1350631433
Control2=IDC_LIST1,listbox,1353777409
Control3=IDC_EDIT1,edit,1082196096
Control4=IDC_BUTTON1,button,1342242816
Control5=IDC_DSP,listbox,1084293377
Control6=IDC_STATIC,static,1073872896
Control7=IDC_TRUNK,listbox,1353777409
Control8=IDC_VOICE,listbox,1352728833
Control9=IDC_VOIP,listbox,1353777409
Control10=IDC_STATIC,static,1073872896
Control11=IDC_BUTTON2,button,1342242816
Control12=IDC_BUTTON3,button,1342242816
Control13=IDC_BUTTON4,button,1342242816
Control14=IDC_BUTTON5,button,1342242816

[MNU:IDR_MENU1]
Type=1
Class=CKeySwitchDlg
Command1=IDR_START_SERVICE
Command2=IDR_EXIT_SYS
Command3=IDR_DEBUG
Command4=IDR_CALL_QUERY
Command5=IDR_ABOUT
CommandCount=5

[DLG:IDD_IN_LINE_SET]
Type=1
Class=CInLineSetDlg
ControlCount=21
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LINE_NO,edit,1350631552
Control12=IDC_PHONE_NUMBER,edit,1350631552
Control13=IDC_ENABLE,combobox,1344340227
Control14=IDC_ENABLE_LONG,combobox,1344340227
Control15=IDC_USER_NAME,edit,1350631552
Control16=IDC_IPADDRESS,SysIPAddress32,1342242816
Control17=IDC_MEMO,RICHEDIT,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_QUICK_DIAL,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_TARGET_USER,edit,1350631552

[DLG:IDD_OUT_LINE_SET]
Type=1
Class=COutLineSetDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_OUT_LINE_NO,edit,1350631552
Control8=IDC_OUT_PHONE_NUMBER,edit,1350631552
Control9=IDC_OUT_MEMO,RICHEDIT,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_ENABLE,combobox,1344340227

[CLS:CInLineSetDlg]
Type=0
HeaderFile=sysset\cinlineset.h
ImplementationFile=sysset\cinlineset.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TARGET_USER
VirtualFilter=dWC

[CLS:COutLineSetDlg]
Type=0
HeaderFile=sysset\outlinesetdlg.h
ImplementationFile=sysset\outlinesetdlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_ENABLE

[DLG:IDD_VOIP_SET]
Type=1
Class=CVoipSetDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_VOIP_NO,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_VOIP_IP,SysIPAddress32,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_VOIP_PORT,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_VOIP_USER,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_VOIP_PASS,edit,1350631552

[CLS:CVoipSetDlg]
Type=0
HeaderFile=VoipSetDlg.h
ImplementationFile=VoipSetDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CVoipSetDlg

