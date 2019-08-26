#include "stdafx.h"
#include "ImgEnhance.h"
#include <math.h>
CImgEnhance::CImgEnhance()
{
	
	m_nColorTableLengthOut=0;
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	
}

CImgEnhance::CImgEnhance(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
}

CImgEnhance::~CImgEnhance()
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

/**********************************************************************
*
* 函数名称：
* AddPepperSaltNoise()
*
* 参数：
* void
*
* 返回值：
* void
*
* 说明：
* 在view中图像添加椒盐噪声
*
**********************************************************************/
void CImgEnhance::AddPepperSaltNoise()
{
	unsigned char* pDIB;
	int bytecount,i;
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	pDIB=m_pImgData;
	if(m_nBitCount==8||m_nBitCount==24)
	{
		bytecount = m_imgWidth * m_imgHeight *m_nBitCount/8; //获取位图数据区的字节数
	}
	else
	{
		AfxMessageBox("只能处理真彩色和8位灰度图像!");
		return ;
	}
	if (m_nBitCount==8)      //处理灰度图像
	{  
        for (i=0;i<bytecount;i++)
        {
			if(rand()>32000)  pDIB[i]=0;
			if(rand()<200)    pDIB[i]=255;
        }
	}
	else	                 //24位真彩色图像
	{
        for (i=0;i<bytecount;i=i+3)
        {
			int num=rand();
            if (num>32000) 
			{
				pDIB[i]=(rand())%255;        //处理每一个像素的RGB值
				pDIB[i+1]=(rand())%255;
				pDIB[i+2]=(rand())%255;
			}
			if (num<200) 
			{
				pDIB[i]=(rand())%255;
				pDIB[i+1]=(rand())%255;
				pDIB[i+2]=(rand())%255;
			}
        }
	}
	//生成新的DIB位图
	m_nBitCountOut=m_nBitCount;
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	int j, pixelByte=m_nBitCountOut/8;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth*pixelByte;j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(pDIB+i*lineByteOut+j);
	}
}

/**********************************************************************
*
* 函数名称：
* AddRandomNoise()
*
* 参数：
* void
*
* 返回值：
* void
*
* 说明：
* 在view中图像添加随机噪声
*
**********************************************************************/
void CImgEnhance::AddRandomNoise()
{
	unsigned char* pDIB;
	int bytecount,i;
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	pDIB=m_pImgData;
	if(m_nBitCount==8)
	{
		bytecount = m_imgWidth * m_imgHeight *m_nBitCount/8; //获取位图数据区的字节数
	}
	else
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    //DIB中添加随机噪声	
    for( i=0;i<bytecount;i++)
	{
		LONG Temp=rand();
		Temp = pDIB[i]*224/256+Temp/512;
		pDIB[i] = Temp >= 255 ? 255 : Temp;
    }
	
	m_nBitCountOut=m_nBitCount;
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	
	int j, pixelByte=m_nBitCountOut/8;
	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth*pixelByte;j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(pDIB+i*lineByteOut+j);
	}
	
}
/**********************************************************************
*
* 函数名称：
* AvgTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef)
*
* 参数：
* int TempH            模板的高度
* int TempW            模板的宽度
* int TempCX           模板的中心元素X坐标 ( < iTempW - 1)
* int TempCY           模板的中心元素Y坐标 ( < iTempH - 1)
* float *fpTempArray   指向模板数组的指针
* float f              模板系数
*
* 返回值：
* void
*
* 说明：
* 该函数用指定的模板(任意大小)来对图像进行操作，参数TempH指定模板
* 的高度，参数TempW指定模板的宽度，参数TempCX和iTempCY指定模板的中心
* 元素坐标，参数fpTempArray指定模板元素，f指定系数。
*
**********************************************************************/
void CImgEnhance::AvgTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float f)
{
	unsigned char*	pSrc;
	unsigned char*	pDst;   
	int	i,j,k,l;
	float	value;
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    //创建要复制的图像区域
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth*m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut = new unsigned char[lineByteOut*m_imgHeight];
	}
	
	int pixelByte = m_nBitCountOut / 8;
	for(i = 0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(m_pImgData+i*lineByteOut+j);
	}
	
	//行处理(去掉边缘几行)
	for (i = TempCY; i < m_imgHeight - TempH + TempCY + 1; i++)
	{
		//列处理(去掉边缘几列)
		for (j = TempCX; j < m_imgWidth - TempW + TempCX + 1; j++)
		{
			//指向新DIB第i行第j列的像素的指针
			pDst = m_pImgDataOut + lineByte * (m_imgHeight - 1 - i) + j;
			
			value=0;
			//计算
			for (k = 0; k < TempH; k++)
			{
				for (l = 0; l < TempW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i 
						+ TempCY - k) + j - TempCX + l;
					//计算加权平均,保存像素值
					value += (*pSrc) * fpTempArray[k * TempW + l];
				}
			}
			//乘以系数
			value*=f;
			//取结果的绝对值
			value=(float)fabs(value);
			if (value > 255)
			{
				*pDst = 255;	
			}
			else
			{
				*pDst = (unsigned char)(value + 0.5);
			}
		}
	}
}
/**********************************************************************
*
* 函数名称：
* FindMedianValue(unsigned char* pbArray,int ArrayLen)
*
* 参数：
* unsigned char* pbArray	指向模板数组首地址的指针
* int ArrayLen             模板数组的长度
*
* 返回值：
* unsigned char
*
* 说明：
* /中值滤波中查找模版中像素中值的算法_利用冒泡排序
*
**********************************************************************/

unsigned char CImgEnhance::FindMedianValue(unsigned char* pbArray,int ArrayLen)
{
	int i,j;
	unsigned char Temp;
	
	for(j =0; j<ArrayLen; j++)
	{
		for(i =0; i <ArrayLen-j-1; i++)
		{
			if(pbArray[i]>pbArray[i+1])
			{
				Temp=pbArray[i];
				pbArray[i]=pbArray[i+1];
				pbArray[i+1]=Temp;
			}
			
		}
	}
	if ((ArrayLen&1)>0)
	{
		Temp=pbArray[(ArrayLen+1)/2];
	}
	else
	{
		Temp=(pbArray[ArrayLen/2]+pbArray[ArrayLen/2+1])/2;
	}
	
	return Temp;
}
/**********************************************************************
*
* 函数名称：
* MedianFilter(int FilterH, int FilterW, int FilterCX, int FilterCY)
*
* 参数：
* int FilterH     模板的高度        
* int FilterW     模板的宽度
* int FilterCX    模板的中心元素X坐标 ( < FilterW - 1)
* int FilterCY    模板的中心元素Y坐标 ( < FilterH - 1)
*
* 返回值：
* void
*
* 说明：
* 中值滤波的算法
*
**********************************************************************/

void CImgEnhance::MedianFilter(int FilterH, int FilterW, int FilterCX, int FilterCY)
{
	unsigned char*	pSrc;
    unsigned char*	pDst;
    int		i,j,k,l;
	unsigned char*	value;         //指向滤波器数组的指针
	
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//计算图像每行的字节数
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
	//分配内存，以保存新图像
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//为处理后的图像分配内存空间
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	int pixelByte = m_nBitCountOut / 8;
	for(i =  0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut + j);
	}
	//暂时分配内存，以保存滤波器数组
	value = new unsigned char[FilterH * FilterW];
	for (i = FilterCY; i < m_imgHeight - FilterH ; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW ; j++)//+ FilterCX + 1
		{
			
			pDst = m_pImgDataOut + lineByte * (m_imgHeight - 1 - i) + j;
			for (k = 0; k < FilterH; k++)
			{
				for (l = 0; l < FilterW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - l - i 
						+ FilterCY - k) + j - FilterCX + l;
					value[k * FilterW + l] = *pSrc;
				}
			}
			*pDst = FindMedianValue(value,FilterW * FilterH);
		}
	}
}

/*************************************************************************
* 函数名称：
*   GradSharp()
* 参数:
*   BYTE  bThre		- 阈值
* 返回值:
*   BOOL            - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行梯度锐化,设定梯度锐化的阈值为30
/************************************************************************/
void CImgEnhance::GradSharp(unsigned char Thre)
{
   	unsigned char*	pSrc;       // 指向源图像的指针
	unsigned char*	pDst; 	
	unsigned char*	pSrc1;
	unsigned char*	pSrc2;	
	LONG	i,j;				// 循环变量
	int	bTemp;
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
	//创建要复制的图像区域
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	
	int pixelByte = m_nBitCountOut / 8;
	for(i = 0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut + j) = *(m_pImgData + i * lineByteOut + j);
	}
	
	for(i = 0; i < m_imgHeight; i++)		// 每行
	{		
		for(j = 0; j < m_imgWidth; j++)		// 每列
		{
			//指向新DIB第i行第j列的像素的指针
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;
			
			// 进行梯度运算
			// 指向DIB第i行，第j个象素的指针
			pSrc  = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) + j;			
			// 指向DIB第i+1行，第j个象素的指针
			pSrc1 = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 2 - i) + j;			
			// 指向DIB第i行，第j+1个象素的指针
			pSrc2 = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) 
				+ j + 1;
			
			bTemp = abs((*pSrc)-(*pSrc1)) + abs((*pSrc)-(*pSrc2));
			
			// 判断是否小于阈值
			if ((bTemp+120) < 255)
			{
				// 判断是否大于阈值，对于小于情况，灰度值不变。
				if (bTemp >= Thre)
				{
					*pSrc = bTemp + 120;
				}
			}
			else
			{
				*pSrc = 255;
			}
			//生成新的DIB像素值
			*pDst = *pSrc;
		}
	}
	
}
/**********************************************************************
*
* 函数名称：
* LapTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float fCoef)
*
* 参数：
* int TempH            模板的高度
* int TempW            模板的宽度
* int TempCX           模板的中心元素X坐标 ( < iTempW - 1)
* int TempCY           模板的中心元素Y坐标 ( < iTempH - 1)
* float *fpTempArray   指向模板数组的指针
* float f              模板系数
*
* 返回值：
* void
*
* 说明：
* 该函数用指定的模板(任意大小)来对图像进行操作，参数TempH指定模板
* 的高度，参数TempW指定模板的宽度，参数TempCX和iTempCY指定模板的中心
* 元素坐标，参数fpTempArray指定模板元素，f指定系数
* 拉普拉斯锐化
**********************************************************************/

void CImgEnhance::LapTemplate(int TempH, int TempW, int TempCX, int TempCY, float *fpTempArray, float f)
{
	unsigned char*	pSrc;
	unsigned char*	pDst;   
	int		i,j,k,l;
	float	value;
	
	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	if(m_nBitCount != 8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    //创建要复制的图像区域
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	
	int pixelByte = m_nBitCountOut / 8;
	for(i = 0; i < m_imgHeight; i++){
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut + j) = *(m_pImgData + i * lineByteOut + j);
	}
	
	//行处理
	for (i = TempCY; i < m_imgHeight - TempH + TempCY + 1; i++)
	{
		//列处理
		for (j = TempCX; j <m_imgWidth - TempW + TempCX + 1; j++)
		{
			//指向新DIB第i行第j列的像素的指针
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;
			
			value=0;
			//计算
			for (k = 0; k < TempH; k++)
			{
				for (l = 0; l < TempW; l++)
				{
					pSrc = m_pImgData + lineByte * (m_imgHeight - 1 - i + TempCY - k)
						+ j - TempCX + l;
					//计算加权平均
					value += (*pSrc) * fpTempArray[k * TempW + l];
				}
			}
			//乘以系数
			value *= f;
			//取结果的绝对值
			value = (float)fabs(value);
			if (value > 255)
			{
				*pDst = 255;	
			}
			else
			{
				*pDst = (unsigned char)(value+0.5);
			}
		}
	}
}

void CImgEnhance::ChooseMask()
{
	if(m_nBitCount!=8)
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return ;
	}
    int n,ji[9],nmin;
	float mean[9],bunsan[9],bmin;
	
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
   	//创建要复制的图像区域
	m_nBitCountOut=m_nBitCount;
    int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	
	memset(m_pImgDataOut,255,lineByteOut * m_imgHeight);
	for(int j=2;j<=m_imgHeight-3;j++)
		for(int i=2;i<=m_imgWidth-3;i++)
		{
			//求9种近邻区域的均值及其方差
			//第1近邻区域
			ji[0]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[1]=m_pImgData[(j-1)*lineByteOut+i];
			ji[2]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[3]=m_pImgData[j*lineByteOut+(i-1)];
			ji[4]=m_pImgData[j*lineByteOut+i];
			ji[5]=m_pImgData[j*lineByteOut+(i+1)];
			ji[6]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			ji[7]=m_pImgData[(j+1)*lineByteOut+i];
			ji[8]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			mean[0]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6]+ji[7]+ji[8])/9;
			bunsan[0]=0;
			for(n=0;n<=8;n++)
				bunsan[0]+=ji[n]*ji[n]-mean[0]*mean[0];
			//第2近邻区域
			ji[0]=m_pImgData[(j-2)*lineByteOut+(i-1)];
			ji[1]=m_pImgData[(j-2)*lineByteOut+i];
			ji[2]=m_pImgData[(j-2)*lineByteOut+(i+1)];
			ji[3]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[4]=m_pImgData[(j-1)*lineByteOut+i];
			ji[5]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[6]=m_pImgData[j*lineByteOut+i];
			mean[1]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[1]=0;
			for(n=0;n<=6;n++)
				bunsan[1]+=ji[n]*ji[n]-mean[1]*mean[1];
			//第3近邻区域
			ji[0]=m_pImgData[(j-1)*lineByteOut+(i-2)];
			ji[1]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[2]=m_pImgData[j*lineByteOut+(i-2)];
			ji[3]=m_pImgData[j*lineByteOut+(i-1)];
			ji[4]=m_pImgData[j*lineByteOut+i];
			ji[5]=m_pImgData[(j+1)*lineByteOut+(i-2)];
			ji[6]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			mean[2]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[2]=0;
			for(n=0;n<=6;n++)
				bunsan[2]+=ji[n]*ji[n]-mean[2]*mean[2];
			//第4近邻区域
			ji[0]=m_pImgData[j*lineByteOut+i];
			ji[1]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			ji[2]=m_pImgData[(j+1)*lineByteOut+i];
			ji[3]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			ji[4]=m_pImgData[(j+2)*lineByteOut+(i-1)];
			ji[5]=m_pImgData[(j+2)*lineByteOut+i];
			ji[6]=m_pImgData[(j+2)*lineByteOut+(i+1)];
			mean[3]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[3]=0;
			for(n=0;n<=6;n++)
				bunsan[3]+=ji[n]*ji[n]-mean[3]*mean[3];
			//第5近邻区域
			ji[0]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[1]=m_pImgData[(j-1)*lineByteOut+(i+2)];
			ji[2]=m_pImgData[j*lineByteOut+i];
			ji[3]=m_pImgData[j*lineByteOut+(i+1)];
			ji[4]=m_pImgData[j*lineByteOut+(i+2)];
			ji[5]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			ji[6]=m_pImgData[(j+1)*lineByteOut+(i+2)]; 
			mean[4]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[4]=0;
			for(n=0;n<=6;n++)
				bunsan[4]+=ji[n]*ji[n]-mean[4]*mean[4];	
			//第6近邻区域
			ji[0]=m_pImgData[(j-2)*lineByteOut+(i+1)];
			ji[1]=m_pImgData[(j-2)*lineByteOut+(i+2)];
			ji[2]=m_pImgData[(j-1)*lineByteOut+i];
			ji[3]=m_pImgData[(j-1)*lineByteOut+(i+1)];
			ji[4]=m_pImgData[(j-1)*lineByteOut+(i+2)];
			ji[5]=m_pImgData[j*lineByteOut+i];
			ji[6]=m_pImgData[j*lineByteOut+(i+1)]; 
			mean[5]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[5]=0;
			for(n=0;n<=6;n++)
				bunsan[5]+=ji[n]*ji[n]-mean[5]*mean[5];
			//第7近邻区域
			ji[0]=m_pImgData[(j-2)*lineByteOut+(i-2)];
			ji[1]=m_pImgData[(j-2)*lineByteOut+(i-1)];
			ji[2]=m_pImgData[(j-1)*lineByteOut+(i-2)];
			ji[3]=m_pImgData[(j-1)*lineByteOut+(i-1)];
			ji[4]=m_pImgData[(j-1)*lineByteOut+i];
			ji[5]=m_pImgData[j*lineByteOut+(i-1)];
			ji[6]=m_pImgData[j*lineByteOut+i];
			mean[6]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[6]=0;
			for(n=0;n<=6;n++)
				bunsan[6]+=ji[n]*ji[n]-mean[6]*mean[6];
			//第8近邻区域
			ji[0]=m_pImgData[j*lineByteOut+(i-1)];
			ji[1]=m_pImgData[j*lineByteOut+i];
			ji[2]=m_pImgData[(j+1)*lineByteOut+(i-2)];
			ji[3]=m_pImgData[(j+1)*lineByteOut+(i-1)];
			ji[4]=m_pImgData[(j+1)*lineByteOut+i];
			ji[5]=m_pImgData[(j+2)*lineByteOut+(i-2)];
			ji[6]=m_pImgData[(j+2)*lineByteOut+(i-1)];
			mean[7]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[7]=0;
			for(n=0;n<=6;n++)
				bunsan[7]+=ji[n]*ji[n]-mean[7]*mean[7];
			//第9近邻区域
			ji[0]=m_pImgData[j*lineByteOut+i];
			ji[1]=m_pImgData[j*lineByteOut+(i+1)];
			ji[2]=m_pImgData[(j+1)*lineByteOut+i];
			ji[3]=m_pImgData[(j+1)*lineByteOut+(i+1)];
			ji[4]=m_pImgData[(j+1)*lineByteOut+(i+2)];
			ji[5]=m_pImgData[(j+2)*lineByteOut+(i+1)];
			ji[6]=m_pImgData[(j+2)*lineByteOut+(i+2)];
			mean[8]=(float)(ji[0]+ji[1]+ji[2]+ji[3]+ji[4]+ji[5]+ji[6])/7;
			bunsan[8]=0;
			for(n=0;n<=6;n++)
				bunsan[8]+=ji[n]*ji[n]-mean[8]*mean[8];
			//求方差最小的近邻区域nmin
			bmin=bunsan[0];
			nmin=0;
			for(n=0;n<=8;n++)
			{
				if(bmin>bunsan[n])
				{
					bmin=bunsan[n];
					nmin=n;
				}
				//把nmin的值四舍五入后作为显示图像的值
				m_pImgDataOut[j*lineByteOut+i]=(int)(mean[nmin]+0.5);
			}				 
		}
}

void CImgEnhance::SNR(unsigned char* m_pSrcImgData)
{
   int c=10;
   int r1=3;
   int r2=10;
  float a=0.05;
   float b=0.1;


	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}


	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	unsigned char *y_pImgData=new unsigned char[lineByte*m_imgHeight];
	memcpy(y_pImgData,m_pImgData,(m_imgHeight*lineByte));

    unsigned float *n_grad=new unsigned float[m_imgWidth*m_imgHeight];
	memset(n_grad,0,(m_imgWidth*m_imgHeight)*sizeof(unsigned float));

	float *m_grad=new float[m_imgWidth*m_imgHeight];
	memset(m_grad,0,(m_imgWidth*m_imgHeight)*sizeof(float));

	//循环变量，图像的坐标
	int i,j;
	int k;
	//中间变量
	double temp;
	float temp1,temp2,temp3,temp4,temp5;
	int temp6;

	for(i=0;i<m_imgHeight-1;i++){
		for(j=0;j<m_imgWidth-1;j++){
			//原图像第i行第j列像素的梯度值  采用绝对差算法

         temp=sqrt((*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))*
					(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
				   +(*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))*
				    (*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j)));
                double h=0;
          h=sqrt((*(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j))*
					(*(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j))
				   +(*(m_pSrcImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j))*
				    (*(m_pSrcImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j)));
		
	          *(n_grad+(m_imgHeight-i-1)*lineByte+j)= *(m_pSrcImgData+(m_imgHeight-i-1)*lineByte+j)/temp;

		}
	}


	for(k=0;k<5;k++){
    for(i=0;i<m_imgHeight-1;i++){
		for(j=0;j<m_imgWidth-1;j++){
			//原图像第i行第j列像素的梯度值  采用绝对差算法
			temp=abs(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
				+abs(*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j));
			//m_grad的第i行第j列像素
			*(m_grad+(m_imgHeight-i-1)*lineByte+j)=(float)1/(temp+1);  
			//n_grad的第i行第j列像素					
		}
	}

	for(i=1;i<m_imgHeight-2;i++){
		for(j=1;j<m_imgWidth-2;j++){

			temp1=a * *(m_grad+(m_imgHeight-i-1)*lineByte+j)*( *(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(m_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(m_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(m_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(m_pImgData+(m_imgHeight-i-1)*lineByte+j));

 			temp2=a* ((*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
 				 *(*(m_grad+(m_imgHeight-i-1)*lineByte+j+1)-*(m_grad+(m_imgHeight-i-1)*lineByte+j))
 				 + (*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
 				 * (*(m_grad+(m_imgHeight-i-2)*lineByte+j)-*(m_grad+(m_imgHeight-i-1)*lineByte+j)));
			
			temp3=2*b*(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(m_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(m_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(m_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(m_pImgData+(m_imgHeight-i-1)*lineByte+j));

			temp4=2*b* *(n_grad+(m_imgHeight-i-1)*lineByte+j)*( *(y_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(y_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(y_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(y_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(y_pImgData+(m_imgHeight-i-1)*lineByte+j));

			temp5=2*b* ((*(y_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(y_pImgData+(m_imgHeight-i-1)*lineByte+j))
				 *(*(n_grad+(m_imgHeight-i-1)*lineByte+j+1)-*(n_grad+(m_imgHeight-i-1)*lineByte+j))
				 + (*(y_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(y_pImgData+(m_imgHeight-i-1)*lineByte+j))
				 * (*(n_grad+(m_imgHeight-i-2)*lineByte+j)-*(n_grad+(m_imgHeight-i-1)*lineByte+j)));

			temp6= (int)(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j)+temp1+temp2+temp3-temp4-temp5);

			if(temp6>255)
				*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=255;
			else if(temp6<0)
				*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=0;
			else
				*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=temp6;
			
		}
	}
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++){
				*(m_pImgData+(m_imgHeight-i-1)*lineByte+j)=*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j);
			}
		}
	}
	delete[] m_grad;
	delete[] n_grad;
	delete[] y_pImgData;
	

}

CString CImgEnhance::PSNR(unsigned char* m_pSrcImgData)
{
    int byteCount;                    // DIB位图的字节数
    int i;
	//计算图像峰值信噪比
	double  temp1=0.0;
	double  temp2=0.0;
	//处理8位灰度图像
	if (m_nBitCount==8)
	{
		//计算整幅DIB位图的字节数
		byteCount =m_imgHeight*m_imgWidth*m_nBitCount/8; 
		for( i=0; i<byteCount; i++)
		{
			temp1=temp1+(double)m_pImgData[i]*m_pImgData[i];
			temp2=temp2+(double)(m_pImgData[i]-m_pSrcImgData[i])*(m_pImgData[i]-m_pSrcImgData[i]);
		}
	}
	else
	{
		AfxMessageBox("只能处理8位灰度图像!");
		return "ERROR";
	}
	double   PSNR=m_imgHeight*m_imgWidth/temp2;
	PSNR=10*log10(255*255*PSNR);       //PSNR即为峰值信噪比
	CString StrPSNR;
    StrPSNR.Format("%f",PSNR);
    return StrPSNR;
}

void CImgEnhance::TvGrad(int c,int r1,int r2,float a,float b)
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}


	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];
	unsigned char *y_pImgData=new unsigned char[lineByte*m_imgHeight];
	memcpy(y_pImgData,m_pImgData,(m_imgHeight*lineByte));

    unsigned float *n_grad=new unsigned float[m_imgWidth*m_imgHeight];
	memset(n_grad,0,(m_imgWidth*m_imgHeight)*sizeof(unsigned float));

	float *m_grad=new float[m_imgWidth*m_imgHeight];
	memset(m_grad,0,(m_imgWidth*m_imgHeight)*sizeof(float));

	//循环变量，图像的坐标
	int i,j;
	int k;
	//中间变量
	int temp;
	float temp1,temp2,temp3,temp4,temp5;
	int temp6;

	for(i=0;i<m_imgHeight-1;i++){
		for(j=0;j<m_imgWidth-1;j++){
			//原图像第i行第j列像素的梯度值  采用绝对差算法
			temp=abs(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
				+abs(*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j));
			//n_grad的第i行第j列像素
	//	if(temp<c)
	//			*(n_grad+(m_imgHeight-i-1)*lineByte+j)=0;
	//		else
	//			*(n_grad+(m_imgHeight-i-1)*lineByte+j)=1+5*exp(-temp/r2);
	          *(n_grad+(m_imgHeight-i-1)*lineByte+j)=temp;

		}
	}


	for(k=0;k<10;k++){
    for(i=0;i<m_imgHeight-1;i++){
		for(j=0;j<m_imgWidth-1;j++){
			//原图像第i行第j列像素的梯度值  采用绝对差算法
			temp=abs(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
				+abs(*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j));
			//m_grad的第i行第j列像素
			*(m_grad+(m_imgHeight-i-1)*lineByte+j)=(float)1/(temp+1);  
			//n_grad的第i行第j列像素					
		}
	}

	for(i=1;i<m_imgHeight-2;i++){
		for(j=1;j<m_imgWidth-2;j++){

			temp1=a * *(m_grad+(m_imgHeight-i-1)*lineByte+j)*( *(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(m_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(m_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(m_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(m_pImgData+(m_imgHeight-i-1)*lineByte+j));

 			temp2=a* ((*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
 				 *(*(m_grad+(m_imgHeight-i-1)*lineByte+j+1)-*(m_grad+(m_imgHeight-i-1)*lineByte+j))
 				 + (*(m_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(m_pImgData+(m_imgHeight-i-1)*lineByte+j))
 				 * (*(m_grad+(m_imgHeight-i-2)*lineByte+j)-*(m_grad+(m_imgHeight-i-1)*lineByte+j)));
			
			temp3=2*b*(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(m_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(m_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(m_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(m_pImgData+(m_imgHeight-i-1)*lineByte+j));

			temp4=2*b* *(n_grad+(m_imgHeight-i-1)*lineByte+j)*( *(y_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(y_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(y_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(y_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(y_pImgData+(m_imgHeight-i-1)*lineByte+j));

			temp5=2*b* ((*(y_pImgData+(m_imgHeight-i-1)*lineByte+j+1)-*(y_pImgData+(m_imgHeight-i-1)*lineByte+j))
				 *(*(n_grad+(m_imgHeight-i-1)*lineByte+j+1)-*(n_grad+(m_imgHeight-i-1)*lineByte+j))
				 + (*(y_pImgData+(m_imgHeight-i-2)*lineByte+j)-*(y_pImgData+(m_imgHeight-i-1)*lineByte+j))
				 * (*(n_grad+(m_imgHeight-i-2)*lineByte+j)-*(n_grad+(m_imgHeight-i-1)*lineByte+j)));

			temp6= (int)(*(m_pImgData+(m_imgHeight-i-1)*lineByte+j)+temp1+temp2+temp3-temp4-temp5);

			if(temp6>255)
				*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=255;
			else if(temp6<0)
				*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=0;
			else
				*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=temp6;
			
		}
	}
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++){
				*(m_pImgData+(m_imgHeight-i-1)*lineByte+j)=*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j);
			}
		}
	}
	delete[] m_grad;
	delete[] n_grad;
	delete[] y_pImgData;
	
}

void CImgEnhance::Testdiejia()
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}


	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

    unsigned float *n_grad=new unsigned float[m_imgWidth*m_imgHeight];
	memset(n_grad,0,(m_imgWidth*m_imgHeight)*sizeof(unsigned float));


	//循环变量，图像的坐标
	int i,j;
	int k;
	//中间变量

	float temp1,temp2;


   for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			*(n_grad+(m_imgHeight-i-1)*lineByte+j)=2*( *(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
				 + *(m_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(m_pImgData+(m_imgHeight-i-2)*lineByte+j) 
				 + *(m_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(m_pImgData+(m_imgHeight-i-1)*lineByte+j));
		}
	} 
	
	for(k=0;k<1500;k++){
		for(i=1;i<m_imgHeight-1;i++){
			for(j=1;j<m_imgWidth-1;j++){
				
				temp1=0.25*( *(m_pImgData+(m_imgHeight-i-1)*lineByte+j+1)
					+ *(m_pImgData+(m_imgHeight-i-1)*lineByte+j-1)+ *(m_pImgData+(m_imgHeight-i-2)*lineByte+j) 
					+ *(m_pImgData+(m_imgHeight-i)*lineByte+j)- 4* *(m_pImgData+(m_imgHeight-i-1)*lineByte+j));

				temp2= *(m_pImgData+(m_imgHeight-i-1)*lineByte+j)+temp1-0.25* *(n_grad+(m_imgHeight-i-1)*lineByte+j);

				if(temp2>255)
					*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=255;
				else
					*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j)=temp2;
			
			}
		}
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++){
				*(m_pImgData+(m_imgHeight-i-1)*lineByte+j)=*(m_pImgDataOut+(m_imgHeight-i-1)*lineByte+j);
			}
		}
	}
	delete[] n_grad;	
}