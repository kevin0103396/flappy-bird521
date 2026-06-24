#include"CBirdAPP.h"
#include"../GameConfig/GameConfig.h"
#include<time.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

WND_PARAM(200, 100, WINDOW_WIDTH, WINDOW_HEIGHT, "FlappyBird")
DYNAMIC_CREATE(CBirdAPP)

int CBirdAPP::m_rand = 0;

CBirdAPP::CBirdAPP()
{
	m_GameStatus = 0; // ��ʼ״̬Ϊ��δ��ʼ��Ϸ
	m_score = 0;
	m_Paused = false;
	m_bestScore = 0;
	m_speedStep = TIMER_GROUND_MOVE_STEP;
	m_columnInterval = TIMER_NEW_COLUMN_INTERVAL;
	m_hasShield = false;
	m_shieldOnMap = false;
	m_shieldX = 0;
	m_shieldY = 0;
	m_lastShieldSpawnScore = -1;
	m_floatMsg = "";
	m_floatMsgTicks = 0;
	m_coins = 0;
	m_coinOnMap = false;
	m_coinX = 0;
	m_coinY = 0;
	m_lastCoinSpawnScore = -1;
	m_doubleJumpOnMap = false;
	m_doubleJumpX = 0;
	m_doubleJumpY = 0;
	m_lastDoubleJumpSpawnScore = -1;
	m_doubleJumpTicks = 0;
	m_flapCooldownTicks = 0;
	m_perfectStreak = 0;
	m_perfectWindow = 18;
	m_upgradeStartShield = false;
	m_upgradeDjBonusTicks = 0;
	m_shopMsg = "";
	m_shopMsgTicks = 0;
	m_shopOpen = false;
	m_passStreak = 0;
	m_feverTicks = 0;
	m_nearMissDist = 8;
	m_scoreMultiplierTicks = 0;
	m_slowMoTicks = 0;
	m_ghostTicks = 0;
	m_shrinkTicks = 0;
	m_powerOnMap = false;
	m_powerType = 0;
	m_powerX = 0;
	m_powerY = 0;
	m_lastPowerSpawnScore = -1;
	m_showIntro = true;
	srand((unsigned int)time(NULL));
	LoadBestScore();
	LoadPlayerSave();
}
CBirdAPP::~CBirdAPP()
{

}


void CBirdAPP::On_SetTimer()
{
	StartTimersForStatus();
}


void CBirdAPP::On_Init()
{
	m_Paused = false;
	m_hasShield = false;
	m_shieldOnMap = false;
	m_lastShieldSpawnScore = -1;
	m_floatMsg = "";
	m_floatMsgTicks = 0;
	m_coinOnMap = false;
	m_lastCoinSpawnScore = -1;
	m_doubleJumpOnMap = false;
	m_lastDoubleJumpSpawnScore = -1;
	m_doubleJumpTicks = 0;
	m_flapCooldownTicks = 0;
	m_perfectStreak = 0;
	m_shopMsg = "";
	m_shopMsgTicks = 0;
	m_shopOpen = false;
	m_passStreak = 0;
	m_feverTicks = 0;
	m_scoreMultiplierTicks = 0;
	m_slowMoTicks = 0;
	m_ghostTicks = 0;
	m_shrinkTicks = 0;
	m_powerOnMap = false;
	m_lastPowerSpawnScore = -1;

	if (m_GameStatus == 0)
	{
		m_score = 0;
		m_speedStep = TIMER_GROUND_MOVE_STEP;
		m_columnInterval = TIMER_NEW_COLUMN_INTERVAL;

		m_ColumnBox.ClearAll();
		m_BackGround.InitBackGround();
		m_Ground.InitGround();
		m_PlayerBird.InitPlayerBird();
		m_BeforeGame.InitBeforeGame();
		m_AfterGame.InitAfterGame();

		// Initialize intro video
		if (m_showIntro)
		{
			m_IntroVideo.InitIntroVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
			m_IntroVideo.Reset();
			m_showIntro = false;
			// 播放开场音乐
			CSoundManager::GetInstance().PlayIntroMusic();
		}

		StartTimersForStatus();
		return;
	}

	if (m_GameStatus == 1)
	{
		if (m_upgradeStartShield)
		{
			m_hasShield = true;
			m_floatMsg = "START SHIELD";
			m_floatMsgTicks = 60;
		}
		// Switch background when starting game
		m_BackGround.SwitchBackground();
		UpdateDifficulty();
		StartTimersForStatus();
		return;
	}
}


void CBirdAPP::On_Paint()
{
	// Show intro video if not finished
	if (!m_IntroVideo.IsFinished() && m_GameStatus == 0)
	{
		m_IntroVideo.ShowNextFrame();
		return;
	}

	if (m_GameStatus == 1)
	{
		m_BackGround.ShowBackGround();
		m_ColumnBox.ShowAllColumn();
		m_Ground.ShowGround();
		ShowScore();
		DrawFeverBar();
		DrawShield();
		DrawCoin();
		DrawDoubleJumpItem();
		DrawPower();

		// 根据是否处于穿墙状态显示不同的小鸟效果
		if (IsGhost())
		{
			m_PlayerBird.ShowPlayerBirdGhost(GetBirdW(), GetBirdH());
		}
		else
		{
			m_PlayerBird.ShowPlayerBird(GetBirdW(), GetBirdH());
		}

		DrawFloatMessage();

		if (m_Paused)
		{
			std::string s = "已暂停";
			RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			::settextcolor(RGB(60, 60, 60));
			settextstyle(60, 0, _T("微软雅黑"));
			setbkmode(TRANSPARENT);
			::drawtext(s.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else if (m_GameStatus == 0)
	{
		m_BackGround.ShowBackGround();
		m_Ground.ShowGround();
		m_BeforeGame.ShowBeforeGame();
		m_PlayerBird.ShowPlayerBird(GetBirdW(), GetBirdH());
		ShowScore();
		DrawShield();
		DrawCoin();
		DrawDoubleJumpItem();
		DrawPower();
		DrawFloatMessage();
		DrawShop();
	}
	else if (m_GameStatus == 2)
	{
		m_BackGround.ShowBackGround();
		m_ColumnBox.ShowAllColumn();
		m_Ground.ShowGround();
		m_PlayerBird.ShowPlayerBird(GetBirdW(), GetBirdH());
		m_AfterGame.ShowAfterGame();
		ShowScore();
		DrawShield();
		DrawCoin();
		DrawDoubleJumpItem();
		DrawPower();
		DrawFloatMessage();
	}
}


void CBirdAPP::On_Close()
{
	StopAllTimers();
	m_ColumnBox.ClearAll();
}


void CBirdAPP::On_WM_LBUTTONDOWN(POINT& po)
{
	// 跳过开场动画
	if (!m_IntroVideo.IsFinished() && m_GameStatus == 0)
	{
		m_IntroVideo.Skip();
		return;
	}

	if (m_GameStatus == 1)
	{
		TryFlap();
	}
	else if (m_GameStatus == 0)
	{
		if (po.x >= 20 && po.x <= 160 && po.y >= 650 && po.y <= 700)
		{
			m_shopOpen = !m_shopOpen;
			if (m_shopOpen)
			{
				m_shopMsg = "商店已打开";
				m_shopMsgTicks = 60;
			}
			return;
		}

		if (m_shopOpen)
		{
			if (po.x >= 20 && po.x <= WINDOW_WIDTH - 20 && po.y >= 520 && po.y <= 560)
			{
				BuyStartShield();
				return;
			}
			if (po.x >= 20 && po.x <= WINDOW_WIDTH - 20 && po.y >= 565 && po.y <= 605)
			{
				BuyPerfectPlus();
				return;
			}
			if (po.x >= 20 && po.x <= WINDOW_WIDTH - 20 && po.y >= 610 && po.y <= 650)
			{
				BuyDjPlus();
				return;
			}
		}
	}

	if (m_GameStatus == 0 && po.x >= BUTTON_POS_X_1 && po.x <= BUTTON_POS_X_2 && po.y >= BUTTON_POS_Y_1 && po.y <= BUTTON_POS_Y_2)
	{
		m_GameStatus = 1;
		On_Init();
	}
	else if (m_GameStatus == 2 && po.x >= BUTTON2_POS_X_1 && po.x <= BUTTON2_POS_X_2 && po.y >= BUTTON2_POS_Y_1 && po.y <= BUTTON2_POS_Y_2)
	{
		m_GameStatus = 0;
		On_Init();
	}
}


void CBirdAPP::On_WM_TIMER(WPARAM wParam, LPARAM lParam)
{
	// 暂停状态下不处理游戏逻辑计时器
	if (m_Paused && wParam == TIMER_GROUND_MOVE_TIMERID)
	{
		return;
	}

	switch (wParam)
	{
	case TIMER_GROUND_MOVE_TIMERID:
	{
		TickShopMsg();
		if (m_feverTicks > 0)
		{
			m_feverTicks--;
		}
		if (m_scoreMultiplierTicks > 0)
		{
			m_scoreMultiplierTicks--;
		}
		if (m_slowMoTicks > 0)
		{
			m_slowMoTicks--;
		}
		if (m_ghostTicks > 0)
		{
			m_ghostTicks--;
		}
		if (m_shrinkTicks > 0)
		{
			m_shrinkTicks--;
		}

		if (m_doubleJumpTicks > 0)
		{
			m_doubleJumpTicks--;
			if (m_doubleJumpTicks == 0)
			{
				m_floatMsg = "⬆️二段跳结束";
				m_floatMsgTicks = 40;
			}
		}

		int worldStep = m_speedStep;
		if (m_slowMoTicks > 0)
		{
			worldStep = std::max(1, worldStep / 2);
		}

		m_Ground.MoveGround(worldStep);
		m_ColumnBox.MoveAllColumn(worldStep);

		UpdateShield();
		CheckShieldPickup();
		SpawnShieldIfNeeded();

		UpdateCoin();
		CheckCoinPickup();
		SpawnCoinIfNeeded();

		UpdateDoubleJumpItem();
		CheckDoubleJumpPickup();
		SpawnDoubleJumpIfNeeded();

		UpdatePower();
		CheckPowerPickup();
		SpawnPowerIfNeeded();

		list<CColumn*>::iterator ite = m_ColumnBox.m_ColLst.begin();
		while (ite != m_ColumnBox.m_ColLst.end())
		{
			if (*ite)
			{
				int bw = GetBirdW();
				int bh = GetBirdH();
				if (!IsGhost() && (*ite)->IfTouchBirdBox(m_PlayerBird.m_x, m_PlayerBird.m_y, bw, bh))
				{
					if (m_hasShield)
					{
						m_hasShield = false;
						m_floatMsg = "🛡️护盾破碎!";
						m_floatMsgTicks = 60;
						m_PlayerBird.m_speed = BIRD_STARTSPEED;
						m_passStreak = 0;
						m_perfectStreak = 0;
						// 播放护盾破碎音效
						CSoundManager::GetInstance().PlayShieldBreak();
					}
					else
					{
						StopAllTimers();
						m_Paused = false;
						m_GameStatus = 2;
						UpdateBestScoreIfNeeded();
						// 播放游戏结束音效
						CSoundManager::GetInstance().PlayGameOver();
						return;
					}
				}
				if ((*ite)->m_x + IMG_COLUMN_WIDTH <= m_PlayerBird.m_x&&!(*ite)->m_IfPass)
				{
					(*ite)->m_IfPass = true;
					m_passStreak++;

					// 播放得分音效
					CSoundManager::GetInstance().PlayScore();

					if (m_passStreak >= 10)
					{
						m_passStreak = 0;
						m_feverTicks = 250;
						m_floatMsg = "🔥狂热模式!";
						m_floatMsgTicks = 60;
						// 播放狂热音效
						CSoundManager::GetInstance().PlayFever();
					}

					m_score += 1 * GetScoreMultiplier();

					int gapTop = (*ite)->m_y + IMG_HALF_COLUMN_HEIGHT;
					int gapBottom = (*ite)->m_y + IMG_COLUMN_DOWN_Y;
					int gapCenter = (gapTop + gapBottom) / 2;
					int birdCenter = m_PlayerBird.m_y + IMG_BIRD_HEIGHT / 2;
					int diff = birdCenter - gapCenter;
					if (diff < 0) diff = -diff;
					if (diff <= m_perfectWindow)
					{
						m_perfectStreak++;
						int streak = m_perfectStreak;
						if (streak > 5) streak = 5;
						int bonus = 2 * streak;
						m_score += bonus * GetScoreMultiplier();
						m_floatMsg = "✨完美 x" + std::to_string(m_perfectStreak) + " +" + std::to_string(bonus);
						m_floatMsgTicks = 60;
						// 播放完美音效
						CSoundManager::GetInstance().PlayPerfect();
					}
					else
					{
						m_perfectStreak = 0;
					}

					int bw2 = GetBirdW();
					int bh2 = GetBirdH();
					int bTop = m_PlayerBird.m_y + bh2 / 4;
					int bBot = m_PlayerBird.m_y + bh2 * 3 / 4;
					int nearTop = std::abs(bTop - gapTop);
					int nearBot = std::abs(gapBottom - bBot);
					if (nearTop <= m_nearMissDist || nearBot <= m_nearMissDist)
					{
						m_score += 1 * GetScoreMultiplier();
						m_floatMsg = "💨险过 +1";
						m_floatMsgTicks = 40;
						// 播放过音效
						CSoundManager::GetInstance().PlayNearMiss();
						if (m_passStreak < 10)
						{
							m_passStreak++;
						}
					}

					UpdateDifficulty();
				}
			}
			ite++;
		}
	}
	break;
	case TIMER_NEW_COLUMN_TIMERID:
	{
		m_rand = rand();  // 确保每次创建Column前更新随机数
		CColumn* newCol = new CColumn;
		newCol->InitColumn();
		m_ColumnBox.m_ColLst.push_back(newCol);
	}
	break;
	case TIMER_BIRD_CHANGE_TIMERID:
	{
		if (m_PlayerBird.m_mode + 1 <= 7)
		{
			m_PlayerBird.m_mode++;
		}
		else
		{
			m_PlayerBird.m_mode = 0;
		}
	}
	break;
	case TIMER_BIRD_SHOW_TIMERID:
	{

	}
	break;
	case TIMER_BIRD_POSITION_TIMERID:
	{
		if (m_flapCooldownTicks > 0)
		{
			m_flapCooldownTicks--;
		}
		m_PlayerBird.MovePlayerBird();
		// ����Ƿ���������
		int bh = GetBirdH();
		bool touchGround = (m_PlayerBird.m_y + bh * 3 / 4 >= m_Ground.m_y);
		if (!IsGhost() && touchGround)
		{
			if (m_hasShield)
			{
				m_hasShield = false;
				m_floatMsg = "SHIELD!";
				m_floatMsgTicks = 60;
				m_PlayerBird.m_speed = BIRD_STARTSPEED;
				m_passStreak = 0;
				m_perfectStreak = 0;
			}
			else
			{
				StopAllTimers();
				m_Paused = false;
				m_GameStatus = 2;
				UpdateBestScoreIfNeeded();
				return;
			}
		}
	}
	break;
	}
}



void CBirdAPP::ReStartStop()
{
	StopAllTimers();
}

void CBirdAPP::ShowScore()
{
	string strScore = std::to_string(m_score);

	RECT rect = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	::settextcolor(RGB(150, 100, 99));
	settextstyle(60, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext(strScore.c_str(), &rect, DT_CENTER | DT_SINGLELINE);

	// 只显示道具状态，隐藏调试信息
	string extra("");
	if (m_hasShield)
	{
		extra += "🛡️护盾 ";
	}
	if (IsGhost())
	{
		extra += "👻穿墙 ";
	}
	if (m_shrinkTicks > 0)
	{
		extra += "🔬缩小 ";
	}
	if (m_slowMoTicks > 0)
	{
		extra += "⏱️慢速 ";
	}
	if (GetScoreMultiplier() > 1)
	{
		extra += "✖️双倍 ";
	}
	if (m_feverTicks > 0)
	{
		extra += "🔥狂热 ";
	}
	if (m_doubleJumpTicks > 0)
	{
		extra += "⬆️二段跳 ";
	}

	// 显示金币数量
	string coinStr = "🪙 " + std::to_string(m_coins);
	RECT rectCoin = { WINDOW_WIDTH - 100, 10, WINDOW_WIDTH, 40 };
	::settextcolor(RGB(240, 180, 0));
	settextstyle(20, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext(coinStr.c_str(), &rectCoin, DT_RIGHT | DT_SINGLELINE);

	if (!extra.empty())
	{
		RECT rectExtra = { 10, 10, WINDOW_WIDTH - 110, 40 };
		::settextcolor(RGB(80, 80, 80));
		settextstyle(20, 0, _T("微软雅黑"));
		setbkmode(TRANSPARENT);
		::drawtext(extra.c_str(), &rectExtra, DT_LEFT | DT_SINGLELINE);
	}

	if (m_GameStatus == 0 || m_GameStatus == 2)
	{
		string best("");
		char bc[16] = { 0 };
		_itoa_s(m_bestScore, bc, 10);
		best += "最高分: ";
		best += bc;

		RECT rectBest = { 0, 70, WINDOW_WIDTH, 140 };
		::settextcolor(RGB(80, 80, 80));
		settextstyle(30, 0, _T("微软雅黑"));
		setbkmode(TRANSPARENT);
		::drawtext(best.c_str(), &rectBest, DT_CENTER | DT_SINGLELINE);
	}
}

void CBirdAPP::SpawnShieldIfNeeded()
{
	if (m_GameStatus != 1)
	{
		return;
	}
	if (m_Paused)
	{
		return;
	}
	if (m_hasShield || m_shieldOnMap)
	{
		return;
	}
	if (m_score <= 0)
	{
		return;
	}
	if (m_score % 8 != 0)
	{
		return;
	}
	if (m_lastShieldSpawnScore == m_score)
	{
		return;
	}

	m_lastShieldSpawnScore = m_score;
	m_shieldOnMap = true;
	m_shieldX = WINDOW_WIDTH + 30;
	int top = 50;
	int bottom = IMG_GROUND_POS_Y - 80;
	if (bottom <= top) bottom = top + 1;
	int r = rand();
	int span = bottom - top;
	m_shieldY = top + (r % span);
}

void CBirdAPP::UpdateShield()
{
	if (!m_shieldOnMap)
	{
		return;
	}
	m_shieldX -= m_speedStep;
	if (m_shieldX < -40)
	{
		m_shieldOnMap = false;
	}
}

bool CBirdAPP::CheckShieldPickup()
{
	if (!m_shieldOnMap)
	{
		return false;
	}
	int r = 14;
	int left = m_shieldX - r;
	int right = m_shieldX + r;
	int top = m_shieldY - r;
	int bottom = m_shieldY + r;

	int birdLeft = m_PlayerBird.m_x;
	int birdRight = m_PlayerBird.m_x + IMG_BIRD_WIDTH;
	int birdTop = m_PlayerBird.m_y;
	int birdBottom = m_PlayerBird.m_y + IMG_BIRD_HEIGHT;

	bool hit = !(right < birdLeft || left > birdRight || bottom < birdTop || top > birdBottom);
	if (hit)
	{
		m_shieldOnMap = false;
		m_hasShield = true;
		m_floatMsg = "🛡️获得护盾!";
		m_floatMsgTicks = 60;
		// 播放道具音效
		CSoundManager::GetInstance().PlayPowerUp();
		return true;
	}
	return false;
}

void CBirdAPP::DrawShield()
{
	if (!m_shieldOnMap)
	{
		return;
	}
	int r = 14;
	setlinecolor(RGB(255, 180, 0));
	circle(m_shieldX, m_shieldY, r);
	circle(m_shieldX, m_shieldY, r - 2);
	RECT rectS = { m_shieldX - 10, m_shieldY - 10, m_shieldX + 10, m_shieldY + 10 };
	::settextcolor(RGB(255, 120, 0));
	settextstyle(20, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext("S", &rectS, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CBirdAPP::DrawFloatMessage()
{
	if (m_floatMsgTicks <= 0)
	{
		return;
	}
	m_floatMsgTicks--;
	RECT rectMsg = { 0, 110, WINDOW_WIDTH, 160 };
	::settextcolor(RGB(60, 60, 60));
	settextstyle(28, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext(m_floatMsg.c_str(), &rectMsg, DT_CENTER | DT_SINGLELINE);
}

void CBirdAPP::On_WM_KEYDOWN(WPARAM wparam)
{
	// 跳过开场动画
	if (!m_IntroVideo.IsFinished() && m_GameStatus == 0)
	{
		m_IntroVideo.Skip();
		return;
	}

	if (m_GameStatus == 0)
	{
		if (wparam == '1')
		{
			BuyStartShield();
			return;
		}
		if (wparam == '2')
		{
			BuyPerfectPlus();
			return;
		}
		if (wparam == '3')
		{
			BuyDjPlus();
			return;
		}
	}

	if (wparam == 'P' || wparam == 'p')
	{
		if (m_GameStatus == 1)
		{
			m_Paused = !m_Paused;
			if (m_Paused)
			{
				StopAllTimers();
			}
			else
			{
				StartTimersForStatus();
			}
		}
		return;
	}

	if (wparam == 'R' || wparam == 'r')
	{
		m_Paused = false;
		m_GameStatus = 0;
		On_Init();
		return;
	}

	if (wparam == 'E' || wparam == 'e')
	{
		if (m_GameStatus == 1 && !m_Paused)
		{
			const int cost = 5;
			if (m_doubleJumpTicks <= 0 && m_coins >= cost)
			{
				m_coins -= cost;
				SavePlayerSave();
				ActivateDoubleJump(600, "⬆️购买二段跳!");
				// 播放购买音效
				CSoundManager::GetInstance().PlayBuyItem();
			}
			else if (m_coins < cost)
			{
				m_floatMsg = "需要5个金币";
				m_floatMsgTicks = 50;
				// 播放错误音效
				CSoundManager::GetInstance().PlayError();
			}
		}
		return;
	}

	if (wparam == VK_SPACE || wparam == VK_UP)
	{
		TryFlap();
		return;
	}
}

void CBirdAPP::TryFlap()
{
	if (m_GameStatus != 1)
	{
		return;
	}
	if (m_Paused)
	{
		return;
	}

	int cooldown = (m_doubleJumpTicks > 0) ? 1 : 4;
	if (m_flapCooldownTicks > 0)
	{
		return;
	}
	m_flapCooldownTicks = cooldown;

	int speed = (m_doubleJumpTicks > 0) ? (BIRD_STARTSPEED - 2) : BIRD_STARTSPEED;
	m_PlayerBird.m_speed = speed;

	// 播放拍翅膀音效
	CSoundManager::GetInstance().PlayFlap();
}

bool CBirdAPP::IsGhost() const
{
	return m_ghostTicks > 0;
}

int CBirdAPP::GetBirdW() const
{
	if (m_shrinkTicks > 0)
	{
		return (int)(IMG_BIRD_WIDTH * 0.7);
	}
	return IMG_BIRD_WIDTH;
}

int CBirdAPP::GetBirdH() const
{
	if (m_shrinkTicks > 0)
	{
		return (int)(IMG_BIRD_HEIGHT * 0.7);
	}
	return IMG_BIRD_HEIGHT;
}

int CBirdAPP::GetScoreMultiplier() const
{
	if (m_feverTicks > 0)
	{
		return 2;
	}
	if (m_scoreMultiplierTicks > 0)
	{
		return 2;
	}
	return 1;
}

void CBirdAPP::SpawnPowerIfNeeded()
{
	if (m_GameStatus != 1 || m_Paused)
	{
		return;
	}
	if (m_powerOnMap)
	{
		return;
	}
	if (m_score <= 0)
	{
		return;
	}
	if (m_score % 7 != 0)
	{
		return;
	}
	if (m_lastPowerSpawnScore == m_score)
	{
		return;
	}
	m_lastPowerSpawnScore = m_score;
	m_powerOnMap = true;
	m_powerX = WINDOW_WIDTH + 20;
	int top = 80;
	int bottom = IMG_GROUND_POS_Y - 140;
	if (bottom <= top) bottom = top + 1;
	int span = bottom - top;
	m_powerY = top + (rand() % span);
	int r = rand() % 4;
	m_powerType = r; // 0 ghost,1 shrink,2 slow,3 x2
}

void CBirdAPP::UpdatePower()
{
	if (!m_powerOnMap)
	{
		return;
	}
	int step = m_speedStep;
	if (m_slowMoTicks > 0)
	{
		step = std::max(1, step / 2);
	}
	m_powerX -= step;
	if (m_powerX < -40)
	{
		m_powerOnMap = false;
	}
}

bool CBirdAPP::CheckPowerPickup()
{
	if (!m_powerOnMap)
	{
		return false;
	}
	int r = 12;
	int left = m_powerX - r;
	int right = m_powerX + r;
	int top = m_powerY - r;
	int bottom = m_powerY + r;

	int bw = GetBirdW();
	int bh = GetBirdH();
	int birdLeft = m_PlayerBird.m_x;
	int birdRight = m_PlayerBird.m_x + bw;
	int birdTop = m_PlayerBird.m_y;
	int birdBottom = m_PlayerBird.m_y + bh;

	bool hit = !(right < birdLeft || left > birdRight || bottom < birdTop || top > birdBottom);
	if (hit)
	{
		int t = m_powerType;
		m_powerOnMap = false;
		ApplyPower(t);
		return true;
	}
	return false;
}

void CBirdAPP::ApplyPower(int type)
{
	// 播放道具音效
	CSoundManager::GetInstance().PlayPowerUp();

	if (type == 0)
	{
		m_ghostTicks = 250;
		m_floatMsg = "👻穿墙模式!";
		m_floatMsgTicks = 50;
		return;
	}
	if (type == 1)
	{
		m_shrinkTicks = 300;
		m_floatMsg = "🔬缩小模式!";
		m_floatMsgTicks = 50;
		return;
	}
	if (type == 2)
	{
		m_slowMoTicks = 250;
		m_floatMsg = "⏱️慢动作!";
		m_floatMsgTicks = 50;
		return;
	}
	if (type == 3)
	{
		m_scoreMultiplierTicks = 250;
		m_floatMsg = "✖️双倍得分!";
		m_floatMsgTicks = 50;
		return;
	}
}

void CBirdAPP::DrawPower()
{
	if (!m_powerOnMap)
	{
		return;
	}
	int r = 12;
	setlinecolor(RGB(120, 120, 120));
	circle(m_powerX, m_powerY, r);
	circle(m_powerX, m_powerY, r - 2);
	const char* label = "?";
	COLORREF c = RGB(80, 80, 80);
	if (m_powerType == 0) { label = "G"; c = RGB(120, 80, 200); }
	if (m_powerType == 1) { label = "K"; c = RGB(40, 140, 255); }
	if (m_powerType == 2) { label = "T"; c = RGB(60, 160, 60); }
	if (m_powerType == 3) { label = "X"; c = RGB(200, 120, 40); }
	RECT rr = { m_powerX - 10, m_powerY - 10, m_powerX + 10, m_powerY + 10 };
	::settextcolor(c);
	settextstyle(18, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext(label, &rr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CBirdAPP::DrawFeverBar()
{
	if (m_GameStatus != 1)
	{
		return;
	}
	int x1 = 10;
	int y1 = 45;
	int x2 = 210;
	int y2 = 60;
	setlinecolor(RGB(80, 80, 80));
	rectangle(x1, y1, x2, y2);
	int fill = (int)((x2 - x1 - 2) * (m_passStreak / 10.0));
	if (fill < 0) fill = 0;
	if (fill > x2 - x1 - 2) fill = x2 - x1 - 2;
	setlinecolor(RGB(200, 80, 80));
	for (int i = 0; i < fill; i++)
	{
		line(x1 + 1 + i, y1 + 1, x1 + 1 + i, y2 - 1);
	}
	RECT r = { x1, y1 - 18, x2, y1 };
	::settextcolor(RGB(80, 80, 80));
	settextstyle(18, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	std::string s = "STREAK " + std::to_string(m_passStreak) + "/10";
	::drawtext(s.c_str(), &r, DT_LEFT | DT_SINGLELINE);
}

void CBirdAPP::SpawnCoinIfNeeded()
{
	if (m_GameStatus != 1 || m_Paused)
	{
		return;
	}
	if (m_coinOnMap)
	{
		return;
	}
	if (m_score <= 0)
	{
		return;
	}
	if (m_score % 3 != 0)
	{
		return;
	}
	if (m_lastCoinSpawnScore == m_score)
	{
		return;
	}

	m_lastCoinSpawnScore = m_score;
	m_coinOnMap = true;
	m_coinX = WINDOW_WIDTH + 20;
	int top = 60;
	int bottom = IMG_GROUND_POS_Y - 100;
	if (bottom <= top) bottom = top + 1;
	int span = bottom - top;
	m_coinY = top + (rand() % span);
}

void CBirdAPP::UpdateCoin()
{
	if (!m_coinOnMap)
	{
		return;
	}
	m_coinX -= m_speedStep;
	if (m_coinX < -40)
	{
		m_coinOnMap = false;
	}
}

bool CBirdAPP::CheckCoinPickup()
{
	if (!m_coinOnMap)
	{
		return false;
	}
	int r = 10;
	int left = m_coinX - r;
	int right = m_coinX + r;
	int top = m_coinY - r;
	int bottom = m_coinY + r;

	int birdLeft = m_PlayerBird.m_x;
	int birdRight = m_PlayerBird.m_x + IMG_BIRD_WIDTH;
	int birdTop = m_PlayerBird.m_y;
	int birdBottom = m_PlayerBird.m_y + IMG_BIRD_HEIGHT;

	bool hit = !(right < birdLeft || left > birdRight || bottom < birdTop || top > birdBottom);
	if (hit)
	{
		m_coinOnMap = false;
		m_coins++;
		SavePlayerSave();
		m_floatMsg = "🪙金币 +1";
		m_floatMsgTicks = 40;
		// 播放金币音效
		CSoundManager::GetInstance().PlayCoin();
		return true;
	}
	return false;
}

void CBirdAPP::DrawCoin()
{
	if (!m_coinOnMap)
	{
		return;
	}
	int r = 10;
	setlinecolor(RGB(240, 200, 0));
	circle(m_coinX, m_coinY, r);
	circle(m_coinX, m_coinY, r - 2);
	RECT rectC = { m_coinX - 10, m_coinY - 10, m_coinX + 10, m_coinY + 10 };
	::settextcolor(RGB(240, 160, 0));
	settextstyle(18, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext("C", &rectC, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CBirdAPP::SpawnDoubleJumpIfNeeded()
{
	if (m_GameStatus != 1 || m_Paused)
	{
		return;
	}
	if (m_doubleJumpOnMap)
	{
		return;
	}
	if (m_score <= 0)
	{
		return;
	}
	if (m_score % 10 != 0)
	{
		return;
	}
	if (m_lastDoubleJumpSpawnScore == m_score)
	{
		return;
	}
	m_lastDoubleJumpSpawnScore = m_score;
	m_doubleJumpOnMap = true;
	m_doubleJumpX = WINDOW_WIDTH + 20;
	int top = 80;
	int bottom = IMG_GROUND_POS_Y - 120;
	if (bottom <= top) bottom = top + 1;
	int span = bottom - top;
	m_doubleJumpY = top + (rand() % span);
}

void CBirdAPP::UpdateDoubleJumpItem()
{
	if (!m_doubleJumpOnMap)
	{
		return;
	}
	m_doubleJumpX -= m_speedStep;
	if (m_doubleJumpX < -40)
	{
		m_doubleJumpOnMap = false;
	}
}

bool CBirdAPP::CheckDoubleJumpPickup()
{
	if (!m_doubleJumpOnMap)
	{
		return false;
	}
	int r = 12;
	int left = m_doubleJumpX - r;
	int right = m_doubleJumpX + r;
	int top = m_doubleJumpY - r;
	int bottom = m_doubleJumpY + r;

	int birdLeft = m_PlayerBird.m_x;
	int birdRight = m_PlayerBird.m_x + IMG_BIRD_WIDTH;
	int birdTop = m_PlayerBird.m_y;
	int birdBottom = m_PlayerBird.m_y + IMG_BIRD_HEIGHT;

	bool hit = !(right < birdLeft || left > birdRight || bottom < birdTop || top > birdBottom);
	if (hit)
	{
		m_doubleJumpOnMap = false;
		ActivateDoubleJump(900, "⬆️获得二段跳!");
		// 播放道具音效
		CSoundManager::GetInstance().PlayPowerUp();
		return true;
	}
	return false;
}

void CBirdAPP::DrawDoubleJumpItem()
{
	if (!m_doubleJumpOnMap)
	{
		return;
	}
	int r = 12;
	setlinecolor(RGB(60, 140, 255));
	circle(m_doubleJumpX, m_doubleJumpY, r);
	circle(m_doubleJumpX, m_doubleJumpY, r - 2);
	RECT rectD = { m_doubleJumpX - 10, m_doubleJumpY - 10, m_doubleJumpX + 10, m_doubleJumpY + 10 };
	::settextcolor(RGB(40, 100, 220));
	settextstyle(18, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	::drawtext("D", &rectD, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CBirdAPP::ActivateDoubleJump(int ticks, const char* reason)
{
	if (ticks <= 0)
	{
		return;
	}
	m_doubleJumpTicks = ticks + m_upgradeDjBonusTicks;
	m_floatMsg = reason;
	m_floatMsgTicks = 60;
}

void CBirdAPP::DrawShop()
{
	if (m_GameStatus != 0)
	{
		return;
	}

	setlinecolor(RGB(80, 80, 80));
	rectangle(20, 650, 160, 700);
	::settextcolor(RGB(50, 50, 50));
	settextstyle(22, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	RECT rBtn = { 20, 650, 160, 700 };
	::drawtext(m_shopOpen ? "返回" : "商店", &rBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (!m_shopOpen)
	{
		return;
	}

	::settextcolor(RGB(50, 50, 50));
	settextstyle(18, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);

	std::string line1 = "商店 (开始界面)  金币: " + std::to_string(m_coins);
	RECT r1 = { 20, 485, WINDOW_WIDTH - 20, 510 };
	::drawtext(line1.c_str(), &r1, DT_LEFT | DT_SINGLELINE);

	rectangle(20, 520, WINDOW_WIDTH - 20, 560);
	rectangle(20, 565, WINDOW_WIDTH - 20, 605);
	rectangle(20, 610, WINDOW_WIDTH - 20, 650);

	std::string i1 = std::string("1) 开局护盾 20🪙  -> ") + (m_upgradeStartShield ? "已拥有" : "点击购买");
	std::string i2 = std::string("2) Perfect+ (窗口=26) 15🪙  -> ") + ((m_perfectWindow >= 26) ? "已拥有" : "点击购买");
	std::string i3 = std::string("3) DJ+ (+300时间) 25🪙  -> ") + ((m_upgradeDjBonusTicks != 0) ? "已拥有" : "点击购买");
	RECT r2 = { 28, 528, WINDOW_WIDTH - 28, 552 };
	RECT r3 = { 28, 573, WINDOW_WIDTH - 28, 597 };
	RECT r4 = { 28, 618, WINDOW_WIDTH - 28, 642 };
	::drawtext(i1.c_str(), &r2, DT_LEFT | DT_SINGLELINE);
	::drawtext(i2.c_str(), &r3, DT_LEFT | DT_SINGLELINE);
	::drawtext(i3.c_str(), &r4, DT_LEFT | DT_SINGLELINE);

	if (m_shopMsgTicks > 0)
	{
		RECT r5 = { 20, 455, WINDOW_WIDTH - 20, 480 };
		::settextcolor(RGB(80, 80, 80));
		settextstyle(20, 0, _T("微软雅黑"));
		::drawtext(m_shopMsg.c_str(), &r5, DT_LEFT | DT_SINGLELINE);
	}
}

void CBirdAPP::LoadPlayerSave()
{
	std::ifstream in("player_save.txt");
	if (!in.is_open())
	{
		return;
	}
	int coins = 0;
	int startShield = 0;
	int perfectWindow = 18;
	int djBonus = 0;
	in >> coins >> startShield >> perfectWindow >> djBonus;
	if (!in.good())
	{
		return;
	}
	if (coins < 0) coins = 0;
	if (perfectWindow < 18) perfectWindow = 18;
	m_coins = coins;
	m_upgradeStartShield = (startShield != 0);
	m_perfectWindow = perfectWindow;
	m_upgradeDjBonusTicks = djBonus;
}

void CBirdAPP::SavePlayerSave()
{
	std::ofstream out("player_save.txt", std::ios::trunc);
	if (!out.is_open())
	{
		return;
	}
	out << m_coins << " " << (m_upgradeStartShield ? 1 : 0) << " " << m_perfectWindow << " " << m_upgradeDjBonusTicks;
}

void CBirdAPP::TickShopMsg()
{
	if (m_shopMsgTicks > 0)
	{
		m_shopMsgTicks--;
		if (m_shopMsgTicks == 0)
		{
			m_shopMsg = "";
		}
	}
}

void CBirdAPP::BuyStartShield()
{
	const int cost = 20;
	if (!m_upgradeStartShield && m_coins >= cost)
	{
		m_coins -= cost;
		m_upgradeStartShield = true;
		SavePlayerSave();
		m_shopMsg = "✅购买开局护盾成功!";
		m_shopMsgTicks = 80;
		// 播放购买音效
		CSoundManager::GetInstance().PlayBuyItem();
		return;
	}
	if (m_upgradeStartShield)
	{
		m_shopMsg = "已拥有此升级";
		m_shopMsgTicks = 60;
		// 播放错误音效
		CSoundManager::GetInstance().PlayError();
		return;
	}
	m_shopMsg = "需要20个金币";
	m_shopMsgTicks = 60;
	// 播放错误音效
	CSoundManager::GetInstance().PlayError();
}

void CBirdAPP::BuyPerfectPlus()
{
	const int cost = 15;
	if (m_perfectWindow < 26 && m_coins >= cost)
	{
		m_coins -= cost;
		m_perfectWindow = 26;
		SavePlayerSave();
		m_shopMsg = "✅购买Perfect+成功!";
		m_shopMsgTicks = 80;
		// 播放购买音效
		CSoundManager::GetInstance().PlayBuyItem();
		return;
	}
	if (m_perfectWindow >= 26)
	{
		m_shopMsg = "已拥有此升级";
		m_shopMsgTicks = 60;
		// 播放错误音效
		CSoundManager::GetInstance().PlayError();
		return;
	}
	m_shopMsg = "需要15个金币";
	m_shopMsgTicks = 60;
	// 播放错误音效
	CSoundManager::GetInstance().PlayError();
}

void CBirdAPP::BuyDjPlus()
{
	const int cost = 25;
	if (m_upgradeDjBonusTicks == 0 && m_coins >= cost)
	{
		m_coins -= cost;
		m_upgradeDjBonusTicks = 300;
		SavePlayerSave();
		m_shopMsg = "✅购买DJ+成功!";
		m_shopMsgTicks = 80;
		// 播放购买音效
		CSoundManager::GetInstance().PlayBuyItem();
		return;
	}
	if (m_upgradeDjBonusTicks != 0)
	{
		m_shopMsg = "已拥有此升级";
		m_shopMsgTicks = 60;
		// 播放错误音效
		CSoundManager::GetInstance().PlayError();
		return;
	}
	m_shopMsg = "需要25个金币";
	m_shopMsgTicks = 60;
	// 播放错误音效
	CSoundManager::GetInstance().PlayError();
}

void CBirdAPP::StopAllTimers()
{
	KillTimer(hwnd, 1);
	KillTimer(hwnd, 2);
	KillTimer(hwnd, 3);
	KillTimer(hwnd, 4);
	KillTimer(hwnd, 5);
	KillTimer(hwnd, 6);
}

void CBirdAPP::StartTimersForStatus()
{
	StopAllTimers();

	if (m_Paused)
	{
		return;
	}

	if (m_GameStatus == 0)
	{
		SetTimer(hwnd, TIMER_GROUND_MOVE_TIMERID, TIMER_GROUND_MOVE_INTERVAL, nullptr);
		SetTimer(hwnd, TIMER_BIRD_CHANGE_TIMERID, TIMER_BIRD_CHANGE_INTERVAL, nullptr);
		return;
	}

	if (m_GameStatus == 1)
	{
		SetTimer(hwnd, TIMER_GROUND_MOVE_TIMERID, TIMER_GROUND_MOVE_INTERVAL, nullptr);
		SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, m_columnInterval, nullptr);
		SetTimer(hwnd, TIMER_BIRD_CHANGE_TIMERID, TIMER_BIRD_CHANGE_INTERVAL, nullptr);
		SetTimer(hwnd, TIMER_BIRD_POSITION_TIMERID, TIMER_BIRD_POSITION_INTERVAL, nullptr);
		return;
	}
}

void CBirdAPP::UpdateDifficulty()
{
	int newStep = TIMER_GROUND_MOVE_STEP + (m_score / 5);
	newStep = std::min(newStep, 10);

	int newInterval = TIMER_NEW_COLUMN_INTERVAL - (m_score / 5) * 200;
	newInterval = std::max(newInterval, 1200);

	bool intervalChanged = (newInterval != m_columnInterval);
	m_speedStep = newStep;
	m_columnInterval = newInterval;

	if (intervalChanged && m_GameStatus == 1 && !m_Paused)
	{
		KillTimer(hwnd, TIMER_NEW_COLUMN_TIMERID);
		SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, m_columnInterval, nullptr);
	}
}

void CBirdAPP::UpdateBestScoreIfNeeded()
{
	if (m_score > m_bestScore)
	{
		m_bestScore = m_score;
		SaveBestScore();
	}
}

void CBirdAPP::LoadBestScore()
{
	std::ifstream in("best_score.txt");
	if (!in.is_open())
	{
		m_bestScore = 0;
		return;
	}
	int v = 0;
	in >> v;
	if (in.good() && v >= 0)
	{
		m_bestScore = v;
	}
	else
	{
		m_bestScore = 0;
	}
}

void CBirdAPP::SaveBestScore()
{
	std::ofstream out("best_score.txt", std::ios::trunc);
	if (!out.is_open())
	{
		return;
	}
	out << m_bestScore;
}
