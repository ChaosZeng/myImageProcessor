#pragma once


// CBlurSetting 对话框

class CBlurSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CBlurSetting)

public:
	CBlurSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlurSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLUR_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	UINT m_operatorCommon;
//	UINT m_operatorGauss;
	afx_msg void OnBnClickedCommonApply();
	afx_msg void OnBnClickedGaussApply();
	afx_msg void OnClose();
	UINT m_operatorCommon;
//	UINT m_operatorGauss;
	BOOL m_blurOpen;
	afx_msg void OnBnClickedBlurOpen();
	afx_msg void OnBnClickedSaveFile();
	afx_msg void OnBnClickedBtnCompare();
	afx_msg void OnBnClickedSharp();
	afx_msg void OnBnClickedEadge();
};
