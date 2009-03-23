#if !defined(AFX_XMSAPI_RECORDSET_H__154FBA91_AE34_46D4_9BDB_50F35A74EF25__INCLUDED_)
#define AFX_XMSAPI_RECORDSET_H__154FBA91_AE34_46D4_9BDB_50F35A74EF25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_RecordSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_RecordSet dialog

class CXMSApi_RecordSet : public CDialog
{
// Construction
public:
	void HandleEnableDisable();
	CXMSApi_RecordSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_RecordSet)
	enum { IDD = IDD_DIALOG_RECORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_RecordSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_RecordSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnRADIONormalStop();
	afx_msg void OnRADIOSingleCode();
	afx_msg void OnRADIOAnyCode();
	virtual void OnOK();
	afx_msg void OnCHECKs8MixEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_RECORDSET_H__154FBA91_AE34_46D4_9BDB_50F35A74EF25__INCLUDED_)
