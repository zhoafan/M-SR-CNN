#pragma once

#ifndef IMAGERESTORE_EXT_INCLUDE
#define IMAGERESTORE_EXT_INCLUDE


class ImageRestoreExt
{

private: 
	// ����ɢ����ָ��
	double *mask;  
			
	// ����ɢ������ɢ������    
	int mask_width;    
			
	// ����ɢ������ɢ����߶�
	int mask_height;   

public:
	//���캯�� 
	ImageRestoreExt(void);

	//��������
	~ImageRestoreExt(void);

public:

	//��������ͶӰ���ָ���8λ�Ҷ����ݺ�24λ�Ҷ���������
	void MapRestoreByteVar(unsigned char *imgBufIn ,int imgWidth, int imgHeight,int pxWidth ,int cycleCount);

	//�����ݳ���ά���˲��ָ�
	bool WienerRestoreByteVar(unsigned char *imgBufIn,int imgWidth,int imgHeight,int pxWidth,double k);

	//ͼ�����˲��ָ�
	bool InverseFilterByte(unsigned char *imgBufIn,int imgWidth,int imgHeight,int pxWidth);

	//PSF����ɢ�����������ɺ���
	void GeneratePSF(double length, double angle);

};
#endif