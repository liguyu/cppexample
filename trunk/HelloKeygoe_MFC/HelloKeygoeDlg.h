// HelloKeygoeDlg.h : header file
//

#if !defined(AFX_HELLOKEYGOEDLG_H__4E36CDAB_26C7_42BE_984F_1F591403F99C__INCLUDED_)
#define AFX_HELLOKEYGOEDLG_H__4E36CDAB_26C7_42BE_984F_1F591403F99C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHelloKeygoeDlg dialog

class CHelloKeygoeDlg : public CDialog
{
// Construction
public:
	CHelloKeygoeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHelloKeygoeDlg)
	enum { IDD = IDD_HELLOKEYGOE_DIALOG };
	CListCtrl	m_ListTrunk;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloKeygoeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHelloKeygoeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOKEYGOEDLG_H__4E36CDAB_26C7_42BE_984F_1F591403F99C__INCLUDED_)
