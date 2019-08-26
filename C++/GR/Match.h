#ifndef _INSIDE_VISUAL_CPP_Match
#define _INSIDE_VISUAL_CPP_Match
#include "ImageCenterDib.h"

//�㴦����
class Match:public ImgCenterDib
{
public:
	int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	LPRGBQUAD m_lpColorTableOut;
	int m_nColorTableLengthOut;
public:
	int m_mask[5][5];//ģ�壬���Ϊ5x5
	Match();
	Match(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
	~Match();

	CSize GetDimensions();

	//�������ƥ�亯��
	CPoint matchBasedSpacial(unsigned char *imgBuf, int width, int height, int pixelByte,
										unsigned char * blockBuf, int blockW, int blockH);

	//�������ƥ������ʾ
	void matchResult(LPCTSTR cankaoImgPath, LPCTSTR jizhunImgPath, int matchStyle);


private:
	//���ͼ��Ŀ�
	int m_imgWidthOut;
	//���ͼ��ĸ�
	int m_imgHeightOut;
	
};
#endif // _INSIDE_VISUAL_CPP_Match