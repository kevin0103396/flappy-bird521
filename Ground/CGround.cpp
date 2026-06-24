#include "CGround.h"
#include "..\PlayerBird\CPlayerBird.h"

void Ground_Init(Ground* p)
{
    p->m_x = 0;
    p->m_y = IMG_GROUND_POS_Y;
    loadimage(&p->m_imgGround, "./res/ground/ground.png");
}

void Ground_Show(Ground* p)
{
    putimage(p->m_x, p->m_y, &p->m_imgGround);
}

void Ground_Move(Ground* p, int step)
{
    p->m_x -= step;
    if (p->m_x <= -IMG_GROUND_WIDTH) {
        p->m_x = 0;
    }
}

int Ground_IfTouch(Ground* p, PlayerBird* bird)
{
    if (bird->m_y + IMG_BIRD_HEIGHT * 3 / 4 >= p->m_y) {
        return 1;
    }
    return 0;
}
