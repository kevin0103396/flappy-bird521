#include<graphics.h>
#include"frame.h"
#include"IntroVideo.h"

extern int WND_POS_X;
extern int WND_POS_Y;
extern int WND_WIDTH;
extern int WND_HEIGHT;
extern const TCHAR* WND_TITLE;

GameFrame* CreatObject();

int main()
{
    g_PG = CreatObject();

    /* Create window first (no timers yet) */
    GameFrame_CreateWindow(g_PG, WND_TITLE, WND_WIDTH, WND_HEIGHT, WND_POS_X, WND_POS_Y);
    GameFrame_Paint(g_PG);

    /* Play intro before starting game timers */
    PlayIntroVideo(g_PG->hwnd);

    /* Now start the game (timers, etc.) */
    GameFrame_Start(g_PG);

    /* Main loop */
    while (!GameFrame_IfEnd(g_PG)) {
        Sleep(200);
    }

    GameFrame_End(g_PG);
    delete g_PG;
    g_PG = NULL;

    return 0;
}
