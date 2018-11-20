// MiddleSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "MiddleSplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiddleSplitter

IMPLEMENT_DYNCREATE(CMiddleSplitter, CSplitterWnd)

CMiddleSplitter::CMiddleSplitter()
{
	m_cxSplitter = 4;    //must >=4 ,�϶�ʱ�϶����Ŀ��
	m_cySplitter = 4;    
	m_cxBorderShare = 0; //�������ʱ�϶�����ƫ����
	m_cyBorderShare = 0;  
	m_cxSplitterGap= 1;  //splitter�϶����Ŀ��   
	m_cySplitterGap= 1;  
}

CMiddleSplitter::~CMiddleSplitter()
{
}


BEGIN_MESSAGE_MAP(CMiddleSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CMiddleSplitter)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiddleSplitter drawing

void CMiddleSplitter::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMiddleSplitter diagnostics

#ifdef _DEBUG
void CMiddleSplitter::AssertValid() const
{
	CSplitterWnd::AssertValid();
}

void CMiddleSplitter::Dump(CDumpContext& dc) const
{
	CSplitterWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMiddleSplitter message handlers

void CMiddleSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	if(point.y<250||point.y>500)
		return;
//	CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CMiddleSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	if(point.y<250||point.y>500)
		return;
//	CSplitterWnd::OnMouseMove(nFlags, point);
}


BOOL CMiddleSplitter::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	CBrush backBrush(RGB(165,204,211)); 
// 	CBrush* pOldBrush = pDC->SelectObject(&backBrush); 
// 	CRect rect; 
// 	pDC->GetClipBox(&rect);  
// 	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY); 
// 	pDC->SelectObject(pOldBrush); 
// 	backBrush.DeleteObject();
// 	DeleteObject(pOldBrush);
// 	return TRUE;
	return CSplitterWnd::OnEraseBkgnd(pDC);
}
