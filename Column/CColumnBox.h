#pragma once
#include"../GameConfig/list.h"
#include "CColumn.h"

typedef struct {
    List m_ColLst;
} ColumnBox;

void ColumnBox_Init(ColumnBox* p);
void ColumnBox_ShowAll(ColumnBox* p);
void ColumnBox_MoveAll(ColumnBox* p, int step);
void ColumnBox_Clear(ColumnBox* p);
