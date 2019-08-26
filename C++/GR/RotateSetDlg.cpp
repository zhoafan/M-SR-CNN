// RotateSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "RotateSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RotateSetDlg dialog


RotateSetDlg::RotateSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RotateSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RotateSetDlg)
	m_rotateAngle = 0;
	m_interpolationSelect = 0;
	//}}AFX_DATA_INIT
}


void RotateSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RotateSetDlg)
	DDX_Text(pDX, IDC_Edit_Angle, m_rotateAngle);
	DDX_Radio(pDX, IDC_RADIO1, m_interpolationSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RotateSetDlg, CDialog)
	//{{AFX_MSG_MAP(RotateSetDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RotateSetDlg message handlers
