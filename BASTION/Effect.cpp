#include "StdAfx.h"
#include "Effect.h"

CEffect::CEffect(void)
:m_pObjMgr(CObjMgr::GetInstance())
{
}

CEffect::~CEffect(void)
{
}

HRESULT CEffect::Initialize(void)
{
	return S_OK;
}

eOBJECT_RESULT CEffect::Update(void)
{
	return OR_OK;
}

void CEffect::Render(void)
{

}

void CEffect::Release(void)
{

}
