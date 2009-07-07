// XMS_SS7MonitorDlg.h : header file
//

#if !defined(AFX_XMS_SS7MONITORDLG_H__DDFDF5A5_0A44_48C8_93E5_94A69B83A6CA__INCLUDED_)
#define AFX_XMS_SS7MONITORDLG_H__DDFDF5A5_0A44_48C8_93E5_94A69B83A6CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_SS7MonitorDlg dialog

class CXMS_SS7MonitorDlg : public CDialog
{
// Construction
public:
	CXMS_SS7MonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_SS7MonitorDlg)
	enum { IDD = IDD_XMS_SS7MONITOR_DIALOG };
	CComboBox	m_ComboTUPMsgType;
	CComboBox	m_ComboISUPMsgType;
	CListCtrl	m_ListTrunk;
	CListCtrl	m_ListPCM;
	CListBox	m_ListMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_SS7MonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_SS7MonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonSendsigmsg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_SS7MONITORDLG_H__DDFDF5A5_0A44_48C8_93E5_94A69B83A6CA__INCLUDED_)
