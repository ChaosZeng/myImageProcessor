#pragma once


// CGrayLineDlg �Ի���

class CGrayLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayLineDlg)

public:
	CGrayLineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGrayLineDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
