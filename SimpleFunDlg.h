#pragma once
#include "afxcmn.h"


// CSimpleFunDlg 对话框

class CSimpleFunDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSimpleFunDlg)

public:
	CSimpleFunDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSimpleFunDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SimpleFun };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_spinAlpha;
	CSpinButtonCtrl m_spinGray;
	afx_msg void OnDeltaposSpinAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	UINT m_alpha;
	UINT m_gray;
	afx_msg void OnDeltaposSpinGray(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedApplyGray();
//	int m_rGain;
//	int m_gGain;
//	int m_bGain;
	CSliderCtrl m_rSlider;
	CSliderCtrl m_gSlider;
	CSliderCtrl m_bSlider;
//	afx_msg void OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMThemeChangedSliderR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_rGain;
	int m_gGain;
	int m_bGain;
	afx_msg void OnBnClickedBtnReset();
};
