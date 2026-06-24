#include"CColumn.h"
#include"../GameConfig/GameConfig.h"
#include"../BirdAPP/CBirdAPP.h"

// Static member initialization
IMAGE CColumn::m_imgLowStyle[5];
IMAGE CColumn::m_imgLowStyleFront[5];
IMAGE CColumn::m_imgUpStyle[5];
IMAGE CColumn::m_imgUpStyleFront[5];
bool CColumn::m_stylesLoaded = false;

CColumn::CColumn()
{
	m_x = 0;
	m_y = 0;
	m_IfPass = 0;
	m_IfMoving = false;
	m_vy = 1;
	m_styleIndex = 0;
}

CColumn::~CColumn()
{

}

void CColumn::LoadStyles()
{
	if (m_stylesLoaded)
	{
		return; // Already loaded
	}

	// Load multiple column styles only once
	for (int i = 0; i < 5; i++)
	{
		char pathLow[256], pathLowFront[256];
		char pathUp[256], pathUpFront[256];
		sprintf_s(pathLow, "./res/low_style%d.png", i + 1);
		sprintf_s(pathLowFront, "./res/low_style%dFront.png", i + 1);
		sprintf_s(pathUp, "./res/up_style%d.png", i + 1);
		sprintf_s(pathUpFront, "./res/up_style%dFront.png", i + 1);
		loadimage(&m_imgLowStyle[i], pathLow);
		loadimage(&m_imgLowStyleFront[i], pathLowFront);
		loadimage(&m_imgUpStyle[i], pathUp);
		loadimage(&m_imgUpStyleFront[i], pathUpFront);
	}

	m_stylesLoaded = true;
}

void CColumn::InitColumn()
{
	loadimage(&m_imgColumn, "./res/column.png");
	loadimage(&m_imgColumnFront, "./res/columnFront.png");

	// Load styles only once (static)
	LoadStyles();

	m_x = WINDOW_WIDTH + 5;
	m_y = (-CBirdAPP::m_rand % (340)) - 120;
	m_IfPass = 0;
	m_IfMoving = ((CBirdAPP::m_rand % 100) < 25);
	m_vy = (CBirdAPP::m_rand % 2 == 0) ? 1 : -1;
	m_styleIndex = CBirdAPP::m_rand % 5; // Random style selection
}

void CColumn::ShowColumn()
{
	// Use styled columns with proper dimensions
	int gapTop = m_y + IMG_HALF_COLUMN_HEIGHT;
	int gapBottom = m_y + IMG_COLUMN_DOWN_Y;

	// Get actual image dimensions from the loaded images
	int upW = m_imgUpStyle[m_styleIndex].getwidth();
	int upH = m_imgUpStyle[m_styleIndex].getheight();
	int lowW = m_imgLowStyle[m_styleIndex].getwidth();
	int lowH = m_imgLowStyle[m_styleIndex].getheight();

	// Draw upper column (up_style) - from top of column to gap
	putimage(m_x, m_y, upW, upH, &m_imgUpStyle[m_styleIndex], 0, 0, SRCPAINT);
	putimage(m_x, m_y, upW, upH, &m_imgUpStyleFront[m_styleIndex], 0, 0, SRCAND);

	// Draw lower column (low_style) - from gap to bottom
	putimage(m_x, gapBottom, lowW, lowH, &m_imgLowStyle[m_styleIndex], 0, 0, SRCPAINT);
	putimage(m_x, gapBottom, lowW, lowH, &m_imgLowStyleFront[m_styleIndex], 0, 0, SRCAND);
}

void CColumn::MoveColumn(int step)
{
	m_x -= step;
	if (m_IfMoving)
	{
		m_y += m_vy;
		if (m_y <= -480)
		{
			m_y = -480;
			m_vy = 1;
		}
		else if (m_y >= -80)
		{
			m_y = -80;
			m_vy = -1;
		}
	}
}

bool CColumn::IfTouchBird(CPlayerBird* player)
{
	return IfTouchBirdBox(player->m_x, player->m_y, IMG_BIRD_WIDTH, IMG_BIRD_HEIGHT);
}

bool CColumn::IfTouchBirdBox(int birdX, int birdY, int birdW, int birdH)
{
	if (m_x <= birdX + birdW * 3 / 4 && m_x + IMG_COLUMN_WIDTH >= birdX)
	{
		int birdTop = birdY + birdH * 1 / 4;
		int birdBottom = birdY + birdH * 3 / 4;
		int gapTop = m_y + IMG_HALF_COLUMN_HEIGHT;
		int gapBottom = m_y + IMG_COLUMN_DOWN_Y;
		if (gapTop >= birdTop || gapBottom <= birdBottom)
		{
			return true;
		}
	}
	return false;
}
