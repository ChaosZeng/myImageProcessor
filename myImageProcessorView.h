
// myImageProcessorView.h : CmyImageProcessorView ��Ľӿ�
//

#pragma once
#include "myImageProcessorDoc.h"

class CmyImageProcessorView : public CView
{
protected: // �������л�����
	CmyImageProcessorView();
	DECLARE_DYNCREATE(CmyImageProcessorView)

// ����
public:
	BYTE*	m_pImageBuffer;				// �༭ͼ��ԭʼ��������
	BYTE*	m_pImageTempBuffer;			// ��������������
	UINT	m_nPicWidth;				// ��ǰ�༭ͼ����
	UINT	m_nPicHeight;				// ��ǰ�༭ͼ��߶�
	UINT	m_nTempWidth;				// �����ͼ��Ŀ��
	UINT	m_nTempHeight;				// �����ͼ��ĸ߶�
	bool	m_bIsEditMode;				// ��ǰ�Ƿ�Ϊ�༭ģʽ
	bool	m_bOpenimage;				// �Ƿ����ͼ��

	void OpenFile(CString& strPath);	// ��ͼ���ļ�
	CmyImageProcessorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname);// ��ͼ����Ϣд��bmp�ļ���
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CmyImageProcessorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

private:
	CString			m_strPath;			// ��ǰ��Ŀ¼
	bool GetImageBuffer(CString& strPath);// ��ָ��·����ͼ����Ϣ��ȡ��������
	void FreeImageBuffer(void);			// �ͷ�ͼ����Ϣռ�õ��ڴ�
	void PreDrawImage(void);			// ���ڴ���Ԥ�Ȼ���ͼ��
	
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnGray();
	afx_msg void OnBlur();
//	afx_msg void OnBnClickedGaussApply2();
};

#ifndef _DEBUG  // myImageProcessorView.cpp �еĵ��԰汾
inline CmyImageProcessorDoc* CmyImageProcessorView::GetDocument() const
   { return reinterpret_cast<CmyImageProcessorDoc*>(m_pDocument); }
#endif

