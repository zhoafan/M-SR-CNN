#pragma once



#ifndef IMAGERESTORE_INCLUDE
#define IMAGERESTORE_INCLUDE

class ImageRestore
{

private: 
	// ����ɢ����ָ��
	double *mask;  
	
	// ����ɢ������ɢ������    
	int mask_width;    
	
	// ����ɢ������ɢ����߶�
	int mask_height;

	int angle; // ģ���Ƕ�
	int length; // ģ������

public:
	//���캯�� 
	ImageRestore(void);

	//��������
	~ImageRestore(void);

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