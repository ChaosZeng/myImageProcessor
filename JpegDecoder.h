#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE unsigned char //����1���ֽڣ�=8���أ�
#define WORD unsigned short int //����2���ֽ�
#define DWORD unsigned int //����4���ֽ�
#define SDWORD signed int //����4���ֽ�
#define SBYTE signed char //����1���ֽ�
#define SWORD signed short int //����2���ֽ�


typedef struct {
	// k��ȡֵ��Χ��1��16
	// Length[k] ��ʾ����Ϊk�Ļ��������������
	BYTE Length[17];
	//��ʾ����Ϊk����С����������ֵ 
	WORD minor_code[17];
	//��ʾ����Ϊk����߻���������ֵ 
	WORD major_code[17];
	// V[k][j] ��ʾ��j������Ϊ k�Ļ���������
	BYTE V[65536];
} Huffman_table;


class JpegDecoder
{
public:
	//�������Ĺ��캯����
	JpegDecoder(FILE *f);
	//���캯��
	~JpegDecoder(void);
	//���ؽ�������������
	int GetJPEGBuffer(WORD X_image, WORD Y_image, BYTE **address_dest_buffer);
	//����ͼ��Ŀ��
	int GetWidth() const { return width; }
	//����ͼ��ĸ߶�
	int GetHeight() const { return height; }

private:
	//��Ա����
	//ͼ��Ŀ��
	DWORD	width;
	//ͼ��ĸ߶�
	DWORD	height;
	//�洢JPEG�ļ���ָ��
	BYTE*	buf;
	//��ǰ�ֽ�
	BYTE	bp;
	//��ǰword
	WORD	wp;
	//��ǰ�ֽڵ�λ��
	DWORD	byte_pos;

	WORD	X_round;
	WORD	Y_round;
	//��RGBA��ʽ���е��������飬������յĽ�����
	BYTE*	im_buffer;
	//ͼ��һ�е��ֽ���
	DWORD	X_image_bytes;

	DWORD	y_inc_value;
	//Y,Cb,Cr������ˮƽ�ʹ�ֱ�����ϵĲ���
	BYTE	YH, YV, CbH, CbV, CrH, CrV;
	WORD	Hmax, Vmax;
	//���������4��
	float*	QT[4];
	//DC��������
	Huffman_table HTDC[4];
	//AC��������
	Huffman_table HTAC[4];
	//Y, Cb, Cr�����������
	BYTE YQ_nr, CbQ_nr, CrQ_nr;
	//Y, Cb, Cr��Ӧ��DC�������������
	BYTE YDC_nr, CbDC_nr, CrDC_nr;
	//Y, Cb, Cr��Ӧ��AC�������������
	BYTE YAC_nr, CbAC_nr, CrAC_nr;
	//���¿�ʼ��ǣ����Ϊ0ʱ��ʾû�����¿�ʼ���
	BYTE Restart_markers;
	//MCU�ؿ�ʼ��ǣ���λ��ÿ��MCU���ǰ��
	WORD MCU_restart;
	SWORD DCY, DCCb, DCCr; // Coeficientii DC pentru Y,Cb,Cr
						   //��ɢ���ұ任ϵ��
	SWORD DCT_coeff[64];
	//��ǰ8x8�ӿ��Y, Cb, Cr
	BYTE Y[64], Cb[64], Cr[64];

	BYTE Y_1[64], Y_2[64], Y_3[64], Y_4[64];
	BYTE tab_1[64], tab_2[64], tab_3[64], tab_4[64];
	SWORD Cr_tab[256], Cb_tab[256]; // Precalculated Cr, Cb tables
	SWORD Cr_Cb_green_tab[65536];

	//�ڴ��е�λ����, ����w1��ƫ�������, ȡֵ��С��16
	BYTE d_k;
	// w1��ʾ�ڴ��еĵ�һ��word ; w2 ��ʾ�ڴ��еĵڶ���word
	WORD w1, w2;
	DWORD mask[17];

	int		shift_temp;

	BYTE*	rlimit_table;
	BYTE*	tempTable;
	char	error_string[90];
	void PrepareRangeLimitTable(void);
	void SkipKbits(BYTE k);
	SWORD GetKbits(BYTE k);
	void CalculateMask(void);
	//��ʼ��������
	void InitQT();
	//����������
	void LoadQuantTable(float *quant_table);
	//�����������
	void LoadHuffmanTable(Huffman_table *HT);
	//������������ݵ�Ԫ
	void ProcessHuffmanDataUnit(BYTE DC_nr, BYTE AC_nr, SWORD *previous_DC);
	//��ɢ�����ұ任
	void IDCTTransform(SWORD *incoeff, BYTE *outcoeff, BYTE Q_nr);
	void Precalculate_Cr_Cb_Tables();
	//ɫ�ʿռ��ת��YCbCr->RGB
	void Convert8x8YCbCrToRGB(BYTE *Y, BYTE *Cb, BYTE *Cr, DWORD im_loc, DWORD X_image_bytes, BYTE *im_buffer);
	void Convert8x8YCbCrToRGBTab(BYTE *Y, BYTE *Cb, BYTE *Cr, BYTE *tab, DWORD im_loc, DWORD X_image_bytes, BYTE *im_buffer);
	void CalculateTabs();
	//JPEG���뺯���ĳ�ʼ������
	int InitJPGDecoding();
	//����JPEG�ļ��ĳ���
	DWORD GetFileSize(FILE *fp);
	void Resync();
	//����1x1��MCU��Ԫ����ͬ
	void DecodeMCU1x1(DWORD im_loc);
	void DecodeMCU2x1(DWORD im_loc);
	void DecodeMCU2x2(DWORD im_loc);
	void DecodeMCU1x2(DWORD im_loc);
	//����JPEGͷ
	bool LoadJPEGHeader(FILE* fp);
	//JPEG���뺯��
	void DecodeJPEGImage();
}; 
#pragma once
