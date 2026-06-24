#pragma once
#include<graphics.h>
#include"../GameConfig/GameConfig.h"

typedef struct {
    IMAGE m_imgBeforeGame;
    IMAGE m_imgBeforeGameFront;
    int m_x;
    int m_y;
} BeforeGame;

void BeforeGame_Init(BeforeGame* p);
void BeforeGame_Show(BeforeGame* p);
void BeforeGame_Start(BeforeGame* p);
