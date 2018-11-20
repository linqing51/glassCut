#include "stdafx.h"
#include "DxfLocus.h"
#include "AdvDXFReader.h"

CLocusChain::CLocusChain()
{

}

CLocusChain::~CLocusChain()
{

}

BOOL CLocusChain::SetNodeDir(UINT nIndex, BYTE nDir /*= 1*/)
{
	if (nIndex < OrderLocus.size())
	{
		OrderLocus.at(nIndex).nDir = nDir;
		return TRUE;
	}
	return FALSE;
}

BOOL CLocusChain::GetNodeDir(UINT nIndex, BYTE& nDir)
{
	if (nIndex < OrderLocus.size())
	{
		nDir = OrderLocus.at(nIndex).nDir;
		return TRUE;
	}
	return FALSE;
}

BOOL CLocusChain::GetNodeMark(UINT nIndex, UINT& nMark)
{
	if (nIndex < OrderLocus.size())
	{
		nMark = OrderLocus.at(nIndex).nMark;
		return TRUE;
	}
	return FALSE;
}

BOOL CLocusChain::SetEntyDir(BYTE nDir /*= 1*/)
{
	for (UINT i = 0; i < OrderLocus.size(); i++)
		OrderLocus.at(i).nDir = nDir;
	return TRUE;
}

BOOL CLocusChain::ReverseEntyDir()
{
	for (UINT i = 0; i < OrderLocus.size(); i++)
		OrderLocus.at(i).nDir = !OrderLocus.at(i).nDir;
	return TRUE;
}

BOOL CLocusChain::Combine(UINT nNodeDst, UINT nNodeTake)
{
	UINT num = OrderLocus.size();
	if (nNodeDst<num&&nNodeTake<num&&nNodeDst^nNodeTake)
	{
		BOOL bFinfTake(FALSE);
		for (int i = 0;i < UserOrder.size();i++)
		{
			for (UINT j = 0;j < UserOrder.at(i).OrderMyNode.size(); j++)
			{
				if (!bFinfTake&&UserOrder.at(i).OrderMyNode.at(j)->nIndex == nNodeTake)
				{
					if (UserOrder.at(i).OrderMyNode.size()==1)
						UserOrder.erase(UserOrder.begin() + i);
					else
						UserOrder.at(i).OrderMyNode.erase(UserOrder.at(i).OrderMyNode.begin() + j);
					bFinfTake = TRUE;
					i = -1;
					break;
				}
				if (bFinfTake&&UserOrder.at(i).OrderMyNode.at(j)->nIndex == nNodeDst)
				{
					OrderLocus.at(nNodeTake).nMark = i;
					if (UserOrder.at(i).OrderMyNode.size() == (j + 1))
						UserOrder.at(i).OrderMyNode.push_back(&OrderLocus.at(nNodeTake));
					else
						UserOrder.at(i).OrderMyNode.insert(UserOrder.at(i).OrderMyNode.begin() + j+1, &OrderLocus.at(nNodeTake));
					return TRUE;
					break;
				}
			}
			
		}
	}
	return FALSE;
}

void CLocusChain::SetNodeSize(UINT nCont, BYTE nDir)
{
	OrderLocus.resize(nCont);
	OrderLocus.shrink_to_fit();
	UserOrder.resize(nCont);
	for (UINT i = 0; i < nCont; i++)
	{
		OrderLocus.at(i).nIndex = i;
		OrderLocus.at(i).nMark = i;
		OrderLocus.at(i).nDir = nDir;

		UserOrder.at(i).nIndex = i;
		UserOrder.at(i).nDir = nDir;
		UserOrder.at(i).OrderMyNode.resize(1);
		UserOrder.at(i).OrderMyNode.at(0) = &OrderLocus.at(i);
	}
}

UINT CLocusChain::GetNodeSize()
{
	return OrderLocus.size();
}

void CLocusChain::ReBuildUserOrder()
{
	UINT num = OrderLocus.size();
	UserOrder.resize(num);
	for (UINT i = 0; i < num; i++)
	{
		UserOrder.at(i).nIndex = i;
		UserOrder.at(i).nDir = 0;
		UserOrder.at(i).OrderMyNode.resize(1);
		UserOrder.at(i).OrderMyNode.at(1) = &OrderLocus.at(i);
	}
}

UINT CLocusChain::GetEntySize()
{
	return UserOrder.size();
}

UINT CLocusChain::GetChainSize(UINT nIndex)
{
	if (nIndex < UserOrder.size())
		return UserOrder.at(nIndex).OrderMyNode.size();
	return 0;
}

const MyNode* CLocusChain::GetNode(UINT nChain, UINT nIndex)
{
	return UserOrder.at(nChain).OrderMyNode.at(nIndex);
}

CDxfLocus::CDxfLocus()
{
	InitializeContext();
}


CDxfLocus::~CDxfLocus()
{
	InitializeContext();
	ClosDxf();
}

void CDxfLocus::ClosDxf()
{
	AtCloseDXF();
}

CString CDxfLocus::ErrorInfo(UINT lErrorID)
{
	CString ErrorInfo[13] = { _T("û�д���"),
		_T("·��Ϊ�մ���"),
		_T("�ļ����ʹ���"),
		_T("�ļ�Ϊ�մ���"),//0x03
		_T("ͼ�����кų�����Χ����"),//0x04
		_T("ͼ�����ʹ���"), //0x05
		_T("���ݶ�ʧ����"),//0x06
		_T("�������ݿռ�ʧ��"),//0x07��mallocʧ�ܣ�
		_T("����켣��Ŀ����"),//0x08
		_T("�켣�������"),//0x09
		_T("����ͼԪ��Ŀ����"),//0x0a
		_T("ͼԪ�������"),//0x0b
		_T("�����������OpenDxf�����ľ��Ȳ�����"),//0x0c

	};
	if (lErrorID < 13)
	{
		return ErrorInfo[lErrorID];
	}
	return _T("��dxf��δ֪����");
}

void CDxfLocus::InitializeContext()
{
	UINT i(0);
	m_ctt.nCount = 0;
	m_bIsOpen = FALSE;
	Locus_Num = 0;
}

BOOL CDxfLocus::OpenDxf(CString filepath,
	double Proportion, //CAD��ʵ��ͼ�ͻ����е�ͼ�ı���
	DWORD  Ellipse_Cut_Min_Angel,  //�ָ�鲹��Բ����С�Ƕ�
	DWORD  Spline_Cut_Step         //һ������Ҫ��Ϊ����ֱ�߶�
	)
{
	int nResult(0xff);
	CString str ;
	char* CfgFile;
	double pt[10] = { 0.0 };
#ifdef UNICODE
	USES_CONVERSION;
	CfgFile = T2A(filepath);
#else
	CfgFile = path;
#endif
	AtCloseDXF();

	InitializeContext();
	try
	{
		nResult = AtOpenDXF(CfgFile, Proportion, Ellipse_Cut_Min_Angel, Spline_Cut_Step);
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	if (nResult==SUCCESS)
	{
		nResult = AtGetLocusCount(&Locus_Num);
		m_LocusChain.SetNodeSize(Locus_Num);

		m_strFile = filepath;
		m_bIsOpen = TRUE;

	}
	else
	{
		AfxMessageBox(ErrorInfo(nResult));
	}
	return m_bIsOpen;
}

BOOL CDxfLocus::Analysis(BYTE nDir)
{
	int nResult(0);
	double pt[12] = { 0.0 };
	DWORD Sample_Num;//��ǰ�켣ͼԪ��
	if (!Locus_Num||!m_bIsOpen)
		return FALSE;
	for (UINT i = 0; i < m_ctt.Ctline.size(); i++)
		m_ctt.Ctline.at(i).Tlline.clear();
	m_ctt.Ctline.clear();
	m_ctt.Ctline.reserve(Locus_Num);

	for (UINT i = 0; i < Locus_Num&&!nResult; i++)
	{
		CTrackLine tl;
		Sample_Num = 0;
		nResult=AtGetSampleCount(&Sample_Num, i + 1);

		tl.Tlline.reserve(Sample_Num + 1);
		for (UINT j = 0; j < Sample_Num&&!nResult;j++)//һ�ι켣�Ķ������
		{
			DWORD nType(0);
			DWORD MoveDirection(0);
			DWORD nPlane(0);
			LPnode lpn;
			ZeroMemory(&lpn, sizeof(LPnode));
			nResult = AtGetType(&nType, nDir, i + 1, j + 1);
			switch (nType)
			{
			case 0://��
				nResult = AtGetLine(&pt[0], &pt[1], &pt[2], &pt[3], &pt[4], &pt[5], &pt[6], nDir, i+1,j+ 1);
				if (nResult == SUCCESS)
				{
					lpn.uType = 1;
					if (!j)
					{
						lpn.bLaserOn = FALSE;
						lpn.EndPoint[0] = pt[0];
						lpn.EndPoint[1] = pt[1];
						lpn.EndPoint[2] = pt[2];
						tl.Tlline.push_back(lpn);
					}
					lpn.bLaserOn = TRUE;
					lpn.EndPoint[0] = pt[3];
					lpn.EndPoint[1] = pt[4];
					lpn.EndPoint[2] = pt[5];
					tl.Tlline.push_back(lpn);

				}else
				{
					CLogHelper::WriteLog(ErrorInfo(nResult));
				}
				break;
			case 2://Բ��
				nResult = AtGetArc(&pt[0], &pt[1], &pt[2],&pt[3],
					&pt[4], &pt[5], &pt[6],
					&pt[7], &pt[8], &pt[9],
					&MoveDirection, &pt[10], &pt[11],
					&nPlane, nDir, i+1, j+1);
				if (nResult == SUCCESS)
				{
					if (!j)
					{
						lpn.uType = 1;
						lpn.bLaserOn = FALSE;
						lpn.EndPoint[0] = pt[4];
						lpn.EndPoint[1] = pt[5];
						lpn.EndPoint[2] = pt[6];
						tl.Tlline.push_back(lpn);
					}
					lpn.uType = 2;
					lpn.bLaserOn = TRUE;
					lpn.EndPoint[0] = pt[7];
					lpn.EndPoint[1] = pt[8];
					lpn.EndPoint[2] = pt[9];

					lpn.CenterPt[0] = pt[0] - pt[4];
					lpn.CenterPt[1] = pt[1] - pt[5];
					lpn.CenterPt[2] = pt[2] - pt[6];
					lpn.nDir = MoveDirection;
					tl.Tlline.push_back(lpn);		
				}
				else
				{
					CLogHelper::WriteLog(ErrorInfo(nResult));
				}
				break;
			case 6://��
			default:
				CLogHelper::WriteLog(_T("�����ݲ�֧�ֵ�ͼԪ���ͣ��Ѻ���"));
				break;
			}
		}
		tl.Tlline.shrink_to_fit();
		tl.nCount = tl.Tlline.size();
		m_ctt.Ctline.push_back(tl);
	}
	m_ctt.Ctline.shrink_to_fit();
	m_ctt.nCount = m_ctt.Ctline.size();
	return !nResult;
}

BOOL CDxfLocus::AnalysisAdv()
{
	int nResult(0);
	CString str;
	double pt[12] = { 0.0 };
	if (!Locus_Num || !m_bIsOpen)
		return FALSE;
	for (UINT i = 0; i < m_ctt.Ctline.size(); i++)
		m_ctt.Ctline.at(i).Tlline.clear();
	m_ctt.Ctline.clear();
	m_ctt.Ctline.reserve(Locus_Num);

	for (UINT chain_num = 0; chain_num < m_LocusChain.GetEntySize(); chain_num++)
	{
		CTrackLine tl;
		for (UINT node_num = 0; node_num < m_LocusChain.GetChainSize(chain_num); node_num++)
		{
			const MyNode* pNode = m_LocusChain.GetNode(chain_num, node_num);
			UINT i = pNode->nIndex;
			BYTE nDir = pNode->nDir;
			DWORD Sample_Num = GetSampleNum(i);//��ǰ�켣ͼԪ��
			tl.Tlline.reserve(tl.Tlline.size()+ Sample_Num);
			for (UINT j = 0; j < Sample_Num; j++)
			{
				DWORD MoveDirection(0);
				DWORD nPlane(0);
				LPnode lpn;
				ZeroMemory(&lpn, sizeof(LPnode));

				BYTE bType = GetType(nDir, i, j);
				str.Format(_T("�켣��%d�����䣺%d�����ͣ�%d\n"), i + 1, j + 1, bType);
				OutputDebugString(str);
				switch (bType)
				{
				case 0:
					nResult = AtGetLine(&pt[0], &pt[1], &pt[2], &pt[3], &pt[4], &pt[5], &pt[6], nDir, i + 1, j + 1);
					if (nResult == SUCCESS)
					{
						lpn.uType = 1;
						if (!node_num&&!j)
						{
							lpn.bLaserOn = FALSE;
							lpn.EndPoint[0] = pt[0];
							lpn.EndPoint[1] = pt[1];
							lpn.EndPoint[2] = pt[2];
							tl.Tlline.push_back(lpn);
						}
						lpn.bLaserOn = TRUE;
						lpn.EndPoint[0] = pt[3];
						lpn.EndPoint[1] = pt[4];
						lpn.EndPoint[2] = pt[5];
						tl.Tlline.push_back(lpn);

					}
					else
					{
						CLogHelper::WriteLog(ErrorInfo(nResult));
					}
					break;
				case 2:
					nResult = AtGetArc(&pt[0], &pt[1], &pt[2], &pt[3],
						&pt[4], &pt[5], &pt[6],
						&pt[7], &pt[8], &pt[9],
						&MoveDirection, &pt[10], &pt[11],
						&nPlane, nDir, i + 1, j + 1);
					if (nResult == SUCCESS)
					{
						if (!node_num&&!j)
						{
							lpn.uType = 1;
							lpn.bLaserOn = FALSE;
							lpn.EndPoint[0] = pt[4];
							lpn.EndPoint[1] = pt[5];
							lpn.EndPoint[2] = pt[6];
							tl.Tlline.push_back(lpn);
						}
						lpn.uType = 2;
						lpn.bLaserOn = TRUE;
						lpn.EndPoint[0] = pt[7];
						lpn.EndPoint[1] = pt[8];
						lpn.EndPoint[2] = pt[9];

						lpn.CenterPt[0] = pt[0] - pt[4];
						lpn.CenterPt[1] = pt[1] - pt[5];
						lpn.CenterPt[2] = pt[2] - pt[6];
						lpn.nDir = MoveDirection;
						tl.Tlline.push_back(lpn);
					}
					else
					{
						CLogHelper::WriteLog(ErrorInfo(nResult));
					}
					break;
				case 6://��
				default:
					CLogHelper::WriteLog(_T("�����ݲ�֧�ֵ�ͼԪ���ͣ��Ѻ���"));
					break;
				}
			}
		}
		tl.Tlline.shrink_to_fit();
		tl.nCount = tl.Tlline.size();
		m_ctt.Ctline.push_back(tl);
	}
	m_ctt.Ctline.shrink_to_fit();
	m_ctt.nCount = m_ctt.Ctline.size();
	return !nResult;
}

BOOL CDxfLocus::GetLine(double *Start_x, /*ֱ����ʼ�������xֵ */ double *Start_y, /*ֱ����ʼ�������yֵ */ double *End_x, /*ֱ����ֹ�������xֵ */ double *End_y, /*ֱ����ֹ�������yֵ */ DWORD Direction, /*�켣�ķ��� */ DWORD Locus_Num, /*�켣�����к� */ DWORD Sample_Num)
{
	int nResult(0xff);
	double pt[3] = { 0.0 };
	if (m_bIsOpen)
	{
		nResult = AtGetLine(Start_x, Start_y, &pt[0],
			End_x, End_y, &pt[1], &pt[2],
			Direction, Locus_Num + 1, Sample_Num + 1);
	}
	return !nResult;
}

BOOL CDxfLocus::GetArc(double * Center_x, /*Բ��Բ�������xֵ */ double * Center_y, /*Բ��Բ�������yֵ */ double * r, /*Բ���İ뾶 */ double * Start_X, /*Բ������ʼ���X���� */ double * Start_Y, /*Բ������ʼ���Y����Ƕ� */ double * End_X, /*Բ������ֹ���X���� */ double * End_Y, /*Բ������ֹ���Y���� */ DWORD * MoveDirection, /*�˶����� */ double * Start_Angle, /*Բ������ʼ�Ƕ� */ double * End_Angle, /*Բ������ֹ�Ƕ� */ DWORD Direction, /*�켣�ķ��� */ DWORD Locus_Num, /*�켣�����к� */ DWORD Sample_Num)
{
	int nResult(0xff);
	double pt[3] = { 0.0 };
	DWORD nPlane(0);
	if (m_bIsOpen)
	{
		nResult = AtGetArc(Center_x, Center_y, &pt[0], r,
			Start_X, Start_Y, &pt[1],
			End_X, End_Y, &pt[2],
			MoveDirection, Start_Angle, End_Angle,
			&nPlane, Direction, Locus_Num + 1, Sample_Num + 1);
	}
	return !nResult;
}

DWORD CDxfLocus::GetLocusNum()
{
	if (AtGetLocusCount(&Locus_Num))
		Locus_Num = 0;
	return Locus_Num;
}

DWORD CDxfLocus::GetSampleNum(DWORD Locus_Num)
{
	DWORD Sample_Num(0);
	if (AtGetSampleCount(&Sample_Num, Locus_Num + 1))
		Sample_Num = 0;
	return Sample_Num;
}

DWORD CDxfLocus::GetType(BYTE nDir, DWORD Locus_Num, DWORD Sample_Num)
{
	DWORD nType(0xff);
	if (AtGetType(&nType, nDir, Locus_Num + 1, Sample_Num + 1))
		nType = 0xff;
	return nType;
}

CRect CDxfLocus::GetTargetRect()
{
	double pt[6] = { 0.0 };
	AreaRc.SetRectEmpty();
	if (m_bIsOpen)
	{
		AtGetMostValue(&pt[0], &pt[1], &pt[2], &pt[3], &pt[4], &pt[5]);
		AreaRc.top = pt[1];
		AreaRc.left = pt[0];
		AreaRc.right = pt[3];
		AreaRc.bottom = pt[4];
	}
	return AreaRc;
}

