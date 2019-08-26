#include "stdafx.h"
#include "GrayTrans.h"
#include "math.h"

/***********************************************************************
* �������ƣ�
* GrayTrans()
*
*˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
GrayTrans::GrayTrans()
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
* GrayTrans()
*
*����������
*  CSize size -ͼ���С�����ߣ�
*  int nBitCount  -ÿ���ر�����
*  LPRGBQUAD lpColorTable  -��ɫ��ָ��
*  unsigned char *pImgData  -λͼ����ָ��
*
*����ֵ��
*   ��
*
*˵����������Ϊ�������Ĺ��캯��������λͼ�Ĵ�С��ÿ����λ������ɫ��
*      ��λͼ���ݣ�����ImgCenterDib()�Ի����Ա��ʼ������Ϊ����ͼ������
*      ��ص����ݳ�Ա,����ʼ�����ͼ����ص����ݳ�Ա
***********************************************************************/
GrayTrans::GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData):
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
*   ~GrayTrans()

*
*˵���������������ͷ���Դ
***********************************************************************/
GrayTrans::~GrayTrans()
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
CSize GrayTrans::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/**************************************************************************
*������
*    ReverseImg()
*
*������
*    ��
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬽�������������������m_pImgDataOut�С�
***************************************************************************/
void GrayTrans::ReverseImg()//����
{
	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
	//���ͼ��ÿ����λ����ԭͼ��ͬ
	m_nBitCountOut=m_nBitCount;

	//��ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//�������ɫ����Ϊ��ɫ������ڴ�
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ������ռ�ֽ���������Ϊ4�ı���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	
	//����ռ䣬��ű任����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ���
	int pixelByte=m_nBitCountOut/8;

	//ѭ������,����ÿ�����ص�ÿ������,�����ɫͼ����������
	int k;

	//����
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k)
				=255-*(m_pImgData+i*lineByteOut+j*pixelByte+k);
		}
	}


}


/**************************************************************************
*������
*    ColorToGray()
*
*������
*    ��
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬽������ֵ�������������m_pImgDataOut�С�
***************************************************************************/
void GrayTrans::ColorToGray()//��ֵ��
{
	//���Ҷ�ͼ��,�򷵻�
	if(m_nBitCount==8) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//��ֵ����,ÿ����λ��Ϊ8����
	m_nBitCountOut=8;

	//��ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������,���ɻҶ�ͼ�����ɫ��
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
	int pixelByteIn=3;
	
	//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//�������ͼ��λͼ���ݻ�����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//ѭ������,ͼ�������
	int i,j;

	//���ݻ�ֵ����ʽΪ���ͼ��ֵ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++)
			*(m_pImgDataOut+i*lineByteOut+j)=0.11**(m_pImgData+i*lineByteIn+j*pixelByteIn+0)
		        	+0.59**(m_pImgData+i*lineByteIn+j*pixelByteIn+1)
		        	+0.30**(m_pImgData+i*lineByteIn+j*pixelByteIn+2)+0.5;
	}

}

/**************************************************************************
*������
*    Binary()
*
*������
*    int threshold   -��ֵ
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬸��ݸ�������ֵthreshold��ͼ����ж�ֵ����
*      �������m_pImgDataOut�С����ǲ�ɫͼ�����Ƚ�ͼ����л�ֵͼ����ٶ�ֵ����
***************************************************************************/
void GrayTrans::Binary(int threshold)
{
	//���ڻҶ�ͼ��
	if(m_nBitCount==8){
		//�ͷžɵ����ͼ�����ݼ���ɫ������
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
		
		//�������ͼ����ɫ����������������ͼ����ɫ���������ͼ����ɫ����
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
		//���ͼ��Ŀ��,������ͼ�����
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;
		
		
		//ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ������
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

		//�������ͼ��λͼ���ݻ�����
		m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

		//ѭ��������ͼ�������
		int i,j;

		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++){
				if(*(m_pImgData+i*lineByte+j)<threshold)
    				*(m_pImgDataOut+i*lineByte+j)=0;
				else
					*(m_pImgDataOut+i*lineByte+j)=255;
			}
		}
	}
	else{

		//���ǲ�ɫͼ��,�ȱ�ɻҶȸ�ʽ���任��m_pImgDataOut�Ѿ��������ڴ棬
        //������˻Ҷ�ͼ������
		ColorToGray();

		//���ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ�������
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

		//ѭ��������ͼ�������
		int i,j;

		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++)
				if(*(m_pImgDataOut+i*lineByteOut+j)<threshold)
					*(m_pImgDataOut+i*lineByteOut+j)=0;
				else
					*(m_pImgDataOut+i*lineByteOut+j)=255;
		}
	}
	
}

/**************************************************************************
*������
*    GrayToColor()
*
*������
*    ��
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬽��Ҷ�ͼ���ɲ�ɫ��ʽ�������
*      ��m_pImgDataOut�С�
*      
***************************************************************************/
void GrayTrans::GrayToColor()
{
	//���ǲ�ɫ��ʽ���򷵻�
	if(m_nBitCount==24) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��Ϊ24����
	m_nBitCountOut=24;

	//��ɫͼ����ɫ����Ϊ0��������ɫ��
	m_nColorTableLengthOut=0;

	//����ĻҶ�ͼ��ÿ��������ռ�ֽ���������Ϊ4�ı���
	int lineByteIn=(m_imgWidth+3)/4*4;

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//����Ĳ�ɫͼ��ÿ��������ռ�ֽ���������Ϊ4�ı���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//�������ͼ��λͼ���ݻ�����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//i,jΪͼ������ѭ��������kΪ��ɫͼ������ͨ����ѭ������
	int i,j,k;

	//�Ҷȱ��ɫ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			//�����ػҶ�ֵΪ��ɫ��ʽ��ÿ��ͨ����ֵ������ͨ����ֵ���
			for(k=0;k<3;k++)
			  *(m_pImgDataOut+i*lineByteOut+j*3+k)=*(m_pImgData+i*lineByteIn+j);
		}
	}
}


/***********************************************************************
* �������ƣ�
*   LinearStrech()
*
*��������:
*   CPoint point[2]  -�ֶ����ԵĶ�Ӧ��
*
*����ֵ: 
*    ��
*
*˵�����ֶ��������캯��,���������ֶε㣬��m_pImgData��ָ�򻺳����еĻҶ�
*      ���ɫͼ������������죬Ϊm_pImgDataOut�����ڴ棬�����������
***********************************************************************/
void GrayTrans::LinearStrech(CPoint point[2])
{
	//�ͷžɵ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//����ǻҶ�ͼ������õ�ͨ�������������캯��
		LinearStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,point);
	else{
		//����ɫͼ�����ÿ��ͨ�������ݽ��з��룬���������죬Ȼ���ٺϳ�Ϊ��ɫͼ��

		//��ͨ������ÿ��������ռ���ֽ���
		int lineByteGray=(m_imgWidth+3)/4*4;

		//��ʱ����������ŵ�ͨ������
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//��ʱ����������ͨ����������Ľ��
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//ѭ��������ͼ������
		int i, j;

		//ѭ��������������ɫͼ�������ͨ��
		int k;

		for(k=0;k<3;k++){
			//��ÿ��ͨ��������ȡ����������bufIn������
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//��bufIn�������е����ݽ������죬����������bufOut��
			LinearStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,point);

			//����ͨ������Ľ���������ͼ��m_pImgDataOut��Ӧͨ����
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//�ͷŻ�����
		delete []bufIn;
		delete []bufOut;
	}

}


/***********************************************************************
* �������ƣ�
*   LinearStrechForSnglChannel()
*
*��������:
*   unsigned char *pImgDataIn   -�������λͼ����ָ��
*   unsigned char *pImgDataOut  -�����λͼ����ָ��
*   int imgWidth   -λͼ��,����Ϊ��λ
*   int imgHeight  -λͼ��,����Ϊ��λ
*   CPoint point[2]  -�ֶ����Ե�����ת�۵�����
*
*����ֵ: 
*    ��
*
*˵��������һ��ͨ�������ݣ��Լ�����ת�۵����꣬���ֶ���������, �����
*     ����pImgDataOut��ָ��Ļ�������
***********************************************************************/
void GrayTrans::LinearStrechForSnglChannel(unsigned char *pImgDataIn, 
		unsigned char *pImgDataOut, int imgWidth,int imgHeight,CPoint point[2])
{
	//ѭ��������ͼ������
	int i,j;

	//ÿ�����ص��ֽ�������ͨ��ͼ��
	int lineByte=(imgWidth+3)/4*4;

	//(x1,y1)��(x2,y2)Ϊ�����ֶε������
	double x1=point[0].x,y1=point[0].y,x2=point[1].x, y2=point[1].y;

	//�����ֶε�ֱ��б��
	double slop1=y1/x1,slop2=(y2-y1)/(x2-x1),slop3=(255-y2)/(255-x2);

	//����ֱ�������б��
	double dis1=0,dis2=y1-slop2*x1,dis3=y2-slop3*x2;

	//ӳ���
	int map[256];

	//���������ֶΣ�������ͼ��ÿ���Ҷȼ�����ӳ���
	for(i=0;i<256;i++)	
	{
		if(i<x1)
			map[i]=(int)(slop1*i+dis1+0.5);
		else if(i<x2)
			map[i]=(int)(slop2*i+dis2+0.5);
		else
			map[i]=(int)(slop3*i+dis3+0.5);
	}

	//��ʱ����
	int tmp;

	//��ÿ�����أ������������ػҶȼ�����ӳ�����Ϊ������ظ�ֵ
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//����ͼ��(i,j)���صĻҶȼ�
			tmp=*(pImgDataIn+i*lineByte+j);

			//����ӳ���Ϊ���ͼ��ֵ
			*(pImgDataOut+i*lineByte+j)=map[tmp];
		}
	}
}


/***********************************************************************
* �������ƣ�
*   DuiShuStrechForSnglChannel()
*
*��������:
*   unsigned char *pImgDataIn   -�������λͼ����ָ��
*   unsigned char *pImgDataOut  -�����λͼ����ָ��
*   int imgWidth   -λͼ��,����Ϊ��λ
*   int imgHeight  -λͼ��,����Ϊ��λ
*   float a  -�������������Ա任����
*   float b  -�������������Ա任����
*   float c  -�������������Ա任����
*
*����ֵ: 
*    ��
*
*˵��������һ��ͨ�������ݣ���a��b��c����������������Ҫ�Ĳ��������з�����
*     ����, ���������pImgDataOut��ָ��Ļ�������
***********************************************************************/
void GrayTrans::DuiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		float a, float b, float c)
{
	//ѭ��������ͼ������
	int i,j;

	//ÿ�����ص��ֽ�������ͨ��ͼ��
	int lineByte=(imgWidth+3)/4*4;

	//��ʱ����
	float tmp1, tmp2, t;

	//�������������Ա任
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//������������ػҶ�ֵ��������������ػҶ�ֵ
			tmp1=log(*(pImgDataIn+i*lineByte+j)+1.0)/log(2.0);
			tmp2=b*log(c)/log(2.0);
			t=a+tmp1/tmp2;
			if(t>255)
				t=255;
			else if(t<0)
				t=0;
			*(pImgDataOut+i*lineByte+j)=t;
		}
	}
}


/***********************************************************************
* �������ƣ�
*   DuiShuStrech()
*
*��������:
*   float a  -�������������Ա任����
*   float b  -�������������Ա任����
*   float c  -�������������Ա任����
*
*����ֵ: 
*    ��
*
*˵���������������������죬����������������m_pImgData��ָ�򻺳����еĻҶ�
*      ���ɫͼ����ж������������죬Ϊm_pImgDataOut�����ڴ棬�����������
***********************************************************************/
void GrayTrans::DuiShuStrech(float a, float b, float c)
{
		//�ͷžɵ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//����ǻҶ�ͼ������õ�ͨ�����ݶ������캯��
		DuiShuStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,a,b,c);
	else{
		//����ɫͼ�����ÿ��ͨ�������ݽ��з��룬�����������죬Ȼ���ٺϳ�Ϊ��ɫͼ��

		//��ͨ������ÿ��������ռ���ֽ���
		int lineByteGray=(m_imgWidth+3)/4*4;

		//��ʱ����������ŵ�ͨ������
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//��ʱ����������ͨ����������Ľ��
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//ѭ��������ͼ������
		int i, j;

		//ѭ��������������ɫͼ�������ͨ��
		int k;

		for(k=0;k<3;k++){
			//��ÿ��ͨ��������ȡ����������bufIn������
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//��bufIn�������е����ݽ������죬����������bufOut��
			DuiShuStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,a,b,c);

			//����ͨ������Ľ���������ͼ��m_pImgDataOut��Ӧͨ����
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//�ͷŻ�����
		delete []bufIn;
		delete []bufOut;
	}
}


/***********************************************************************
* �������ƣ�
*   ZhiShuStrechForSnglChannel()
*
*��������:
*   unsigned char *pImgDataIn   -�������λͼ����ָ��
*   unsigned char *pImgDataOut  -�����λͼ����ָ��
*   int imgWidth   -λͼ��,����Ϊ��λ
*   int imgHeight  -λͼ��,����Ϊ��λ
*   float a  -ָ�����������Ա任����
*   float b  -ָ�����������Ա任����
*   float c  -ָ�����������Ա任����
*
*����ֵ: 
*    ��
*
*˵��������һ��ͨ�������ݣ���a��b��c����ָ����������Ҫ�Ĳ��������з�����
*     ����, ���������pImgDataOut��ָ��Ļ�������
***********************************************************************/
void GrayTrans::ZhiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		float a, float b, float c)
{
	//ѭ��������ͼ������
	int i,j;

	//ÿ�����ص��ֽ�������ͨ��ͼ��
	int lineByte=(imgWidth+3)/4*4;

	//��ʱ����
	float t;

	//�������������Ա任
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//������������ػҶ�ֵ��������������ػҶ�ֵ
			t=c*(*(pImgDataIn+i*lineByte+j)-a);
			t=pow(b,t)-1;
			if(t>255)
				t=255;
			else if(t<0)
				t=0;
			*(pImgDataOut+i*lineByte+j)=t;
		}
	}
}


/***********************************************************************
* �������ƣ�
*   ZhiShuStrech()
*
*��������:
*   float a  -ָ�����������Ա任����
*   float b  -ָ�����������Ա任����
*   float c  -ָ�����������Ա任����
*
*����ֵ: 
*    ��
*
*˵����ָ����������������֣�����ָ�������������m_pImgData��ָ�򻺳����еĻҶ�
*      ���ɫͼ�����ָ�����������죬Ϊm_pImgDataOut�����ڴ棬�����������
***********************************************************************/
void GrayTrans::ZhiShuStrech(float a, float b, float c)
{
		//�ͷžɵ����ͼ�񻺳���
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��ÿ����λ��������ͼ����ͬ
	m_nBitCountOut=m_nBitCount;

	//������ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������ͼ�����ɫ�������ͼ����ɫ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//���ͼ��ÿ��������ռ���ֽ���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//����ǻҶ�ͼ������õ�ͨ������ָ�����캯��
		ZhiShuStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,a,b,c);
	else{
		//����ɫͼ�����ÿ��ͨ�������ݽ��з��룬�����������죬Ȼ���ٺϳ�Ϊ��ɫͼ��

		//��ͨ������ÿ��������ռ���ֽ���
		int lineByteGray=(m_imgWidth+3)/4*4;

		//��ʱ����������ŵ�ͨ������
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//��ʱ����������ͨ����������Ľ��
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//ѭ��������ͼ������
		int i, j;

		//ѭ��������������ɫͼ�������ͨ��
		int k;

		for(k=0;k<3;k++){
			//��ÿ��ͨ��������ȡ����������bufIn������
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//��bufIn�������е����ݽ������죬����������bufOut��
			ZhiShuStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,a,b,c);

			//����ͨ������Ľ���������ͼ��m_pImgDataOut��Ӧͨ����
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//�ͷŻ�����
		delete []bufIn;
		delete []bufOut;
	}
}