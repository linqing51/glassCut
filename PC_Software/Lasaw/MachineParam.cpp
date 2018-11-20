// CMachineParam.cpp: implementation of the CProjectParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MachineParam.h"
#include "json\json.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMachineParam::CMachineParam()
{
	int i(0);
	//-Input
	In_Homed[0] = 0;
	In_Homed[1] = 1;
	In_EmgStop = 2;
	In_TrayIndex = 5;
	In_PanePosture = 6;
	In_ExtStart = 7;//
	In_ExtStop = 8;//
	//-Output
	Ou_HomeTrigger[0] = 0;
	Ou_HomeTrigger[1] = 1;
	Ou_ExtCircleDone = 5;//
	Ou_ExtAlarm = 6;
	Ou_ExtOnline =7;//����

	fProportion = 1;
	nMin_Angle = 1;
	nSpline_Step = 10;
	nComPort[0] = 4;
	nComPort[1] = 4;
	for (i = 0; i < 4; ++i)
	{
		HOME_VEL[i] = 25.010+i;
		HOME_ACC[i] = 100.040+i;
		Run_VEL[i] = 25+i;
		Run_ACC[i] = 100+i;
	}
	dSoftLimit[0] = -100;
	dSoftLimit[0] = 900;
	dSoftLimit[0] = -100;
	dSoftLimit[0] = 900;
	nlQuantity = 0;
	nMirror = 0;
	DefaultProjectName = _T("00-DEFAULTS");
	nPrjNum = 0;
	GetCurrentPath();
	CreatePrjFolders(_T("00-DEFAULTS"));
// 	m_cListPrjName.RemoveAll();
	FindPrjFolders();
	LPnode lp;
	ZeroMemory(&lp, sizeof(lp));
	ZeroMemory(&dCCD_Nozzle,sizeof(dCCD_Nozzle));
	PtArray.Ctline.resize(2);
	PtArray.nCount = 2;
	PtArray.Ctline.at(0).nCount = 1;
	PtArray.Ctline.at(0).Tlline.push_back(lp);
	PtArray.Ctline.at(1).nCount = 1;
	PtArray.Ctline.at(1).Tlline.push_back(lp);
	MotionPoint mt = { 0.0 };
	ProductSnapShotF.push_back(mt);
	ProductSnapShotB.push_back(mt);
	rcSearch.resize(2);
	rorcSearch.resize(2);
	TempData.resize(2);
	SlineData.resize(2);
	InitializePrjParam();
}

CMachineParam::~CMachineParam()
{
	ZeroMemory(&PrjCfg, sizeof(PrjCfg));

}

void CMachineParam::InitializePrjParam()
{
	int i(0);
	ZeroMemory(&PrjCfg, sizeof(PrjCfg));
	PrjCfg.bProductNum = 2;
	PrjCfg.nPaneNum = 4;
	PrjCfg.uTemplateSts=0;
	PrjCfg.uSelectPane = 0x0f;
	PrjCfg.uPPU = 2;
	PrjCfg.uAngleCali=FALSE;
	PrjCfg.fLaser2VelRatio = 0.1;
	PrjCfg.fPWMfrequency = 2;
	//X_Y_Axis	
	PrjCfg.Magic[0] = 12345678;
	PrjCfg.Magic[1] = 87654321;
	PrjCfg.dZPosition[0] = 40;
	for (i = 0; i < 2; ++i)
	{
		PrjCfg.nExposure[i] = 9100;
		PrjCfg.nExposure[2+i] = 9100;
		//product
		PrjCfg.fPaneSize[i]=150-70*i;//���ߴ�		
	}
	PrjCfg.fPower = 50;
	//X_Y_Z_Axis	
}

BOOL CMachineParam::IsFileExisted(LPCTSTR lpszFileName)
{
	BOOL bFinded = FALSE;
	CFileFind filefind;
	bFinded = filefind.FindFile(lpszFileName);
	filefind.Close();
	return bFinded;
}

BOOL CMachineParam::GetCurrentPath(void)
{
	TCHAR path_buffer[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	if (GetModuleFileName(NULL, path_buffer, _MAX_PATH) == 0)
		return FALSE;
	_tsplitpath_s(path_buffer, drive, dir, fname, ext);
	m_szCurrentFolder.Format(_T("%s%s"), drive, dir);
	return TRUE;
}

BOOL CMachineParam::CreateLocalDataStoragePath(CString strDir)
{
	BOOL bSuccess(FALSE);
	//remove ending / if exists
	if (strDir.Right(1) == "\\")
		strDir = strDir.Left(strDir.GetLength() - 1);
	// base case . . .if directory exists
	if (GetFileAttributes(strDir) != INVALID_FILE_ATTRIBUTES)
		return TRUE;
	// recursive call, one less directory
	int nFound = strDir.ReverseFind('\\');
	if (CreateLocalDataStoragePath(strDir.Left(nFound)))
	{// actual work
		if (CreateDirectory(strDir, NULL))
			return TRUE;
	}
	return bSuccess;

}

BOOL CMachineParam::LoadParameter(CString strFileName,BOOL bLoadSave)
{
	CFile file;
	CFileException fileEx;
	Json::Reader m_nJsonReader;
	Json::Value root;
	int i(0);
	CStringA filestr;
	CStringA str;
	const CStringA	IOString[2][16] = { 
		"X�Ḵλ���", "Y�Ḵλ���", "��ͣ��ť", "�����ͣ","���ģʽ", "���ֹͣ","�������","��巭��", 
		"�о����", "״̬1", "״̬2", "״̬3", "״̬4", "ReserveI13", "ReserveI14", "ReserveI15",
		//OUT
		"X�Ḵλ����", "Y�Ḵλ����", "�豸����",	"�����������","�������","����1", "����2", "����3","����4",
		"ReserveO9", "ReserveO10", "ReserveO11", "ReserveO12", "ReserveO13", "ReserveO14", "ReserveO15",
	};
	if (strFileName.IsEmpty())
		strFileName = m_szCurrentFolder + _T("Config.json");//

	if (bLoadSave)//Load
	{
		if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary, &fileEx))
			return FALSE;
		file.Read(filestr.GetBuffer(file.GetLength()), file.GetLength());
		file.Close();
		filestr.ReleaseBuffer();
		if (!m_nJsonReader.parse((LPCSTR)filestr, root))
			return FALSE;
		if (!root["״̬��Ϣ"].isNull())
		{
			nlQuantity = root["״̬��Ϣ"]["��ǰ����"].asUInt();
		}
		if (!root["ͨѶ�˿�"].isNull())
		{
			nComPort[0] = root["ͨѶ�˿�"]["��Դ���ƿ�"].asUInt();
			nComPort[1] = root["ͨѶ�˿�"]["�������ƿ�"].asUInt();
		}
		if (!root["�ļ�֧��"].isNull())
		{
			fProportion = root["�ļ�֧��"]["ͼֽ���ű���"].asFloat();
			nMin_Angle = root["�ļ�֧��"]["��Բ���зֽǶ�"].asUInt();
			nSpline_Step = root["�ļ�֧��"]["���������з�����"].asUInt();
		}
		if (!root["������Ϣ"].isNull())
		{
			if (root["������Ϣ"]["Ĭ����Ŀ"].empty())
				DefaultProjectName = _T("00-DEFAULTS");
			else
				DefaultProjectName = root["������Ϣ"]["Ĭ����Ŀ"].asCString();
			nMirror = root["������Ϣ"]["ͼ��ת"].asUInt();
			dCCD_Nozzle[0][0] = int(root["������Ϣ"]["CCD_1ƫ��X"].asDouble()*10000)/10000.0;
			dCCD_Nozzle[0][1] = int(root["������Ϣ"]["CCD_1ƫ��Y"].asDouble() * 10000) / 10000.0;
			dCCD_Nozzle[1][0] = int(root["������Ϣ"]["CCD_2ƫ��X"].asDouble() * 10000) / 10000.0;
			dCCD_Nozzle[1][1] = int(root["������Ϣ"]["CCD_2ƫ��Y"].asDouble() * 10000) / 10000.0;
			dCCD_Nozzle[2][0] = int(root["������Ϣ"]["CCD_3ƫ��X"].asDouble() * 10000) / 10000.0;
			dCCD_Nozzle[2][1] = int(root["������Ϣ"]["CCD_3ƫ��Y"].asDouble() * 10000) / 10000.0;
			dCCD_Nozzle[3][0] = int(root["������Ϣ"]["CCD_4ƫ��X"].asDouble() * 10000) / 10000.0;
			dCCD_Nozzle[3][1] = int(root["������Ϣ"]["CCD_4ƫ��Y"].asDouble() * 10000) / 10000.0;
			for (i = 0; i < 3; ++i)
			{
				str.Format("Home_Vel_%c", 88 + i);
				HOME_VEL[i] = root["������Ϣ"]["������"][0][str].asDouble();
				str.Format("Home_Acc_%c", 88 + i);
				HOME_ACC[i] = root["������Ϣ"]["������"][1][str].asDouble();
			}
			str.Format("Run_Vel_����");
			Run_VEL[0] = root["������Ϣ"]["������"][2][str].asDouble();
			str.Format("Run_Vel_�ӹ�");
			Run_VEL[1] = root["������Ϣ"]["������"][2][str].asDouble();
			str.Format("Run_Acc_����");
			Run_ACC[0] = root["������Ϣ"]["������"][3][str].asDouble();
			str.Format("Run_Acc_�ӹ�");
			Run_ACC[1] = root["������Ϣ"]["������"][3][str].asDouble();
			str.Format("����λX��");
			dSoftLimit[0] = root["������Ϣ"]["������"][4][str].asDouble();
			str.Format("����λX��");
			dSoftLimit[1] = root["������Ϣ"]["������"][4][str].asDouble();
			str.Format("����λY��");
			dSoftLimit[2] = root["������Ϣ"]["������"][4][str].asDouble();
			str.Format("����λY��");
			dSoftLimit[3] = root["������Ϣ"]["������"][4][str].asDouble();
		}

		//-Input
		if (!root["����"].isNull())
		{
			In_Homed[0] = root["����"][IOString[0][0]].asUInt();
			In_Homed[1] = root["����"][IOString[0][1]].asUInt();
			In_EmgStop = root["����"][IOString[0][2]].asUInt();
			In_ExtStop = root["����"][IOString[0][5]].asUInt();
			In_ExtStart = root["����"][IOString[0][6]].asUInt();
			In_PanePosture = root["����"][IOString[0][7]].asUInt();
			In_TrayIndex = root["����"][IOString[0][8]].asUInt();
		}
		//-Output
		if (!root["���"].isNull())
		{
			Ou_HomeTrigger[0] = root["���"][IOString[1][0]].asUInt();
			Ou_HomeTrigger[1] = root["���"][IOString[1][1]].asUInt();
			Ou_ExtOnline = root["���"][IOString[1][2]].asUInt();
			Ou_ExtCircleDone = root["���"][IOString[1][3]].asUInt();
			Ou_ExtAlarm = root["���"][IOString[1][4]].asUInt();
		}
	}
	else{//Save
		Json::FastWriter writer;
		Json::Value MainItem;
		Json::Value MotorItem;
		Json::Value HomeVelItem;
		Json::Value HomeAccItem;
		Json::Value RunVelItem;
		Json::Value RunAccItem;
		Json::Value LimitItem;
		Json::Value PresetItem;
		Json::Value InputItem;
		Json::Value OutItem;
		Json::Value StatusItem;
		Json::Value PortItem;
		Json::Value dxfItem;

		StatusItem["��ǰ����"] = nlQuantity;
		root["״̬��Ϣ"] = StatusItem;

		PortItem["��Դ���ƿ�"] = nComPort[0];
		PortItem["�������ƿ�"] = nComPort[1];
		root["ͨѶ�˿�"] = PortItem;

		dxfItem["ͼֽ���ű���"] = fProportion;
		dxfItem["��Բ���зֽǶ�"] = nMin_Angle;
		dxfItem["���������з�����"] = nSpline_Step;
		root["�ļ�֧��"] = dxfItem;

		MainItem["Ĭ����Ŀ"] = (LPCSTR)CStringA(DefaultProjectName);
		MainItem["ͼ��ת"] = nMirror;
		MainItem["CCD_1ƫ��X"] = dCCD_Nozzle[0][0];
		MainItem["CCD_1ƫ��Y"] = dCCD_Nozzle[0][1];
		MainItem["CCD_2ƫ��X"] = dCCD_Nozzle[1][0];
		MainItem["CCD_2ƫ��Y"] = dCCD_Nozzle[1][1];
		MainItem["CCD_3ƫ��X"] = dCCD_Nozzle[2][0];
		MainItem["CCD_3ƫ��Y"] = dCCD_Nozzle[2][1];
		MainItem["CCD_4ƫ��X"] = dCCD_Nozzle[3][0];
		MainItem["CCD_4ƫ��Y"] = dCCD_Nozzle[3][1];
		for (i = 0; i < 3; i++)
		{
			str.Format("Home_Vel_%c", 88 + i);
			HomeVelItem[str] = HOME_VEL[i];
			str.Format("Home_Acc_%c", 88 + i);
			HomeAccItem[str] = HOME_ACC[i];
		}

		str.Format("Run_Vel_����");
		RunVelItem[str] = Run_VEL[0];
		str.Format("Run_Acc_����");
		RunAccItem[str] = Run_ACC[0];
		str.Format("Run_Vel_�ӹ�");
		RunVelItem[str] = Run_VEL[1];
		str.Format("Run_Acc_�ӹ�");
		RunAccItem[str] = Run_ACC[1];

		str.Format("����λX��");
		LimitItem[str] = dSoftLimit[0];
		str.Format("����λX��");
		LimitItem[str] = dSoftLimit[1];
		str.Format("����λY��");
		LimitItem[str] = dSoftLimit[2];
		str.Format("����λY��");
		LimitItem[str] = dSoftLimit[3];

		MotorItem.append(HomeVelItem);
		MotorItem.append(HomeAccItem);
		MotorItem.append(RunVelItem);
		MotorItem.append(RunAccItem);
		MotorItem.append(LimitItem);
		MainItem["������"] = MotorItem;
		root["������Ϣ"] = MainItem;

		InputItem[IOString[0][0]] = In_Homed[0];
		InputItem[IOString[0][1]] = In_Homed[1];
		InputItem[IOString[0][2]] = In_EmgStop;
		InputItem[IOString[0][5]] = In_ExtStop;
		InputItem[IOString[0][6]] = In_ExtStart;
		InputItem[IOString[0][7]] = In_PanePosture;
		InputItem[IOString[0][8]] = In_TrayIndex;
		root["����"] = InputItem;

		OutItem[IOString[1][0]] = Ou_HomeTrigger[0];
		OutItem[IOString[1][1]] =Ou_HomeTrigger[1];
		OutItem[IOString[1][2]] =  Ou_ExtOnline;
		OutItem[IOString[1][3]] = Ou_ExtCircleDone;
		OutItem[IOString[1][4]] = Ou_ExtAlarm;
		root["���"] = OutItem;

		Json::StyledWriter sw;
		filestr = sw.write(root).data();
		if (!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite, &fileEx))
			return FALSE;
		UINT n = filestr.GetLength();
		file.Write(filestr, n);
		file.Close();

	}
	return TRUE;
}

BOOL CMachineParam::CreatePrjFolders(LPCTSTR lpPrjName)
{
	CString FolderPathName;
	FolderPathName.Format(_T("%sParameter\\%s"), m_szCurrentFolder, lpPrjName);
	CreateLocalDataStoragePath(m_szCurrentFolder + _T("\\NG"));
	if (CreateLocalDataStoragePath(FolderPathName + _T("\\Images\\")))
		CreateLocalDataStoragePath(FolderPathName + _T("\\Templates\\"));
	else
		return FALSE;
	return TRUE;

}

void CMachineParam::FindPrjFolders()
{
	CFileFind tempFind;
	CString strTmp;
	BOOL bFound;
	CStringList Num_list;
	bFound = tempFind.FindFile(m_szCurrentFolder + _T("Parameter\\*.*")); // 
	while (bFound)
	{
		bFound = tempFind.FindNextFile();
		//����һ���ļ�
		if (tempFind.IsDots()) continue;
		if (tempFind.IsSystem()) continue;
		if (tempFind.IsDirectory())
		{
			CString Foldername = tempFind.GetFileName();
			Foldername.MakeUpper();
			strTmp.Format(_T("%sParameter\\%s\\%s.prj"), m_szCurrentFolder, Foldername, Foldername);
			if (IsFileExisted(strTmp))
			{
				strTmp = Foldername.Left(2);
				if (Num_list.IsEmpty() || Num_list.Find(strTmp) == NULL){
					Num_list.AddTail(strTmp);
				}
				else
				{
					AfxMessageBox(_T("��������ظ�����Ŀ�ļ����ڣ�������޸��ظ���Ŀ�ļ����ƣ�"));
				}

				if (m_cListPrjName.IsEmpty()||m_cListPrjName.Find(Foldername) == NULL){
					m_cListPrjName.AddTail(Foldername);
				}
			}
		}
	}
	tempFind.Close();
	return;
}

BOOL CMachineParam::CreateNewPrj(LPCTSTR lpPrjName)
{
	BOOL bSuccess(FALSE);
	if (m_cListPrjName.Find(CString(lpPrjName)) == NULL)
	{
		if (!CreatePrjFolders(lpPrjName))
			return FALSE;
		m_cListPrjName.AddTail(CString(lpPrjName));
		bSuccess = LoadPrjData(lpPrjName, FALSE);
	}
	return bSuccess;

}

BOOL CMachineParam::PrjRename(LPCTSTR lpPrjName, LPCTSTR lpNewPrjName)
{
	BOOL bSuccess(FALSE);
	CString FolderPathOld;
	CString FolderPathNew;
	CString PathNew;

	FolderPathOld.Format(_T("%sParameter\\%s"), m_szCurrentFolder, lpPrjName);
	FolderPathNew.Format(_T("%sParameter\\%s"), m_szCurrentFolder, lpNewPrjName);

	if (IsFileExisted(FolderPathNew))
		return bSuccess;
	POSITION pos = m_cListPrjName.Find(CString(lpPrjName));
	if (pos != NULL && m_cListPrjName.Find(CString(lpNewPrjName)) == NULL)
	{
		CFile::Rename(FolderPathOld, FolderPathNew);
		FolderPathOld.Format(_T("%s\\%s.pt"), FolderPathNew, lpPrjName);
		PathNew.Format(_T("%s\\%s.pt"), FolderPathNew, lpNewPrjName);
		CopyFile(FolderPathOld, PathNew, FALSE);

		FolderPathOld.Format(_T("%s\\%s.prj"), FolderPathNew, lpPrjName);
		PathNew.Format(_T("%s\\%s.prj"), FolderPathNew, lpNewPrjName);
		CopyFile(FolderPathOld, PathNew, FALSE);


		m_cListPrjName.RemoveAt(pos);
		m_cListPrjName.AddTail(CString(lpNewPrjName));
		FolderPathOld = lpPrjName;
		if (FolderPathOld == DefaultProjectName)
		{
			DefaultProjectName = lpNewPrjName;
			PathNew = DefaultProjectName.Left(2);
			nPrjNum = STR_INT(PathNew);
		}
		bSuccess = TRUE;
	}
	return bSuccess;
}

BOOL CMachineParam::LoadPrjData(LPCTSTR lpszFileName, BOOL bIn_Out /*= TRUE*/)
{
	UINT i(0),j(0);
	CFile file;
	CFileException fileEx;
	CString str;
	CString FilePathName;
	BOOL berrorfile(FALSE);
	if (lpszFileName == NULL)
		lpszFileName = DefaultProjectName;

	str.Format(_T("%sParameter\\%s"), m_szCurrentFolder, lpszFileName);
	FilePathName.Format(_T("%s\\%s.prj"), str, lpszFileName);
	if (bIn_Out)//
	{
		if (!file.Open(FilePathName, CFile::modeRead, &fileEx))
			return FALSE;
		FilePathName.Format(_T("%s\\Reference.png"), str);
		file.Read(reinterpret_cast<TCHAR*>(&PrjCfg), sizeof(ProjectCfg));
		rcSearch.resize(2);
		rorcSearch.resize(2);
		TempData.resize(2);
		SlineData.resize(2);
		file.Read(reinterpret_cast<TCHAR*>(&rcSearch[0]), 2 * sizeof(Rect));
		file.Read(reinterpret_cast<TCHAR*>(&rorcSearch[0]), 2 * sizeof(Rect));
		file.Read(reinterpret_cast<TCHAR*>(&TempData[0]), 2 * sizeof(TemplateData));
		file.Read(reinterpret_cast<TCHAR*>(&SlineData[0]), 2 * sizeof(SlineData));
		UINT num = PrjCfg.bProductNum;
		if (num)
		{
			ProductSnapShotF.resize(num);
			file.Read(reinterpret_cast<TCHAR*>(&ProductSnapShotF[0]), num*sizeof(MotionPoint));
			ProductSnapShotB.resize(num);
			file.Read(reinterpret_cast<TCHAR*>(&ProductSnapShotB[0]), num*sizeof(MotionPoint));
		}
		else
		{
			berrorfile = TRUE;
			PrjCfg.bProductNum = 2;
			ProductSnapShotF.resize(num);
			ProductSnapShotB.resize(num);

		}
		file.Close();
		if (!PrjCfg.nPaneNum)
		{
			berrorfile = TRUE;
			PrjCfg.nPaneNum = 4;
		}
		DefaultProjectName = lpszFileName;
	}
	else
	{
		BYTE num = PrjCfg.bProductNum;
		if (rcSearch.size() < 2)
			rcSearch.resize(2);
		if (rorcSearch.size() < 2)
			rorcSearch.resize(2);
		if (TempData.size() < 2)
			TempData.resize(2);
		if (SlineData.size() < 2)
			SlineData.resize(2);
		if (!file.Open(FilePathName, CFile::modeCreate | CFile::modeWrite, &fileEx))
			return FALSE;
		file.Write(reinterpret_cast<TCHAR*>(&PrjCfg), sizeof(ProjectCfg));
		file.Write(reinterpret_cast<TCHAR*>(&rcSearch[0]), 2 * sizeof(Rect));
		file.Write(reinterpret_cast<TCHAR*>(&rorcSearch[0]), 2 * sizeof(Rect));
		file.Write(reinterpret_cast<TCHAR*>(&TempData[0]), 2 * sizeof(TemplateData));
		file.Write(reinterpret_cast<TCHAR*>(&SlineData[0]), 2*sizeof(SlineData));
		if (num)
		{
			if (ProductSnapShotF.size() < num)
				ProductSnapShotF.resize(num);
			file.Write(reinterpret_cast<TCHAR*>(&ProductSnapShotF[0]), num*sizeof(MotionPoint));
			if (ProductSnapShotB.size() < num)
				ProductSnapShotB.resize(num);
			file.Write(reinterpret_cast<TCHAR*>(&ProductSnapShotB[0]), num*sizeof(MotionPoint));
		}
		else
			berrorfile = TRUE;
		file.Close();
		DefaultProjectName = lpszFileName;
	}
	LoadLaserPath(lpszFileName,bIn_Out);
	str = DefaultProjectName.Left(2);
	nPrjNum = STR_INT(str);
	if (berrorfile)
		AfxMessageBox(_T("���������δ����Ч�������������ݻ��ļ�"));

	return TRUE;
}

BOOL CMachineParam::LoadLaserPath(LPCTSTR lpszFileName, BOOL bIn_Out /*= TRUE*/)
{
	UINT i(0), j(0);
	CFile file;
	CFileException fileEx;
	CString str;
	CString FilePathName;
	BOOL berrorfile(FALSE);
	if (lpszFileName == NULL)
		lpszFileName = DefaultProjectName;

	str.Format(_T("%sParameter\\%s"), m_szCurrentFolder, DefaultProjectName);
	FilePathName.Format(_T("%s\\%s.pt"), str, lpszFileName);

	LPnode lp;
	ZeroMemory(&lp, sizeof(lp));

	if (bIn_Out)//
	{
		if (!file.Open(FilePathName, CFile::modeRead, &fileEx))
			return FALSE;
		file.Read(reinterpret_cast<TCHAR*>(&j), sizeof (UINT));//
		if (j)
		{
			PtArray.nCount = j < 2 ? 2 : j;
			PtArray.Ctline.resize(PtArray.nCount);
			for (i = 0; i < PtArray.nCount; i++)
			{
				CTrackLine &tl = PtArray.Ctline.at(i);
				tl.Tlline.clear();
				if (i < j)
				{
					file.Read(reinterpret_cast<TCHAR*>(&tl.nCount), sizeof(UINT));
					if (tl.nCount)
					{
						tl.Tlline.resize(tl.nCount);
						file.Read(reinterpret_cast<TCHAR*>(&tl.Tlline[0]), tl.nCount * sizeof(LPnode));
						continue;
					}
				}
				{
					PtArray.Ctline.at(i).nCount = 1;
					PtArray.Ctline.at(i).Tlline.push_back(lp);
				}
			}
		}
		else
			berrorfile = TRUE;
		file.Close();
		if (berrorfile)
		{
			PtArray.Ctline.resize(2);
			PtArray.nCount = 2;
			PtArray.Ctline.at(0).nCount = 1;
			PtArray.Ctline.at(0).Tlline.push_back(lp);
			PtArray.Ctline.at(1).nCount = 1;
			PtArray.Ctline.at(1).Tlline.push_back(lp);
		}
	}
	else
	{
		if (!file.Open(FilePathName, CFile::modeCreate | CFile::modeWrite, &fileEx))
			return FALSE;
		if (PtArray.nCount^PtArray.Ctline.size())
			PtArray.nCount = PtArray.Ctline.size();
		file.Write(reinterpret_cast<TCHAR*>(&PtArray.nCount), sizeof(UINT));
		for (i = 0; i < PtArray.nCount; i++)
		{
			CTrackLine &tl = PtArray.Ctline.at(i);
			if (tl.nCount^tl.Tlline.size())
				tl.nCount = tl.Tlline.size();
			file.Write(reinterpret_cast<TCHAR*>(&tl.nCount), sizeof(UINT));
			file.Write(reinterpret_cast<TCHAR*>(&tl.Tlline[0]), tl.nCount * sizeof(LPnode));
		}
		file.Close();
	}

	return TRUE;
}

BOOL CMachineParam::InitializeContext()
{
	BOOL bSuccess(FALSE);
	CString	pFileName = m_szCurrentFolder + _T("Config.json");//
	CString str;

	//��ȡIO���õȻ�������
	BOOL bExisted = IsFileExisted(pFileName);
	bSuccess = LoadParameter(pFileName, bExisted);
	if (m_cListPrjName.Find(DefaultProjectName) == NULL)
	{
		str.Format(_T("�Ҳ����ϴ���Ŀ�ļ��У�[%s],\n���أ�[00-DEFAULTS]"), DefaultProjectName);
		AfxMessageBox(str);
		DefaultProjectName = _T("00-DEFAULTS");
	}
	if (!LoadPrjData(DefaultProjectName))
	{
		if (!CreateNewPrj(DefaultProjectName))
			return FALSE;
		str.Format(_T("�Ҳ���ָ�������ļ�[%s],����������"), DefaultProjectName);
		AfxMessageBox(str);
	}
	return TRUE;
}

int CMachineParam::ChangePrj(UINT nTarget)
{
	int bChanged(FALSE);
	if (nTarget^nPrjNum)
	{
		bChanged = -1;
		for (int i = 0; i < m_cListPrjName.GetCount(); i++)
		{
			CString sz = m_cListPrjName.GetAt(m_cListPrjName.FindIndex(i));
			CString sz_Num = sz.Left(2);
			if (nTarget == STR_INT(sz_Num))
			{
				if (LoadPrjData(sz))
					bChanged = 1;
				break;
			}
		}
	}
	return bChanged;
}

BOOL CMachineParam::AutoSaveParam()
{
	CFile file;
	CFileException fileEx;
	Json::Reader m_nJsonReader;
	Json::Value root;
	Json::Value StatusItem;
	Json::FastWriter writer;
	int i(0);
	CStringA filestr;
	CStringA str;
	CString	strFileName = m_szCurrentFolder + _T("Config.json");//
	if (!file.Open(strFileName, CFile::modeReadWrite | CFile::typeBinary, &fileEx))
		return FALSE;
	file.Read(filestr.GetBuffer(file.GetLength()), file.GetLength());
	filestr.ReleaseBuffer();
	if (!m_nJsonReader.parse((LPCSTR)filestr, root))
		return FALSE;
	if (root["״̬��Ϣ"].isNull())
	{
		StatusItem["��ǰ����"] = nlQuantity;
		root["״̬��Ϣ"] = StatusItem;
	}
	else
	{
		root["״̬��Ϣ"]["��ǰ����"] = nlQuantity;
	}

	Json::StyledWriter sw;
	filestr = sw.write(root).data();
	UINT n = filestr.GetLength();
	file.SeekToBegin();
	file.Write(filestr, n);
	file.Close();
	return TRUE;
}

