
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "MyDialogBar.h"
#include "MiddleSplitter.h"
#include "CustomStatusBar.h"
#include "ImageView.h"
#include "MainCtrlDlg.h"
#include "MotionDlg.h"
#include "OptionsViewDlg.h"
#include "WaiteProcessDlg.h"
#include "MyDialogBar.h"
#include "SerialPort.h"
#include "DxfDlg.h"

class CMainFrame : public CFrameWnd
{
	CImageView*					m_pCCDView;
	CSerialPort					m_LightControl;
	DxfDlg*						m_pDxfDlg;
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CMainCtrlDlg*				m_pMainDlg;
	CMotionDlg*					m_pMotionDlg;
	COptionsViewDlg*			m_pOptionViewDlg;
	CWaiteProcessDlg*			m_pWaiteProcessDlg;

	BOOL						m_bFileThreadAlive;
	// ����
public:
	BOOL	InitialDefine();
	void	UpdateTitle();
	BOOL	ShowWaiteProcessDlg(BOOL bShow = TRUE);
	BOOL	SetHardWareRun();
	void	AddedProcessMsg(CString Info, BOOL bClear = FALSE);


	BOOL	StartTemplateRW(BOOL bLoad = TRUE, BYTE nMode = 0);//
	void	ModifyCamView(BYTE nCameraID);
	void	StopCommand(BOOL bEmg = FALSE);
	BOOL	LightControllerOpen(BOOL bOpen = TRUE);
	BOOL	LightControllerSet(BYTE nID, BYTE nValue, BYTE nMode = 0);

	//�����߳�

	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CReBar				m_wndReBar;
	CMyDialogBar		m_wndDlgBar;
	CToolBar          m_wndToolBar;
	CCustomStatusBar        m_wndStatusBar;
	CMiddleSplitter		m_frmSplitterWnd;
	CMiddleSplitter 	m_leftSplitterWnd;
	CMiddleSplitter 	m_rightSplitterWnd;
	BOOL				m_bCreateSpl;
	ULONG					m_lQuantityStart;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMainFrm(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT	OnHomed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnPositionChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIochange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowErrorMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommRecvString(WPARAM wString, LPARAM lLength);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void	UpdateWaiteProcessDlg(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommand(CCmdUI *pCmdUI);
	afx_msg void OnCommandStop();
	afx_msg void OnOperationMode(UINT idCtl);
	afx_msg void OnViewSetting(UINT idCtl);
	afx_msg void OnViewIODlg();
	afx_msg void OnViewDxfDlg();
	afx_msg void OnViewTool();
	afx_msg void ResetSts();
	afx_msg void ResetProcess();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonEmg();
	afx_msg void OnButtonZoom();
};


