; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPlayDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DJIPPBXGateway.h"

ClassCount=4
Class1=CDJIPPBXGatewayApp
Class2=CDJIPPBXGatewayDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DJIPPBXGATEWAY_DIALOG
Class4=CPlayDlg
Resource4=IDD_DIALOG_PLAY

[CLS:CDJIPPBXGatewayApp]
Type=0
HeaderFile=DJIPPBXGateway.h
ImplementationFile=DJIPPBXGateway.cpp
Filter=N

[CLS:CDJIPPBXGatewayDlg]
Type=0
HeaderFile=DJIPPBXGatewayDlg.h
ImplementationFile=DJIPPBXGatewayDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_CONTROLINFO

[CLS:CAboutDlg]
Type=0
HeaderFile=DJIPPBXGatewayDlg.h
ImplementationFile=DJIPPBXGatewayDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC_NAMEINFO,static,1342308480
Control3=IDC_STATIC_COPYRIGHTINFO,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DJIPPBXGATEWAY_DIALOG]
Type=1
Class=CDJIPPBXGatewayDlg
ControlCount=11
Control1=IDOK,button,1073840128
Control2=IDCANCEL,button,1073840128
Control3=IDC_LIST_CALLINFO,SysListView32,1350631425
Control4=IDC_STATIC_INFOLIST,button,1342177287
Control5=IDC_LIST_LOGINFO,listbox,1352728841
Control6=IDC_STATIC_EVENTINFO,button,1342177287
Control7=IDC_STATIC_CTRLBTN,button,1342177287
Control8=IDC_BUTTON_EXIT,button,1342275585
Control9=IDC_BUTTON_CALLNUMCFG,button,1342275584
Control10=IDC_BUTTON_PLAY,button,1342275584
Control11=IDC_EDIT_CONTROLINFO,edit,1350633600

[DLG:IDD_DIALOG_PLAY]
Type=1
Class=CPlayDlg
ControlCount=8
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_EDIT_VOCDSPID,edit,1350639744
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_VOCCHID,edit,1350639744
Control7=IDC_EDIT_FILE,edit,1350631552
Control8=IDC_BUTTON_FILE,button,1342275584

[CLS:CPlayDlg]
Type=0
HeaderFile=PlayDlg.h
ImplementationFile=PlayDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

