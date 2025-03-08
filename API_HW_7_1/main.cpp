#include <windows.h>
#include "DoubleBuffering.h"
#include "CookieMove.h"
#include "CCookie.h"
#include "CBraveCookie.h"
#include "CCottoncandyCookie.h"
#include "CPistachioCookie.h"
#include "CMob.h"
#include "CYellowBee.h"
#include "CPitayaCookie.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID    CALLBACK MainLoopProc(HWND, UINT, UINT, DWORD);

HINSTANCE g_hInst;
HWND hWndMain;
LPSTR lpszClass = "[ CookieRun : Universe ]";

#define ID_TM_MAINLOOP	1
#define ID_TM_ANIMATION 2

int GameState = 0;				// 게임 화면
int EnterState = 0;
int NumState = 0;
int EnterCooldown = 0;

int g_nFrame = 0;				// 화면 갱신 카운트
int g_nMonIdx = 0;				// 
int CookieCount = 0;			// 쿠키 갯수
int MobCount = 0, MobCooldownNow, MobCooldown = 5000;

int GroundY = 168;
int x, y = GroundY, Vy, Ay;		// 쿠키 좌표
int Cstate;						// 쿠키 상태
int BossState;					// 보스 상태
int TowerState;					// 타워 상태
int Cost;
int CookieVanguard = -1, MobVanguard = -1;

int GameRound = 1;				// 게임 라운드
int PowerUpStack[4];			// 파워업 스택

SURFACEINFO g_sfBack;			// 버퍼 표면
SURFACEINFO g_sfStart;			// 시작 화면
SURFACEINFO g_sfStartHUD[10];	// 시작 화면 요소들
SURFACEINFO g_sfBG;				// 배경 표면 
SURFACEINFO g_sfGround;			// 땅 표면
SURFACEINFO g_sfMenuBG[2];		// 메뉴 배경
SURFACEINFO g_sfCostBar[4];		// 코스트 바
SURFACEINFO g_sfCookieButton[8];	// 쿠키 버튼
SURFACEINFO g_sfFont[15];		// 숫자 폰트
SURFACEINFO g_sfResult[2];		// 승패 표시
SURFACEINFO g_sfBlack;			// 어두운 화면
SURFACEINFO g_sfPowerUps[12];	// 파워업 요소들

OBJ_TOWER	g_objTower;			// 아군 타워
OBJ_COOKIE	g_objCookie[4];		// 쿠키 애니메이션
OBJ_MOB		g_objMob;			// 몹 애니메이션
OBJ_BOSS	g_objBoss;		// 보스 애니메이션

CCookie*	Cookies[101];		// 쿠키 할당할 배열
CMob*		Mobs[101];			// 몹 할당할 배열	

POINTS		g_ptMouse;			// 마우스 좌표 

BOOL		MobCooldownState = false;

COOLDOWN	CooldownState[10];	// 쿨타임 관련 변수들

enum BossState
{
	BSTATE_IDLE,
	BSTATE_FAINT,
	BSTATE_MAX
};

enum GameState
{
	GSTATE_MAIN,
	GSTATE_GAME,
	GSTATE_GAMERULE,
	GSTATE_GAMECOUNT,
	GSTATE_WIN,
	GSTATE_LOSE,
	GSTATE_OPTIONSELECT,
	GSTATE_MAX
};

void __sleep(int iMilliseconds) { int iStart = GetTickCount64();	while ((int)(GetTickCount64() - iStart) < iMilliseconds) {} }
void PutStart();
void PutBackGround();
void PutCostBar();
void PutCookieButton();
void PutCookie(HWND hWnd, int nBgX, int nBGroundX);
void PutMob();
void PutBoss();
void PutTower();
void CheckCookieVanguard();
void CheckMobVanguard();
void WinLose();
void RoundInit();
void RoundReset();
void PutPowerUPs();
void PutPowerUPsCenter();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_SYSMENU | WS_CAPTION, //WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 915, 645,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (TRUE)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT) break;
			DispatchMessage(&Message);
		}
		else
		{
			if (GameState == 1)
			{
				for(int i = COOKIETYPE_BRAVECOOKIE; i < COOKIETYPE_MAX; i++)
					if (CooldownState[i].CookieButtonState == true && CooldownState[i].CookieCooldownState == true)
					{
						while (Cookies[CookieCount] != NULL) { CookieCount++; }
						switch (i)
						{
						case COOKIETYPE_BRAVECOOKIE:
							Cookies[CookieCount] = new CBraveCookie;
							break;
						case COOKIETYPE_COTTONCANDYCOOKIE:
							Cookies[CookieCount] = new CCottoncandyCookie;
							break;
						case COOKIETYPE_PISTACHIOCOOKIE:
							Cookies[CookieCount] = new CPistachioCookie;
							break;
						case COOKIETYPE_PITAYACOOKIE:
							Cookies[CookieCount] = new CPitayaCookie;
							break;
						}
						CooldownState[i].m_nCooldownNow = GetTickCount64();
						CooldownState[i].CookieButtonState = false;
						if (CookieCount >= 100) CookieCount = 0;
						Cost -= Cookies[CookieCount]->GetStat(STATTYPE_COST);
					}
				if (MobCooldownState == false)
				{
					while (Mobs[MobCount] != NULL) { MobCount++; }
					Mobs[MobCount] = new CYellowBee;
					MobCooldownNow = GetTickCount64();
					if (MobCount >= 100) MobCount = 0;
					MobCooldownState = true;
				}
				__sleep(10);
			}
			__KeyProc(hWnd);
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC dcScreen;

	switch (iMessage)
	{
	case WM_CREATE:
		//// 표면 생성 및 기타
		dcScreen = GetDC(hWnd);
		__Init(dcScreen);
		ReleaseDC(hWnd, dcScreen);

		for (int i = 0; i < COOKIETYPE_MAX; i++)
		{
			g_objCookie[i].nAniWalk = 0;
			g_objCookie[i].nAniAttack = 0;
			g_objCookie[i].nAniFaint = 0;
		}

		g_objBoss.nAniIdle = 0;
		g_objBoss.nAniAttack = 0;
		g_objBoss.nAniFaint = 0;

		g_objTower.nAni = 0;
		////
		__sleep(100);

		//// 타이머 생성		
		// 60 FPS ( 1000ms / 60fps = 16.6ms )
		// 정밀도 NT : 10ms ( 100 fps )
		//        98 : 55ms (  18 fps )
		SetTimer(hWnd, ID_TM_MAINLOOP, 16, MainLoopProc);
		SetTimer(hWnd, ID_TM_ANIMATION, 60, NULL);

		return 0;

	case WM_SETFOCUS:
		::OutputDebugString("WM_SETFOCUS");
		return 0;

	case WM_MOUSEMOVE: g_ptMouse = MAKEPOINTS(lParam);
		return 0;


	case WM_TIMER:
		if (wParam == ID_TM_ANIMATION)
		{
			CheckCookieVanguard();
			CheckMobVanguard();

			//// 애니메이션 인덱스
			for (int i = 0; i < COOKIETYPE_MAX; i++)
			{
				g_objCookie[i].nAniWalk++;
				g_objCookie[i].nAniAttack++;
				g_objCookie[i].nAniFaint++;
				g_objCookie[i].nAniAttackMotion++;

				if (g_objCookie[i].nAniWalk > g_objCookie[i].nAniWalkMax - 1)
					g_objCookie[i].nAniWalk = 0;

				if (g_objCookie[i].nAniAttackMotion > g_objCookie[i].nAniAttackMotionMax - 1)
					g_objCookie[i].nAniAttackMotion = 0;

				if (g_objCookie[i].nAniFaint > g_objCookie[i].nAniFaintMax - 1)
				{
					g_objCookie[i].nAniFaint = 0;
					for (int j = 0; j < 101; j++)
						if (Cookies[j] != NULL && Cookies[j]->GetStat(STATTYPE_COOKIESTATE) == STATE_FAINT)
						{
							delete Cookies[i];
							Cookies[j] = NULL;
						}
				}

				if (g_objCookie[i].nAniAttack > g_objCookie[i].nAniAttackMax - 1)
				{
					g_objCookie[i].nAniAttack = 0;
					for (int j = 0; j < 101; j++)
					{
						if (Cookies[j] != NULL)
						{
							if (MobVanguard != -1 && Cookies[j]->GetStat(STATTYPE_COOKIESTATE) == STATE_ATTACK) // 쿠키가 몹을 공격하는 로직
							{
								Mobs[MobVanguard]->Attacked(Cookies[j]->GetStat(STATTYPE_POWER));
								if (Mobs[MobVanguard]->GetStat(MSTATTYPE_HP) <= 0)
								{
									Mobs[MobVanguard]->SetStat(MSTATTYPE_MOBSTATE, MSTATE_FAINT);
									CheckMobVanguard();
								}
							}
							else if (Cookies[j]->GetStat(STATTYPE_COOKIESTATE) == STATE_ATTACKBOSS && BossState == BSTATE_IDLE) // 쿠키가 보스를 공격하는 로직
							{
								g_objBoss.nLife -= Cookies[j]->GetStat(STATTYPE_POWER);
								if (g_objBoss.nLife < 0) BossState = BSTATE_FAINT;
							}
						}
					}
				}
			}

			g_objBoss.nAniIdle++;
			g_objBoss.nAniAttack++;
			g_objBoss.nAniFaint++;

			g_objTower.nAni++;
			g_objMob.nAniIdle++;

			if (g_objBoss.nAniIdle > 4) g_objBoss.nAniIdle = 0;
			if (g_objBoss.nAniAttack > 7) g_objBoss.nAniAttack = 0;
			if (g_objBoss.nAniFaint > 12) g_objBoss.nAniFaint = 0;

			if (g_objTower.nAni > 4) g_objTower.nAni = 0;

			// 쿠키, 몹 최선방 몹 정하는 로직

			// Attack 상태 로직

			for (int i = 0; i < 101; i++)
			{
				if (Cookies[i] != NULL)
				{
					if (MobVanguard == -1 && Cookies[i]->GetStat(STATTYPE_RANGE) >= 1400 - Cookies[i]->GetStat(STATTYPE_X))
						Cookies[i]->SetStat(STATTYPE_COOKIESTATE, STATE_ATTACKBOSS);
					else if (MobVanguard != -1 && Mobs[MobVanguard]->GetStat(MSTATTYPE_X) - Cookies[i]->GetStat(STATTYPE_X) <= Cookies[i]->GetStat(STATTYPE_RANGE) && Mobs[MobVanguard]->GetStat(MSTATTYPE_X) - Cookies[i]->GetStat(STATTYPE_X) > 0 && Cookies[i]->GetStat(STATTYPE_COOKIESTATE) == STATE_IDLE)
						Cookies[i]->SetStat(STATTYPE_COOKIESTATE, STATE_ATTACK);
					else if (MobVanguard == -1 || Mobs[MobVanguard]->GetStat(MSTATTYPE_X) - Cookies[i]->GetStat(STATTYPE_X) > Cookies[i]->GetStat(STATTYPE_RANGE) && Cookies[i]->GetStat(STATTYPE_COOKIESTATE) == STATE_ATTACK)
						Cookies[i]->SetStat(STATTYPE_COOKIESTATE, STATE_IDLE);
				}

				if (Mobs[i] != NULL)
				{
					if (CookieVanguard == -1 && Mobs[i]->GetStat(MSTATTYPE_RANGE) >= Mobs[i]->GetStat(MSTATTYPE_X) - 140)
						Mobs[i]->SetStat(MSTATTYPE_MOBSTATE, MSTATE_ATTACKTOWER);
					else if (CookieVanguard != -1 && Mobs[i]->GetStat(MSTATTYPE_X) - Cookies[CookieVanguard]->GetStat(STATTYPE_X) <= Mobs[i]->GetStat(MSTATTYPE_RANGE) && Mobs[i]->GetStat(MSTATTYPE_X) - Cookies[CookieVanguard]->GetStat(STATTYPE_X) > 0 && Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) == MSTATE_IDLE)
						Mobs[i]->SetStat(MSTATTYPE_MOBSTATE, MSTATE_ATTACK);
					else if (CookieVanguard == -1 || Mobs[i]->GetStat(MSTATTYPE_X) - Cookies[CookieVanguard]->GetStat(STATTYPE_X) > Mobs[i]->GetStat(MSTATTYPE_RANGE) && Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) == MSTATE_ATTACK)
						Mobs[i]->SetStat(MSTATTYPE_MOBSTATE, MSTATE_IDLE);
				}
			}

			for (int i = 0; i < 101; i++)
			{
				if (Cookies[i] != NULL)
				{
					if (Cookies[i]->GetStat(STATTYPE_COOKIESTATE) == STATE_IDLE)
						Cookies[i]->XPlus(); // 쿠키 이동 (Idle 상태에서만)
				}
				if (Mobs[i] != NULL)
				{
					if (Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) == MSTATE_IDLE)
						Mobs[i]->XMinus(); // 몹 이동 (Idle 상태에서만)
				}
			}

			if (g_objMob.nAniIdle > 2)
			{
				g_objMob.nAniIdle = 0;
				for (int i = 0; i < 101; i++) 
				{
					if (Mobs[i] != NULL)
					{
						if (CookieVanguard != -1 && Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) == MSTATE_ATTACK)// 몹이 쿠키를 공격하는 로직
						{
							Cookies[CookieVanguard]->Attacked(Mobs[i]->GetStat(MSTATTYPE_POWER));
							if (Cookies[CookieVanguard]->GetStat(STATTYPE_HP) <= 0)
							{
								Cookies[CookieVanguard]->SetStat(STATTYPE_COOKIESTATE, STATE_FAINT);
								CheckCookieVanguard();
							}
						}
						else if (Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) == MSTATE_ATTACKTOWER && TowerState == BSTATE_IDLE) // 몹이 타워를 공격하는 로직
						{
							g_objTower.nLife -= Mobs[i]->GetStat(MSTATTYPE_POWER);
							if (g_objTower.nLife < 0) TowerState = BSTATE_FAINT;
						}
					}
				}
			}

			for (int i = 0; i < 101; i++)
				if (Cookies[i] != NULL && Cookies[i]->GetStat(STATTYPE_COOKIESTATE) == STATE_FAINT)
					if (Cookies[i]->GetStat(STATTYPE_FAINTANI) < g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniFaintMax)
						Cookies[i]->SetStat(STATTYPE_FAINTANI, Cookies[i]->GetStat(STATTYPE_FAINTANI) + 1);

			for (int i = 0; i < 101; i++)
			{
				if (Cookies[i] != NULL && Cookies[i]->GetStat(STATTYPE_COOKIESTATE) >= 0 && Cookies[i]->GetStat(STATTYPE_FAINTANI) == g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniFaintMax)
				{
					delete Cookies[i];
					Cookies[i] = NULL;
				}
				if (Mobs[i] != NULL && Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) == MSTATE_FAINT)
				{
					delete Mobs[i];
					Mobs[i] = NULL;
				}
			}

			for (int i = COOKIETYPE_BRAVECOOKIE; i < COOKIETYPE_MAX; i++)
				if ((int)(GetTickCount64() - CooldownState[i].m_nCooldownNow) >= CooldownState[i].m_nCooldown)
				{
					CooldownState[i].CookieCooldownState = false;
					CooldownState[i].CookieButtonState = false;
				}

			if ((int)(GetTickCount64() - MobCooldownNow) >= MobCooldown)
				MobCooldownState = false;

			if ((int)(GetTickCount64() - EnterCooldown) >= 100)
				EnterState = 0;

			NumState = 0;

			if (Cost < 100 && GameState == 1) Cost++;
		}
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, ID_TM_MAINLOOP);
		KillTimer(hWnd, ID_TM_ANIMATION);
		__DestroyAll();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

static int nBgX = 1;
static int nBgGroundX = 1;
void CALLBACK MainLoopProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	char  strBuff[24];
	HDC   dcScreen;
	BOOL  bRval;

	switch (GameState)
	{
	case GSTATE_MAIN:
		dcScreen = GetDC(hWnd);
		{
			PutStart();

			//// 출력 완료
			__CompleteBlt(dcScreen, &g_sfBack);
		}
		ReleaseDC(hWnd, dcScreen);
		break;
	case GSTATE_GAMERULE:
		dcScreen = GetDC(hWnd);
		{
			bRval = __PutStretchSprite(g_sfBack.dcSurface, 0, 0, &g_sfStart, 0.3f, 0.3f);
			if (!bRval)	::OutputDebugString("__PutSprite - fail");

			bRval = __PutStretchSprite(g_sfBack.dcSurface, 23, 100, &g_sfStartHUD[5], 0.5f, 0.5f);
			if (!bRval)	::OutputDebugString("__PutSprite - fail");

			bRval = __PutStretchSprite(g_sfBack.dcSurface, 23, 100, &g_sfStartHUD[4], 0.5f, 0.5f);
			if (!bRval)	::OutputDebugString("__PutSprite - fail");

			//// 출력 완료
			__CompleteBlt(dcScreen, &g_sfBack);
		}
		ReleaseDC(hWnd, dcScreen);
		break;
	case GSTATE_GAME:
		//// 연산부
		nBgX -= 2;
		nBgGroundX -= 2;
		if (nBgX < -1018) nBgX = 0;
		if (nBgGroundX < -129) nBgGroundX = 0;

		//// 출력부
		dcScreen = GetDC(hWnd);
		{
			// 배경
			PutBackGround();

			// Cost Bar
			PutCostBar();

			// Cookie Button
			PutCookieButton();

			PutPowerUPs();

			//// 오브젝트 및 기타 인터페이스창

			PutMob();

			PutTower();

			PutBoss();

			PutCookie(hWnd, nBgX, nBgGroundX);

			WinLose();

			//// 출력 완료
			__CompleteBlt(dcScreen, &g_sfBack);
		}
		ReleaseDC(hWnd, dcScreen);
		break;
	case GSTATE_GAMECOUNT:
	{
		int nCount = GetTickCount64();
		while (1)
		{
			//// 연산부
			nBgX = 0;
			nBgGroundX = 0;

			//// 출력부
			dcScreen = GetDC(hWnd);
			{
				// 배경
				PutBackGround();

				// Cost Bar
				PutCostBar();

				// Cookie Button
				PutCookieButton();

				PutPowerUPs();

				//// 오브젝트 및 기타 인터페이스창

				PutTower();

				PutBoss();

				if (GetTickCount64() - nCount < 1000)
				{
					__PutImageBlend(g_sfBack.dcSurface, 0, 0, &g_sfBlack, 100);
					bRval = __PutStretchSprite(g_sfBack.dcSurface, 700, 130, &(g_sfFont[12]), 2.0f, 2.0f);
					if (!bRval)	::OutputDebugString("__PutSprite - fail");
				}
				else if (GetTickCount64() - nCount < 2000)
				{
					__PutImageBlend(g_sfBack.dcSurface, 0, 0, &g_sfBlack, 100);
					bRval = __PutStretchSprite(g_sfBack.dcSurface, 700, 130, &(g_sfFont[11]), 2.0f, 2.0f);
					if (!bRval)	::OutputDebugString("__PutSprite - fail");
				}
				else if (GetTickCount64() - nCount < 3000)
				{
					__PutImageBlend(g_sfBack.dcSurface, 0, 0, &g_sfBlack, 100);
					bRval = __PutStretchSprite(g_sfBack.dcSurface, 730, 130, &(g_sfFont[10]), 2.0f, 2.0f);
					if (!bRval)	::OutputDebugString("__PutSprite - fail");
				}
				else
				{
					GameState = GSTATE_GAME;
				}

				//// 출력 완료
				__CompleteBlt(dcScreen, &g_sfBack);

				if (GameState == GSTATE_GAME) break;
			}
			ReleaseDC(hWnd, dcScreen);
		}
		break;
	}
	case GSTATE_OPTIONSELECT:
			//// 연산부
			nBgX = 0;
			nBgGroundX = 0;

			//// 출력부
			dcScreen = GetDC(hWnd);
			{
				// 배경
				PutBackGround();

				// Cost Bar
				PutCostBar();

				// Cookie Button
				PutCookieButton();

				PutPowerUPs();

				//// 오브젝트 및 기타 인터페이스창

				PutTower();

				PutBoss();

				__PutImageBlend(g_sfBack.dcSurface, 0, 0, &g_sfBlack, 100);

				PutPowerUPsCenter();

				//// 출력 완료
				__CompleteBlt(dcScreen, &g_sfBack);
			}
			ReleaseDC(hWnd, dcScreen);
		break;
	}
}

void PutStart()
{
	BOOL bRval;
	TCHAR* str = TEXT("Cookierun : Universe");

	g_sfStart.crColorKey = RGB(0, 255, 255);

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 0, 0, &g_sfStart, 0.3f, 0.3f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 90, 430, &g_sfStartHUD[0], 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 270, 500, &g_sfStartHUD[3], 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");
}

void PutBackGround()
{
	BOOL bRval;

	__PutImage(g_sfBack.dcSurface, nBgX, 0, &g_sfBG);
	__PutImage(g_sfBack.dcSurface, nBgX + 1018, 0, &g_sfBG);
	__PutImage(g_sfBack.dcSurface, nBgX + 2036, 0, &g_sfBG);
	for (int i = 0; i < 15; i++)
		__PutImage(g_sfBack.dcSurface, nBgGroundX + 129 * i, 350, &g_sfGround);

	bRval = __PutStretchSprite(g_sfBack.dcSurface, -50, 390, &(g_sfMenuBG[0]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 889, 390, &(g_sfMenuBG[0]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");
}

void PutCostBar()
{
	BOOL bRval;

	bRval = TransparentBlt(g_sfBack.dcSurface, 26, 405, g_sfCostBar[0].nWidth * 0.5 * Cost * 0.01 , g_sfCostBar[0].nHeight * 0.5, g_sfCostBar[0].dcSurface, 0, 0, g_sfCostBar[0].nWidth * Cost * 0.01, g_sfCostBar[0].nHeight, g_sfCostBar[0].crColorKey);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 0, 400, &(g_sfCostBar[2]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 4, 403, &(g_sfCostBar[3]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (Cost == 100)
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 185, 405, &(g_sfFont[1]), 1.0f, 1.0f);
	if (Cost >= 10)
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 200, 405, &(g_sfFont[(Cost / 10) % 10]), 1.0f, 1.0f);
	bRval = __PutStretchSprite(g_sfBack.dcSurface, 220, 405, &(g_sfFont[Cost % 10]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");
}

void PutCookieButton()
{
	BOOL bRval;

	if (!CooldownState[COOKIETYPE_BRAVECOOKIE].CookieCooldownState)
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 20, 450, &(g_sfCookieButton[0]), 0.5f, 0.5f);
	else
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 20, 450, &(g_sfCookieButton[1]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 37, 468, &(g_sfCookieButton[2]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 35, 450, &(g_sfFont[1]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 75, 510, &(g_sfFont[1]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 85, 510, &(g_sfFont[0]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");										// BraveCookie

	if (!CooldownState[COOKIETYPE_COTTONCANDYCOOKIE].CookieCooldownState)
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 120, 450, &(g_sfCookieButton[3]), 0.5f, 0.5f);
	else
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 120, 452, &(g_sfCookieButton[4]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 137, 468, &(g_sfCookieButton[5]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 130, 450, &(g_sfFont[2]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 172, 510, &(g_sfFont[2]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 185, 510, &(g_sfFont[0]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");										// CottoncandyCookie

	if (!CooldownState[COOKIETYPE_PISTACHIOCOOKIE].CookieCooldownState)
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 220, 450, &(g_sfCookieButton[0]), 0.5f, 0.5f);
	else
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 220, 450, &(g_sfCookieButton[1]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 237, 468, &(g_sfCookieButton[6]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 230, 450, &(g_sfFont[3]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 272, 510, &(g_sfFont[2]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 285, 510, &(g_sfFont[5]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");										// PistachioCookie

	if (!CooldownState[COOKIETYPE_PITAYACOOKIE].CookieCooldownState)
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 320, 450, &(g_sfCookieButton[0]), 0.5f, 0.5f);
	else
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 320, 450, &(g_sfCookieButton[1]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 337, 468, &(g_sfCookieButton[7]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 330, 450, &(g_sfFont[4]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 372, 510, &(g_sfFont[5]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 385, 510, &(g_sfFont[0]), 0.75f, 0.75f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");										// PitayaCookie
}

void PutCookie(HWND hWnd, int nBgX, int nBGroundX)
{
	HDC AlphaDC;
	BOOL bRval;

	AlphaDC = GetDC(hWnd);

	for (int i = 0; i < 101; i++)
	{
		if (Cookies[i] != NULL)
		{
			int puty = y;
			if (Cookies[i]->GetStat(STATTYPE_COOKIETYPE) == COOKIETYPE_PISTACHIOCOOKIE) puty = y - 18;
			if (Cookies[i]->GetStat(STATTYPE_COOKIETYPE) == COOKIETYPE_PITAYACOOKIE) puty = y + 22;
			switch (Cookies[i]->GetStat(STATTYPE_COOKIESTATE))
			{
			case STATE_IDLE:
				bRval = __PutStretchSprite(g_sfBack.dcSurface, Cookies[i]->GetStat(STATTYPE_X), puty, &(g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].g_sfCookieWalk[g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniWalk]), 0.5f, 0.5f);
				break;
			case STATE_ATTACK:
			case STATE_ATTACKBOSS:
				bRval = __PutStretchSprite(g_sfBack.dcSurface, Cookies[i]->GetStat(STATTYPE_X), puty, &(g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].g_sfCookieAttack[g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniAttack]), 0.5f, 0.5f);
				/*if (Cookies[i]->GetStat(STATTYPE_COOKIETYPE) == COOKIETYPE_COTTONCANDYCOOKIE)
				{
					if(MobVanguard != -1)
						bRval = __PutAlphaStretchSprite(g_sfBack.dcSurface, AlphaDC, Mobs[MobVanguard]->GetStat(STATTYPE_X), puty, nBgX, nBGroundX, &(g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].g_sfCookieAttackMotion[g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniAttackMotion]), &g_sfBG, &g_sfGround, &(g_sfMenuBG[0]), 100);
					else
						bRval = __PutAlphaStretchSprite(g_sfBack.dcSurface, AlphaDC, 1400, puty, nBgX, nBGroundX, &(g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].g_sfCookieAttackMotion[g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniAttackMotion]), &g_sfBG, &g_sfGround, &(g_sfMenuBG[0]), 100);
				}*/
					
				break;
			case STATE_FAINT:
				bRval = __PutStretchSprite(g_sfBack.dcSurface, Cookies[i]->GetStat(STATTYPE_X), puty, &(g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].g_sfCookieFaint[g_objCookie[Cookies[i]->GetStat(STATTYPE_COOKIETYPE)].nAniFaint]), 0.5f, 0.5f);
				break;
			}
			if (!bRval)	::OutputDebugString("__PutSprite - fail");
		}
	}
}

void PutMob()
{
	BOOL bRval;

	for (int i = 0; i < 101; i++)
	{
		if (Mobs[i] != NULL)
		{
			bRval = __PutStretchSprite(g_sfBack.dcSurface, Mobs[i]->GetStat(MSTATTYPE_X), 270, &(g_objMob.g_sfMobIdle[g_objMob.nAniIdle]), 0.5f, 0.5f);
			if (!bRval)	::OutputDebugString("__PutSprite - fail");
		}
	}
}

void PutBoss()
{
	BOOL bRval;

	switch (BossState)
	{
	case BSTATE_IDLE:
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1400, 70, &(g_objBoss.g_sfBossIdle[g_objBoss.nAniIdle]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
		break;
	case BSTATE_FAINT:
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1400, 70, &(g_objBoss.g_sfBossFaint[g_objBoss.nAniFaint]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
		break;
	}
}

void PutTower()
{
	BOOL bRval;

	switch (TowerState)
	{
	case BSTATE_IDLE:
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 0, 104, &(g_objTower.g_sfTowerAni[g_objTower.nAni]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
		break;
	}
}

void CheckCookieVanguard()
{
	for (int i = 0; i < 101; i++)
	{
		if (Cookies[i] != NULL && Cookies[i]->GetStat(STATTYPE_COOKIESTATE) != STATE_FAINT)
		{
			CookieVanguard = i;
			return;
		}
	}
	for (int i = 0; i < 101; i++)
	{
		if (Cookies[i] != NULL && Cookies[i]->GetStat(STATTYPE_COOKIESTATE) != STATE_FAINT && Cookies[i]->GetStat(STATTYPE_X) > Cookies[CookieVanguard]->GetStat(STATTYPE_X))
		{
			CookieVanguard = i;
			return;
		}
	}
	CookieVanguard = -1;
}

void CheckMobVanguard()
{
	for (int i = 0; i < 101; i++)
	{
		if (Mobs[i] != NULL && Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) != MSTATE_FAINT)
		{
			MobVanguard = i;
			return;
		}
	}
	for (int i = 0; i < 101; i++)
	{
		if (Mobs[i] != NULL && Mobs[i]->GetStat(MSTATTYPE_MOBSTATE) != MSTATE_FAINT && Mobs[i]->GetStat(MSTATTYPE_X) < Mobs[MobVanguard]->GetStat(MSTATTYPE_X))
		{
			MobVanguard = i;
			return;
		}
	}
	MobVanguard = -1;
}

void WinLose()
{
	BOOL bRval = true;

	if (BossState == BSTATE_FAINT || TowerState == BSTATE_FAINT)
		__PutImageBlend(g_sfBack.dcSurface, 0, 0, &g_sfBlack, 100);

	if (BossState == BSTATE_FAINT)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 700, 200, &g_sfResult[0], 1.0f, 1.0f);
		GameState = GSTATE_WIN;
		RoundInit();
	}
	else if (TowerState == BSTATE_FAINT)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 700, 200, &g_sfResult[1], 1.0f, 1.0f);
		GameState = GSTATE_LOSE;
		RoundReset();
	}

	if (!bRval)	::OutputDebugString("__PutSprite - fail");
}

void RoundInit()
{
	GameRound++;
	BossState = BSTATE_IDLE;
	TowerState = BSTATE_IDLE;
	Cost = 0;
	for (int i = 0; i < 101; i++)
	{
		if (Cookies[i] != NULL)
		{
			delete Cookies[i];
			Cookies[i] = NULL;
		}
		if (Mobs[i] != NULL)
		{
			delete Mobs[i];
			Mobs[i] = NULL;
		}
	}
}

void RoundReset()
{
	GameRound = 1;
	BossState = BSTATE_IDLE;
	TowerState = BSTATE_IDLE;
	Cost = 0;
	for (int i = 0; i < 101; i++)
	{
		if (Cookies[i] != NULL)
		{
			delete Cookies[i];
			Cookies[i] = NULL;
		}
		if (Mobs[i] != NULL)
		{
			delete Mobs[i];
			Mobs[i] = NULL;
		}
	}
}

void PutPowerUPs()
{
	BOOL bRval;

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1380, 450, &(g_sfPowerUps[6]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1391, 464, &(g_sfPowerUps[8]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[0] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1380, 450, &(g_sfPowerUps[PowerUpStack[0] - 1]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1480, 450, &(g_sfPowerUps[6]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1491, 464, &(g_sfPowerUps[9]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[1] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1480, 450, &(g_sfPowerUps[PowerUpStack[1] - 1]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1580, 450, &(g_sfPowerUps[6]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1591, 464, &(g_sfPowerUps[10]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[2] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1580, 450, &(g_sfPowerUps[PowerUpStack[2] - 1]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1680, 450, &(g_sfPowerUps[6]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1691, 464, &(g_sfPowerUps[11]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[3] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1680, 450, &(g_sfPowerUps[PowerUpStack[3] - 1]), 0.5f, 0.5f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}
}

void PutPowerUPsCenter()
{
	BOOL bRval;

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 400, 230, &(g_sfPowerUps[6]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 420, 255, &(g_sfPowerUps[8]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[0] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 400, 230, &(g_sfPowerUps[PowerUpStack[0] - 1]), 1.0f, 1.0f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 420, 220, &(g_sfFont[10]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 650, 230, &(g_sfPowerUps[6]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 670, 255, &(g_sfPowerUps[9]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[1] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 650, 230, &(g_sfPowerUps[PowerUpStack[1] - 1]), 1.0f, 1.0f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 650, 220, &(g_sfFont[11]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 900, 230, &(g_sfPowerUps[6]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 920, 255, &(g_sfPowerUps[10]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[2] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 900, 230, &(g_sfPowerUps[PowerUpStack[2] - 1]), 1.0f, 1.0f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 900, 220, &(g_sfFont[12]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1150, 230, &(g_sfPowerUps[6]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1170, 255, &(g_sfPowerUps[11]), 1.0f, 1.0f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");

	if (PowerUpStack[3] != 0)
	{
		bRval = __PutStretchSprite(g_sfBack.dcSurface, 1150, 230, &(g_sfPowerUps[PowerUpStack[3] - 1]), 1.0f, 1.0f);
		if (!bRval)	::OutputDebugString("__PutSprite - fail");
	}

	bRval = __PutStretchSprite(g_sfBack.dcSurface, 1150, 220, &(g_sfFont[13]), 0.5f, 0.5f);
	if (!bRval)	::OutputDebugString("__PutSprite - fail");
}