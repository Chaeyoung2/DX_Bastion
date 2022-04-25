
// TOOLDoc.cpp : CTOOLDoc 클래스의 구현
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


// CTOOLDoc 생성/소멸

CTOOLDoc::CTOOLDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTOOLDoc::~CTOOLDoc()
{
}

BOOL CTOOLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTOOLDoc serialization

void CTOOLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTOOLDoc 진단

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


// CTOOLDoc 명령
