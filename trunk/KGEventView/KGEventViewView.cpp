// KGEventViewView.cpp : implementation of the CKGEventViewView class
//

#include "stdafx.h"
#include "KGEventView.h"
#include "KGEventViewDoc.h"
#include "KGEventViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewView

IMPLEMENT_DYNCREATE(CKGEventViewView, CView)

BEGIN_MESSAGE_MAP(CKGEventViewView, CView)
	//{{AFX_MSG_MAP(CKGEventViewView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewView construction/destruction

CKGEventViewView::CKGEventViewView()
{
	// TODO: add construction code here

}

CKGEventViewView::~CKGEventViewView()
{
}

BOOL CKGEventViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewView drawing

void CKGEventViewView::OnDraw(CDC* pDC)
{
	CKGEventViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewView diagnostics

#ifdef _DEBUG
void CKGEventViewView::AssertValid() const
{
	CView::AssertValid();
}

void CKGEventViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKGEventViewDoc* CKGEventViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKGEventViewDoc)));
	return (CKGEventViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewView message handlers

