// GrayLineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "myImageProcessor.h"
#include "GrayLineDlg.h"
#include "afxdialogex.h"
#include "Mainfrm.h"
#include "myImageProcessorView.h"


// CGrayLineDlg �Ի���

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


// CGrayLineDlg ��Ϣ�������


BOOL CGrayLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CmyImageProcessorView* pView = (CmyImageProcessorView*)pMain->GetActiveView();
	CRect cr;
	GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
	ClientToScreen(&cr);//ת��ΪӫĻ����   
	cr.right = pView->m_nPicWidth;
	cr.bottom = pView->m_nPicHeight;

	MoveWindow(cr.Width(), cr.Height()/4, cr.right, cr.bottom);//���ô�����ʾ��λ���Լ���С  

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CGrayLineDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CDC* pDC = GetDC();
	Graphics graph(pDC->GetSafeHdc());
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CmyImageProcessorView* pView = (CmyImageProcessorView*)pMain->GetActiveView();

	Bitmap bitmap(pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nPicHeight * 4,
		PixelFormat32bppARGB, pView->m_pImageTempBuffer);

	graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
}

