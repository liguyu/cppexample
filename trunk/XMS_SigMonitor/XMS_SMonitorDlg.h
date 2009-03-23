// XMS_SMonitorDlg.h : header file
//

#if !defined(AFX_XMS_SMONITORDLG_H__7114E36E_472F_4D88_A33C_4E45C2A520FA__INCLUDED_)
#define AFX_XMS_SMONITORDLG_H__7114E36E_472F_4D88_A33C_4E45C2A520FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_SMonitorDlg dialog

class CXMS_SMonitorDlg : public CDialog
{
// Construction
public:
	CXMS_SMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_SMonitorDlg)
	enum { IDD = IDD_XMS_SMONITOR_DIALOG };
	CComboBox	m_RawLine;
	CListCtrl	m_RawInfo;
	CButton	m_BtnStart;
	CEdit	m_Port;
	CEdit	m_IPAddress;
	CListCtrl	m_DeviceInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_SMonitorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_SMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonClear();
	afx_msg void OnEditchangeComboLine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_SMONITORDLG_H__7114E36E_472F_4D88_A33C_4E45C2A520FA__INCLUDED_)
