#include "SoundManager.h"
#include <thread>

CSoundManager::CSoundManager()
{
    m_soundEnabled = true;
}

CSoundManager::~CSoundManager()
{
}

CSoundManager& CSoundManager::GetInstance()
{
    static CSoundManager instance;
    return instance;
}

void CSoundManager::BeepSound(int freq, int duration)
{
    if (!m_soundEnabled)
    {
        return;
    }
    // 使用异步方式播放，不阻塞游戏
    std::thread([freq, duration]() {
        ::Beep(freq, duration);
    }).detach();
}

void CSoundManager::PlayFlap()
{
    BeepSound(800, 50);  // 高频短促
}

void CSoundManager::PlayScore()
{
    BeepSound(1000, 80); // 得分音效
}

void CSoundManager::PlayCoin()
{
    // 金币拾取：上升音调
    std::thread([]() {
        ::Beep(600, 50);
        ::Beep(800, 50);
        ::Beep(1000, 50);
    }).detach();
}

void CSoundManager::PlayPowerUp()
{
    // 道具拾取：上升音调
    std::thread([]() {
        ::Beep(500, 60);
        ::Beep(700, 60);
        ::Beep(900, 60);
        ::Beep(1100, 60);
    }).detach();
}

void CSoundManager::PlayShieldBreak()
{
    // 护盾破碎：下降音调
    std::thread([]() {
        ::Beep(1000, 80);
        ::Beep(700, 80);
        ::Beep(400, 100);
    }).detach();
}

void CSoundManager::PlayGameOver()
{
    // 游戏结束：低沉下降
    std::thread([]() {
        ::Beep(600, 150);
        ::Beep(500, 150);
        ::Beep(400, 150);
        ::Beep(300, 200);
    }).detach();
}

void CSoundManager::PlayFever()
{
    // 狂热模式：快速上升
    std::thread([]() {
        for (int i = 0; i < 5; i++)
        {
            ::Beep(600 + i * 100, 40);
        }
    }).detach();
}

void CSoundManager::PlayPerfect()
{
    // 完美通过：清脆音效
    std::thread([]() {
        ::Beep(1200, 60);
        ::Beep(1500, 60);
    }).detach();
}

void CSoundManager::PlayNearMiss()
{
    // 险过：短促警告
    BeepSound(900, 40);
}

void CSoundManager::PlayBuyItem()
{
    // 购买物品：成功音效
    std::thread([]() {
        ::Beep(800, 80);
        ::Beep(1000, 80);
        ::Beep(1200, 100);
    }).detach();
}

void CSoundManager::PlayError()
{
    // 错误提示：低沉警告
    BeepSound(300, 150);
}

void CSoundManager::PlayIntroMusic()
{
    // 开场音乐：简单旋律
    std::thread([]() {
        ::Beep(523, 200); // C5
        ::Beep(659, 200); // E5
        ::Beep(784, 200); // G5
        ::Beep(1047, 400); // C6
    }).detach();
}

void CSoundManager::StopAllSound()
{
    // 当前实现使用Beep，无法中途停止
    // 如果使用PlaySound可以停止
}
