#include "CAfterGame.h"
#include <string>

void CAfterGame::InitAfterGame()
{
	m_x = 0;
	m_y = 0;
	loadimage(&m_imgAfterGame, "./res/gameover.png");
	loadimage(&m_imgAfterGameFront, "./res/gameoverFront.png");
}

void CAfterGame::ShowAfterGame()
{
	putimage(m_x, m_y, IMG_BACKGROUND_WIDTH, IMG_BACKGROUND_HEIGHT, &m_imgAfterGame, 0, 0, SRCPAINT);
	putimage(m_x, m_y, IMG_BACKGROUND_WIDTH, IMG_BACKGROUND_HEIGHT, &m_imgAfterGameFront, 0, 0, SRCAND);

	// 显示"早八迟到了"文字
	std::string msg = "早八迟到了!";
	RECT rect = { 0, 250, IMG_BACKGROUND_WIDTH, 350 };
	::settextcolor(RGB(255, 50, 50));
	settextstyle(48, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext(msg.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CAfterGame::ReStartGame()
{
}
