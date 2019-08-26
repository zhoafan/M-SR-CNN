#ifndef _INSIDE_VISUAL_CPP_CImgEnhance
#define _INSIDE_VISUAL_CPP_CImgEnhance
#include "ImageCenterDib.h"

//����ͼ����ǿ��
class CImgEnhance:public ImgCenterDib  
{
public:
    int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	LPRGBQUAD m_lpColorTableOut;
	int m_nColorTableLengthOut;
public:
	//���캯��
	CImgEnhance();
	CImgEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
    ~CImgEnhance();
	
	//��ӽ�������
	void AddPepperSaltNoise();
	
	//��Ӹ�˹����
	void AddRandomNoise();
    
	//���þ�ֵģ�����ͼ��ƽ��
	void AvgTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef);
	
	//��ֵ�˲�
	BYTE FindMedianValue(unsigned char* lpbArray,int iArrayLen);
	void MedianFilter(int iFilterH, int iFilterW, int iFilterCX, int iFilterCY);
	
	//������˹�� ת��Ϊģ������
	void LapTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef);
	
	//�ݶ���
	void GradSharp(BYTE bThre);
	
	//ѡ����ģƽ��
	void ChooseMask();
	
	//ͼ�������SNR
	void SNR(unsigned char* m_pImgSrcData);
	
	//ͼ���ֵ�����
	CString PSNR(unsigned char* m_pImgSrcData);

	//ͼ���ȫ���-�ݶ���ǿ
	void TvGrad(int c,int r1,int r2,float a,float b);

	void Testdiejia();
	
};

#endif // _INSIDE_VISUAL_CPP_CImgEnhance