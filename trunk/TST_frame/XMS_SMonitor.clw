; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXMS_SMonitorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XMS_SMonitor.h"

ClassCount=3
Class1=CXMS_SMonitorApp
Class2=CXMS_SMonitorDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_XMS_SMONITOR_DIALOG

[CLS:CXMS_SMonitorApp]
Type=0
HeaderFile=XMS_SMonitor.h
ImplementationFile=XMS_SMonitor.cpp
Filter=N

[CLS:CXMS_SMonitorDlg]
Type=0
HeaderFile=XMS_SMonitorDlg.h
ImplementationFile=XMS_SMonitorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BUTTON_CLEAR

[CLS:CAboutDlg]
Type=0
HeaderFile=XMS_SMonitorDlg.h
ImplementationFile=XMS_SMonitorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_XMS_SMONITOR_DIALOG]
Type=1
Class=CXMS_SMonitorDlg
ControlCount=11
Control1=IDC_LIST_DEVICE,SysListView32,1350631425
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_IPADDRESS,edit,1350631552
Control6=IDC_EDIT_PORT,edit,1350631552
Control7=IDC_BUTTON_EXIT,button,1342242816
Control8=IDC_BUTTON_START,button,1476460544
Control9=IDC_LIST_RAWDATA_INFO,SysListView32,1350631425
Control10=IDC_BUTTON_SEND,button,1342242816
Control11=IDC_EDIT_DATA,edit,1350631552

