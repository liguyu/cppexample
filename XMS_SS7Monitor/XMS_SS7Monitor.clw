; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXMS_SS7MonitorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XMS_SS7Monitor.h"

ClassCount=3
Class1=CXMS_SS7MonitorApp
Class2=CXMS_SS7MonitorDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_XMS_SS7MONITOR_DIALOG

[CLS:CXMS_SS7MonitorApp]
Type=0
HeaderFile=XMS_SS7Monitor.h
ImplementationFile=XMS_SS7Monitor.cpp
Filter=N

[CLS:CXMS_SS7MonitorDlg]
Type=0
HeaderFile=XMS_SS7MonitorDlg.h
ImplementationFile=XMS_SS7MonitorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_COMBO_ISUPMSGTYPE

[CLS:CAboutDlg]
Type=0
HeaderFile=XMS_SS7MonitorDlg.h
ImplementationFile=XMS_SS7MonitorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_XMS_SS7MONITOR_DIALOG]
Type=1
Class=CXMS_SS7MonitorDlg
ControlCount=12
Control1=IDOK,button,1342242816
Control2=IDC_LIST_PCM,SysListView32,1350631425
Control3=IDC_LIST_TRUNK,SysListView32,1350631433
Control4=IDC_LIST_MSG,listbox,1353777411
Control5=IDC_BUTTON_SEND,button,1342242817
Control6=IDC_EDIT_CMD,edit,1350631552
Control7=IDC_LIST_SS7LINK,SysListView32,1350631425
Control8=IDC_BUTTON_SENDSIGMSG,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_COMBO_ISUPMSGTYPE,combobox,1344340226
Control11=IDC_COMBO_TUPMSGTYPE,combobox,1344340226
Control12=IDC_STATIC,static,1342308352
