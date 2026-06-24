#include "CPlayerBird.h"
#include"../GameConfig/GameConfig.h"

void PlayerBird_Init(PlayerBird* p)
{
    loadimage(&p->m_imgPlayer[0], "./res/dragon/0.png");
    loadimage(&p->m_imgPlayer[1], "./res/dragon/1.png");
    loadimage(&p->m_imgPlayer[2], "./res/dragon/2.png");
    loadimage(&p->m_imgPlayer[3], "./res/dragon/3.png");
    loadimage(&p->m_imgPlayer[4], "./res/dragon/4.png");
    loadimage(&p->m_imgPlayer[5], "./res/dragon/5.png");
    loadimage(&p->m_imgPlayer[6], "./res/dragon/6.png");
    loadimage(&p->m_imgPlayer[7], "./res/dragon/7.png");

    loadimage(&p->m_imgPlayerFront[0], "./res/dragon/0Front.png");
    loadimage(&p->m_imgPlayerFront[1], "./res/dragon/1Front.png");
    loadimage(&p->m_imgPlayerFront[2], "./res/dragon/2Front.png");
    loadimage(&p->m_imgPlayerFront[3], "./res/dragon/3Front.png");
    loadimage(&p->m_imgPlayerFront[4], "./res/dragon/4Front.png");
    loadimage(&p->m_imgPlayerFront[5], "./res/dragon/5Front.png");
    loadimage(&p->m_imgPlayerFront[6], "./res/dragon/6Front.png");
    loadimage(&p->m_imgPlayerFront[7], "./res/dragon/7Front.png");

    p->m_x = BIRD_POS_X;
    p->m_y = BIRD_POS_Y;
    p->m_mode = 0;
    p->m_speed = 0;
    p->m_a = BIRD_A;
}

void PlayerBird_Show(PlayerBird* p)
{
    putimage(p->m_x, p->m_y, IMG_BIRD_WIDTH, IMG_BIRD_HEIGHT,
        &p->m_imgPlayer[p->m_mode], 0, 0, SRCPAINT);
    putimage(p->m_x, p->m_y, IMG_BIRD_WIDTH, IMG_BIRD_HEIGHT,
        &p->m_imgPlayerFront[p->m_mode], 0, 0, SRCAND);
}

void PlayerBird_Move(PlayerBird* p)
{
    p->m_y += p->m_speed;
    if (p->m_speed + p->m_a > BIRD_MAX_DOWN_SPEED) {
        p->m_speed = BIRD_MAX_DOWN_SPEED;
    } else {
        p->m_speed += p->m_a;
    }
}
