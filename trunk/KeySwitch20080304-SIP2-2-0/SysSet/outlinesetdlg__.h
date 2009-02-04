#if !defined(AFX_OUTLINESETDLG_H__39414089_754D_4F64_AF96_C39B930DF040__INCLUDED_)
#define AFX_OUTLINESETDLG_H__39414089_754D_4F64_AF96_C39B930DF040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// outlinesetdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutLineSetDlg dialog


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

class COutLineSetDlg : public CDialog
{
// Construction
public:
	COutLineSetDlg(CWnd* pParent = NULL);   // standard constructor
	DJ_S32	mDspNo;						
	long	mDeviceNo;
	bool	pResult;

// Dialog Data
	//{{AFX_DATA(COutLineSetDlg)
	enum { IDD = IDD_OUT_LINE_SET };
	CEdit	m_LineNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(COutLineSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	/*自定义函数*/
	bool	ReadLineInfo();
	bool	SaveLineInfo();
	bool	VerifyData();

	// Generated message map functions
	//{{AFX_MSG(COutLineSetDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLINESETDLG_H__39414089_754D_4F64_AF96_C39B930DF040__INCLUDED_)
