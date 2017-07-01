// GrayLineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myImageProcessor.h"
#include "GrayLineDlg.h"
#include "afxdialogex.h"
#include "Mainfrm.h"
#include "myImageProcessorView.h"


// CGrayLineDlg 对话框

IMPLEMENT_DYNAMIC(CGrayLineDlg, CDialogEx)

CGrayLineDlg::CGrayLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAY, pParent)
{

}

CGrayLineDlg::~CGrayLineDlg()
{
}

void CGrayLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGrayLineDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGrayLineDlg 消息处理程序


BOOL CGrayLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CmyImageProcessorView* pView = (CmyImageProcessorView*)pMain->GetActiveView();
	CRect cr;
	GetClientRect(&cr);//获取对话框客户区域大小   
	ClientToScreen(&cr);//转换为荧幕坐标   
	cr.right = pView->m_nPicWidth;
	cr.bottom = pView->m_nPicHeight;

	MoveWindow(cr.Width(), cr.Height()/4, cr.right, cr.bottom);//设置窗口显示的位置以及大小  

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CGrayLineDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CDC* pDC = GetDC();
	Graphics graph(pDC->GetSafeHdc());
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CmyImageProcessorView* pView = (CmyImageProcessorView*)pMain->GetActiveView();

	Bitmap bitmap(pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nPicHeight * 4,
		PixelFormat32bppARGB, pView->m_pImageTempBuffer);

	graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
}

