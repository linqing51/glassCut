// CMachineParam.h: interface for the CProjectParam class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "StdAfx.h"
// using namespace std;

#ifndef MAX_ProductNumber
#define MAX_ProductNumber		8
#endif

typedef struct _ProjectCfg{
	BYTE			bIntensity[4];//��Դ����
	BYTE			bProductNum;//����
	BYTE			nPaneNum;//������
	BYTE			uTemplateSts;
	BYTE			uSelectPane;//��Чѡ��
	BYTE			uPPU;//Ƶ�ٱ�:ÿ�̵��ܶ�
	BYTE			uAngleCali;//���ӽǶ�У����
	BYTE			bReserve[10];//ռλ����
	//X_Y_Z_Axis	
	float			fLaser2VelRatio;
	double			dLoadPosition[2];//����/ȡ��;
	double			dCaliPosition[2];// У�����;
	double			dZPosition[4];//0- ��ȫ�߶�;1-��̽��λ;2-У�����;3-�ӹ��߶�;
	float			fPower;//��������
	float			fPWMfrequency;
	float			fAngleCali;//��׼�Ƕ�
  	float			fReserve[18];//ռλ����

	UINT			Magic[2];
	//product
	float			fPaneSize[2];//���ߴ�
	float			fMarkPos[2][2];//Mark������
	float			fMarkPtOffset[2][3];//Mark��ƫ��
	UINT			nExposure[4];//����ع�ֵ
	UINT			nReserve[20];//ռλ����
	//Image procee

}ProjectCfg,*lpProjectCfg;

class CMachineParam  
{
	UINT nPrjNum;
public:
	CMachineParam();
	virtual ~CMachineParam();
	ProjectCfg		PrjCfg;
	BOOL IsFileExisted(LPCTSTR lpszFileName);
	BOOL GetCurrentPath(void);
	UINT GetCurrentPrjNo(void){ return nPrjNum; };
	BOOL CreateLocalDataStoragePath(CString strDir);
	BOOL CreatePrjFolders(LPCTSTR lpPrjName);
	void FindPrjFolders();

	BOOL LoadParameter(CString strFileName, BOOL bLoadSave);
	BOOL LoadPrjData(LPCTSTR lpszFileName, BOOL bIn_Out = TRUE);
	BOOL LoadLaserPath(LPCTSTR lpszFileName, BOOL bIn_Out = TRUE);

	int	 ChangePrj(UINT nTarget);
	BOOL CreateNewPrj(LPCTSTR lpPrjName);
	BOOL PrjRename(LPCTSTR lpPrjName, LPCTSTR lpNewPrjName);
	BOOL InitializeContext();
	void InitializePrjParam();
	BOOL AutoSaveParam();
	// 	CList<CString, CString&>		m_cListPrjName;
	CString			DefaultProjectName;
	CString			m_szCurrentFolder;
	//IO Port
	//Common Input
	BYTE			In_Homed[2];//1��2 - �������
	BYTE			In_EmgStop;//3
	BYTE			In_ExtStop;//4
	BYTE			In_ExtStart;// 8 �������
	BYTE			In_PanePosture[2];//6/7 - ǰ���������
	BYTE			In_TrayIndex;//5 - ���λ��

	//0--Output
	//Common Output
	BYTE			Ou_HomeTrigger[2];
	BYTE			Ou_ExtCircleDone;//���
	BYTE			Ou_ExtAlarm;//����
	BYTE			Ou_ExtOnline;//�����ź�
// 	BYTE			Ou_Tower_yellowlamp;
// 	BYTE			Ou_Tower_Greenlamp;
	//
	BYTE			nDemoMode;
	BYTE			nlaserMode;
	BYTE			nMirror;
	BYTE			nComPort[2];
	USHORT			uDelayTime[2];//���ع���ʱ
	UINT			nlQuantity;
	float			fProportion;
	BYTE			nMin_Angle;
	UINT			nSpline_Step;
	double			dSoftLimit[4];
	double			HOME_VEL[4];
	double			HOME_ACC[4];
	double			Run_VEL[4];//0:����;1:�ӹ�;2:;3:;
	double			Run_ACC[4];
	double			dCCD_Nozzle[4][2];//���ƫ�ƣ�
	CStringList		m_cListPrjName;
	std::vector <MotionPoint>			ProductSnapShotF;//��������λ��
	std::vector <MotionPoint>			ProductSnapShotB;//��������λ��
	CContourTrack						PtArray;//��Ʒ�켣��
	std::vector<Rect>					rcSearch;
	std::vector<RotatedRect>			rorcSearch;
	// 	std::vector<Annulus>				arcSearch;
	std::vector<TemplateData>			TempData;
	std::vector<EdgeData>				SlineData;
};

