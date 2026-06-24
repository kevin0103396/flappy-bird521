#pragma once
#include"../GameFrame/frame.h"
#include"../BackGround/CBackGround.h"
#include"../GameFrame/frame.h"
#include"../GameFrame/IntroVideo.h"
#include"../GameFrame/SoundManager.h"
#include"../Ground/CGround.h"
#include"../Column/CColumnBox.h"
#include"../PlayerBird/CPlayerBird.h"
#include"../BeforeGame/BeforeGame.h"
#include"../AfterGame/CAfterGame.h"
#include <string>

class CBirdAPP : public CGameFrame
{
private:
	bool m_Paused;
	int m_bestScore;
	int m_speedStep;
	int m_columnInterval;
	bool m_hasShield;
	bool m_shieldOnMap;
	int m_shieldX;
	int m_shieldY;
	int m_lastShieldSpawnScore;
	std::string m_floatMsg;
	int m_floatMsgTicks;
	int m_coins;
	bool m_coinOnMap;
	int m_coinX;
	int m_coinY;
	int m_lastCoinSpawnScore;
	bool m_doubleJumpOnMap;
	int m_doubleJumpX;
	int m_doubleJumpY;
	int m_lastDoubleJumpSpawnScore;
	int m_doubleJumpTicks;
	int m_flapCooldownTicks;
	int m_perfectStreak;
	int m_perfectWindow;
	bool m_upgradeStartShield;
	int m_upgradeDjBonusTicks;
	std::string m_shopMsg;
	int m_shopMsgTicks;
	bool m_shopOpen;
	int m_passStreak;
	int m_feverTicks;
	int m_nearMissDist;
	int m_scoreMultiplierTicks;
	int m_slowMoTicks;
	int m_ghostTicks;
	int m_shrinkTicks;
	bool m_powerOnMap;
	int m_powerType;
	int m_powerX;
	int m_powerY;
	int m_lastPowerSpawnScore;

	void StopAllTimers();
	void StartTimersForStatus();
	void UpdateDifficulty();
	void UpdateBestScoreIfNeeded();
	void LoadBestScore();
	void SaveBestScore();
	void SpawnShieldIfNeeded();
	void UpdateShield();
	void DrawShield();
	void DrawFloatMessage();
	bool CheckShieldPickup();
	void SpawnCoinIfNeeded();
	void UpdateCoin();
	void DrawCoin();
	bool CheckCoinPickup();
	void SpawnDoubleJumpIfNeeded();
	void UpdateDoubleJumpItem();
	void DrawDoubleJumpItem();
	bool CheckDoubleJumpPickup();
	void ActivateDoubleJump(int ticks, const char* reason);
	void TryFlap();
	void DrawShop();
	void LoadPlayerSave();
	void SavePlayerSave();
	void TickShopMsg();
	void BuyStartShield();
	void BuyPerfectPlus();
	void BuyDjPlus();
	void DrawFeverBar();
	int GetBirdW() const;
	int GetBirdH() const;
	void SpawnPowerIfNeeded();
	void UpdatePower();
	void DrawPower();
	bool CheckPowerPickup();
	void ApplyPower(int type);
	bool IsGhost() const;
	int GetScoreMultiplier() const;

public:
	CBirdAPP();
	~CBirdAPP();

	CBackGround m_BackGround;
	CGround m_Ground;
	CPlayerBird m_PlayerBird;
	CColumnBox m_ColumnBox;
	CBeforeGame m_BeforeGame;
	CAfterGame m_AfterGame;
	CIntroVideo m_IntroVideo;
	bool m_showIntro;
	static int m_rand;
	int m_GameStatus;
	int m_score;

	void ReStartStop();
	void On_SetTimer();
	void ShowScore();
	virtual void On_Init();
	virtual void On_Paint();
	virtual void On_Close();
	virtual void On_WM_LBUTTONDOWN(POINT& po);
	virtual void On_WM_TIMER(WPARAM wParam, LPARAM lParam);
	virtual void On_WM_KEYDOWN(WPARAM wparam);


};
