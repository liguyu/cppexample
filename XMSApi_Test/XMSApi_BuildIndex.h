#if !defined(AFX_XMSAPI_BUILDINDEX_H__C3DD3C05_C01E_4ADF_9BBB_F831AB8306F3__INCLUDED_)
#define AFX_XMSAPI_BUILDINDEX_H__C3DD3C05_C01E_4ADF_9BBB_F831AB8306F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_BuildIndex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_BuildIndex dialog

class CXMSApi_BuildIndex : public CDialog
{
// Construction
public:
	void GetNowSetVal(void);
	CXMSApi_BuildIndex(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_BuildIndex)
	enum { IDD = IDD_DIALOG_BuildPlayIndex };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_BuildIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_BuildIndex)
	afx_msg void OnBUTTONSelectBuildIndex();
	afx_msg void OnBUTTONBuild();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_BUILDINDEX_H__C3DD3C05_C01E_4ADF_9BBB_F831AB8306F3__INCLUDED_)
