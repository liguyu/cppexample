#if !defined(AFX_DIALOGFAXHEADER_H__154CA004_821E_4891_BA5A_A0F35B8336C7__INCLUDED_)
#define AFX_DIALOGFAXHEADER_H__154CA004_821E_4891_BA5A_A0F35B8336C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogFaxHeader.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFaxHeader dialog

class CDialogFaxHeader : public CDialog
{
// Construction
public:
	CDialogFaxHeader(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogFaxHeader)
	enum { IDD = IDD_DIALOG_PARAM_FAXHEADER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogFaxHeader)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogFaxHeader)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGFAXHEADER_H__154CA004_821E_4891_BA5A_A0F35B8336C7__INCLUDED_)
