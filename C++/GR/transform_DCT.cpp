#include "stdafx.h"
#include "transform_DCT.h"
#include "math.h"

transform_DCT::transform_DCT()
{
	pDctBuff = NULL;
}

transform_DCT::transform_DCT(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
Transform_FFT(size, nBitCount, lpColorTable, pImgData)
{
	pDctBuff = NULL;
}

transform_DCT::~transform_DCT()
{
	if (pDctBuff != NULL)
	{
		delete[] pDctBuff;
		pDctBuff = NULL;
	}
}
/**********************************************************************
 *
 * �������ƣ�
 * DCT()
 *
 * ������
 * double *f ʱ������ָ��
 * double *F Ƶ������ָ��
 * int r �߶�
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ���ÿ��ٸ���Ҷ�任ʵ�ֿ�����ɢ���ұ任
 *
 **********************************************************************/
void transform_DCT::DCT(double *f, double *F, int r)
{
	
	int	count;			// ��ɢ���ұ任����	
	int		i;				// ѭ������	
	double	dTemp;	
	
	count = 1<<r;			// ������ɢ���ұ任����	
	ComplexNumber *array = new ComplexNumber[count*2];	
	memset(array, 0, sizeof(ComplexNumber) * count * 2);	// ����ֵΪ0
	
	// ��ʱ���д������array
	for(i=0;i<count;i++)
	{
		array[i].real = (float)f[i];
		array[i].imag = 0;
	}

	FFT1D(array, count * 2);// ���ÿ��ٸ���Ҷ�任		
	
	dTemp = 1 / sqrt(count);	// ����ϵ��		
	F[0] = array[0].real * dTemp;	// ��F[0]	
	dTemp *= sqrt(2);
	
	// ��F[u]	
	for(i = 1; i < count; i++)
	{
		F[i] = (array[i].real * cos(i*PI/(count*2)) + array[i].imag * 
			sin(i*PI/(count*2))) * dTemp;
	}
	
	delete[] array;
}
/**********************************************************************
 *
 * �������ƣ�
 * ImgDCT()
 *
 * ������
 * ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ʵ��ͼ�����ɢ���ұ任
 *
 **********************************************************************/
void transform_DCT::ImgDCT()
{
    unsigned char*	pSrc;	// ָ��Դͼ���ָ��	
	unsigned char*  pDst;
	int	i,j;				// ѭ������
	double	dTemp;

	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//����ͼ��ÿ�е��ֽ���
   	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	if(m_nBitCount!=8)
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		return ;
	}
	//�����ڴ棬�Ա�����ͼ��
	m_nBitCountOut=m_nBitCount;
    int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	int pixelByte=m_nBitCountOut/8;
	for(i=0; i<m_imgHeight; i++){
		for(j=0; j<m_imgWidth*pixelByte; j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(m_pImgData+i*lineByteOut+j);
	}
		
	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w * 2 <= m_imgWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= m_imgHeight)
	{
		h *= 2;
		hp++;
	}
	
	double *f = new double[w * h];
	pDctBuff = new double[w * h];
	double *F = pDctBuff;
	
	
	for(i = 0; i < h; i++)			// ��
	{		
		for(j = 0; j < w; j++)		// ��
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			pSrc = (unsigned char*)m_pImgData + lineByte * (m_imgHeight - 1 - i) + j;
			
			// ��ʱ��ֵ
			f[j + i * w] = *(pSrc);
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// ��y���������ɢ���ұ任
		DCT(&f[w * i], &F[w * i], wp);
	}
	
	// ���������
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			f[j * h + i] = F[j + w * i];
		}
	}
	
	for(j = 0; j < w; j++)
	{
		// ��x���������ɢ���ұ任
		DCT(&f[j * h], &F[j * h], hp);
	}	
	
	for(i = 0; i < h; i++)			// ��
	{
		for(j = 0; j < w; j++)		// ��
		{
			//ָ����DIB��i�е�j�е����ص�ָ��
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;

			dTemp = fabs(F[j*h+i]);	// ����Ƶ��
			
			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				dTemp = 255;
			}
			// ������DIBλͼ������ֵ
			*pDst=(BYTE)(dTemp);
		}
	}
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	delete[] f;
}

/*************************************************************************
* �������ƣ�
*   IDCT()
* ����:
*   double * F	- ָ��Ƶ��ֵ��ָ��
*   double * f	- ָ��ʱ��ֵ��ָ��
*   r��2������
* ����ֵ:
*   �ޡ�
* ˵��:
*   �ú�������ʵ�ֿ�����ɢ���ҷ��任���ú���Ҳ����2N��Ŀ��ٸ���Ҷ�任
* ��ʵ����ɢ���ҷ��任��
************************************************************************/
void transform_DCT::IDCT(double *F, double *f, int r)
{
	LONG	count;			// ��ɢ���ҷ��任����
	int		i;				// ѭ������
	double	dTemp, d0;	

	count = 1<<r;			// ������ɢ���ұ任����
	ComplexNumber *array = new ComplexNumber[count*2];
	memset(array, 0, sizeof(ComplexNumber) * count * 2);	// ����ֵΪ0
	
	// ��Ƶ��任���д������array
	for(i=0;i<count;i++)
	{
		array[i].real = (float)(F[i] * cos(i*PI/(count*2)));
		array[i].imag = (float)(F[i] * sin(i*PI/(count*2)));
	}
	
	IFFT1D(array, count * 2);// ���ÿ��ٸ���Ҷ���任	
	
	// ����ϵ��
	dTemp = sqrt(2.0/count);
	d0 = (sqrt(1.0/count) - dTemp) * F[0];
	
	for(i = 0; i < count; i++)
	{
		f[i] = d0 + array[i].real * dTemp * 2 * count;
	}
	
	delete[] array;
}

/**********************************************************************
 *
 * �������ƣ�
 * ImgIDCT()
 *
 * ������
 * ��
 *
 * ����ֵ��
 * void
 *
 * ˵����
 * ʵ��ͼ�����ɢ���ҷ��任
 *
 **********************************************************************/
void transform_DCT::ImgIDCT()
{
	if (pDctBuff == NULL)
	{
		AfxMessageBox("���Ƚ���DCT�任��");
		return;
	}	
	unsigned char*  pDst;	// ָ��ʱ��ͼ���ָ��	
	int	i,j;	// ѭ������
	double	dTemp;

	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//����ͼ��ÿ�е��ֽ���
   	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�����ڴ棬�Ա�����ͼ��
	m_nBitCountOut=m_nBitCount;
    int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	if (!m_pImgDataOut)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	}
	int pixelByte=m_nBitCountOut/8;
	for(i=0; i<m_imgHeight; i++){
		for(j=0; j<m_imgWidth*pixelByte; j++)
			*(m_pImgDataOut+i*lineByteOut+j)= *(m_pImgData+i*lineByteOut+j);
	}
	
	double *f = new double[w * h];
	double *F = pDctBuff;
	
	for(i = 0; i < h; i++)
	{
		// ��y���������ɢ���ұ任
		IDCT(&F[w * i], &f[w * i], wp);
	}
	
	// ���������
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			F[j * h + i] = f[j + w * i];
		}
	}
	
	for(j = 0; j < w; j++)
	{
		// ��x���������ɢ���ұ任
		IDCT(&F[j * h], &f[j * h], hp);
	}	
	
	for(i = 0; i < h; i++)			// ��
	{
		for(j = 0; j < w; j++)		// ��
		{
			//ָ����DIB��i�е�j�е����ص�ָ��
			pDst = m_pImgDataOut + lineByte * (m_imgHeight -1 - i) + j;

			dTemp = fabs(f[i*h+j]);	// ����Ƶ��,ת�ò���
			
			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				dTemp = 255;
			}
			// ������DIBλͼ������ֵ
			*pDst=(BYTE)(dTemp);
		}
	}
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	delete[] f;
}