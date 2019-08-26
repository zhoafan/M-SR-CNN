//WaveletApi.h

#ifndef _WAVELETAPI_H_
#define _WAVELETAPI_H_

/*
*
*	���ܣ�����
*
*/
void Convolution(double *signal, int signLen, double *filter, int filterLen, double *conv);

/*
*
*	���ܣ�������
*
*/
void Iconvolution(double *signal, int signLen, double *filter, int filterLen, double *conv);

/*
*
*	���ܣ�����ת��
*
*/
void Revert(double *matrix1, int mHeight, int mWidth, double *matrix2);

/*
*
*	���ܣ����²���
*
*/
void Downsample(double *matrix1, int mHeight, int mWidth, double *matrix2);

/*
*
*	���ܣ����ϲ���
*
*/
void Upsample(double *matrix1, int mHeight, int mWidth, double *matrix2);

/*
*
*	���ܣ�����ӷ�
*
*/
void Plus(double *matrix1, int mHeight, int mWidth, double *matrix2);

/*
*
*	���ܣ�������
*
*/
void Copy_f2f(double *original, int mHeight, int mWidth, double *newmatrix, int nHeight, int nWidth);

/*
* ���ܣ�Mallat�����С���ֽ�
*/
BOOL MallatDWT(double *matrix, int mHeight, int mWidth, int nscale,
					  double *pFilterLow, double *pFilterHigh, int filterLength, double *coeff, int staticHeight, int staticWidth);


/*
* ���ܣ�Mallat�����С���ع�
*/
BOOL MallatIDWT(double *coeff, int mHeight, int mWidth, int nscale,
					   double *pFilterLow, double *pFilterHigh, int filterLength, int staticHeight, int staticWidth);

/*
* ��������Wavelet
*
* ���ܣ�Mallat�����С���ֽ�
*
* ������
* @param matrix Ҫ�ֽ�Ķ�ά����
* @param mHeight �������������߶ȣ�
* @param mWidth ��������������ȣ�
* @param nscale �ֽ�߶�
* @param filterType �˲������0Ϊ����B������1Ϊ����B������2Ϊdb9/7��3Ϊsym7��4Ϊsym8��
* @param coeff �ֽ��ϵ������
* @param staticHeight ͼ��̶��߶ȣ�һ��ͬmHeight��
* @param staticWidth ͼ��Ĺ̶���ȣ�һ��ͬmWidth��
*
* ���룺matrix
*
* �����coeff
*
* ����ֵ��TRUEΪ�ֽ�ɹ�
*/
BOOL Wavelet(double *matrix, int mHeight, int mWidth, int nscale,
					  int filterType, double *coeff, int staticHeight, int staticWidth);

/*
* ��������InverseWavelet
*
* ���ܣ�Mallat�����С���ع�
*
* ������
* @param coeff Ҫ�ع��Ķ�άС��ϵ������
* @param mHeight �������������߶ȣ�
* @param mWidth ��������������ȣ�
* @param nscale �ֽ�߶�
* @param filterType �˲������0Ϊ����B������1Ϊ����B������2Ϊdb9/7��3Ϊsym7��4Ϊsym8��
* @param staticHeight ͼ��̶��߶ȣ�һ��ͬmHeight��
* @param staticWidth ͼ��Ĺ̶���ȣ�һ��ͬmWidth��
*
* ���룺coeff
*
* �����coeff
*
* ����ֵ��TRUEΪ�ع��ɹ�
*/
BOOL InverseWavelet(double *coeff, int mHeight, int mWidth, int nscale,
						   int filterType, int staticHeight, int staticWidth);

#endif //_WAVELETAPI_H_