
// myImageProcessor.h : myImageProcessor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "gdiplusinit.h"


// CmyImageProcessorApp:
// �йش����ʵ�֣������ myImageProcessor.cpp
//

class CmyImageProcessorApp : public CWinAppEx
{
public:
	CmyImageProcessorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	GdiplusStartupInput m_GdiplusStartupInput;
	ULONG_PTR m_GdiplusToken;
};

extern CmyImageProcessorApp theApp;
