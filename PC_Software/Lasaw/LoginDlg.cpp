// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Lasaw.h"
#include "Mainfrm.h"
#include "LoginDlg.h"

// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	nType = 0;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CLoginDlg::OnBnClickedButtonModify)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BYTE CLoginDlg::CheckPassword(BOOL bOld)
{
	CLasawApp* pApp = (CLasawApp*)AfxGetApp();
	srand((unsigned)time(NULL));
	BYTE nSuccess = (rand() % (0xff - 0x03)) + 0x03;
	int i = ((CComboBox*)GetDlgItem(IDC_COMBO_USERTYPE))->GetCurSel();
	CString str;
	CString Str_Wrong = _T("�������!\r\n");
	if (bOld)
		GetDlgItem(IDC_EDIT_OLDPASSWORD)->GetWindowText(str);
	else
		GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(str);
	UINT m = STR_INT(str);
	switch (i)
	{
	case 1:
		if (m == pApp->m_cParam.PrjCfg.Magic[0] || m == 20176666)
			nSuccess = 1;
		else
			AfxMessageBox(Str_Wrong);
		break;
	case 2:
		if (m == pApp->m_cParam.PrjCfg.Magic[1] || m == 20188888)
			nSuccess = 2;
		else
			AfxMessageBox(Str_Wrong);

		break;
	case 0:
	default:
		nSuccess = 0;
		break;
	}

	return nSuccess;
}


// CLoginDlg ��Ϣ�������


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CComboBox*)GetDlgItem(IDC_COMBO_USERTYPE))->SetCurSel(theApp.GetUserType());

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void CLoginDlg::OnBnClickedButtonLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CLasawApp* pApp = (CLasawApp*)AfxGetApp();
	BYTE nSuccess = CheckPassword();
	if (nSuccess <3)
	{
		pApp->SetUserType(nSuccess);
		CDialogEx::OnOK();
	}
}


void CLoginDlg::OnBnClickedButtonModify()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CLasawApp* pApp = (CLasawApp*)AfxGetApp();
	CString str;
	CString Str_Wrong = _T("���������! ");
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(str);
	UINT m = STR_INT(str);
	nType = CheckPassword(TRUE);
	switch (nType)
	{
	case 1:
	case 2:
		pApp->m_cParam.PrjCfg.Magic[nType - 1] = m;
		AfxMessageBox(_T("�޸ĳɹ�"));
		break;
	case 0:
		AfxMessageBox(_T("��֧�ֵ�ǰ�û����ͣ�\r\n"));
		break;
	}
}


void CLoginDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void CLoginDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
}


void CLoginDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}
