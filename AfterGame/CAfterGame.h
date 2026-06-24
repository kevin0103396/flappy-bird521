#pragma once
#include<graphics.h>
#include"../GameConfig/GameConfig.h"

typedef struct {
    IMAGE m_imgAfterGame;
    IMAGE m_imgAfterGameFront;
    int m_x;
    int m_y;
} AfterGame;

void AfterGame_Init(AfterGame* p);
void AfterGame_Show(AfterGame* p);
void AfterGame_ReStart(AfterGame* p);
