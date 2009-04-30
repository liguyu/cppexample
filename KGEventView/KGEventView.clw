; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CEditView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "KGEventView.h"
LastPage=0

ClassCount=10
Class1=CKGEventViewApp
Class2=CKGEventViewDoc
Class3=CKGEventViewView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_DLGRUN
Class6=CInfoDetail
Class7=CInfoRaw
Resource4=IDD_DIALOGBAR (English (U.S.))
Class8=CMyDlgBar
Resource5=IDD_ABOUTBOX (English (U.S.))
Class9=DialogRun
Class10=EditTxt
Resource6=IDR_MAINFRAME (English (U.S.))

[CLS:CKGEventViewApp]
Type=0
HeaderFile=KGEventView.h
ImplementationFile=KGEventView.cpp
Filter=N
LastObject=CKGEventViewApp

[CLS:CKGEventViewDoc]
Type=0
HeaderFile=KGEventViewDoc.h
ImplementationFile=KGEventViewDoc.cpp
Filter=N
LastObject=CKGEventViewDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CKGEventViewView]
Type=0
HeaderFile=KGEventViewView.h
ImplementationFile=KGEventViewView.cpp
Filter=C
LastObject=CKGEventViewView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_Stop




[CLS:CAboutDlg]
Type=0
HeaderFile=KGEventView.cpp
ImplementationFile=KGEventView.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_APP_ABOUT
CommandCount=12
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_DLG_BAR
Command3=ID_RUN_DLG
Command4=ID_Stop
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_RUN_DLG
Command3=ID_Stop
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=IDD_VIEW_DIALOGBAR
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
CommandCount=10

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CInfoDetail]
Type=0
HeaderFile=InfoDetail.h
ImplementationFile=InfoDetail.cpp
BaseClass=CListView
Filter=C
LastObject=CInfoDetail
VirtualFilter=VWC

[CLS:CInfoRaw]
Type=0
HeaderFile=InfoRaw.h
ImplementationFile=InfoRaw.cpp
BaseClass=CListView
Filter=C
LastObject=CInfoRaw
VirtualFilter=VWC

[DLG:IDD_DIALOGBAR (English (U.S.))]
Type=1
Class=CMyDlgBar
ControlCount=13
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_APPID,combobox,1344340771
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO_DEVTYPE,combobox,1344340771
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BUTTON_SELECT,button,1342242827
Control8=IDC_STATIC,static,1342308352
Control9=IDC_COMBO_API,combobox,1344340771
Control10=IDC_STATIC,static,1342308352
Control11=IDC_COMBO_EVT,combobox,1344340771
Control12=IDC_COMBO_DSPID,combobox,1344340771
Control13=IDC_COMBO_CHNLID,combobox,1344340771

[CLS:CMyDlgBar]
Type=0
HeaderFile=MyDlgBar.h
ImplementationFile=MyDlgBar.cpp
BaseClass=CDialog
Filter=D
LastObject=CMyDlgBar

[CLS:DialogRun]
Type=0
HeaderFile=DialogRun.h
ImplementationFile=DialogRun.cpp
BaseClass=CDialog
Filter=D
LastObject=DialogRun
VirtualFilter=dWC

[DLG:IDD_DLGRUN]
Type=1
Class=DialogRun
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_FILE_PATH,edit,1484849280
Control5=IDC_BUTTON_FPATH,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_IPADDRESS,SysIPAddress32,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_PORT,edit,1350639744

[CLS:EditTxt]
Type=0
HeaderFile=EditTxt.h
ImplementationFile=EditTxt.cpp
BaseClass=CEditView
Filter=C
VirtualFilter=VWC
LastObject=EditTxt

