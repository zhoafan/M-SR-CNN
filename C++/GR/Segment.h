#ifndef _INSIDE_VISUAL_CPP_IMGSEGMENT
#define _INSIDE_VISUAL_CPP_IMGSEGMENT
#include "ImageCenterDib.h"

//ͼ��ָ���
class ImgSegment:public ImgCenterDib
{
public:
	//���ͼ��ÿ����λ��
	int m_nBitCountOut;

	//���ͼ��λͼ����ָ��
	unsigned char * m_pImgDataOut;

	//���ͼ����ɫ��
	LPRGBQUAD m_lpColorTableOut;
private:
	//���ͼ��Ŀ�
	int m_imgWidthOut;

	//���ͼ��ĸ�
	int m_imgHeightOut;

	//���ͼ����ɫ����
	int m_nColorTableLengthOut;
public:
	//���������Ĺ��캯��
	ImgSegment();

	//�������Ĺ��캯��
	ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		        unsigned char *pImgData);

	//��������
	~ImgSegment();

	//������Ϊ��λ�������ͼ��ĳߴ�
	CSize GetDimensions();

	//�����ֵ�ָ�
	int threshOtus(int histArray[256]);

	//Roberts����
	void Roberts();

	//Sobel����
	void Sobel();

	//Prewitt����
	void Prewitt();

	//Laplacian����
	void Laplacian();

	//Krisch����
	void Krisch();

	//Gauss-Laplacian����
	void GaussLaplacian();

	//�Զ���ģ�����Ե
	void EdgeByAnyMask(int *mask ,int maskW, int maskH);

	//Hough�任
	void Hough(float radiusResolution, float angleResolution, 
		        float *radius, float *angle);

	//ͨ��Hough�任���ͼ�������
	void longestLineDetectByHough(float radiusResolution, float angleResolution);

	//��������
	void RegionGrow(CPoint seed, int thresh);

	//������ȡ
	void ContourExtract();

	//���߸���
	void ContourTrace();


private:
	//ģ��������
	void TemplateEdge(unsigned char *imgIn, int width, int height, int nBitCount,
							int *mask, int maskW, int maskH, unsigned char *imgOut);

	//��������ʽ�µ��߼��hough�任
	void HoughTransform(unsigned char *imgBinaryIn, int width, int height, int *houghBuf,
		int  houghWidth, int houghHeight,float radiusResolution, float angleResolution,
					 float *radius, float *angle);
};
#endif // _INSIDE_VISUAL_CPP_IMGSEGMENT