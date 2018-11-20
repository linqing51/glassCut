#pragma once

class CLocusChain 
{
	std::vector<MyNode> OrderLocus;
	std::vector<MyChain> UserOrder;
	void ReBuildUserOrder();
public:
	CLocusChain();
	virtual ~CLocusChain();
	void SetNodeSize(UINT nCont, BYTE nDir = 0);
	UINT GetNodeSize();

	BOOL SetNodeDir(UINT nIndex, BYTE nDir = 1);
	BOOL GetNodeDir(UINT nIndex, BYTE& nDir);
	BOOL GetNodeMark(UINT nIndex, UINT& nMark);
	BOOL SetEntyDir(BYTE nDir = 1);
	BOOL ReverseEntyDir();
	UINT GetEntySize();
	UINT GetChainSize(UINT nIndex);
	BOOL Combine(UINT nNodeDst, UINT nNodeTake);
	const MyNode* GetNode(UINT nChain, UINT nIndex);
};

class CDxfLocus
{
	DWORD Locus_Num;//�켣��
public:

	CDxfLocus();
	virtual ~CDxfLocus();
	BOOL OpenDxf(CString filepath,
		double Proportion,             //CAD��ʵ��ͼ�ͻ����е�ͼ�ı���
		DWORD  Ellipse_Cut_Min_Angel,  //�ָ�鲹��Բ����С�Ƕ�
		DWORD  Spline_Cut_Step         //һ������Ҫ��Ϊ����ֱ�߶�
		);
	void ClosDxf();
	BOOL Analysis(BYTE nDir = 0);//0����1����
	BOOL AnalysisAdv();//

	DWORD GetLocusNum();
	DWORD GetSampleNum(DWORD Locus_Num);
	CRect GetTargetRect();

	DWORD GetType(BYTE nDir,DWORD Locus_Num, DWORD  Sample_Num);

	BOOL GetLine(double *Start_x,    //ֱ����ʼ�������xֵ
		double *Start_y,    //ֱ����ʼ�������yֵ
		double *End_x,      //ֱ����ֹ�������xֵ
		double *End_y,      //ֱ����ֹ�������yֵ
		DWORD   Direction,  //�켣�ķ���
		DWORD   Locus_Num,  //�켣�����к�
		DWORD   Sample_Num);//ͼԪ�����к�
	BOOL GetArc(double * Center_x,    //Բ��Բ�������xֵ
		double * Center_y,    //Բ��Բ�������yֵ
		double * r,           //Բ���İ뾶
		double * Start_X,     //Բ������ʼ���X����
		double * Start_Y,     //Բ������ʼ���Y����Ƕ�
		double * End_X,       //Բ������ֹ���X����
		double * End_Y,       //Բ������ֹ���Y����
		DWORD  * MoveDirection, //�˶�����
		double * Start_Angle, //Բ������ʼ�Ƕ�
		double * End_Angle,   //Բ������ֹ�Ƕ�
		DWORD    Direction,  //�켣�ķ���
		DWORD    Locus_Num,   //�켣�����к�
		DWORD    Sample_Num); //ͼԪ�����к�

	BOOL m_bIsOpen;
	CRect AreaRc;
	CString m_strFile;
	CContourTrack m_ctt;
	CLocusChain	m_LocusChain;
protected:
	void InitializeContext();
	CString ErrorInfo(UINT lErrorID);

};

