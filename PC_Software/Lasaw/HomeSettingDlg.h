#pragma once


// CHomeSettingDlg �Ի���

class CHomeSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CHomeSettingDlg)

public:
	CHomeSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHomeSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_HOMINGSETTING };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
