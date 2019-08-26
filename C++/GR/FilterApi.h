//FilterApi.h

#ifndef _FILTERAPI_H_
#define _FILTERAPI_H_
//////////////////////////////////////////////////////////////////////////////////////////////
/*
*
*	���ܣ�Ѱ����λֵ
*
*/
/*
double FindMedianValue(double* lpbArray, int iArrayLen);
*/
/*
*
*	���ܣ���ֵ�˲�
*
*/
/*
BOOL MedianFilter(HDIB hDIB,	double* lpDIBBits, int iFilterH, int iFilterW, int iFilterCX, 
						 int iFilterCY);
*/
//////////////////////////////////////////////////////////////////////////////////////////////

/*
*
*	���ܣ�����������һ������
*
*/
int Partition(int* L, int low, int high);

/*
*
*	���ܣ���������
*
*/
void QSort (double* L, int low, int high);

/*
*
*	���ܣ���С��ϵ������λֵ
*
*/
double MedianCoeff(double* coeff, int h, int w);

/*
*
*	���ܣ�ȡ��һ�����ķ���
*
*/
int GetSgn(int number);

/*
* ��������GetThreshold
*
* ���ܣ������������ֵ
*
* ������
* @param coeff ȡ��ֵ����������磺С��ϵ���ĸ�Ƶ����
* @param cHeight �������������߶ȣ�
* @param cWidth ��������������ȣ�
* @param type ��ֵ��ʽ���0Ϊsqrt(2*log(n))*q��
*							1Ϊ0.3*sqrt(2*log(n))*q��
*							2Ϊsqrt(2*log(n)/n)*q��
*							3Ϊ3*q��
*							����qΪ�������logΪ��Ȼ������
*
* ���룺coeff
*
* �����return
*
* ����ֵ����ֵ
*/
double GetThreshold(double* coeff, int h, int w, int type/* =1*/);

/*
* ��������SoftThreshold
*
* ���ܣ����������ֵʹ������ֵ����������ľ����з�
*
* ������
* @param coeff ��������磺С��ϵ���ĸ�Ƶ����
* @param cHeight �������������߶ȣ�
* @param cWidth ��������������ȣ�
* @param threshold ��ֵ
* @param staticHeight ͼ��̶��߶ȣ�һ��ͬmHeight��
* @param staticWidth ͼ��Ĺ̶���ȣ�һ��ͬmWidth��
*
* ���룺coeff
*
* �����coeff
*
* ����ֵ����
*/
void SoftThreshold(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth);

/*
* ��������HardThreshold
*
* ���ܣ����������ֵʹ��Ӳ��ֵ����������ľ����з�
*
* ������
* @param coeff ��������磺С��ϵ���ĸ�Ƶ����
* @param cHeight �������������߶ȣ�
* @param cWidth ��������������ȣ�
* @param threshold ��ֵ
* @param staticHeight ͼ��̶��߶ȣ�һ��ͬmHeight��
* @param staticWidth ͼ��Ĺ̶���ȣ�һ��ͬmWidth��
*
* ���룺coeff
*
* �����coeff
*
* ����ֵ����
*/
void HardThreshold(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth);

/*
*
*	���ܣ�����ģֵ����ǳ�ʼ��ģ����ֵ
*
*/
void FindMoMax(int scale, double** pMo, int cHeight, int cWidth,
					  unsigned char* pMoMax1/* =NULL*/, unsigned char* pMoMax2/* =NULL*/, unsigned char* pMoMax3/* =NULL*/);

/*
* ��������GetMoMax
*
* ���ܣ����ҵ�ģֵ����ǣ�Ȼ���ʼ�����߶�ģ����ֵ����pMoMax1��pMoMax2, pMoMax3��
*
* ������
* @param coeff ����С��ϵ������
* @param cHeight �������������߶ȣ�
* @param cWidth ��������������ȣ�
* @param scaleSum �ֽ�߶�
* @param threshold1 �߶�1��ģ����ֵ��ֵ����ʼΪ0
* @param threshold2 �߶�2��ģ����ֵ��ֵ����ʼΪ0
* @param threshold3 �߶�3��ģ����ֵ��ֵ����ʼΪ0
* @param pMoMax1 �߶�1��ģ����ֵ���󣬱���������Ϊ��ָ������ռ�
* @param pMoMax2 �߶�2��ģ����ֵ���󣬱���������Ϊ��ָ������ռ�
* @param pMoMax3 �߶�3��ģ����ֵ���󣬱���������Ϊ��ָ������ռ�
*
* ���룺coeff, pMoMax1, pMoMax2, pMoMax3
*
* �����pMoMax1, pMoMax2, pMoMax3
*
* ����ֵ����
*/
void GetMoMax(double* coeff, int cHeight, int cWidth, int scaleSum,
					 double threshold1/* =0*/, double threshold2/* =0*/, double threshold3/* =0*/, 
					 unsigned char* pMoMax1/* =NULL*/, unsigned char* pMoMax2/* =NULL*/, unsigned char* pMoMax3/* =NULL*/);


#endif //_FILTERAPI_H_