// ProjectSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lasaw.h"
#include "ProjectSettingDlg.h"
#include "MainFrm.h"

// CProjectSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectSettingDlg, CDialogEx)

CProjectSettingDlg::CProjectSettingDlg(CWnd* pParent)
: CDialogEx(CProjectSettingDlg::IDD)
{
	int i(0);
	m_bOnCreatNew = FALSE;
	m_nCurrentPrjSel = 0;
// 	ZeroMemory(m_dHomeSpeed, sizeof(m_dHomeSpeed));
	ZeroMemory(m_nLaserParam, sizeof(m_nLaserParam));
	ZeroMemory(m_dRunSpeed, sizeof(m_dRunSpeed));
	bFront = 0;
}

CProjectSettingDlg::~CProjectSettingDlg()
{
}

void CProjectSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	int i(0);
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEWPROJECT, m_MaskName);
	DDX_Control(pDX, IDC_COMBO_PRJNAME, m_cbPrjName);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_btnCreate);
	DDX_Control(pDX, IDC_BUTTON_RELOAD, m_btnLoad);
	DDX_Text(pDX, IDC_EDIT_PPI, m_nLaserParam[0]);
	DDX_Text(pDX, IDC_EDIT_FREQ, m_nLaserParam[1]);
	DDX_Text(pDX, IDC_EDIT_POWER, m_nLaserParam[2]);
	for (i = 0; i < 2; i++)
	{
		DDX_Text(pDX, IDC_EDIT_MU_VEL + 2 * i, m_dRunSpeed[i][0]);
		DDX_Text(pDX, IDC_EDIT_MU_VEL + 2 * i + 1, m_dRunSpeed[i][1]);
	}
	DDX_Radio(pDX, IDC_RADIO_FRONT, bFront);

}


BEGIN_MESSAGE_MAP(CProjectSettingDlg, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_DELETE, &CProjectSettingDlg::OnPopDelete)
	ON_COMMAND(ID_EDIT_NEW, &CProjectSettingDlg::OnPopNew)
	ON_COMMAND(ID_CMD_MOVETO, &CProjectSettingDlg::OnPopMoveto)
	ON_COMMAND(ID_BTN_GETUPDATE, &CProjectSettingDlg::OnPopUpdatePos)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT_PPI, IDC_EDIT_POWER, OnKillFocusProductEdit)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT_MU_VEL, IDC_EDIT_AU_ACVEL, OnKillFocusEditVel)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CProjectSettingDlg::OnClickedBtnCreate)
	ON_CBN_SELCHANGE(IDC_COMBO_PRJNAME, &CProjectSettingDlg::OnCbnSelchangeComboPrjname)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_FRONT, IDC_RADIO_BACK, OnBnChangePosture)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CProjectSettingDlg::OnBnClickedButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CProjectSettingDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CProjectSettingDlg::OnBnClickedButtonApply2)
	ON_BN_CLICKED(IDC_BUTTON_LASERTEST, &CProjectSettingDlg::OnBnClickedButtonLasertest)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CProjectSettingDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CProjectSettingDlg::OnBnClickedButtonCancel)
	ON_MESSAGE(WM_USER_UPDATEUI, &CProjectSettingDlg::OnUpdateDlg)
	ON_BN_CLICKED(IDC_BUTTON_TURNOVER, &CProjectSettingDlg::OnBnClickedButtonTurnover)
	ON_COMMAND(ID_BTN_GETCENTER, &CProjectSettingDlg::OnBtnGetCenter)
	ON_UPDATE_COMMAND_UI(ID_BTN_GETCENTER, &CProjectSettingDlg::OnUpdateBtnGetCenter)
	ON_UPDATE_COMMAND_UI(ID_BTN_GETUPDATE, &CProjectSettingDlg::OnUpdateBtnGetUpdate)
	ON_UPDATE_COMMAND_UI(ID_CMD_MOVETO, &CProjectSettingDlg::OnUpdateBtnGetUpdate)
END_MESSAGE_MAP()


// CProjectSettingDlg ��Ϣ�������


BOOL CProjectSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int i(0);
	CString szNew = theApp.m_cParam.DefaultProjectName.Left(2);
	m_MaskName.EnableMask(_T("DD-AAAAAAAA"), _T("__-________"), _T('0'));
	m_MaskName.SetValidChars(_T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	m_MaskName.EnableGetMaskedCharsOnly(FALSE);
	m_MaskName.EnableSelectByGroup();
	i = STR_INT(szNew);
	szNew.Format(_T("%02d-ADDEDNEW"), i + 1);
	m_MaskName.SetWindowText(szNew);

	POSITION pos = theApp.m_cParam.m_cListPrjName.GetHeadPosition();
	for (i = 0; (pos != NULL) && (i < theApp.m_cParam.m_cListPrjName.GetCount()); i++)
	{
		szNew = theApp.m_cParam.m_cListPrjName.GetNext(pos);
		int nItem = m_cbPrjName.AddString(szNew);
		if ((nItem != CB_ERR) && (nItem != CB_ERRSPACE) && (szNew == theApp.m_cParam.DefaultProjectName))
		{
			m_cbPrjName.SetCurSel(nItem);
			m_nCurrentPrjSel = nItem;
		}
	}
	PostMessage(WM_USER_UPDATEUI, 1, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CProjectSettingDlg::OnCbnSelchangeComboPrjname()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_nCurrentPrjSel = m_cbPrjName.GetCurSel();
}

void CProjectSettingDlg::OnClickedBtnCreate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString szNew;
	m_btnCreate.GetWindowText(szNew);
	if (szNew == _T("���"))
	{
		m_MaskName.GetWindowText(szNew);
		if (szNew.TrimRight().IsEmpty())
		{
			AfxMessageBox(_T("��Ŀ���Ʋ���Ϊ��!\r\n"));
			return;
		}
		szNew.TrimLeft();
		if (CB_ERR != m_cbPrjName.FindString(-1, szNew))
			AfxMessageBox(_T("��Ŀ�����Ѿ�����!\r\n"));
		else
		{
			int nItem = m_cbPrjName.AddString(szNew);
			if ((nItem != CB_ERR) && (nItem != CB_ERRSPACE))
			{
				m_cbPrjName.SetCurSel(nItem);
				m_nCurrentPrjSel = nItem;
				if (!theApp.m_cParam.CreateNewPrj(szNew))
				{
					AfxMessageBox(_T("����Ŀ�ļ��Ѿ�����!\r\n"));
				}else
					AfxMessageBox(_T("��ӳɹ�!\r\n"));

				PostMessage(WM_USER_UPDATEUI, 6, 0);
				::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_USER_UPDATEUI, 0, 1);

			}
		}
	}
	else //modify
	{
		CString szold;
		m_cbPrjName.GetLBText(m_nCurrentPrjSel, szold);
		if (m_cbPrjName.IsWindowVisible())
		{
			m_cbPrjName.ShowWindow(SW_HIDE);
			m_MaskName.SetWindowText(szold);
			m_MaskName.ShowWindow(SW_SHOW);
		}
		else
		{
			m_MaskName.GetWindowText(szNew);
			int nIndex = m_cbPrjName.FindString(-1, szNew);
			if (CB_ERR != nIndex)
				AfxMessageBox(_T("��Ŀ�����Ѿ�����!\r\n"));
			else
			{
				if (!theApp.m_cParam.PrjRename(szold, szNew))
				{
					AfxMessageBox(_T("����ͬ���ļ�����δ�ҵ�����Ŀ�ļ� !\r\n"));
					return;
				}
				m_cbPrjName.DeleteString(m_nCurrentPrjSel);
				m_cbPrjName.InsertString(m_nCurrentPrjSel, szNew);
				::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_USER_UPDATEUI, 0, 0);

			}
		}
	}
}

void CProjectSettingDlg::OnKillFocusProductEdit(UINT idCtl)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int i(0);
	UINT	nNum(0);
	CString Tip;
	UpdateData();
	switch (idCtl)
	{
	case IDC_EDIT_PPI:break;

	case IDC_EDIT_FREQ:
		if (m_nLaserParam[1] > 90)
		{
			m_nLaserParam[1] = 96;
			UpdateData(FALSE);
		}
		break;
	case IDC_EDIT_POWER:
		if (m_nLaserParam[2] > 99.99)
		{
			m_nLaserParam[2] = 100;
			UpdateData(FALSE);
		}
		break;
	}

}

void CProjectSettingDlg::OnKillFocusOthersEdit(UINT idCtl)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	UINT	nNum(0);
	CString Tip;
}

LRESULT CProjectSettingDlg::OnUpdateDlg(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int i(0);
	UINT nIndex = wParam;
	CStatic* pStatic = NULL;
	BOOL bEnable = theApp.GetUserType() && !(theApp.m_nCurrentRunMode >> 1);
	switch (nIndex)
	{
	case 0:
		m_wndLaserTrack.EnableWindow(bEnable);
		m_cbPrjName.EnableWindow(bEnable);
		m_btnCreate.EnableWindow(bEnable);
		m_btnLoad.EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_NEWPROJECT)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MU_VEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_MU_ACVEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_AU_VEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_AU_ACVEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_TURNOVER)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_POWER)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_PPI)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_FREQ)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_LASERTEST)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(bEnable);
		break;
	case 1:
	case 2:
		///******���½�������******///
		m_nLaserParam[0] = theApp.m_cParam.PrjCfg.fLaser2VelRatio;
		m_nLaserParam[1] = theApp.m_cParam.PrjCfg.fPWMfrequency;
		m_nLaserParam[2] = theApp.m_cParam.PrjCfg.fPower;
		for (i = 0; i < 2; i++)
		{
			m_dRunSpeed[i][0] = theApp.m_cParam.Run_VEL[i];
			m_dRunSpeed[i][1] = theApp.m_cParam.Run_ACC[i];
		}
		UpdateData(FALSE);

	case 3:
		if (lParam)
			break;
	case 4:
		UpdateTaskList(lParam);
		break;
	case 5:

		break;
	case 6:
		if (lParam)
		{
			m_btnCreate.SetWindowText(_T("���"));
			m_cbPrjName.ShowWindow(SW_HIDE);
			m_btnLoad.EnableWindow(FALSE);
			m_MaskName.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnCreate.SetWindowText(_T("�޸�����"));
			m_cbPrjName.ShowWindow(SW_SHOW);
			m_MaskName.ShowWindow(SW_HIDE);
			m_btnLoad.EnableWindow(TRUE);

		}
		break;
	default:
		break;
	}

	return 0;
}

void CProjectSettingDlg::OnBnClickedButtonReload()
{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int i(0);
	CString prjname;
	m_cbPrjName.GetLBText(m_nCurrentPrjSel, prjname);
	if (theApp.m_cParam.LoadPrjData(prjname))
	{
		pFrame->StartTemplateRW(TRUE,1);
		::PostMessage(pFrame->GetSafeHwnd(), WM_USER_UPDATEUI, 0, 1);
	}
}

void CProjectSettingDlg::OnBnClickedButtonApply2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString Tip = _T("ȷ���޸Ĳ�����\n");
	if (IDYES == MessageBox(Tip, _T("��ʾ"), MB_YESNO))
	{
		UpdateData();
		theApp.m_cParam.PrjCfg.fLaser2VelRatio= m_nLaserParam[0] ;
		theApp.m_cParam.PrjCfg.fPWMfrequency = m_nLaserParam[1];
		theApp.m_cParam.PrjCfg.fPower = m_nLaserParam[2];
		OnBnClickedButtonApply();
	}
	else
	{
		m_nLaserParam[0] = theApp.m_cParam.PrjCfg.fLaser2VelRatio;
		m_nLaserParam[1] = theApp.m_cParam.PrjCfg.fPWMfrequency;
		m_nLaserParam[2] = theApp.m_cParam.PrjCfg.fPower;
		UpdateData(FALSE);
	}
}

void CProjectSettingDlg::UpdateTaskList(BOOL bSaveAndValidate /*= FALSE*/)
{
	BYTE nIndex(0);
	int nflag(0);
	UINT i(0), j(0);
	UINT nNum(0);
	CString szNo;
	if (!bSaveAndValidate)
	{
		m_wndLaserTrack.RemoveAll();
		if (theApp.m_cParam.PtArray.Ctline.size())
		{
			CTrackLine &tl = theApp.m_cParam.PtArray.Ctline.at(bFront);
			for (i = 0; i < tl.Tlline.size(); i++)
				m_wndLaserTrack.New(tl.Tlline.at(i));
		}
		m_wndLaserTrack.AdjustLayout();
	}
	else
	{
		int nCount = m_wndLaserTrack.GetRowCount();
		if (!theApp.m_cParam.PtArray.nCount)
		{
			theApp.m_cParam.PtArray.Ctline.resize(2);
			theApp.m_cParam.PtArray.nCount = 2;
		}
		if (!nCount)
			return;
		CTrackLine &tl = theApp.m_cParam.PtArray.Ctline.at(bFront);
		tl.Tlline.resize(nCount);
		tl.nCount = nCount;
		for (; nCount != 0; nCount--)
		{
			LPnode& lpn = tl.Tlline.at(nCount - 1);
			CBCGPGridRow* pRow = m_wndLaserTrack.GetRow(nCount - 1);
			lpn.bLaserOn = pRow->GetItem(0)->GetValue() ? TRUE : FALSE;
			szNo = pRow->GetItem(1)->GetValue();
			lpn.uType = szNo == _T("����") ? 2 :  1;

			for (j = 0; j < 2; j++)
				lpn.EndPoint[j] = (double)pRow->GetItem(j + 2)->GetValue();

			szNo = pRow->GetItem(4)->GetValue();
			lpn.nDir = szNo == _T("˳ʱ��") ? 0 : 1;

			for (j = 0; j < 2; j++)
				lpn.CenterPt[j] = pRow->GetItem(j + 5)->GetValue();
			lpn.Vel = theApp.m_cParam.Run_VEL[lpn.bLaserOn];
			lpn.ACC = theApp.m_cParam.Run_ACC[lpn.bLaserOn];
		}

	}
}

void CProjectSettingDlg::OnButtonClear()
{

}

void CProjectSettingDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO:  �ڴ˴������Ϣ����������
	CPoint pt = point;
	BOOL bEnable = theApp.GetUserType() && !(theApp.m_nCurrentRunMode >> 1);
	if (!bEnable)
		return;
	if (pWnd->GetSafeHwnd() == m_wndLaserTrack.GetSafeHwnd())
	{
		m_wndLaserTrack.ScreenToClient(&pt);
		CMenu menu;
		menu.LoadMenuW(IDR_CONTEXT_POPMUNE);

		CMenu *pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
	}

}

void CProjectSettingDlg::OnPopUpdateOffset()
{
	// TODO:  �ڴ���������������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	UINT nTotal = 0;
	CString Tip = _T("ȷ�ϸ���Pin����λ�� ��������趨��λģ����״̬��ʹ�ô�����");
}


void CProjectSettingDlg::OnPopDelete()
{
	// TODO:  �ڴ���������������
	m_wndLaserTrack.Delete();
}


void CProjectSettingDlg::OnPopNew()
{
	// TODO:  �ڴ���������������
	LPnode lpn;
	int i(0);
	ZeroMemory(&lpn, sizeof(LPnode));
	for (i = 0; i < 2; i++)
		lpn.EndPoint[i] = theApp.m_GgMotion.m_dAxisCurPos[i];
	lpn.uType = 1;
	lpn.Vel = theApp.m_cParam.Run_VEL[0];
	lpn.ACC = theApp.m_cParam.Run_ACC[0];

	m_wndLaserTrack.New(lpn);
// 	::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_USER_UPDATEUI, 0, 1);
}


void CProjectSettingDlg::OnPopMoveto()
{
	// TODO:  �ڴ���������������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CBCGPGridItem* pSelcItem = m_wndLaserTrack.GetCurSelItem();
	CBCGPGridRow* pRow = m_wndLaserTrack.GetCurSel();
	int nIndex = pSelcItem->GetColumnId();
	TrackPoint tempPoint;
	ZeroMemory(&tempPoint, sizeof(TrackPoint));
	for (BYTE i = 0; i < 2; i++)
		tempPoint.EndPoint[i] = pRow->GetItem(2 + i)->GetValue();
	if (!theApp.m_GgMotion.m_bIsRuninMode)
		theApp.m_GgMotion.RuninConfig(1);
	theApp.AddedPointMove(tempPoint, TRUE, FALSE);
}

void CProjectSettingDlg::OnPopUpdatePos()
{
	// TODO:  �ڴ���������������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString Tip;
	CBCGPGridRow* pRow = m_wndLaserTrack.GetCurSel();
	int nRow = pRow->GetRowId();
	Tip.Format(_T("ȷ�ϸ��µ�%d������ ��"), nRow);
	for (int i = 0; i < 2; i++)
		pRow->GetItem(2 + i)->SetValue(theApp.m_GgMotion.m_dAxisCurPos[i]);
}

void CProjectSettingDlg::OnBtnGetCenter()
{
	// TODO:  �ڴ���������������
	// TODO: �ڴ���������������
	int j;
	double pt[3][2];
	double cp[2];
	ZeroMemory(&cp, 2 * sizeof(double));

	CBCGPGridRow* pRow = m_wndLaserTrack.GetCurSel();
	int nRow = pRow->GetRowId();
	if (nRow){
		//��ȡѡ���ƽ�淽��
		CBCGPGridItem* pItem ;
		CBCGPGridRow* pLastRow = m_wndLaserTrack.GetRow(nRow - 1);
		//����β��
		pItem = pRow->GetItem(2);
		pt[2][0] = pItem->GetValue();
		pItem = pRow->GetItem(3);
		pt[2][1] = pItem->GetValue();
		//�������
		pItem = pLastRow->GetItem(2);
		pt[0][0] = pItem->GetValue();
		pItem = pLastRow->GetItem(3);
		pt[0][1] = pItem->GetValue();
		//�����м�ĳ��
		pt[1][0] = theApp.m_GgMotion.m_dAxisCurPos[0];
		pt[1][1] = theApp.m_GgMotion.m_dAxisCurPos[1];

	}
	else{
		BCGPMessageBox(_T("�˵ز���Ϊ���ߣ���Ϊû��㰡"));
		return;
	}
	switch (GetCenter(pt, cp))
	{
	case 0:
	case 1:
		pRow->GetItem(5)->SetValue(cp[0]);
		pRow->GetItem(6)->SetValue(cp[1]);
		break;
	case 2:
		BCGPMessageBox(_T("�����ط������ԣ�"));
		break;
	}

}

void CProjectSettingDlg::OnUpdatePopOffset(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
}


int CProjectSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	RECT rc1 = { 400, 90, 900, 390 };
	m_wndLaserTrack.Create(WS_CHILD | WS_VISIBLE, rc1, this, IDC_LIST_LASERTRACK);

	// TODO:  �ڴ������ר�õĴ�������
	return 0;
}


void CProjectSettingDlg::OnBnClickedButtonUpdate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();
	UpdateTaskList(TRUE);
	EndWaitCursor();
}


void CProjectSettingDlg::OnBnClickedButtonLasertest()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
// 	theApp.LaserTestOut();
	theApp.LaserOut(TRUE,FALSE,m_nLaserParam[1]);
}


void CProjectSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_wndLaserTrack.m_hWnd == NULL)
		return;      // Return if window is not created yet.

	CRect rectClient;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_CONTAIN);
	if (pWnd)
	{
		pWnd->GetWindowRect(rectClient);
		ScreenToClient(rectClient);
		m_wndLaserTrack.MoveWindow(rectClient);
	}

	// TODO:  �ڴ˴������Ϣ����������
}


void CProjectSettingDlg::PreInitDialog()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialogEx::PreInitDialog();
}


void CProjectSettingDlg::OnKillFocusEditVel(UINT idCtl)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	UINT nIndex = idCtl - IDC_EDIT_MU_VEL;
	double dvalue = m_dRunSpeed[nIndex>>1][nIndex&0x01];
	UpdateData();
	float& NewValue = m_dRunSpeed[nIndex >> 1][nIndex & 0x01];
	switch (nIndex)
	{
	case 0://IDC_EDIT_MU_VEL
	case 2://IDC_EDIT_AU_VEL
		if (NewValue < 1 || NewValue>399){
			NewValue = dvalue;
			UpdateData(FALSE);
		}
		break;
	case 1://IDC_EDIT_MU_ACVEL
	case 3://IDC_EDIT_AU_ACVEL
		if (NewValue < 1||NewValue>1500){
			NewValue = dvalue;
			UpdateData(FALSE);
		}
		break;
	}

}

// CProjectSettingDlg ��Ϣ�������

void CProjectSettingDlg::OnKillFocusEditPos(UINT idCtl)
{
}

void CProjectSettingDlg::OnBnClickedButtonApply()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int i(0);
	if (IDYES == MessageBox(_T("���¼ӹ���ת���ü��켣?"), _T("��Ҫ��ʾ"), MB_YESNO))
	{
		UpdateData();
		for (i = 0; i < 2; i++)
		{
			theApp.m_cParam.Run_VEL[i] = m_dRunSpeed[i][0];
			theApp.m_cParam.Run_ACC[i] = m_dRunSpeed[i][1];
			theApp.m_GgMotion.m_dCrdVel[i] = theApp.m_cParam.Run_VEL[i];
			theApp.m_GgMotion.m_dCrdAcc[i] = theApp.m_cParam.Run_ACC[i];
		}
		CTrackLine &tl = theApp.m_cParam.PtArray.Ctline.at(bFront);
		i = tl.Tlline.size();
		for (; i != 0; i--)
		{
			LPnode& lpn = tl.Tlline.at(i - 1);
			lpn.Vel = theApp.m_cParam.Run_VEL[lpn.bLaserOn & 0x01];
			lpn.ACC = theApp.m_cParam.Run_ACC[lpn.bLaserOn & 0x01];
		}
		return;
	}
}



void CProjectSettingDlg::OnBnClickedButtonTest()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	theApp.WorkProcess(CLasawApp::CUTTING_TEST+bFront);
}


void CProjectSettingDlg::OnBnClickedButtonCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	PostMessage(WM_USER_UPDATEUI, 6, 0);
	PostMessage(WM_USER_UPDATEUI, 2, 0);
	theApp.LaserOut(FALSE,FALSE,0);
}


void CProjectSettingDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

// 	CDialogEx::OnCancel();
}


void CProjectSettingDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

// 	CDialogEx::OnOK();
}

void CProjectSettingDlg::OnBnChangePosture(UINT idCtl)
{
	CString str[] = { _T("F��"), _T("B��")};
	UpdateData();
	PostMessage(WM_USER_UPDATEUI, 4, 0);
	SetDlgItemText(IDC_STATIC_LEFT, str[1>>bFront]);
	SetDlgItemText(IDC_STATIC_RIGHT, str[bFront]);
}


void CProjectSettingDlg::OnBnClickedButtonTurnover()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str[] = { _T("F��"), _T("B��") };
	CString msg;
	msg.Format(_T("ִ�к�%s�켣������%s�켣��X�ᷭת���������ֱ����Ч��ȷ��ִ�м��㣿"),str[bFront],str[1>>bFront]);
	if (IDYES == MessageBox(msg, _T("��ת��ʾ"), MB_YESNO))
	{
		if (theApp.TurnOverPane(theApp.m_cParam.PtArray.Ctline.at(bFront).Tlline, theApp.m_cParam.PtArray.Ctline.at(1 >> bFront).Tlline))
			PostMessage(WM_USER_UPDATEUI, 4, 0);
		else
			AfxMessageBox(_T("û�й켣����ת��"));
	}
}



void CProjectSettingDlg::OnUpdateBtnGetCenter(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	BOOL bShow(FALSE);
	CBCGPGridItem* pSelcItem = m_wndLaserTrack.GetCurSelItem();
	if (pSelcItem)
	{
		CBCGPGridItemID nid = pSelcItem->GetGridItemID();
		switch (nid.m_nColumn)
		{
		case 5:
		case 6:
			if (pSelcItem->IsEnabled())
				bShow = TRUE;
			break;
		}
	}
	pCmdUI->Enable(bShow);
}


void CProjectSettingDlg::OnUpdateBtnGetUpdate(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	BOOL bShow(FALSE);
	CBCGPGridItem* pSelcItem = m_wndLaserTrack.GetCurSelItem();
	if (pSelcItem)
	{
		CBCGPGridItemID nid = pSelcItem->GetGridItemID();
		switch (nid.m_nColumn)
		{
		case 2:
		case 3:
			if (pSelcItem->IsEnabled())
				bShow = TRUE;
			break;
		}
	}
	pCmdUI->Enable(bShow && (theApp.m_nIsHomeEnd==0x03));
}


void CProjectSettingDlg::OnUpdateCmdMoveto(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
}
