#pragma once

#include "SimpleFunDlg.h"
#include "BlurSetting.h"
#include "atltypes.h"

// CGrayLineDlg 对话框

class CGrayLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayLineDlg)

public:
	CGrayLineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGrayLineDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CSimpleFunDlg dlg;
	CBlurSetting blurdlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	afx_msg LRESULT OnRecvUsermsg(WPARAM wParam, LPARAM lParam);
	unsigned int m_gama;
	unsigned int m_operatorCommon;
	int m_type;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CPoint m_lButPoint;
	bool m_isOpenBlur;
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
//	short m_wheelMove;
//	float m_wheelMove;
	double m_wheelMove;
	bool m_isForCompare;
	int m_gray;
	int m_rGain;
	int m_gGain;
	int m_bGain;
	int m_gainType;
};
