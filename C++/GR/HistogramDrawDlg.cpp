// HistogramDrawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "HistogramDrawDlg.h"
#include "ImageCenterDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg dialog


HistogramDrawDlg::HistogramDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(HistogramDrawDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HistogramDrawDlg)
	//��ʼ����ɫͨ��Ϊ������Ϣ
	m_colorSelect = 3;
	//}}AFX_DATA_INIT
}


void HistogramDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HistogramDrawDlg)
	DDX_CBIndex(pDX, IDC_COMBO_colorSelect, m_colorSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HistogramDrawDlg, CDialog)
	//{{AFX_MSG_MAP(HistogramDrawDlg)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_colorSelect, OnSelchangeCOMBOcolorSelect)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HistogramDrawDlg message handlers
//�Ի����ʼ��
BOOL HistogramDrawDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//����̬��1����rectָ��λ�ã�������ֱ��ͼ
	CRect rect;
	rect=CRect(25,51,279,151);
	GetDlgItem(IDC_STATIC_HistRect)->MoveWindow(&rect);

	//����Ͽ�����rectָ����λ��
	rect=CRect(14,15,293,200);
	GetDlgItem(IDC_STATIC_ZHK)->MoveWindow(&rect);

	//���б������rectָ����λ��
	rect=CRect(70,15,200,115);
	GetDlgItem(IDC_COMBO_colorSelect)->MoveWindow(&rect);

	//����ɫ��ʾ������ָ��λ��
	rect=CRect(25,155,279,168);
	GetDlgItem(IDC_STATIC_BmpBar)->MoveWindow(&rect);

	//����ֵ��̬������ָ��λ��
	rect=CRect(29,176,130,193);
	GetDlgItem(IDC_STATIC_Average)->MoveWindow(&rect);

	//�����̬������ָ��λ��
	rect=CRect(180,176,279,193);
	GetDlgItem(IDC_STATIC_Deviation)->MoveWindow(&rect);

	//��ȷ����ť����ָ��λ��
	rect=CRect(221,210,277,234);
	GetDlgItem(IDOK)->MoveWindow(&rect);

	// ����ǻҶ�ͼ������ûҶ�ֱ��ͼͳ�ƺ���
	if(m_histogramDib.m_nBitCount==8)
    	m_histogramDib.computeHistGray();
	else//��ɫͼ����ͳ������ֱ��ͼ
		m_histogramDib.computeHistBrightness();

	//����ֱ��ͼ�ľ�ֵ�ͷ���
	m_histogramDib.computeAverage();
	m_histogramDib.computeDeviation();

	//����ֵ��Ϣ��ʾ�ھ�ֵ��̬����
	char szStr[10];    
    sprintf(szStr, "��ֵ:%.2f",m_histogramDib.m_average );            
    SetDlgItemText(IDC_STATIC_Average, szStr);         
	
	//��������Ϣ��ʾ�ڷ��̬����
    sprintf(szStr, "����:%.2f", m_histogramDib.m_deviation);            
    SetDlgItemText(IDC_STATIC_Deviation, szStr); 

	return TRUE;  
}

void HistogramDrawDlg::OnPaint() 
{
	//�豸��������
	CPaintDC dc(this); // device context for painting

	//��ɫ�ͻҶ�ͼ����Ч
	if(m_histogramDib.m_nBitCount != 8 && m_histogramDib.m_nBitCount != 24) 
		return ;

	//��ȡֱ��ͼ���ƾ�̬��ľ�������
 	CRect clientRect;  
	GetDlgItem(IDC_STATIC_HistRect)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);

	BeginWaitCursor();	    // Draw Back Ground
	//������
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(clientRect.left-1, clientRect.top,
                          clientRect.right+3, clientRect.bottom+1);
	
	//��ֱ��ͼ�Ҷ�Ƶ����ֱ��ͼ���ƾ������������ϵ����ֱ��ͼ
	unsigned int max=0;;
	for(int i=0;i<256;i++){
		if(m_histogramDib.m_histArray[i]>max)
			max=m_histogramDib.m_histArray[i];
	}
	DWORD bufTemp[256];
	double x=clientRect.Height();
	for(i=0;i<256;i++){
		bufTemp[i]=(unsigned long)((float)m_histogramDib.m_histArray[i]*x/max);
	}
	for(i=clientRect.left;i<=clientRect.right;i++){
		dc.MoveTo(i,clientRect.bottom);
		dc.LineTo(i,clientRect.bottom-bufTemp[i-clientRect.left]);
	}   

 	EndWaitCursor();
}

void HistogramDrawDlg::OnSelchangeCOMBOcolorSelect() 
{
	//�ú��������û�ѡ��ֱ��ͼ��ɫͨ����ͬ��������Ӧ����
	//���Ի����û����õ����ݴ���Ի�����
	UpdateData(1);
	if(m_histogramDib.m_nBitCount!=24){
		m_colorSelect=3;
		UpdateData(0);
		return;
	}

	//�����û���ѡ�������Ӧͨ����ֱ��ͼͳ�ƺ���
	if(m_colorSelect==0)
		m_histogramDib.computeHistBlue();
	else if(m_colorSelect==1)
		m_histogramDib.computeHistGreen();
	else if(m_colorSelect==2)
		m_histogramDib.computeHistRed();
	else if(m_colorSelect==3)
		m_histogramDib.computeHistBrightness();

	//����ֱ��ͼ�ľ�ֵ�ͷ���
	m_histogramDib.computeAverage();
	m_histogramDib.computeDeviation();

	//��ʾ��ֵ�ͷ�������Ӧ��̬����
	char szStr[10];    
    sprintf(szStr, "��ֵ:%.2f",m_histogramDib.m_average );            
    SetDlgItemText(IDC_STATIC_Average, szStr);            
    sprintf(szStr, "����:%.2f", m_histogramDib.m_deviation);            
    SetDlgItemText(IDC_STATIC_Deviation, szStr); 

	//ˢ��
	Invalidate();
}

void HistogramDrawDlg::OnSize(UINT nType, int cx, int cy) 
{
	//�ú����ڴ����Ի���ʱ���ã�ȷ��ֱ��ͼ�Ի���Ĵ�С���ͻ����е�λ��

	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	rect=CRect(405,261,717,545);
	MoveWindow(&rect);
}

void HistogramDrawDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
