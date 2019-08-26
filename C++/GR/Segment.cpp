#include "stdafx.h"
#include "Segment.h"
#include "math.h"

/***********************************************************************
* �������ƣ�
* ImgSegment()
*
*˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
ImgSegment::ImgSegment()
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

/***********************************************************************
* �������ƣ�
* ImgSegment()
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
*      ��λͼ���ݣ�����ImgCenterDib()�Ի����Ա��ʼ��������ʼ���������
*      ���ݳ�Ա
***********************************************************************/
ImgSegment::ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0
}

/***********************************************************************
* �������ƣ�
*   ~ImgSegment()
*
*˵���������������ͷ���Դ
***********************************************************************/
ImgSegment::~ImgSegment()
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
CSize ImgSegment::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* �������ƣ�
* threshOtus()
*
*����������
*  int histArray[256]  -ͼ���ͳ��ֱ��ͼ
*
*  ����ֵ��
*     �����ֵ
*
*˵���������ֵѡ����������ֱ��ͼ���飬���ݷ������ԭ���Զ�ѡ����ֵ��
*      ���ڲ�ɫͼ�񣬸ú�����������ֱ��ͼ������ֵ
***********************************************************************/
int ImgSegment::threshOtus(int histArray[256])
{
	//c0��c1��ľ�ֵ
	float u0,u1;

	//c0��c1������ĸ���
	float w0,w1;

	//c0���������
	int count0;

	//��ֵt����¼�������ʱ�������ֵmaxT
	int t, maxT;

	//�����󷽲�
	float devi, maxDevi=0;

	//ѭ������
	int i;

	//ͳ��ֱ��ͼ�����صĸ����������sum��
	int sum=0;
	for(i=0;i<256;i++)
		sum = sum+histArray[i];

	for(t=0;t<255;t++){
		//������ֵΪtʱ��c0��ľ�ֵ�Ͳ����ĸ���
		u0=0;
		count0=0;
		for(i=0; i<=t;i++){
			u0 += i*histArray[i];
			count0 += histArray[i];
		}
		u0=u0/count0;
		w0=(float)count0/sum;

		//������ֵΪtʱ��c1��ľ�ֵ�Ͳ����ĸ���
		u1=0;
		for(i=t+1; i<256;i++)
			u1+=i*histArray[i];
		//C0����������C1��������֮��Ϊͼ������������
		u1=u1/(sum-count0);
		w1=1-w0;

		//�����ķ���
		devi=w0*w1*(u1-u0)*(u1-u0);

		//��¼���ķ�������ֵλ��
		if(devi>maxDevi){
			maxDevi=devi;
			maxT=t;
		}

	}

	//���������ֵ
	return maxT;

}


/***********************************************************************
* �������ƣ�
* Roberts()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Roberts��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Roberts()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Roberts����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x=*(m_pImgData+i*lineByte+j*pixelByte+k)
					-*(m_pImgData+(i+1)*lineByte+j*pixelByte+k);

				//y�����ݶ�
				y=*(m_pImgData+i*lineByte+j*pixelByte+k)
					-*(m_pImgData+i*lineByte+(j+1)*pixelByte+k);

				t=sqrt(x*x+y*y)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}
}

/***********************************************************************
* �������ƣ�
* Sobel()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Sobel��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Sobel()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Sobel����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + 2 * *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y�����ݶ�
				y= *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 + 2 * *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k);

				t=sqrt(x*x+y*y)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}

}

/***********************************************************************
* �������ƣ�
* Prewitt()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Prewitt��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Prewitt()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Prewitt����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y�����ݶ�
				y= *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k);

				t=sqrt(x*x+y*y)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}

}


/***********************************************************************
* �������ƣ�
* Laplacian()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Laplacian��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Laplacian()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int t;

	//Laplacian����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				t= 4 * *(m_pImgData+i*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+i*lineByte+(j+1)*pixelByte+k);

				t=abs(t)+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}
}

/*************************************************************************
 *
 * �������ƣ�
 *   TemplateEdge()
 *
 * ����:
 *   unsigned char *imgIn	- ����ͼ������ָ��
 *   int width, int height  - ͼ��Ŀ�͸ߣ�����Ϊ��λ��
 *   int nBitCount  -ͼ��ÿ����λ��
 *   int *mask -  ģ��ָ��
 *   int maskW, int maskH  - ģ��Ŀ�͸�
 *   unsigned char *imgOut - ͼ����ģ�����󣨱�Ե��⣩�����
 *
 * ����ֵ:
 *   ��
 *
 * ˵��:
 *   ģ���������ú����ø�����maskģ��������ͼ����о��,�������ҶȺͲ�ɫ
 *   ������ͬͨ�����ݵĺϳɣ��ֱ�Բ�ͬͨ�����ݽ���ģ�������Ӷ���ɻҶ�
 *   �Ͳ�ɫͼ��ı�Ե���
 ************************************************************************/
void ImgSegment::TemplateEdge(unsigned char *imgIn, int width, int height, int nBitCount,
							int *mask, int maskW, int maskH, unsigned char *imgOut)
{
	//ÿ�������ֽ���
	int lineByte=(width*nBitCount/8+3)/4*4;

	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ���
	int pixelByte=nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//ѭ������������ģ��������
	int  c, r;
	
	//�м����
	int sum;

	//ģ�������㣬��Ե���ز�����
	for(i=maskH/2;i<height-maskH/2; i++){
		for(j=maskW/2; j<width-maskH/2; j++){
			//��ÿ��ͨ�����ݽ��о��
			for(k=0; k<pixelByte; k++){
				//������
				sum=0;
				for(r=-maskH/2;r<=maskH/2;r++){
					for(c=-maskW/2;c<=maskW/2;c++){
						sum += *(mask+(r+maskH/2)*maskW+c+maskW/2) 
							* *(imgIn+(i+r)*lineByte+(j+c)*pixelByte+k);
					}
				}
				
				//ȡ����ֵ
				sum=abs(sum);
				if(sum>255)
					sum=255;
				else if(sum<0)
					sum=0;
				
				*(imgOut+i*lineByte+j*pixelByte+k)=sum;
			}
		}
	}
}


/***********************************************************************
* �������ƣ�
* EdgeByAnyMask()
*
*����������
*  int *mask   -��ά��ģ��
*  int maskW   -ģ���
*  int maskH   -ģ���
*
*����ֵ��
*   ��
*
*˵���������Զ���ģ�����Ե��ͬʱ�ʺ��ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::EdgeByAnyMask(int *mask ,int maskW, int maskH)
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		       mask, maskW, maskH, m_pImgDataOut);	
}

/***********************************************************************
* �������ƣ�
* Krisch()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Krisch��Ե��⣬ͬʱ�ʺ��ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::Krisch()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//���ͼ�񻺳�����ʼ��Ϊ0
	memset(m_pImgDataOut, 0, lineByte*m_imgHeight);

	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;	

	//����ָ�����
	unsigned  char *p1, *p2;
	//������ʱ������,����м���
	unsigned char *buf=new unsigned char[lineByte* m_imgHeight];

	//ģ������
	int mask[9];

	// ����Kirschģ��1����
	mask[0] = 5;
	mask[1] = 5;
	mask[2] = 5;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = -3;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount,
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}

	// ����Kirschģ��2����
	mask[0] = -3;
	mask[1] = 5;
	mask[2] = 5;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = 5;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = -3;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, 
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}

	// ����Kirschģ��3����
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = 5;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = 5;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = 5;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, 
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}


	// ����Kirschģ��4����
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = 5;
	mask[6] = -3;
	mask[7] = 5;
	mask[8] = 5;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, 
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}

	// ����Kirschģ��5����
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = -3;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = 5;
	mask[7] = 5;
	mask[8] = 5;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}

	// ����Kirschģ��6����
	mask[0] = -3;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = 5;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = 5;
	mask[7] = 5;
	mask[8] = -3;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, 
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}

	// ����Kirschģ��7����
	mask[0] = 5;
	mask[1] = -3;
	mask[2] = -3;
	mask[3] = 5;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = 5;
	mask[7] = -3;
	mask[8] = -3;

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, 
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				p1=m_pImgDataOut+i*lineByte+j*pixelByte+k;
				p2=buf+i*lineByte+j*pixelByte+k;
				if(*p1<*p2)
					*p1=*p2;
			}
		}
	}

	// ����Kirschģ��8����
	mask[0] = 5;
	mask[1] = 5;
	mask[2] = -3;
	mask[3] = 5;
	mask[4] = 0;
	mask[5] = -3;
	mask[6] = -3;
	mask[7] = -3;
	mask[8] = -3;
	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, 
		         mask, 3, 3, buf);

	//����������ͼ������ֵ,�������ֵ����m_pImgDataOut��
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++){
				if(*(m_pImgDataOut+i*lineByte+j*pixelByte+k)<*(buf+i*lineByte+j*pixelByte+k))
					*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=*(buf+i*lineByte+j*pixelByte+k);
			}
		}
	}
}

/***********************************************************************
* �������ƣ�
* GaussLaplacian()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Gauss-Laplacian��Ե��⣬ͬʱ�ʺ��ڻҶȺͲ�ɫͼ��
***********************************************************************/
void ImgSegment::GaussLaplacian()
{
	//����Gauss-Laplacianģ��
	int mask[25];
	mask[0] = -2;
	mask[1] = -4;
	mask[2] = -4;
	mask[3] = -4;
	mask[4] = -2;
	mask[5] = -4;
	mask[6] = 0;
	mask[7] = 8;
	mask[8] = 0;
	mask[9] = -4;
	mask[10] = -4;
	mask[11] = 8;
	mask[12] = 24;
	mask[13] = 8;
	mask[14] = -4;
	mask[15] = -4;
	mask[16] = 0;
	mask[17] = 8;
	mask[18] = 0;
	mask[19] = -4;
	mask[20] = -2;
	mask[21] = -4;
	mask[22] = -4;
	mask[23] = -4;
	mask[24] = -2;

	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//ģ������������
	TemplateEdge(m_pImgData, m_imgWidth, m_imgHeight, m_nBitCount, mask, 5, 5, m_pImgDataOut);

}


/***********************************************************************
* �������ƣ�
* HoughTransform()
*
*����������
*  unsigned char *imgBinaryIn  -��ֵͼ����������ָ��
*  int width  -ͼ��� ��
*  int height -ͼ��ĸ�
*  int *houghBuf  -Hough�任����Ҫ�Ļ�����ָ��
*  int  houghWidth  -Hough�任�������Ŀ�
*  int houghHeight  -Hough�任�������ĸ�
*  float radiusResolution  -Hough�任�ļ�����뾶�ļ��ֱ���
*  float angleResolution  -Hough�任�ĽǶȼ��ֱ��� 
*  float *radius  -��������Hough�任���������ֱ�ߵļ��뾶
*  float *angle   -��������Hough�任���������ֱ�ߵĽǶ�

*����ֵ��
*   ��
*
*˵��������ͼ�����ݣ�ͨ���߼��hough�任���ֱ��
***********************************************************************/
void ImgSegment::HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    float *radius, float *angle)
{
	//����ѭ������
	int i, j;

	//����任�ռ��ۼ�����
	for(i=0;i<houghHeight;i++){
		for(j=0;j<houghWidth;j++){
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//ѭ������
	int r, a;

	//�м����
	float tempR, tempA; 

	//����ͼ������
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			//����һ��Ŀ���
			if(*(imgBinaryIn+i*width+j)==0){

				//a����Ƕȵ�ѭ���������ڱ任�ռ��ۼ�����Ĵ�ֱ������
				for(a=0;a<houghHeight;a++){

					//���ո����任�Ƕȵķֱ��ʣ���ȡ�Ƕ�
					tempA=(a-houghHeight/2)*angleResolution;

					//���ݵ�ǰ�����ĽǶȼ�x,yֵ��ȡ��Ӧ���뾶
					tempR=(j-width/2)*cos(tempA*2*3.1415926/360)+(i-height/2)*sin(tempA*2*3.1415926/360);
					r=tempR/radiusResolution;

					//�ۼ������ۼ�
					*(houghBuf+a*houghWidth+ r+houghWidth/2)+=1;
				}
			}
		}
	}

	//���ۼ�����ļ���ֵ������¼��ʱ����������
	int max, maxR, maxA;
	max=*(houghBuf+0*houghWidth+0);
	maxR=0;
	maxA=0;
	for(a=0;a<houghHeight;a++){
		for(r=0;r<houghWidth;r++){
			if(max<=*(houghBuf+a*houghWidth+r)){
				max=*(houghBuf+a*houghWidth+r);
				maxR=r;
				maxA=a;
			}
		}
	}

	//������ֵλ��ת���ɼ�����뾶�ͽǶ�,��ͨ����������
	*radius=(maxR-houghWidth/2)*radiusResolution;
	*angle=(maxA-houghHeight/2)*angleResolution;
}


/***********************************************************************
* �������ƣ�
* Hough()
*
*����������
*  float radiusResolution  -Hough�任�ļ�����뾶�ļ��ֱ���
*  float angleResolution  -Hough�任�ĽǶȼ��ֱ��� 
*  float *radius  -��������Hough�任���������ֱ�ߵļ��뾶
*  float *angle   -��������Hough�任���������ֱ�ߵĽǶ�

*����ֵ��
*   ��
*
*˵��������Hough�任�ļ��뾶�ֱ��ʺͽǶȷֱ��ʣ�ͨ������HoughTransform()
*      ������ͼ��m_pImgData���߼�⣬Hough�任�Ľ�������m_pImgDataOut��
***********************************************************************/
void ImgSegment::Hough(float radiusResolution, float angleResolution,
					   float *radius, float *angle)
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷŻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ���ÿ����λ������ɫ����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

	//ͼ�������Ϊ����ԭ��,����ԭ��ľ������
	//Ϊsqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,����Hough�任
	//�ĸ߶�Ϊ��
	int houghWidth=sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight);
	 houghWidth /= radiusResolution;

	//�ߵĽǶ���[-90,90]֮��,����������ۼ�����߶�Ϊ181/angleResolution
	int houghHeight=181/angleResolution;

	//�����ۼ����黺����
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough�任���������houghBuf��
	HoughTransform(m_pImgData, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, radius, angle);
	
	//���ͼ��Ĵ�С��Hough�任�ۼ�����Ĵ�С			
	m_imgWidthOut=houghWidth;
	m_imgHeightOut=houghHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//����ۼ���������ֵ
	int i, j, max=0; 
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			if(max<=*(houghBuf+i*houghWidth+j))
				max=*(houghBuf+i*houghWidth+j);
		}
	}
	//�������ֵ���ۼ�����ӳ�䵽���ͼ��
	for(i=0;i<m_imgHeightOut;i++){
		for(j=0;j<m_imgWidthOut;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=
				*(houghBuf+i*houghWidth+j)*255.0/max;
		}
	}
	
	//�ͷŻ�����
	delete []houghBuf;
}

/***********************************************************************
* �������ƣ�
* longestLineDetectByHough()
*
*����������
*  float radiusResolution  -Hough�任�ļ�����뾶�ļ��ֱ���
*  float angleResolution  -Hough�任�ĽǶȼ��ֱ��� 
*  float *radius  -��������Hough�任���������ֱ�ߵļ��뾶
*  float *angle   -��������Hough�任���������ֱ�ߵĽǶ�

*����ֵ��
*   ��
*
*˵��������Hough�任�ļ��뾶�ֱ��ʺͽǶȷֱ��ʣ�ͨ������HoughTransform()
*      ������ͼ��m_pImgData���߼�⣬������ߵĽǶȺͼ�����뾶,����
*     ���������m_pImgDataOut��
***********************************************************************/
void ImgSegment::longestLineDetectByHough(float radiusResolution, float angleResolution)
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷŻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ���ÿ����λ������ɫ����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

	//ͼ�������Ϊ����ԭ��,����ԭ��ľ������
	//Ϊsqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,����Hough�任
	//�ĸ߶�Ϊ��
	int houghWidth=sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight);
	 houghWidth /= radiusResolution;

	//�ߵĽǶ���[-90,90]֮��,����������ۼ�����߶�Ϊ181/angleResolution
	int houghHeight=181/angleResolution;

	//�����ۼ����黺����
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough�任���������houghBuf��
	float radius, angle;
	HoughTransform(m_pImgData, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, &radius, &angle);

	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//������õĽǶȺͼ�����뾶,�ڰ�ɫ�����»����ֱ�߲���Ϊ���ͼ����ʾ
	memset(m_pImgDataOut,255,lineByteOut*m_imgHeightOut);

	//angle�ĵ�λ�Ƕ�,�˴�ת��Ϊ���Ƚ��м���
	float alfa=angle*2*3.1415926/360;

	
	int x, y, i, j;
	for( x=-m_imgWidthOut/2;x<m_imgWidthOut/2;x++){
		//ͼ�������λ��Ϊ����ԭ��,(x,y)Ϊֱ������ϵ�еĵ�,
		//����ת��������ԭ����ͼ�����½ǵ�����ϵ(i,j)
		y=radius/sin(alfa)-x/tan(alfa);
		j=x-m_imgWidthOut/2;
		i=y+m_imgHeightOut/2;
		if(i>0&&i<m_imgHeightOut)
	    	*(m_pImgDataOut+i*m_imgWidthOut+j)=0;
	}

	//�ռ��ͷ�
	delete []houghBuf;
}



/***********************************************************************
* �������ƣ�
* RegionGrow()
*
*����������
*  CPoint seed  -���ӵ�
*  int thresh   -��ֵ
*
*����ֵ��
*   ��
*
*˵�������������㷨������һ�����ӵ�seed����ֵthresh�������ӵ㴦��ʼ������
*      �������ӵ����ػҶ�ֵ֮��thresh�����غϲ���һ���γ�һ�����򣬸ú�
*      ��ֻ����Ҷ�ͼ��
***********************************************************************/
void ImgSegment::RegionGrow(CPoint seed, int thresh)
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			     sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//������������������

	//ѭ������
	int i, j;

	//�����ͼ���ʼ����255,��0�������ص��������
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByte+j)=255;
		}
	}

	//��ά����direction�����������ص�8����������õ���x��y�����ϵ�ƫ��,
	//���е�һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//ջ���룬�˴��ٶ���ջ���������Ϊͼ����������
	CPoint *stack=new CPoint[m_imgWidth*m_imgHeight];

	//ջ��ָ��
	int top;

	//��ǰ������ĵ�͵����ĵ�
	CPoint currentPoint, popPoint;

	//ѭ������������array����ĵ�һά�±�
	int k;

	//��Ǳ���
	int label;

	//��ʱ����
	int temp1, temp2;

	//��¼�������صĻҶ�ֵ
	temp1=*(m_pImgData+seed.y*lineByte+seed.x);

	//���������ӵ��ñ��0,��ջ
	*(m_pImgDataOut+seed.y*lineByte+seed.x)=0;
	top=0;
	stack[top].x=seed.x;
	stack[top].y=seed.y;

	//��ջ
	while(top>-1){
		//����ջ��Ԫ��,��Ԫ���Ѿ�������
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//���쵯��������Χ�Ƿ���û������������
		for(k=0;k<8;k++){

			//������������
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//���������ĵ㲻��ͼ���ڣ�������
			if(currentPoint.x<0||currentPoint.x>m_imgWidth-1||
				currentPoint.y<0||currentPoint.y>m_imgHeight-1)
				continue;
			
			//�õ���
			label=*(m_pImgDataOut+currentPoint.y*lineByte+currentPoint.x);

			//�����ĵ���Χ����û�����ĵ�
			if(label==255){
				temp2=*(m_pImgData+currentPoint.y*lineByte+currentPoint.x);
				//�����ǰ����������ػҶ�ֵ�����ӵ�Ҷ�ֵ֮��С�ڸ�������ֵ,
				//����Ϊ����,�����ջ����
				if(abs(temp1-temp2)<thresh){
					//���õ����������0
					*(m_pImgDataOut+currentPoint.y*lineByte+currentPoint.x)=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}
		
	}
	
	//���������
	delete []stack;
}



/***********************************************************************
* �������ƣ�
* ContourExtract()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵��������һ����ֵͼ�񣬰Ѻ�ɫ������Ŀ�꣬��ɫ������������ȡĿ�������
***********************************************************************/
void ImgSegment::ContourExtract()
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//������������ȡ����

	//������ͼ�����ݿ��������ͼ�񻺳���
	memcpy(m_pImgDataOut,m_pImgData,lineByte*m_imgHeight);

	//��źڵ�����8��������ػҶ�ֵ
	int array[8];

	//�������
	int sum;

	//ѭ������
	int i, j, k;

	//����ͼ���еĺڵ㣬�����Ǳ߽��ϵĵ�
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			//�ҵ�һ���ڵ�
			if(*(m_pImgData+i*lineByte+j)==0){
				//������Χ8�������ؽ�array����
		    	array[0]=*(m_pImgData+i*lineByte+j+1);
				array[1]=*(m_pImgData+(i+1)*lineByte+j+1);
				array[2]=*(m_pImgData+(i+1)*lineByte+j);
				array[3]=*(m_pImgData+(i+1)*lineByte+j-1);
				array[4]=*(m_pImgData+i*lineByte+j-1);
				array[5]=*(m_pImgData+(i-1)*lineByte+j-1);
				array[6]=*(m_pImgData+(i-1)*lineByte+j);
				array[7]=*(m_pImgData+(i-1)*lineByte+j+1);
				
				//���������
				sum=0;
				for(k=0;k<8;k++)
					sum += array[k];

				//��Χ8�����Ϊ�ڵ㣬�����ͼ���Ӧ�����ð�ɫ
				if(sum==0)
				   *(m_pImgDataOut+i*lineByte+j)=255;
				
			}
		}
	}

}

/***********************************************************************
* �������ƣ�
* ContourTrace()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵��������һ����ֵͼ�񣬰Ѻ�ɫ������Ŀ�꣬��ɫ��������������Ŀ��ı߽�
***********************************************************************/
void ImgSegment::ContourTrace()
{
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			           sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//�����ͼ���ʼ����255,��0�����������ر��
	memset(m_pImgDataOut,255,lineByte*m_imgHeight);

	//˳ʱ�붨���������ص��8�������꣬��һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

	//�߽���ʼ��,������ĵ�ǰ��,��ǰ��������
	CPoint startP, currentP, neighborP;

	//�Ƿ�ǰ������ʼ�߽���غϵı�־����
	int findStartPoint;

	//�����߽���ʼ��
	findStartPoint=0;

	//ѭ��������ͼ������
	int i,j;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			//�ҵ���ʼ�߽��
			if(*(m_pImgData+i*lineByte+j)==0){
				startP.x=j;
				startP.y=i;
				*(m_pImgDataOut+i*lineByte+j)=0;
				findStartPoint=1;
				break;
			}
		}

		//�Ѿ��ҵ���ʼ�߽��
		if(findStartPoint)
			break;
	}

	//�߽����
	//�ӳ�ʼ�㿪ʼ����
	currentP.x=startP.x;
	currentP.y=startP.y;

	//������Ƿ�߽���־����
	int isContourP;

	//��ʼ����
	int startDirect=0;

	//0��ʾ��û�з�������ı߽���ʼ��
	findStartPoint=0;
	while(findStartPoint==0){
		isContourP=false;
		while(isContourP==false){
			neighborP.x=currentP.x+direction[startDirect][0];
			neighborP.y=currentP.y+direction[startDirect][1];

			//�����������
			if(*(m_pImgData+neighborP.y*lineByte+neighborP.x)==0){
				isContourP=true;
				currentP.x=neighborP.x;
				currentP.y=neighborP.y;

				if(currentP.x==startP.x&&currentP.y==startP.y)
					findStartPoint=true;//�ص��߽���ʼ����

				*(m_pImgDataOut+currentP.y*lineByte+currentP.x)=0;

				//ɨ�跽����ʱ����ת90��
				startDirect-=2;
				if(startDirect<0)
					startDirect+=8;
			}
			else{
				//ɨ�跽��˳ʱ����ת45��
				startDirect++;
				if(startDirect==8)
					startDirect=0;
			}

		}
	}
	
}

