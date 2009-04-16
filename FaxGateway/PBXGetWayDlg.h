// PBXGetWayDlg.h : header file
//

#if !defined(AFX_PBXGETWAYDLG_H__DF2C1C1D_7931_41AF_8134_2380632CBD57__INCLUDED_)
#define AFX_PBXGETWAYDLG_H__DF2C1C1D_7931_41AF_8134_2380632CBD57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPBXGetWayDlg dialog

class CPBXGetWayDlg : public CDialog
{
// Construction
public:
	CPBXGetWayDlg(CWnd* pParent = NULL);	// standard constructor
	int ConvertTrunkChnInt(int *DspID, int *ChnID, char *ChStr);
	int ConvertVoiceChnInt(int *DspID, int *ChnID, int iDspID);
	int ConvertVoipChnInt(int *DspID, int *ChnID, char *ChStr);

// Dialog Data
	//{{AFX_DATA(CPBXGetWayDlg)
	enum { IDD = IDD_PBXGETWAY_DIALOG };
	CListCtrl	m_ListVOIP;
	CListCtrl	m_ListDTrunk;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPBXGetWayDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPBXGetWayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONRecord();
	afx_msg void OnDblclkVoip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDTrunk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBXGETWAYDLG_H__DF2C1C1D_7931_41AF_8134_2380632CBD57__INCLUDED_)
