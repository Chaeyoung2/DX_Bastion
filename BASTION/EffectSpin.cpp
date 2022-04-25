#include "StdAfx.h"
#include "EffectSpin.h"

CEffectSpin::CEffectSpin(void)
{
}

CEffectSpin::~CEffectSpin(void)
{
}

HRESULT CEffectSpin::Initialize(void)
{
	m_eRenderID = RENDER_EFFECT;

	return S_OK;
}

eOBJECT_RESULT CEffectSpin::Update(void)
{
	return OR_OK;
}

void CEffectSpin::Render(void)
{

}

void CEffectSpin::Release(void)
{

}

void CEffectSpin::FrameCheck(void)
{

}

void CEffectSpin::MatrixCheck(void)
{

}
