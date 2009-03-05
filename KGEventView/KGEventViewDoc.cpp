// KGEventViewDoc.cpp : implementation of the CKGEventViewDoc class
//

#include "stdafx.h"
#include "KGEventView.h"
#include "KGEventViewDoc.h"
#include "SAStatusLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewDoc

IMPLEMENT_DYNCREATE(CKGEventViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CKGEventViewDoc, CDocument)
	//{{AFX_MSG_MAP(CKGEventViewDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewDoc construction/destruction

CKGEventViewDoc::CKGEventViewDoc()
{
	// TODO: add one-time construction code here

}

CKGEventViewDoc::~CKGEventViewDoc()
{
}

//BOOL CKGEventViewDoc::OnNewDocument()
//{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
//
//	// TODO: add reinitialization code here
//	// (SDI documents will reuse this document)
//
//	return TRUE;
// }



/////////////////////////////////////////////////////////////////////////////
// CKGEventViewDoc serialization

void CKGEventViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewDoc diagnostics

#ifdef _DEBUG
void CKGEventViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKGEventViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewDoc commands

void CKGEventViewDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	CString   strFile(COleDateTime::GetCurrentTime().Format\
		( "%Y.%m.%d-%H.%M.%S.txt "));
	AfxMessageBox("on save!");

//    FilePath=strFile;
// 	g_statusLog.Init(FilePath);
// 	g_statusLog.Enable(TRUE);

}

void CKGEventViewDoc::OnFileSaveAs() 
{

	CString   strFile(COleDateTime::GetCurrentTime().Format( "%Y.%m.%d-%H.%M.%S.txt ")); 
	CFileDialog     filedlg(false,   
	"txt ",   
	strFile,   
	OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,   "bitmap   file   (*.txt)   |*.txt|   | "); 
	strFile= "保存文件: "+strFile; 
    filedlg.m_ofn.lpstrTitle   =   strFile;                         
    filedlg.m_ofn.lpstrInitialDir   =   "e:\\ ";//初始路径 
    filedlg.m_ofn.nMaxFile   =   MAX_PATH; 

    if(filedlg.DoModal()==IDCANCEL) 
                 return;
	CString name=filedlg.GetPathName();

//	FilePath=name;
// 	g_statusLog.InitPath(FilePath);
// 	g_statusLog.Enable(TRUE);
}

BOOL CKGEventViewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	

	// TODO: Add your specialized creation code here
 	pFram= (CMainFrame*)AfxGetMainWnd();
 	if( !pFram->m_bMonitor )
	{
       pFram->OpenFile(lpszPathName);
	   UpdateAllViews(NULL, NULL, this);
	}
	return FALSE;
}
