#include "StdAfx.h"
#include "StaticDetect.h"

/***********************************************************************
* �������ƣ�
*     CStaticDetect()
*
*˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
CStaticDetect::CStaticDetect(int nwidth, int nheight)
{
	m_nFrame = 0;
	count = 0;
	WIDTH = nwidth;
	HEIGHT = nheight;
	IMAGESIZE = WIDTH*HEIGHT;
	
	m_pCurImage = new BYTE[IMAGESIZE*3];
	memset(m_pCurImage,0,IMAGESIZE*3);
	m_pGrayImage = new BYTE[IMAGESIZE];
	memset(m_pGrayImage,0,IMAGESIZE);
	m_pBackground = new BYTE[IMAGESIZE];
	memset(m_pBackground,0,IMAGESIZE);
    m_pDiffImage = new BYTE[IMAGESIZE]; 
	memset(m_pDiffImage,0,IMAGESIZE);
	m_pDetectImage = new BYTE[IMAGESIZE*3];
	memset(m_pDetectImage,0,IMAGESIZE*3);
	m_pBackHistgram = new short int[IMAGESIZE*256];
	memset(m_pBackHistgram,0,IMAGESIZE*256*sizeof(short int));
	m_pTemplate = new UINT[IMAGESIZE];
	memset(m_pTemplate,0,IMAGESIZE*sizeof(UINT));
}

/***********************************************************************
* �������ƣ�
*     ~CStaticDetect()
*
*˵���������������ͷ���Դ
***********************************************************************/
CStaticDetect::~CStaticDetect()
{
	delete [ ]m_pBackHistgram;
	delete [ ]m_pDiffImage;
	delete [ ]m_pGrayImage;
	delete [ ]m_pBackground;
	delete [ ]m_pDetectImage;
	delete [ ]m_pCurImage;
	delete [ ]m_pTemplate;
}

/***********************************************************************
* �������ƣ�
*     ReceiveFrame
* 
* ������
*     int index-֡��
*     BYTE * sBuf-����ͼ�񻺳�
* 
* ˵����
*     ����ͼ����м���
/***********************************************************************/
void CStaticDetect::ReceiveFrame(int index,BYTE* sBuf)
{
	m_nFrame = index;
	memcpy(m_pCurImage,sBuf,WIDTH*HEIGHT*3);

	//ת��Ϊ�Ҷ�ͼ��
	RGBToGray(m_pCurImage,m_pGrayImage,WIDTH,HEIGHT);

	//�ۼ�ͼ��
	GetMultiData(m_pGrayImage,WIDTH,HEIGHT);
	
	if(m_nFrame>BACK_ALL_NUM)
	{
		if(m_nFrame==(BACK_ALL_NUM+1))
		{
			//��ñ���
			GetBgImage();
			memset(m_pTemplate,0,IMAGESIZE*sizeof(UINT));
			count = 0;
		}
		else if(m_nFrame%(2*BACK_ALL_NUM)==1) //���±���ͼ��
		{
			GetBgImage();
			memset(m_pTemplate,0,IMAGESIZE*sizeof(UINT));
			count = 0;
		}

		//���ͼ��
		DiffImage(m_pGrayImage,m_pBackground,WIDTH,HEIGHT,THRESHOLD);

	}
	
}

/************************************************************************
* �������ƣ�
*     RGBToGray
*
* ������
*     BYTE *sRGB-����RGBͼ��
*     BYTE *sGray-����Ҷ�ͼ��
*     int nWidth-ͼ����
*     int nHeight-ͼ��߶�
*
* ˵������RGBͼ��ת��Ϊ�Ҷ�ͼ��
************************************************************************/
void CStaticDetect::RGBToGray(BYTE *sRGB,BYTE *sGray,int nWidth, int nHeight)
{
	int i,j;
	for(j=0;j<nHeight;j++)
	{
		for(i=0;i<nWidth;i++)
		{
			sGray[nWidth*j+i]=(5*sRGB[3*(nWidth*j+i)]+3*sRGB[3*(nWidth*j+i)+1]+2*sRGB[3*(nWidth*j+i)+2])/10;
		}
	}

}

/************************************************************************
* �������ƣ�
*     DiffImage
* 
* ������
*     BYTE *SGray-����Ҷ�ͼ��
*     BYTE *pGray- ���뱳��ͼ��
*     int nWidth-ͼ����
*     int nHeight-ͼ��߶�
*     int nThreshold-��ֵ
*
* ˵����
*      ���ͼ����Ŀ��
************************************************************************/
void CStaticDetect::DiffImage(BYTE *sGray,BYTE *pGray,int nWidth, int nHeight,int nThreshold)
{
	int i,j;
	for(j=0;j<nHeight;j++)
	{
		for(i=0;i<nWidth;i++)
		{
			int diff=sGray[j*nWidth+i]-pGray[nWidth*j+i];		
			if(diff>nThreshold||diff<-nThreshold)
			{
				m_pDiffImage[j*nWidth+i]=255;
			}
			else
			{
				m_pDiffImage[j*nWidth+i]=0;
			}
		}
	}
}

/************************************************************************
* �������ƣ�
*     GetMultiData
* 
* ������
*     BYTE *sGray-����ͼ��
*     int nWidth-ͼ����
*     int nHeight-ͼ��߶�
*
* ˵������֡�ۼ�
************************************************************************/
void CStaticDetect::GetMultiData(BYTE *sGray,int nWidth, int nHeight)
{
    count++;
	for(int j=0;j<nHeight;j++)
		for(int i=0;i<nWidth;i++)
			m_pTemplate[j*nWidth+i]+=sGray[j*nWidth+i];
}

/************************************************************************
* �������ƣ�
*     GetBgImage
*
* ˵������ñ���ͼ��
************************************************************************/
void CStaticDetect::GetBgImage()
{
	int i,j,t,pixel;
	for(j=0;j<HEIGHT;j++)
	{
		for(i=0;i<WIDTH;i++)
		{
			t = j*WIDTH+i;
			pixel = UINT(m_pTemplate[t]/count+0.5);
			if(pixel>255)
				m_pBackground[t] = BYTE(255);
			else if(pixel<0)
				m_pBackground[t] = BYTE(0);
			else
				m_pBackground[t] = BYTE(pixel);
		}
	}
}
