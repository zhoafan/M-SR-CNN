#include "stdafx.h"
#include "GrayTrans.h"
#include "math.h"

/***********************************************************************
* 函数名称：
* GrayTrans()
*
*说明：无参数的构造函数，对成员变量进行初始化
***********************************************************************/
GrayTrans::GrayTrans()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0	

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

/***********************************************************************
* 函数名称：
* GrayTrans()
*
*函数参数：
*  CSize size -图像大小（宽、高）
*  int nBitCount  -每像素比特数
*  LPRGBQUAD lpColorTable  -颜色表指针
*  unsigned char *pImgData  -位图数据指针
*
*返回值：
*   无
*
*说明：本函数为带参数的构造函数，给定位图的大小、每像素位数、颜色表
*      及位图数据，调用ImgCenterDib()对基类成员初始化，作为输入图像数据
*      相关的数据成员,并初始化输出图像相关的数据成员
***********************************************************************/
GrayTrans::GrayTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{

    m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}


/***********************************************************************
* 函数名称：
*   ~GrayTrans()

*
*说明：析构函数，释放资源
***********************************************************************/
GrayTrans::~GrayTrans()
{
	//释放输出图像位图数据缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//释放输出图像颜色表
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***********************************************************************
* 函数名称：
* GetDimensions()
*
*函数参数：
*  无
*
*返回值：
*   图像的尺寸，用CSize类型表达
*
*说明：返回输出图像的宽和高
***********************************************************************/
CSize GrayTrans::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}


/**************************************************************************
*函数名
*    ReverseImg()
*
*参数名
*    无
*
*返回值
*    无
*
*说明：该函数将m_pImgData作为输入图像，将其求负相操作，结果存入m_pImgDataOut中。
***************************************************************************/
void GrayTrans::ReverseImg()//负相
{
	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	
	//输出图像每像素位数与原图相同
	m_nBitCountOut=m_nBitCount;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//如果有颜色表，则为颜色表分配内存
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	
	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素占字节数，必须为4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	
	//申请空间，存放变换后结果
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//循环变量，图像的坐标
	int i,j;

	//每像素占字节数
	int pixelByte=m_nBitCountOut/8;

	//循环变量,遍历每个像素的每个分量,比如彩色图像三个分量
	int k;

	//求负相
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			for(k=0;k<pixelByte;k++)
				*(m_pImgDataOut+i*lineByteOut+j*pixelByte+k)
				=255-*(m_pImgData+i*lineByteOut+j*pixelByte+k);
		}
	}


}


/**************************************************************************
*函数名
*    ColorToGray()
*
*参数名
*    无
*
*返回值
*    无
*
*说明：该函数将m_pImgData作为输入图像，将其求灰值操作，结果存入m_pImgDataOut中。
***************************************************************************/
void GrayTrans::ColorToGray()//灰值化
{
	//若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//灰值化后,每像素位数为8比特
	m_nBitCountOut=8;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//申请颜色表缓冲区,生成灰度图像的颜色表
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占字节数,必须是4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//循环变量,图像的坐标
	int i,j;

	//根据灰值化公式为输出图像赋值
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++)
			*(m_pImgDataOut+i*lineByteOut+j)=0.11**(m_pImgData+i*lineByteIn+j*pixelByteIn+0)
		        	+0.59**(m_pImgData+i*lineByteIn+j*pixelByteIn+1)
		        	+0.30**(m_pImgData+i*lineByteIn+j*pixelByteIn+2)+0.5;
	}

}

/**************************************************************************
*函数名
*    Binary()
*
*参数名
*    int threshold   -阈值
*
*返回值
*    无
*
*说明：该函数将m_pImgData作为输入图像，根据给定的阈值threshold对图像进行二值化，
*      结果存入m_pImgDataOut中。若是彩色图像，则先将图像进行灰值图像后再二值化。
***************************************************************************/
void GrayTrans::Binary(int threshold)
{
	//对于灰度图像
	if(m_nBitCount==8){
		//释放旧的输出图像数据及颜色表缓冲区
		if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//输出图像的每像素位数、颜色表长度
		m_nBitCountOut=m_nBitCount;
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		
		//申请输出图像颜色表缓冲区，并将输入图像颜色表拷贝至输出图像颜色表中
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
		//输出图像的宽高,与输入图像相等
		m_imgWidthOut=m_imgWidth;
		m_imgHeightOut=m_imgHeight;
		
		
		//图像每行像素所占字节数，输入图像与输出图像每行像素所占字节数相等
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

		//申请输出图像位图数据缓冲区
		m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

		//循环变量，图像的坐标
		int i,j;

		//二值化
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

		//若是彩色图像,先变成灰度格式，变换后m_pImgDataOut已经申请了内存，
        //并存放了灰度图像数据
		ColorToGray();

		//输出图像每行像素所占字节数，输入图像与输出图像每行像素所占字节数不等
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

		//循环变量，图像的坐标
		int i,j;

		//二值化
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
*函数名
*    GrayToColor()
*
*参数名
*    无
*
*返回值
*    无
*
*说明：该函数将m_pImgData作为输入图像，将灰度图像变成彩色格式，结果存
*      入m_pImgDataOut中。
*      
***************************************************************************/
void GrayTrans::GrayToColor()
{
	//若是彩色格式，则返回
	if(m_nBitCount==24) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像每像素位数为24比特
	m_nBitCountOut=24;

	//彩色图像颜色表长度为0，即无颜色表
	m_nColorTableLengthOut=0;

	//输入的灰度图像每行像素所占字节数，必须为4的倍数
	int lineByteIn=(m_imgWidth+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出的彩色图像每行像素所占字节数，必须为4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//i,j为图像坐标循环变量，k为彩色图像三个通道的循环变量
	int i,j,k;

	//灰度变彩色
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			//用像素灰度值为彩色格式中每个通道赋值，三个通道数值相等
			for(k=0;k<3;k++)
			  *(m_pImgDataOut+i*lineByteOut+j*3+k)=*(m_pImgData+i*lineByteIn+j);
		}
	}
}


/***********************************************************************
* 函数名称：
*   LinearStrech()
*
*函数参数:
*   CPoint point[2]  -分段线性的对应点
*
*返回值: 
*    无
*
*说明：分段线性拉伸函数,给定两个分段点，对m_pImgData所指向缓冲区中的灰度
*      或彩色图像进行线性拉伸，为m_pImgDataOut申请内存，存放拉伸结果。
***********************************************************************/
void GrayTrans::LinearStrech(CPoint point[2])
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像每像素位数与输入图像相同
	m_nBitCountOut=m_nBitCount;

	//计算颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//若有颜色表，输入图像的颜色表与输出图像颜色表相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占的字节数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//如果是灰度图像，则调用单通道数据线性拉伸函数
		LinearStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,point);
	else{
		//若彩色图像，则把每个通道的数据进行分离，并线性拉伸，然后再合成为彩色图像

		//单通道数据每行像素所占的字节数
		int lineByteGray=(m_imgWidth+3)/4*4;

		//临时缓冲区，存放单通道数据
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//临时缓冲区，单通道数据拉伸的结果
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//循环变量，图像坐标
		int i, j;

		//循环变量，遍历彩色图像的三个通道
		int k;

		for(k=0;k<3;k++){
			//将每个通道数据提取出来，存入bufIn缓冲区
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//对bufIn缓冲区中的数据进行拉伸，拉伸结果存入bufOut中
			LinearStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,point);

			//将单通道拉伸的结果存入输出图像m_pImgDataOut对应通道中
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//释放缓冲区
		delete []bufIn;
		delete []bufOut;
	}

}


/***********************************************************************
* 函数名称：
*   LinearStrechForSnglChannel()
*
*函数参数:
*   unsigned char *pImgDataIn   -待拉伸的位图数据指针
*   unsigned char *pImgDataOut  -拉伸后位图数据指针
*   int imgWidth   -位图宽,像素为单位
*   int imgHeight  -位图高,像素为单位
*   CPoint point[2]  -分段线性的两个转折点坐标
*
*返回值: 
*    无
*
*说明：给定一个通道的数据，以及两个转折点坐标，做分段线性拉伸, 将结果
*     存入pImgDataOut所指向的缓冲区。
***********************************************************************/
void GrayTrans::LinearStrechForSnglChannel(unsigned char *pImgDataIn, 
		unsigned char *pImgDataOut, int imgWidth,int imgHeight,CPoint point[2])
{
	//循环变量，图像坐标
	int i,j;

	//每行像素的字节数，单通道图像
	int lineByte=(imgWidth+3)/4*4;

	//(x1,y1)和(x2,y2)为两个分段点的坐标
	double x1=point[0].x,y1=point[0].y,x2=point[1].x, y2=point[1].y;

	//三个分段的直线斜率
	double slop1=y1/x1,slop2=(y2-y1)/(x2-x1),slop3=(255-y2)/(255-x2);

	//三个直线坐标的斜率
	double dis1=0,dis2=y1-slop2*x1,dis3=y2-slop3*x2;

	//映射表
	int map[256];

	//根据三个分段，对输入图像每个灰度级计算映射表
	for(i=0;i<256;i++)	
	{
		if(i<x1)
			map[i]=(int)(slop1*i+dis1+0.5);
		else if(i<x2)
			map[i]=(int)(slop2*i+dis2+0.5);
		else
			map[i]=(int)(slop3*i+dis3+0.5);
	}

	//临时变量
	int tmp;

	//对每个像素，根据输入像素灰度级查找映射表，并为输出像素赋值
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//输入图像(i,j)像素的灰度级
			tmp=*(pImgDataIn+i*lineByte+j);

			//根据映射表为输出图像赋值
			*(pImgDataOut+i*lineByte+j)=map[tmp];
		}
	}
}


/***********************************************************************
* 函数名称：
*   DuiShuStrechForSnglChannel()
*
*函数参数:
*   unsigned char *pImgDataIn   -待拉伸的位图数据指针
*   unsigned char *pImgDataOut  -拉伸后位图数据指针
*   int imgWidth   -位图宽,像素为单位
*   int imgHeight  -位图高,像素为单位
*   float a  -对数函数非线性变换参数
*   float b  -对数函数非线性变换参数
*   float c  -对数函数非线性变换参数
*
*返回值: 
*    无
*
*说明：给定一个通道的数据，及a、b、c三个对数拉伸所需要的参数，进行非线性
*     拉伸, 将结果存入pImgDataOut所指向的缓冲区。
***********************************************************************/
void GrayTrans::DuiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		float a, float b, float c)
{
	//循环变量，图像坐标
	int i,j;

	//每行像素的字节数，单通道图像
	int lineByte=(imgWidth+3)/4*4;

	//临时变量
	float tmp1, tmp2, t;

	//对数函数非线性变换
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//根据输入点像素灰度值，计算输出点像素灰度值
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
* 函数名称：
*   DuiShuStrech()
*
*函数参数:
*   float a  -对数函数非线性变换参数
*   float b  -对数函数非线性变换参数
*   float c  -对数函数非线性变换参数
*
*返回值: 
*    无
*
*说明：对数函数非线性拉伸，给定三个参数，对m_pImgData所指向缓冲区中的灰度
*      或彩色图像进行对数非线性拉伸，为m_pImgDataOut申请内存，存放拉伸结果。
***********************************************************************/
void GrayTrans::DuiShuStrech(float a, float b, float c)
{
		//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像每像素位数与输入图像相同
	m_nBitCountOut=m_nBitCount;

	//计算颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//若有颜色表，输入图像的颜色表与输出图像颜色表相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占的字节数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//如果是灰度图像，则调用单通道数据对数拉伸函数
		DuiShuStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,a,b,c);
	else{
		//若彩色图像，则把每个通道的数据进行分离，并非线性拉伸，然后再合成为彩色图像

		//单通道数据每行像素所占的字节数
		int lineByteGray=(m_imgWidth+3)/4*4;

		//临时缓冲区，存放单通道数据
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//临时缓冲区，单通道数据拉伸的结果
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//循环变量，图像坐标
		int i, j;

		//循环变量，遍历彩色图像的三个通道
		int k;

		for(k=0;k<3;k++){
			//将每个通道数据提取出来，存入bufIn缓冲区
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//对bufIn缓冲区中的数据进行拉伸，拉伸结果存入bufOut中
			DuiShuStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,a,b,c);

			//将单通道拉伸的结果存入输出图像m_pImgDataOut对应通道中
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//释放缓冲区
		delete []bufIn;
		delete []bufOut;
	}
}


/***********************************************************************
* 函数名称：
*   ZhiShuStrechForSnglChannel()
*
*函数参数:
*   unsigned char *pImgDataIn   -待拉伸的位图数据指针
*   unsigned char *pImgDataOut  -拉伸后位图数据指针
*   int imgWidth   -位图宽,像素为单位
*   int imgHeight  -位图高,像素为单位
*   float a  -指数函数非线性变换参数
*   float b  -指数函数非线性变换参数
*   float c  -指数函数非线性变换参数
*
*返回值: 
*    无
*
*说明：给定一个通道的数据，及a、b、c三个指数拉伸所需要的参数，进行非线性
*     拉伸, 将结果存入pImgDataOut所指向的缓冲区。
***********************************************************************/
void GrayTrans::ZhiShuStrechForSnglChannel(unsigned char *pImgDataIn,
		unsigned char *pImgDataOut,int imgWidth,int imgHeight,
		float a, float b, float c)
{
	//循环变量，图像坐标
	int i,j;

	//每行像素的字节数，单通道图像
	int lineByte=(imgWidth+3)/4*4;

	//临时变量
	float t;

	//对数函数非线性变换
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<lineByte;j++)
		{
			//根据输入点像素灰度值，计算输出点像素灰度值
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
* 函数名称：
*   ZhiShuStrech()
*
*函数参数:
*   float a  -指数函数非线性变换参数
*   float b  -指数函数非线性变换参数
*   float c  -指数函数非线性变换参数
*
*返回值: 
*    无
*
*说明：指数函数非线性拉伸分，给定指数拉伸参数，对m_pImgData所指向缓冲区中的灰度
*      或彩色图像进行指数非线性拉伸，为m_pImgDataOut申请内存，存放拉伸结果。
***********************************************************************/
void GrayTrans::ZhiShuStrech(float a, float b, float c)
{
		//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像每像素位数与输入图像相同
	m_nBitCountOut=m_nBitCount;

	//计算颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//若有颜色表，输入图像的颜色表与输出图像颜色表相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占的字节数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];


	if(m_nBitCount==8)//如果是灰度图像，则调用单通道数据指数拉伸函数
		ZhiShuStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		                             m_imgWidth, m_imgHeight,a,b,c);
	else{
		//若彩色图像，则把每个通道的数据进行分离，并非线性拉伸，然后再合成为彩色图像

		//单通道数据每行像素所占的字节数
		int lineByteGray=(m_imgWidth+3)/4*4;

		//临时缓冲区，存放单通道数据
		unsigned char *bufIn=new unsigned char[m_imgHeight*lineByteGray];

		//临时缓冲区，单通道数据拉伸的结果
		unsigned char *bufOut=new unsigned char[m_imgHeight*lineByteGray];
		//循环变量，图像坐标
		int i, j;

		//循环变量，遍历彩色图像的三个通道
		int k;

		for(k=0;k<3;k++){
			//将每个通道数据提取出来，存入bufIn缓冲区
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*lineByteGray+j]=*(m_pImgData+i*lineByteOut+j*3+k);
			}

			//对bufIn缓冲区中的数据进行拉伸，拉伸结果存入bufOut中
			ZhiShuStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,a,b,c);

			//将单通道拉伸的结果存入输出图像m_pImgDataOut对应通道中
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					*(m_pImgDataOut+i*lineByteOut+j*3+k)=bufOut[i*lineByteGray+j];
			}
		}

		//释放缓冲区
		delete []bufIn;
		delete []bufOut;
	}
}