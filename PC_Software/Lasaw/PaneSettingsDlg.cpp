// PaneSettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Lasaw.h"
#include "PaneSettingsDlg.h"
#include "afxdialogex.h"


// CPaneSettingsDlg �Ի���

IMPLEMENT_DYNAMIC(CPaneSettingsDlg, CDialogEx)

CPaneSettingsDlg::CPaneSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPaneSettingsDlg::IDD, pParent)
{
	bFront = 0;
}

CPaneSettingsDlg::~CPaneSettingsDlg()
{
}

void CPaneSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int i;
	for (i = 0; i < 3; i++)
		DDX_Text(pDX, IDC_EDIT_MARKOFFSET_X + i, dOffset[i]);
	for (i = 0; i < 2; i++)
	{
		DDX_Text(pDX, IDC_EDIT_MARK_POSX + i, dpos[i]);
		DDX_Text(pDX, IDC_EDIT_PANE_L + i, dSize[i]);
	}
	DDX_Radio(pDX, IDC_RADIO_FRONT, bFront);
}


BEGIN_MESSAGE_MAP(CPaneSettingsDlg, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_FRONT, IDC_RADIO_BACK, OnBnChangePosture)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CPaneSettingsDlg::OnBnClickedButtonApply)
	ON_MESSAGE(WM_USER_UPDATEUI, &CPaneSettingsDlg::OnUpdateDlg)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CPaneSettingsDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CPaneSettingsDlg ��Ϣ�������

BOOL CPaneSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	PostMessage(WM_USER_UPDATEUI, 2, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CPaneSettingsDlg::OnBnChangePosture(UINT idCtl)
{
	UpdateData();
	PostMessage(WM_USER_UPDATEUI, 2, 0);
}

void CPaneSettingsDlg::OnBnClickedButtonApply()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UINT i(0), j(0);
	CString Tip = _T("ȷ���޸Ĳ�����\n");
	if (IDYES == MessageBox(Tip, _T("��ʾ"), MB_YESNO))
	{
		UpdateData();
		for (i = 0; i < 3; i++)
			theApp.m_cParam.PrjCfg.fMarkPtOffset[bFront][i] = dOffset[i];
		for (i = 0; i < 2; i++)
		{
			theApp.m_cParam.PrjCfg.fPaneSize[i] = dSize[i];
			theApp.m_cParam.PrjCfg.fMarkPos[bFront][i] = dpos[i];
		}

	}
}

LRESULT CPaneSettingsDlg::OnUpdateDlg(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	int i(0);
	UINT nIndex = wParam;
	BOOL bEnable = theApp.GetUserType() && !(theApp.m_nCurrentRunMode >> 1);
	switch (nIndex)
	{
	case 0:
		GetDlgItem(IDC_RADIO_FRONT)->EnableWindow(bEnable);
		GetDlgItem(IDC_RADIO_BACK)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MARK_POSX)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MARK_POSY)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MARKOFFSET_X)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MARKOFFSET_Y)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MARKOFFSET_R)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(bEnable);

		break;
	case 1:
	case 2:
		///******���½�������******///
		for (i = 0; i < 2;i++)
		{
			dSize[i] = theApp.m_cParam.PrjCfg.fPaneSize[i];
			dpos[i] = theApp.m_cParam.PrjCfg.fMarkPos[bFront][i];
		}
		dOffset[0] = theApp.m_cParam.PrjCfg.fMarkPtOffset[bFront][0];
		dOffset[1] = theApp.m_cParam.PrjCfg.fMarkPtOffset[bFront][1];
		dOffset[2] = theApp.m_cParam.PrjCfg.fMarkPtOffset[bFront][2];
		UpdateData(FALSE);
	default:break;
	}
	return 0;
}

void CPaneSettingsDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

// 	CDialogEx::OnCancel();
}


void CPaneSettingsDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

// 	CDialogEx::OnOK();
}


void CPaneSettingsDlg::OnBnClickedBtnCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	PostMessage(WM_USER_UPDATEUI, 2, 0);

}
