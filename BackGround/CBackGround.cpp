#include <stdio.h>
#include "CBackGround.h"
#include "../GameConfig/GameConfig.h"

void BackGround_Init(BackGround* p)
{
    for (int i = 0; i < 8; i++) {
        char path[64];
        sprintf_s(path, "./res/background/bg_%d.png", i + 1);
        loadimage(&p->m_imgBack[i], path);
    }
    p->m_x = 0;
    p->m_y = 0;
    p->m_idx = 0;
}

void BackGround_Show(BackGround* p)
{
    int imgW = p->m_imgBack[p->m_idx].getwidth();
    putimage(p->m_x, 0, &p->m_imgBack[p->m_idx]);
    putimage(p->m_x + imgW, 0, &p->m_imgBack[p->m_idx]);
}

void BackGround_Move(BackGround* p, int step)
{
    int imgW = p->m_imgBack[p->m_idx].getwidth();
    p->m_x -= step;
    if (p->m_x <= -imgW) {
        p->m_x = 0;
        p->m_idx = (p->m_idx + 1) % 8;
    }
}
