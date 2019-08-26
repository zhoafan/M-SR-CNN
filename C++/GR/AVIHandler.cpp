#include "StdAfx.h"
#include "AVIHandler.h"
#include <vfw.h>

#pragma comment(lib,"vfw32.lib")


AVIHandler::AVIHandler(void):gray_value(NULL),gray_value_square(NULL)
{
	this->loaded = FALSE;
}

AVIHandler::AVIHandler(CString filename):gray_value(NULL),gray_value_square(NULL)
{
	this ->loaded = FALSE;
	LoadVideo(filename);
}
AVIHandler::~AVIHandler(void)
{
	if(gray_value != NULL) free(gray_value);
	if(gray_value_square != NULL) free(gray_value_square);
}



//������Ƶ����
BOOL AVIHandler::LoadVideo(CString filename)
{
		//��ȡAVI�ļ���Ϣ
	HRESULT result;
	AVIFileInit(); //ʹ��AVI�庯����ȡAVI�ļ�ǰӦ���ȵ���
	
	PAVIFILE pfile;//AVI�ļ�ָ��
	AVIFILEINFO pfi;//AVI�ļ���Ϣ
	PAVISTREAM pavi;//AVI��
	AVISTREAMINFO stinfo;//AVI����Ϣ
	
	//CString file = "d:\plane.avi";
	//��AVI�ļ�
	result=AVIFileOpen(&pfile,filename,OF_READ,NULL);
	//AVI�ļ���Ϣ
	result=AVIFileInfo(pfile,&pfi,sizeof(AVIFILEINFO));
	//���AVI��Ƶ��
	result=AVIFileGetStream(pfile,&pavi,streamtypeVIDEO, 
		0);
	//���AVI��Ƶ����Ϣ
	result=AVIStreamInfo(pavi,&stinfo,sizeof(AVISTREAMINFO));
	//���AVI��
	pgf = AVIStreamGetFrameOpen(pavi,NULL);
 
    if (pgf==NULL)
    {
		//���ݳ���
		return FALSE;
    }

	HRESULT hr;

	//�������Ŀ�͸�
	int cx=pfi.dwWidth;//ͼ�����
	int cy=pfi.dwHeight;
	
	width=cx;
	height=cy;
	length=pfi.dwLength;

	
	//ʹ��Ĭ�ϳ��ȣ�ֻ����100֡
	//length=50;

	//AVI���ʽ��λͼHeader,λͼ�����ʽ��
	lpbm = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf,1);
	
	defaultPad=3;

	//���仺���ڴ�ռ�
	//����仺��
	gray_value = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
	//��仺��
	gray_value_square = (unsigned char*)malloc(sizeof(unsigned char)*(width+defaultPad)*(height+defaultPad)*3);

	this->loaded = TRUE;

	return TRUE;
}

//���һ��Frame
unsigned char* AVIHandler::GetSingleFrame(int i)
{
	return (unsigned char*)AVIStreamGetFrame(pgf,i)+40;
	//return NULL;
}

int AVIHandler::GetFrameCount(void)
{
	return this->length;
}


int AVIHandler::GetFrameWidth(void)
{
	return this->width;
	//return 0;
}


int AVIHandler::GetFrameHeight(void)
{
	return this->height;
	//return 0;
}

//����ͼ���BMP��ʽ����
LPBITMAPINFOHEADER AVIHandler::GetLPBM(void)
{
	return this->lpbm;
}

BOOL AVIHandler::isLoaded(void)
{
	return this->loaded;
}
