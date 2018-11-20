// MyDialogBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lasaw.h"
#include "MyDialogBar.h"


// CMyDialogBar �Ի���

IMPLEMENT_DYNAMIC(CMyDialogBar, CDialogBar)

CMyDialogBar::CMyDialogBar()
: CDialogBar()
{
	m_bAutoZoom = TRUE;
}

CMyDialogBar::~CMyDialogBar()
{
}

void CMyDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDialogBar, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTip)
	ON_BN_CLICKED(ID_MENUBUTTON_USER, &CMyDialogBar::OnBnClickedMenubuttonUser)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_BUTTON_ZOOM, &CMyDialogBar::OnBnClickedButtonZoom)
END_MESSAGE_MAP()

LRESULT CMyDialogBar::OnInitDialog(UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	// Add Code here
	if (!UpdateData(FALSE)){
		TRACE0("Warning, Unalbe to init update./n");
	}
	m_menu.LoadMenu(IDR_MENU_USER);
	EnableToolTips();
	return bRet;
}

BOOL CMyDialogBar::OnToolTip(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMHDR;
	UINT uID = pNMHDR->idFrom;
	if (pTTT->uFlags & TTF_IDISHWND)
		uID = ::GetDlgCtrlID((HWND)uID);
	if (uID == NULL)   return FALSE;
	switch (uID)
	{
	case  ID_BUTTON_AUTO:
		pTTT->lpszText = _T("���������Զ�����ģʽ");
		break;
	case  ID_BUTTON_HOME:
		pTTT->lpszText = _T("���л��㸴λ״̬");
		break;
	case  ID_BUTTON_STOP:
		pTTT->lpszText = _T("ֹͣ��ǰ����״̬");
		break;
	case  ID_BUTTON_EMG:
		pTTT->lpszText = _T("�쳣ֹͣ������㸴λ");
		break;
	case  ID_RESET_STATUS:
		pTTT->lpszText = _T("�����쳣״̬");
		break;
	case  ID_RESET_PROCESS:
		pTTT->lpszText = _T("������ת����");
		break;
	case  ID_VIEW_IO:
		pTTT->lpszText = _T("��������źż���״̬");
		break;
	case  ID_BUTTON_ZOOM:
		pTTT->lpszText = _T("ͼ�����Ż�����Ӧ");
		break;
	case  ID_MENUBUTTON_USER:
		pTTT->lpszText = _T("�˻���¼");
		break;
	}
	return   TRUE;
}


// CMyDialogBar ��Ϣ�������


void CMyDialogBar::OnBnClickedMenubuttonUser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMFCMenuButton* pBtn = (CMFCMenuButton*)GetDlgItem(ID_MENUBUTTON_USER);
	switch (pBtn->m_nMenuResult)
	{
	case ID_USER_LOGIN:
		theApp.UserLogin();
		break;
	case ID_USER_LOGOUT:
		theApp.UserLogOut();
		break;
	default:

		break;
	}
}

void CMyDialogBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogBar::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
// 		CRect ClientRc; GetWindowRect(ClientRc);
// 		GetDockingFrame()->GetClientRect(&ClientRc);
// 		m_sizeDefault.cx = ClientRc.Width();

		CMFCMenuButton* pBtn = (CMFCMenuButton*)GetDlgItem(ID_MENUBUTTON_USER);
		pBtn->m_hMenu = m_menu.GetSubMenu(0)->GetSafeHmenu();
// 		pBtn->m_bRightArrow = TRUE;
// 		pBtn->RedrawWindow();
	}
	// TODO:  �ڴ˴������Ϣ����������
}


void CMyDialogBar::OnUpdateBtnUI(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->Enable();
}

BOOL CMyDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate,
	UINT nStyle, UINT nID, BOOL bChange)
{
	if (!CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID))
		return FALSE;
	m_sizeDefault.cx = 1280;
	m_bChangeDockedSize = bChange;
	m_sizeFloating = m_sizeDocked = m_sizeDefault;
	return TRUE;
}

BOOL CMyDialogBar::Create(CWnd* pParentWnd,
	LPCTSTR lpszTemplateName,
	UINT nStyle, UINT nID, BOOL bChange)
{
	if (!CDialogBar::Create(pParentWnd, lpszTemplateName,
		nStyle, nID))
		return FALSE;

	m_bChangeDockedSize = bChange;
	m_sizeFloating = m_sizeDocked = m_sizeDefault;
	return TRUE;
}

CSize CMyDialogBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	// Return default if it is being docked or floated
	if ((dwMode & LM_VERTDOCK) || (dwMode & LM_HORZDOCK))
	{
		if (dwMode & LM_STRETCH) // if not docked stretch to fit
			return CSize((dwMode & LM_HORZ) ? 32767 : m_sizeDocked.cx,
			(dwMode & LM_HORZ) ? m_sizeDocked.cy : 32767);
		else
			return m_sizeDocked;
	}
	if (dwMode & LM_MRUWIDTH)
		return m_sizeFloating;
	// In all other cases, accept the dynamic length
	if (dwMode & LM_LENGTHY)
		return CSize(m_sizeFloating.cx, (m_bChangeDockedSize) ?
		m_sizeFloating.cy = m_sizeDocked.cy = nLength :
		m_sizeFloating.cy = nLength);
	else
		return CSize((m_bChangeDockedSize) ?
		m_sizeFloating.cx = m_sizeDocked.cx = nLength :
		m_sizeFloating.cx = nLength, m_sizeFloating.cy);
}


void CMyDialogBar::OnBnClickedButtonZoom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString btntext[2] = { _T("����Ӧ"), _T("ͼ������") };
	theApp.m_NiVision.EnableZoomDisplay(0xff, m_bAutoZoom);
	m_bAutoZoom = !m_bAutoZoom;
	SetDlgItemText(ID_BUTTON_ZOOM, btntext[m_bAutoZoom]);
}
