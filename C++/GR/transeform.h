#ifndef _INSIDE_VISUAL_CPP_WLTTRANSFORM
#define _INSIDE_VISUAL_CPP_WLTTRANSFORM
#include "ImageCenterDib.h"

//С���任��
class transeform_wavelet : public ImgCenterDib
{
public:
	int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	LPRGBQUAD m_pColorTableOut;
	int m_nColorTableLengthOut;
	double* pCoeff;
public:
	void ReplaceDIBForNew(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	//�ֽ�߶�
	int m_nScale;
	transeform_wavelet();
	transeform_wavelet(CSize size, int nBitCount, LPRGBQUAD pColorTable, unsigned char *pImgData);
	~transeform_wavelet();
	
	//����������
	void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
		unsigned char *matrixDest, int heightDest, int widthDest);
	
	//�������
	void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);	
	
	//�в���
	void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
	
	//�в���
	void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
	
	//һ��С���ֽ�
	void Trans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	
	//С���任
	void SimpleWaveletTrans(int scale);
	
	//�������
	void transeform_wavelet::MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);
	
	//�в��
	void transeform_wavelet::MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);
	
	//�в��
	void transeform_wavelet::MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);
	
	//һ��С���ع�
	void Reverse(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	
	//С���ع�
	void SimpleWaveletReverse();
	
	//��Ƶϵ�����㷽��ȥ����
	void H2Zero(double* coeff, int cHeight, int cWidth, 
						  int staticHeight, int staticWidth);
	void SetH2Zero();
	
	//Ӳ��ֵ����ȥ����
	void Hard(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth);
	//Ӳ��ֵ����ȥ��Ķ���ӿ�
	void HardThreshold();
	
	int GetSgn(double number);
	//����ֵ����ȥ����
	void Soft(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth);
	//����ֵ����ȥ��Ķ���ӿ�
	void SoftThreshold();
	
	void Sym8WaveletTrans(int scale);
	void Sym8WaveletReverse();
};
#endif // _INSIDE_VISUAL_CPP_WLTTRANSFORM