#include "stdafx.h"
#include "Morphology.h"

/***********************************************************************
* �������ƣ�
* Morphology()
*
*˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
Morphology::Morphology()
{

	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

	m_maskBuf=NULL;//�ṹԪ��ָ��Ϊ��

	m_maskW=0;//�ṹԪ�ؿ�Ϊ0

	m_maskH=0;//�ṹԪ�ظ�Ϊ0
}

/***********************************************************************
* �������ƣ�
* Morphology()
*
*����������
*  CSize size -ͼ���С�����ߣ�
*  int nBitCount  -ÿ������ռλ��
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*
*����ֵ��
*   ��
*
*˵����������Ϊ�������Ĺ��캯��������λͼ�Ĵ�С��ÿ����λ������ɫ��
*      ��λͼ���ݣ�����ImgCenterDib()�Ի����Ա��ʼ����,����ʼ���������
*      ���ݳ�Ա
***********************************************************************/
Morphology::Morphology(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					   unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{

	//���ͼ���ʽ������ͼ����ͬ���˴�ֱ���ڹ��캯���з������ͼ��
	//����Ҫ�Ļ�����������д���ͼ�����Ϣ
    m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_lpColorTableOut=NULL;
	int colorTableLength=ComputeColorTabalLength(m_nBitCountOut);
	if(colorTableLength!=0){
    	m_lpColorTableOut=new RGBQUAD[colorTableLength];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*colorTableLength);
	}

	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ������Ҫ�Ļ�����
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	m_maskBuf=NULL;//�ṹԪ��ָ��Ϊ��

	m_maskW=0;//�ṹԪ�ؿ�Ϊ0

	m_maskH=0;//�ṹԪ�ظ�Ϊ0

}

/***********************************************************************
* �������ƣ�
*   ~Morphology()
*
*˵���������������ͷ���Դ
***********************************************************************/
Morphology::~Morphology()
{
	//�ͷ����ͼ��λͼ���ݻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//�ͷ����ͼ����ɫ��
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//�ͷŽṹԪ�ػ�����
	if(m_maskBuf){
		delete []m_maskBuf;
		m_maskBuf=NULL;
	}
}

/***********************************************************************
* �������ƣ�
* GetDimensions()
*
*����������
*  ��
*
*����ֵ��
*   ͼ��ĳߴ磬��CSize���ͱ��
*
*˵�����������ͼ��Ŀ�͸�
***********************************************************************/
CSize Morphology::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/***********************************************************************
* �������ƣ�
* InputMask()
*
*����������
*  int *mask   -�ṹԪ�ػ�����ָ��
*  int maskW   -�ṹԪ�ؿ�
*  int maskH   -�ṹԪ�ظ�
*
*����ֵ��
*   ��
*
*˵�����ṹԪ�����뺯�����ڵ�����������ǰ�����ȵ��ô˺���
***********************************************************************/
void Morphology::InputMask(int *mask,int maskW, int maskH)
{
	if(m_maskBuf){
		delete []m_maskBuf;
		m_maskBuf=NULL;
	}

	//������ĽṹԪ�ؿ�����m_maskBuf��������
	m_maskBuf=new int[maskW*maskH];
	int i, j;
	for(i=0;i<maskH;i++){
		for(j=0;j<maskW;j++){
			m_maskBuf[i*maskW+j]=mask[i*maskW+j];
		}
	}

	//Ϊ�ṹԪ�ؿ�͸߸�ֵ
	m_maskW=maskW;
	m_maskH=maskH;
}


/***********************************************************************
* �������ƣ�
* basicErosionForBinary()
*
*����������
*  unsigned char *imgBufIn   -����ʴ��ͼ��
*  unsigned char *imgBufOut  -��ʴ��Ľ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  int *maskBuf   -�ṹԪ�ػ�����ָ��
*  int maskW   -�ṹԪ�ؿ�
*  int maskH   -�ṹԪ�ظ�
*
*����ֵ��
*   ��
*
*˵������ֵ��ʴ��������,����Ϊ��ɫ,Ŀ��Ϊ��ɫ,��/��/��ֵ��̬ѧ�߽�Ȳ���
*      ��Ҫ�����������
***********************************************************************/
void Morphology::BasicErosionForBinary(unsigned char *imgBufIn, 
					 unsigned char *imgBufOut,int imgWidth,int imgHeight,
					 int *maskBuf, int maskW, int maskH)
{
	//ÿ��ͼ�����ݵ��ֽ���Ϊ4�ı���
	int lineByte=(imgWidth+3)/4*4;

	//������ͼ�񻺳����Ŀ��������ͼ�񻺳���
	memcpy(imgBufOut,imgBufIn,lineByte*imgHeight);
	
	//�������
	int i,j,k,l;

	//��־����
	int judge;

	//���ͼ����ÿһ������λ�ã��ж��Ƿ�ṹԪ��������Ŀ���ڲ�
	for(i=maskH/2;i<imgHeight-maskH/2;i++)
	{
		for(j=maskW/2;j<imgWidth-maskW/2;j++)
		{			
			//�жϽṹԪ���Ƿ�����ڵ�ǰ������Ŀ���ڲ�,1Ϊ����,0Ϊ������
			judge=1;
			for(k=-maskH/2;k<=maskH/2;k++)
			{
				for(l=-maskW/2;l<=maskW/2;l++)
				{
					//�����ǰ�ṹԪ��λ��Ϊ1,�ж����Ӧͼ���ϵ����ص��Ƿ�Ϊ��0
					if(maskBuf[(k+maskH/2)*maskW+l+maskW/2])
					{
						//���ͼ��ǰ����Ϊ0,��û�л��иõ�,���Ǹ�ʴ�����
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							judge=0;	
					}
				}
			}


			if(judge)
				*(imgBufOut+i*lineByte+j)=255;
			else
				*(imgBufOut+i*lineByte+j)=0;
		}
	}
}

/***********************************************************************
* �������ƣ�
* BasicDilationForBinary()
*
*����������
*  unsigned char *imgBufIn   -�����͵�ͼ��
*  unsigned char *imgBufOut  -���ͺ�Ľ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  int *maskBuf   -�ṹԪ�ػ�����ָ��
*  int maskW   -�ṹԪ�ؿ�
*  int maskH   -�ṹԪ�ظ�
*
*����ֵ��
*   ��
*
*˵������ֵ���ͻ�������,����Ϊ��ɫ,Ŀ��Ϊ��ɫ,��/��/��ֵ��̬ѧ�߽�Ȳ���
*      ��Ҫ�����������
***********************************************************************/
void Morphology::BasicDilationForBinary(unsigned char *imgBufIn,
					unsigned char *imgBufOut,int imgWidth,int imgHeight,
					 int *maskBuf, int maskW, int maskH)
{
	//ÿ��ͼ�����ݵ��ֽ���Ϊ4�ı���
	int lineByte=(imgWidth+3)/4*4;

	//ѭ������
	int i,j,k,l;

	//�����Ƕ�ͼ����Ŀ�겹���ĸ�ʴ�������������ͼ�����ݵĲ���
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufIn+i*lineByte+j)=255-*(imgBufIn+i*lineByte+j);
		}
	}

	//�����ǽṹԪ�صĶԳƼ��Բ����ĸ�ʴ,�˴�tempMask���m_mask�ĶԳƼ�
	int *tempMask=new int[maskW*maskH];
	for(k=0;k<maskH;k++){
		for(l=0;l<maskW;l++){
			tempMask[k*maskW+l]=maskBuf[(maskH-1-k)*maskW+maskW-1-l];
		}
	}

	//��־����
	int judge;

	//���ͼ����ÿһ������λ�ã��ж��Ƿ�ṹԪ��������Ŀ���ڲ�
	for(i=maskH/2;i<imgHeight-maskH/2;i++)
	{
		for(j=maskW/2;j<imgWidth-maskW/2;j++)
		{			
			//�жϽṹԪ���Ƿ�����ڵ�ǰ������Ŀ���ڲ�,1Ϊ����,0Ϊ������
			judge=1;
			for(k=-maskH/2;k<=maskH/2;k++)
			{
				for(l=-maskW/2;l<=maskW/2;l++)
				{
					//�����ǰ�ṹԪ��λ��Ϊ1,�ж����Ӧͼ���ϵ����ص��Ƿ�Ϊ��0
					if(tempMask[(k+maskH/2)*maskW+l+maskW/2])
					{
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							judge=0;	
					}
				}
			}


			if(judge)
				*(imgBufOut+i*lineByte+j)=255;
			else
				*(imgBufOut+i*lineByte+j)=0;
		}
	}

	//�ýṹԪ�ضԳƼ���Ŀ�겹����ʴ��,��Ҫ�Խ������һ�β���,�������ͽ�����
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufOut+i*lineByte+j)=255-*(imgBufOut+i*lineByte+j);
		}
	}

	//�ָ�ԭʼ����
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			*(imgBufIn+i*lineByte+j)=255-*(imgBufIn+i*lineByte+j);
		}
	}
	
	delete []tempMask;
}

/***********************************************************************
* �������ƣ�
* BinaryErosion()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ��ʴ��m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryErosion()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//��ֵ��ʴ
	BasicErosionForBinary(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

}

/***********************************************************************
* �������ƣ�
* BinaryDilation()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ���ͣ�m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryDilation()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//��ֵ����
	BasicDilationForBinary(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
}


/***********************************************************************
* �������ƣ�
* BinaryOpen()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ�����㣬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryOpen()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//�ȸ�ʴ
	BasicErosionForBinary(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//������
	BasicDilationForBinary(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//�ͷŻ�����
	delete []buf;

}


/***********************************************************************
* �������ƣ�
* BinaryClose()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ�����㣬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryClose()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//������
	BasicDilationForBinary(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//��ʴ
	BasicErosionForBinary(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//�ͷŻ�����
	delete []buf;
}

/***********************************************************************
* �������ƣ�
* BinaryInnerEdge()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵�������ֵ��̬ѧ�ڱ߽磬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryInnerEdge()
{
	//���û��ģ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;

	//��ʴ
	BasicErosionForBinary(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//ԭʼ���ݼ���ʴ���
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<lineByte;j++){
			*(m_pImgDataOut+i*lineByte+j) =
				*(m_pImgData+i*lineByte+j)- *(m_pImgDataOut+i*lineByte+j);
		}
	}
	
}

/***********************************************************************
* �������ƣ�
* BinaryOuterEdge()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵�������ֵ��̬ѧ��߽磬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryOuterEdge()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;

	//��ʴ
	BasicDilationForBinary(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//���ͽ����ȥԭʼ����
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<lineByte;j++){
			*(m_pImgDataOut+i*lineByte+j) -= *(m_pImgData+i*lineByte+j);
		}
	}
}

/***********************************************************************
* �������ƣ�
* BinaryContour()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵�������ֵ��̬ѧ�߽磬m_pImgDataΪ���룬m_pImgDataOutΪ���
***********************************************************************/
void Morphology::BinaryContour()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//��ʴ
	BasicErosionForBinary(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//����
	BasicDilationForBinary(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	
	//���ͽ������ʴ���
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<lineByte;j++){
			*(m_pImgDataOut+i*lineByte+j) -= *(buf+i*lineByte+j);
		}
	}

	//�ͷŻ�����
	delete []buf;
}


/***********************************************************************
* �������ƣ�
* HitAndMiss()
*
*����������
*  unsigned char *imgBufIn   -���任������ͼ��
*  unsigned char *imgBufOut   -���ͼ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  struct ElementPair hitMissMask  -���л����нṹԪ�ض�
*
*����ֵ��
*   ��
*
*˵�������л����б任,0������,255����Ŀ��
***********************************************************************/
void Morphology::HitAndMiss(unsigned char *imgBufIn, unsigned char *imgBufOut,
					int imgWidth,int imgHeight, ElementPair hitMissMask)						 
{
	//ѭ������
	int i, j;

	//��־������1��ʾ�ṹԪ�ض���Ч��0��ʾ��Ч
	int validateFlag=1;

	//���ṹԪ�ض��Ƿ񽻼�Ϊ��,��������Ϊ����Ϊ��Ч�ṹԪ�ض�,�㷨���˳�
	for(i=0; i<3;i++){
		for(j=0;j<3;j++){
			if(hitMissMask.hitElement[i*3+j]&&hitMissMask.missElement[i*3+j]){
				validateFlag=0;
				break;
			}
		}
	}

	//�Ƿ��ṹԪ�ض�,����
	if(validateFlag==0)
		return;

	//ͼ��ÿ��������ռ�ֽ���
	int lineByte=(imgWidth+3)/4*4;

	//���ͼ����0
	memset(imgBufOut, 0, lineByte*imgHeight);

	//ѭ������
	int k,l;
	
	//���б�־�����ͻ����б�־����
	int hitFlag, missFlag;

	for(i=1; i<imgHeight-1; i++){
		for(j=1;j<imgWidth-1; j++){
			hitFlag=1;
			missFlag=1;
			for(k=-1;k<=1;k++){
				for(l=-1;l<=1;l++){
					//������нṹԪ�ص�ǰλ��Ϊ1
					if(hitMissMask.hitElement[(k+1)*3+l+1]==1){
						//�ж�ͼ���Ӧ���Ƿ�Ϊ0,�����,��û�л���ͼ��ǰ��
						if(!*(imgBufIn+(i+k)*lineByte+j+l))
							hitFlag=0;
					}

					//��������нṹԪ�ص�ǰλ��Ϊ1
					if(hitMissMask.missElement[(k+1)*3+l+1]==1){
						//�ж�ͼ���Ӧ���Ƿ�Ϊ0,�����,��û�л���ͼ��ǰ��
						if(*(imgBufIn+(i+k)*lineByte+j+l))
							missFlag=0;
					}
				}
			}

			//�������255
			if(hitFlag&&missFlag)
				*(imgBufOut+i*lineByte+j)=255;
		}
	}
}

/***********************************************************************
* �������ƣ�
* DefineElementPair()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Ϊ���л����б任�ĽṹԪ�ض�m_hitMissMask��ֵ����8��
***********************************************************************/
void Morphology::DefineElementPair()
{
	//����8������Ļ��л����б任�ṹԪ�ض�
	int i, k, l;
	for(i=0;i<8;i++){
		for(k=0;k<3;k++){
			for(l=0;l<3;l++){
				m_hitMissMask[i].hitElement[k*3+l]=0;
				m_hitMissMask[i].missElement[k*3+l]=0;
			}
		}
	}

	m_hitMissMask[0].hitElement[0]=1;
	m_hitMissMask[0].hitElement[1]=1;
	m_hitMissMask[0].hitElement[2]=1;
	m_hitMissMask[0].hitElement[4]=1;
	m_hitMissMask[0].missElement[6]=1;
	m_hitMissMask[0].missElement[7]=1;
	m_hitMissMask[0].missElement[8]=1;

	m_hitMissMask[1].hitElement[6]=1;
	m_hitMissMask[1].hitElement[7]=1;
	m_hitMissMask[1].hitElement[8]=1;
	m_hitMissMask[1].hitElement[4]=1;
	m_hitMissMask[1].missElement[0]=1;
	m_hitMissMask[1].missElement[1]=1;
	m_hitMissMask[1].missElement[2]=1;

	m_hitMissMask[2].hitElement[2]=1;
	m_hitMissMask[2].hitElement[5]=1;
	m_hitMissMask[2].hitElement[8]=1;
	m_hitMissMask[2].hitElement[4]=1;
	m_hitMissMask[2].missElement[0]=1;
	m_hitMissMask[2].missElement[3]=1;
	m_hitMissMask[2].missElement[6]=1;

	m_hitMissMask[3].hitElement[0]=1;
	m_hitMissMask[3].hitElement[3]=1;
	m_hitMissMask[3].hitElement[6]=1;
	m_hitMissMask[3].hitElement[4]=1;
	m_hitMissMask[3].missElement[2]=1;
	m_hitMissMask[3].missElement[5]=1;
	m_hitMissMask[3].missElement[8]=1;


	m_hitMissMask[4].hitElement[0]=1;
	m_hitMissMask[4].hitElement[1]=1;
	m_hitMissMask[4].hitElement[3]=1;
	m_hitMissMask[4].hitElement[4]=1;
	m_hitMissMask[4].missElement[5]=1;
	m_hitMissMask[4].missElement[7]=1;
	m_hitMissMask[4].missElement[8]=1;

	m_hitMissMask[5].hitElement[5]=1;
	m_hitMissMask[5].hitElement[7]=1;
	m_hitMissMask[5].hitElement[8]=1;
	m_hitMissMask[5].hitElement[4]=1;
	m_hitMissMask[5].missElement[0]=1;
	m_hitMissMask[5].missElement[1]=1;
	m_hitMissMask[5].missElement[3]=1;


	m_hitMissMask[6].hitElement[1]=1;
	m_hitMissMask[6].hitElement[2]=1;
	m_hitMissMask[6].hitElement[5]=1;
	m_hitMissMask[6].hitElement[4]=1;
	m_hitMissMask[6].missElement[3]=1;
	m_hitMissMask[6].missElement[6]=1;
	m_hitMissMask[6].missElement[7]=1;

	m_hitMissMask[7].hitElement[3]=1;
	m_hitMissMask[7].hitElement[6]=1;
	m_hitMissMask[7].hitElement[7]=1;
	m_hitMissMask[7].hitElement[4]=1;
	m_hitMissMask[7].missElement[1]=1;
	m_hitMissMask[7].missElement[2]=1;
	m_hitMissMask[7].missElement[5]=1;
}


/***********************************************************************
* �������ƣ�
* MorphoThinning()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵�����û��л����б任������ͼ�����ϸ��
***********************************************************************/
void Morphology::MorphoThinning()
{
	//����8������ĽṹԪ�ض�
	DefineElementPair();


	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(m_imgWidth+3)/4*4;

	//���뻺����������м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];
	memcpy(buf, m_pImgData, lineByte*m_imgHeight);

	//ѭ������
	int i, k, l;

	//thinFlag������¼��ǰһ��ѭ���������Ƿ���ϸ�����ĵ�,���Ϊ0����û
	//�п���ϸ���ĵ����,ϸ����ֹ	
	int thinFlag=1;
	while(thinFlag){
		thinFlag=0;
		//��8������ĽṹԪ�ضԶ���ͼ����һ�λ��л����б任,����������
		//��ԭͼ��ȥ��
		for(i=0;i<8;i++){
			HitAndMiss(buf, m_pImgDataOut, m_imgWidth, m_imgHeight,m_hitMissMask[i]);
			//�����л����б任����������ԭ������ȥ��
			for(k=0;k<m_imgHeight;k++){
				for(l=0;l<lineByte;l++){
					if(*(m_pImgDataOut+k*lineByte+l)==255){
						*(buf+k*lineByte+l)=0;
						thinFlag=1;
					}
				}
			}
		}
	}

	//�����ϸ���Ľ��������m_pImgDataOut
	memcpy(m_pImgDataOut, buf,  lineByte*m_imgHeight);

	//�ͷŻ�����
	delete []buf;

}

/***********************************************************************
* �������ƣ�
* BasicErosionForGray()
*
*����������
*  unsigned char *imgBufIn   -����ʴ��ͼ��
*  unsigned char *imgBufOut  -��ʴ��Ľ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  int *maskBuf   -�ṹԪ�ػ�����ָ��
*  int maskW   -�ṹԪ�ؿ�
*  int maskH   -�ṹԪ�ظ�
*
*����ֵ��
*   ��
*
*˵������ֵ��ʴ��������,����Ļ�ֵ��/��/��̬ѧ�ݶȵȲ�����Ҫ�����������
***********************************************************************/
void Morphology::BasicErosionForGray(unsigned char *imgBufIn,
					unsigned char *imgBufOut,int imgWidth,int imgHeight,
					int *maskBuf, int maskW, int maskH)
{
	//ѭ��   ����
	int i,j,k,l;

	//�Ҷ�ͼ��ÿ��������ռ�ֽ���
	int lineByte=(imgWidth+3)/4*4;

	//���뻺��������м���
	unsigned char *buf=new unsigned char[(imgHeight+maskH)*(lineByte+maskW)];

	//������ͼ��,���¼�m_maskH/2�����صĻҶ�Ϊ255�����ߣ����Ҽ�m_maskW/2����
	//�صĻҶ�Ϊ255�����ߣ��ӱߺ��ͼ�����buf
	for(i=0;i<imgHeight+maskH;i++)
	{
		for(j=0;j<lineByte+maskW;j++)
		{
			if(i<maskH/2||i>=imgHeight+maskH/2||j<maskW/2||j>=lineByte+maskW/2)
				*(buf+i*(lineByte+maskW)+j)=255;
			else
				*(buf+i*(lineByte+maskW)+j)=*(imgBufIn+(i-maskH/2)*lineByte+j-maskW/2);
		}
	}

	//��ʴ���㣬���������������ݼ���Ӧģ�����ݣ�������Сֵ������Сֵ���Ǹõ����
	int gray,tmp;
	for(i=maskH/2;i<imgHeight+maskH/2;i++)
	{
		for(j=maskW/2;j<lineByte+maskW/2;j++)
		{
			gray=255;
			for(k=-maskH/2;k<=maskH/2;k++)
			{
				for(l=-maskW/2;l<=maskW/2;l++)
				{
					if(maskBuf[(k+maskH/2)*maskW+l+maskW/2]){
						tmp=*(buf+(i+k)*(lineByte+maskW)+j+l)-
							maskBuf[(k+maskH/2)*maskW+l+maskW/2];
						if(tmp<gray)		
							gray=tmp;
					}
				}
			}
			if(gray<0)
				*(imgBufOut+(i-maskH/2)*lineByte+j-maskW/2)=0;
			else
				*(imgBufOut+(i-maskH/2)*lineByte+j-maskW/2)=gray;
		}
	}

	//�ͷŻ�����
  	delete []buf;
}

/***********************************************************************
* �������ƣ�
* BasicDilationForGray()
*
*����������
*  unsigned char *imgBufIn   -����ʴ��ͼ��
*  unsigned char *imgBufOut  -��ʴ��Ľ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  int *maskBuf   -�ṹԪ�ػ�����ָ��
*  int maskW   -�ṹԪ�ؿ�
*  int maskH   -�ṹԪ�ظ�
*
*����ֵ��
*   ��
*
*˵������ֵ���ͻ�������,����Ļ�ֵ��/��/��̬ѧ�ݶȵȲ�����Ҫ�����������
***********************************************************************/
void Morphology::BasicDilationForGray(unsigned char *imgBufIn, 
					unsigned char *imgBufOut,int imgWidth,int imgHeight,
					int *maskBuf, int maskW, int maskH)
{
	//ѭ������
	int i,j,k,l;

	//ͼ��ÿ��������ռ�ֽ���
	int lineByte=(imgWidth+3)/4*4;
	int gray,tmp;

		//������ͼ��,���¼�m_maskH/2�����صĻҶ�Ϊ0�ĺڱߣ����Ҽ�m_maskW/2����
	//�صĻҶ�Ϊ0�ĺڱߣ��ӱߺ��ͼ�����buf
	unsigned char *buf=new unsigned char [(imgHeight+maskH)*(lineByte+maskW)];
	for(i=0;i<imgHeight+maskH;i++)
	{
		for(j=0;j<lineByte+maskW;j++)
		{
			if(i<maskH/2||i>=imgHeight+maskH/2||j<maskW/2||j>=lineByte+maskW/2)
				*(buf+i*(lineByte+maskW)+j)=0;
			else
				*(buf+i*(lineByte+maskW)+j)=*(imgBufIn+(i-maskH/2)*lineByte+j-maskW/2);
		}
	}


	//��������
	for(i=maskH/2;i<imgHeight+maskH/2;i++)
	{
		for(j=maskW/2;j<imgWidth+maskW/2;j++)
		{
			gray=0;
			for(k=-maskH/2;k<=maskH/2;k++)
			{
				for(l=-maskW/2;l<=maskW/2;l++)
				{
					if(maskBuf[(k+maskH/2)*maskW+l+maskW/2]){
						tmp=*(buf+(i+k)*(lineByte+maskW)+j+l)
							+maskBuf[(k+maskH/2)*maskW+l+maskW/2];
						if(tmp>gray)		
							gray=tmp;
					}
				}
			}
			if(gray>255)
				*(imgBufOut+(i-maskH/2)*lineByte+j-maskW/2)=255;
			else
				*(imgBufOut+(i-maskH/2)*lineByte+j-maskW/2)=gray;
		}
	}

	//�ͷŻ�����
	delete []buf;
}


/***********************************************************************
* �������ƣ�
* GrayErosion()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ��ʴ��m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayErosion()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//��ʴ
	BasicErosionForGray(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
}


/***********************************************************************
* �������ƣ�
* GrayDilation()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ���ͣ�m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayDilation()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//����
	BasicDilationForGray(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

}

/***********************************************************************
* �������ƣ�
* GrayOpen()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ�����㣬m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayOpen()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//ÿ��ͼ�����ݵ��ֽ���,Ϊ4�ı���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//��ʴ
	BasicErosionForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//����
	BasicDilationForGray(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	
	//�ͷŻ�����
	delete []buf;
}

/***********************************************************************
* �������ƣ�
* GrayClose()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ�����㣬m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayClose()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//ÿ��ͼ�����ݵ��ֽ���,Ϊ4�ı���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//����
	BasicDilationForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//��ʴ
	BasicErosionForGray(buf, m_pImgDataOut, m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//�ͷŻ�����
	delete []buf;
}

/***********************************************************************
* �������ƣ�
* GrayGradient()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ֵ��̬ѧ�ݶȣ�m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayGradient()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//ÿ��ͼ�����ݵ��ֽ���,Ϊ4�ı���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//��ʴ
	BasicErosionForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//����
	BasicDilationForGray(m_pImgData, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//��̬ѧ�ݶ�Ϊ���͵Ľ������ʴ�Ľ��
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0; j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByte+j) -= *(buf+i*lineByte+j);
		}
	}

	//�ͷŻ�����
	delete []buf;

}

/***********************************************************************
* �������ƣ�
* GrayTopHatPeak()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵��������������m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayTopHatPeak()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//ÿ��ͼ�����ݵ��ֽ���,Ϊ4�ı���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//�ȸ�ʴ������Ϊ������
	BasicErosionForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicDilationForGray(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//��������Ϊԭͼ��ȥ������Ľ��
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0; j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByte+j) 
				= *(m_pImgData+i*lineByte+j)-*(m_pImgDataOut+i*lineByte+j);
		}
	}

	//�ͷŻ�����
	delete []buf;

}

/***********************************************************************
* �������ƣ�
* GrayTopHatVally()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵�������ȼ������m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayTopHatVally()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//ÿ��ͼ�����ݵ��ֽ���,Ϊ4�ı���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//�����ͺ�ʴΪ������
	BasicDilationForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicErosionForGray(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);


	//���ȼ����Ϊ�������ȥԭͼ�Ľ��
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0; j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByte+j) -= *(m_pImgData+i*lineByte+j);
		}
	}

	//�ͷŻ�����
	delete []buf;
}

/***********************************************************************
* �������ƣ�
* GrayTopHatPeakVally()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵������ȼ������m_pImgDataΪ����ͼ��m_pImgDataOutΪ���ͼ��
***********************************************************************/
void Morphology::GrayTopHatPeakVally()
{
	//���û�нṹԪ�����룬�򷵻�
	if(m_maskBuf==NULL)
		return;

	//ÿ��ͼ�����ݵ��ֽ���,Ϊ4�ı���
	int lineByte=(m_imgWidth+3)/4*4;

	//�м�������������,������Ÿ�ʴ����м���
	unsigned char *buf=new unsigned char[lineByte*m_imgHeight];

	//�����ͺ�ʴΪ������,�������m_pImgDataOut�д���˱�����Ľ��
	BasicDilationForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicErosionForGray(buf, m_pImgDataOut,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	////�м�������������,������Ÿ�ʴ����м���
	unsigned char *bufOpen=new unsigned char[lineByte*m_imgHeight];

	//�ȸ�ʴ������Ϊ�����㣬�������bufOpen�д���˿�����Ľ��
	BasicErosionForGray(m_pImgData, buf,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);
	BasicDilationForGray(buf, bufOpen,m_imgWidth,m_imgHeight,
		m_maskBuf, m_maskW, m_maskH);

	//��ȼ����Ϊ�������ȥ������Ľ��
	int i, j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0; j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByte+j) -= *(bufOpen+i*lineByte+j);
		}
	}

	//�ͷŻ�����
	delete []buf;
	delete []bufOpen;
}

