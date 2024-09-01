#pragma once
#ifndef __DOUBLEBUFFERING_H__
#define __DOUBLEBUFFERING_H__

#include <windows.h>

typedef struct SURFACEINFOtag
{
	HDC			dcSurface;		// 비트맵이 적재되있는 표면 DC
	HBITMAP		hBmp;			// 비트맵 핸들
	HBITMAP		hOldBmp;		// 이전 비트맵 핸들 ( dc 해제시 참조 )
	int			nWidth;
	int			nHeight;
	COLORREF	crColorKey;		// 컬러키
} SURFACEINFO;

typedef struct OBJ_COOKIEtag
{
	SURFACEINFO	g_sfCookieWalk[32];
	SURFACEINFO g_sfCookieAttack[32];
	SURFACEINFO g_sfCookieFaint[32];
	SURFACEINFO g_sfCookieAttackMotion[32];
	int			nAniWalk;
	int			nAniAttack;
	int			nAniFaint;
	int			nAniAttackMotion;
	int			nAniWalkMax;
	int			nAniAttackMax;
	int			nAniFaintMax;
	int			nAniAttackMotionMax;
} OBJ_COOKIE;

typedef struct OBJ_BOSStag
{
	SURFACEINFO	g_sfBossIdle[5];
	SURFACEINFO g_sfBossAttack[8];
	SURFACEINFO g_sfBossFaint[13];
	int			nAniIdle;
	int			nAniAttack;
	int			nAniFaint;
	int			nLife;
} OBJ_BOSS;

typedef struct OBJ_MOBtag
{
	SURFACEINFO	g_sfMobIdle[3];
	int			nAniIdle;
	int			nLife;
	int			nStepX;
	int			nStepY;
} OBJ_MOB;

typedef struct OBJ_TOWERtag
{
	SURFACEINFO	g_sfTowerAni[5];
	int			nAni;
	int			nLife;
} OBJ_TOWER;

typedef struct COOLDOWNtag
{
	BOOL		CookieButtonState;	
	BOOL		CookieCooldownState;
	int			m_nCooldown;
	int			m_nCooldownNow;
	int			CookieType;
} COOLDOWN;

BOOL __CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo);
void __LoadSurface(HDC dcScreen, SURFACEINFO* psInfo);
void __ReleaseSurface(SURFACEINFO* psInfo);
BOOL __PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo);
BOOL __PutImageBlend(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int nAlpha);
BOOL __PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo);
BOOL __PutStretchSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, float xmag, float ymag);
BOOL __PutAlphaStretchSprite(HDC dcDst, HDC AlphaDC, int x, int y, int nBgX, int nBGroundX, SURFACEINFO* psInfo, SURFACEINFO* backInfo, SURFACEINFO* groundInfo, SURFACEINFO* MenuInfo, int nAlpha);
BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo);
HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path);
void __SetImgSurface(SURFACEINFO* psfInfo);
void __SetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey);
void __Init(HDC dcScreen);
void __DestroyAll();
void CookieInit();
void CooldownInit();
void TowerInit(int nValue);
void BossInit(int nValue);

#endif