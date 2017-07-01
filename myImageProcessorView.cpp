
// myImageProcessorView.cpp : CmyImageProcessorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "myImageProcessor.h"
#endif

#include "myImageProcessorDoc.h"
#include "myImageProcessorView.h"
#include "GrayLineDlg.h"

#include "JpegDecoder.h"
#include <iostream>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct s_RGB {
	BYTE B;
	BYTE G;
	BYTE R;
} RGB;

typedef struct s_BM_header {
	WORD BMP_id; // 'B''M'
	DWORD size; // size in bytes of the BMP file
	DWORD zero_res; // 0
	DWORD offbits; // 54
	DWORD biSize; // 0x28
	DWORD Width;  // X
	DWORD Height;  // Y
	WORD  biPlanes; // 1
	WORD  biBitCount; // 24
	DWORD biCompression; // 0 = BI_RGB
	DWORD biSizeImage; // 0
	DWORD biXPelsPerMeter; // 0xB40
	DWORD biYPelsPerMeter; // 0xB40
	DWORD biClrUsed; //0
	DWORD biClrImportant; //0
} BM_header;

// CmyImageProcessorView

IMPLEMENT_DYNCREATE(CmyImageProcessorView, CView)

BEGIN_MESSAGE_MAP(CmyImageProcessorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CmyImageProcessorView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CmyImageProcessorView::OnFileSave)
	ON_COMMAND(ID_GRAY, &CmyImageProcessorView::OnGray)
END_MESSAGE_MAP()

// CmyImageProcessorView ����/����

CmyImageProcessorView::CmyImageProcessorView()
{
	// TODO: �ڴ˴���ӹ������
	m_pImageBuffer = NULL;
	m_pImageTempBuffer = NULL;
	m_nPicWidth = m_nPicHeight = 0;
	m_bOpenimage = 0;
	m_strPath.Empty();
}

CmyImageProcessorView::~CmyImageProcessorView()
{
}

BOOL CmyImageProcessorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CmyImageProcessorView ����

void CmyImageProcessorView::OnDraw(CDC* /*pDC*/)
{
	CmyImageProcessorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (m_bOpenimage == 1) {
		CDC* pDC = GetDC();
		Graphics graph(pDC->GetSafeHdc());
		Image image(m_strPath);
		//MessageBox(m_strPath);

		graph.DrawImage(&image, 0, 0);
	}

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

/******************************************************************************
*	����:		��ָ��·�����ļ�
******************************************************************************/
void CmyImageProcessorView::OpenFile(CString& strPath)
{
	FreeImageBuffer();

	//int nIndex = strPath.ReverseFind('\\');
	//m_strPath = strPath.Left(nIndex);
	//CString strFileName = strPath.Right(strPath.GetLength() - nIndex - 1);

	PreDrawImage();
	Invalidate();

	if (m_bIsEditMode)
	{
		GetImageBuffer(strPath);
	}
}

void CmyImageProcessorView::PreDrawImage(void)
{
	//CDC* pDC = GetDC();
	//Graphics graph(pDC->GetSafeHdc());
	//Image image(L"h:\\lena.jpg");
	//MessageBox(m_strPath);

	//graph.DrawImage(&image,0,0);
}

void CmyImageProcessorView::FreeImageBuffer(void)
{
	if (m_pImageBuffer != NULL)
	{
		free(m_pImageBuffer);
		m_pImageBuffer = NULL;
	}

	if (m_pImageTempBuffer != NULL)
	{
		free(m_pImageTempBuffer);
		m_pImageTempBuffer = NULL;
	}
}

/******************************************************************************
*	����:			��ָ��·����ͼ����Ϣ��ȡ��������
*	����:
*		strPath		ͼ���ļ���ַ
******************************************************************************/
bool CmyImageProcessorView::GetImageBuffer(CString& strPath)
{
	CString fileNameExt = strPath.Right(3);
	string filePath = (LPCSTR)CStringA(strPath);
	//FILE *fp1;
	//fopen_s(&fp1, "h:\\1.txt", "wb");

	if (fileNameExt == "jpg" || fileNameExt == "JPG")
	{
		FILE *fp;
		//CString st,st_temp;

		fopen_s(&fp, filePath.c_str(), "rb");
		JpegDecoder dec(fp);
		fclose(fp);

		m_nPicWidth = m_nTempWidth = dec.GetWidth();
		m_nPicHeight = m_nTempHeight = dec.GetHeight();
		dec.GetJPEGBuffer(m_nPicWidth, m_nPicHeight, &m_pImageBuffer);
		int sizeb = m_nPicWidth * m_nPicHeight * 4;
		/*
		st_temp.Format(_T("%d"), m_nPicWidth);
		st = st + st_temp + _T("+");
		st_temp.Format(_T("%d"), m_nPicHeight);
		st = st + st_temp + _T("+");
		st_temp.Format(_T("%d"), sizeb);
		st = st + st_temp;
		MessageBox(st);*/

		m_pImageTempBuffer = (BYTE*)malloc(sizeb);
		memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeb);
		//st.Format(_T("%d"), m_pImageTempBuffer[1]- m_pImageTempBuffer[0]);
		//MessageBox(st);
		//fwrite(m_pImageTempBuffer, 1, sizeb, fp1);
	}
	return 0;
}

// CmyImageProcessorView ��ӡ

BOOL CmyImageProcessorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CmyImageProcessorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CmyImageProcessorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CmyImageProcessorView ���

#ifdef _DEBUG
void CmyImageProcessorView::AssertValid() const
{
	CView::AssertValid();
}

void CmyImageProcessorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmyImageProcessorDoc* CmyImageProcessorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmyImageProcessorDoc)));
	return (CmyImageProcessorDoc*)m_pDocument;
}
#endif //_DEBUG


// CmyImageProcessorView ��Ϣ�������


void CmyImageProcessorView::OnFileOpen()
{
	// TODO: �ڴ���������������
	BOOL isOpen = TRUE;     //�Ƿ��(����Ϊ����)  
	//CString filePath;
	CString defaultDir = L"h:\\";   //Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = L"";         //Ĭ�ϴ򿪵��ļ���  
	CString filter = L"�ļ� (*.jpg; *.JPG;)|*.jpg;*.JPG||";   //�ļ����ǵ�����  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"h:\\";
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) {
		m_strPath = openFileDlg.GetPathName();
	}
	//m_strPath.Format() = filePath.GetString();
	m_bOpenimage = 1;
	MessageBox(m_strPath);
	OpenFile(m_strPath);
}

/******************************************************************************
*	����:			��ͼ����Ϣд��bmp�ļ���
*	����:
*		im_buffer	ͼ����Ϣ
*		X_bitmap	���
*		Y_bitmap	�߶�
*		BMPname		����bmp�ļ�·��
******************************************************************************/
void CmyImageProcessorView::WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname)
{
	SWORD x, y;
	RGB *pixel;
	BM_header BH;
	FILE *fp_bitmap;
	DWORD im_loc_bytes;
	BYTE nr_fillingbytes, i;
	BYTE zero_byte = 0;

	fopen_s(&fp_bitmap, BMPname, "wb");
	if (fp_bitmap == NULL)
	{
		AfxMessageBox(L"����! �ļ��޷�����.", MB_ICONWARNING);
		return;
	}

	if (X_bitmap % 4 != 0) nr_fillingbytes = 4 - ((X_bitmap * 3L) % 4);
	else nr_fillingbytes = 0;

	BH.BMP_id = 'M' * 256 + 'B';     fwrite(&BH.BMP_id, 2, 1, fp_bitmap);
	BH.size = 54 + Y_bitmap*(X_bitmap * 3 + nr_fillingbytes); fwrite(&BH.size, 4, 1, fp_bitmap);
	BH.zero_res = 0;             fwrite(&BH.zero_res, 4, 1, fp_bitmap);
	BH.offbits = 54;             fwrite(&BH.offbits, 4, 1, fp_bitmap);
	BH.biSize = 0x28;            fwrite(&BH.biSize, 4, 1, fp_bitmap);
	BH.Width = X_bitmap;	      fwrite(&BH.Width, 4, 1, fp_bitmap);
	BH.Height = Y_bitmap;	      fwrite(&BH.Height, 4, 1, fp_bitmap);
	BH.biPlanes = 1;             fwrite(&BH.biPlanes, 2, 1, fp_bitmap);
	BH.biBitCount = 24;          fwrite(&BH.biBitCount, 2, 1, fp_bitmap);
	BH.biCompression = 0;        fwrite(&BH.biCompression, 4, 1, fp_bitmap);
	BH.biSizeImage = 0;          fwrite(&BH.biSizeImage, 4, 1, fp_bitmap);
	BH.biXPelsPerMeter = 0xB40;  fwrite(&BH.biXPelsPerMeter, 4, 1, fp_bitmap);
	BH.biYPelsPerMeter = 0xB40;  fwrite(&BH.biYPelsPerMeter, 4, 1, fp_bitmap);
	BH.biClrUsed = 0;	          fwrite(&BH.biClrUsed, 4, 1, fp_bitmap);
	BH.biClrImportant = 0;	      fwrite(&BH.biClrImportant, 4, 1, fp_bitmap);

	im_loc_bytes = (DWORD)im_buffer + ((DWORD)Y_bitmap - 1)*X_bitmap * 4;

	for (y = 0; y<Y_bitmap; y++)
	{
		for (x = 0; x<X_bitmap; x++)
		{
			pixel = (RGB *)im_loc_bytes;
			fwrite(pixel, 3, 1, fp_bitmap);
			im_loc_bytes += 4;
		}
		for (i = 0; i<nr_fillingbytes; i++)
			fwrite(&zero_byte, 1, 1, fp_bitmap);
		im_loc_bytes -= 2L * X_bitmap * 4;
	}
	fclose(fp_bitmap);
}

void CmyImageProcessorView::OnFileSave()
{
	// TODO: �ڴ���������������
	if (m_bOpenimage != 1)
	{
		MessageBox(L"��������δ����ͼƬ�ļ�!", L"Magic House", MB_ICONWARNING);
		return;
	}
	if (m_pImageTempBuffer == NULL)
	{
		MessageBox(L"û�п��Ա�����ļ�!", L"Magic House", MB_ICONWARNING);
		return;
	}
	CFileDialog SaveDlg(FALSE, L"bmp", L"δ����", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, L"ͼ���ļ���ʽ(*.bmp) |*.bmp||", NULL);
	if (SaveDlg.DoModal() == IDOK)
	{
		CString tempname = SaveDlg.GetPathName();
		string savePath = (LPCSTR)CStringA(tempname);
		WriteBufferToBMP(m_pImageTempBuffer, m_nTempWidth, m_nTempHeight, savePath.c_str());
		tempname.ReleaseBuffer();
		MessageBox(L"�ļ��ѳɹ�����!");
	}
}


void CmyImageProcessorView::OnGray()
{
	// TODO: �ڴ���������������
	CGrayLineDlg dlg;
	dlg.DoModal();
}
