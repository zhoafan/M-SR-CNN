#define		BACK_ALL_NUM	100				//���㱳������֡��
#define		BACK_SEQ_NUM	3				//���㱳��ѭ����Ŀ
#define		THRESHOLD			20              //���ò����ֵ
class CStaticDetect  
{
public:
	CStaticDetect(int nwidth, int nheight);
	virtual ~CStaticDetect();
	void ReceiveFrame(int index, BYTE* sBuf);	//�����ú���
public:
	BYTE *m_pCurImage;		 //��ǰͼ��
	BYTE *m_pDetectImage;    //�����ͼ��
	
	int m_nFrame, count;
	int WIDTH, HEIGHT,IMAGESIZE;
	short int *m_pBackHistgram;		//������ɫͳ��
	BYTE *m_pGrayImage;               //��ǰ�Ҷ�ͼ��
	BYTE *m_pBackground;	             //����
	BYTE *m_pDiffImage;          //���ͼ��
	UINT *m_pTemplate;           //��ȡ��֡����

private:
	void RGBToGray(BYTE *sRGB,BYTE *sGray,int nWidth, int nHeight);
	void DiffImage(BYTE *sGray,BYTE *pGray,int nWidth, int nHeight,int nThreshold);
	void GetMultiData(BYTE *sGray,int nWidth, int nHeight);
	void GetBgImage();

};
