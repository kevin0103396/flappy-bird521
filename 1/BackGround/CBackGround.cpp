#include"CBackGround.h"
#include"../GameConfig/GameConfig.h"
#include <cstdlib>
#include <ctime>

CBackGround::CBackGround()
{
	m_x = 0;
	m_y = 0;
	m_currentBg = 0;
}

CBackGround::~CBackGround()
{
}

void CBackGround::InitBackGround()
{
	loadimage(&m_imgBack, "./res/background.png");

	// Load multiple backgrounds
	for (int i = 0; i < 8; i++)
	{
		char path[256];
		sprintf_s(path, "./res/bg_%d.png", i + 1);
		loadimage(&m_imgBg[i], path);
	}
	m_imgBg[8] = m_imgBack; // Default background as last one

	m_currentBg = rand() % 9;
	m_x = 0;
	m_y = 0;
}

void CBackGround::ShowBackGround()
{
	putimage(m_x, m_y, &m_imgBg[m_currentBg]);
}

void CBackGround::SwitchBackground()
{
	m_currentBg = rand() % 9;
}

//void CBackGround::MoveBackGround(int step)
//{
//	
//		m_x -= step;
//	
//	if(m_x<=-IMG_BACKGROUND_WIDTH)
//	{
//		m_x = 0;
//	}
//}
