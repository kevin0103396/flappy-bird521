#pragma once
#include<graphics.h>

typedef struct PlayerBird PlayerBird;

typedef struct {
    int m_x;
    int m_y;
    int m_style;
    int m_pipeH;
    int m_IfPass;
} Column;

void Column_Init(Column* p);
void Column_Show(Column* p);
void Column_Move(Column* p, int step);
int  Column_IfTouch(Column* p, PlayerBird* bird);
