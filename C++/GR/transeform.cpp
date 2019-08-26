#include "stdafx.h"
#include "transeform.h"
#include <math.h>

#include "WaveletApi.h"
#include "FilterApi.h"
#include "DIBapi.h"

#pragma comment(lib, "wavelet_3_8.lib")

transeform_wavelet::transeform_wavelet()
{
	m_nColorTableLengthOut=0;
	m_nScale = 0;
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_pColorTableOut=NULL;
	pCoeff = NULL;
}

transeform_wavelet::transeform_wavelet(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	m_nBitCountOut=m_nBitCount;
	m_pColorTableOut=m_lpColorTable;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	for(int i=0 ; i < m_imgWidth ; i++)
		for ( int j = 0 ; j < m_imgHeight ; j++)
		     m_pImgDataOut[j * lineByte + i]=pImgData[j * lineByte + i];
	pCoeff = NULL;
}

transeform_wavelet::~transeform_wavelet()
{
	if (m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
		pCoeff = NULL;
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * MatrixRegionCopy()
 *
 * ������
 * unsigned char *matrixRegion Դ��������ָ��
 * int heightRegion ��������ĸ߶�
 * int widthRegion ��������Ŀ��
 * unsigned char *matrixDest Ŀ�ľ�������ָ��
 * int heightDest Ŀ�ľ�������ĸ߶�
 * int widthDest Ŀ�ľ�������Ŀ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ��Դ�����ָ���������ݸ��Ƶ�Ŀ�ľ����ָ������
 *
 **********************************************************************/
void transeform_wavelet::MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
				 unsigned char *matrixDest, int heightDest, int widthDest)
{
	//��������߶ȺͿ��
	int heightMin = heightRegion>heightDest ? heightDest : heightRegion;
	int widthMin = widthRegion>widthDest ? widthDest : widthRegion;
	int h, w;
	for (h=0; h<heightMin; h++)
	{
		for (w=0; w<widthMin; w++)
		{
			matrixDest[h * widthDest + w] = matrixRegion[h * widthRegion + w];
		}
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * MatrixSub()
 *
 * ������
 * unsigned char *matrixA ��������A����ָ��
 * unsigned char *matrixB ��������B����ָ��
 * int height �߶�
 * int width ���
 * unsigned char *result ���������ָ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ���������������A��B���
 *
 **********************************************************************/
void transeform_wavelet::MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] - matrixB[h * width + w] + 128;
			result[h * width + w] = temp;
		}
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * MatrixSampleRow()
 *
 * ������
 * unsigned char *matrixInput ��������������ָ��
 * unsigned char *matrixOutputOdd �����в������ɵľ���
 * unsigned char *matrixOutputEven ż���в������ɵľ���
 * int heightOutput �������߶�
 * int widthOutput ���������
 * int widthInput ���������
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * �������������г�����
 *
 **********************************************************************/
void transeform_wavelet::MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
										  unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * 2 * widthInput + w];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[(h * 2 + 1) * widthInput + w];
		}
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * MatrixSampleLine()
 *
 * ������
 * unsigned char *matrixInput ��������������ָ��
 * unsigned char *matrixOutputOdd �����в������ɵľ���
 * unsigned char *matrixOutputEven ż���в������ɵľ���
 * int heightOutput �������߶�
 * int widthOutput ���������
 * int widthInput ���������
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * �������������г�����
 *
 **********************************************************************/
void transeform_wavelet::MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
										  unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * widthInput + w * 2];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[h * widthInput + w * 2 + 1];
		}
	}
}


/**********************************************************************
 *
 * �������ƣ�
 * MatrixSub()
 *
 * ������
 * unsigned char *matrixA ����;���A����ָ��
 * unsigned char *matrixB ����;���B����ָ��
 * int height �߶�
 * int width ���
 * unsigned char *result �;�������ָ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ���������������A��B���
 *
 **********************************************************************/
void transeform_wavelet::MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] + matrixB[h * width + w] - 128;
			result[h * width + w] = temp;
		}
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * MatrixInsertRow()
 *
 * ������
 * unsigned char *matrixInputOdd ���������в������ɵľ���
 * unsigned char *matrixInputEven ����ż���в������ɵľ���
 * unsigned char *matrixOutput ���������������ָ��
 * int heightInput �������߶�
 * int widthInput ���������
 * int widthOutput ���������
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * �������������в�����
 *
 **********************************************************************/
void transeform_wavelet::MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
										 unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput)
{
	int h, w;
	for (h=0; h<heightInput*2; h++)
	{
		for (w=0; w<widthInput; w++)
		{
			if (h%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[(int)(h/2) * widthInput + w];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[(int)(h/2) * widthInput + w];
			}		
		}
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * MatrixInsertLine()
 *
 * ������
 * unsigned char *matrixInputOdd ���������в������ɵľ���
 * unsigned char *matrixInputEven ����ż���в������ɵľ���
 * unsigned char *matrixOutput ���������������ָ��
 * int heightInput �������߶�
 * int widthInput ���������
 * int widthOutput ���������
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * �������������в�����
 *
 **********************************************************************/
void transeform_wavelet::MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
										  unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput)
{
	int h, w;
	for (h=0; h<heightInput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			if (w%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[h * widthInput + w/2];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[h * widthInput + w/2];
			}		
		}
	}
}

/**********************************************************************
 *
 * �������ƣ�
 * Trans()
 *
 * ������
 * unsigned char *inputData �����������ָ��
 * int inputHeight �������߶�
 * int inputWidth ���������
 * int lineByte �������ֽ���
 * int scale �ֽ�߶�
 * unsigned char *outputData �����������ָ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ����һ��С���ֽ�
 *
 **********************************************************************/
void transeform_wavelet::Trans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
	
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	//��ͼ���������в���
	this->MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, lineByte);
	//�в�֣��õ���Ƶ���Ƶ����
	this->MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ե�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�ֵõ�LL��LH
	this->MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ը�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�֣��õ�HL��HH
	this->MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	
	this->MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2),
		buff+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, lineByte);
	this->MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, lineByte);
	this->MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, lineByte);
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	
	scale--;
	//������LL���еݹ�ֽ�
	if (scale > 0)
	{
		this->Trans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2),
		imageHeight, lineByte);
	delete[] temp00;

	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}

/**********************************************************************
 *
 * �������ƣ�
 * SimpleWaveletTrans()
 *
 * ������
 * int scale �ֽ�߶ȣ�Ĭ��Ϊ1
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ��ͼ�����ݽ��и����߶ȵ�С���ֽ�
 *
 **********************************************************************/
void transeform_wavelet::SimpleWaveletTrans(int scale = 1)
{
	if (scale<1 || scale>5)
	{
		return;
	}
	if(m_pImgDataOut!=NULL){
		delete[] m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	this->m_nScale = scale;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	this->Trans(m_pImgData, m_imgHeight, m_imgWidth, lineByte, scale, m_pImgDataOut);
}

/**********************************************************************
 *
 * �������ƣ�
 * Reverse()
 *
 * ������
 * unsigned char *inputData �����������ָ��
 * int inputHeight �������߶�
 * int inputWidth ���������
 * int lineByte �������ֽ���
 * int scale �ֽ�߶�
 * unsigned char *outputData �����������ָ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ����һ��С���ع�
 *
 **********************************************************************/
void transeform_wavelet::Reverse(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte,
								 int scale, unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;


	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData, imageHeight, lineByte, temp10, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+(int)(imageWidth/2), imageHeight, lineByte, temp11, (imageHeight/2), (imageWidth/2));
	this->MatrixPlus(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp11, temp10, temp1, (imageHeight/2), (imageWidth/2), imageWidth);
	
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2), imageHeight, lineByte, temp00, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2),
		imageHeight, lineByte, temp01, (imageHeight/2), (imageWidth/2));
	
	scale--;
	//�ݹ��LL���ݽ����ع�
	if (scale > 0)
	{
		this->Reverse(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixPlus(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp01, temp00, temp2, (imageHeight/2), (imageWidth/2), imageWidth);

	this->MatrixPlus(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	this->MatrixInsertRow(temp1, temp2, buff, (imageHeight/2), imageWidth, lineByte);
	
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	delete[] temp00;

	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}

/**********************************************************************
 *
 * �������ƣ�
 * SimpleWaveletReverse()
 *
 * ������
 * void
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ����ӦС���ֽ�ϵ������С���ع�
 *
 **********************************************************************/
void transeform_wavelet::SimpleWaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pImgDataOut==NULL)
	{
		return;
	}
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	this->Reverse(m_pImgDataOut, m_imgHeight, m_imgWidth, lineByte, m_nScale, m_pImgDataOut);
}

/**********************************************************************
 *
 * �������ƣ�
 * ReplaceDIBForNew()
 *
 * ������
 * CSize size 
 * int nBitCount 
 * LPRGBQUAD lpColorTable ��ɫ��ָ��
 * unsigned char *pImgData ͼ������
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ��view��ͼ�����ݸ���
 *
 **********************************************************************/
void transeform_wavelet::ReplaceDIBForNew(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData)
{
	ReplaceDib(size, nBitCount,  lpColorTable, pImgData);

	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	m_nBitCountOut=m_nBitCount;
	 m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	m_pColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	memcpy(m_pColorTableOut,m_lpColorTable, sizeof(RGBQUAD)*m_nColorTableLengthOut);
}


/***********************************************************************
 *
 * �������ƣ�
 * H2Zero()
 *
 * ������
 * coeff ��������磺С��ϵ���ĸ�Ƶ����
 * cHeight �������������߶ȣ�
 * cWidth ��������������ȣ�
 * staticHeight ԭʼͼ��̶��߶�
 * staticWidth ԭʼͼ��Ĺ̶����
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * �Ը�Ƶϵ������
 *
 **********************************************************************/
void transeform_wavelet::H2Zero(double* coeff, int cHeight, int cWidth, 
						  int staticHeight, int staticWidth)
{
/*	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pImgDataOut==NULL)
	{
		return;
	}
	int lineByte=(m_imgWidth * m_nBitCount / 8+3)/4*4;


	int k = 1 << m_nScale;
	int h, w, s, d, m = 0;
	for (s = 1, d = 0; s <= m_nScale; s++)
	{
			d = m * lineByte + (m_imgWidth / (1 << s));
			for (h=0; h < m_imgHeight / (1 << s); h++)
			{
				for (w=0; w < m_imgWidth / (1 << s); w++)
				{
					//
					m_pImgDataOut[d + h * lineByte + w] = (BYTE)0;
				}
			}
			m += (m_imgHeight / (1 << s));
	}*/
	int i,j;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			*(coeff + i * staticWidth + j) =0;
		}
	}
}

/***********************************************************************
 *
 * �������ƣ�
 * Hard()
 *
 * ������
 * coeff ��������磺С��ϵ���ĸ�Ƶ����
 * cHeight �������������߶ȣ�
 * cWidth ��������������ȣ�
 * threshold ��ֵ
 * staticHeight ԭʼͼ��̶��߶�
 * staticWidth ԭʼͼ��Ĺ̶����
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ���������ֵʹ��Ӳ��ֵ����������ľ����з�
 *
 **********************************************************************/
void transeform_wavelet::Hard(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth)
{
	
	int i,j;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			if((*(coeff + i * staticWidth + j)>=0)&&(*(coeff + i * staticWidth + j) <= threshold) ||
				(*(coeff + i * staticWidth + j)<0)&&(*(coeff + i * staticWidth + j)*(-1) <= threshold))
			{
				*(coeff + i * staticWidth + j) =0;
			}
		}
	}
}

/***********************************************************************
 *
 * �������ƣ�
 * Soft()
 *
 * ������
 * coeff ��������磺С��ϵ���ĸ�Ƶ����
 * cHeight �������������߶ȣ�
 * cWidth ��������������ȣ�
 * threshold ��ֵ
 * staticHeight ԭʼͼ��̶��߶�
 * staticWidth ԭʼͼ��Ĺ̶����
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ���������ֵʹ������ֵ����������ľ����з�
 *
 **********************************************************************/
int transeform_wavelet::GetSgn(double number)
{
	if(number ==0)
	{
		return 0;
	}
	else if(number >0)
	{
		return 1;
	}
	else 
	{
		return (-1);
	}
}
void transeform_wavelet::Soft(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth)
{
	
	int i,j;
	double temp;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			if((*(coeff + i * staticWidth + j)>=0)&&(*(coeff + i * staticWidth + j) <= threshold) ||
				(*(coeff + i * staticWidth + j)<0)&&(*(coeff + i * staticWidth + j)*(-1) <= threshold))
			{
				*(coeff + i * staticWidth + j) =0;
			}
			else
			{
				temp =*(coeff + i * staticWidth + j);
				if(temp<0)
				{
					temp *=(-1);
				}
				temp -=threshold;
				*(coeff + i * staticWidth + j) = GetSgn(*(coeff + i * staticWidth + j))*temp;
			}
		}
	}
}


//����sym8�˲���ϵ������С���ֽ�
/***********************************************************************
 *
 * �������ƣ�
 * Sym8WaveletTrans()
 *
 * ������
 * scale �ֽ�߶ȣ�Ĭ��Ϊ1
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ʹ����wavelet_3_8.lib��
 *
 **********************************************************************/
void transeform_wavelet::Sym8WaveletTrans(int scale = 1)
{
	if (scale<1 || scale>5)
	{
		return;
	}
	if(m_pImgDataOut!=NULL){
		delete[] m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	this->m_nScale = scale;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	int N= m_imgWidth;
	int M= m_imgHeight; 
	
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
	}
	pCoeff = new double[M*N];
	double* coeff1 = new double[M*N];

	::Copy(m_pImgData, M, N, coeff1, M, N);
	//fan->zheng
	::ReverseDIBBits(coeff1, M, N);

	if(Wavelet(coeff1, M, N, scale, 4, pCoeff, M, N))
	{
		//zheng->fan->save
		::ReverseDIBBits(pCoeff, M, N);
		::CopyPositive(pCoeff,M, N,m_pImgDataOut,M, N);
	}
	delete[] coeff1;
}

//����sym8�˲���ϵ������С���ع�
/***********************************************************************
 *
 * �������ƣ�
 * Sym8WaveletReverse()
 *
 * ������
 * void
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ʹ����wavelet_3_8.lib��
 *
 **********************************************************************/
void transeform_wavelet::Sym8WaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pImgDataOut==NULL)
	{
		return;
	}
	if (pCoeff == NULL)
	{
		return;
	}
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	int N= m_imgWidth;
	int M= m_imgHeight;
	::ReverseDIBBits(pCoeff, M, N);
	if(InverseWavelet(pCoeff, M, N, m_nScale, 4, M, N))
	{
		//zheng->fan
		::ReverseDIBBits(pCoeff, M, N);
		::CopyPositive(pCoeff,M, N,m_pImgDataOut,M, N);
	}
}

//��Ƶϵ�����㷽��ȥ��������ӿ�
void transeform_wavelet::SetH2Zero()
{
	Sym8WaveletTrans(2);
	int N= m_imgWidth;	//���
	int M= m_imgHeight;	//����
	int scale =0,t=1,start =0;
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		//
		::ReverseDIBBits(pCoeff,M,N);
		
		H2Zero(pCoeff + N/t,M/t,N/t,M,N);
		H2Zero(pCoeff + N*(M/t),M/t,N/t*2,M,N);
/*		H2Zero(pCoeff + N*(M/t) + N/t,M/t,N/t,M,N);*/
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	Sym8WaveletReverse();
	
	return;
}

//Ӳ��ֵ����ȥ��Ķ���ӿ�
void transeform_wavelet::HardThreshold()
{
	Sym8WaveletTrans(2);
	int N= m_imgWidth;	//���
	int M= m_imgHeight;	//����
	int k,scale =0,t=1,start =0;
	double Threshold;
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		double* thresholdTemp = new double[M/t*N/t];
		k =start+N/t;
		::Copy(pCoeff+k,M,N,thresholdTemp,M/t,N/t);
		Threshold =::GetThreshold(thresholdTemp, M/t, N/t, 1);
		Threshold += 10;
		
		delete[] thresholdTemp;
		//
		::ReverseDIBBits(pCoeff,M,N);
		
		Hard(pCoeff + N/t,M/t,N/t,Threshold,M,N);
		Hard(pCoeff + N*(M/t),M/t,N/t*2,Threshold,M,N);
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	Sym8WaveletReverse();
}

//����ֵ����ȥ��Ķ���ӿ�
void transeform_wavelet::SoftThreshold()
{
	Sym8WaveletTrans(2);
	int N= m_imgWidth;	//���
	int M= m_imgHeight;	//����
	int k,scale =0,t=1,start =0;
	double Threshold;
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		double* thresholdTemp = new double[M/t*N/t];
		k =start+N/t;
		::Copy(pCoeff+k,M,N,thresholdTemp,M/t,N/t);
		Threshold =::GetThreshold(thresholdTemp, M/t, N/t, 1);
//		Threshold += 10;
		
		delete[] thresholdTemp;
		//
		::ReverseDIBBits(pCoeff,M,N);
		
		Soft(pCoeff + N/t,M/t,N/t,Threshold,M,N);
		Soft(pCoeff + N*(M/t),M/t,N/t*2,Threshold,M,N);
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	Sym8WaveletReverse();
}