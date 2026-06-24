#pragma once
#include<graphics.h>

typedef struct PlayerBird {
    IMAGE m_imgPlayer[8];
    IMAGE m_imgPlayerFront[8];
    int m_x;
    int m_y;
    int m_speed;
    int m_mode;
    int m_a;
} PlayerBird;

void PlayerBird_Init(PlayerBird* p);
void PlayerBird_Move(PlayerBird* p);
void PlayerBird_Show(PlayerBird* p);
