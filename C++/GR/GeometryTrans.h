#ifndef _INSIDE_VISUAL_CPP_GeometryTrans
#define _INSIDE_VISUAL_CPP_GeometryTrans
#include "ImageCenterDib.h"

//���α任��
class GeometryTrans:public ImgCenterDib
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
	//���캯��
	GeometryTrans();

	//�������Ĺ��캯��
	GeometryTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);

	//��������
	~GeometryTrans();

	//������Ϊ��λ�������ͼ��Ŀ�͸�
	CSize GetDimensions();

	//ƽ��
	void Move(int offsetX, int offsetY);

	//ˮƽ����
	void MirrorHori();

	//��ֱ����
	void MirrorVerti();

	//ת��
	void ZhuanZhi();
	
	//˳ʱ����ת90��
	void Clockwise90();

	//��ʱ����ת90��
	void Anticlockwise90();

	//��ת180
	void Rotate180();

	//��ת 
	void Rotate(int angle, int interpolationflag);//angle��ת�Ƕ�

	//����
	void Zoom(int zoomWidthPixel, int zoomHeightPixel, int interpolationflag);//����

private:
	//˫���Բ�ֵ
	unsigned char interpolationDblLinear(unsigned char array[2][2], float xpos, float ypos);

	//���������ֵ
	unsigned char interpolationCube(unsigned char array[4][4], float xpos, float ypos);


	//���ڲ�ֵ����
    void ZoomNeighbor(unsigned char *pImgIn, int widthIn, int heightIn, int biBitCount,
										 unsigned char *pImgOut, int widthOut, int heightOut);
	//˫���Բ�ֵ����
	void ZoomDblLinear(unsigned char *pImgIn, int widthIn, int heightIn, int biBitCount,
										 unsigned char *pImgOut, int widthOut, int heightOut);

	//���������ֵ����
	void ZoomCube(unsigned char *pImgIn, int widthIn, int heightIn, int biBitCount,
										 unsigned char *pImgOut, int widthOut, int heightOut);

	//���ڲ�ֵ��ת
	void RotateNeighbor(int angle);

	//˫���Բ�ֵ��ת
	void RotateDblLinear(int angle);

	//���������ֵ��ת
	void RotateCube(int angle);
};
#endif // _INSIDE_VISUAL_CPP_GeometryTrans