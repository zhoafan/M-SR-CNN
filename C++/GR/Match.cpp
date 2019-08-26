#include "stdafx.h"
#include "Match.h"
#include "math.h"
Match::Match()
{

	m_nColorTableLengthOut=0;
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
    m_imgWidthOut=0;
	m_imgHeightOut=0;
	
}

Match::Match(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{

    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
	m_imgWidthOut=0;
	m_imgHeightOut=0;
}

Match::~Match()
{
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut==NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

CSize Match::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


//�������ƥ�亯��
CPoint Match::matchBasedSpacial(unsigned char *imgBuf, int width, int height, int pixelByte,
										unsigned char * blockBuf, int blockW, int blockH)
{

	//ͼ��ÿ�����ص��ֽ���
	int lineByte=(width*pixelByte+3)/4*4;
	//��׼ͼ��ÿ���ֽ���
	int blockLineByte=(blockW*pixelByte+3)/4*4;

	//ѭ����������׼ͼ����
	int i, j;

	//ѭ����������׼ͼ���꣨��������Сͼ��
	int i0, j0, k;

	//�м������ÿ����һ��λ�ü�¼��λ���ϻ�׼ͼ���׼ͼ֮�������Բ��
	//��������������֮�����ֵ�������Ϊ����Բ�ȣ�����sumֵ��С��λ��Ϊ���ƥ��λ��
	int sum;

	//��min��¼������Сsum��ֵ, 
	int min;
	//������Сsum��λ��
	int minX, minY;

	//�м����
	int t1, t2;

	//���������Ϊheight-blockH�����������Ϊwidth-blockW���ɿ����ݴ�С����
	//�������ֲ���������������ˣ�Ϊ�˽�ʡʱ�䣬�˴��з�����ݷ�����������Ϊ3��
	//���߿��Ը�����Ҫ�Լ�����������������Ϊ����������������
	for(i=0;i<height-blockH;i+=3){
		for(j=0;j<width-blockW; j+=3){
			sum=0;

			//���(i,j)ƥ��λ�ã����������ڵ����أ�Ϊ��ʡ�ռ䣬
			//�˴�(i0,j0)����������Ҳ�������������Ҫ����
			for(i0=0;i0<blockH;i0++){
				for(j0=0;j0<blockW;j0++){
					
					//ÿ��ͨ�����ݶ�Ӧ��������������ֵ֮�ͣ����ڻҶ�ͼ���Ƕ�Ӧ���ػҶ�ֵ�����
					// ���ڲ�ɫͼ�񣬴˴�Ϊ����ͨ����Ӧ���
					for(k=0;k<pixelByte;k++){
						//��ǰ�ο�ͼ�϶�Ӧ���ص�k��ͨ������
						t1=*(imgBuf+(i+i0)*lineByte+(j+j0)*pixelByte+k);

						//��ǰ��׼ͼ�϶�Ӧ���ص�k��ͨ������
						t2=*(blockBuf+i0*blockLineByte+j0*pixelByte+k);

						//��ľ���ֵ֮��
						sum += abs(t1-t2);

					}
				}
			}
			
			//��һ���������򣬴�ʱҲ����Сsumֵ��λ��
			if(i==0&&j==0){
				min=sum;
				minX=0;
				minY=0;
			}
			else{//�Ѿ����ǵ�һ������λ����
				if(min>sum){//����һ���µ���Сλ�ã���¼��ʱ����Сֵ����λ������
					min=sum;
					minX=j;
					minY=i;
				}
			}

		}
	}

	//����ƥ��λ��
	CPoint pos(minX,minY);
	return pos;
}

//
void Match::matchResult(LPCTSTR  cankaoImgPath, LPCTSTR jizhunImgPath,  int matchStyle)
{
	//�����׼ͼ���ú����ǻ���ImgCenterDib�еĺ�����Ա��ʵ�ֹ�����xfyDib.cpp�ļ���
	//�ú������ú�m_pImgData, m_imgWidth, m_imgHeight��ŵ��ǻ�׼ͼ����
	Read(jizhunImgPath);

	//��׼ͼ���ݱ���
	int blockBitCount=m_nBitCount;
	int blockW=m_imgWidth;
	int blockH=m_imgHeight;

	//��׼ͼÿ�������ֽ���
	int blockLineByte=(blockW*blockBitCount/8+3)/4*4;

	//����ռ��Ż�׼ͼͼ������
	unsigned char *blockBuf=new unsigned char[blockLineByte*blockH];
	memcpy(blockBuf, m_pImgData, blockLineByte*blockH);

	//����ο�ͼ���ú������ú�m_pImgData, m_imgWidth, m_imgHeight��ŵ��ǲο�ͼͼ������
	Read(cankaoImgPath);

	//��׼ͼ��ο�ͼͼ���ʽ��ͳһ
	if(m_nBitCount!=blockBitCount){
		//�ͷſռ�
		delete []blockBuf;
		//����
		return;
	}

	//ÿ�����ֽ���
	int pixelByte=m_nBitCount/8;

	//ƥ��λ�ñ���
	CPoint macthPos;
		macthPos.x=0;
	macthPos.y=0;

	if(matchStyle==0)  //�ռ����ƥ��
		macthPos=matchBasedSpacial(m_pImgData, m_imgWidth, m_imgHeight, pixelByte,
										blockBuf, blockW, blockH);
/*	else if(matchStyle==1)//Ƶ��ƥ��
�˴�Ҫ������λƥ��ͽǵ�ƥ��

*/

	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����,�˴�Ϊ�Ҷ�����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ�ߣ���ο�ͼ��С��ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	// �ο�ͼÿ�������ֽ���
	int lineByte=(m_imgWidth*blockBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeightOut];

	memcpy( m_pImgDataOut,m_pImgData, lineByte*m_imgHeightOut);

	//����׼ͼ��λ�û��ڲο�ͼ�У�����ʾƥ��������ȷ��

	//����ѭ������
	int i, k;

	//����׼ͼ���±߽��ڲο�ͼ��Ӧ�е������ú�ɫ����ÿ��ͨ����0
	for(i=0;i<blockW;i++){
		for(k=0;k<pixelByte; k++){
			*(m_pImgDataOut+(macthPos.y+0)*lineByte+(macthPos.x+i)*pixelByte+k)=0;
		}
		for(k=0;k<pixelByte; k++){
			*(m_pImgDataOut+(macthPos.y+blockH-1)*lineByte+(macthPos.x+i)*pixelByte+k)=0;
		}
	}

	//����׼ͼ���ұ߽��ڲο�ͼ��Ӧ�е������ú�ɫ����ÿ��ͨ����0
	for(i=0;i<blockH;i++){
		for(k=0;k<pixelByte; k++){
			*(m_pImgDataOut+(macthPos.y+i)*lineByte+(macthPos.x+0)*pixelByte+k)=0;
		}
		for(k=0;k<pixelByte; k++){
			*(m_pImgDataOut+(macthPos.y+i)*lineByte+(macthPos.x+blockW)*pixelByte+k)=0;
		}
	}

	//����ռ�
	 delete []blockBuf;

}
