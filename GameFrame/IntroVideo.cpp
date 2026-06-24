#include "IntroVideo.h"
#include <graphics.h>
#include <stdio.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

bool PlayIntroVideo(HWND hwnd)
{
    static bool played = false;
    if (played) return false;
    played = true;

    // count frames
    char path[MAX_PATH];
    int frameCount = 0;
    for (int i = 1; i <= 9999; i++) {
        sprintf_s(path, "res/intro/frame_%04d.png", i);
        FILE* f = NULL;
        fopen_s(&f, path, "rb");
        if (!f) break;
        fclose(f);
        frameCount++;
    }

    if (frameCount == 0) return false;

    // pre-load all frames
    IMAGE* frames = new IMAGE[frameCount];
    for (int i = 0; i < frameCount; i++) {
        sprintf_s(path, "res/intro/frame_%04d.png", i + 1);
        loadimage(&frames[i], path);
    }

    timeBeginPeriod(1);
    BeginBatchDraw();
    DWORD t0 = GetTickCount();
    for (int i = 0; i < frameCount; i++) {
        putimage(0, 0, &frames[i]);
        FlushBatchDraw();
        DWORD target = t0 + (i + 1) * 50;
        while (GetTickCount() < target) Sleep(1);
    }
    EndBatchDraw();
    timeEndPeriod(1);
    delete[] frames;
    return true;
}
