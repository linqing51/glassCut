
// LasawView.cpp : CLasawView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Lasaw.h"
#endif

#include "LasawDoc.h"
#include "LasawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLasawView

IMPLEMENT_DYNCREATE(CLasawView, CView)

BEGIN_MESSAGE_MAP(CLasawView, CView)
END_MESSAGE_MAP()

// CLasawView ����/����

CLasawView::CLasawView()
{
	// TODO:  �ڴ˴���ӹ������

}

CLasawView::~CLasawView()
{
}

BOOL CLasawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CLasawView ����

void CLasawView::OnDraw(CDC* /*pDC*/)
{
	CLasawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CLasawView ���

#ifdef _DEBUG
void CLasawView::AssertValid() const
{
	CView::AssertValid();
}

void CLasawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLasawDoc* CLasawView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLasawDoc)));
	return (CLasawDoc*)m_pDocument;
}
#endif //_DEBUG


// CLasawView ��Ϣ�������
