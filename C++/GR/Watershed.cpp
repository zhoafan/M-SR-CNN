#include "stdafx.h"
#include "Watershed.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/***********************************************************************
* �������ƣ�
* Watershed()
*
*˵�����޲����Ĺ��캯�����Գ�Ա�������г�ʼ��
***********************************************************************/
Watershed::Watershed()
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

	//���л��������ȣ�û����watershedǰ��������л�����
	queueLng=0;

	objectCount=0;//����Ŀ������0
}

/***********************************************************************
* �������ƣ�
* Watershed()
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
Watershed::Watershed(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData):
          ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	

	m_imgWidthOut=0;//���ͼ��Ŀ�Ϊ0

	m_imgHeightOut=0;//���ͼ��ĸ�Ϊ0

	//���л��������ȣ�û����watershedǰ��������л�����
	queueLng=0;

	objectCount=0;//����Ŀ������0
}


/***********************************************************************
* �������ƣ�
*   ~Watershed()
*
*˵���������������ͷ���Դ
***********************************************************************/
Watershed::~Watershed()
{
	//�ͷ����ͼ������ɫ������
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut==NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	//�ͷŶ��л�����
	if(queueLng!=0){
		//ѭ�������������±�
		int i,j;
		//������л�����
		for(i=0; i<256;i++){
			for(j=0;j<4;j++){
				delete []queue[i][j];
			}
		}

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
CSize Watershed::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

/***********************************************************************
* �������ƣ�
* Pop()
*
*����������
*  int gradient  -����Ԫ�صĻҶ�ֵ��gradient�еĶ���
*  int mark  -����Ԫ�صı�ǣ�mark�еĶ���
*  CPoint* pPopPoint  -����Ԫ�ص�����ָ��
*
*����ֵ��
*  ��
*
*˵����һ����256*4�����У�������ÿһ��Ԫ����һ��CPoint���͵������������
*      ͼ�����ص����꣬�ú���ʹ��gradient��mark�еĶ��г���һ��Ԫ�أ�Ԫ������
*      ��ָ��pPopPoint�з���
***********************************************************************/
void Watershed::Pop(int gradient,int mark,CPoint* pPopPoint)
{
	//��gradient�Ҷȼ����Ϊmark�Ķ��еĶ�ͷ
	int top=headPointer[gradient][mark];

	//��gradient�Ҷȼ����Ϊmark�Ķ��еĶ�ͷ����
	pPopPoint->x=(queue[gradient][mark]+top)->x;
	pPopPoint->y=(queue[gradient][mark]+top)->y;

	//��gradient�Ҷȼ����Ϊmark�Ķ��еĶ�β
	int tail=tailPointer[gradient][mark];

	//�����ͷָ��Ͷ�β����������п�
	if(top==tail){
		headPointer[gradient][mark]=0;
		tailPointer[gradient][mark]=-1;
	}
	else{//���򣬶�ͷָ���ۼ�
		headPointer[gradient][mark] += 1;
		//ѭ�����У�ÿ�ν���Ҫ����ģ����
		headPointer[gradient][mark] = headPointer[gradient][mark]%queueLng;
	}
	
}

/***********************************************************************
* �������ƣ�
* Push()
*
*����������
*  int gradient  -����Ԫ�صĻҶ�ֵ��gradient�еĶ���
*  int mark  -����Ԫ�صı�ǣ�mark�еĶ���
*  CPoint pushPoint  -����Ԫ�ص�����
*
*����ֵ��
*  ��
*
*˵����һ����256*4�����У�������ÿһ��Ԫ����һ��CPoint���͵������������
*      ͼ�����ص����꣬�ú���ʹ��gradient��mark�еĶ��н���һ��Ԫ�أ�Ԫ������
*      ΪpushPoint��ֵ
***********************************************************************/
void Watershed::Push(int gradient,int mark,CPoint pushPoint)
{
	//��βָ���1
	tailPointer[gradient][mark] +=1;

	//�Ըö�����ģ����
	tailPointer[gradient][mark]=tailPointer[gradient][mark]%queueLng;

	//�м��������βָ��
	int tail=tailPointer[gradient][mark];

	//��Ԫ�ؽ���
	(queue[gradient][mark]+tail)->x=pushPoint.x;
	(queue[gradient][mark]+tail)->y=pushPoint.y;
}

/***********************************************************************
* �������ƣ�
* IsEmpty()
*
*����������
*  int gradient  -gradient�еĶ���
*  int mark  -mark�еĶ���
*
*����ֵ��
*   1�����棬0�����
*
*˵�����жϵ�gradient��mark�еĶ����Ƿ�Ϊ�գ�������ֵΪ1����пգ����򲻿�
***********************************************************************/
bool Watershed::IsEmpty(int gradient,int mark) 
{	
	if(tailPointer[gradient][mark]==-1)
	    return true;
	else return false;
}

/***********************************************************************
* �������ƣ�
* IsOverflow()
*
*����������
*  int gradient  -gradient�еĶ���
*  int mark  -mark�еĶ���
*
*����ֵ��
*   1�����棬0�����
*
*˵�����жϵ�gradient��mark�еĶ����Ƿ�����������ֵΪ1���������������
***********************************************************************/
bool Watershed::IsOverflow(int gradient,int mark) 
{
	//��ͷָ��
	int head=headPointer[gradient][mark];
	
	//��βԪ��ָ���1
	int tail=(tailPointer[gradient][mark]+1)%queueLng;

	if(tailPointer[gradient][mark]!=-1 && tail==head)
	    return true;
	else return false;
}

/***********************************************************************
* �������ƣ�
* InitQueue()
*
*����������
*  unsigned char* pGradient -ͼ����ݶ�ͼ
*  unsigned char* pMarker  -ͼ��ı��ͼ
*  int width  -ͼ��Ŀ�
*  int height  -ͼ��ĸ�
*
*����ֵ��
*   ��
*
*˵���������ݶ�ͼ�ͱ��ͼ���Զ��н��г�ʼ��
***********************************************************************/
void Watershed::InitQueue(unsigned char* pGradient, unsigned char* pMarker, 
						  int width,int height)
{
	//ÿ�������ֽ���
	int linebyte=(width+3)/4*4;

	//ѭ������
	int i,j,k;

	//��ά����direct����8�����������������ص���x��y�����ϵ�ƫ��,
	//���е�һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direct[8][2]={{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};

	//�м�������Ҷ�ֵ�ͱ��
	int gradient, mark;

	//Ҫ���ӵ�Ԫ��
	CPoint pushPoint;

	//��ʼ����ͷ�Ͷ�βָ��
	for(i=0;i<256;i++){
		for(j=0;j<=objectCount;j++){
			tailPointer[i][j]=-1;
			headPointer[i][j]=0;
		}
	}

	//��ǰ�������ص������
	CPoint neighborPoint;
	for(i=0;i<height;i++){//ͼ���Ե������
		for(j=0;j<width;j++){
			mark=pMarker[i*linebyte+j];
			if(mark!=0){//�ҵ�һ����ǵ�			
				for(k=0;k<8;k++){
					neighborPoint.x=j+direct[k][0];
					neighborPoint.y=i+direct[k][1];

					//����ͼ��Χ�ڵĵ㣬����
					if(neighborPoint.x<0||neighborPoint.x>width-1
						||neighborPoint.y<0||neighborPoint.y>height-1)
						continue;

					//�ñ�ǵ��Ǹ�����ı�Ե�㣬�����
					if(*(pMarker+ neighborPoint.y*linebyte+neighborPoint.x)==0||
						*(pMarker+ neighborPoint.y*linebyte+neighborPoint.x)!=mark){
						gradient=*(pGradient+ i*linebyte+j);
						
						if(IsOverflow(gradient,mark)){
							CString str;
							str.Format("%d",gradient);
							CString str1="��"+ str + "���������";
							::MessageBox(0,str1,MB_OK,0);
							return;
						}
						else {
							pushPoint.x=j;
							pushPoint.y=i;
							Push(gradient,mark,pushPoint);
						}
						
						break;
					}
				}
			
			}//if end
		}//for(j)  end
	}//for(i) end
}

/***********************************************************************
* �������ƣ�
* WatershedTrans()
*
*����������
*  unsigned char* pGradient -ͼ����ݶ�ͼ
*  unsigned char* pMarker  -ͼ��ı��ͼ
*  int width  -ͼ��Ŀ�
*  int height  -ͼ��ĸ�
*  bool flag_contour  -��־������1�����ڲ�ͬ��Ŀ�����Ϸ�ˮ����(����)��0������
*
*����ֵ��
*   ��
*
*˵���������ݶ�ͼ�ͱ��ͼ����ͼ�����ˮ���������������������ͼpMarker��
***********************************************************************/
void Watershed::WatershedTrans(unsigned char* pGradient, unsigned char* pMarker, 
						  int width,int height,bool flag_contour)
{
	//ÿ�������ֽ���
	int linebyte=(width+3)/4*4;

	//ѭ��������ͼ�������
	int i,j;

	//��ά����direct����8�����������������ص���x��y�����ϵ�ƫ��,
	//���е�һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direct[8][2]={{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};

	// ѭ��������direct����8�����±꣨��1ά��
	int k;

	//���ӵ㡢���ӵ�͵�ǰ�������ص������
	CPoint pushPoint,popPoint, neighborPoint;

	//��־������1����ǰ���ص���Ŀ��������㣬������
	bool contourFlg;

	//��־������1����ǰ���ص���������������ı�Ե����һ��������Ŀ���������
	//������
	bool edgeFlg;

	//��ʼ������
	InitQueue(pGradient,pMarker,width,height);

	//���صĻҶ�ֵ�ͱ��
	int gradient, mark;
	
	//��¼��ǰ�ҵ���Ҫ���ӵĵ��ֵ
	int minGradient;

	//ˮ������
	for(i=0;i<256;i++){
		for(j=0;j<objectCount+1;j++){
			while(!IsEmpty(i,j)){
				//����һ��Ԫ��
				Pop(i,j,&popPoint);
				minGradient=256;
				pushPoint.x=-1;
				pushPoint.y=-1;
				for(k=0;k<8;k++){ 
					neighborPoint.x=popPoint.x+direct[k][0];
					neighborPoint.y=popPoint.y+direct[k][1];

					//����㲻��ͼ���ڣ�������
					if(neighborPoint.y<0||neighborPoint.y>height-1
						||neighborPoint.x<0||neighborPoint.x>width-1)
						continue;

					mark=*(pMarker+neighborPoint.y*linebyte+neighborPoint.x);
					if(mark==0){
						gradient=*(pGradient+neighborPoint.y*linebyte+neighborPoint.x);
						if(gradient<minGradient){
							minGradient=gradient;
							pushPoint.x=neighborPoint.x;
							pushPoint.y=neighborPoint.y;
						}
					}
					
				} //for(k) end
				
				//��������ӵ��µ�
				if(pushPoint.x!=-1&&pushPoint.y!=-1){
					*(pMarker+pushPoint.y*linebyte+pushPoint.x)=j;
					edgeFlg=0;
					contourFlg=0;

					//�жϸ��µ��Ƿ��Ե���������
					for(k=0;k<8;k++){
						neighborPoint.x=pushPoint.x+direct[k][0];
						neighborPoint.y=pushPoint.y+direct[k][1];

						//����㲻��ͼ���ڣ�������
						if(neighborPoint.y<0||neighborPoint.y>height-1
							||neighborPoint.x<0||neighborPoint.x>width-1)
							continue;

						mark=*(pMarker+neighborPoint.y*linebyte+neighborPoint.x);
						if( mark==0  ){
							edgeFlg=1;							
						}	
						if(  mark!=j && mark!=objectCount+1 &&mark!=0){
							contourFlg=1;
						}
					}
					if(flag_contour){//��ͬ���ӷ�ˮ���ߵı�־(����)
						if(contourFlg) 
							*(pMarker+pushPoint.y*width+pushPoint.x)=objectCount+1;
					}
					if(edgeFlg){										
						if(minGradient<i){
							if(IsOverflow(i,j)){
								CString str;
								str.Format("%d",minGradient);
								CString str1="��"+ str + "���������";
								::MessageBox(0,str1,MB_OK,0);
								return;
							}
							
							Push(i,j,pushPoint);
						}					
						else{
							if(IsOverflow(minGradient,j)){
								CString str;
								str.Format("%d",minGradient);
								CString str1="��"+ str + "���������";
								::MessageBox(0,str1,MB_OK,0);
								return;
							}
							Push(minGradient,j,pushPoint);
							
						}	
					}
				}

				//����ղų��ӵĵ㣬���õ���Ȼ�Ǳ�Ե�㣬�����½���
				edgeFlg=0;
				contourFlg=0;

				//�жϱ��������Ƿ��Ե���ҷ�����
				for(k=0;k<8;k++){
					neighborPoint.x=popPoint.x+direct[k][0];
					neighborPoint.y=popPoint.y+direct[k][1];

					//����㲻��ͼ��Χ�ڣ�������
					if(neighborPoint.y<0||neighborPoint.y>height-1
						||neighborPoint.x<0||neighborPoint.x>width-1)
						continue;
					mark=*(pMarker+neighborPoint.y*linebyte+neighborPoint.x);
					if( mark==0  ){
						edgeFlg=1;
					}
					if(  mark!=j && mark!=objectCount+1 &&mark!=0){
						contourFlg=1;
					}
				}				
				
				if(flag_contour){//��ͬ���ӷ�ˮ���ߵı�־(����)
					if(contourFlg) //�����߱�־
						*(pMarker+popPoint.y*linebyte+popPoint.x)=objectCount+1;						
				}
				if(edgeFlg){				
					if(IsOverflow(i,j)){
						CString str;
						str.Format("%d",minGradient);
						CString str1="��"+ str + "���������";
						::MessageBox(0,str1,MB_OK,0);
						return;
					}
					Push(i,j,popPoint);
				}
				
				
			}//while end
			
		}// for(j) end

	}//for(i) end

}

/***********************************************************************
* �������ƣ�
* GenerateMarkerByPos()
*
*����������
*  unsigned char* pMarker  -ͼ��ı��ͼ
*  int width  -ͼ��Ŀ�
*  int height  -ͼ��ĸ�
*  CPoint position[]   -ָ�����ӵ�����
*  int objectCount   -������
*
*����ֵ��
*   ��
*
*˵�����������ӵ�����position��������objectCount���������ͼpMarker
***********************************************************************/
void Watershed::GenerateMarkerByPos(unsigned char* pMarker, int width,
							int height, CPoint position[], int objectCount)
{
	//ÿ�������ֽ���
	int linebyte=(width+3)/4*4;

	//��ʼ�����ͼ��0��0����δ����ǵĵ�(�ȴ������ĵ�)
	memset(pMarker, 0, linebyte*height);

	//ѭ������
	int k;
	//����k��ָ�����óɱ��k��
	for(k=0;k<objectCount;k++){
		*(pMarker+position[k].y*linebyte+position[k].x)=k+1;
	}
}

/***********************************************************************
* �������ƣ�
* Sobel()
*
*����������
*  unsigned char* imgBuf  -ͼ������
*  int width  -ͼ��Ŀ�
*  int height  -ͼ��ĸ�
*  unsigned char *gradient  -�ݶ�ͼ
*
*����ֵ��
*   ��
*
*˵��������ͼ�����ݣ�����sobel�任���õ����ݶ�ͼ����gradient���������˴�
*      û�е���ǰ��ImgSegment���е�Sobel()����Ϊ��Watershed��ļ����ԣ���
*      �ڴ�����ֲ
***********************************************************************/
void Watershed::Sobel(unsigned char *imgBuf, int width, int height,
					  unsigned char *gradient)
{
	//ÿ�������ֽ���
	int linebyte=(width+3)/4*4;

	//�ݶ�ͼ�񻺳�����ʼ����0��
	memset(gradient, 0, linebyte*height);

	//ѭ��������ͼ�������
	int i, j;

	//�м����
	int x, y, t;

	for(i=1;i<height-1; i++){
		for(j=1; j<width-1; j++){
			//x�����ݶ�
			x=*(imgBuf+(i+1)*linebyte+j-1)+2**(imgBuf+(i+1)*linebyte+j)
				+*(imgBuf+(i+1)*linebyte+j+1) -*(imgBuf+(i-1)*linebyte+j-1)
				-2**(imgBuf+(i-1)*linebyte+j)-*(imgBuf+(i-1)*linebyte+j+1);

			//y�����ݶ�
			y=*(imgBuf+(i+1)*linebyte+j-1)+2**(imgBuf+i*linebyte+j-1)
				+*(imgBuf+(i-1)*linebyte+j)-*(imgBuf+(i+1)*linebyte+j+1)
				-2**(imgBuf+i*linebyte+j+1)-*(imgBuf+(i-1)*linebyte+j+1);

			//�ݶ�
			t=sqrt(x*x+y*y);
			if(t>255) t=255;
			*(gradient+i*linebyte+j)=t;
		}
	}
}

/***********************************************************************
* �������ƣ�
* WatershedSegment()
*
*����������
*  CPoint position[]  -���ӵ�����
*  int classCount  -������
*  int contourFlg  -��־�Ա�����1Ϊ���ӣ��������ߣ���0Ϊ�����ӣ����������ߣ�
*
*����ֵ��
*   ��
*
*˵�����������ӵ㼰��������������ͼ�����ˮ��ָ��
***********************************************************************/
void Watershed::WatershedSegment(CPoint position[], int classCount, int contourFlg)
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

	//���ͼ��Ϊ���ͼ������,�˴�Ϊ�Ҷ�����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
              sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByte=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//queueLng��ʾ���еĳ��ȣ�Ϊ��ʡ�ռ䣬��ֵΪͼ������������1/16��ʵ�ʴ����ж�
	//���������ԶԶС�ڱ������趨��ֵ�����߲���ָ�뷽ʽʵ�ֶ��и�Ϊ���� 
	queueLng=m_imgWidth*m_imgHeight/16;

	//�����
	objectCount=classCount;

	//ѭ������
	int i, j;

	//������л�����
	for(i=0;i<256;i++){
		for(j=0;j<4;j++)
    		queue[i][j]=new CPoint[queueLng];

	}

	//ͨ��ָ��������������,��ʱm_pImgDataOut���ǳ�ʼ���ͼ
	GenerateMarkerByPos(m_pImgDataOut, m_imgWidth, m_imgHeight, 
		position, objectCount);


	//���뻺���������sobel�任�ݶ�ͼ
	unsigned char *sobelBuf=new unsigned char[lineByte*m_imgHeight];
	Sobel(m_pImgData, m_imgWidth, m_imgHeight, sobelBuf);

	//ˮ��任���任��m_pImgDataOut�ڴ�ŵ���ˮ�������Ժ�ı��ͼ�����ָ���
	WatershedTrans(sobelBuf, m_pImgDataOut, m_imgWidth, m_imgHeight,contourFlg);

	//���ͼ��һ��������ʾ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			if(*(m_pImgDataOut+i*lineByte+j)==objectCount+1)
				*(m_pImgDataOut+i*lineByte+j)=255;
				else
	     		*(m_pImgDataOut+i*lineByte+j)= 
				(*(m_pImgDataOut+i*lineByte+j)-1)*255/objectCount;
		}
	}

	//����ݶ�ͼ������
	delete []sobelBuf;

	//������л�����
	if(queueLng!=0){
		for(i=0;i<256;i++){
			for(j=0;j<4;j++){
				//�ͷŶ��л�����
				delete []queue[i][j];
			}
			
		}
		queueLng=0;
	}
}



/*
//��ֱ��ͼ�������ɱ��ͼ,threshInterval��ֵ������
void Watershed::calculateMarker(unsigned char* pImgBuf, unsigned char* pMarker, int width,int height, int threshInterval[], int objectCount)
{
	//ÿ�������ֽ���
	int linebyte=(width+3)/4*4;

	//��ʼ�����ͼ��0��0����δ����ǵĵ�(�ȴ������ĵ�)
	memset(pMarker, 0, linebyte*height);
	
	//ѭ������
	int i,j,k;
	unsigned char t;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			for(k=0;k<objectCount;k++){
				//����Ϊ(i,j)���ص�ָ��
				t=*(pImgBuf+i*linebyte+j);
				if(t>=threshInterval[k*2+0]  &&  t<threshInterval[k*2+1])//��k��Ŀ��Ҷ�����
					*(pMarker+i*linebyte+j)=k+1;
			}
		}
	}
	
}

void Watershed::WatershedSegment1(int threshInterval[], int classCount, int contourFlg)
{
	if(classCount<2||classCount>3){
		::MessageBox(0,"������ֻ����2��3",MB_OK,0);
		return;
	}
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

	//���ͼ��Ϊ���ͼ������,�˴�Ϊ�Ҷ�����
	m_nBitCountOut=8;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByte=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	//���ӵ�������ÿ������ı�Ե���أ���Ե���ص���ĿҪС�������������
	//��ͼ���ֱ��ֳ�classCount��Ŀ���࣬���ÿ�����е��ԶС��ͼ�����ص�����
	//maxQueueBufLng������Ϊ������л�������С�����ݣ�Ϊ��ʡ�ռ䣬�˴�maxQueueBufLng��ֵΪͼ��������������16
	//ʵ�ʴ����ж����������ԶԶС�ڱ������趨��ֵ
	maxQueueBufLng=m_imgWidth*m_imgHeight/16;

	//�����
	objectCount=classCount;
	//ѭ������
	int i, j;

	//������л�����
	for(i=0;i<256;i++){
		for(j=0;j<4;j++)
    		queue[i][j]=new CPoint[maxQueueBufLng];

	}

	//ͨ��ָ��������������ͼ,��ʱm_pImgDataOut�ǳ�ʼ���ͼ
    calculateMarker(m_pImgData, m_pImgDataOut, m_imgWidth, m_imgHeight, threshInterval, objectCount);

	//���뻺����������ݶ�ͼ
	unsigned char *sobelBuf=new unsigned char[lineByte*m_imgHeight];

	//����sobel�ݶ�ͼ
	sobel(m_pImgData, m_imgWidth, m_imgHeight, sobelBuf);

	//ˮ��任
	waterShed(sobelBuf, m_pImgDataOut, m_imgWidth, m_imgHeight,contourFlg);
	//��ʾ
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			if(*(m_pImgDataOut+i*lineByte+j)==objectCount+1)
				*(m_pImgDataOut+i*lineByte+j)=255;
				else
	     		*(m_pImgDataOut+i*lineByte+j)= 
				(*(m_pImgDataOut+i*lineByte+j)-1)*255/objectCount;
		}
	}
	
	

	//����ݶȻ�����
	delete []sobelBuf;


	//������л�����
	if(maxQueueBufLng!=0){
		for(i=0;i<256;i++){
			for(j=0;j<4;j++){
			//������л�����
			delete []queue[i][j];
			}
			
		}
		maxQueueBufLng=0;
	}
}

  
	*/