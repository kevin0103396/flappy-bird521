#include <stdio.h>
#include <stdlib.h>
#include "CColumn.h"
#include "../GameConfig/GameConfig.h"
#include "../PlayerBird/CPlayerBird.h"

static const int PIPE_GAP = 149;
static IMAGE s_up[5], s_upF[5], s_low[5], s_lowF[5];
static int s_upH[5];
static int s_loaded = 0;

extern int g_rand;

void Column_Init(Column* p)
{
    if (!s_loaded) {
        for (int i = 0; i < 5; i++) {
            char path[64];
            sprintf_s(path, "./res/column/column_up_%d.png", i + 1);
            loadimage(&s_up[i], path);
            sprintf_s(path, "./res/column/column_up_%d_front.png", i + 1);
            loadimage(&s_upF[i], path);
            sprintf_s(path, "./res/column/column_low_%d.png", i + 1);
            loadimage(&s_low[i], path);
            sprintf_s(path, "./res/column/column_low_%d_front.png", i + 1);
            loadimage(&s_lowF[i], path);
            s_upH[i] = s_up[i].getheight();
        }
        s_loaded = 1;
    }

    p->m_style = rand() % 5;
    p->m_pipeH = s_upH[p->m_style];
    p->m_x = WINDOW_WIDTH + 5;
    p->m_y = (-g_rand % (340)) - 120;
    p->m_IfPass = 0;
}

void Column_Show(Column* p)
{
    int s = p->m_style;
    int h = p->m_pipeH;
    int gap = PIPE_GAP;

    putimage(p->m_x, p->m_y, IMG_COLUMN_WIDTH, h, &s_upF[s], 0, 0, SRCAND);
    putimage(p->m_x, p->m_y, IMG_COLUMN_WIDTH, h, &s_up[s], 0, 0, SRCPAINT);

    int lh = s_low[s].getheight();
    putimage(p->m_x, p->m_y + h + gap, IMG_COLUMN_WIDTH, lh, &s_lowF[s], 0, 0, SRCAND);
    putimage(p->m_x, p->m_y + h + gap, IMG_COLUMN_WIDTH, lh, &s_low[s], 0, 0, SRCPAINT);
}

void Column_Move(Column* p, int step)
{
    p->m_x -= step;
}

int Column_IfTouch(Column* p, PlayerBird* bird)
{
    int h = p->m_pipeH;
    int gap = PIPE_GAP;

    int birdL = bird->m_x;
    int birdR = bird->m_x + IMG_BIRD_WIDTH * 3 / 4;
    int birdT = bird->m_y + IMG_BIRD_HEIGHT * 1 / 4;
    int birdB = bird->m_y + IMG_BIRD_HEIGHT * 3 / 4;

    int colL = p->m_x;
    int colR = p->m_x + IMG_COLUMN_WIDTH;

    if (colL <= birdR && colR >= birdL &&
        (birdT <= p->m_y + h || birdB >= p->m_y + h + gap)) {
        return 1;
    }
    return 0;
}
