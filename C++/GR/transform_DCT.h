#ifndef _INSIDE_VISUAL_CPP_TRANSFORM_DCT
#define _INSIDE_VISUAL_CPP_TRANSFORM_DCT
#include "FFT.h"

//��ɢ���ұ任��
class transform_DCT : public Transform_FFT
{
protected:
	double *pDctBuff;
	LONG	w;				// ���и���Ҷ�任�Ŀ�ȣ�2�������η���
	LONG	h;				// ���и���Ҷ�任�ĸ߶ȣ�2�������η���
	int		wp;
	int		hp;

public:
    transform_DCT();
    transform_DCT(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
    ~transform_DCT();

protected:	
	//������ɢ���ұ任�ĺ���
	void DCT(double *f, double *F, int power);
	void IDCT(double *F, double *f, int power);

public:
	void ImgDCT();
	void ImgIDCT();
};

#endif //_INSIDE_VISUAL_CPP_TRANSFORM_DCT
