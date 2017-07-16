// CBlurSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "myImageProcessor.h"
#include "BlurSetting.h"
#include "afxdialogex.h"
#include "Mainfrm.h"
#include "myImageProcessorView.h"
#include "GrayLineDlg.h"
#include "common.h"
#include <string>
#include <iostream>
using namespace std;

#define WM_MY_OWN_MSG WM_USER + 100

// CBlurSetting 对话框

IMPLEMENT_DYNAMIC(CBlurSetting, CDialogEx)

CBlurSetting::CBlurSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BLUR_SETTING, pParent)
	, m_operatorCommon(0)
	, m_blurOpen(FALSE)
{

}

CBlurSetting::~CBlurSetting()
{
}

void CBlurSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_OPERATOR, m_operatorCommon);
	//  DDV_MinMaxUInt(pDX, m_operatorCommon, 1, 9);
	//  DDX_Text(pDX, IDC_OPERATOR_GAUSS, m_operatorGauss);
	//  DDV_MinMaxUInt(pDX, m_operatorGauss, 1, 9);
	DDX_Text(pDX, IDC_OPERATOR, m_operatorCommon);
	DDV_MinMaxUInt(pDX, m_operatorCommon, 0, 99);
	DDX_Check(pDX, IDC_BLUR_OPEN, m_blurOpen);
}


BEGIN_MESSAGE_MAP(CBlurSetting, CDialogEx)
	ON_BN_CLICKED(IDC_COMMON_APPLY, &CBlurSetting::OnBnClickedCommonApply)
	ON_BN_CLICKED(IDC_GAUSS_APPLY, &CBlurSetting::OnBnClickedGaussApply)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BLUR_OPEN, &CBlurSetting::OnBnClickedBlurOpen)
	ON_BN_CLICKED(IDC_SAVE_FILE, &CBlurSetting::OnBnClickedSaveFile)
	ON_BN_CLICKED(IDC_BTN_COMPARE, &CBlurSetting::OnBnClickedBtnCompare)
	ON_BN_CLICKED(IDC_SHARP, &CBlurSetting::OnBnClickedSharp)
	ON_BN_CLICKED(IDC_EADGE, &CBlurSetting::OnBnClickedEadge)
END_MESSAGE_MAP()


// CBlurSetting 消息处理程序


void CBlurSetting::OnBnClickedCommonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_MY_OWN_MSG, m_operatorCommon, COMMAND_COMMON_BLUR);
}


void CBlurSetting::OnBnClickedGaussApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_MY_OWN_MSG, 0, COMMAND_GAUSS_BLUR);
}


void CBlurSetting::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_CLOSE, 0, 0);
	CDialogEx::OnClose();
}


void CBlurSetting::OnBnClickedBlurOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_BLUR_OPEN))
	{
		m_blurOpen = 1;
	}
	else
	{
		m_blurOpen = 0;
	}
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_MY_OWN_MSG, m_blurOpen, COMMAND_CIRCLE_BLUR_OPEN);
}


void CBlurSetting::OnBnClickedSaveFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CmyImageProcessorView* pView = (CmyImageProcessorView*)pMain->GetActiveView();

	CFileDialog SaveDlg(FALSE, L"bmp", L"未命名", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, L"图像文件格式(*.bmp) |*.bmp||", NULL);
	if (SaveDlg.DoModal() == IDOK)
	{
		CString tempname = SaveDlg.GetPathName();
		string savePath = (LPCSTR)CStringA(tempname);
		pView->WriteBufferToBMP(pView->m_pImageTempBuffer, pView->m_nTempWidth, pView-> m_nTempHeight, savePath.c_str());
		tempname.ReleaseBuffer();
		MessageBox(L"文件已成功保存!");
	}
}


void CBlurSetting::OnBnClickedBtnCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	static CGrayLineDlg dlg;
	dlg.m_isForCompare = 1;
	dlg.Create(IDD_GRAY, this);
	dlg.ShowWindow(SW_SHOW);
}


void CBlurSetting::OnBnClickedSharp()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_MY_OWN_MSG, 0, COMMAND_SHARP);
}


void CBlurSetting::OnBnClickedEadge()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_MY_OWN_MSG, 0, COMMAND_EADGE);
}
