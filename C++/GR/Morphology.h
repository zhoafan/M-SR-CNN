#ifndef _INSIDE_VISUAL_CPP_MORPHOLOGY
#define _INSIDE_VISUAL_CPP_MORPHOLOGY
#include "ImageCenterDib.h"

//�ṹԪ�ض�,�ýṹר��Ϊ���л����б任������
struct ElementPair
{
	int hitElement[9];
	int missElement[9];
};

//��̬ѧ������
class Morphology:public ImgCenterDib
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

	//�ṹԪ�أ�ģ�壩ָ��
	int *m_maskBuf;

	//�ṹԪ�ؿ�
	int m_maskW;

	//�ṹԪ�ظ�
	int m_maskH;

	//����8������Ļ��л����б任�ṹԪ�ض�
	ElementPair m_hitMissMask[8];
public:
	//���������Ĺ��캯��
	Morphology();

	//�������Ĺ��캯��
	Morphology(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);

	//��������
	~Morphology();

	//�������ͼ��ĳߴ�
	CSize GetDimensions();

	//�ṹԪ�أ�ģ�壩���뺯�����ڵ���������̬ѧ������ǰ�����ȵ��ô˺���
	void InputMask(int *mask,int maskW, int maskH);

	//��ֵ��ʴ
	void BinaryErosion();

	//��ֵ����
	void BinaryDilation();

	//��ֵ��
	void BinaryOpen();

	//��ֵ��
	void BinaryClose();

	//��ֵ�ڱ߽�
	void BinaryInnerEdge();

	//��ֵ��߽�
	void BinaryOuterEdge();

	//��ֵ��̬ѧ�߽�
	void BinaryContour();

	//���л�����ϸ��
	void MorphoThinning();

	//��ֵ��ʴ
	void GrayErosion();

	//��ֵ����
	void GrayDilation();

	//��ֵ��
	void GrayOpen();

	//��ֵ��
	void GrayClose();

	//��ֵ��̬ѧ�ݶ�
	void GrayGradient();

	//��ֵ������
	void GrayTopHatPeak();

	//��ֵ���ȼ��
	void GrayTopHatVally();

	//��ֵ��ȼ����
	void GrayTopHatPeakVally();
private:
	//��ֵ��ʴ��������,����Ϊ��ɫ,Ŀ��Ϊ��ɫ,��ֵ�����ա���̬ѧ�ݶȵȲ�����Ҫ��
	//���������
	void BasicErosionForBinary(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,int *maskBuf, int maskW, int maskH);

	//��ֵ���ͻ�������,����Ϊ��ɫ,Ŀ��Ϊ��ɫ,��ֵ�����ա���̬ѧ�ݶȵȲ�����Ҫ����
	//�������
	void BasicDilationForBinary(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,int *maskBuf, int maskW, int maskH);
					 

	//������л����б任�ĽṹԪ�ض�
	void DefineElementPair();

	//���л����б任
	void HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,ElementPair hitMissMask);

	//��ֵ��ʴ��������,��ֵ�����ա���̬ѧ�ݶȵȲ�����Ҫ�����������
	void BasicErosionForGray(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,int *maskBuf, int maskW, int maskH);
					
	//��ֵ���ͻ�������,��ֵ�����ա���̬ѧ�ݶȲ����ȶ�Ҫ�����������
	void BasicDilationForGray(unsigned char *imgBufIn, unsigned char *imgBufOut,
		int imgWidth,int imgHeight,int *maskBuf, int maskW, int maskH);
					
};
#endif // _INSIDE_VISUAL_CPP_MORPHOLOGY