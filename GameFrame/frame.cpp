#include "frame.h"
#include <windowsx.h>

GameFrame* g_PG = NULL;

void GameFrame_CreateWindow(GameFrame* p, const TCHAR* title, int w, int h, int x, int y)
{
    p->hwnd = initgraph(w, h);
    MoveWindow(p->hwnd, x, y, w, h, TRUE);
    SetWindowText(p->hwnd, title);
    setbkcolor(RGB(255, 255, 255));
    cleardevice();
    SetWindowLongPtr(p->hwnd, GWLP_WNDPROC, (LONG_PTR)Wndproc);
}

void GameFrame_Start(GameFrame* p)
{
    if (p->On_Init) p->On_Init(p);
}

void GameFrame_Paint(GameFrame* p)
{
    if (!p) return;
    BeginBatchDraw();
    cleardevice();
    if (p->On_Paint) p->On_Paint(p);
    EndBatchDraw();
}

void GameFrame_End(GameFrame* p)
{
    if (!p) return;
    if (p->On_Close) p->On_Close(p);
    closegraph();
    p->m_end = 1;
}

int GameFrame_IfEnd(GameFrame* p)
{
    return p ? p->m_end : 1;
}

LRESULT CALLBACK Wndproc(HWND hwnd, UINT msgid, WPARAM wParam, LPARAM lParam)
{
    if (!g_PG || GameFrame_IfEnd(g_PG))
        return DefWindowProc(hwnd, msgid, wParam, lParam);

    switch (msgid) {
    case WM_LBUTTONDOWN:
        if (g_PG->On_MouseDown) {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            g_PG->On_MouseDown(g_PG, x, y);
        }
        break;
    case WM_CLOSE:
        GameFrame_End(g_PG);
        return 0;
    case WM_KEYDOWN:
        if (g_PG->On_KeyDown) g_PG->On_KeyDown(g_PG, wParam);
        break;
    case WM_CHAR:
        if (g_PG->On_Char) g_PG->On_Char(g_PG, wParam);
        break;
    case WM_TIMER:
        if (g_PG->On_Timer) g_PG->On_Timer(g_PG, wParam, lParam);
        break;
    default:
        return DefWindowProc(hwnd, msgid, wParam, lParam);
    }

    if (!GameFrame_IfEnd(g_PG)) {
        GameFrame_Paint(g_PG);
    }

    return DefWindowProc(hwnd, msgid, wParam, lParam);
}
