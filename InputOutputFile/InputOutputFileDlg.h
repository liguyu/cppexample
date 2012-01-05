// InputOutputFileDlg.h : header file
//

#if !defined(AFX_INPUTOUTPUTFILEDLG_H__907880E6_235E_4454_AA38_EAFEDB1F7864__INCLUDED_)
#define AFX_INPUTOUTPUTFILEDLG_H__907880E6_235E_4454_AA38_EAFEDB1F7864__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInputOutputFileDlg dialog

class CInputOutputFileDlg : public CDialog
{
// Construction
public:
	CInputOutputFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInputOutputFileDlg)
	enum { IDD = IDD_INPUTOUTPUTFILE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputOutputFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInputOutputFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInput();
	afx_msg void OnButtonOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTOUTPUTFILEDLG_H__907880E6_235E_4454_AA38_EAFEDB1F7864__INCLUDED_)
