#pragma once
#include<graphics.h>

typedef struct {
    IMAGE m_imgBack[8];
    int m_x;
    int m_y;
    int m_idx;
} BackGround;

void BackGround_Init(BackGround* p);
void BackGround_Show(BackGround* p);
void BackGround_Move(BackGround* p, int step);
