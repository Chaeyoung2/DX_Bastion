#include "StdAfx.h"
#include "UI.h"

CUI::CUI(void)
: m_pObserver(NULL), m_pDataSubject(CDataSubject::GetInstance())
{
}

CUI::~CUI(void)
{
}
