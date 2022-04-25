#pragma once

#include "Enum.h"

extern HWND			g_hWnd;

extern eViewID		g_eViewID;
extern bool			g_IsMainView;
extern eSceneType	g_eScene;

extern bool			g_bIsFadeIn;
extern bool			g_bIsFadeOut;
extern bool			g_bIsZoomIn;
extern bool			g_bIsZoomOut;
extern bool			g_bIsMenuToStage1;
extern bool			g_bIsUIRender;
extern bool			g_bIsShaking;
extern int			g_iImageCount;


const int WINCX = 1280;
const int WINCY = 720;

// const int WINCX = 1000;
// const int WINCY = 600;

const WORD MIN_STR    = 64;
const WORD MIDDLE_STR = 128;
const WORD MAX_STR	  = 256;

const int TILECX = 120;
const int TILECY = 66;

const int TILEX = 45;
const int TILEY = 95;

const int TILEXDIA = 25;
const int TILEYDIA = 60;

const int TILEX2 = 45;
const int TILEY2 = 95;
// ¸Ê
const int STAGE1CX = 5400;
const int STAGE1CY = 3135;


// const DWORD ITEM_WEAPON = 0x00000001;
// const DWORD ITEM_ARMOR  = 0x00000002;
// const DWORD ITEM_POTION = 0x00000004;

// À¯´Ö
const int PLAYERCX	= 500;
const int PLAYERCY	= 500;
const int PLAYERCX_ORI = 92;
const int PLAYERCY_ORI = 120;
const int GASCX		= 70;
const int GASCY		= 144;
const int DEFENSECX	= 130;
const int DEFENSECY	= 180;
const int BOSSCX	=	500;
const int BOSSCY	=	400;
