
// TOOLDoc.cpp : CTOOLDoc Ŭ������ ����
//

#include "stdafx.h"
#include "TOOL.h"

#include "TOOLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTOOLDoc

IMPLEMENT_DYNCREATE(CTOOLDoc, CDocument)

BEGIN_MESSAGE_MAP(CTOOLDoc, CDocument)
END_MESSAGE_MAP()


// CTOOLDoc ����/�Ҹ�

CTOOLDoc::CTOOLDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CTOOLDoc::~CTOOLDoc()
{
}

BOOL CTOOLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CTOOLDoc serialization

void CTOOLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CTOOLDoc ����

#ifdef _DEBUG
void CTOOLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTOOLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTOOLDoc ���
