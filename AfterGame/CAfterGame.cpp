#include "CAfterGame.h"

void AfterGame_Init(AfterGame* p)
{
    p->m_x = 0;
    p->m_y = 0;
    loadimage(&p->m_imgAfterGame, "./res/gameover/gameover.png");
    loadimage(&p->m_imgAfterGameFront, "./res/gameover/gameoverFront.png");
}

void AfterGame_Show(AfterGame* p)
{
    putimage(p->m_x, p->m_y, IMG_BACKGROUND_WIDTH, IMG_BACKGROUND_HEIGHT,
        &p->m_imgAfterGame, 0, 0, SRCPAINT);
    putimage(p->m_x, p->m_y, IMG_BACKGROUND_WIDTH, IMG_BACKGROUND_HEIGHT,
        &p->m_imgAfterGameFront, 0, 0, SRCAND);
}

void AfterGame_ReStart(AfterGame* p)
{
    (void)p;
}
