// XMSApi_TestDlg.h : header file
//

#if !defined(AFX_XMSAPI_TESTDLG_H__A39246B6_8B39_4D99_9DE8_12583EF59E11__INCLUDED_)
#define AFX_XMSAPI_TESTDLG_H__A39246B6_8B39_4D99_9DE8_12583EF59E11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_TestDlg dialog

class CXMSApi_TestDlg : public CDialog
{
// Construction
public:
	CXMSApi_TestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_TestDlg)
	enum { IDD = IDD_XMSAPI_TEST_DIALOG };
	CComboBox	m_FaxType;
	CEdit	m_DspPort;
	CEdit	m_DspIP;
	CComboBox	m_ComboIndexList;
	CComboBox	m_ComboParam;
	CListBox	m_ListMsgHidden;
	CListBox	m_ListMsg;
	CListBox	m_ListDevInfo;
	CListBox	m_ListDevOpened;
	CComboBox	m_ComboMsg;
	CComboBox	m_ComboDevice;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_TestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_TestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONGetDeviceList();
	afx_msg void OnSelchangeListDeviceInfo();
	afx_msg void OnBUTTONOpenDevice();
	afx_msg void OnSelchangeListDeviceOpened();
	afx_msg void OnDblclkListDeviceOpened();
	afx_msg void OnBUTTONExchange();
	afx_msg void OnSelchangeComboDevice();
	afx_msg void OnSelchangeComboMsg();
	afx_msg void OnBUTTONCloseDevice();
	afx_msg void OnBUTTONResetDevice();
	afx_msg void OnBUTTONGetDeviceState();
	afx_msg void OnBUTTONAlertCall();
	afx_msg void OnBUTTONAnswerCallIn();
	afx_msg void OnBUTTONClearCall();
	afx_msg void OnBUTTONMakeCallOut();
	afx_msg void OnCHECKAutoScroll();
	afx_msg void OnBUTTONLinkDevice();
	afx_msg void OnBUTTONUnlinkDevice();
	afx_msg void OnBUTTONPlay();
	afx_msg void OnBUTTONSendFax();
	afx_msg void OnBUTTONStopSendFax();
	afx_msg void OnBUTTONReceiveFax();
	afx_msg void OnBUTTONStopReceiveFax();
	afx_msg void OnSelchangeListMsg();
	afx_msg void OnBUTTONSelectSendFaxFile();
	afx_msg void OnBUTTONSelectReceiveFaxFile();
	afx_msg void OnBUTTONSelectPlayFile();
	afx_msg void OnBUTTONSelectRecordFile();
	afx_msg void OnDblclkListDeviceInfo();
	afx_msg void OnBUTTONMyStopPlay();
	afx_msg void OnBUTTONRecord();
	afx_msg void OnBUTTONMyStopRecord();
	afx_msg void OnBUTTONJoinConf();
	afx_msg void OnBUTTONLeaveFromConf();
	afx_msg void OnBUTTONClearConf();
	afx_msg void OnBUTTONControlPlay();
	afx_msg void OnBUTTONPlayDetail();
	afx_msg void OnBUTTONRecordDetail();
	afx_msg void OnBUTTONControlRecord();
	afx_msg void OnBUTTONSetParam();
	afx_msg void OnSelchangeCOMBOParamType();
	afx_msg void OnBUTTONInitPlayIndex();
	afx_msg void OnBUTTONBuildPlayIndex();
	afx_msg void OnBUTTONSendIOData();
	afx_msg void OnButton1();
	afx_msg void OnButtonConn();
	afx_msg void OnButtonReccsp();
	afx_msg void OnButtonCtrlreccsp();
	afx_msg void OnBUTTONSelectRecordFile2();
	afx_msg void OnButtonCtxlink();
	afx_msg void OnButtonCtxunlink();
	afx_msg void OnButtonDisconn();
	afx_msg void OnBUTTONSendFaxT38();
	afx_msg void OnBUTTONStopSendFaxT38();
	afx_msg void OnBUTTONReceiveFaxT38();
	afx_msg void OnBUTTONStopReceiveFaxT38();
	afx_msg void OnButtonPlaycsp();
	afx_msg void OnBUTTONGetBoardInfo();
	afx_msg void OnButtonCtrlplaycsp();
	afx_msg void OnBUTTONGetDeviceStateVoIP();
	afx_msg void OnRADIOOther();
	afx_msg void OnRadioH323();
	afx_msg void OnRadioSip();
	afx_msg void OnRadioOther();
	afx_msg void OnRadioVoip();
	afx_msg void OnRadioSend();
	afx_msg void OnRadioRecv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_TESTDLG_H__A39246B6_8B39_4D99_9DE8_12583EF59E11__INCLUDED_)
