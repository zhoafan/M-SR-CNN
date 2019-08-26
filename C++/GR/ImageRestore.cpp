
#include "stdafx.h"
#include "ImageRestore.h"
#include "ImageRestoreExt.h"

#include <math.h>
#include "FourierTransform.h"
#define PI 3.1415926

#pragma comment(lib,"FourierTransformLib8")
#pragma comment(lib,"ImageRestoreLib8")


ImageRestore::ImageRestore(void) : 
mask(NULL),mask_width(0),mask_height(0)
{
}

ImageRestore::~ImageRestore(void)
{
	if(mask != NULL)
		free(mask);
}


/***************************************************************************
*  MapRestoreByteVar
*  ���ܣ�ͶӰ���˶�ģ���ָ�
*  ������
*  imgBufIn    ����ͼ������
*  imgWidth    ͼ����
*  imgHeight   ͼ��߶�
*  pxWidth     ���ؿ��
*  cycleCount  ��������
*  ����ԭ������ �ɾ���Ĳ��� H * F���������� 
***************************************************************************/
void ImageRestore::MapRestoreByteVar(unsigned char *imgBufIn ,int imgWidth, int imgHeight,int pxWidth ,int cycleCount)
{

	ImageRestoreExt ire8;
	ire8.GeneratePSF(this->length,this->angle);
	ire8.MapRestoreByteVar(imgBufIn,imgWidth,imgHeight,pxWidth,cycleCount);
	

}


/***************************************************************************
* WienerRestoreByteVar
* ���ܣ�ά���˲��˶�ģ���ָ�
* ����˵����
* imgBufIn  ����ͼ�����ݣ�8λ�Ҷȣ�
* imgWidth  ͼ����
* imgHeight ͼ��߶�
* pxWidth   ͼ��λ�� ����Ϊ1��
* k         ������0- 1��
***************************************************************************/
bool ImageRestore::WienerRestoreByteVar(unsigned char *imgBufIn,int imgWidth,int imgHeight,int pxWidth,double k)
{
	if(imgBufIn == NULL || mask == NULL)
		return false;
	
	BYTE* Y = imgBufIn;

	int w=1,h=1;
	//������ڵ���ԭʼͼ���С��2����
	while(w < imgWidth)
	{
		w *= 2;
	}
	
	while(h < imgHeight)
	{
		h *= 2;
	}

	complex<double> *freqData= new complex<double>[w * h * pxWidth];
	FourierTransform ftm;

	//��ͼ����и���Ҷ�任
	ftm.ImageFourierEx(imgBufIn, imgWidth, imgHeight,  freqData,1);


	//ģ������
	complex<double> *timeMaskData;
	complex<double> *freqMaskData;
	timeMaskData = new complex<double>[w * h * pxWidth];
	freqMaskData = new complex<double>[w * h * pxWidth];

	int i = 0,j = 0;

	int acce = 0;

	//���ģ�壬����ģ��Ϊ2���������ݣ�����Ĳ������0	
	for(i = 0; i < h; i++){
		for(j = 0; j < w * pxWidth; j= j + 1){
			if(i < mask_height && j < mask_width * pxWidth){

				double Value = *((double*)mask + acce);
				acce ++ ;
				timeMaskData[w * i * pxWidth + j] = complex<double>(Value, 0.0f);
			}
			else{

				timeMaskData[w * i * pxWidth + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}
	
	//��ģ����и���Ҷ�任
	if (ftm.Fourier(timeMaskData, w, h, freqMaskData,pxWidth) == FALSE){

		return false;	
	}

	double conf=0;
	//ά���˲�
	for(i = 0; i < h; i++){
		for(j = 0; j < w * pxWidth ; j = j + 1){

			complex<double> mask_item =freqMaskData[w * i * pxWidth + j];
			conf=(abs(mask_item) * abs(mask_item)) / ((abs(mask_item) * abs(mask_item)) + k);
			freqData[w * i * pxWidth + j]   = (freqData[w * i * pxWidth + j] * complex<double>(conf,conf) ) / mask_item;

		}
	}

	//���˲����������任
	if (ftm.IFourier(
		
		Y, imgWidth, imgHeight, freqData,1) == FALSE){
		return false;
	}

	delete[] freqMaskData;
	delete[] timeMaskData;
	delete[] freqData;

	return true;
}


/**************************************************************************
*������
*    InverseFilterByte()
*
*������
*   unsigned char *imgBufIn   -����ͼ�񻺳�
*   int imgWidth              -ͼ����
*   int imgHeight             -ͼ��߶�
*   int pxWidth               -���ؿ��(8λ�Ҷ�ͼ���뽫�ò�������Ϊ 1)
*
*����ֵ
*    ��
*
*˵�����ú���������ͼ�� imgBufIn��Ϊ����ͼ�񣬲����ռ���õĵ���ɢ��������
       ���˲��ָ���ע���ڵ��øú���ǰ������ʹ��GeneratePSF��������ɢ������ 
***************************************************************************/

bool ImageRestore::InverseFilterByte(unsigned char *imgBufIn,int imgWidth,int imgHeight,int pxWidth)
{

	if(imgBufIn == NULL || mask == NULL)
		return false;

	BYTE* Y = imgBufIn;

	int w=1,h=1;
	// ������ڵ���ԭʼͼ���С��2����
	while(w < imgWidth)
	{
		w *= 2;
	}
	
	while(h < imgHeight)
	{
		h *= 2;
	}

	complex<double> *freqData= new complex<double>[w * h * pxWidth];


	FourierTransform ftm;
	// ��ͼ����и���Ҷ�任��freqDataΪ������
	ftm.ImageFourier(imgBufIn, imgWidth, imgHeight,  freqData,pxWidth);


	// �������ɢ���������Ƶ��ռ�
	complex<double> *timeMaskData = new complex<double>[w * h * pxWidth];
	complex<double> *freqMaskData = new complex<double>[w * h * pxWidth];


	int i = 0, j = 0;
	// ������ɢ�����ռ䣬����ģ��Ϊ2���������ݣ�����Ĳ������0
	int acce = 0;
	for(i = 0; i < h; i++){
		for(j = 0; j < w * pxWidth; j= j + 1){
			if(i < mask_height && j < mask_width * pxWidth){

				double Value = *((double*)mask + acce);
				acce ++ ;
				timeMaskData[w * i * pxWidth + j] = complex<double>(Value, 0.0f);
			}
			else{
				timeMaskData[w * i * pxWidth + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}
	
	// ��ģ����и���Ҷ�任
	if (ftm.Fourier(timeMaskData, w, h, freqMaskData,pxWidth) == FALSE)
		return false;	

	double conf=0;
	
	// ���˲�
	for(i = 0; i < h; i++){
		for(j = 0; j < w * pxWidth ; j = j + 1){
			complex<double> mask=freqMaskData[w * i * pxWidth + j];

			if(mask.real() * mask.real() + mask.imag() * mask.imag()!= 0)
				freqData[w * i * pxWidth + j] = freqData[w * i * pxWidth + j] / mask;
			else
				freqData[w * i * pxWidth + j] = 0;

		}
	}

	// ���˲����������任
	if (ftm.IFourier(Y, imgWidth, imgHeight, freqData, pxWidth) == FALSE)
		return false;

	// �ͷŻ�����
	delete[] timeMaskData;
	delete[] freqMaskData;
	delete[] freqData;


	return true;

}


/***************************************************************************
* GeneratePSF
* ���ܣ���������ɢ��������
* length   ģ������
* angle    ģ���Ƕ�
* ˵��������������Ϣ�Ļָ����������ȸ���ģ����Ϣ��������ɢ����
*       ��ʹ�����˲��Ȼָ���������˶�ģ���ָ���
***************************************************************************/
void ImageRestore::GeneratePSF(double length, double angle)
{
	this->angle = angle;
	this->length = length;

	float half = length;
	double phi = (double)(angle/180)*PI;  //ת��Ϊ����
	double cosphi = cos(phi);
	double sinphi = sin(phi);
	int xsign = cosphi?(cosphi>0?1:-1):0;

	if(cosphi<0) 
		xsign = -1;

	//�߿��
	int linewdt = 1;

	//����һ��ͼ��ĳ��Ϳ�
	int sx = (int)(half * cosphi * xsign + linewdt + 0.5);
	int sy = (int)(half * sinphi + linewdt + 0.5);
	mask_width = sx;
	mask_height = sy;

	//�������ռ�
	//������Ӧ���ڴ�ռ�
	if(mask != NULL)
		free(mask);
	
	mask = (double*)malloc(sizeof(double)*sx*sy);

	double **dist2line_matrix = (double**)malloc(sizeof(double*)*sy); //����֯

	int i = 0,j = 0;
	//ָ��ָ��ÿһ�е��׵�ַ
	if(xsign == 1){
		for(i = 0; i< sy ; i++){
			dist2line_matrix[i] = mask + i * sx;
		}
	}
	else{
		for(i = sy - 1; i >= 0 ; i--){
			dist2line_matrix[i] = mask + (sy - i - 1) * sx;
		}
	}

	//���ȶ������
	double rad = 0;

	//--���ǵ�������90��180֮�䣬ֻ����x����ķ�������ڿ����߼���ֻ�����x����˳�򼴿�
	int x_position = 0,x_start = 0;

	x_start = 0;

	//�����һ���㵽��ת�ߵ���̾���
	//���봹ֱ����ת��
	for(i = 0; i < sy ; i ++){

		x_position = x_start;  

		for(j = 0; j < sx; j++){
			//�߾���
			dist2line_matrix[i][j] = i * abs(cosphi) - j * sinphi;
			//�뾶����
			rad = sqrt((double)(i * i + j * j));

			if(rad >= half && abs(dist2line_matrix[i][j]) <= linewdt){
				int x2lastpix = half - abs((x_position + dist2line_matrix[i][j] * sinphi) / cosphi);
				dist2line_matrix[i][j] = sqrt(dist2line_matrix[i][j] * dist2line_matrix[i][j] + x2lastpix * x2lastpix);
				
			}
			
			dist2line_matrix[i][j] = linewdt - abs(dist2line_matrix[i][j]);
			
			if(dist2line_matrix[i][j] < 0) dist2line_matrix[i][j]=0;

			x_position ++;

		}
	}
	

	double sum = 0;
	for(i = 0; i < sy ; i ++){
		for(j = 0; j < sx; j++){
			sum += dist2line_matrix[i][j];
		}
	}

	//�Ծ������ݽ��й�һ������
	for(i = 0; i < sy ; i ++){
		for(j = 0; j < sx; j++){
			dist2line_matrix[i][j] /= sum;
		}
	}

	//ɾ����ά����
	delete[] dist2line_matrix;

	return;
}
