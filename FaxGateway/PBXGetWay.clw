; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPBXGetWayDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PBXGetWay.h"

ClassCount=4
Class1=CPBXGetWayApp
Class2=CPBXGetWayDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_DIALOGBAR (English (U.S.))
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=DspInformation
Resource4=IDD_PBXGETWAY_DIALOG

[CLS:CPBXGetWayApp]
Type=0
HeaderFile=PBXGetWay.h
ImplementationFile=PBXGetWay.cpp
Filter=N

[CLS:CPBXGetWayDlg]
Type=0
HeaderFile=PBXGetWayDlg.h
ImplementationFile=PBXGetWayDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CPBXGetWayDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=PBXGetWayDlg.h
ImplementationFile=PBXGetWayDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PBXGETWAY_DIALOG]
Type=1
Class=CPBXGetWayDlg
ControlCount=11
Control1=IDC_STATIC,static,1342308866
Control2=IDC_EDIT_IPADDR,edit,1484849280
Control3=IDC_STATIC,static,1342308866
Control4=IDC_EDIT_PORT,edit,1484849280
Control5=IDC_STATICType,static,1342308866
Control6=IDC_EDITVoip,edit,1484849280
Control7=IDC_BUTTONRecord,button,1342242816
Control8=IDC_DTrunk,SysListView32,1350631425
Control9=IDC_VOIP,SysListView32,1350631425
Control10=IDC_STATIC,static,1342308866
Control11=IDC_EDITRecordDev,listbox,1352728835

[DLG:IDD_DIALOGBAR (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:DspInformation]
Type=0
HeaderFile=DspInformation.h
ImplementationFile=DspInformation.cpp
BaseClass=CDialog
Filter=D
LastObject=DspInformation
VirtualFilter=dWC

