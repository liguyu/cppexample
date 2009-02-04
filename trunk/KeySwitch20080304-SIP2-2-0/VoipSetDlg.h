#if !defined(AFX_VOIPSETDLG_H__BA7CF082_6B21_4BA5_AF3B_DD31AC75E47E__INCLUDED_)
#define AFX_VOIPSETDLG_H__BA7CF082_6B21_4BA5_AF3B_DD31AC75E47E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VoipSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVoipSetDlg dialog

class CVoipSetDlg : public CDialog
{
// Construction
public:
	CVoipSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVoipSetDlg)
	enum { IDD = IDD_VOIP_SET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVoipSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVoipSetDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOIPSETDLG_H__BA7CF082_6B21_4BA5_AF3B_DD31AC75E47E__INCLUDED_)
