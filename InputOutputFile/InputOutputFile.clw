; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInputOutputFileDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "InputOutputFile.h"

ClassCount=4
Class1=CInputOutputFileApp
Class2=CInputOutputFileDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_INPUTOUTPUTFILE_DIALOG

[CLS:CInputOutputFileApp]
Type=0
HeaderFile=InputOutputFile.h
ImplementationFile=InputOutputFile.cpp
Filter=N

[CLS:CInputOutputFileDlg]
Type=0
HeaderFile=InputOutputFileDlg.h
ImplementationFile=InputOutputFileDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=InputOutputFileDlg.h
ImplementationFile=InputOutputFileDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_INPUTOUTPUTFILE_DIALOG]
Type=1
Class=CInputOutputFileDlg
ControlCount=12
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON_INPUT,button,1342242816
Control3=IDC_BUTTON_OUTPUT,button,1342242816
Control4=IDC_EDIT_INPUT_FILE,edit,1350631552
Control5=IDC_EDIT_OUTPUT_FILE,edit,1350631552
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EDIT_INT,edit,1350631552
Control8=IDC_EDIT_FLOAT,edit,1350631552
Control9=IDC_EDIT_STRING,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

