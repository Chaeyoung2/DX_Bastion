#pragma once

#include "Include.h"


class CTOOLView;
class CBackGround
{
private:
	vector<PTILE>	m_vecTile;
	CTOOLView*		m_pMainView;

public:
	vector<PTILE>*	GetTile() { return &m_vecTile; }
	void			SetMainView(CTOOLView* pView) { m_pMainView = pView; }

public:
	HRESULT			Initialize();
	int				Update();
	void			Render();
	void			Release();

public:
	int		GetTileIndex(const D3DXVECTOR3& vPos);
	void	TileChange(const D3DXVECTOR3& vPos, const int& iDrawID, const int& iOption); // DrawID, Option º¯°æ
	void	TileSetting(const D3DXVECTOR3& vPos, const float& fHeight, const float& fMoveSpeed);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	void	MiniMapRender();

public:
	bool	m_iStage;

private:
	bool	m_bRenderOp;
	bool	m_bRenderIndex;
	bool	m_bRenderBor;

public:
	void SetRenderOp(bool bRenderOp) { m_bRenderOp = bRenderOp; }
	void SetRenderIndex(bool bRenderIndex) { m_bRenderIndex = bRenderIndex; }
	void SetRenderBor(bool bRenderBor) { m_bRenderBor = bRenderBor; }

public:
	CBackGround(void);
	~CBackGround(void);
};
