// KGEventViewDoc.h : interface of the CKGEventViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KGEVENTVIEWDOC_H__FA9DA5FC_B2C5_48AE_B83D_C58503DD3881__INCLUDED_)
#define AFX_KGEVENTVIEWDOC_H__FA9DA5FC_B2C5_48AE_B83D_C58503DD3881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MainFrm.h"

class CKGEventViewDoc : public CDocument
{
protected: // create from serialization only
	CKGEventViewDoc();
	DECLARE_DYNCREATE(CKGEventViewDoc)

// Attributes
public:
    CMainFrame *pFram;
// Operations
public:
      
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKGEventViewDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKGEventViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CKGEventViewDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KGEVENTVIEWDOC_H__FA9DA5FC_B2C5_48AE_B83D_C58503DD3881__INCLUDED_)
