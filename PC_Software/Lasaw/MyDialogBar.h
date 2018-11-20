#pragma once


// CMyDialogBar �Ի���

class CMyDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CMyDialogBar)

public:
	CMyDialogBar();   // ��׼���캯��
	virtual ~CMyDialogBar();
	BOOL						m_bAutoZoom;

// �Ի�������
	enum { IDD = IDD_DIALOGBAR };
	//Ϊ���������Զ�̬�ı��С����������������Ա����  
	CSize m_sizeDocked;
	CSize m_sizeFloating;
	BOOL m_bChangeDockedSize;   // Indicates whether to keep a default size for dockin  
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle,
		UINT nID, BOOL = TRUE);
	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName,
		UINT nStyle, UINT nID, BOOL = TRUE);
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	afx_msg BOOL OnToolTip(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnInitDialog(UINT wParam, LONG lParam); 
	afx_msg void OnBnClickedMenubuttonUser();
	CMenu m_menu;
	CButton m_btTest;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnUpdateBtnUI(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedButtonZoom();
};
