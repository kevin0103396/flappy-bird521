#include "BeforeGame.h"

void BeforeGame_Init(BeforeGame* p)
{
    loadimage(&p->m_imgBeforeGame, "./res/start/start.png");
    loadimage(&p->m_imgBeforeGameFront, "./res/start/startFront.png");
    p->m_x = 0;
    p->m_y = 0;
}

void BeforeGame_Show(BeforeGame* p)
{
    putimage(p->m_x, p->m_y, IMG_BACKGROUND_WIDTH, IMG_BACKGROUND_HEIGHT, &p->m_imgBeforeGame, 0, 0, SRCPAINT);
    putimage(p->m_x, p->m_y, IMG_BACKGROUND_WIDTH, IMG_BACKGROUND_HEIGHT, &p->m_imgBeforeGameFront, 0, 0, SRCAND);
}

void BeforeGame_Start(BeforeGame* p)
{
    (void)p;
}
