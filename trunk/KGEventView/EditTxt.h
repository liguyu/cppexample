#if !defined(AFX_EDITTXT_H__9B813004_164F_4E9D_8645_FF0A079A4449__INCLUDED_)
#define AFX_EDITTXT_H__9B813004_164F_4E9D_8645_FF0A079A4449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditTxt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditTxt view

class EditTxt : public CEditView
{
protected:
	EditTxt();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(EditTxt)

// Attributes
public:

	CEdit   * pEdit;
// Operations
public:
	void SetText(CString msg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditTxt)
	public:
	virtual void OnFinalRelease();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~EditTxt();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(EditTxt)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(EditTxt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITTXT_H__9B813004_164F_4E9D_8645_FF0A079A4449__INCLUDED_)
