#include "IntroVideo.h"
#include <string>

CIntroVideo::CIntroVideo()
{
    m_currentFrame = 0;
    m_totalFrames = 201;
    m_finished = false;
    m_x = 0;
    m_y = 0;
    m_width = 400;
    m_height = 740;
    m_frameDelay = 0;
    m_frameDelayMax = 3; // 每3次绘制更新一帧
}

CIntroVideo::~CIntroVideo()
{
}

void CIntroVideo::InitIntroVideo(int width, int height)
{
    m_width = width;
    m_height = height;
    m_currentFrame = 0;
    m_finished = false;
    m_frameDelay = 0;

    // Load all frames
    for (int i = 1; i <= 201; i++)
    {
        char path[256];
        sprintf_s(path, "./res/intro/frame_%04d.png", i);
        loadimage(&m_frames[i - 1], path);
    }
}

void CIntroVideo::ShowNextFrame()
{
    if (m_finished)
    {
        return;
    }

    // 帧率控制：只有当延迟计数器达到最大值时才更新帧
    m_frameDelay++;
    if (m_frameDelay < m_frameDelayMax)
    {
        // 显示当前帧（不更新）
        if (m_currentFrame < m_totalFrames)
        {
            putimage(0, 0, m_width, m_height, &m_frames[m_currentFrame], 0, 0, SRCCOPY);
        }
        return;
    }
    m_frameDelay = 0;

    if (m_currentFrame < m_totalFrames)
    {
        putimage(0, 0, m_width, m_height, &m_frames[m_currentFrame], 0, 0, SRCCOPY);
        m_currentFrame++;
    }
    else
    {
        m_finished = true;
    }
}

bool CIntroVideo::IsFinished() const
{
    return m_finished;
}

void CIntroVideo::Reset()
{
    m_currentFrame = 0;
    m_finished = false;
    m_frameDelay = 0;
}

void CIntroVideo::Skip()
{
    m_finished = true;
}
