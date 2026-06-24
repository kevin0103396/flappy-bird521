#pragma once
#include"../GameFrame/frame.h"
#include"../BackGround/CBackGround.h"
#include"../Ground/CGround.h"
#include"../Column/CColumnBox.h"
#include"../PlayerBird/CPlayerBird.h"
#include"../BeforeGame/BeforeGame.h"
#include"../AfterGame/CAfterGame.h"

typedef struct {
    GameFrame base;  /* MUST be first member */

    BackGround  m_BackGround;
    Ground      m_Ground;
    PlayerBird  m_PlayerBird;
    ColumnBox   m_ColumnBox;
    BeforeGame  m_BeforeGame;
    AfterGame   m_AfterGame;

    int m_GameStatus;
    int m_score;
} BirdGame;

extern int g_rand;

/* Create the BirdGame object (used by DYNAMIC_CREATE) */
GameFrame* BirdGame_Create(void);

/* Internal functions */
void BirdGame_SetTimer(BirdGame* p);
void BirdGame_ShowLogo(BirdGame* p);
void BirdGame_ShowScore(BirdGame* p);
void BirdGame_ReStartStop(BirdGame* p);

/* Virtual function implementations */
void BirdGame_OnInit(GameFrame* base);
void BirdGame_OnPaint(GameFrame* base);
void BirdGame_OnClose(GameFrame* base);
void BirdGame_OnMouseDown(GameFrame* base, int x, int y);
void BirdGame_OnTimer(GameFrame* base, WPARAM wParam, LPARAM lParam);
