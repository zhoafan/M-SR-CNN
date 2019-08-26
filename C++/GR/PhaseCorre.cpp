#include "StdAfx.h"

#include "PhaseCorre.h"

#include "FourierTransform.h"

#define PI 3.1415926

//Ƶ��λ��ת��
void FFT2Shiftd(double * FD,int w,int h){

	double * temp = new double[w*h];
	memcpy(temp,FD,sizeof(double)*w*h);  //����ת��


	for(int i = 0; i < h; i++)
	{
		// ��
		for(int j = 0; j < w; j=j+1)
		{
			double dataTemp = temp[w * i + j];
			//double dTemp = sqrt(FD[w * i * 3 + j*3].real() * FD[w * i * 3 + j*3].real() + 
			//	         FD[w * i * 3 + j*3].imag() * FD[w * i * 3 + j*3].imag()) / 50;
			
			// ָ��DIB��(i<h/2 ? i+h/2 : i-h/2)�У���(j<w/2 ? j+w/2 : j-w/2)�����ص�ָ��
			// �˴���ֱ��ȡi��j����Ϊ�˽��任���ԭ���Ƶ�����
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			double* lpSrc = (double*)FD + w * (h - 1 - (i < h / 2 ? i + h / 2: i - h/2)) + (j < w/2 ? j + w/2: j - w/2);
			//unsigned char* lpSrc = (unsigned char*)Y + lLineBytes * 
			//	(lHeight - 1 - (i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2)*3;
			
			//�������
			*(lpSrc) = dataTemp;
			// ����Դͼ��
			//* (lpSrc) = (BYTE)(dTemp);
			//* (lpSrc+1) = (BYTE)(dTemp);
			//* (lpSrc+2) = (BYTE)(dTemp);
		}
	}

	delete temp;
}
//Ƶ��λ��ת��
void FFT2Shift(complex<double> * FD,int w,int h){

	complex<double> * temp = new complex<double>[w*h];
	memcpy(temp,FD,sizeof(complex<double>)*w*h);  //����ת��


	for(int i = 0; i < h; i++)
	{
		// ��
		for(int j = 0; j < w; j=j++)
		{
			complex<double> dataTemp = temp[w * i + j];
			//double dTemp = sqrt(FD[w * i * 3 + j*3].real() * FD[w * i * 3 + j*3].real() + 
			//	         FD[w * i * 3 + j*3].imag() * FD[w * i * 3 + j*3].imag()) / 50;
			
			// ָ��DIB��(i<h/2 ? i+h/2 : i-h/2)�У���(j<w/2 ? j+w/2 : j-w/2)�����ص�ָ��
			// �˴���ֱ��ȡi��j����Ϊ�˽��任���ԭ���Ƶ�����
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			complex<double>* lpSrc = (complex<double>*)FD + w * (h - 1 - (i<h/2 ? i+h/2:i-h/2)) + (j < w/2 ? j + w/2: j - w/2);
			//unsigned char* lpSrc = (unsigned char*)Y + lLineBytes * 
			//	(lHeight - 1 - (i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2)*3;
			
			//�������
			*(lpSrc) = dataTemp;
			// ����Դͼ��
			//* (lpSrc) = (BYTE)(dTemp);
			//* (lpSrc+1) = (BYTE)(dTemp);
			//* (lpSrc+2) = (BYTE)(dTemp);
		}
	}

	delete temp;
}


/**********************************************************
* ��������
*     PhaseCorrelation
* ������
*     unsigned char * aRegion-ͼ��A
*     unsigned char * bRegion-ͼ��B
*     int dWidth-ͼ����
*     int dHeight-ͼ��߶�
*     int *xpos-����x����λ��
*     int *ypos-����y����λ��
*
* ˵����
*     �ú���������λ��ط���������ͼ�������׼�����ص�����λ�þ������ƽ������ֵ
************************************************************/
void PhaseCorrelation(unsigned char * aRegion ,unsigned char *bRegion,int dWidth,int dHeight,int *xpos,int *ypos)
{
	//Ƶ������
	complex<double> *aFD= new complex<double>[dWidth * dHeight];
	complex<double> *bFD= new complex<double>[dWidth * dHeight];

	FourierTransform ft;

	ft.ImageFourier(aRegion,dWidth,dHeight,aFD,1); //ͼ��A�ĸ���Ҷ�任����
	ft.ImageFourier(bRegion,dWidth,dHeight,bFD,1); //ͼ��B�ĸ���Ҷ�任����

	FFT2Shift(aFD,dWidth,dHeight); //
	FFT2Shift(bFD,dWidth,dHeight); //

	//��λ���
	complex<double> *gFD= new complex<double>[dWidth * dHeight];
	
	int i;
	for(i = 0; i < dHeight ; i++){
		for(int j = 0; j < dWidth ; j ++){
			complex<double> temp =aFD[i * dWidth + j] * complex<double>(bFD[i * dWidth +j].real(),-bFD[i * dWidth + j].imag());
			gFD[i * dWidth + j] = temp/abs(temp);
		}
	}
	//����Ҷ�任����Ϣ���ڴ洦��
	delete[] aFD;
	delete[] bFD;

	//�渵��Ҷ�任
	double * gTD = (double*)malloc(sizeof(double)*dWidth*dHeight);
	ft.IFourier(gTD, dWidth, dHeight, gFD,1);
	delete[] gFD; //��λ���Ƶ���ڴ��ͷ�
	FFT2Shiftd(gTD,dWidth,dHeight);

	//���������ֵ
	double max = -1000;
	*xpos = 0,*ypos = 0;
	for(i = 0; i<dHeight ; i++){
		for(int j = 0; j<dWidth; j ++){

			if(gTD[i * dWidth + j] > max){
				max = gTD[i * dWidth + j];
				*xpos = j;
				*ypos = i;
			}
		}
	}

	//��λ���Ƶ����任ͼ���ڴ��ͷ�
	free(gTD);	
}

/***********************************************************************
* �������ƣ�
*     PhaseAngleDiff
*
* ����������
*     unsigned char * image1-ͼ��1
*     unsigned char * image2-ͼ��2
*     int imgWidth-ͼ����
*     int imgHeight-ͼ��߶�
* ˵����
*     ʹ����ǲ�����˶�ʸ�����
***********************************************************************/
void PhaseAngleDiff(unsigned char * image1,unsigned char * image2, int imgWidth,int imgHeight)
{
	//Ƶ����Ϣ
	complex<double> *aFD= new complex<double>[imgWidth * imgHeight];
	complex<double> *bFD= new complex<double>[imgWidth * imgHeight];

	FourierTransform ft;

	//��ͼ����и���Ҷ�任
	ft.ImageFourier(image1,imgWidth,imgHeight,aFD,1); // ͼ��1����Ҷ�任
	FFT2Shift(aFD,imgWidth,imgHeight);  // �ƶ�Ƶ��
	ft.ImageFourier(image2,imgWidth,imgHeight,bFD,1); // ͼ��2����Ҷ�任
	FFT2Shift(bFD,imgWidth,imgHeight);	// �ƶ�Ƶ��

	//����ǲ�
	float angle1,angle2;
	float *diff = new float[imgWidth * imgHeight]; // ��ǲ�
	for(int i = 0 ; i < imgWidth * imgHeight; i ++){
		angle1 = atan2(aFD[i].imag(),aFD[i].real());
		angle2 = atan2(bFD[i].imag(),bFD[i].real());
		int n = floor(angle1 / (2 * PI));
		diff[i] = angle2 - (angle1 - n * 2 * PI);
		n = floor(diff[i] / (2 * PI));
		diff[i] = diff[i] - n * 2 * PI;
	}
	delete[] aFD,bFD;  //�ͷ�Ƶ������
	float *xline = new float[imgWidth];
	float *yline = new float[imgHeight];
	//��x����ӳ����ǲ�
	for(i = 0; i < imgWidth ; i++)
	{
		xline[i] = diff[(imgHeight / 2) * imgWidth + i];
	}
	//��y����ӳ����ǲ�
	for(i = 0; i < imgHeight ; i++)
	{
		yline[i] = diff[i * imgWidth + imgWidth / 2];
	}
	delete[] diff;
	float currcx = 0;
	float currcy = 0;
	//unwrap���������
	float *xlinew = new float[imgWidth];
	float *ylinew = new float[imgHeight];
	//ͬʱ��x��y����ӳ�����ǲ����unwrap
	int winWidth = 20; //��Ϣ���ڴ�С
	for(i = imgWidth / 2 - winWidth ;i < imgWidth / 2 + winWidth; i ++)
	{
		if(abs(xline[i] - (xline[i - 1])) > 6.0){
			if(xline[i] - (xline[i - 1]) > 0){
			 currcx -= 2 * PI;
			}else
				currcx += 2 * PI;
		}
		if(abs(yline[i] - (yline[i - 1])) > 6.0){
			if(yline[i] - (yline[i - 1]) > 0){
				currcy -= 2 * PI;
			}else
				currcy += 2 * PI;
		}
		xlinew[i] = xline[i] + currcx;
		ylinew[i] = yline[i] + currcy;
	}
	delete[] xline,yline;

	float xvar = 0,yvar = 0; 
	CString str;
	xvar = xlinew[imgWidth / 2 - winWidth];// + fabs(6.0 - (int)xlinew[imgWidth / 2 + winWidth - 1]);
	yvar = ylinew[imgWidth / 2 - winWidth];// + fabs(6.0 - (int)ylinew[imgWidth / 2 + winWidth - 1]);
	
	str.Format("����ڵ�һ��ͼ��\n x�����ƶ�Ϊ%d��y�����ƶ�Ϊ%d��",int(xvar+0.5),int(yvar+0.5));
	CString temp;
	for(i = imgWidth / 2 - winWidth ;i < imgWidth / 2 + winWidth; i ++)
	{
		//temp.Format("(%f %f)\n",xlinew[i],ylinew[i]);
		//str.Insert(str.GetLength(),temp.GetBuffer(temp.GetLength()));
		TRACE("%f %f;\n",xlinew[i],ylinew[i]);
	}
	AfxMessageBox(str.GetBuffer(str.GetLength()));

	//��ylinew��xline����С������ϣ���б��
	//������	
	delete[] xlinew,ylinew; 
}
