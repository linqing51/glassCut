// HomeSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lasaw.h"
#include "HomeSettingDlg.h"
#include "MainFrm.h"


// CHomeSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CHomeSettingDlg, CDialog)

CHomeSettingDlg::CHomeSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHomeSettingDlg::IDD, pParent)
{
}

CHomeSettingDlg::~CHomeSettingDlg()
{
}

void CHomeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CHomeSettingDlg, CDialog)
END_MESSAGE_MAP()

BOOL CHomeSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i(0);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
