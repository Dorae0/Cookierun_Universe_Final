#include "DoubleBuffering.h"
#include "CCookie.h"

extern int g_nFrame;			// ȭ�� ���� ī��Ʈ
extern int g_nMonIdx;			// 
extern int BossState;

extern SURFACEINFO g_sfStart;
extern SURFACEINFO g_sfStartHUD[10];
extern SURFACEINFO g_sfClear[2];
extern SURFACEINFO g_sfBack;	// ���� ǥ��
extern SURFACEINFO g_sfBG;		// ��� ǥ�� 
extern SURFACEINFO g_sfGround;	// �� ǥ��
extern SURFACEINFO g_sfMenuBG[2];	// �޴� ���
extern SURFACEINFO g_sfCostBar[4];	// �ڽ�Ʈ ��
extern SURFACEINFO g_sfCookieButton[8];	// ��Ű ��ư
extern SURFACEINFO g_sfFont[15];	// ���� ��Ʈ
extern SURFACEINFO g_sfResult[2];		// ���� ǥ��
extern SURFACEINFO g_sfBlack;			// ��ο� ȭ��
extern SURFACEINFO g_sfPowerUps[12];	// �Ŀ��� ��ҵ�

extern OBJ_TOWER	g_objTower;		// Ÿ�� ǥ��
extern OBJ_COOKIE	g_objCookie[4];	// ��Ű �ִϸ��̼�
extern OBJ_MOB		g_objMob;			// �� �ִϸ��̼�
extern OBJ_BOSS		g_objBoss;	// ���� �ִϸ��̼�

extern COOLDOWN	CooldownState[10];
extern POINTS g_ptMouse;		// ���콺 ��ǥ 

//// dcScreen�� ȣȯ�Ǵ� �ĸ�ǥ��DC ����
BOOL __CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo)
{
	psInfo->dcSurface = CreateCompatibleDC(dcScreen);
	psInfo->hBmp = CreateCompatibleBitmap(dcScreen, nWidth, nHeight);
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp);
	PatBlt(psInfo->dcSurface, 0, 0, nWidth, nHeight, PATCOPY);
	psInfo->nWidth = nWidth;
	psInfo->nHeight = nHeight;

	return TRUE;
}

//// ǥ�� ���� �� �ε�
//// ���� ������Ʈ �Ǵ� ��Ʈ�� DC ���� �� ��Ʈ�� �ε�
void __LoadSurface(HDC dcScreen, SURFACEINFO* psInfo)
{
	BITMAP  bit;

	psInfo->dcSurface = CreateCompatibleDC(dcScreen);
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp);

	GetObject(psInfo->hBmp, sizeof(BITMAP), &bit);
	psInfo->nWidth = bit.bmWidth;
	psInfo->nHeight = bit.bmHeight;
}

//// ������ ǥ�� ����
void __ReleaseSurface(SURFACEINFO* psInfo)
{
	SelectObject(psInfo->dcSurface, psInfo->hOldBmp);
	DeleteDC(psInfo->dcSurface);
	DeleteObject(psInfo->hBmp);
}

//// �̹��� ���
BOOL __PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
}

//// �̹��� ���� 
BOOL __PutImageBlend(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int nAlpha)
{
	BLENDFUNCTION bf;

	//// BLENDFUNCTION �ʱ�ȭ
	bf.BlendOp = AC_SRC_OVER;	//
	bf.BlendFlags = 0;				//			
	bf.AlphaFormat = 0;				//
	bf.SourceConstantAlpha = nAlpha; // 0 - 255

	//// ���� ����
	return 	AlphaBlend(dcDst,
		x, y, psInfo->nWidth, psInfo->nHeight,
		psInfo->dcSurface,
		0, 0, psInfo->nWidth, psInfo->nHeight,
		bf);
}

//// ��������Ʈ
BOOL __PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	return TransparentBlt(dcDst,
		x, y, psInfo->nWidth, psInfo->nHeight,
		psInfo->dcSurface,
		0, 0, psInfo->nWidth, psInfo->nHeight,
		psInfo->crColorKey);
}

BOOL __PutStretchSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, float xmag, float ymag)
{
	return TransparentBlt(dcDst,
		x, y, psInfo->nWidth * xmag, psInfo->nHeight * ymag,
		psInfo->dcSurface,
		0, 0, psInfo->nWidth, psInfo->nHeight,
		psInfo->crColorKey);
}

BOOL __PutAlphaStretchSprite(HDC dcDst, HDC AlphaDC, int x, int y, int nBgX, int nBGroundX, SURFACEINFO* psInfo, SURFACEINFO* backInfo, SURFACEINFO* groundInfo, SURFACEINFO* MenuInfo, int nAlpha)
{
	TransparentBlt(AlphaDC,
		x + 1400, y, psInfo->nWidth * 0.25, psInfo->nHeight * 0.25,
		psInfo->dcSurface,
		0, 0, psInfo->nWidth, psInfo->nHeight,
		RGB(0, 255, 255));

	BLENDFUNCTION bf;

	//// BLENDFUNCTION �ʱ�ȭ
	bf.BlendOp = AC_SRC_OVER;	//
	bf.BlendFlags = 0;				//			
	bf.AlphaFormat = 0;				//
	bf.SourceConstantAlpha = nAlpha; // 0 - 255

	return 	AlphaBlend(dcDst,
		x + 1400, y, psInfo->nWidth, psInfo->nHeight,
		AlphaDC,
		x + 1400, y, psInfo->nWidth, psInfo->nHeight,
		bf);
}

//// dcScreen�� ǥ���� ���� ( �ĸ�ǥ�� -> ȭ��DC )
BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo)
{
	BitBlt(dcScreen, 0, 0, g_sfBack.nWidth, g_sfBack.nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
	return TRUE;
}

//// DIB -> DDB�� ��ȯ
HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path)
{
	HANDLE  hFile;
	DWORD   FileSize, dwRead;
	HBITMAP hBit;
	BITMAPFILEHEADER* fh;
	BITMAPINFO* ih;
	BITMAPINFOHEADER* iheader;
	LPBYTE			 lpMemBlock;

	//// ������ ����
	hFile = CreateFile(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);

	lpMemBlock = (LPBYTE)malloc(FileSize);
	fh = (BITMAPFILEHEADER*)lpMemBlock;
	ReadFile(hFile, lpMemBlock, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	ih = (BITMAPINFO*)(lpMemBlock + sizeof(BITMAPFILEHEADER));
	iheader = (BITMAPINFOHEADER*)(lpMemBlock + sizeof(BITMAPFILEHEADER));

	//// DDB�� ��ȯ�Ѵ�.
	hBit = CreateDIBitmap(hdc,						// hdc
		iheader,					// BITMAPINFOHEADER ���
		CBM_INIT,					// 0 �Ǵ� CBM_INIT ( �ʱ�ȭ )
		lpMemBlock + fh->bfOffBits,	// ������ ��巡��
		ih,							// BITMAPINFO ���
		DIB_RGB_COLORS);

	free(lpMemBlock);

	return hBit;
}

//// SURFACEINFO �̹��� �ʱ�ȭ
void __SetImgSurface(SURFACEINFO* psfInfo)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
}

//// SURFACEINFO ��������Ʈ �ʱ�ȭ
void __SetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
	psfInfo->crColorKey = crColorKey;
}

//// �ϰ� ǥ�� ���� 
void __Init(HDC dcScreen)
{
	int i;
	char strName[256];

	CookieInit();
	CooldownInit();
	TowerInit(2000);
	BossInit(2000);

	////
	__CreateBackBuffer(dcScreen, 2800, 609, 32, &g_sfBack);

	// ���� �׸� �ε�
	__SetImgSurface(&g_sfStart);
	g_sfStart.hBmp = __MakeDDBFromDIB(dcScreen, "Drawings\\Map\\StartScreen.bmp");
	__LoadSurface(dcScreen, &g_sfStart);

	// Ŭ���� �׸� �ε�
	__SetImgSurface(&g_sfClear[0]);
	g_sfClear[0].hBmp = __MakeDDBFromDIB(dcScreen, "Drawings\\Map\\ClearScreen.bmp");
	__LoadSurface(dcScreen, &g_sfClear[0]);

	// Ŭ���� ���� �ε�
	__SetImgSurface(&g_sfClear[1]);
	g_sfClear[1].hBmp = __MakeDDBFromDIB(dcScreen, "Drawings\\Map\\GameClear.bmp");
	__LoadSurface(dcScreen, &g_sfClear[1]);

	// ��� �ε�
	__SetImgSurface(&g_sfBG);
	g_sfBG.hBmp = __MakeDDBFromDIB(dcScreen, "Drawings\\Map\\ovenbreak0002_tm006_bg1.bmp");
	__LoadSurface(dcScreen, &g_sfBG);

	// �� �׸� �ε�
	__SetImgSurface(&g_sfGround);
	g_sfGround.hBmp = __MakeDDBFromDIB(dcScreen, "Drawings\\Map\\ovenbreak0002_tm006_fh.bmp");
	__LoadSurface(dcScreen, &g_sfGround);

	// �޴� ��� �ε�
	for (i = 0; i < 2; i++)
	{
		__SetSprSurface(&(g_sfMenuBG[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Map\\kakao5000_tm102_bg%d.bmp", i + 1);
		g_sfMenuBG[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_sfMenuBG[i]));
	}

	// �ڽ�Ʈ �� �ε�
	for (i = 0; i < 4; i++)
	{
		__SetSprSurface(&(g_sfCostBar[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\CostBar\\costbar_%d.bmp", i + 1);
		g_sfCostBar[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_sfCostBar[i]));
	}

	// ��Ű ��ư �ε�
	for (i = 0; i < 8; i++)
	{
		__SetSprSurface(&(g_sfCookieButton[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\CookieButton\\cookiebutton_%d.bmp", i + 1);
		g_sfCookieButton[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_sfCookieButton[i]));
	}

	// ��Ʈ �ε�
	for (i = 0; i < 15; i++)
	{
		__SetSprSurface(&(g_sfFont[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Fonts\\%d.bmp", i);
		g_sfFont[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_sfFont[i]));
	}

	// Ÿ�� �ε�
	for (i = 0; i < 5; i++)
	{
		__SetSprSurface(&(g_objTower.g_sfTowerAni[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Map\\common_platform_flag_end_%d.bmp", i + 1);
		g_objTower.g_sfTowerAni[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_objTower.g_sfTowerAni[i]));
	}

	//��Ű �׸� �ε�
	for (int cookieindex = 0; cookieindex < COOKIETYPE_MAX; cookieindex++)
	{
		for (i = 0; i < g_objCookie[cookieindex].nAniWalkMax; i++)
		{
			__SetSprSurface(&(g_objCookie[cookieindex].g_sfCookieWalk[i]), RGB(0, 255, 255));

			wsprintf(strName, "Drawings\\cookie000%d_bmp\\cookie000%d_walk%d.bmp", cookieindex + 1, cookieindex + 1, i + 1);
			g_objCookie[cookieindex].g_sfCookieWalk[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

			__LoadSurface(dcScreen, &(g_objCookie[cookieindex].g_sfCookieWalk[i]));
		}

		for (i = 0; i < g_objCookie[cookieindex].nAniAttackMax; i++)
		{
			__SetSprSurface(&(g_objCookie[cookieindex].g_sfCookieAttack[i]), RGB(0, 255, 255));

			wsprintf(strName, "Drawings\\cookie000%d_bmp\\cookie000%d_attack%d.bmp", cookieindex + 1, cookieindex + 1, i + 1);
			g_objCookie[cookieindex].g_sfCookieAttack[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

			__LoadSurface(dcScreen, &(g_objCookie[cookieindex].g_sfCookieAttack[i]));
		}

		for (i = 0; i < g_objCookie[cookieindex].nAniFaintMax; i++)
		{
			__SetSprSurface(&(g_objCookie[cookieindex].g_sfCookieFaint[i]), RGB(0, 255, 255));

			wsprintf(strName, "Drawings\\cookie000%d_bmp\\cookie000%d_faint%d.bmp", cookieindex + 1, cookieindex + 1, i + 1);
			g_objCookie[cookieindex].g_sfCookieFaint[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

			__LoadSurface(dcScreen, &(g_objCookie[cookieindex].g_sfCookieFaint[i]));
		}

		for (i = 0; i < g_objCookie[cookieindex].nAniAttackMotionMax; i++)
		{
			__SetSprSurface(&(g_objCookie[cookieindex].g_sfCookieAttackMotion[i]), RGB(0, 255, 255));

			wsprintf(strName, "Drawings\\cookie000%d_bmp\\cookie000%d_attackmotion%d.bmp", cookieindex + 1, cookieindex + 1, i + 1);
			g_objCookie[cookieindex].g_sfCookieAttackMotion[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

			__LoadSurface(dcScreen, &(g_objCookie[cookieindex].g_sfCookieAttackMotion[i]));
		}
	}


	// ���� �׸� �ε�
	for (i = 0; i < 5; i++)
	{
		__SetSprSurface(&(g_objBoss.g_sfBossIdle[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Boss\\raidboss0001_3_boss_idle_%d.bmp", i + 1);
		g_objBoss.g_sfBossIdle[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_objBoss.g_sfBossIdle[i]));
	}

	for (i = 0; i < 8; i++)
	{
		__SetSprSurface(&(g_objBoss.g_sfBossAttack[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Boss\\RaidBoss_0001_attack_%d.bmp", i + 1);
		g_objBoss.g_sfBossAttack[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_objBoss.g_sfBossAttack[i]));
	}

	for (i = 0; i < 13; i++)
	{
		__SetSprSurface(&(g_objBoss.g_sfBossFaint[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Boss\\raidboss0001_2_boss_faint_%d.bmp", i + 1);
		g_objBoss.g_sfBossFaint[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_objBoss.g_sfBossFaint[i]));
	}

	// �� �׸� �ε�
	for (int i = 0; i < 3; i++)
	{
		__SetSprSurface(&(g_objMob.g_sfMobIdle[i]), RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Mobs\\land0002_tm001_bl1_ing_%d.bmp", i + 1);
		g_objMob.g_sfMobIdle[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &(g_objMob.g_sfMobIdle[i]));
	}

	// ���� �ε�
	for (int i = 0; i < 2; i++)
	{
		__SetSprSurface(&g_sfResult[i], RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\WinLose\\WinLose_%d.bmp", i);
		g_sfResult[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &g_sfResult[i]);
	}

	// ��ο� ȭ�� �ε�

	__SetSprSurface(&g_sfBlack, RGB(0, 255, 255));

	wsprintf(strName, "Drawings\\Black.bmp");
	g_sfBlack.hBmp = __MakeDDBFromDIB(dcScreen, strName);

	__LoadSurface(dcScreen, &g_sfBlack);

	// Ÿ��Ʋ ȭ�� ��� �ε�

	for (int i = 0; i < 6; i++)
	{
		__SetSprSurface(&g_sfStartHUD[i], RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\Map\\Title_%d.bmp", i);
		g_sfStartHUD[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &g_sfStartHUD[i]);
	}

	// �Ŀ��� ��� �ε�

	for (int i = 0; i < 12; i++)
	{
		__SetSprSurface(&g_sfPowerUps[i], RGB(0, 255, 255));

		wsprintf(strName, "Drawings\\PowerUPs\\PowerUPs_%d.bmp", i + 1);
		g_sfPowerUps[i].hBmp = __MakeDDBFromDIB(dcScreen, strName);

		__LoadSurface(dcScreen, &g_sfPowerUps[i]);
	}
}

//// �ϰ� ǥ�� �ı�
void __DestroyAll()
{
	int i;

	for (i = 0; i < 4; i++)
		__ReleaseSurface(&(g_objCookie[0].g_sfCookieWalk[i]));
	for (i = 0; i < 6; i++)
		__ReleaseSurface(&(g_objCookie[0].g_sfCookieAttack[i]));
	for (i = 0; i < 9; i++)
		__ReleaseSurface(&(g_objCookie[0].g_sfCookieFaint[i]));

	for (i = 0; i < 5; i++)
		__ReleaseSurface(&(g_objBoss.g_sfBossIdle[i]));
	for (i = 0; i < 8; i++)
		__ReleaseSurface(&(g_objBoss.g_sfBossAttack[i]));
	for (i = 0; i < 13; i++)
		__ReleaseSurface(&(g_objBoss.g_sfBossFaint[i]));

	for (i = 0; i < 5; i++)
		__ReleaseSurface(&(g_objTower.g_sfTowerAni[i]));

	__ReleaseSurface(&g_sfBG);
	__ReleaseSurface(&g_sfBack);
	__ReleaseSurface(&g_sfGround);
}

void CookieInit()
{
	for (int i = COOKIETYPE_BRAVECOOKIE; i < COOKIETYPE_MAX; i++)
	{
		switch (i)
		{
		case COOKIETYPE_BRAVECOOKIE:
			g_objCookie[i].nAniWalkMax = 4;
			g_objCookie[i].nAniAttackMax = 6;
			g_objCookie[i].nAniFaintMax = 9;
			g_objCookie[i].nAniAttackMotionMax = 0;
			break;
		case COOKIETYPE_COTTONCANDYCOOKIE:
			g_objCookie[i].nAniWalkMax = 4;
			g_objCookie[i].nAniAttackMax = 5;
			g_objCookie[i].nAniFaintMax = 7;
			g_objCookie[i].nAniAttackMotionMax = 7;
			break;
		case COOKIETYPE_PISTACHIOCOOKIE:
			g_objCookie[i].nAniWalkMax = 4;
			g_objCookie[i].nAniAttackMax = 4;
			g_objCookie[i].nAniFaintMax = 5;
			g_objCookie[i].nAniAttackMotionMax = 0;
			break;
		case COOKIETYPE_PITAYACOOKIE:
			g_objCookie[i].nAniWalkMax = 4;
			g_objCookie[i].nAniAttackMax = 4;
			g_objCookie[i].nAniFaintMax = 6;
			g_objCookie[i].nAniAttackMotionMax = 0;
			break;
		}
	}
}

void CooldownInit()
{
	for (int i = 0; i < COOKIETYPE_MAX; i++)
	{
		CooldownState[i].CookieButtonState = false;
		CooldownState[i].CookieCooldownState = false;
		CooldownState[i].CookieType = i;
		CooldownState[i].m_nCooldownNow = 0;
	}
	CooldownState[COOKIETYPE_BRAVECOOKIE].m_nCooldown = 3000;
	CooldownState[COOKIETYPE_COTTONCANDYCOOKIE].m_nCooldown = 4000;
	CooldownState[COOKIETYPE_PISTACHIOCOOKIE].m_nCooldown = 4000;
	CooldownState[COOKIETYPE_PITAYACOOKIE].m_nCooldown = 10000;
}

void TowerInit(int nValue)
{
	g_objTower.nLife = nValue;
}

void BossInit(int nValue)
{
	g_objBoss.nLife = nValue;
	BossState = 0;
}