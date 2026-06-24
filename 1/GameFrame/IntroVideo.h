#pragma once
#include <easyx.h>
#include <string>

class CIntroVideo
{
public:
    IMAGE m_frames[201];
    int m_currentFrame;
    int m_totalFrames;
    bool m_finished;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_frameDelay;      // 帧延迟计数器
    int m_frameDelayMax;   // 最大延迟值（控制播放速度）

    CIntroVideo();
    ~CIntroVideo();

    void InitIntroVideo(int width, int height);
    void ShowNextFrame();
    bool IsFinished() const;
    void Reset();
    void Skip();           // 跳过动画
};
