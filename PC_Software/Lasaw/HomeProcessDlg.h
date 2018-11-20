#pragma once
#include "Label.h"
// CHomeProcessDlg �Ի���

class CHomeProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeProcessDlg)

public:
	CHomeProcessDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHomeProcessDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_HOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CAnimateCtrl m_wndAnimate;
	CProgressCtrl m_ctlProgress;
	virtual void OnCancel();
	CLabel m_wndTips;
	virtual BOOL OnInitDialog();
	afx_msg LRESULT	OnHoming(WPARAM wParam, LPARAM lParam);
};
