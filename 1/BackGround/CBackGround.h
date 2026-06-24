#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include<easyx.h>

class CBackGround
{
public:
	IMAGE m_imgBack;
	IMAGE m_imgBg[9]; // Multiple backgrounds (bg_1 to bg_8 + default)
	int m_currentBg;
	int m_x;
	int m_y;

	CBackGround();
	~CBackGround();

	void InitBackGround();
	void ShowBackGround();
	void SwitchBackground();
	// void MoveBackGround(int step);

};