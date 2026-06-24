#pragma once
#include<graphics.h>
#include"../GameConfig/GameConfig.h"

typedef struct PlayerBird PlayerBird;

typedef struct {
    IMAGE  m_imgGround;
    int m_x;
    int m_y;
} Ground;

void Ground_Init(Ground* p);
void Ground_Show(Ground* p);
void Ground_Move(Ground* p, int step);
int  Ground_IfTouch(Ground* p, PlayerBird* bird);
