// demoDoc.cpp : implementation of the CDemoDoc class
//

#include "stdafx.h"
#include "demo.h"

#include "demoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc

IMPLEMENT_DYNCREATE(CDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction

CDemoDoc::CDemoDoc()
{
	avi = NULL;
	isAvi = false;
}

CDemoDoc::~CDemoDoc()
{
	if(avi!= NULL)
		delete avi;
}

BOOL CDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemoDoc serialization

void CDemoDoc::Serialize(CArchive& ar)
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
// CDemoDoc diagnostics

#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands

BOOL CDemoDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if(m_dib.Write(lpszPathName) == TRUE)	{
		SetModifiedFlag(FALSE);     // have saved
		return TRUE;
	}
	else 
		return 0;
}

BOOL CDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{

    if (m_dib.Read(lpszPathName) == TRUE) {	
		SetModifiedFlag(FALSE);     // start off with unmodified
		return TRUE;
	}
	else 
		return 0;
	

}

