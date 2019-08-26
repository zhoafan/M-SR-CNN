#include "stdafx.h"
#include"histogram.h"
#include "math.h"

/***********************************************************************
* �������ƣ�
* Histogram()
*
*˵�����޲����Ĺ��캯������ʼ����Ա����
***********************************************************************/
Histogram::Histogram()
{
	//ֱ��ͼ������0
	for(int i=0;i<256;i++)
    	m_histArray[i]=0;

	m_average=0;//��ֵΪ0

	m_deviation=0;//����Ϊ0

    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

}

/***********************************************************************
* �������ƣ�
* Histogram()
*
*����������
*  CSize size -ͼ���С�����ߣ�
*  int nBitCount  -ÿ������ռλ��
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*
*����ֵ��
*   ��
*
*˵����������Ϊ�������Ĺ��캯��������λͼ�Ĵ�С��ÿ����λ������ɫ��
*      ��λͼ���ݣ�����ImgCenterDib()�Ի����Ա��ʼ��������ʼ���������
*      ���ݳ�Ա
***********************************************************************/
Histogram::Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	//ֱ��ͼ������0
	for(int i=0;i<256;i++)
    	m_histArray[i]=0;

	m_average=0;//��ֵΪ0

	m_deviation=0;//����Ϊ0

    m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

/***********************************************************************
* �������ƣ�
*   ~Histogram()

*
*˵���������������ͷ���Դ
***********************************************************************/

Histogram::~Histogram()
{
	//�ͷ����ͼ��λͼ���ݻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//�ͷ����ͼ����ɫ��
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***********************************************************************
* �������ƣ�
* GetDimensions()
*
*����������
*  ��
*
*����ֵ��
*   ͼ��ĳߴ磬��CSize���ͱ��
*
*˵�����������ͼ��Ŀ�͸�
***********************************************************************/
CSize Histogram::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�
* computeHistGray()
*
*˵�����Ҷ�ͼ��ͳ��ֱ��ͼ��m_histArray�д���˵�ǰͼ���ͳ������
***********************************************************************/
void Histogram::computeHistGray()
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ�ƻҶ�ֱ��ͼ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=*(m_pImgData+i*lineByte+j);
		    m_histArray[temp]++;
		}
	}
}


/***********************************************************************
* �������ƣ�
* computeHistBrightness()
*
*˵������ɫͼ������ֱ��ͼ��m_histArray�д���˵�ǰͼ�������ͳ������
***********************************************************************/
void Histogram::computeHistBrightness()
{
	//��ɫͼ����Ч
	if(m_nBitCount!=24)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ������ֱ��ͼ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=0.11**(m_pImgData+i*lineByte+j*3+0)
				+0.59**(m_pImgData+i*lineByte+j*3+1)
				+0.30**(m_pImgData+i*lineByte+j*3+2)+0.5;
		    m_histArray[temp]++;
		}
	}
}


/***********************************************************************
* �������ƣ�
* computeHistRed()
*
*˵������ɫͼ���ɫͨ��ֱ��ͼ����ͼ���ɫ������ͳ��
***********************************************************************/
void Histogram::computeHistRed()
{
		//��ɫͼ����Ч
	if(m_nBitCount!=24)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ�ƺ�ɫͨ��ֱ��ͼ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=*(m_pImgData+i*lineByte+j*3+2);
		    m_histArray[temp]++;
		}
	}
}

/***********************************************************************
* �������ƣ�
* computeHistGreen()
*
*˵������ɫͼ����ɫͨ��ֱ��ͼ����ͼ����ɫ������ͳ��
***********************************************************************/
void Histogram::computeHistGreen()
{
		//��ɫͼ����Ч
	if(m_nBitCount!=24)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ����ɫͨ��ֱ��ͼ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=*(m_pImgData+i*lineByte+j*3+1);
		    m_histArray[temp]++;
		}
	}
}

/***********************************************************************
* �������ƣ�
* computeHistBlue()
*
*˵������ɫͼ����ɫͨ��ֱ��ͼ����ͼ����ɫ������ͳ��
***********************************************************************/
void Histogram::computeHistBlue()
{
	//��ɫͼ����Ч
	if(m_nBitCount!=24)
		return;

	//ѭ������
	int i,j;

	//ֱ��ͼ������0
	for(i=0;i<256;i++)
    	m_histArray[i]=0;

	//ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�м����
	int temp;

	//ͳ����ɫͨ��ֱ��ͼ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			temp=*(m_pImgData+i*lineByte+j*3+0);
		    m_histArray[temp]++;
		}
	}
}


/***********************************************************************
* �������ƣ�
* computeAverage()
*
*˵��������ֱ��ͼ��ֵ��ֱ��ͼ��ͳ������
***********************************************************************/
void Histogram::computeAverage()
{
	int sum=0;
	for(int i=0;i<256;i++)
		sum+= i*m_histArray[i];
	m_average=(float)sum/(m_imgWidth*m_imgHeight);
}

/***********************************************************************
* �������ƣ�
* computeDeviation()
*
*˵��������ֱ��ͼ���ֱ��ͼ��ͳ������
***********************************************************************/
void Histogram::computeDeviation()
{

	double deviation=0;
	for(int i=0;i<256;i++)
		deviation += (i-m_average)*(i-m_average)*m_histArray[i];

	deviation/=(m_imgWidth*m_imgHeight);

	m_deviation=sqrt(deviation);
}

/***********************************************************************
* �������ƣ�
* histAve()
*
*˵����ֱ��ͼ���⣬�ú���ֻ�ԻҶ�ͼ����Ч
***********************************************************************/
void Histogram::histAve()
{
	// ֻ����Ҷ�
	if(m_nBitCount!=8)
		return;

	//�ͷžɵ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//ѭ������
	int i,j;

	//ӳ���
	double map[256];
	int map1[256],map2[256],map3[256];

	//�м����
	int sum,tmp;
	int k=0,n=0,m=0,p=0,q=0;

	//ͳ�ƻҶ�ֱ��ͼ
	computeHistGray();

	

	for(i=0;i<256;i++){
		if(m_histArray[i]>0){
			map1[k]=m_histArray[i];
			k=k+1;
		}		
	}
	for(m=1;m<k-1-1;m++){
		if(map1[m]>map1[m-1]&&map1[m]>map1[m+1]){
			map2[n]=map1[m];
			n=n+1;
		}		
	}
	for(p=1;p<n-1-1;p++){
		if(map2[p]>map2[p-1]&&map2[p]>map2[p+1]){
			map3[q]=map2[p];
			q=q+1;
		}		
	}
	//��map3�еľ�ֵ
	int sum1=0,r;
	for(r=0;r<q-1;r++){
		sum1+=map3[r];
				
	}
	int temp1=(sum1)/(q); 

	



	for(i=0;i<256;i++){
		if(m_histArray[i]>(int)(1.2*temp1))
			m_histArray[i]=(int)(1.2*temp1);
		else  if(m_histArray[i]<(int)(0.3*temp1))
            m_histArray[i]=(int)(0.2*temp1);
	}
	//����ӳ���
	sum=0;
	for(i=0;i<256;i++){
		sum+=m_histArray[i];
		map[i]=(double)sum*255/(m_imgWidth*m_imgHeight)+0.5;
		if(map[i]>255)
			map[i]=255;
	}

	
	//������ݸ�ֵ
	for(i=0;i<m_imgHeight;i++)
	{
		for(j=0;j<m_imgWidth;j++)
		{		
			tmp=*(m_pImgData+i*lineByteOut+j);
			*(m_pImgDataOut+i*lineByteOut+j)=(int)map[tmp];
		}
	}

}

