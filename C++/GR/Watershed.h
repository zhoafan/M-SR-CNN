#ifndef __WATERSHED_H__
#define __WATERSHED_H__

#include "ImageCenterDib.h"

//ˮ��任��
class Watershed:public ImgCenterDib
{
public:	
	int m_nBitCountOut;//���ͼ��ÿ����λ��

	unsigned char * m_pImgDataOut;//���ͼ��λͼ����ָ��

	LPRGBQUAD m_lpColorTableOut;//���ͼ����ɫ��

	int objectCount;	//Ŀ����,��������Ҫ��ǵ�������

private:
	int m_imgWidthOut;//���ͼ��Ŀ�
	
	int m_imgHeightOut;//���ͼ��ĸ�

	int m_nColorTableLengthOut;//���ͼ����ɫ����
	
	CPoint *queue[256][4];//����
	
	int queueLng;//���л������ĳ���

	int headPointer[256][4];//����ָ�� 

	int tailPointer[256][4];//��βָ��
	
public:
	Watershed();//���������Ĺ��캯��

	//�������Ĺ��캯��
	Watershed(CSize size, int nBitCount, LPRGBQUAD lpColorTable,
		        unsigned char *pImgData);

	~Watershed();//��������

	CSize GetDimensions();//��ȡ���ͼ��ĳߴ�
	
	//ˮ��ָ�
	void WatershedSegment(CPoint position[], int classCount, int contourFlg);

private:
	//����
	void Pop(int grey,int mark,CPoint* pPopPoint);

	//���
	void Push(int gray,int mark,CPoint pushPoint);

	//�ж϶ӿ�
	bool IsEmpty(int gray,int mark);

	//�ж϶���
	bool IsOverflow(int gray,int mark);

	//sobel�任
	void Sobel(unsigned char *imgBuf, int width, int height, unsigned char *gradient);

	//���г�ʼ��
	void InitQueue(unsigned char* pGradient, unsigned char* pMarker, int width,int height);

	//ˮ��任
	void WatershedTrans(unsigned char* pGradient, unsigned char* pMarker, int width,int height,bool flag_contour);

	//ͨ��ָ����������
	void GenerateMarkerByPos(unsigned char* pMarker, int width,int height, 
		CPoint position[], int objectCount);
};
#endif