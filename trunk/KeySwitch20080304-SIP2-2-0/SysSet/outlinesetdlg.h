#if !defined(AFX_OUTLINESETDLG_H__39414089_754D_4F64_AF96_C39B930DF040__INCLUDED_)
#define AFX_OUTLINESETDLG_H__39414089_754D_4F64_AF96_C39B930DF040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// outlinesetdlg.h : header file
//

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"


/////////////////////////////////////////////////////////////////////////////
// COutLineSetDlg dialog

class COutLineSetDlg : public CDialog
{
// Construction
public:
	DJ_S32	mDspNo;						
	long	mDeviceNo;
	bool	pResult;

	COutLineSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COutLineSetDlg)
	enum { IDD = IDD_OUT_LINE_SET };
	CComboBox	m_Enable;
	CEdit	m_LineNo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutLineSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutLineSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	
	/*�Զ��庯��*/
	bool	ReadLineInfo();
	bool	SaveLineInfo();
	bool	VerifyData();
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLINESETDLG_H__39414089_754D_4F64_AF96_C39B930DF040__INCLUDED_)
