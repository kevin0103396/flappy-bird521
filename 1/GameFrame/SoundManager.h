#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <string>

#pragma comment(lib, "winmm.lib")

class CSoundManager
{
public:
    static CSoundManager& GetInstance();

    void PlayFlap();           // 拍翅膀音效
    void PlayScore();          // 得分音效
    void PlayCoin();           // 拾取金币音效
    void PlayPowerUp();        // 拾取道具音效
    void PlayShieldBreak();    // 护盾破碎音效
    void PlayGameOver();       // 游戏结束音效
    void PlayFever();          // 狂热模式音效
    void PlayPerfect();        // 完美通过音效
    void PlayNearMiss();       // 险过音效
    void PlayBuyItem();        // 购买物品音效
    void PlayError();          // 错误提示音效
    void PlayIntroMusic();     // 开场音乐
    void StopAllSound();       // 停止所有音效

private:
    CSoundManager();
    ~CSoundManager();

    // 使用Windows Beep API生成简单音效
    void BeepSound(int freq, int duration);

    bool m_soundEnabled;
};
