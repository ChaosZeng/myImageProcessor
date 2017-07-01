#pragma once


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
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
