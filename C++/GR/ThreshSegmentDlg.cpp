// ThreshSegmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "ThreshSegmentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ThreshSegmentDlg dialog


ThreshSegmentDlg::ThreshSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ThreshSegmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ThreshSegmentDlg)
	//��ʼֱ��ͼΪ����ͳ��ֱ��ͼ
	m_colorSelect = 3;
	//��ʼ��ֵΪ128
	m_threshSelect = 128;
	//}}AFX_DATA_INIT
}


void ThreshSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ThreshSegmentDlg)
	DDX_CBIndex(pDX, IDC_COMBO_colorSelect, m_colorSelect);
	DDX_Slider(pDX, IDC_Slider_ThreshSelect, m_threshSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ThreshSegmentDlg, CDialog)
	//{{AFX_MSG_MAP(ThreshSegmentDlg)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_colorSelect, OnSelchangeCOMBOcolorSelect)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ThreshSegmentDlg message handlers

BOOL ThreshSegmentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rect;
	//ָ��ֱ��ͼ���������ڶԻ����е�λ��
	rect=CRect(25,51,279,151);
	GetDlgItem(IDC_STATIC_HistRect)->MoveWindow(&rect);

	//ָ����Ͽ��ڶԻ����е�λ��
	rect=CRect(14,15,293,200);
	GetDlgItem(IDC_STATIC_ZHK)->MoveWindow(&rect);

	//ָ����ɫѡ����Ͽ��ڶԻ����е�λ��
	rect=CRect(70,15,200,115);
	GetDlgItem(IDC_COMBO_colorSelect)->MoveWindow(&rect);

	//ָ����ɫ���ڶԻ����е�λ��
	rect=CRect(25,155,279,168);
	GetDlgItem(IDC_STATIC_BmpBar)->MoveWindow(&rect);

	//ָ��ֱ��ͼ�ҶȾ�ֵͳ�ƾ�̬�ı����ڶԻ����е�λ��
	rect=CRect(29,176,130,193);
	GetDlgItem(IDC_STATIC_Average)->MoveWindow(&rect);

	//ָ��ֱ��ͼ����ͳ�ƾ�̬�ı����ڶԻ����е�λ��
	rect=CRect(180,176,279,193);
	GetDlgItem(IDC_STATIC_Deviation)->MoveWindow(&rect);


	//ָ����ֵѡ�񻬸����ڶԻ����е�λ��
	rect=CRect(20,205,285,230);
	GetDlgItem(IDC_Slider_ThreshSelect)->MoveWindow(&rect);

	rect=CRect(280,213,310,230);
	GetDlgItem(IDC_STATIC_thresh)->MoveWindow(&rect);

	//ָ����ȷ������ť�ڶԻ����е�λ��
	rect=CRect(120,250,170,274);
	GetDlgItem(IDOK)->MoveWindow(&rect);

	//ָ����ȡ������ť�ڶԻ����е�λ��
	rect=CRect(200,250,250,274);
	GetDlgItem(IDCANCEL)->MoveWindow(&rect);

	// TODO: Add extra initialization here
	//����ǻҶ�ͼ�������computeHistGrey()����ͳ��ֱ��ͼ��
	//������ڲ�ɫͼ��Ҫ��������ֱ��ͼ
	if(m_histogramDib.m_nBitCount==8)
    	m_histogramDib.computeHistGray();
	else
		m_histogramDib.computeHistBrightness();
	m_histogramDib.computeAverage();
	m_histogramDib.computeDeviation();

	char szStr[10];    
    sprintf(szStr, "��ֵ:%.2f",m_histogramDib.m_average );            
    SetDlgItemText(IDC_STATIC_Average, szStr);            
    sprintf(szStr, "����:%.2f", m_histogramDib.m_deviation);            
    SetDlgItemText(IDC_STATIC_Deviation, szStr); 

	CSliderCtrl *pSlider=(CSliderCtrl *)GetDlgItem(IDC_Slider_ThreshSelect);
	pSlider->SetRange(0,255);
	pSlider->SetPos(m_threshSelect);
	sprintf(szStr, "%d",m_threshSelect);            
    SetDlgItemText(IDC_STATIC_thresh, szStr);            
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ThreshSegmentDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting

	if(m_histogramDib.m_nBitCount != 8 && m_histogramDib.m_nBitCount != 24) return ;
 	CRect clientRect;  // Set Draw Window

	GetDlgItem(IDC_STATIC_HistRect)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);

	BeginWaitCursor();	    // Draw Back Ground
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(clientRect.left-1, clientRect.top,
                          clientRect.right+3, clientRect.bottom+1);
	
	
	unsigned int max=0;
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

	//������ɫ����
	CPen *pBluePen=new CPen;
	pBluePen->CreatePen(PS_SOLID,1,RGB(0,0,255));

	//ѡ�е�ǰ��ɫ���ʣ�������ɵĻ���
	CGdiObject *pOldPen=dc.SelectObject(pBluePen);

	//��ֱ��ͼ�л�����ֵ��λ��
	dc.MoveTo(m_threshSelect+25,clientRect.bottom);
	dc.LineTo(m_threshSelect+25,clientRect.top);

	//�ָ��ɻ���
	dc.SelectObject(pOldPen);

 	EndWaitCursor();
}

void ThreshSegmentDlg::OnSelchangeCOMBOcolorSelect() 
{
	UpdateData(1);
	if(m_histogramDib.m_nBitCount!=24){
		m_colorSelect=3;
		UpdateData(0);
		return;
	}
	//�����ɫѡ�����Ϊ0���������ɫͨ��ֱ��ͼ
	if(m_colorSelect==0)
		m_histogramDib.computeHistBlue();
	else if(m_colorSelect==1)//��ɫͨ��ֱ��ͼ
		m_histogramDib.computeHistGreen();
	else if(m_colorSelect==2)
		m_histogramDib.computeHistRed();//��ɫͨ��ֱ��ͼ
	else if(m_colorSelect==3)
		m_histogramDib.computeHistBrightness();//����ֱ��ͼ
	m_histogramDib.computeAverage();//����ֱ��ͼ��ֵ
	m_histogramDib.computeDeviation();//����ֱ��ͼ����

	char szStr[10];    
    sprintf(szStr, "��ֵ:%.2f",m_histogramDib.m_average );            
    SetDlgItemText(IDC_STATIC_Average, szStr);            
    sprintf(szStr, "����:%.2f", m_histogramDib.m_deviation);            
    SetDlgItemText(IDC_STATIC_Deviation, szStr); 
	Invalidate();
	
}

void ThreshSegmentDlg::OnSize(UINT nType, int cx, int cy) 
{
	//������ֵ�ָ�Ի���Ĵ�С
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	rect=CRect(405,261,722,580);
	//���Ի�����������ָ��λ��
	MoveWindow(&rect);
}

void ThreshSegmentDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// ���ݻ�����λ�ø�����ֵ
	CSliderCtrl *pSlider=(CSliderCtrl *)pScrollBar;
	m_threshSelect=pSlider->GetPos();

	//�ھ�̬������ʾ��������ֵ
	char szStr[20];
	sprintf(szStr, "%d",m_threshSelect);            
    SetDlgItemText(IDC_STATIC_thresh, szStr);  
	//ˢ����ʾ
	Invalidate();
}
