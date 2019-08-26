#ifndef _INSIDE_VISUAL_CPP_histogram
#define _INSIDE_VISUAL_CPP_histogram
#include "ImageCenterDib.h"

//ֱ��ͼ��
class Histogram:public ImgCenterDib
{
public:
	//ֱ��ͼ����
	int m_histArray[256];

	//ֱ��ͼƽ��ֵ
	float m_average;

	//ֱ��ͼ����
	float m_deviation;

	//���ͼ��ÿ����λ��
	int m_nBitCountOut;

	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;

	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;
	
private:
	//���ͼ��Ŀ�����Ϊ��λ
	int m_imgWidthOut;

	//���ͼ��ĸߣ�����Ϊ��λ
	int m_imgHeightOut;

	//���ͼ����ɫ����
	int m_nColorTableLengthOut;

public:
	//���������Ĺ��캯��
	Histogram();

	//�������Ĺ��캯��
	Histogram(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);

	//��������
	~Histogram();

	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();

	//ͳ��ֱ��ͼ
	void computeHistGray();

	//��ɫͼ������ֱ��ͼ
	void computeHistBrightness();

	//��ɫͼ���ɫͨ��ֱ��ͼ
	void computeHistRed();

	//��ɫͼ����ɫͨ��ֱ��ͼ
	void computeHistGreen();

	//��ɫͼ����ɫͨ��ֱ��ͼ
	void computeHistBlue();

	//����ֱ��ͼ��ֵ
	void computeAverage();

	//����ֱ��ͼ����
	void computeDeviation();

	//ֱ��ͼ����
	void histAve();
};
#endif // _INSIDE_VISUAL_CPP_ProcessBasedPixel