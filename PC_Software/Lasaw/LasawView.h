
// LasawView.h : CLasawView ��Ľӿ�
//

#pragma once


class CLasawView : public CView
{
protected: // �������л�����
	CLasawView();
	DECLARE_DYNCREATE(CLasawView)

// ����
public:
	CLasawDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CLasawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LasawView.cpp �еĵ��԰汾
inline CLasawDoc* CLasawView::GetDocument() const
   { return reinterpret_cast<CLasawDoc*>(m_pDocument); }
#endif

