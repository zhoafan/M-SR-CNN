#ifndef _INSIDE_VISUAL_CPP_GRAYTRANS
#define _INSIDE_VISUAL_CPP_GRAYTRANS
#include "ImageCenterDib.h"

//�Ҷȱ任��
class GrayTrans:public ImgCenterDib
{
public:
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
	GrayTrans();

	//�������Ĺ��캯��
	GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);

	//��������
	~GrayTrans();
	
	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();

	//����
	void ReverseImg();

	//��ɫ��Ҷȸ�ʽ
	void ColorToGray();

	//�Ҷȱ��ɫ��ʽ
	void GrayToColor();

	//��ֵ��
	void Binary(int threshold=128);

	//��������
	void LinearStrech(CPoint point[2]);

	//��������������
	void DuiShuStrech(float a, float b, float c);

	//ָ������������
	void ZhiShuStrech(float a, float b, float c);

private:
	//��ͨ��������������
	void LinearStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,CPoint point[2]);

	//��ͨ�����ݶ�����������������
	void DuiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		float a, float b, float c);

	//��ͨ������ָ����������������
	void ZhiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		float a, float b, float c);
		
};
#endif // _INSIDE_VISUAL_CPP_GRAYTRANS