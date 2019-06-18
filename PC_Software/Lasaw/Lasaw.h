
// Lasaw.h : Lasaw Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "StopWatch.h"
#include "MachineParam.h"
#include "LoginDlg.h"
#include "ErrorInfoDlg.h"
#include "HomeProcessDlg.h"
#include "IOStatusDlg.h"
#include "NiVisionCtl.h"
#include "GgMotion.h"
#include "DxfLocus.h"

// CLasawApp:
// �йش����ʵ�֣������ Lasaw.cpp
//

class CLasawApp : public CWinApp
{
	UINT						m_nUserType;
	CLoginDlg*					m_pLoginDlg;
	CErrorInfoDlg*				m_pErrorInfoDlg;
	CHomeProcessDlg*			m_pHomeProcessDlg;
	CIOStatusDlg*				m_pIODlg;
public:
	CLasawApp();
	enum { BASIC_INFO_LAYER = 0, MATCH_INFO_LAYER, CALCULATED_INFO_LAYER, DETECT_INFO_LAYER, ASSISANT_INFO_LAYER,
		SEARCH_RC_TEMPLATE, RC_TEMPLATE, SEARCH_RC_LINE	};
	enum { RELOAD_POSITION = 0, SNAPSHOT_POSITION,LASER_POSITION, LASER_CALI_POSITION };
	enum { START_CMD, SNAPSHOT_MATCH, PROCESSING_READY, PROCESSING_START, CUTTING_TEST };
	enum { START_DONE, SNAPSHOT_MATCH_DONE, PROCESSING_READY_DONE, PROCESSING_DONE, CUTTING_TEST_DONE };
	enum { NORMAL = 0, VISION_FAIL = 1, VISION_OK = 2, VISION_FAIL_LIMIT = 3, CUTTING_OK = 4, CUTTING_FAIL = 5 };//
	enum { FLASH_DISABLE = 0, FLASH_ENABLE = 1 };
	enum { SET_STS = 0, ADD_STS = 1, ERASING_STS = 2 };

	CMachineParam						m_cParam;
	CStopWatch							m_StopWatch;
	CStopWatch							m_CircleWatch;
	CDxfLocus							m_DxfLocus;
	CNiVisionCtl							m_NiVision;
	CGgMotion								m_GgMotion;
	BYTE									m_nCameraID[4];
	BYTE									m_nCurrentCameraID;
	BYTE									m_nSemiRun;
	BYTE									m_nCurrentRunMode;//bit 0:�ߵ���ģʽ��bit 1:����ģʽ�У�bit 2:�Զ�ģʽ�У�bit 3:����ģʽ�У�bit 4ģʽ�У�
	BYTE									m_nCurrentPane[2];//0:������1:�����
	BYTE									m_bSortingDir;
	BYTE									m_nPanePosture;//����
	BYTE									m_nCalculateDone;//�켣�����ʶλ

	UINT									m_nHwReady;//Ӳ��״̬
	UINT									m_nIsHomeEnd;
	UINT									m_bWaitforTrigger;

	BOOL									m_bIsNeedStop;
	BOOL									m_bAppointByMu;
	BOOL									m_bIdling;
	BOOL									m_bOnline;
	BOOL									m_bSettleSuccess;
	BOOL									m_bSalveThreadAlive;
	BOOL									m_bLearningAlive;
	BOOL									m_bAutoThreadAlive;
	BOOL									m_bHomeThreadAlive;
	pLuckyBag					m_pImagePack[8];

	HANDLE						m_hImgProcessEvent[9];//
	HANDLE						m_hProcessEvent[5];//
	HANDLE						m_hSlaveEvent;//
	HANDLE						m_hOneCycleStart;
	HANDLE								m_hEStopRun;
	HANDLE								m_hStopRun;
	HANDLE								m_hRs485Event;//
	std::vector<VC_Result>				m_dProductResult;
	std::vector<Image*>					m_pTemplate;
	std::vector<CTrackLine>				m_CtActline;//ʵ�ʹ켣·��,4Ƭ

	CString m_szUserName = _T("����Ա");
	enum UI_COLOR
	{
		DarkRed = 0x00008B,
		DarkCyan = 0x8B8B00,//����ɫ
		DarkGoldenrod = 0x0B86B8,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkOliveGreen = 0x2F6B55,
		DarkOrange = 0x008CFF,
		DarkBlue = 0x8B0000,
		DarkSeaGreen = 0x8FBC8F,
		DarkSlateBlue = 0x8B3D48,
		DarkSlateGray = 0x4F4F2F,
		DeepPink = 0x9314FF,
		DeepSkyBlue = 0xFFBF00,
		FloralWhite = 0xF0FAFF,
		ForestGreen = 0x228B22,
		GhostWhite = 0xFFF8F8,
		Gold = 0x00D7FF,
		Goldenrod = 0x20A5DA,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0x2FFFAD,
		LawnGreen = 0x00FC7C,
		LightBlue = 0xE6D8AD,
		LightGoldenrodYellow = 0xD2FAFA,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xC1B6FF,
		LightSeaGreen = 0xAAB220,
		LightSkyBlue = 0xFACE87,
		LightSlateGray = 0x998877,
		LightSteelBlue = 0xDEC4B0,
		LightYellow = 0xE0FFFF,
		MediumSlateBlue = 0xEE687B,
		MistyRose = 0xE1E4FF,
		Orange = 0x00A5FF,
		OrangeRed = 0x0045FF,
		Pink = 0xCBC0FF,
		PowderBlue = 0xE6E0B0,
		Purple = 0x800080,
		Red = 0x0000FF,
		RosyBrown = 0x8F8FBC,
		RoyalBlue = 0xE16941,
		SaddleBrown = 0x13458B,
		SandyBrown = 0x60A4F4,
		SeaGreen = 0x578B2E,
		Silver = 0xC0C0C0,
		SkyBlue = 0xEBCE87,
		SlateBlue = 0xCD5A6A,
		SlateGray = 0x908070,
		Snow = 0xFAFAFF,
		SpringGreen = 0x7FFF00,
		SteelBlue = 0xB48246,
		Tomato = 0x4763FF,
		Violet = 0xEE82EE,
		Wheat = 0xB3DEF5,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Yellow = 0x00FFFF,
		YellowGreen = 0x9ACD32,
	};

	BOOL	InitialDefine();
	void	InitEvent();
	void	AddedErrorMsg(CString Info);
	void	AddedProcessMsg(CString Info, BOOL bClear = FALSE);
	BOOL	CreateErrorDlg();
	void	UserLogin();
	void	UserLogOut();
	void	SetUserType(BYTE nId);
	BYTE	GetUserType();
	BOOL	ShowHomeProcessDlg(BOOL bShow = TRUE);
	BOOL	ShowWaiteProcessDlg(BOOL bShow = TRUE);
	BOOL	InitHardware();
	int		InitializeCamera();
	int		InitializeMotionCard(UINT nID = 0);
	BOOL	OpenFileDlg(BOOL bOpen = TRUE,BYTE bType=0);
	BOOL	LoadCalibration(UINT nIndex);
	BOOL	LoadReference(Image* &dstImg, BOOL bSave = FALSE);
	BOOL	RwTemplate(BOOL bLoad);
	BOOL	RwTemplate(UINT nIndex, BOOL);
	BOOL	LearnTemplate(UINT nIndex, Image* mask, Rect& rc, int offsetx = 0, int offsety = 0);
	BOOL	ClearOverlayInfo(UINT nScreen = 0, UINT nIndex = 0);
	UINT    MatchProcessing(Image* image, BYTE nIndex, Rect rc, UINT nScore, BYTE nMode, IVA_StepResults* stepResults);
	UINT    FindLine(Image* image, RotatedRect rorc, BYTE nMode, IVA_StepResults* stepResults);
	//************************************
	// Method:    GetImg2Buffers
	// FullName:  CLasawApp::GetImg2Buffers
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: UINT nCurl ��ǰ�к�
	// Parameter: UINT nIndex ������
	//************************************
	BOOL	GetImg2Buffers(UINT nCurl, UINT nIndex);//
	BOOL	SnapshotProcess(/*UINT nIndex*/);
	//************************************
	// Method:    SettleProcess
	// FullName:  CLasawApp::SettleProcess
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: Image *  & Img
	// Parameter: UINT nIndex ��Ʒ��
	//************************************
	BOOL	SettleProcess(Image* &Img,UINT nIndex);
	BOOL	CuttingPrepareProcess(BYTE nIndex,BYTE bPlane);
	BOOL	CuttingProcess(/*UINT nIndex*/);
	BOOL	CuttingTest(UINT nIndex);

	void	OnUpdateSts(UINT nNum = 100, BYTE nSts = 0, BYTE bMode = 0);//mode0: ���ã�1���ӣ�2������
	void	ResetSts();
	void	ResetProcess();
	void	SetStop(BOOL bState, BOOL bNeedReset = 0);
	BOOL	GoPosition(UINT nPos, BOOL bWait = TRUE);
	int		WaitMotionDone(DWORD dwMilliseconds = 10000);
	BOOL	AddedPointMove(const TrackPoint &lpPoint, BOOL bStart = FALSE, BOOL bWaitDone = FALSE, UINT nWaitTime = 50000);
	BOOL	AddedTrackMove(std::vector<LPnode> tTracks, BOOL bStart = FALSE, BOOL bWaitDone = FALSE, UINT nWaitTime = 150000);
	BOOL	LaserOut(BOOL bOnOff, BYTE bBufCtrlMode, double dPower);
	BOOL	LaserTestOut( double dPower = 2);
	BOOL	TransformPane(std::vector<LPnode> &tdst, const std::vector<LPnode> &tTracks, TransformCoordinate &trans);
	//************************************
	// Method:    TurnOverPane
	// FullName:  CLasawApp::TurnOverPane
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: std::vector<LPnode> & tdst
	// Parameter: const std::vector<LPnode> & tTracks
	// Parameter: BYTE nMode ��λ��ʶ��ģʽ��bit0-��X�᣻bit1-��Y��.
	//************************************
	BOOL	TurnOverPane(std::vector<LPnode> &tdst, const std::vector<LPnode> &tTracks, BYTE nMode=0x01);
	BOOL    MergeLocus(UINT nDest);
	UINT	WorkProcess(UINT nStep);
	int		MasterWaitFor(UINT& nFlage);
	//�����߳�
	BOOL	StartReturnHome();
	BOOL	StartLearning(UINT nIndex, Image* mask,Rect& rc, int offsetx = 0, int offsety = 0);
	BOOL	StartSlaveThread(UINT nIndex);
	BOOL	StartAuto(BOOL bEnable = FALSE);//

	static UINT _cdecl  FileRWProcess(LPVOID lpParam);
protected:
	static void	IMAQ_CALLBACK UserDrawnProc(WindowEventType, int, Tool, Rect);
	static UINT _cdecl	LearningProcess(LPVOID lpParam);
	static UINT _cdecl	HomeThread(LPVOID lpParam);
	static UINT _cdecl	AutoThread(LPVOID lpParam);
	static UINT _cdecl	SlaveThread(LPVOID lpParam);
	static UINT _cdecl	SettleProcessThread(LPVOID lpParam);

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState();
	void	UpdateWaiteProcessDlg(WPARAM wParam = 0, LPARAM lParam = 0);

// ʵ��
	afx_msg void OnAppAbout();
	afx_msg void OnUserLogin();
	afx_msg void OnUserLogout();
	afx_msg void OnUpdateUserLogout(CCmdUI *pCmdUI);
	afx_msg void OnViewIODlg();
	afx_msg LRESULT	OnHomed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

extern CLasawApp theApp;
