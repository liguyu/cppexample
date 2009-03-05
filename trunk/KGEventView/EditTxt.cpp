// EditTxt.cpp : implementation file
//

#include "stdafx.h"
#include "KGEventView.h"
#include "EditTxt.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditTxt

IMPLEMENT_DYNCREATE(EditTxt, CEditView)

EditTxt::EditTxt()
{
// 	CMainFrame *pFrm=(CMainFrame *)AfxGetMainWnd();
// 	pFrm->m_edit=(EditTxt*)this;

	EnableAutomation();
}

EditTxt::~EditTxt()
{
}

void EditTxt::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CEditView::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(EditTxt, CEditView)
	//{{AFX_MSG_MAP(EditTxt)
	ON_WM_CHAR()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(EditTxt, CEditView)
	//{{AFX_DISPATCH_MAP(EditTxt)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IEditTxt to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {EAA40242-A9C5-4936-A441-81A7B1B97BB7}
static const IID IID_IEditTxt =
{ 0xeaa40242, 0xa9c5, 0x4936, { 0xa4, 0x41, 0x81, 0xa7, 0xb1, 0xb9, 0x7b, 0xb7 } };

BEGIN_INTERFACE_MAP(EditTxt, CEditView)
	INTERFACE_PART(EditTxt, IID_IEditTxt, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditTxt drawing

void EditTxt::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

/////////////////////////////////////////////////////////////////////////////
// EditTxt diagnostics

#ifdef _DEBUG
void EditTxt::AssertValid() const
{
	CEditView::AssertValid();
}

void EditTxt::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// EditTxt message handlers

BOOL EditTxt::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{

	// TODO: Add your specialized code here and/or call the base class
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void EditTxt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
//	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

void EditTxt::OnDestroy() 
{
	
}

void EditTxt::SetText(CString msg)
{
    //	pEdit->EnableWindow(false);  //禁止用户输入
    pEdit->SetWindowText(msg);
}

void EditTxt::OnInitialUpdate() 
{
	CFont * f; 
	CEdit  &   n_edit=GetEditCtrl();

	CEditView::OnInitialUpdate();
	CMainFrame *pFrm=(CMainFrame *)AfxGetMainWnd();
	pFrm->m_edit=(EditTxt*)this;
	// TODO: Add your specialized code here and/or call the base class
	
	pEdit=&n_edit;
	
    //设置字体
	f = new CFont; 
	f->CreateFont(15, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		0, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Arial")); // lpszFac 
    pEdit->SetFont(f); //IDC_EDIT_RECEIVE是Edit框标号
}

void EditTxt::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void EditTxt::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CEditView::OnKeyUp(nChar, nRepCnt, nFlags);
}
