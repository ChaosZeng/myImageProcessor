#pragma once

BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height, int bt = 4);
void SmoothAsh(BYTE* image0, BYTE* image1,unsigned int w, unsigned int h, unsigned int size);
void SmoothCl(BYTE* image0, BYTE* image1,unsigned int w, unsigned int h, unsigned int size);
void SmoothGaussCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
void CircleBlurCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int x, 
	unsigned int y, unsigned int nxn);
void SharpLaplacianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
void SideSobel(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, bool type);
void GrayHandle(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, double ratio);
void RGBGainHandle(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h,
	int type, double gain);
