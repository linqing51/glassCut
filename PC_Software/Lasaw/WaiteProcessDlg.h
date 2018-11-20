#pragma once

#include "Label.h"
// CWaiteProcessDlg �Ի���

class CWaiteProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaiteProcessDlg)

public:
	CWaiteProcessDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaiteProcessDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROGRESS };
	UINT nProcess ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
// 	CAnimateCtrl m_wndAnimate;
	CProgressCtrl m_ctlProgress;
	CLabel m_wndTips;
	virtual BOOL OnInitDialog();
	afx_msg LRESULT	OnProcessing(WPARAM wParam, LPARAM lParam);
};
