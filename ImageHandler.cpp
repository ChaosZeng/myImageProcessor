#include "stdafx.h"
#include "Mainfrm.h"


int GetAsh(BYTE** imageBuf, int x, int y)
{
	int clr;
	clr = (imageBuf[y][x * 4] + imageBuf[y][x * 4 + 1]
		+ imageBuf[y][x * 4 + 2]) / 3;
	return clr;
}

/********************************************************
* �����δ洢������ת��Ϊ��ά������ʽ
* ����: image ���δ洢�����أ� width,height ͼ��ĳ���
********************************************************/
BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height, int bt = 4)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*)*(height));
	for (int y = 0; y<height; y++)
	{
		//ʹimageBuf��ÿ��ָ��ֱ�ָ�����±��ʾ���е����׵�ַ
		imageBuf[y] = image + y*width*bt;
	}
	return imageBuf;
}

/**************************************************
* ����: �趨ָ��λ�õ����ػҶ�
* ����: imageBufΪĿ��ͼ�� x,yΪҪ�趨���ص�����
**************************************************/
void SetPixelXY(BYTE** imageBuf1, int x, int y, int a)
{
	imageBuf1[y][x * 4] = a;
	imageBuf1[y][x * 4 + 1] = a;
	imageBuf1[y][x * 4 + 2] = a;
	imageBuf1[y][x * 4 + 3] = 255;
}

void SetPixel(BYTE* image1, unsigned int n, int a)
{
	image1[n] = a;
	image1[n + 1] = a;
	image1[n + 2] = a;
	image1[n + 3] = 255;
}

/**************************************************
* ����: ʹ��ģ��Բ�ɫͼ�����������
* ����: imageBufΪĿ��ͼ�� w��hΪͼ���С
*       templtΪģ�� twΪ�����С
*		x,yΪҪȡ�����ص�����
*       cnΪ��ɫ������� 0Ϊ��ɫ 1Ϊ��ɫ 2Ϊ��ɫ
**************************************************/
int TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)
{
	int i, j;                        //ѭ������
	int m = 0;                      //������ż�Ȩ��
	int px, py;
	//���ζ�������ÿ�����ؽ�������
	for (i = 0; i<tw; i++)
	{
		for (j = 0; j<tw; j++)
		{
			//�����Ӧģ����λ�õ�������ԭͼ���е�λ��
			py = y - tw / 2 + i;
			px = x - tw / 2 + j;
			//��Ȩ���
			m += imageBuf0[py][px * 4 + cn] * templt[i*tw + j];
		}
	}
	return m;                     //���ؽ��
}

/*****************************************************************
* ����: ʹ��ģ��ԻҶ�ͼ�����������
* ����: imageBufΪĿ��ͼ�� w��hΪͼ���С
*       templtΪģ�� twΪ�����С
*		x,yΪҪȡ�����ص�����
******************************************************************/
int TempltExcuteAsh(BYTE** imageBuf0, int w, int h,
	int* templt, int tw, int x, int y)
{
	int i, j;                        //ѭ������
	int m = 0;                      //������ż�Ȩ��
	int px, py;
	//���ζ�������ÿ�����ؽ�������
	for (i = 0; i<tw; i++)
	{
		for (j = 0; j<tw; j++)
		{
			//�����Ӧģ����λ�õ�������ԭͼ���е�λ��
			py = y - tw / 2 + i;
			px = x - tw / 2 + j;
			//��Ȩ���
			m += GetAsh(imageBuf0, px, py) * templt[i*tw + j];
		}
	}
	return m;                     //���ؽ��
}

/******************************************************************
* ����: �Ҷ�ͼ��ļ�ƽ������
* ����: image0Ϊԭͼ�Σ�image1Ϊƽ�������
*		w��hΪͼ��Ŀ�͸�
*       sizeΪ����ƽ��������߳�
******************************************************************/
void SmoothAsh(BYTE* image0, BYTE* image1,
	unsigned int w, unsigned int h, unsigned int size)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//����ģ��
	int* templt;
	int x, y;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];
	for (x = 0; x<size*size; x++)
	{
		templt[x] = 1;
	}
	//�趨˥������
	scale = size*size;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for (y = size / 2; y<h - size / 2; y++)
	{
		for (x = size / 2; x<w - size / 2; x++)
		{
			a = TempltExcuteAsh(imageBuf0, w, h, templt, size, x, y);
			a /= scale;
			//���޴���
			a = a>255 ? 255 : a;
			a = a<0 ? 0 : a;
			SetPixelXY(imageBuf1, x, y, a);
		}
	}

	//�����ڴ�
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* ����: ��ɫͼ��ļ�ƽ������
* ����: image0Ϊԭͼ�Σ�image1ƽ�������
*		w��hΪͼ��Ŀ�͸�
*       sizeΪ����ƽ��������߳�
******************************************************************/
void SmoothCl(BYTE* image0, BYTE* image1,
	unsigned int w, unsigned int h, unsigned int size)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//����ģ��
	int* templt;
	int x, y, c;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];
	for (x = 0; x<size*size; x++)
	{
		templt[x] = 1;
	}
	//�趨˥������
	scale = size*size;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for (y = size / 2; y<h - size / 2; y++)
	{
		for (x = size / 2; x<w - size / 2; x++)
		{
			//���ζԲ�ɫͼ������������ҶȽ��д���
			for (c = 0; c<3; c++)
			{
				a = TempltExcuteCl(imageBuf0, w, h, templt, size, x, y, c);
				a /= scale;
				//���޴���
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}
		}
	}

	//�����ڴ�
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* ����: ��ɫͼ��ĸ�˹ƽ������
* ����: image0Ϊԭͼ�Σ�image1ƽ�������
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void SmoothGaussCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int templt[9] = { 1,2,1,2,4,2,1,2,1 };
	//int templt[9] = { 1,4,1,4,8,4,1,4,1 };
	int x, y, c;
	int a;
	int scale;

	//�趨˥������
	scale = 16;
	//scale = 28;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for (y = 1; y<h - 1; y++)
		for (x = 1; x<w - 1; x++)
			for (c = 0; c<3; c++)
			{
				//���ø�˹ģ���������д���
				a = TempltExcuteCl(imageBuf0, w, h, templt, 3, x, y, c);
				a /= scale;
				//���޴���
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}

	//�����ڴ�
	free(imageBuf0);
	free(imageBuf1);
}

void CircleSmoothCl(BYTE** imageBuf0, BYTE** imageBuf1,
	unsigned int w, unsigned int h, unsigned int size)
{
	//����ģ��
	int* templt;
	int x, y, c;
	int a;
	int scale;

	//���������С�趨ģ��
	templt = new int[size * size];
	for (x = 0; x<size*size; x++)
	{
		templt[x] = 1;
	}
	//�趨˥������
	scale = size*size;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for (y = size / 2; y<h - size / 2; y++)
	{
		for (x = size / 2; x<w - size / 2; x++)
		{
			//���ζԲ�ɫͼ������������ҶȽ��д���
			for (c = 0; c<3; c++)
			{
				a = TempltExcuteCl(imageBuf0, w, h, templt, size, x, y, c);
				a /= scale;
				//���޴���
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}
		}
	}

	//�����ڴ�
	delete[] templt;
}

void CircleBlurCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, 
	unsigned int pointx, unsigned int pointy, unsigned int nxn)
{
#define R 60
	int x, y;
	int i = 0;
	BYTE** buf = CreatImage(image0, w, h);
	BYTE** bufTemp = CreatImage(image1, w, h);
	//unsigned int x = m_lButPoint.x;
	//unsigned int y = m_lButPoint.y;

	CircleSmoothCl(buf, bufTemp, w, h, nxn);
	static CPoint right[180];
	static CPoint left[180];
	for (int jj = 0; jj < 180; jj++)
	{
		x = pointx + (int)(R*cos((jj - 90)*3.1415926 / 180.0));
		y = pointy + (int)(R*sin((jj - 90)*3.1415926 / 180.0));
		right[jj].x = x;
		right[jj].y = y;
	}
	for (int jj = 0; jj < 180; jj++)
	{
		x = pointx + (int)(R*cos((360 - jj - 90)*3.1415926 / 180.0));
		y = pointy + (int)(R*sin((360 - jj - 90)*3.1415926 / 180.0));
		left[jj].x = x;
		left[jj].y = y;
	}
	for (int jj = 0; jj < 180; jj++)
	{
		i = 0;
		for (int ii = left[jj].x; ii < right[jj].x; ii++)
		{
			//dc.SetPixel(left[jj].x + i, left[jj].y, RGB(255, 0, 0));
			bufTemp[left[jj].y][(left[jj].x + i) * 4 + 0] = buf[left[jj].y][(left[jj].x + i) * 4 + 0];
			bufTemp[left[jj].y][(left[jj].x + i) * 4 + 1] = buf[left[jj].y][(left[jj].x + i) * 4 + 1];
			bufTemp[left[jj].y][(left[jj].x + i) * 4 + 2] = buf[left[jj].y][(left[jj].x + i) * 4 + 2];
			i++;
		}
	}
}
/*
void ZoomUp()
{
	UINT outWidth, outHeight, size;
	UINT beginX, beginY;
	BYTE* tempPixel;
	double x = 0.0, y = 0.0;
	CString st;

	beginX = (UINT)(m_lButPoint.x * m_wheelMove) - m_lButPoint.x;
	beginY = (UINT)(m_lButPoint.y * m_wheelMove) - m_lButPoint.y;
	outWidth = (UINT)(pView->m_nPicWidth * m_wheelMove);
	outHeight = (UINT)(pView->m_nPicHeight * m_wheelMove);
	size = outWidth * outHeight * 4;
	tempPixel = new BYTE[size];
	memset(tempPixel, 255, size);
	BYTE** buf = CreatImage(pView->m_pImageBuffer, pView->m_nPicWidth, pView->m_nPicHeight);
	BYTE** bufTemp = CreatImage(tempPixel, outWidth, outHeight);
	BYTE** bufOutTemp = CreatImage(pView->m_pImageTempBuffer, pView->m_nPicWidth, pView->m_nPicHeight);

	for (UINT j = 0; j < outHeight; j++)
	{
		y = j / m_wheelMove;
		for (UINT i = 0; i < outWidth; i++)
		{
			x = i / m_wheelMove;
			//4���ٽ��ĵ�
			int x1, y1, x2, y2;
			x1 = (int)x;
			y1 = (int)y;
			x2 = x1 + 1;
			y2 = y1 + 1;

			if (x2 == 512 || y2 == 512)
				continue;

			BYTE clr1, clr2, clr3, clr4;
			double u, v;
			u = x - x1;
			v = y - y1;

			for (UINT k = 0; k < 3; k++)
			{
				if (j == 613) {
					st.Format(_T("i=%d,j=%d,x1=%d,y1=%d"), i, j, x1, y1);
					//MessageBox(st);
				}
				clr1 = buf[y1][x1 * 4 + k];
				clr2 = buf[y1][x2 * 4 + k];
				clr3 = buf[y2][x1 * 4 + k];
				clr4 = buf[y2][x2 * 4 + k];
				double f1, f2;
				f1 = clr1 * (1 - u) + clr2 * u;
				f2 = clr3 * (1 - u) + clr4 * u;
				bufTemp[j][i * 4 + k] = (BYTE)(f1 * (1 - v) + f2 * v);
			}
		}
	}
	for (UINT j = 0; j < pView->m_nPicHeight; j++)
	{
		for (UINT i = 0; i < pView->m_nPicWidth; i++)
		{
			for (UINT k = 0; k < 3; k++)
				bufOutTemp[j][i + k] = bufTemp[j + beginY][i + beginX + k];
		}
	}
}*/

void SharpLaplacianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int templt[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	int x, y;
	int a;
	int scale;

	scale = 3;//�趨˥������

			  //���ζ�ԭͼ���ÿ�����ؽ��д���
	for (y = 1; y<h - 1; y++)
		for (x = 1; x<w - 1; x++)
		{
			//����������˹ģ���������д���
			a = TempltExcuteAsh(imageBuf0, w, h, templt, 3, x, y);
			a /= scale;
			//���������ؽ�����ǿ
			a = GetAsh(imageBuf0, x, y) + a;
			//���޴���
			a = a>255 ? 255 : a;
			a = a<0 ? 0 : a;
			SetPixelXY(imageBuf1, x, y, a);
		}

	//�����ڴ�
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* ����: ��ɫͼ���������˹�񻯴���scale = 3��
* ����: image0Ϊԭͼ�Σ�image1�񻯽����
*		w��hΪͼ��Ŀ�͸�
******************************************************************/
void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//��ͼ��ת��Ϊ������ʽ
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//�趨ģ��
	int templt[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	int x, y, c;
	int a;
	int scale;

	//�趨˥������
	scale = 3;

	//���ζ�ԭͼ���ÿ�����ؽ��д���
	for (y = 1; y<h - 1; y++)
	{
		for (x = 1; x<w - 1; x++)
		{
			for (c = 0; c<3; c++)
			{
				//����������˹ģ���������д���
				a = TempltExcuteCl(imageBuf0, w, h, templt, 3, x, y, c);
				a /= scale;
				//���������ؽ�����ǿ
				a = imageBuf0[y][x * 4 + c] + a;
				//���޴���
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}
		}
	}
	//�����ڴ�
	free(imageBuf0);
	free(imageBuf1);
}

/***********************************************************************
* Sobel��Ե���  ��scale=0.5��
* ����: image0Ϊԭͼ�Σ�image1Ϊ��Ե�������w��hΪͼ��Ŀ�͸�
* ��typeΪtrueʱ����ֽ��ȡˮƽ�ʹ�ֱ�������нϴ��ߣ�����ȡƽ��ֵ
************************************************************************/
void SideSobel(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, bool type)
{
	int x, y, a, aHr, aHg, aHb, aVr, aVg, aVb, aH, aV;
	long n;
	double scale = 0.5;
	//���δ���ÿ������
	for (y = 1; y < h - 1; y++)
		for (x = 1; x < w - 1; x++)
		{
			//�������ص�ƫ��λ��
			n = (y*w + x) * 4;
			//�����ɫ����ˮƽ�ҶȲ�
			aHr = abs((image0[n - w * 4 - 4] + image0[n - 4] * 2 + image0[n + w * 4 - 4])
				- (image0[n - w * 4 + 4] + image0[n + 4] * 2 + image0[n + w * 4 + 4]));
			//�����ɫ������ֱ�ҶȲ�
			aVr = abs((image0[n - w * 4 - 4] + image0[n - w * 4] * 2 + image0[n - w * 4 + 4])
				- (image0[n + w * 4 - 4] + image0[n + w * 4] * 2 + image0[n + w * 4 + 4]));
			//������ɫ����ˮƽ�ҶȲ�
			aHg = abs((image0[n - w * 4 - 4 + 1] + image0[n - 4 + 1] * 2 + image0[n + w * 4 - 4 + 1])
				- (image0[n - w * 4 + 4 + 1] + image0[n + 4 + 1] * 2 + image0[n + w * 4 + 4 + 1]));
			//������ɫ������ֱ�ҶȲ�
			aVg = abs((image0[n - w * 4 - 4 + 1] + image0[n - w * 4 + 1] * 2 + image0[n - w * 4 + 4 + 1])
				- (image0[n + w * 4 - 4 + 1] + image0[n + w * 4 + 1] * 2 + image0[n + w * 4 + 4 + 1]));
			//������ɫ����ˮƽ�ҶȲ�
			aHb = abs((image0[n - w * 4 - 4 + 2] + image0[n - 4 + 2] * 2 + image0[n + w * 4 - 4 + 2])
				- (image0[n - w * 4 + 4 + 2] + image0[n + 4 + 2] * 2 + image0[n + w * 4 + 4 + 2]));
			//������ɫ������ֱ�ҶȲ�
			aVb = abs((image0[n - w * 4 - 4 + 2] + image0[n - w * 4 + 2] * 2 + image0[n - w * 4 + 4 + 2])
				- (image0[n + w * 4 - 4 + 2] + image0[n + w * 4 + 2] * 2 + image0[n + w * 4 + 4 + 2]));

			//����ˮƽ�ۺϻҶȲ�
			aH = aHr + aHg + aHb;
			//���㴹ֱ�ۺϻҶȲ�
			aV = aVr + aVg + aVb;

			if (type)
			{
				//ȡˮƽ�ʹ�ֱ�������нϴ���
				if (aH > aV) a = aH;
				else a = aV;
			}
			else
			{
				//ȡˮƽ�ʹ�ֱ�����ֵ�ƽ��ֵ
				a = (aH + aV) / 2;
			}
			a = a *scale;
			a = a>255 ? 255 : a;
			//���ɱ�Եɨ����
			SetPixel(image1, n, a);
		}
}

void GrayHandle(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, double ratio)
{
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int k = 0; k < 3; k++)
			{
				imageBuf1[j][i * 4 + k] = imageBuf0[j][i * 4 + k] * ratio;
			}
		}
	}
}

void RGBGainHandle(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h,
	int type, double gain)
{
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			if (type == 3)
			{
				for (int k = 0; k < 3; k++)
				{
					imageBuf1[j][i * 4 + k] = imageBuf0[j][i * 4 + k];
				}
			}
			imageBuf1[j][i * 4 + type] = imageBuf0[j][i * 4 + type] * gain;
		}
	}
}
