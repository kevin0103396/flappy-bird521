#pragma once
#define _WIN32_WINNT 0x0601
#include<graphics.h>
#include<windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- GameFrame base struct ---- */
typedef struct GameFrame {
    HWND hwnd;
    int  m_end;

    /* Virtual function table */
    void (*On_Init)(struct GameFrame*);
    void (*On_Paint)(struct GameFrame*);
    void (*On_Close)(struct GameFrame*);
    void (*On_MouseDown)(struct GameFrame*, int x, int y);
    void (*On_KeyDown)(struct GameFrame*, WPARAM);
    void (*On_Char)(struct GameFrame*, WPARAM);
    void (*On_Timer)(struct GameFrame*, WPARAM, LPARAM);
} GameFrame;

/* ---- Functions ---- */
void GameFrame_CreateWindow(GameFrame* p, const TCHAR* title, int w, int h, int x, int y);
void GameFrame_Start(GameFrame* p);
void GameFrame_Paint(GameFrame* p);
void GameFrame_End(GameFrame* p);
int  GameFrame_IfEnd(GameFrame* p);

/* ---- Windows callback ---- */
extern GameFrame* g_PG;
LRESULT CALLBACK Wndproc(HWND hwnd, UINT msgid, WPARAM wParam, LPARAM lParam);

/* ---- Macros ---- */
#define WND_PARAM(POS_X,POS_Y,WIDTH,HEIGHT,TITLE)\
    int WND_POS_X = POS_X;\
    int WND_POS_Y = POS_Y;\
    int WND_WIDTH = WIDTH;\
    int WND_HEIGHT = HEIGHT;\
    const TCHAR* WND_TITLE = TITLE;

#define DYNAMIC_CREATE(FN)\
    GameFrame* CreatObject() { return FN(); }

#ifdef __cplusplus
}
#endif
