// KGEventViewView.h : interface of the CKGEventViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KGEVENTVIEWVIEW_H__E5253049_F50E_470B_871F_2ACCC0059E1E__INCLUDED_)
#define AFX_KGEVENTVIEWVIEW_H__E5253049_F50E_470B_871F_2ACCC0059E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CKGEventViewView : public CView
{
protected: // create from serialization only
	CKGEventViewView();
	DECLARE_DYNCREATE(CKGEventViewView)

// Attributes
public:
	CKGEventViewDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKGEventViewView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKGEventViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CKGEventViewView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in KGEventViewView.cpp
inline CKGEventViewDoc* CKGEventViewView::GetDocument()
   { return (CKGEventViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KGEVENTVIEWVIEW_H__E5253049_F50E_470B_871F_2ACCC0059E1E__INCLUDED_)
