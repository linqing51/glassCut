
// LasawDoc.cpp : CLasawDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Lasaw.h"
#endif
#include "MainFrm.h"

#include "LasawDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLasawDoc

IMPLEMENT_DYNCREATE(CLasawDoc, CDocument)

BEGIN_MESSAGE_MAP(CLasawDoc, CDocument)
	ON_COMMAND(ID_PROJECT_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)

	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)

	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CLasawDoc::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CLasawDoc::OnUpdateFileOpen)
END_MESSAGE_MAP()


// CLasawDoc ����/����

CLasawDoc::CLasawDoc()
{
	// TODO:  �ڴ����һ���Թ������

}

CLasawDoc::~CLasawDoc()
{
}

BOOL CLasawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CLasawDoc ���л�

void CLasawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CLasawDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CLasawDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CLasawDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLasawDoc ���

#ifdef _DEBUG
void CLasawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLasawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLasawDoc ����
void CLasawDoc::OnFileSave()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->StartTemplateRW(FALSE);

}

void CLasawDoc::OnFileSaveAs()
{
	theApp.OpenFileDlg(FALSE);
}

void CLasawDoc::OnFileOpen()
{
	theApp.OpenFileDlg();
}

void CLasawDoc::OnFileNew()
{
	::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_USER_UPDATEUI, 4, 1);

}



void CLasawDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
// 	pCmdUI->Enable(theApp.m_nCurrentCameraID);
}


void CLasawDoc::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
// 	pCmdUI->Enable(theApp.m_nCurrentCameraID);

}
