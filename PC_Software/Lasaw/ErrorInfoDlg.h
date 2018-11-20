#pragma once
#include "Include/Label.h"

// CErrorInfoDlg �Ի���

class CErrorInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CErrorInfoDlg)
	static CCriticalSection		m_Critical;
	CLabel						m_lTitle;
	CLabel						m_lErrorList;
	CEdit						m_lErrorListBox;

public:
	CErrorInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CErrorInfoDlg();
	static UINT AddedErrorMsg(CString Info);
	void ShowErrorInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_ERRORINFO };
	static CStringList m_cListErr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnErrorInfo(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
