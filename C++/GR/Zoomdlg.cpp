// Zoomdlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "Zoomdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg dialog


Zoomdlg::Zoomdlg(CWnd* pParent /*=NULL*/)
	: CDialog(Zoomdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Zoomdlg)
	m_newHeight = 0;
	m_newWidth = 0;
	m_interpolationSelect = 0;
	//}}AFX_DATA_INIT
}


void Zoomdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Zoomdlg)
	DDX_Text(pDX, IDC_Edit_newH, m_newHeight);
	DDX_Text(pDX, IDC_Edit_newW, m_newWidth);
	DDX_Radio(pDX, IDC_RADIO1, m_interpolationSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Zoomdlg, CDialog)
	//{{AFX_MSG_MAP(Zoomdlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg message handlers

BOOL Zoomdlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	//��ԭʼͼ��ĸ���ʾ��IDΪIDC_Static_OldH�ľ�̬����
	char szStr[50];    
    sprintf(szStr, "ԭͼ��:%4d ����",m_oldHeight);            
    SetDlgItemText(IDC_Static_OldH, szStr);

	//��ԭʼͼ��Ŀ���ʾ��IDΪIDC_Static_OldW�ľ�̬����
	sprintf(szStr, "ԭͼ��:%4d ����",m_oldWidth);            
    SetDlgItemText(IDC_Static_OldW, szStr);  

	//��ͼ��Ĵ�С��ʼֵ����Ϊԭͼ��Ĵ�С
	m_newHeight=m_oldHeight;
	m_newWidth=m_oldWidth;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
