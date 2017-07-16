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
* 把线形存储的像素转化为二维数组形式
* 参数: image 线形存储的像素， width,height 图象的长宽
********************************************************/
BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height, int bt = 4)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*)*(height));
	for (int y = 0; y<height; y++)
	{
		//使imageBuf中每个指针分别指向其下标表示的行的行首地址
		imageBuf[y] = image + y*width*bt;
	}
	return imageBuf;
}

/**************************************************
* 功能: 设定指定位置的像素灰度
* 参数: imageBuf为目标图像 x,y为要设定像素的坐标
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
* 功能: 使用模板对彩色图邻域进行运算
* 参数: imageBuf为目标图像 w、h为图像大小
*       templt为模板 tw为邻域大小
*		x,y为要取得像素的坐标
*       cn为颜色分量编号 0为蓝色 1为绿色 2为红色
**************************************************/
int TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn)
{
	int i, j;                        //循环变量
	int m = 0;                      //用来存放加权和
	int px, py;
	//依次对邻域中每个像素进行运算
	for (i = 0; i<tw; i++)
	{
		for (j = 0; j<tw; j++)
		{
			//计算对应模板上位置的像素在原图像中的位置
			py = y - tw / 2 + i;
			px = x - tw / 2 + j;
			//加权求和
			m += imageBuf0[py][px * 4 + cn] * templt[i*tw + j];
		}
	}
	return m;                     //返回结果
}

/*****************************************************************
* 功能: 使用模板对灰度图邻域进行运算
* 参数: imageBuf为目标图像 w、h为图像大小
*       templt为模板 tw为邻域大小
*		x,y为要取得像素的坐标
******************************************************************/
int TempltExcuteAsh(BYTE** imageBuf0, int w, int h,
	int* templt, int tw, int x, int y)
{
	int i, j;                        //循环变量
	int m = 0;                      //用来存放加权和
	int px, py;
	//依次对邻域中每个像素进行运算
	for (i = 0; i<tw; i++)
	{
		for (j = 0; j<tw; j++)
		{
			//计算对应模板上位置的像素在原图像中的位置
			py = y - tw / 2 + i;
			px = x - tw / 2 + j;
			//加权求和
			m += GetAsh(imageBuf0, px, py) * templt[i*tw + j];
		}
	}
	return m;                     //返回结果
}

/******************************************************************
* 功能: 灰度图像的简单平滑处理
* 参数: image0为原图形，image1为平滑结果，
*		w、h为图象的宽和高
*       size为进行平滑的邻域边长
******************************************************************/
void SmoothAsh(BYTE* image0, BYTE* image1,
	unsigned int w, unsigned int h, unsigned int size)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//定义模板
	int* templt;
	int x, y;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];
	for (x = 0; x<size*size; x++)
	{
		templt[x] = 1;
	}
	//设定衰减因子
	scale = size*size;

	//依次对原图像的每个像素进行处理
	for (y = size / 2; y<h - size / 2; y++)
	{
		for (x = size / 2; x<w - size / 2; x++)
		{
			a = TempltExcuteAsh(imageBuf0, w, h, templt, size, x, y);
			a /= scale;
			//过限处理
			a = a>255 ? 255 : a;
			a = a<0 ? 0 : a;
			SetPixelXY(imageBuf1, x, y, a);
		}
	}

	//清理内存
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* 功能: 彩色图像的简单平滑处理
* 参数: image0为原图形，image1平滑结果，
*		w、h为图象的宽和高
*       size为进行平滑的邻域边长
******************************************************************/
void SmoothCl(BYTE* image0, BYTE* image1,
	unsigned int w, unsigned int h, unsigned int size)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//定义模板
	int* templt;
	int x, y, c;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];
	for (x = 0; x<size*size; x++)
	{
		templt[x] = 1;
	}
	//设定衰减因子
	scale = size*size;

	//依次对原图像的每个像素进行处理
	for (y = size / 2; y<h - size / 2; y++)
	{
		for (x = size / 2; x<w - size / 2; x++)
		{
			//依次对彩色图像的三个分量灰度进行处理
			for (c = 0; c<3; c++)
			{
				a = TempltExcuteCl(imageBuf0, w, h, templt, size, x, y, c);
				a /= scale;
				//过限处理
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}
		}
	}

	//清理内存
	delete[] templt;
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* 功能: 彩色图像的高斯平滑处理
* 参数: image0为原图形，image1平滑结果，
*		w、h为图像的宽和高
******************************************************************/
void SmoothGaussCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int templt[9] = { 1,2,1,2,4,2,1,2,1 };
	//int templt[9] = { 1,4,1,4,8,4,1,4,1 };
	int x, y, c;
	int a;
	int scale;

	//设定衰减因子
	scale = 16;
	//scale = 28;

	//依次对原图像的每个像素进行处理
	for (y = 1; y<h - 1; y++)
		for (x = 1; x<w - 1; x++)
			for (c = 0; c<3; c++)
			{
				//利用高斯模板对邻域进行处理
				a = TempltExcuteCl(imageBuf0, w, h, templt, 3, x, y, c);
				a /= scale;
				//过限处理
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}

	//清理内存
	free(imageBuf0);
	free(imageBuf1);
}

void CircleSmoothCl(BYTE** imageBuf0, BYTE** imageBuf1,
	unsigned int w, unsigned int h, unsigned int size)
{
	//定义模板
	int* templt;
	int x, y, c;
	int a;
	int scale;

	//根据邻域大小设定模板
	templt = new int[size * size];
	for (x = 0; x<size*size; x++)
	{
		templt[x] = 1;
	}
	//设定衰减因子
	scale = size*size;

	//依次对原图像的每个像素进行处理
	for (y = size / 2; y<h - size / 2; y++)
	{
		for (x = size / 2; x<w - size / 2; x++)
		{
			//依次对彩色图像的三个分量灰度进行处理
			for (c = 0; c<3; c++)
			{
				a = TempltExcuteCl(imageBuf0, w, h, templt, size, x, y, c);
				a /= scale;
				//过限处理
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}
		}
	}

	//清理内存
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
			//4个临近的点
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
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int templt[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	int x, y;
	int a;
	int scale;

	scale = 3;//设定衰减因子

			  //依次对原图像的每个像素进行处理
	for (y = 1; y<h - 1; y++)
		for (x = 1; x<w - 1; x++)
		{
			//利用拉普拉斯模板对邻域进行处理
			a = TempltExcuteAsh(imageBuf0, w, h, templt, 3, x, y);
			a /= scale;
			//对中心像素进行增强
			a = GetAsh(imageBuf0, x, y) + a;
			//过限处理
			a = a>255 ? 255 : a;
			a = a<0 ? 0 : a;
			SetPixelXY(imageBuf1, x, y, a);
		}

	//清理内存
	free(imageBuf0);
	free(imageBuf1);
}

/******************************************************************
* 功能: 彩色图像的拉普拉斯锐化处理（scale = 3）
* 参数: image0为原图形，image1锐化结果，
*		w、h为图象的宽和高
******************************************************************/
void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h)
{
	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(image0, w, h);
	BYTE** imageBuf1 = CreatImage(image1, w, h);
	//设定模板
	int templt[9] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	int x, y, c;
	int a;
	int scale;

	//设定衰减因子
	scale = 3;

	//依次对原图像的每个像素进行处理
	for (y = 1; y<h - 1; y++)
	{
		for (x = 1; x<w - 1; x++)
		{
			for (c = 0; c<3; c++)
			{
				//利用拉普拉斯模板对邻域进行处理
				a = TempltExcuteCl(imageBuf0, w, h, templt, 3, x, y, c);
				a /= scale;
				//对中心像素进行增强
				a = imageBuf0[y][x * 4 + c] + a;
				//过限处理
				a = a>255 ? 255 : a;
				a = a<0 ? 0 : a;
				imageBuf1[y][x * 4 + c] = a;
			}
		}
	}
	//清理内存
	free(imageBuf0);
	free(imageBuf1);
}

/***********************************************************************
* Sobel边缘检测  （scale=0.5）
* 参数: image0为原图形，image1为边缘检测结果，w、h为图像的宽和高
* 当type为true时，差分结果取水平和垂直方向差分中较大者，否则取平均值
************************************************************************/
void SideSobel(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, bool type)
{
	int x, y, a, aHr, aHg, aHb, aVr, aVg, aVb, aH, aV;
	long n;
	double scale = 0.5;
	//依次处理每个像素
	for (y = 1; y < h - 1; y++)
		for (x = 1; x < w - 1; x++)
		{
			//计算像素的偏移位置
			n = (y*w + x) * 4;
			//计算红色分量水平灰度差
			aHr = abs((image0[n - w * 4 - 4] + image0[n - 4] * 2 + image0[n + w * 4 - 4])
				- (image0[n - w * 4 + 4] + image0[n + 4] * 2 + image0[n + w * 4 + 4]));
			//计算红色分量垂直灰度差
			aVr = abs((image0[n - w * 4 - 4] + image0[n - w * 4] * 2 + image0[n - w * 4 + 4])
				- (image0[n + w * 4 - 4] + image0[n + w * 4] * 2 + image0[n + w * 4 + 4]));
			//计算绿色分量水平灰度差
			aHg = abs((image0[n - w * 4 - 4 + 1] + image0[n - 4 + 1] * 2 + image0[n + w * 4 - 4 + 1])
				- (image0[n - w * 4 + 4 + 1] + image0[n + 4 + 1] * 2 + image0[n + w * 4 + 4 + 1]));
			//计算绿色分量垂直灰度差
			aVg = abs((image0[n - w * 4 - 4 + 1] + image0[n - w * 4 + 1] * 2 + image0[n - w * 4 + 4 + 1])
				- (image0[n + w * 4 - 4 + 1] + image0[n + w * 4 + 1] * 2 + image0[n + w * 4 + 4 + 1]));
			//计算蓝色分量水平灰度差
			aHb = abs((image0[n - w * 4 - 4 + 2] + image0[n - 4 + 2] * 2 + image0[n + w * 4 - 4 + 2])
				- (image0[n - w * 4 + 4 + 2] + image0[n + 4 + 2] * 2 + image0[n + w * 4 + 4 + 2]));
			//计算蓝色分量垂直灰度差
			aVb = abs((image0[n - w * 4 - 4 + 2] + image0[n - w * 4 + 2] * 2 + image0[n - w * 4 + 4 + 2])
				- (image0[n + w * 4 - 4 + 2] + image0[n + w * 4 + 2] * 2 + image0[n + w * 4 + 4 + 2]));

			//计算水平综合灰度差
			aH = aHr + aHg + aHb;
			//计算垂直综合灰度差
			aV = aVr + aVg + aVb;

			if (type)
			{
				//取水平和垂直方向差分中较大者
				if (aH > aV) a = aH;
				else a = aV;
			}
			else
			{
				//取水平和垂直方向差分的平均值
				a = (aH + aV) / 2;
			}
			a = a *scale;
			a = a>255 ? 255 : a;
			//生成边缘扫描结果
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
