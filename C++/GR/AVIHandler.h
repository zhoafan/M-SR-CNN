#pragma once
#include <vfw.h>

#ifndef AVIHandler_H
#define AVIHandler_H

#include "MediaSource.h"


class AVIHandler : MediaSource
{
public:
	AVIHandler(void);
	AVIHandler(CString filename);
	
public:
	~AVIHandler(void);
public:
	BOOL LoadVideo(CString filename);
private:
	int width; //ͼ����
	int height;//ͼ��߶�
	int length;//����

	int defaultPad;
	
	BOOL loaded;

	CString AVIfilename; //AVI�ļ���
	
	LPBITMAPINFOHEADER lpbm;

	//δ���ҶȻ��壨24Bit�ɲ�ɫ����
	unsigned char* gray_value;
	
	//�����ҶȻ��壨24Bit�ɲ�ɫ����
	unsigned char* gray_value_square;

	PGETFRAME pgf;

public:
	unsigned char* GetSingleFrame(int i);
	int GetFrameCount(void);

	int GetFrameWidth(void);

	int GetFrameHeight(void);

	LPBITMAPINFOHEADER GetLPBM(void);

	BOOL isLoaded(void);
};

#endif