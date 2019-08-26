  // demoView.cpp : implementation of the CDemoView class
//
#include "stdio.h"
#include "stdafx.h"
#include "demo.h"

#include "demoDoc.h"
#include "demoView.h"
#include"math.h"
#include "MainFrm.h"
#include "ImageCenterDib.h"//dib��ͷ�ļ�
#include "SpecialEffectShow.h"//ͼ�����Ч��ʾ��ͷ�ļ�
#include "GrayTrans.h"//�Ҷȱ任��
#include "linearStretchParamDlg.h"//��������������öԻ���
#include "histogram.h"//ֱ��ͼ��
#include "HistogramDrawDlg.h"//ֱ��ͼ���ƶԻ���
#include "GeometryTrans.h"//���α任��
#include "GeometryMoveDlg.h"//ƽ�����öԻ���
#include "FFT.h"//���ٸ���Ҷ�任��
#include "Morphology.h"//��̬ѧ������
#include "MaskDefineDlg.h"//ģ���Զ���Ի���
#include "ThreshSegmentDlg.h"//��ֵ�ָ�Ի���
#include "Segment.h"//ͼ��ָ���
#include "RotateSetDlg.h"//��ת���öԻ���
#include "Match.h"//ƥ����
#include "NonlinearStretchDlg.h"//����������(������ָ��)�������öԻ���
#include "RegionGrowDlg.h" //���������������öԻ���
#include "Watershed.h"//ˮ��ָ���
#include "WatershedParaDlg.h"//ˮ��ָ�������öԻ���
#include "ImageRestore.h"//�˶�ģ���ָ���
#include "ProjectRestoreDlg.h"//ͶӰ���˶�ģ���ָ����öԻ���
#include "WienterFilterDlg.h"//ά���˲����öԻ���
#include "InverseFilterDlg.h"//�����˲����öԻ���
#include "transeform.h"//С���任
#include "MedianDialog.h"//��ֵ�˲�ѡ��ģ��Ի���
#include "AvgDialog.h"//��ֵ�˲�ѡ��ģ��Ի���
#include "ImgEnhance.h"//ͼ����ǿ��
#include "transform_DCT.h"//��ɢ���ұ任
#include "Register.h"
#include "Zoomdlg.h"//�������öԻ���
#include "Register.h"
#include "PhaseCorre.h"

//ȫ�ֱ���
Transform_FFT FFTTrans;//����Ҷ�任��Ķ���
transeform_wavelet wltTrans;//С���任��Ķ���
transform_DCT DIBDCT;//DCT�任�����
ImgCenterDib DIB1;//��׼�㷨������ͼ��1
ImgCenterDib DIB2;//��׼�㷨������ͼ��2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoView

IMPLEMENT_DYNCREATE(CDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CDemoView, CScrollView)
	//{{AFX_MSG_MAP(CDemoView)
	ON_COMMAND(ID_SOBEL, OnSobel)
	ON_COMMAND(id_ImgReverse, OnImgReverse)
	ON_COMMAND(id_ColorToGray, OnColorToGray)
	ON_COMMAND(id_Binary, OnBinary)
	ON_COMMAND(id_GrayToColor, OnGrayToColor)
	ON_COMMAND(id_LinearStrecth, OnLinearStrecth)
	ON_COMMAND(id_HistgramAver, OnHistgramAver)
	ON_COMMAND(id_HistogramDraw, OnHistogramDraw)
	ON_COMMAND(ID_FFT, OnFft)
	ON_COMMAND(id_IFFT, OnIfft)
	ON_COMMAND(id_Clockwise90, OnClockwise90)
	ON_COMMAND(id_Anticlockwise90, OnAnticlockwise90)
	ON_COMMAND(id_Rotate180, OnRotate180)
	ON_COMMAND(id_Zoom, OnZoom)
	ON_COMMAND(id_Rotate, OnRotate)
	ON_COMMAND(id_ErosionForBinary, OnErosionForBinary)
	ON_COMMAND(ID__DilationForBinary, OnDilationForBinary)
	ON_COMMAND(id_OpenForBinary, OnOpenForBinary)
	ON_COMMAND(id_CloseForBinary, OnCloseForBinary)
	ON_COMMAND(id_InnerEdgeForBinary, OnInnerEdgeForBinary)
	ON_COMMAND(id_OuterEdgeForBinary, OnOuterEdgeForBinary)
	ON_COMMAND(id_HitMissThinning, OnHitMissThinning)
	ON_COMMAND(id_ErosionForGray, OnErosionForGray)
	ON_COMMAND(id_DilationForGray, OnDilationForGray)
	ON_COMMAND(id_OpenForGray, OnOpenForGray)
	ON_COMMAND(id_CloseForGray, OnCloseForGray)
	ON_COMMAND(id_MorphoGradientForGray, OnMorphoGradientForGray)
	ON_COMMAND(id_TopHatPeak, OnTopHatPeak)
	ON_COMMAND(id_TopHatVally, OnTopHatVally)
	ON_COMMAND(id_TopHatPeakAndVally, OnTopHatPeakAndVally)
	ON_COMMAND(id_InteractiveThresh, OnInteractiveThresh)
	ON_COMMAND(id_OtusThreshold, OnOtusThreshold)
	ON_COMMAND(id_Robert, OnRobert)
	ON_COMMAND(id_Prewitt, OnPrewitt)
	ON_COMMAND(id_EdgeSelectTemplete, OnEdgeSelectTemplete)
	ON_COMMAND(id_HoughTransform, OnHoughTransform)
	ON_COMMAND(id_LineDetectByHough, OnLineDetectByHough)
	ON_COMMAND(id_RegionGrow, OnRegionGrow)
	ON_COMMAND(id_ContourExtract, OnContourExtract)
	ON_COMMAND(id_Gauss_laplacian, OnGausslaplacian)
	ON_COMMAND(id_WatershedSegment, OnWatershedSegment)
	ON_COMMAND(id_DuiShuStretch, OnDuiShuStretch)
	ON_COMMAND(id_ZhiShuStretch, OnZhiShuStretch)
	ON_COMMAND(id_Move, OnMove)
	ON_COMMAND(id_ZhuanZhi, OnZhuanZhi)
	ON_COMMAND(id_HorizontalMirror, OnHorizontalMirror)
	ON_COMMAND(id_VerticalMirror, OnVerticalMirror)
	ON_COMMAND(id_FFTEx, OnFFTEx)
	ON_COMMAND(id_IFFTEx, OnIFFTEx)
	ON_COMMAND(id_Laplacian, OnLaplacian)
	ON_COMMAND(id_Krisch, OnKrisch)
	ON_COMMAND(id_ContourTrace, OnContourTrace)
	ON_COMMAND(id_MorphoContourForBinary, OnMorphoContourForBinary)
	ON_COMMAND(ID_SCAN, OnScan)
	ON_COMMAND(ID_SLIDE, OnSlide)
	ON_COMMAND(ID_FADEIN, OnFadeIn)
	ON_COMMAND(ID_MOSAIK, OnMosaik)
	ON_COMMAND(ID_ProjRestore, OnProjRestore)
	ON_COMMAND(ID_WienerFilter, OnWienerFilter)
	ON_COMMAND(ID_InverseFilter, OnInverseFilter)
	ON_COMMAND(ID_SET2ZERO_DENOISE, OnSet2zeroDenoise)
	ON_COMMAND(ID__HARDTHRESHOLD, OnHardthreshold)
	ON_COMMAND(ID_SOFTTHRESHOLD, OnSoftthreshold)
	ON_COMMAND(ID_PepperSaltNoise, OnPepperSaltNoise)
	ON_COMMAND(ID_Random, OnRandom)
	ON_COMMAND(ID_AveSmooth, OnAveSmooth)
	ON_COMMAND(id_MiddleSmooth, OnMiddleSmooth)
	ON_COMMAND(ID_LaplacienSharp, OnLaplacienSharp)
	ON_COMMAND(ID_CHOOSE_MASK, OnChooseMask)
	ON_COMMAND(ID_POWER_SMOOTH, OnPowerSmooth)
	ON_COMMAND(id_HarrWaveletTrans, OnHarrWaveletTrans)
	ON_COMMAND(id_HarrWaveletITrans, OnHarrWaveletITrans)
	ON_COMMAND(ID_DCT, OnDct)
	ON_COMMAND(ID_GRADESHARP, OnGradesharp)
	ON_COMMAND(ID_SNR, OnSnr)
	ON_COMMAND(id_IDCT, OnIdct)
	ON_COMMAND(ID_SECONDIMAGE, OnSecondimage)
	ON_COMMAND(ID_FIRSTIMAGE, OnFirstimage)
	ON_COMMAND(id_MatchConner, OnMatchConner)
	ON_COMMAND(id_MatchTemplet, OnMatchTemplet)
	ON_COMMAND(id_ConnerDetect, OnConnerDetect)
	ON_COMMAND(ID_MOTION_FOPEN1, OnMotionFopen1)
	ON_COMMAND(ID_MOTION_FOPEN2, OnMotionFopen2)
	ON_COMMAND(ID_MOTION_FEST, OnMotionFest)
	ON_COMMAND(ID_MITON_DEST, OnMitonDest)
	ON_COMMAND(ID_MENUITEM32881, OnMenuitem32881)
	ON_COMMAND(ID_MENUITEM32882, OnMenuitem32882)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoView construction/destruction

CDemoView::CDemoView()
{

}

CDemoView::~CDemoView()
{

}

BOOL CDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoView drawing

void CDemoView::OnDraw(CDC* pDC)
{

	//��ȡ�ĵ���ָ��
	CDemoDoc* pDoc = GetDocument();

	if(!pDoc->isAvi){
		//����m_dib��ָ��
		ImgCenterDib *pDib=pDoc->GetPDib();

		//��ȡDIB�ĳߴ�
		CSize sizeFileDib = pDib->GetDimensions();

		//��ʾDIB
		pDib->Draw(pDC, CPoint(0, 0), sizeFileDib);

	}
	else
	{
		
	}
}

void CDemoView::OnInitialUpdate()
{

	CScrollView::OnInitialUpdate();

	//��ȡ�ĵ���ָ��
	CDemoDoc* pDoc = GetDocument();

	//��ȡDIB��ָ��
	ImgCenterDib *pDib=pDoc->GetPDib();

	//����DIB�ߴ������Ӵ���С
	if(pDib!=NULL)
    	SetScrollSizes(MM_TEXT, pDib->GetDimensions());
	else{
		//���m_dibΪ�գ�������һ���̶��ĳߴ�
		CSize sizeTotal;
		sizeTotal.cx = sizeTotal.cy = 100;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CDemoView printing

BOOL CDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDemoView diagnostics

#ifdef _DEBUG
void CDemoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDemoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDemoDoc* CDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoView message handlers



void CDemoView::OnImgReverse() 
{
	//��ȡ�ĵ���ָ��
	CDemoDoc *pDoc=GetDocument();

	//��ȡImgCenterDib������ָ�룬���ʵ�ǰDIB����
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//DIB�����ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	GrayTrans graytrans(pDib->GetDimensions(),
		pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);

	//����ReverseImg()��ͼ����
	graytrans.ReverseImg();
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

	//�����½��ļ�����Ϣ������һ���µ��ĵ�-��ͼ
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//��ȡ�½���ͼָ��
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	

	//��ȡ��������µ��ĵ���ָ��
	CDemoDoc* pDocNew=pView->GetDocument();

	//��ȡ���ĵ��е�ImgCenterDib�����ָ��
	ImgCenterDib *dibNew=pDocNew->GetPDib();

	//���任������ͼ����Ϊ�½��ĵ���DIB������ʾ
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	
	//���ù�������
	pView->OnInitialUpdate();

	//�ĵ��������࣬��ʾ������Ϣ
	pDocNew->SetModifiedFlag(TRUE);

	//����ͼˢ����ʾ
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnColorToGray() 
{
	//��ȡ�ĵ���ָ��
	CDemoDoc *pDoc=GetDocument();
	//��ȡImgCenterDib�����m_dib��ָ�룬���ʵ�ǰDIB����
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ�����ɫͼ��
	if(pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ�����ɫͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//��ɫ��Ҷȸ�ʽ
	graytrans.ColorToGray();
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//�����½��ļ�����Ϣ������һ���µ��ĵ�-��ͼ
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//��ȡ�½���ͼָ��
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	
	//��ȡ��������µ��ĵ���ָ��
	CDemoDoc* pDocNew=pView->GetDocument();

	//��ȡ���ĵ��е�ImgCenterDib�����ָ��
	ImgCenterDib *dibNew=pDocNew->GetPDib();

	//���任������ͼ����Ϊ�½��ĵ���DIB������ʾ
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	
	//���ù�������
	pView->OnInitialUpdate();


	//�ĵ��������࣬��ʾ������Ϣ
	pDocNew->SetModifiedFlag(TRUE);

	//����ͼˢ����ʾ
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//����Binary()��ͼ����ж�ֵ����ȱʡ״̬����ֵΪ
	graytrans.Binary();
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnGrayToColor() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//�Ҷȱ��ɫ��ʽ
	graytrans.GrayToColor();
	
	//����һ������ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnLinearStrecth() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}

	//��������������öԻ���
	linearStretchParamDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		//��ȡ�û����õ���������ת�۵�
		CPoint point[2];
		point[0].x=dlgPara.m_firstPointX;
		point[0].y=dlgPara.m_firstPointY;
		point[1].x=dlgPara.m_secondPointX;
		point[1].y=dlgPara.m_secondPointY;

		//�жϲ����ĺ�����
		if(point[0].x<0||point[0].x>255||point[1].x<0||point[1].x>255
			||point[0].y<0||point[0].y>255||point[1].y<0||point[1].y>255
			||point[0].x>point[1].x){
			::MessageBox(0,"����������",MB_OK,0);
			return;
		}

		//����Ҷȱ任�������
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);

		//��������
		graytrans.LinearStrech(point);
		
		//�½�������ʾ
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);		
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();	
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}


void CDemoView::OnDuiShuStretch() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}
	
	NonlinearStretchDlg dlgPara;
	//�������������ʼ��
	dlgPara.m_a=0;
	dlgPara.m_b=(float)0.033;
	dlgPara.m_c=(float)2;
	if(dlgPara.DoModal()==IDOK){
		if(dlgPara.m_b==0||dlgPara.m_c==1){
			::MessageBox(0,"b����Ϊ0����c����Ϊ1",MB_OK,0);
			return;
		}
		//����Ҷȱ任�������
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//���ݶ���������������з���������
		graytrans.DuiShuStrech(dlgPara.m_a, dlgPara.m_b, dlgPara.m_c);
		
		//�½�������ʾ
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);		
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();	
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnZhiShuStretch() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
		return ;
	}
	
	NonlinearStretchDlg dlgPara;
	//ָ�����������ʼ��
	dlgPara.m_a=(float)128;
	dlgPara.m_b=2;
	dlgPara.m_c=(float)0.6;
	if(dlgPara.DoModal()==IDOK){
		//����Ҷȱ任�������
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//����ָ��������������з���������
		graytrans.ZhiShuStrech(dlgPara.m_a, dlgPara.m_b, dlgPara.m_c);
		
		//�½�������ʾ
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);		
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnHistgramAver() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����Histogram��Ķ���hist
	Histogram hist(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//ֱ��ͼ����
	hist.histAve();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(hist.GetDimensions(),hist.m_nBitCount,hist.m_lpColorTable, hist.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnHistogramDraw() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//�쳣�ж�
  if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
	  ::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
	  return ;
  }

  //ֱ��ͼ���ƶԻ���
  HistogramDrawDlg dlgHist;
  dlgHist.m_histogramDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
	  pDib->m_lpColorTable, pDib->m_pImgData);

  //��ʾֱ��ͼ
  dlgHist.DoModal();
}



void CDemoView::OnFft() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж�ͼ���С�Ƿ�Ϊ2���ݴεķ���
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//ͼ�����2���ݴη�
		isValidateFlag=0;

	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//ͼ��߲���2���ݴη�
		isValidateFlag=0;

	if(isValidateFlag==0){
		::MessageBox(0,"����ͼ��Ŀ�͸�ӦΪ2���ݴη�",MB_OK,0);
		return ;
	}
	
	//����ͼ�����ݸ�transform_FFT�����FFTTrans
	FFTTrans.InputImgData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//���ٸ���Ҷ�任
	FFTTrans.Fourier();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(FFTTrans.GetDimensions(),FFTTrans.m_nBitCountOut,
		FFTTrans.m_lpColorTable, FFTTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CDemoView::OnIfft() 
{
	//����Ҷ���任
	FFTTrans.IFourier();
	
    //�½���ͼ���ڣ���ʾͼ��ָ��Ľ��
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(FFTTrans.GetDimensions(),FFTTrans.m_nBitCountOut,
		FFTTrans.m_lpColorTable, FFTTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}


void CDemoView::OnFFTEx() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
    //����ͼ�����ݸ�transform_FFT�����FFTTrans
	FFTTrans.InputImgData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//����FourierEx()�������п��ٸ���Ҷ�任���ú�����Ҫ������ͼ��Ϊ2���ݴεķ���
	FFTTrans.FourierEx();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(FFTTrans.GetDimensions(),FFTTrans.m_nBitCountOut,
		FFTTrans.m_lpColorTable, FFTTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CDemoView::OnIFFTEx() 
{
	//����Ҷ���任
	FFTTrans.IFourierEx();
	
	//�½���ͼ���ڣ���ʾͼ��ָ��Ľ��
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(FFTTrans.GetDimensions(),FFTTrans.m_nBitCountOut,
		FFTTrans.m_lpColorTable, FFTTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnMove() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//ƽ�Ʋ������öԻ���
	GeometryMoveDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//��dlgPara.m_OffsetX, dlgPara.m_OffsetY��Ϊƽ��������ͼ��ƽ��
		geoTrans.Move(dlgPara.m_OffsetX, dlgPara.m_OffsetY);
		
		//�½���ͼ���ڣ���ʾ�任���
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}


void CDemoView::OnHorizontalMirror() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//ˮƽ����
	geoTrans.MirrorHori();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnVerticalMirror() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//��ֱ����
	geoTrans.MirrorVerti();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnZhuanZhi() 
{
	
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//ͼ��ת��
	geoTrans.ZhuanZhi();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW); 
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}


void CDemoView::OnClockwise90() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//˳ʱ��90��
	geoTrans.Clockwise90();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnAnticlockwise90() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//��ʱ��90��
	geoTrans.Anticlockwise90();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnRotate180() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//��ת180��
	geoTrans.Rotate180();
	
	//�½���ͼ���ڣ���ʾ�任���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnZoom() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//ԭͼ��ߴ�
	CSize oldImgSize=pDib->GetDimensions();
	
	//���Ų������öԻ���
	Zoomdlg dlgPara;
	
	//���Ų������öԻ�����ԭͼ��ߴ������ֵ
	dlgPara.m_oldHeight=oldImgSize.cy;
	dlgPara.m_oldWidth=oldImgSize.cx;
	
	if(dlgPara.DoModal()==IDOK){
		//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//����ָ������ͼ�Ĵ�С��ͼ�������������
		geoTrans.Zoom(dlgPara.m_newWidth, dlgPara.m_newHeight,
			         dlgPara.m_interpolationSelect);
		
		//�½���ͼ���ڣ���ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnRotate() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	//��ת�������öԻ���
	RotateSetDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		//����GeometryTrans��Ķ���geoTrans���õ�ǰDIB�����ʼ��
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//����Rotate��������ͼ����ת�����ǶȺͲ�ֵ��ʽ������������
		geoTrans.Rotate(dlgPara.m_rotateAngle, dlgPara.m_interpolationSelect);
		
		//�½���ͼ���ڣ���ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
	
}

/***********************************************************************
* �������ƣ�
* ValidBinaryMorphMask()
*
*����������
*  int *maskBuf  -�ṹԪ�ػ�����
*  int maskW  -�ṹԪ�صĿ�
*  int maskH  -�ṹԪ�صĸ�
*
*����ֵ��
*   ��
*
*˵�������л����б任,0������,255����Ŀ��
***********************************************************************/
bool CDemoView::ValidBinaryMorphMask(int *maskBuf, int maskW, int maskH)
{
	int i, j, count=0, flag=1;
	for(i=0;i<maskH;i++){
		for(j=0;j<maskW;j++){
			if(maskBuf[i*maskW+j]==1)
				count++;
			if(maskBuf[i*maskW+j]!=0&&maskBuf[i*maskW+j]!=1)
				flag=0;
		}
	}
	//����ṹԪ�����з�0��1��������1�ĸ�������1����Ϊ��Ч���룬����Ϊ��
	//Ч�ṹԪ��
	if(flag==0||count<1)
		return 0;
	else 
		return 1;
}
void CDemoView::OnErosionForBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��ʴ
		morph.BinaryErosion();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}

}

void CDemoView::OnDilationForBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ����
		morph.BinaryDilation();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
	
}

void CDemoView::OnOpenForBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��
		morph.BinaryOpen();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
	
}

void CDemoView::OnCloseForBinary() 
{
//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��
		morph.BinaryClose();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
	
}

void CDemoView::OnInnerEdgeForBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��̬ѧ�ڱ߽�
		morph.BinaryInnerEdge();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}

}

void CDemoView::OnOuterEdgeForBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��̬ѧ��߽�
		morph.BinaryOuterEdge();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
}

void CDemoView::OnMorphoContourForBinary() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧ�ṹԪ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//��֤�ṹԪ�ص���Ч��
		if(ValidBinaryMorphMask(dlgPara.m_defineMask,5,5)==0){
			::MessageBox(0,"�ṹԪ����������ֻ����0��1��������������һ�����ϵ�1��",MB_OK,0);
			return;
		}

		//����ṹԪ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��̬ѧ�߽�
		morph.BinaryContour();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}

}

void CDemoView::OnHitMissThinning() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}


	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//���л����н���ϸ���ĽṹԪ�����ڲ������,����Ҫ����
	//morph.InputMask(mask);

	//���л����б任����ϸ��
	morph.MorphoThinning();

	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnErosionForGray() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	int i,j;
	for(i=1;i<4;i++){
			for(j=1;j<4;j++){
				dlgPara.m_defineMask[i*5+j]=1;
			}
	}
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��ʴ
		morph.GrayErosion();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}

}

void CDemoView::OnDilationForGray() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ����
		morph.GrayDilation();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
}

void CDemoView::OnOpenForGray() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��
		morph.GrayOpen();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
}

void CDemoView::OnCloseForGray() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��
		morph.GrayClose();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}	
}

void CDemoView::OnMorphoGradientForGray() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ֵ��̬ѧ�ݶ�
		morph.GrayGradient();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
}

void CDemoView::OnTopHatPeak() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//������
		morph.GrayTopHatPeak();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
}

void CDemoView::OnTopHatVally() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//���ȼ��
		morph.GrayTopHatVally();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}

}

void CDemoView::OnTopHatPeakAndVally() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//��̬ѧģ��ѡ��Ի���
	MaskDefineDlg dlgPara;

	//ȱʡ�ĽṹԪ��ģ��
	dlgPara.m_defineMask[7]=1;
	dlgPara.m_defineMask[11]=1;
	dlgPara.m_defineMask[12]=1;
	dlgPara.m_defineMask[13]=1;
	dlgPara.m_defineMask[17]=1;
	if(dlgPara.DoModal()==IDOK){
		//����ģ��
		morph.InputMask(dlgPara.m_defineMask,5,5);

		//��ȼ��
		morph.GrayTopHatPeakVally();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);	
	}
}

void CDemoView::OnInteractiveThresh() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//��ֵ�ָ�Ի��������threshDlg
	ThreshSegmentDlg threshDlg;
	
	//�õ�ǰDIB�����滻threshDlg�г�Աm_histogramDib��ԭʼ����
	threshDlg.m_histogramDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	if(threshDlg.DoModal()==IDOK)
	{
		//����Ҷȱ任��Ķ���graytrans��������ǰDIB������Ϊ�������ݴ���graytrans��
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);

		//�����û�ѡ�����ֵ�����ö�ֵ�������ָ�ͼ��
		graytrans.Binary(threshDlg.m_threshSelect);
		
		//�½���ͼ����ʾ�ָ���
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
	
}

void CDemoView::OnOtusThreshold() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}

	//��ֱ��ͼ�������ͳ��ֱ��ͼ
	//����ֱ��ͼ��Ķ���hist�������õ�ǰDIB���ݶ�hist��ʼ��
	Histogram hist(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//ͳ��ֱ��ͼ
	if(pDib->m_nBitCount==8)
		hist.computeHistGray();
	else
		hist.computeHistBrightness();

	//�÷ָ���������ֱ��ͼ�Ĵ����ֵ
	ImgSegment segment;
    int thresh=segment.threshOtus(hist.m_histArray);

	//�������ֵthresh��Ϊ��ֵ���ָ�Ĳ�������ͼ���ֵ���ָ�	
	//����GrayTrans��Ķ���graytrans�����õ�ǰDIB���ݶ�graytrans��ʼ��
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	graytrans.Binary(thresh);
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CDemoView::OnRobert() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Roberts����
	segment.Roberts();
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnSobel() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Sobel����
	segment.Sobel();
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnPrewitt() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	segment.Prewitt();
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}



void CDemoView::OnLaplacian() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//Laplacian����
	segment.Laplacian();
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnKrisch() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Krisch����
	segment.Krisch();
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnGausslaplacian() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//��˹������˹����
	segment.GaussLaplacian();
	
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnEdgeSelectTemplete() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����ҶȺͲ�ɫͼ��
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"ֻ����ҶȺͲ�ɫͼ��",MB_OK,0);
		return ;
	}
	
	int  i, j, mask[25];

	//ģ���Զ���Ի���
	MaskDefineDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		//�Զ���ģ��
		for(i=0;i<5;i++){
			for(j=0;j<5;j++){
				mask[i*5+j]=dlgPara.m_defineMask[i*5+j];
			}
		}
		
		//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
		ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);

		//ģ���Զ����Ե���
		segment.EdgeByAnyMask(mask,5,5);
		
		//�½���ͼ����ʾ�ָ���
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,
			          segment.m_lpColorTableOut, segment.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnHoughTransform() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//���뾶���ֱ��ʺͽǶȼ��ֱ���
	float radiusResolution=1;
	float angleResolution=1;
	float r, a;

	//Hough�任
	segment.Hough(radiusResolution,angleResolution, &r, &a);
	
	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CDemoView::OnLineDetectByHough() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}
	
	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//���뾶���ֱ��ʺͽǶȼ��ֱ���
	float radiusResolution=1;
	float angleResolution=1;

	//������
	segment.longestLineDetectByHough(radiusResolution,angleResolution);
	
	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnRegionGrow() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//���������������öԻ���
	RegionGrowDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		CSize imgSize=pDib->GetDimensions();
		//�ж����ӵ��Ƿ���ͼ��Χ��
		if(dlgPara.m_seedX<0||dlgPara.m_seedX>imgSize.cx-1
			||dlgPara.m_seedY<0||dlgPara.m_seedY>imgSize.cy-1)
		{
			::MessageBox(0,"���ӵ㲻��ͼ��Χ��",MB_OK,0);
			return ;
		}

		//�ж���ֵ�Ƿ�����Ч��Χ��
		if(dlgPara.m_thresh<0||dlgPara.m_thresh>255)
		{
			::MessageBox(0,"��ֵӦ��0-255֮��",MB_OK,0);
			return ;
		}

		//�����ָ�����󣬽���ǰDIB��Ϊ�������ݶ�segment��ʼ��
		ImgSegment segment(imgSize,pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//�趨���ӵ�λ��
		CPoint p(dlgPara.m_seedX,dlgPara.m_seedY);
		
		//��ֵ
		int thresh=dlgPara.m_thresh;
		
		//��������
		segment.RegionGrow(p, thresh);
		
		//�½���ͼ����ʾ�ָ���
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnContourTrace() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}
	//����ͼ��ָ������segment���õ�ǰDIB�����ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//�߽����
	segment.ContourTrace();
	
	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnContourExtract() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�",MB_OK,0);
		return ;
	}
	
	//�ж��Ƿ��ֵͼ����ͼ���лҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255������,��Ƕ�ֵͼ
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255)
			{
				flag=0;
				break;
			}
		}
	}

	//�Ƕ�ֵͼ��
	if(!flag){
		::MessageBox(0,"��Ҫ�����ֵͼ��",MB_OK,0);
		return ;
	}

	//����ͼ��ָ������segment���õ�ǰDIB�����ʼ��
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//������ȡ
	segment.ContourExtract();
	
	//�½���ͼ����ʾ�������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();	
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,
		           segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnWatershedSegment() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//ˮ��ָ�������öԻ���
	WatershedParaDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		int objectCount=dlgPara.m_objectCount;
		if( objectCount!=2 && objectCount!=3){
			::MessageBox(0,"ֻ��2���3��Ŀ��",MB_OK,0);
			return ;
		}

		int contourFlg=dlgPara.m_contourFlg;

		CSize imgSize=pDib->GetDimensions();
		CPoint point[3];
		point[0].x=dlgPara.m_point1X; 
		point[0].y=dlgPara.m_point1Y;
		point[1].x=dlgPara.m_point2X; 
		point[1].y=dlgPara.m_point2Y;
		if(objectCount==3){
			point[2].x=dlgPara.m_point3X; 
			point[2].y=dlgPara.m_point3Y;
		}

		//�ж����ӵ��Ƿ���ͼ��Χ��
		if(point[0].x<0||point[0].x>imgSize.cx-1
			||point[0].y<0||point[0].y>imgSize.cy-1){
				::MessageBox(0,"��һ�����ӵ㲻��ͼ��Χ��",MB_OK,0);
	    		return ;
		}
		if(point[1].x<0||point[1].x>imgSize.cx-1
			||point[1].y<0||point[1].y>imgSize.cy-1){
				::MessageBox(0,"�ڶ������ӵ㲻��ͼ��Χ��",MB_OK,0);
	    		return ;
		}
		if(objectCount==3){
			if(point[2].x<0||point[2].x>imgSize.cx-1
				||point[2].y<0||point[2].y>imgSize.cy-1){
				::MessageBox(0,"���������ӵ㲻��ͼ��Χ��",MB_OK,0);
				return ;
			}
		}
		
		//����Watershed��Ķ���watershed���õ�ǰDIB���ݶ����ʼ��
		Watershed  watershed(imgSize,pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);

		//ˮ��ָ�
		watershed.WatershedSegment(point,objectCount,contourFlg);
		
		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(watershed.GetDimensions(),watershed.m_nBitCountOut,
			      watershed.m_lpColorTableOut, watershed.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}


void CDemoView::OnScan() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	//ֻ�����ɫͼ���Ҷ�ͼ��
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
	  ::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
	  return ;
	}

	//����SpecialEffectShow��Ķ���SpShow���õ�ǰDIB����Ϊ���ʼ��
	SpecialEffectShow SpShow(pDib->GetDimensions(),
	  pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);

	CClientDC aDC(this);//����豸��������
	SpShow.Scan(&aDC);//����ɨ����򣬴����豸����ָ��

}

void CDemoView::OnSlide() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
	  ::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
	  return ;
	}
	//����SpecialEffectShow��Ķ���SpShow���õ�ǰDIB����Ϊ���ʼ��
	SpecialEffectShow SpShow(pDib->GetDimensions(),
	  pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);

	CClientDC aDC(this);//����豸��������
	SpShow.Slide(&aDC);//���û�����ʾ���򣬴����豸����ָ��
}

void CDemoView::OnFadeIn() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
	  ::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
	  return ;
	}

	//����SpecialEffectShow��Ķ���SpShow���õ�ǰDIB����Ϊ���ʼ��
	SpecialEffectShow SpShow(pDib->GetDimensions(),
	  pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);

	CClientDC aDC(this);//����豸��������
	SpShow.FadeIn(&aDC);//���ý�����ʾ���򣬴����豸����ָ��
}

void CDemoView::OnMosaik() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
	  ::MessageBox(0,"ֻ�����ɫ�ͻҶ�ͼ��",MB_OK,0);
	  return ;
	}

	//����SpecialEffectShow��Ķ���SpShow���õ�ǰDIB����Ϊ���ʼ��
	SpecialEffectShow SpShow(pDib->GetDimensions(),
	  pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);

	CClientDC aDC(this);//����豸��������
	SpShow.Mosaik(&aDC);//������������ʾ���򣬴����豸����ָ��	
}

void CDemoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
}

// ͶӰ���˶�ģ���ָ�
void CDemoView::OnProjRestore() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//���ͼ��Ŀ�Ⱥ͸߶�
	int imgWidth = pDib->GetDimensions().cx;
	int imgHeight = pDib->GetDimensions().cy;

	ImageRestore ir;

	
	ProjectRestoreDlg prd;
	if(prd.DoModal() == IDOK){
		
		//���������ʱ�ռ�
		unsigned char * outBuffer = new unsigned char[imgWidth * imgHeight];
		memcpy(outBuffer,pDib->m_pImgData,sizeof(unsigned char) * imgWidth * imgHeight);
	
		// ������Ҫ����
		int angle = prd.m_angle;
		int length = prd.m_length;
		int cycleCount = prd.m_cycount;		

		// �������ɢ����
		ir.GeneratePSF(length,angle);

		// ͶӰ���ָ�
		ir.MapRestoreByteVar(outBuffer,imgWidth,imgHeight,1,cycleCount);
		
		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable,outBuffer);   // �����µ�ͼ��
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

		delete[] outBuffer;
	}
}

//ά���˲��¼���Ӧ����
void CDemoView::OnWienerFilter() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//���ͼ��Ŀ�Ⱥ͸߶�
	int imgWidth = pDib->GetDimensions().cx;
	int imgHeight = pDib->GetDimensions().cy;

	ImageRestore ir;
	
	WienterFilterDlg wfd;
	if(wfd.DoModal() == IDOK){
		
		//���������ʱ�ռ�
		unsigned char * outBuffer = new unsigned char[imgWidth * imgHeight];
		memcpy(outBuffer,pDib->m_pImgData,sizeof(unsigned char) * imgWidth * imgHeight);
	
		// ������Ҫ����
		int angle = wfd.m_angle;
		int length = wfd.m_length;
		double k = wfd.m_k;
		
		//�������ɢ����
		ir.GeneratePSF(length,angle);

		//ά���˲�
		ir.WienerRestoreByteVar(outBuffer,imgWidth,imgHeight,1,k);
		
		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable,outBuffer);   // �����µ�ͼ��
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

		delete[] outBuffer;
	}
	
}

// ���˲���Ӧ����
void CDemoView::OnInverseFilter() 
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//���ͼ��Ŀ�Ⱥ͸߶�
	int imgWidth = pDib->GetDimensions().cx;
	int imgHeight = pDib->GetDimensions().cy;

	ImageRestore ir;
	
	InverseFilterDlg ifd;
	if(ifd.DoModal() == IDOK){
		
		//���������ʱ�ռ�
		unsigned char * outBuffer = new unsigned char[imgWidth * imgHeight];
		memcpy(outBuffer,pDib->m_pImgData,sizeof(unsigned char) * imgWidth * imgHeight);
	
		// ������Ҫ����
		int angle = ifd.m_angle;
		int length = ifd.m_length;
		
		ir.GeneratePSF(length,angle);

		ir.InverseFilterByte(outBuffer,imgWidth,imgHeight,1);
		
		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable,outBuffer);   // �����µ�ͼ��
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

		delete[] outBuffer;
	}	
}

//transeform_wavelet wltTrans;
void CDemoView::OnHarrWaveletTrans() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	wltTrans.ReplaceDIBForNew(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	wltTrans.SimpleWaveletTrans(2);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
}

void CDemoView::OnHarrWaveletITrans() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	wltTrans.SimpleWaveletReverse();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
}

void CDemoView::OnSet2zeroDenoise() 
{
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	wltTrans.ReplaceDIBForNew(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	wltTrans.SetH2Zero();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
}

void CDemoView::OnHardthreshold() 
{
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	wltTrans.ReplaceDIBForNew(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	wltTrans.HardThreshold();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);	
}

void CDemoView::OnSoftthreshold() 
{
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	wltTrans.ReplaceDIBForNew(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	wltTrans.SoftThreshold();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
	dibNew->ReplaceDib(wltTrans.GetDimensions(), wltTrans.m_nBitCountOut, wltTrans.m_pColorTableOut, wltTrans.m_pImgDataOut);	
}

void CDemoView::OnPepperSaltNoise() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	imgnoise.AddPepperSaltNoise();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
}

void CDemoView::OnRandom() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	imgnoise.AddRandomNoise();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
}

void CDemoView::OnAveSmooth() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	AvgDialog *dlg=new AvgDialog();
	dlg->DoModal();
	int TempH, TempW, TempCX, TempCY;
	float fCoef;
	float Temp[81];
	int i;
	//��ģ��Ϊƽ��ģ��
	for (i = 0; i < 81; i++)
	{
		Temp[i]=1.0;
	}
	switch(dlg->m_nIndex)
	{
	case 0:
		//��ʼ��ģ�崰��
		TempH=3;
		TempW=3;
		TempCX=1;
		TempCY=1;
		fCoef=(float)(1.0/9.0);
		break;
	case 1:
		//��ʼ��ģ�崰��
		TempH=5;
		TempW=5;
		TempCX=2;
		TempCY=2;
		fCoef=(float)(1.0/25.0);
		break;
	case 2:
		//��ʼ��ģ�崰��
		TempH=9;
		TempW=9;
		TempCX=4;
		TempCY=4;
		fCoef=(float)(1.0/81.0);
		break;
	default:
		break;
	}
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    imgnoise.AvgTemplate(TempH,TempW,TempCX,TempCY,Temp,fCoef);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	return;
}

void CDemoView::OnMiddleSmooth() 
{
	// TODO: Add your command handler code here
	
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CMedianDialog *dlg=new CMedianDialog();
	dlg->DoModal();
	int modal;
	switch(dlg->m_nIndex)
	{
	case 0:
		modal=3;break;
	case 1:
		modal=5;break;
	case 2:
		modal=7;break;
	default:
		break;
	}

	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	int iFilterH=modal;
	int iFilterW=modal;
	int iFilterCX=(modal-1)/2;
	int iFilterCY=(modal-1)/2;
	imgnoise.MedianFilter(iFilterH, iFilterW, iFilterCX, iFilterCY);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	
}

void CDemoView::OnLaplacienSharp() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	int TempH, TempW, TempCX, TempCY;
	float fCoef;
	float Temp[9];
	//��ģ��Ϊƽ��ģ��
	Temp[0]=-1.0;
	Temp[1]=-1.0;
	Temp[2]=-1.0;
	Temp[3]=-1.0;
	Temp[4]=9.0;
	Temp[5]=-1.0;
	Temp[6]=-1.0;
	Temp[7]=-1.0;
	Temp[8]=-1.0;
    //��ʼ���Ի������
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	fCoef=1.0;
    imgnoise.LapTemplate(TempH,TempW,TempCX,TempCY,Temp,fCoef);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
}

void CDemoView::OnChooseMask() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgSmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	imgSmooth.ChooseMask();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgSmooth.GetDimensions(),imgSmooth.m_nBitCountOut,imgSmooth.m_lpColorTable, imgSmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
}

void CDemoView::OnPowerSmooth() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgSmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	int TempH, TempW, TempCX, TempCY;
	float fCoef;
	/*
	float Temp[9];
	//��ģ��Ϊƽ��ģ��
	Temp[0]=1.0;
	Temp[1]=2.0;
	Temp[2]=1.0;
	Temp[3]=2.0;
	Temp[4]=4.0;
	Temp[5]=2.0;
	Temp[6]=1.0;
	Temp[7]=2.0;
	Temp[8]=1.0;
    //��ʼ���Ի������
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	fCoef=(float)(1.0/16.0);*/
	float Temp[25];
	//��ģ��Ϊƽ��ģ��
	Temp[0]=0.0;
	Temp[1]=1.0;
	Temp[2]=2.0;
	Temp[3]=1.0;
	Temp[4]=0.0;

	Temp[5]=1.0;
	Temp[6]=2.0;
	Temp[7]=4.0;
	Temp[8]=2.0;
	Temp[9]=1.0;

	Temp[10]=2.0;
	Temp[11]=4.0;
	Temp[12]=8.0;
	Temp[13]=4.0;
	Temp[14]=2.0;

	Temp[15]=1.0;
	Temp[16]=2.0;
	Temp[17]=4.0;
	Temp[18]=2.0;
	Temp[19]=1.0;

	Temp[20]=0.0;
	Temp[21]=1.0;
	Temp[22]=2.0;
	Temp[23]=1.0;
	Temp[24]=0.0;
    //��ʼ���Ի������
	TempH=5;
	TempW=5;
	TempCX=2;
	TempCY=2;
	fCoef=(float)(1.0/48.0);

	imgSmooth.AvgTemplate(TempH,TempW,TempCX,TempCY,Temp,fCoef);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib( imgSmooth.GetDimensions(), imgSmooth.m_nBitCountOut, imgSmooth.m_lpColorTable,  imgSmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
}

/***********************************************************************
* �������ƣ�
* OnDct()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����������ͼ����п�����ɢ���ұ任��Ҫ������ͼ��Ŀ�͸߱�����2���ݴη�
***********************************************************************/
void CDemoView::OnDct() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//�ж�ͼ���С�Ƿ�Ϊ2���ݴεķ���
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//ͼ�����2���ݴη�
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//ͼ��߲���2���ݴη�
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"����ͼ��Ŀ�͸�ӦΪ2���ݴη�",MB_OK,0);
		return ;
	}

	DIBDCT.InputImgData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    DIBDCT.ImgDCT();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(DIBDCT.GetDimensions(), DIBDCT.m_nBitCountOut,
		DIBDCT.m_lpColorTable, DIBDCT.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
}

/***********************************************************************
* �������ƣ�
* OnIDct()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����������ͼ����п�����ɢ���ҷ��任
***********************************************************************/
void CDemoView::OnIdct() 
{
	//��ɢ���ҷ��任
	DIBDCT.ImgIDCT();
	
    //�½���ͼ���ڣ���ʾͼ��ָ��Ľ��
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(DIBDCT.GetDimensions(),DIBDCT.m_nBitCountOut,
		DIBDCT.m_lpColorTable, DIBDCT.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnGradesharp() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	unsigned char bThre=30;
	imgnoise.GradSharp(bThre);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
}

void CDemoView::OnSnr() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	//����ԭʼDIBλͼ
	CString lpfilter="BMP Filter(*.bmp)|*.bmp";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpfilter);
	if (dlg.DoModal()!=IDOK)
	{
		AfxMessageBox("��ѡ��BMPλͼ");
		return;
	}
	CString fileName = dlg.GetPathName();
	ImgCenterDib m_Srcdib;
	if (m_Srcdib.Read(fileName) != TRUE)
	{
		AfxMessageBox("ԭʼͼ�����ʧ��!",MB_ICONINFORMATION);
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	imgnoise.SNR(m_Srcdib.m_pImgData);
//	CString PSNR=imgnoise.PSNR(m_Srcdib.m_pImgData);
//	MessageBox("�����:"+SNR+"\n"+"��ֵ�����:"+PSNR+"\n","ͼ����������",MB_ICONINFORMATION);
    CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);  
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();

}

void CDemoView::OnFirstimage() 
{	
	TCHAR *pszFile = new TCHAR[MAX_PATH * MAX_PATH];
    memset(pszFile,0,sizeof(TCHAR)*MAX_PATH * MAX_PATH);

	//�ļ��򿪶Ի���
	CFileDialog fdlg(TRUE,NULL, NULL, OFN_HIDEREADONLY,
		_T("BMPͼƬ (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	fdlg.m_ofn.lpstrFile = pszFile;
	fdlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH;
	CString str;
	if ( fdlg.DoModal() == IDOK )
	{
		POSITION pos = fdlg.GetStartPosition();
		if (pos != NULL)
			str = fdlg.GetNextPathName(pos);

		//��������
		DIB1.Read(str);
		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		pDocNew->SetTitle(str);
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(DIB1.GetDimensions(),DIB1.m_nBitCount,
			DIB1.m_lpColorTable, DIB1.m_pImgData);
		pView->OnInitialUpdate();
		pDocNew->UpdateAllViews(pView);
	}
	delete []pszFile;
}

void CDemoView::OnSecondimage() 
{
	TCHAR *pszFile = new TCHAR[MAX_PATH * MAX_PATH];
    memset(pszFile,0,sizeof(TCHAR)*MAX_PATH * MAX_PATH);
	
	//�ļ��򿪶Ի���
	CFileDialog fdlg(TRUE,NULL, NULL, OFN_HIDEREADONLY,
		_T("BMPͼƬ (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	fdlg.m_ofn.lpstrFile = pszFile;
	fdlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH;
	CString str;
	if ( fdlg.DoModal() == IDOK )
	{
		POSITION pos = fdlg.GetStartPosition();
		if (pos != NULL)
			str = fdlg.GetNextPathName(pos);

		//��������
		DIB2.Read(str);
		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		pDocNew->SetTitle(str);
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(DIB2.GetDimensions(),DIB2.m_nBitCount,DIB2.m_lpColorTable, DIB2.m_pImgData);
		pView->OnInitialUpdate();
		pDocNew->UpdateAllViews(pView);
	}
	delete []pszFile;
}

void CDemoView::OnMatchConner() 
{
	CSize size1=DIB1.GetDimensions();
	CSize size2=DIB2.GetDimensions();
	if (size1.cx==0||size1.cy==0){
		::MessageBox(0,"���������׼ͼ��1",MB_OK,0);
		return ;
	}
	if (size2.cx==0||size2.cy==0){
		::MessageBox(0,"���������׼ͼ��2",MB_OK,0);
		return ;
	}
	if(size1.cx!=size2.cx || size1.cy!=size2.cy){
		::MessageBox(0,"����׼������ͼ��Ӧ�ô�С���",MB_OK,0);
		return ;
	}

	if ( DIB1.m_nBitCount!=8&&DIB2.m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	//����ͼ����׼�����
	Register match;

	//�������׼������ͼ��
	match.InputImg1(DIB1.GetDimensions(),DIB1.m_nBitCount,
		DIB1.m_lpColorTable,DIB1.m_pImgData);
	match.InputImg2(DIB2.GetDimensions(),DIB2.m_nBitCount,
		DIB2.m_lpColorTable,DIB2.m_pImgData);

	//�ǵ�ƥ��
	if (match.ConnerRegistering())
	{
		//�½���ͼ,��ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		pDocNew->SetTitle("�ǵ�ƥ����");
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(match.GetDimensions(), match.m_nBitCountOut,
			match.m_lpColorTableOut, match.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnMatchTemplet() 
{
	CSize size1=DIB1.GetDimensions();
	CSize size2=DIB2.GetDimensions();
	if (size1.cx==0||size1.cy==0){
		::MessageBox(0,"������ο�ͼ",MB_OK,0);
		return ;
	}
	if (size2.cx==0||size2.cy==0){
		::MessageBox(0,"�������׼ͼ",MB_OK,0);
		return ;
	}
	if(size1.cx<=size2.cx || size1.cy<=size2.cy){
		::MessageBox(0,"��׼ͼ�ߴ�ӦС�ڲο�ͼ",MB_OK,0);
		return ;
	}

	if (DIB1.m_nBitCount!=8&&DIB2.m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	//����ͼ����׼�����
	Register match;

	//����ο�ͼ�ͻ�׼ͼ
	match.InputImg1(DIB1.GetDimensions(),DIB1.m_nBitCount,
		DIB1.m_lpColorTable,DIB1.m_pImgData);
	match.InputImg2(DIB2.GetDimensions(),DIB2.m_nBitCount,
		DIB2.m_lpColorTable,DIB2.m_pImgData);

	//ģ��ƥ��
	if (match.relativityRegistering())
	{
		//��������ͼ,��ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		pDocNew->SetTitle("ģ��ƥ����");
		ImgCenterDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(match.GetDimensions(), match.m_nBitCountOut,
			match.m_lpColorTableOut, match.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
	}
}

void CDemoView::OnConnerDetect() 
{
	CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	//ֻ����Ҷ�ͼ��
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	Register match;
	match.InputImg1(pDib->GetDimensions(),pDib->m_nBitCount,
			                pDib->m_lpColorTable,pDib->m_pImgData);
	match.HarisConner();
	
	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(match.GetDimensions(),match.m_nBitCountOut,
		match.m_lpColorTableOut, match.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
}

void CDemoView::OnMotionFopen1() 
{
	TCHAR *pszFile = new TCHAR[MAX_PATH * MAX_PATH];
    memset(pszFile,0,sizeof(TCHAR)*MAX_PATH * MAX_PATH);

	//�ļ��򿪶Ի���
	CFileDialog fdlg(TRUE,NULL, NULL, OFN_HIDEREADONLY,
		_T("BMPͼƬ (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	fdlg.m_ofn.lpstrFile = pszFile;
	fdlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH;
	CString str;
	if ( fdlg.DoModal() == IDOK )
	{
		POSITION pos = fdlg.GetStartPosition();
		if (pos != NULL)
			str = fdlg.GetNextPathName(pos);

		//��������
		DIB1.Read(str);
		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		pDocNew->SetTitle(str);
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(DIB1.GetDimensions(),DIB1.m_nBitCount,
			DIB1.m_lpColorTable, DIB1.m_pImgData);
		pView->OnInitialUpdate();
		pDocNew->UpdateAllViews(pView);
	}
	delete []pszFile;
	
}

void CDemoView::OnMotionFopen2() 
{
	TCHAR *pszFile = new TCHAR[MAX_PATH * MAX_PATH];
    memset(pszFile,0,sizeof(TCHAR)*MAX_PATH * MAX_PATH);
	
	//�ļ��򿪶Ի���
	CFileDialog fdlg(TRUE,NULL, NULL, OFN_HIDEREADONLY,
		_T("BMPͼƬ (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	fdlg.m_ofn.lpstrFile = pszFile;
	fdlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH;
	CString str;
	if ( fdlg.DoModal() == IDOK )
	{
		POSITION pos = fdlg.GetStartPosition();
		if (pos != NULL)
			str = fdlg.GetNextPathName(pos);

		//��������
		DIB2.Read(str);
		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew=pView->GetDocument();
		pDocNew->SetTitle(str);
		ImgCenterDib *dibNew=pDocNew->GetPDib();	
		dibNew->ReplaceDib(DIB2.GetDimensions(),DIB2.m_nBitCount,DIB2.m_lpColorTable, DIB2.m_pImgData);
		pView->OnInitialUpdate();
		pDocNew->UpdateAllViews(pView);
	}
	delete []pszFile;
	
}

void CDemoView::OnMotionFest() 
{
	CSize size1=DIB1.GetDimensions();
	CSize size2=DIB2.GetDimensions();
	if (size1.cx==0||size1.cy==0){
		::MessageBox(0,"���������׼ͼ��1",MB_OK,0);
		return ;
	}
	if (size2.cx==0||size2.cy==0){
		::MessageBox(0,"���������׼ͼ��2",MB_OK,0);
		return ;
	}
	if(size1.cx!=size2.cx || size1.cy!=size2.cy){
		::MessageBox(0,"����׼������ͼ��Ӧ�ô�С���",MB_OK,0);
		return ;
	}

	int i = 1;int k = 1;
	while(k < size1.cx)
	{
		k = k * 2;
	}

	if(k!=size1.cx || k!=size2.cx){
		::MessageBox(0,"����׼������ͼ���СӦΪ2���ݴ�",MB_OK,0);
		return ;
	}
	if(size1.cx!=size1.cy || size2.cx!=size2.cy){
		::MessageBox(0,"ͼ��ĳ���������",MB_OK,0);
		return ;
	}
	
	if ( DIB1.m_nBitCount!=8&&DIB2.m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	int x,y;
	PhaseCorrelation(DIB1.m_pImgData,DIB2.m_pImgData,size1.cx,size2.cy,&x,&y);

	CString str;
	str.Format("����ڵ�һ��ͼ��\n x�����ƶ�Ϊ%d��y�����ƶ�Ϊ%d��",x,y);
	MessageBox(str.GetBuffer(str.GetLength()),MB_OK,0);
	
}

void CDemoView::OnMitonDest() 
{
	CSize size1=DIB1.GetDimensions();
	CSize size2=DIB2.GetDimensions();
	if (size1.cx==0||size1.cy==0){
		::MessageBox(0,"���������׼ͼ��1",MB_OK,0);
		return ;
	}
	if (size2.cx==0||size2.cy==0){
		::MessageBox(0,"���������׼ͼ��2",MB_OK,0);
		return ;
	}
	if(size1.cx!=size2.cx || size1.cy!=size2.cy){
		::MessageBox(0,"����׼������ͼ��Ӧ�ô�С���",MB_OK,0);
		return ;
	}

	int i = 1;int k = 1;
	while(k < size1.cx)
	{
		k = k * 2;
	}

	if(k!=size1.cx || k!=size2.cx){
		::MessageBox(0,"����׼������ͼ���СӦΪ2���ݴ�",MB_OK,0);
		return ;
	}
	if(size1.cx!=size1.cy || size2.cx!=size2.cy){
		::MessageBox(0,"ͼ��ĳ���������",MB_OK,0);
		return ;
	}
	
	if ( DIB1.m_nBitCount!=8&&DIB2.m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}

	int x,y;
	PhaseAngleDiff(DIB1.m_pImgData,DIB2.m_pImgData,size1.cx,size2.cy);

}

void CDemoView::OnMenuitem32881() 
{
	// TODO: Add your command handler code here
		CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

//	unsigned char bThre=30;

    imgnoise.TvGrad(10,3,10,0.05,0.1);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);  
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
	
}

void CDemoView::OnMenuitem32882() 
{
	// TODO: Add your command handler code here
		CDemoDoc *pDoc=GetDocument();
	ImgCenterDib *pDib=pDoc->GetPDib();
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"ֻ����Ҷ�ͼ��",MB_OK,0);
		return ;
	}
	
	CImgEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

//	unsigned char bThre=30;
	imgnoise.Testdiejia();

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	ImgCenterDib *dibNew=pDocNew->GetPDib();
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();	
}
