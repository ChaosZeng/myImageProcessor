// GrayLineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myImageProcessor.h"
#include "GrayLineDlg.h"
#include "afxdialogex.h"
#include "Mainfrm.h"
#include "myImageProcessorView.h"
#include "ImageHandler.h"
#include "common.h"

#define WM_MY_OWN_MSG WM_USER + 100


// CGrayLineDlg 对话框

IMPLEMENT_DYNAMIC(CGrayLineDlg, CDialogEx)

CGrayLineDlg::CGrayLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAY, pParent)
	, m_gama(0)
	, m_operatorCommon(0)
	, m_type(0)
	, m_lButPoint(0)
	, m_isOpenBlur(false)
	, m_wheelMove(1.0)
	, m_isForCompare(false)
	, m_gray(0)
	, m_rGain(0)
	, m_gGain(0)
	, m_bGain(0)
	, m_gainType(0)
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
	ON_MESSAGE(WM_MY_OWN_MSG, OnRecvUsermsg)
	ON_WM_LBUTTONDOWN()
//	ON_WM_MBUTTONUP()
ON_WM_MOUSEWHEEL()
//ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()


// CGrayLineDlg 消息处理程序

LRESULT CGrayLineDlg::OnRecvUsermsg(WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	case COMMAND_GRAY:
		m_nFlags = COMMAND_GRAY;
		m_gray = wParam;
		break;
	case COMMAND_ALPHA:
		m_nFlags = COMMAND_ALPHA;
		m_gama = wParam;
		break;
	case COMMAND_COMMON_BLUR:
		m_nFlags = COMMAND_COMMON_BLUR;
		m_operatorCommon = wParam;
		break;
	case COMMAND_GAUSS_BLUR:
		m_nFlags = COMMAND_GAUSS_BLUR;
		m_operatorCommon = wParam;
		break;
	case COMMAND_CIRCLE_BLUR_OPEN:
		m_nFlags = COMMAND_CIRCLE_BLUR_OPEN;
		m_isOpenBlur = wParam;
		break;
	case COMMAND_SHARP:
		m_nFlags = COMMAND_SHARP;
		break;
	case COMMAND_EADGE:
		m_nFlags = COMMAND_EADGE;
		break;
	case COMMAND_R_GAIN:
		m_rGain = wParam;
		m_nFlags = COMMAND_RGB_GAIN;
		m_gainType = 0;
		break;
	case COMMAND_G_GAIN:
		m_gGain = wParam;
		m_nFlags = COMMAND_RGB_GAIN;
		m_gainType = 1;
		break;
	case COMMAND_B_GAIN:
		m_bGain = wParam;
		m_nFlags = COMMAND_RGB_GAIN;
		m_gainType = 2;
		break;
	case COMMAND_GAIN_RESET:
		m_nFlags = COMMAND_RGB_GAIN;
		m_rGain = 1;
		m_gGain = 1;
		m_bGain = 1;
		m_gainType = 3;
		break;
	default:
		break;
	}
	Invalidate();
	return 0;
}

BOOL CGrayLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDialog::OnInitDialog();

	if (m_type == 1)
	{
		dlg.Create(IDD_SimpleFun, this);
		dlg.ShowWindow(SW_SHOW);
	}
	else if (m_type == 2)
	{
		blurdlg.Create(IDD_BLUR_SETTING, this);
		blurdlg.ShowWindow(SW_SHOW);
	}

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
	if (m_isForCompare) {
		if (m_nFlags ==COMMAND_SCALLING_UP)
		{
			//ZoomUp(pView->m_pImageBuffer, pView->m_pImageTempBuffer, pView->m_nPicWidth,
			//	pView->m_nPicHeight, m_lButPoint.x, m_lButPoint.y, m_wheelMove);
			int beginX, beginY;
			Bitmap bitmap(pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nPicHeight * 4,
				PixelFormat32bppARGB, pView->m_pImageBuffer);

			beginX = (UINT)(m_lButPoint.x * m_wheelMove) - m_lButPoint.x;
			beginY = (UINT)(m_lButPoint.y * m_wheelMove) - m_lButPoint.y;

			graph.DrawImage(&bitmap, -beginX, -beginY, pView->m_nPicWidth * m_wheelMove, pView->m_nPicHeight * m_wheelMove);
			return;
		}
		if ( m_nFlags == COMMAND_SCALLING_DOWN)
		{
			int beginX, beginY;
			Bitmap bitmap(pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nPicHeight * 4,
				PixelFormat32bppARGB, pView->m_pImageBuffer);

			beginX = (UINT)(m_lButPoint.x * m_wheelMove) - m_lButPoint.x;
			beginY = (UINT)(m_lButPoint.y * m_wheelMove) - m_lButPoint.y;

			graph.DrawImage(&bitmap, -beginX, -beginY, pView->m_nPicWidth * m_wheelMove,
				pView->m_nPicHeight * m_wheelMove);
			return;
		}
		Bitmap bitmap(pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nPicHeight * 4,
			PixelFormat32bppARGB, pView->m_pImageBuffer);
		graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);

		return;
	}
	Bitmap bitmap(pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nPicHeight * 4,
		PixelFormat32bppARGB, pView->m_pImageTempBuffer);
	float gama = (float)m_gama / 100;
	double gray = (double)m_gray / 100;

	switch (m_nFlags)
	{
		case COMMAND_GRAY:
		{
			GrayHandle(pView->m_pImageBuffer, pView->m_pImageTempBuffer,
				pView->m_nPicWidth, pView->m_nPicHeight, gray);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			break;
		}
		case COMMAND_ALPHA:
		{
			CLSID encodecClsid;
			Rect rect(0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			Bitmap image(pView->m_nPicWidth, pView->m_nPicHeight);

			ColorMatrix colorMatrix = {
				1,0,0,0,0,
				0,1,0,0,0,
				0,0,1,0,0,
				0,0,0,gama,0,
				0,0,0,0,1
			};
			ImageAttributes imageAttr;
			imageAttr.SetColorMatrix(&colorMatrix);

			graph.DrawImage(&bitmap, rect, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight,
				UnitPixel, &imageAttr);

			//GetEncoderClsid(L"image/jpeg", &encodecClsid);
			//MessageBox(_T("get child message"));
			break;
		}
		case COMMAND_RGB_GAIN:
		{
			double gain;
			if (m_gainType == 0)	gain = (double)m_rGain / 100;
			else if (m_gainType == 1)	gain = (double)m_gGain / 100;
			else if (m_gainType == 2)	gain = (double)m_bGain / 100;
			else if (m_gainType == 3)	gain = 1;
			//CString st;
			//st.Format(_T("m_gainType=%d,gain=%f"), m_gainType, gain);
			//MessageBox(st);

			RGBGainHandle(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
				pView->m_nPicWidth, pView->m_nPicHeight, m_gainType, gain);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			break;
		}
		case COMMAND_COMMON_BLUR:
		{
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			if (m_operatorCommon == 0)
				m_operatorCommon = 1;
			SmoothCl(pView->m_pImageBuffer, pView->m_pImageTempBuffer, pView->m_nPicWidth, 
				pView->m_nPicHeight, m_operatorCommon);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			break;
		}
		case COMMAND_GAUSS_BLUR:
		{
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			SmoothGaussCl(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
				pView->m_nPicWidth, pView->m_nPicHeight);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			break;
		}
		case COMMAND_DIAMOND_BLUR:
		{
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			//CircleBlurCl(pView->m_pImageBuffer, pView->m_pImageTempBuffer, pView->m_nPicWidth, 
				//pView->m_nPicHeight, m_lButPoint.x, m_lButPoint.y);
			BYTE** buf = CreatImage(pView->m_pImageBuffer, pView->m_nPicWidth, pView->m_nPicHeight);
			BYTE** bufTemp = CreatImage(pView->m_pImageTempBuffer, pView->m_nPicWidth, pView->m_nPicHeight);
			int size = 100;
			int y_min = -1, y_max = -1, x_min = -1, x_max = -1;
			unsigned int x = m_lButPoint.x;
			unsigned int y = m_lButPoint.y;
			unsigned int h = pView->m_nPicHeight;
			unsigned int w = pView->m_nPicWidth;
			CString st;
			st.Format(_T("x=%d,y=%d"), pView->m_nPicWidth, pView->m_nPicHeight);
			MessageBox(st);

			SmoothCl(pView->m_pImageBuffer, pView->m_pImageTempBuffer, pView->m_nPicWidth, 
				pView->m_nPicHeight, 7);
			for (int i = -size; i <= 0; i++)
			{
				if ((y_min != -1) && (x_min != -1))
					break;
				if ((y_min == -1) && ((m_lButPoint.y + i) > 0))
					if (y_min == -1)
						y_min = i;
				if ((x_min == -1) && ((m_lButPoint.x + i) > 0))
					if (x_min == -1)
						x_min = i;
			}
			//st.Format(_T("in y=%d,height=%d,x=%d,width=%d"), m_lButPoint.y, pView->m_nPicHeight,
			//	m_lButPoint.x, pView->m_nPicWidth
			//);
			for (int i = size; i >= size; i--)
			{
				if ((y_max != -1) && (x_max != -1))
					break;
				if ((y_max == -1) && ((y + i) < h))
					if (y_max == -1)
						y_max = i;
				if ((x_max == -1) && ((x + i) < w))
					if (x_max == -1)
						x_max = i;
			}
			//st.Format(_T("out y=%d,height=%d,x=%d,width=%d"), m_lButPoint.y, pView->m_nPicHeight,
			//	m_lButPoint.x, pView->m_nPicWidth
			//);
			if (y_min == -1) y_min = -size;
			if (y_max == -1) y_max = size;
			if (x_min == -1) x_min = -size;
			if (x_max == -1) x_max = size;

			//st.Format(_T("x_min = %d,x_max = %d, y_min = %d, y_max = %d"), x_min, x_max, y_min, y_max);
			//MessageBox(st);
			for (int i = y_min; i < 0; i++)
			{
				for (int j = -(size + i + 1) / 2; j < (size + i + 1) / 2; j++)
				{
					if ((j > x_min) && (j < x_max))
					{
						bufTemp[y + i][(x + j) * 4 + 0] = buf[y + i][(x + j) * 4 + 0];
						bufTemp[y + i][(x + j) * 4 + 1] = buf[y + i][(x + j) * 4 + 1];
						bufTemp[y + i][(x + j) * 4 + 2] = buf[y + i][(x + j) * 4 + 2];
					}
				}
			}
			for (int i = 0; i <= y_max; i++)
			{
				for (int j = -(size - i + 1) / 2; j < (size - i + 1) / 2; j++)
				{
					if ((j > x_min) && (j < x_max))
					{
						bufTemp[y + i][(x + j) * 4 + 0] = buf[y + i][(x + j) * 4 + 0];
						bufTemp[y + i][(x + j) * 4 + 1] = buf[y + i][(x + j) * 4 + 1];
						bufTemp[y + i][(x + j) * 4 + 2] = buf[y + i][(x + j) * 4 + 2];
					}
				}
			}
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);

			break;
		}
		case COMMAND_CIRCLE_PAINT:
		{
//#define R 55.3
#define R 60
			int x, y;
			int i=0;

			static CPoint right[181];
			static CPoint left[181];
			for (int jj = 0; jj<=180; jj++)
			{
				x = m_lButPoint.x + (int)(R*cos((jj - 90)*3.1415926 / 180.0));
				y = m_lButPoint.y + (int)(R*sin((jj - 90)*3.1415926 / 180.0));
				right[jj].x = x;
				right[jj].y = y;
				dc.SetPixel(right[jj].x, right[jj].y, RGB(255, 0, 0));
			}
			for (int jj = 0; jj <= 180; jj++)
			{
				x = m_lButPoint.x + (int)(R*cos((360 - jj - 90)*3.1415926 / 180.0));
				y = m_lButPoint.y + (int)(R*sin((360 - jj - 90)*3.1415926 / 180.0));
				left[jj].x = x;
				left[jj].y = y;
				dc.SetPixel(left[jj].x, left[jj].y, RGB(255, 0, 0));
			}
			for (int jj = 0; jj < 180; jj++)
			{
				i = 0;
				for (int ii = left[jj].x; ii < right[jj].x; ii++)
				{
					dc.SetPixel(left[jj].x + i, left[jj].y, RGB(255, 0, 0));
					i++;
				}
			}
			break;
		}
		case COMMAND_PREVIEW_LBUTTON:
		{
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			if (m_operatorCommon == 0)
				m_operatorCommon = 11;
			CircleBlurCl(pView->m_pImageBuffer, pView->m_pImageTempBuffer, pView->m_nPicWidth,
				pView->m_nPicHeight, m_lButPoint.x, m_lButPoint.y, m_operatorCommon);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);

			break;
		}
		case COMMAND_SCALLING_UP:
		{
			//ZoomUp(pView->m_pImageBuffer, pView->m_pImageTempBuffer, pView->m_nPicWidth,
			//	pView->m_nPicHeight, m_lButPoint.x, m_lButPoint.y, m_wheelMove);
			int beginX, beginY;

			beginX = (UINT)(m_lButPoint.x * m_wheelMove) - m_lButPoint.x;
			beginY = (UINT)(m_lButPoint.y * m_wheelMove) - m_lButPoint.y;

			graph.DrawImage(&bitmap, -beginX, -beginY, pView->m_nPicWidth * m_wheelMove, pView->m_nPicHeight * m_wheelMove);
			break;
		}
		case COMMAND_SCALLING_DOWN:
		{
			int beginX, beginY;

			beginX = (UINT)(m_lButPoint.x * m_wheelMove) - m_lButPoint.x;
			beginY = (UINT)(m_lButPoint.y * m_wheelMove) - m_lButPoint.y;

			graph.DrawImage(&bitmap, -beginX, -beginY, pView->m_nPicWidth * m_wheelMove, 
				pView->m_nPicHeight * m_wheelMove);
			break;
		}
		case COMMAND_SHARP:
		{
			SharpLaplacianCl(pView->m_pImageBuffer, pView->m_pImageTempBuffer,
				pView->m_nPicWidth, pView->m_nPicHeight);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			break;
		}
		case COMMAND_EADGE:
		{
			SideSobel(pView->m_pImageBuffer, pView->m_pImageTempBuffer,
				pView->m_nPicWidth, pView->m_nPicHeight, true);
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
		}

		default:
		{
			graph.DrawImage(&bitmap, 0, 0, pView->m_nPicWidth, pView->m_nPicHeight);
			break;
		}
	}
}

int CGrayLineDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageDecodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}


void CGrayLineDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isOpenBlur) {
		m_nFlags = COMMAND_PREVIEW_LBUTTON;
		m_lButPoint = point;
		Invalidate();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CGrayLineDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//zDelta的值为120的倍数，大于0表明滚向远处（离开用户）
	m_wheelMove += 0.1 * zDelta / 120;
	//先获取鼠标相对于屏幕的坐标
	GetCursorPos(&m_lButPoint);
	//然后得到static控件的rect坐标
	CRect pRect;
	this->GetClientRect(&pRect);
	//最后把当前鼠标的坐标转化为相对于rect的坐标
	this->ScreenToClient(&m_lButPoint);

	if (zDelta > 0)
		m_nFlags = COMMAND_SCALLING_UP;
	else
		m_nFlags = COMMAND_SCALLING_DOWN;
	Invalidate();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
