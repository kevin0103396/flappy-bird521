#include "CPlayerBird.h"
#include"../GameConfig/GameConfig.h"


CPlayerBird::CPlayerBird()
{
	m_mode = 0;
	m_speed = 0;
	m_y = 0;
	m_x = 0;
	//m_direct = 0;
	m_a = 0; // ���ٶ�
}

CPlayerBird::~CPlayerBird()
{
}

void CPlayerBird::InitPlayerBird()
{
	loadimage(&m_imgPlayer[0], "./res/0.png");
	loadimage(&m_imgPlayer[1], "./res/1.png");
	loadimage(&m_imgPlayer[2], "./res/2.png");
	loadimage(&m_imgPlayer[3], "./res/3.png");
	loadimage(&m_imgPlayer[4], "./res/4.png");
	loadimage(&m_imgPlayer[5], "./res/5.png");
	loadimage(&m_imgPlayer[6], "./res/6.png");
	loadimage(&m_imgPlayer[7], "./res/7.png");

	loadimage(&m_imgPlayerFront[0], "./res/0Front.png");
	loadimage(&m_imgPlayerFront[1], "./res/1Front.png");
	loadimage(&m_imgPlayerFront[2], "./res/2Front.png");
	loadimage(&m_imgPlayerFront[3], "./res/3Front.png");
	loadimage(&m_imgPlayerFront[4], "./res/4Front.png");
	loadimage(&m_imgPlayerFront[5], "./res/5Front.png");
	loadimage(&m_imgPlayerFront[6], "./res/6Front.png");
	loadimage(&m_imgPlayerFront[7], "./res/7Front.png");

	m_x = BIRD_POS_X;
	m_y = BIRD_POS_Y;
	m_mode = 0;
	m_speed = 0;
	//m_direct = 0;
	m_a = BIRD_A; // ���ٶ�
}

void CPlayerBird::ShowPlayerBird(int displayW, int displayH)
{
	putimage(m_x, m_y, displayW, displayH, &m_imgPlayer[m_mode], 0, 0, SRCPAINT);
	putimage(m_x, m_y, displayW, displayH, &m_imgPlayerFront[m_mode], 0, 0, SRCAND);
}

void CPlayerBird::ShowPlayerBirdGhost(int displayW, int displayH)
{
	// Ghost mode: semi-transparent effect using XOR
	putimage(m_x, m_y, displayW, displayH, &m_imgPlayer[m_mode], 0, 0, SRCPAINT);
	putimage(m_x, m_y, displayW, displayH, &m_imgPlayerFront[m_mode], 0, 0, SRCAND);

	// Draw ghost outline
	setlinecolor(RGB(120, 80, 200));
	rectangle(m_x + 2, m_y + 2, m_x + displayW - 2, m_y + displayH - 2);
}

void CPlayerBird::MovePlayerBird()
{
	// 先更新速度，再移动位置
	m_speed += m_a;
	if (m_speed > BIRD_MAX_DOWN_SPEED)
	{
		m_speed = BIRD_MAX_DOWN_SPEED;
	}
	m_y += m_speed;
}


