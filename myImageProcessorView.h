
// myImageProcessorView.h : CmyImageProcessorView 类的接口
//

#pragma once
#include "myImageProcessorDoc.h"

class CmyImageProcessorView : public CView
{
protected: // 仅从序列化创建
	CmyImageProcessorView();
	DECLARE_DYNCREATE(CmyImageProcessorView)

// 特性
public:
	BYTE*	m_pImageBuffer;				// 编辑图像原始像素数组
	BYTE*	m_pImageTempBuffer;			// 处理后的像素数组
	UINT	m_nPicWidth;				// 当前编辑图像宽度
	UINT	m_nPicHeight;				// 当前编辑图像高度
	UINT	m_nTempWidth;				// 处理后图像的宽度
	UINT	m_nTempHeight;				// 处理后图像的高度
	bool	m_bIsEditMode;				// 当前是否为编辑模式
	bool	m_bOpenimage;				// 是否打开了图像

	void OpenFile(CString& strPath);	// 打开图像文件
	CmyImageProcessorDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname);// 将图像信息写入bmp文件中
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CmyImageProcessorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
	CString			m_strPath;			// 当前打开目录
	bool GetImageBuffer(CString& strPath);// 将指定路径的图像信息提取到数组中
	void FreeImageBuffer(void);			// 释放图像信息占用的内存
	void PreDrawImage(void);			// 在内存中预先绘制图像
	
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnGray();
	afx_msg void OnBlur();
//	afx_msg void OnBnClickedGaussApply2();
};

#ifndef _DEBUG  // myImageProcessorView.cpp 中的调试版本
inline CmyImageProcessorDoc* CmyImageProcessorView::GetDocument() const
   { return reinterpret_cast<CmyImageProcessorDoc*>(m_pDocument); }
#endif

