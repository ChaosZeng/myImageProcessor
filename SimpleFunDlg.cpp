// SimpleFunDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myImageProcessor.h"
#include "SimpleFunDlg.h"
#include "afxdialogex.h"
#include "common.h"

#define WM_MY_OWN_MSG WM_USER + 100

// CSimpleFunDlg 对话框

IMPLEMENT_DYNAMIC(CSimpleFunDlg, CDialogEx)

CSimpleFunDlg::CSimpleFunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SimpleFun, pParent)
	, m_alpha(0)
	, m_gray(0)
	, m_rGain(0)
	, m_gGain(0)
	, m_bGain(0)
{

}

CSimpleFunDlg::~CSimpleFunDlg()
{
}

void CSimpleFunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_ALPHA, m_spinAlpha);
	DDX_Control(pDX, IDC_SPIN_GRAY, m_spinGray);
	DDX_Text(pDX, IDC_ALPHA, m_alpha);
	DDV_MinMaxUInt(pDX, m_alpha, 0, 100);
	DDX_Text(pDX, IDC_GRAY, m_gray);
	DDV_MinMaxUInt(pDX, m_gray, 0, 100);
	//  DDX_Slider(pDX, IDC_SLIDER_R, m_rGain);
	//  DDX_Slider(pDX, IDC_SLIDER_G, m_gGain);
	//  DDX_Slider(pDX, IDC_SLIDER_B, m_bGain);
	DDX_Control(pDX, IDC_SLIDER_R, m_rSlider);
	DDX_Control(pDX, IDC_SLIDER_G, m_gSlider);
	DDX_Control(pDX, IDC_SLIDER_B, m_bSlider);
}


BEGIN_MESSAGE_MAP(CSimpleFunDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ALPHA, &CSimpleFunDlg::OnDeltaposSpinAlpha)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GRAY, &CSimpleFunDlg::OnDeltaposSpinGray)
	ON_BN_CLICKED(IDC_APPLY, &CSimpleFunDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_APPLY_GRAY, &CSimpleFunDlg::OnBnClickedApplyGray)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_RESET, &CSimpleFunDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CSimpleFunDlg 消息处理程序


void CSimpleFunDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 关闭的同时关闭父窗口
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	::SendMessage(Handle, WM_CLOSE, 0, 0);
	CDialogEx::OnClose();
}


BOOL CSimpleFunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_spinAlpha.SetRange(0, 100);
	m_spinGray.SetRange(0, 100);
	m_alpha = 100;
	m_gray = 100;
	SetDlgItemInt(IDC_ALPHA, m_alpha);
	SetDlgItemInt(IDC_GRAY, m_gray);
	m_spinAlpha.SetBuddy(GetDlgItem(IDC_ALPHA));
	m_spinGray.SetBuddy(GetDlgItem(IDC_GRAY));

	m_rSlider.SetRange(0, 200);
	m_rSlider.SetTicFreq(1);
	m_rSlider.SetPos(100);
	m_gSlider.SetRange(0, 200);
	m_gSlider.SetTicFreq(1);
	m_gSlider.SetPos(100);
	m_bSlider.SetRange(0, 200);
	m_bSlider.SetTicFreq(1);
	m_bSlider.SetPos(100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSimpleFunDlg::OnDeltaposSpinAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头  
	{
		if (m_alpha > 0 && m_alpha <= 100)
			m_alpha -= 1;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头  
	{
		if (m_alpha >= 0 && m_alpha < 100)
			m_alpha += 1;
	}
	UpdateData(false);
	*pResult = 0;
}


void CSimpleFunDlg::OnDeltaposSpinGray(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头  
	{
		if(m_gray>0 && m_gray<=100)
			m_gray -= 1;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头  
	{
		if (m_gray >= 0 && m_gray < 100)
			m_gray += 1;
	}
	UpdateData(false);
	*pResult = 0;
}


void CSimpleFunDlg::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	//::SendMessage(Handle, WM_PAINT, 0, 0);
	::SendMessage(Handle, WM_MY_OWN_MSG, m_alpha, COMMAND_ALPHA);
}


void CSimpleFunDlg::OnBnClickedApplyGray()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();
	//::SendMessage(Handle, WM_PAINT, 0, 0);
	::SendMessage(Handle, WM_MY_OWN_MSG, m_gray, COMMAND_GRAY);
}


void CSimpleFunDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	if (pSlider->GetDlgCtrlID() == IDC_SLIDER_R)
	{
		m_rGain = m_rSlider.GetPos();
		CWnd *parent = GetParent();
		HWND Handle = parent->GetSafeHwnd();

		::SendMessage(Handle, WM_MY_OWN_MSG, m_rGain, COMMAND_R_GAIN);
	}
	else if (pSlider->GetDlgCtrlID() == IDC_SLIDER_G)
	{
		m_gGain = m_gSlider.GetPos();
		CWnd *parent = GetParent();
		HWND Handle = parent->GetSafeHwnd();

		::SendMessage(Handle, WM_MY_OWN_MSG, m_gGain, COMMAND_G_GAIN);
	}
	else if (pSlider->GetDlgCtrlID() == IDC_SLIDER_B)
	{
		m_bGain = m_bSlider.GetPos();
		CWnd *parent = GetParent();
		HWND Handle = parent->GetSafeHwnd();

		::SendMessage(Handle, WM_MY_OWN_MSG, m_bGain, COMMAND_B_GAIN);
	}
}


void CSimpleFunDlg::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_rSlider.SetPos(100);
	m_gSlider.SetPos(100);
	m_bSlider.SetPos(100);
	CWnd *parent = GetParent();
	HWND Handle = parent->GetSafeHwnd();

	::SendMessage(Handle, WM_MY_OWN_MSG, 0, COMMAND_GAIN_RESET);
}
