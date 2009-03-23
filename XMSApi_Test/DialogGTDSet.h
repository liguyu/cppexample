#if !defined(AFX_DIALOGGTDSET_H__B753708C_3F95_4454_9157_4F33267A50D7__INCLUDED_)
#define AFX_DIALOGGTDSET_H__B753708C_3F95_4454_9157_4F33267A50D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogGTDSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGTDSet dialog

class CDialogGTDSet : public CDialog
{
// Construction
public:
	CDialogGTDSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogGTDSet)
	enum { IDD = IDD_DIALOG_GTDSET };
	CComboBox	m_ComboEnvelop;
	CTabCtrl	m_tab_gtd_tone;
	CTabCtrl	m_tab_gtd_freq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogGTDSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogGTDSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTabGtdFreq(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnClickTabGtdTone(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboEnvelop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGGTDSET_H__B753708C_3F95_4454_9157_4F33267A50D7__INCLUDED_)
