// HomeProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Lasaw.h"
#include "WaiteProcessDlg.h"


// CWaiteProcessDlg �Ի���

IMPLEMENT_DYNAMIC(CWaiteProcessDlg, CDialogEx)

CWaiteProcessDlg::CWaiteProcessDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CWaiteProcessDlg::IDD, pParent)
{
	nProcess = 0;
}

CWaiteProcessDlg::~CWaiteProcessDlg()
{
}

void CWaiteProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
// 	DDX_Control(pDX, IDC_ANIMATE, m_wndAnimate);
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_wndTips);
}


BEGIN_MESSAGE_MAP(CWaiteProcessDlg, CDialogEx)
	ON_MESSAGE(WM_USER_SHOWPROCESS, &CWaiteProcessDlg::OnProcessing)
END_MESSAGE_MAP()


// CWaiteProcessDlg ��Ϣ�������

BOOL CWaiteProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 640, 480, SWP_NOZORDER);
	CenterWindow();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
// 	m_wndAnimate.Open(IDR_ANIMATION);
	
// 	m_wndAnimate.Open(theApp.m_cParam.m_szCurrentFolder+_T("Res.Bin"));
// 	m_wndAnimate.Play(0, -1, -1);
	m_wndTips.SetText(_T("���ݴ�����..."));
	m_wndTips.SetTextColor(0xff11ad);
	m_ctlProgress.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

LRESULT CWaiteProcessDlg::OnProcessing(WPARAM wParam, LPARAM lParam)
{
	CString info;
	switch ((UINT)wParam)
	{
	case 0:
		nProcess += lParam;
		break;
	default:
		if (nProcess )
			--nProcess;
		break;
	}
	info.Format(_T("���ݴ�����...ʣ��%d"), nProcess);
	m_wndTips.SetText(info);
	if (nProcess<100)
		m_ctlProgress.SetPos(100-nProcess);
	else
		m_ctlProgress.SetPos(1);
	return 0;
}
