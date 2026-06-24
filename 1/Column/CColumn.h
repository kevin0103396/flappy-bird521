#pragma once
#include<easyx.h>
#include"../PlayerBird/CPlayerBird.h"

class CColumn
{
public:
	IMAGE m_imgColumn;
	IMAGE m_imgColumnFront;
	static IMAGE m_imgLowStyle[5];      // Lower column styles (static, loaded once)
	static IMAGE m_imgLowStyleFront[5]; // Lower column style masks (static)
	static IMAGE m_imgUpStyle[5];       // Upper column styles (static)
	static IMAGE m_imgUpStyleFront[5];  // Upper column style masks (static)
	static bool m_stylesLoaded;         // Flag to check if styles are loaded
	int m_styleIndex;            // Current style (0-4)
	int m_x;
	int m_y;
	bool m_IfPass;
	bool m_IfMoving;
	int m_vy;

	CColumn();
	~CColumn();
	static void LoadStyles();    // Static method to load styles once
	void InitColumn();
	void ShowColumn();
	void MoveColumn(int step);
	bool IfTouchBird(CPlayerBird* player);
	bool IfTouchBirdBox(int birdX, int birdY, int birdW, int birdH);
};
