#include "CBirdAPP.h"
#include "../GameConfig/GameConfig.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

WND_PARAM(200, 100, WINDOW_WIDTH, WINDOW_HEIGHT, "FlappyBird")
DYNAMIC_CREATE(BirdGame_Create)

int g_rand = 0;

static BirdGame* self(GameFrame* base)
{
    return reinterpret_cast<BirdGame*>(base);
}

GameFrame* BirdGame_Create(void)
{
    BirdGame* game = new (std::nothrow) BirdGame;
    if (!game) return NULL;

    /* Manual zero-init (don't memset - would corrupt IMAGE objects) */
    game->m_GameStatus = 0;
    game->m_score = 0;
    game->base.m_end = 0;

    /* Init sub-modules */
    BackGround_Init(&game->m_BackGround);
    Ground_Init(&game->m_Ground);
    PlayerBird_Init(&game->m_PlayerBird);
    BeforeGame_Init(&game->m_BeforeGame);
    AfterGame_Init(&game->m_AfterGame);
    ColumnBox_Init(&game->m_ColumnBox);

    /* Set function pointers (virtual function table) */
    game->base.On_Init = BirdGame_OnInit;
    game->base.On_Paint = BirdGame_OnPaint;
    game->base.On_Close = BirdGame_OnClose;
    game->base.On_MouseDown = BirdGame_OnMouseDown;
    game->base.On_Timer = BirdGame_OnTimer;
    game->base.On_KeyDown = NULL;
    game->base.On_Char = NULL;

    return &game->base;
}

/* ========== Virtual function implementations ========== */

void BirdGame_OnInit(GameFrame* base)
{
    BirdGame* p = self(base);

    if (p->m_GameStatus == 1) {
        srand((unsigned int)time(NULL));
        AfterGame_Init(&p->m_AfterGame);
        BirdGame_SetTimer(p);
    } else if (p->m_GameStatus == 0) {
        p->m_score = 0;
        ColumnBox_Clear(&p->m_ColumnBox);
        BackGround_Init(&p->m_BackGround);
        Ground_Init(&p->m_Ground);
        PlayerBird_Init(&p->m_PlayerBird);
        BeforeGame_Init(&p->m_BeforeGame);
        BirdGame_SetTimer(p);
    }
}

void BirdGame_OnPaint(GameFrame* base)
{
    BirdGame* p = self(base);

    if (p->m_GameStatus == 1) {
        BackGround_Show(&p->m_BackGround);
        ColumnBox_ShowAll(&p->m_ColumnBox);
        Ground_Show(&p->m_Ground);
        BirdGame_ShowLogo(p);
        BirdGame_ShowScore(p);
        PlayerBird_Show(&p->m_PlayerBird);
    } else if (p->m_GameStatus == 0) {
        BackGround_Show(&p->m_BackGround);
        Ground_Show(&p->m_Ground);
        BirdGame_ShowLogo(p);
        BeforeGame_Show(&p->m_BeforeGame);
        PlayerBird_Show(&p->m_PlayerBird);
    } else if (p->m_GameStatus == 2) {
        BackGround_Show(&p->m_BackGround);
        ColumnBox_ShowAll(&p->m_ColumnBox);
        Ground_Show(&p->m_Ground);
        BirdGame_ShowLogo(p);
        PlayerBird_Show(&p->m_PlayerBird);
        AfterGame_Show(&p->m_AfterGame);
        BirdGame_ShowScore(p);
    }
}

void BirdGame_OnClose(GameFrame* base)
{
    BirdGame* p = self(base);
    KillTimer(base->hwnd, 1);
    KillTimer(base->hwnd, 2);
    KillTimer(base->hwnd, 3);
    KillTimer(base->hwnd, 4);
    KillTimer(base->hwnd, 5);
    KillTimer(base->hwnd, 6);
    ColumnBox_Clear(&p->m_ColumnBox);
}

void BirdGame_OnMouseDown(GameFrame* base, int x, int y)
{
    BirdGame* p = self(base);

    if (p->m_GameStatus == 1) {
        p->m_PlayerBird.m_speed = BIRD_STARTSPEED;
    } else if (p->m_GameStatus == 0 &&
               x >= BUTTON_POS_X_1 && x <= BUTTON_POS_X_2 &&
               y >= BUTTON_POS_Y_1 && y <= BUTTON_POS_Y_2) {
        p->m_GameStatus = 1;
        BirdGame_OnInit(base);
    } else if (p->m_GameStatus == 2 &&
               x >= BUTTON2_POS_X_1 && x <= BUTTON2_POS_X_2 &&
               y >= BUTTON2_POS_Y_1 && y <= BUTTON2_POS_Y_2) {
        p->m_GameStatus = 0;
        BirdGame_OnInit(base);
    }
}

void BirdGame_OnTimer(GameFrame* base, WPARAM wParam, LPARAM lParam)
{
    BirdGame* p = self(base);
    (void)lParam;

    switch (wParam) {
    case TIMER_GROUND_MOVE_TIMERID: {
        BackGround_Move(&p->m_BackGround, 1);
        Ground_Move(&p->m_Ground, TIMER_GROUND_MOVE_STEP);
        ColumnBox_MoveAll(&p->m_ColumnBox, TIMER_GROUND_MOVE_STEP);

        /* Check column collision and scoring */
        ListNode* node = p->m_ColumnBox.m_ColLst.head;
        while (node) {
            Column* col = static_cast<Column*>(node->data);
            if (col) {
                if (Column_IfTouch(col, &p->m_PlayerBird)) {
                    BirdGame_ReStartStop(p);
                    p->m_GameStatus = 2;
                }
                if (col->m_x + IMG_COLUMN_WIDTH <= p->m_PlayerBird.m_x && !col->m_IfPass) {
                    col->m_IfPass = 1;
                    p->m_score++;
                }
            }
            node = node->next;
        }
        break;
    }
    case TIMER_RAND_NEW_TIMERID: {
        g_rand = rand();
        break;
    }
    case TIMER_NEW_COLUMN_TIMERID: {
        Column* newCol = new (std::nothrow) Column;
        if (newCol) {
            Column_Init(newCol);
            List_PushBack(&p->m_ColumnBox.m_ColLst, newCol);
        }
        break;
    }
    case TIMER_BIRD_CHANGE_TIMERID: {
        if (p->m_PlayerBird.m_mode + 1 <= 7) {
            p->m_PlayerBird.m_mode++;
        } else {
            p->m_PlayerBird.m_mode = 0;
        }
        break;
    }
    case TIMER_BIRD_SHOW_TIMERID: {
        PlayerBird_Show(&p->m_PlayerBird);
        break;
    }
    case TIMER_BIRD_POSITION_TIMERID: {
        PlayerBird_Move(&p->m_PlayerBird);
        if (Ground_IfTouch(&p->m_Ground, &p->m_PlayerBird)) {
            BirdGame_ReStartStop(p);
            p->m_GameStatus = 2;
        }
        break;
    }
    }
}

/* ========== Helper functions ========== */

void BirdGame_SetTimer(BirdGame* p)
{
    HWND hwnd = p->base.hwnd;

    if (p->m_GameStatus == 1) {
        SetTimer(hwnd, TIMER_RAND_NEW_TIMERID, TIMER_RAND_NEW_INTERVAL, NULL);
        SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, TIMER_NEW_COLUMN_INTERVAL, NULL);
        SetTimer(hwnd, TIMER_BIRD_POSITION_TIMERID, TIMER_BIRD_POSITION_INTERVAL, NULL);
    } else if (p->m_GameStatus == 0) {
        SetTimer(hwnd, TIMER_GROUND_MOVE_TIMERID, TIMER_GROUND_MOVE_INTERVAL, NULL);
        SetTimer(hwnd, TIMER_BIRD_CHANGE_TIMERID, TIMER_BIRD_CHANGE_INTERVAL, NULL);
        SetTimer(hwnd, TIMER_BIRD_SHOW_TIMERID, TIMER_BIRD_SHOW_INTERVAL, NULL);
    }
}

void BirdGame_ReStartStop(BirdGame* p)
{
    HWND hwnd = p->base.hwnd;
    KillTimer(hwnd, 1);
    KillTimer(hwnd, 2);
    KillTimer(hwnd, 3);
    KillTimer(hwnd, 4);
    KillTimer(hwnd, 5);
    KillTimer(hwnd, 6);
}

void BirdGame_ShowLogo(BirdGame* p)
{
    (void)p;
    settextcolor(WHITE);
    settextstyle(20, 0, "Arial");
    setbkmode(TRANSPARENT);
    outtextxy(10, IMG_GROUND_POS_Y + 10, "NUIST");
    outtextxy(10, IMG_GROUND_POS_Y + 40, "Flappy Dragon");
}

void BirdGame_ShowScore(BirdGame* p)
{
    char sc[16];
    sprintf_s(sc, sizeof(sc), "%d", p->m_score);

    RECT rect = { 0, 0, WINDOW_HEIGHT, WINDOW_WIDTH };
    settextcolor(RGB(150, 100, 99));
    settextstyle(60, 0, _T("Arial"));
    setbkmode(TRANSPARENT);
    drawtext(sc, &rect, DT_CENTER | DT_SINGLELINE);
}
