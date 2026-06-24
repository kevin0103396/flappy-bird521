# Flappy Bird EasyX 项目深度解析教程

> 🎯 本教程面向零基础小白，从「为什么」到「怎么做」，带你完整理解一个游戏项目是如何从零到落地的。

---

## 目录

- [第一章：项目全景——我们到底在做什么？](#第一章项目全景我们到底在做什么)
- [第二章：技术栈科普——这些名词都是什么意思？](#第二章技术栈科普这些名词都是什么意思)
- [第三章：项目结构——代码是怎么组织的？](#第三章项目结构代码是怎么组织的)
- [第四章：程序入口——程序从哪里开始运行？](#第四章程序入口程序从哪里开始运行)
- [第五章：游戏框架——CGameFrame 基类的设计哲学](#第五章游戏框架cgameframe-基类的设计哲学)
- [第六章：消息机制——程序怎么知道你点了鼠标？](#第六章消息机制程序怎么知道你点了鼠标)
- [第七章：游戏状态机——开始、游戏中、结束是怎么切换的？](#第七章游戏状态机开始游戏中结束是怎么切换的)
- [第八章：定时器系统——游戏是怎么「动」起来的？](#第八章定时器系统游戏是怎么动起来的)
- [第九章：小鸟——物理、动画与渲染](#第九章小鸟物理动画与渲染)
- [第十章：管道障碍物——生成、移动与碰撞检测](#第十章管道障碍物生成移动与碰撞检测)
- [第十一章：地面与背景——滚动效果的实现](#第十一章地面与背景滚动效果的实现)
- [第十二章：计分系统——从简单到复杂的进化](#第十二章计分系统从简单到复杂的进化)
- [第十三章：道具系统——护盾、金币、强化](#第十三章道具系统护盾金币强化)
- [第十四章：商店系统——持久化存储](#第十四章商店系统持久化存储)
- [第十五章：渲染技巧——透明贴图的两层绘制法](#第十五章渲染技巧透明贴图的两层绘制法)
- [第十六章：设计哲学总结——从这个项目我们能学到什么？](#第十六章设计哲学总结从这个项目我们能学到什么)
- [附录：如何自己编译运行这个项目](#附录如何自己编译运行这个项目)

---

## 第一章：项目全景——我们到底在做什么？

### 1.1 什么是 Flappy Bird？

Flappy Bird 是一款非常简单的游戏：

- 一只小鸟在屏幕上不断地往下掉（重力）
- 你点击屏幕，小鸟会往上飞一下（拍翅膀）
- 屏幕上有上下两根管子组成的障碍物，从右往左移动
- 你的目标是让小鸟穿过管子之间的缝隙，不要撞到管子或地面
- 每穿过一对管子，得 1 分

**为什么用它来学习？** 因为它麻雀虽小五脏俱全——包含了游戏开发的所有核心概念：

```
游戏循环 → 输入处理 → 物理模拟 → 碰撞检测 → 渲染绘制 → 状态管理 → 数据持久化
```

### 1.2 这个项目做了哪些「增强」？

原版 Flappy Bird 非常简单，这个项目在基础上增加了：

| 特性 | 说明 |
|------|------|
| **完美穿越** | 从管子缝隙正中间穿过，获得额外加分，连续完美有连击奖励 |
| **惊险穿越** | 差一点就碰到管子边缘，也能加分 |
| **Fever 系统** | 连续穿过 10 根管子不死亡，进入双倍得分模式 |
| **护盾道具** | 地图上出现的黄色圆圈，吃到后可以抵挡一次碰撞 |
| **金币道具** | 收集金币，在商店购买永久升级 |
| **二段跳道具** | 临时获得更强的跳跃能力 |
| **随机道具** | 无敌(Ghost)、缩小(Shrink)、减速(SlowMo)、双倍(2X) |
| **商店系统** | 永久升级：开局护盾、完美窗口加大、二段跳增强 |
| **难度递增** | 分数越高，管子移动越快，生成越频繁 |
| **移动管子** | 25% 概率生成会上下移动的管子 |
| **最高分记录** | 保存到文件，关闭游戏后不丢失 |

---

## 第二章：技术栈科普——这些名词都是什么意思？

### 2.1 C++ 语言

C++ 是一种编程语言。你可以把它理解为「和计算机沟通的语言」。

```
// 这就是 C++ 代码
int score = 0;          // 创建一个变量，叫 score，初始值为 0
score = score + 1;      // 让 score 加 1
```

**为什么选 C++？** 因为它运行速度快，适合做游戏。游戏需要每秒刷新几十次画面，每次都计算位置、判断碰撞，所以速度很重要。

### 2.2 EasyX 图形库

EasyX 是一个 **Windows 平台的 C++ 图形库**，专门用来画图、显示图片、处理鼠标键盘输入。

**它帮你做了什么？** 如果没有 EasyX，你需要直接调用 Windows API（非常底层、非常复杂）。EasyX 把这些复杂操作封装成了简单的函数：

```cpp
// 没有 EasyX（原始 Windows GDI 编程）：
HDC hdc = GetDC(hwnd);
HDC memDC = CreateCompatibleDC(hdc);
HBITMAP bmp = (HBITMAP)LoadImage(NULL, L"bird.png", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
SelectObject(memDC, bmp);
BitBlt(hdc, 0, 0, 56, 48, memDC, 0, 0, SRCCOPY);
DeleteObject(bmp);
DeleteDC(memDC);
ReleaseDC(hwnd, hdc);

// 有 EasyX（一行搞定）：
IMAGE img;
loadimage(&img, "./res/0.png");  // 加载图片
putimage(100, 200, &img);         // 在坐标(100,200)处画出来
```

**EasyX 提供的核心功能：**

| 函数 | 作用 | 类比 |
|------|------|------|
| `initgraph(w, h)` | 创建一个窗口 | 打开一块画布 |
| `loadimage(&img, path)` | 从文件加载图片 | 把照片放进相框 |
| `putimage(x, y, &img)` | 在指定位置画图片 | 把相框挂到墙上 |
| `settextcolor()` | 设置文字颜色 | 选墨水颜色 |
| `drawtext()` | 在窗口上写字 | 用毛笔写字 |
| `circle(x, y, r)` | 画圆 | 用圆规画圆 |
| `BeginBatchDraw()` | 开始批量绘制 | 先在草稿纸上画 |
| `EndBatchDraw()` | 结束批量绘制，一次性显示 | 把草稿纸翻过来展示 |
| `SetTimer()` | 设置定时闹钟 | 每隔 X 毫钟响一次 |
| `KillTimer()` | 取消定时闹钟 | 关掉闹钟 |

### 2.3 Win32 消息机制

Windows 操作系统是基于「消息」工作的。当你：

- **点击鼠标** → Windows 发出 `WM_LBUTTONDOWN` 消息
- **按下键盘** → Windows 发出 `WM_KEYDOWN` 消息
- **定时器到时间了** → Windows 发出 `WM_TIMER` 消息
- **关闭窗口** → Windows 发出 `WM_CLOSE` 消息

你的程序需要「监听」这些消息并做出反应。这就是为什么需要一个「回调函数」。

### 2.4 面向对象编程 (OOP)

这个项目用了 C++ 的面向对象编程思想。核心概念：

**类 (Class)** = 设计图纸
**对象 (Object)** = 根据图纸造出来的东西

```cpp
// 类 = 设计图纸：「小鸟应该有哪些属性和行为」
class CPlayerBird {
    int m_x, m_y;        // 属性：位置
    int m_speed;          // 属性：速度
    void MovePlayerBird(); // 行为：移动
    void ShowPlayerBird(); // 行为：显示
};

// 对象 = 根据图纸造出来的「一只具体的小鸟」
CPlayerBird m_PlayerBird;  // 在 CBirdAPP 中创建
```

**继承 (Inheritance)** = 在现有图纸基础上改进

```cpp
// CGameFrame 是通用游戏框架的「图纸」
class CGameFrame {
    virtual void On_Init() = 0;   // 「初始化」是必须的，但具体怎么做由子类决定
    virtual void On_Paint() = 0;  // 「画画面」也是必须的
};

// CBirdAPP 是 Flappy Bird 具体游戏的「图纸」，继承了 CGameFrame
class CBirdAPP : public CGameFrame {
    void On_Init() override;   // 具体实现：加载小鸟图片、设置定时器...
    void On_Paint() override;  // 具体实现：画背景、画管子、画小鸟...
};
```

**组合 (Composition)** = 一个大东西由多个小东西组成

```
CBirdAPP（整个游戏）
  ├── CBackGround（背景）
  ├── CGround（地面）
  ├── CPlayerBird（小鸟）
  ├── CColumnBox（管子容器）
  │     └── 多个 CColumn（单根管子）
  ├── CBeforeGame（开始画面）
  └── CAfterGame（结束画面）
```

---

## 第三章：项目结构——代码是怎么组织的？

### 3.1 目录结构一览

```
Flappy-Bird-EasyX-main/
│
├── GameFrame/                  ← 【骨架】程序入口 + 游戏框架基类
│   ├── main.cpp                    程序从这里开始运行
│   └── frame.h                     游戏框架的「设计图纸」（抽象基类）
│
├── GameConfig/                 ← 【配置】所有魔法数字的定义
│   └── GameConfig.h                窗口大小、图片尺寸、定时器间隔、物理参数...
│
├── BirdAPP/                    ← 【大脑】核心游戏逻辑（最最重要！）
│   ├── CBirdAPP.h                  游戏类的声明（有哪些变量和方法）
│   └── CBirdAPP.cpp                游戏类的实现（1405 行，所有游戏逻辑都在这）
│
├── PlayerBird/                 ← 【角色】小鸟
│   ├── CPlayerBird.h
│   └── CPlayerBird.cpp             小鸟的位置、速度、重力、动画、渲染
│
├── Column/                     ← 【障碍】管子
│   ├── CColumn.h / .cpp            单根管子：位置、移动、碰撞检测
│   └── CColumnBox.h / .cpp         管子容器：管理所有管子的生命周期
│
├── BackGround/                 ← 【背景】静态背景图
│   ├── CBackGround.h
│   └── CBackGround.cpp
│
├── Ground/                     ← 【地面】滚动的地面
│   ├── CGround.h
│   └── CGround.cpp                 地面滚动 + 接触检测
│
├── BeforeGame/                 ← 【界面】开始画面
│   ├── BeforeGame.h
│   └── BeforeGame.cpp
│
├── AfterGame/                  ← 【界面】结束画面
│   ├── CAfterGame.h
│   └── CAfterGame.cpp
│
├── res/                        ← 【资源】所有图片素材
│   ├── 0.png ~ 7.png               小鸟动画（遮罩层）
│   ├── 0Front.png ~ 7Front.png     小鸟动画（颜色层）
│   ├── background.png              背景图
│   ├── ground.png                  地面图
│   ├── column.png / columnFront.png 管子图
│   ├── start.png / startFront.png  开始画面
│   └── gameover.png / gameoverFront.png 结束画面
│
├── best_score.txt              ← 最高分记录
├── player_save.txt             ← 玩家存档（金币、升级）
└── FlappyBird.exe              ← 编译好的可执行文件
```

### 3.2 为什么要这样分？

**一个类一个文件夹**——这是 C++ 项目的常见组织方式。每个文件夹代表一个「模块」，每个模块负责一件事。

**这样做的好处：**

1. **分工明确**：改小鸟的物理？去 `PlayerBird/`。改管子的碰撞？去 `Column/`。
2. **易于维护**：1405 行的 `CBirdAPP.cpp` 已经很大了，如果所有代码都写在一个文件里，会更难找。
3. **便于复用**：如果要做另一个游戏，`CGameFrame` 框架可以直接拿去用。

### 3.3 `#include` 的路径关系

代码中经常看到这种写法：

```cpp
#include "../GameConfig/GameConfig.h"
```

`..` 表示「上一级目录」。因为 `PlayerBird/CPlayerBird.cpp` 在 `PlayerBird/` 文件夹里，要引用 `GameConfig/GameConfig.h`，需要先回到上一级 `../`，再进入 `GameConfig/`。

```
项目根目录/
├── PlayerBird/
│   └── CPlayerBird.cpp    ← 你在这里
└── GameConfig/
    └── GameConfig.h       ← 你要引用这个文件
    └── 从 PlayerBird/ 出发：../GameConfig/GameConfig.h
```

---

## 第四章：程序入口——程序从哪里开始运行？

### 4.1 `main.cpp` —— 一切的起点

每个 C++ 程序都有一个 `main()` 函数，它是程序的入口点。就像一本书的第一页。

```cpp
// main.cpp 简化版（去掉注释）
#include <easyx.h>
#include "frame.h"
#include <windowsx.h>

CGameFrame* P_G = nullptr;  // 全局指针，指向游戏对象

int main()
{
    // 第一步：创建游戏对象
    P_G = CreatObject();

    // 第二步：初始化游戏（创建窗口、加载资源）
    P_G->InitGame(WND_TITLE, WND_WIDTH, WND_HEIGHT, WND_POS_X, WND_POS_Y);

    // 第三步：画第一帧画面
    P_G->PaintGame();

    // 第四步：等待游戏结束（真正的逻辑在定时器和回调函数里）
    while (!P_G->IfEndGame()) {
        Sleep(200);  // 每 200 毫秒检查一次是否要退出
    }

    // 第五步：清理资源
    delete P_G;
    return 0;
}
```

**关键理解：`main()` 函数并不是游戏的「主循环」！**

传统的游戏循环长这样：

```cpp
// 传统游戏循环（这个项目没有用这种方式）
while (true) {
    处理输入();    // 检查鼠标、键盘
    更新状态();    // 移动小鸟、管子、检测碰撞
    渲染画面();    // 把所有东西画到屏幕上
}
```

但这个项目用的是 **Windows 消息驱动** 的方式——`main()` 只负责创建和等待，真正的游戏逻辑分散在各个「回调函数」中，由 Windows 定时器触发。

### 4.2 `CreatObject()` —— 工厂模式

```cpp
// frame.h 中定义的宏
#define DYNAMIC_CREATE(CLASS_OBJECT)                \
CGameFrame* CreatObject(){                          \
    return new CLASS_OBJECT;                        \
}

// CBirdAPP.cpp 中使用这个宏
DYNAMIC_CREATE(CBirdAPP)
```

宏展开后变成：

```cpp
CGameFrame* CreatObject(){
    return new CBirdAPP;  // 创建一个 CBirdAPP 对象
}
```

**为什么要用这种写法？** 这是一种简化版的「工厂模式」。`main.cpp` 不需要知道具体的游戏类叫什么名字，它只需要调用 `CreatObject()` 就能获得一个游戏对象。如果以后要做另一个游戏（比如俄罗斯方块），只需要写一个新的类，换一行 `DYNAMIC_CREATE(CSquareAPP)` 就行，`main.cpp` 完全不用改。

### 4.3 `WND_PARAM` 宏 —— 窗口参数

```cpp
// CBirdAPP.cpp 第 9 行
WND_PARAM(200, 100, WINDOW_WIDTH, WINDOW_HEIGHT, "FlappyBird")
```

展开后：

```cpp
int WND_POS_X = 200;        // 窗口左上角 X 坐标
int WND_POS_Y = 100;        // 窗口左上角 Y 坐标
int WND_WIDTH = 400;         // 窗口宽度（来自 GameConfig.h）
int WND_HEIGHT = 740;        // 窗口高度
const TCHAR* WND_TITLE = "FlappyBird";  // 窗口标题
```

这些变量是 `extern` 的，`main.cpp` 可以直接使用。

---

## 第五章：游戏框架——CGameFrame 基类的设计哲学

### 5.1 什么是「框架」？

想象一下盖房子：

- **框架** = 房子的骨架（承重墙、梁柱）—— 已经搭好了，你不用管
- **你填的东西** = 墙壁的颜色、家具的摆放 —— 你自己决定

`CGameFrame` 就是游戏的「骨架」，它定义了：

- 怎么创建窗口
- 怎么处理消息
- 怎么刷新画面
- 怎么结束游戏

具体的游戏逻辑（画什么、怎么玩）由子类 `CBirdAPP` 来实现。

### 5.2 纯虚函数 = 「必须填的作业」

```cpp
class CGameFrame {
    // 纯虚函数（= 0 表示「必须由子类实现」）
    virtual void On_Init() = 0;    // 初始化：你必须告诉我怎么初始化
    virtual void On_Paint() = 0;   // 绘制：你必须告诉我怎么画
    virtual void On_Close() = 0;   // 关闭：你必须告诉我怎么清理

    // 虚函数（有默认实现，子类可以选择性覆盖）
    virtual void On_WM_LBUTTONDOWN(POINT& po) {}  // 鼠标点击：不覆盖就什么都不做
    virtual void On_WM_KEYDOWN(WPARAM wparam) {}   // 键盘按下：不覆盖就什么都不做
    virtual void On_WM_TIMER(WPARAM wParam, LPARAM lParam) {}  // 定时器：不覆盖就什么都不做
};
```

**打个比方：**

- `CGameFrame` 说：「你必须告诉我怎么初始化游戏（`On_Init`）、怎么画画面（`On_Paint`）、怎么结束（`On_Close`）。至于鼠标点击和定时器，你可以选择处理也可以不处理。」
- `CBirdAPP` 说：「好，初始化时我加载图片、设置定时器；画画面时我画背景、管子、小鸟；结束时我清理资源。鼠标点击时我让小鸟飞，定时器到了我更新位置。」

### 5.3 `InitGame()` —— 框架提供的初始化流程

```cpp
void InitGame(const TCHAR* title, int width, int height, int x, int y)
{
    // 1. 创建图形窗口
    hwnd = initgraph(width, height);
    MoveWindow(hwnd, x, y, width, height, TRUE);
    SetWindowText(hwnd, title);
    setbkcolor(RGB(255, 255, 255));
    cleardevice();

    // 2. 初始化消息映射表
    InitMsgMap();

    // 3. 调用子类的初始化（由 CBirdAPP 实现）
    On_Init();

    // 4. 设置窗口回调函数（把 Windows 消息转发给我们的代码）
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&Wndproc);
}
```

**框架帮你做了 1、2、4 这些「通用工作」，你只需要实现第 3 步的 `On_Init()`。** 这就是框架的价值。

### 5.4 `PaintGame()` —— 双缓冲绘制

```cpp
void PaintGame()
{
    BeginBatchDraw();   // 开始「批量绘制」（先画到内存里，不显示）
    cleardevice();      // 用背景色清空画布

    On_Paint();         // 调用子类的绘制函数（由 CBirdAPP 实现）

    EndBatchDraw();     // 结束批量绘制（一次性把内存中的画面显示出来）
}
```

**为什么要「批量绘制」？**

想象你在翻页动画书上画画：

- ❌ **没有双缓冲**：画一笔就显示一笔 → 用户会看到画面闪烁（因为你先清空了旧画面，然后慢慢画新画面）
- ✅ **有双缓冲**：先在一张白纸上把整幅画画完，然后瞬间「啪」地替换掉旧画面 → 用户看到的是流畅的切换

`BeginBatchDraw()` 就是说「我要开始在草稿纸上画了」，`EndBatchDraw()` 就是说「画完了，把草稿纸翻过来给用户看」。

---

## 第六章：消息机制——程序怎么知道你点了鼠标？

### 6.1 回调函数 `Wndproc`

```cpp
LRESULT Wndproc(HWND hwnd, UINT msgid, WPARAM wparam, LPARAM lparam)
{
    // 检查消息映射表里有没有处理这个消息的函数
    if (P_G->m_msgmap.count(msgid))
    {
        // 根据消息类型，调用不同的处理函数
        switch (P_G->m_msgmap[msgid].msg_type)
        {
        case EM_MOUSE:   // 鼠标消息
            (P_G->*(P_G->m_msgmap[msgid].p_fun_EM_MOUSE))(po);
            break;
        case EM_KEY:     // 键盘消息
            (P_G->*(P_G->m_msgmap[msgid].p_fun_EM_KEY))(wparam);
            break;
        case EM_WINDOW:  // 窗口消息（包括定时器）
            (P_G->*(P_G->m_msgmap[msgid].p_fun_EM_WINDOW))(wparam, lparam);
            break;
        }

        // 处理完消息后，重新绘制画面
        if (!P_G->IfEndGame()) {
            P_G->PaintGame();
        }
    }
    return DefWindowProc(hwnd, msgid, wparam, lparam);
}
```

### 6.2 消息映射表 —— 「翻译官」

```cpp
map<UINT, Fun_Type> m_msgmap;
```

这是一个 `map`（映射表），key 是消息 ID（如 `WM_LBUTTONDOWN`），value 是对应的处理函数。

**打个比方：** 想象一个电话接线员：

- 来电显示 `WM_LBUTTONDOWN`（鼠标左键按下）→ 转接到 `On_WM_LBUTTONDOWN` 部门
- 来电显示 `WM_KEYDOWN`（键盘按下）→ 转接到 `On_WM_KEYDOWN` 部门
- 来电显示 `WM_TIMER`（定时器）→ 转接到 `On_WM_TIMER` 部门

`INIT_MSGMAP` 宏就是设置这个转接表：

```cpp
#define INIT_MSGMAP(MSGID, MSGTYPE)                              \
    m_msgmap[MSGID].msg_type = MSGTYPE;                          \
    m_msgmap[MSGID].p_fun_##MSGTYPE = &CGameFrame::On_##MSGID;

// 使用：
INIT_MSGMAP(WM_LBUTTONDOWN, EM_MOUSE)
// 展开为：
m_msgmap[WM_LBUTTONDOWN].msg_type = EM_MOUSE;
m_msgmap[WM_LBUTTONDOWN].p_fun_EM_MOUSE = &CGameFrame::On_WM_LBUTTONDOWN;
```

### 6.3 成员函数指针 —— C++ 的高级技巧

```cpp
typedef void(CGameFrame::* P_FUN_MOUSE)(POINT&);
```

这是一个「指向 CGameFrame 类成员函数的指针」。普通函数指针只能指向全局函数，但游戏的处理函数是类的成员函数，所以需要成员函数指针。

调用方式：

```cpp
(P_G->*(P_G->m_msgmap[msgid].p_fun_EM_MOUSE))(po);
//  等价于：
P_G->On_WM_LBUTTONDOWN(po);
```

**这段代码看不懂没关系！** 你只需要知道它的效果是：「当用户点击鼠标时，自动调用 `On_WM_LBUTTONDOWN` 函数」。

---

## 第七章：游戏状态机——开始、游戏中、结束是怎么切换的？

### 7.1 什么是状态机？

状态机是游戏开发中最核心的概念之一。它描述了一个系统在不同状态之间的切换规则。

```
                点击开始按钮
    ┌──────────────────────────────┐
    │                              ▼
┌───┴────┐    碰撞死亡    ┌───────────┐
│ 状态 0 │ ◄──────────── │  状态 1   │
│ 开始   │               │  游戏中   │
│ 画面   │               │           │
└───┬────┘               └─────┬─────┘
    │                          │
    │    ┌───────────┐         │
    └───►│  状态 2   │◄────────┘
         │  游戏结束  │  碰撞死亡
         └─────┬─────┘
               │
               │  点击重新开始
               └──► 回到状态 0
```

### 7.2 代码中的状态切换

```cpp
// 状态变量
int m_GameStatus;  // 0=开始画面, 1=游戏中, 2=游戏结束

// 在 On_WM_LBUTTONDOWN（鼠标点击）中：

// 状态 0（开始画面）→ 状态 1（游戏中）
if (m_GameStatus == 0 && 点击了开始按钮) {
    m_GameStatus = 1;  // 切换状态
    On_Init();          // 重新初始化（加载游戏资源、启动定时器）
}

// 状态 1（游戏中）→ 碰撞检测在 On_WM_TIMER 中处理
if (碰撞了) {
    m_GameStatus = 2;   // 切换到结束状态
    StopAllTimers();     // 停止所有定时器
}

// 状态 2（结束画面）→ 状态 0（开始画面）
if (m_GameStatus == 2 && 点击了重新开始按钮) {
    m_GameStatus = 0;   // 回到开始画面
    On_Init();           // 重新初始化
}
```

### 7.3 不同状态下的渲染

```cpp
void CBirdAPP::On_Paint()
{
    if (m_GameStatus == 1)  // 游戏中
    {
        m_BackGround.ShowBackGround();      // 画背景
        m_ColumnBox.ShowAllColumn();         // 画所有管子
        m_Ground.ShowGround();               // 画地面
        ShowScore();                          // 画分数
        DrawFeverBar();                       // 画 Fever 条
        m_PlayerBird.ShowPlayerBird();        // 画小鸟
        // ... 还有道具、文字等
    }
    else if (m_GameStatus == 0)  // 开始画面
    {
        m_BackGround.ShowBackGround();
        m_Ground.ShowGround();
        m_BeforeGame.ShowBeforeGame();       // 画开始界面覆盖层
        m_PlayerBird.ShowPlayerBird();
        DrawShop();                           // 画商店界面
    }
    else if (m_GameStatus == 2)  // 游戏结束
    {
        m_BackGround.ShowBackGround();
        m_ColumnBox.ShowAllColumn();          // 保留最后的管子位置
        m_Ground.ShowGround();
        m_PlayerBird.ShowPlayerBird();
        m_AfterGame.ShowAfterGame();          // 画结束界面覆盖层
        ShowScore();
    }
}
```

**关键理解：** 绘制顺序很重要！后画的会覆盖先画的。所以先画背景，再画管子，再画地面，最后画小鸟——这样小鸟才会在所有东西的「上面」。

---

## 第八章：定时器系统——游戏是怎么「动」起来的？

### 8.1 为什么需要定时器？

游戏需要不断地更新状态：小鸟的位置每 30 毫秒更新一次，地面每 20 毫秒移动一次，小鸟的动画每 100 毫秒切换一帧。

**Win32 定时器** 就像一个闹钟：你设置「每 30 毫秒响一次」，Windows 就会每 30 毫秒给你发一个 `WM_TIMER` 消息。

### 8.2 六个定时器的分工

```cpp
// GameConfig.h 中定义的定时器参数
#define TIMER_GROUND_MOVE_TIMERID    1    // 定时器 ID
#define TIMER_GROUND_MOVE_INTERVAL   20   // 间隔 20 毫秒

#define TIMER_NEW_COLUMN_TIMERID     3
#define TIMER_NEW_COLUMN_INTERVAL    3000 // 间隔 3 秒

#define TIMER_BIRD_CHANGE_TIMERID    4
#define TIMER_BIRD_CHANGE_INTERVAL   100  // 间隔 100 毫秒

#define TIMER_BIRD_POSITION_TIMERID  6
#define TIMER_BIRD_POSITION_INTERVAL 30   // 间隔 30 毫秒
```

**每个定时器负责什么：**

```
定时器 1 (20ms)  ─── 地面滚动 + 管子移动 + 碰撞检测 + 计分 + 道具逻辑
定时器 3 (3000ms) ── 生成新的管子（间隔会随难度递减）
定时器 4 (100ms) ─── 小鸟动画帧切换（8 帧循环）
定时器 6 (30ms)  ─── 小鸟物理（重力 + 位置更新 + 地面碰撞）
```

### 8.3 定时器的启动和停止

不同游戏状态下，启动不同的定时器：

```cpp
void CBirdAPP::StartTimersForStatus()
{
    StopAllTimers();  // 先停掉所有定时器

    if (m_Paused) return;  // 暂停状态不启动任何定时器

    if (m_GameStatus == 0)  // 开始画面
    {
        SetTimer(hwnd, 1, 20, nullptr);   // 地面滚动（让地面在开始画面也有动画）
        SetTimer(hwnd, 4, 100, nullptr);  // 小鸟动画
    }

    if (m_GameStatus == 1)  // 游戏中
    {
        SetTimer(hwnd, 1, 20, nullptr);             // 地面+管子+碰撞+计分
        SetTimer(hwnd, 3, m_columnInterval, nullptr); // 生成管子
        SetTimer(hwnd, 4, 100, nullptr);             // 小鸟动画
        SetTimer(hwnd, 6, 30, nullptr);              // 小鸟物理
    }
    // 状态 2（结束）：不启动任何定时器，画面静止
}
```

### 8.4 定时器回调 —— 游戏的核心逻辑

```cpp
void CBirdAPP::On_WM_TIMER(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case 1:  // 每 20ms 执行一次 —— 主逻辑
    {
        // 移动地面
        m_Ground.MoveGround(worldStep);
        // 移动所有管子
        m_ColumnBox.MoveAllColumn(worldStep);
        // 更新道具（护盾、金币、二段跳、随机道具）
        UpdateShield(); CheckShieldPickup(); SpawnShieldIfNeeded();
        UpdateCoin();   CheckCoinPickup();   SpawnCoinIfNeeded();
        // ... 更多道具 ...

        // 遍历所有管子，检测碰撞和计分
        for (each column) {
            if (column->IfTouchBirdBox(小鸟位置)) {
                // 碰撞！
                if (有护盾) { 消耗护盾; }
                else { 游戏结束; }
            }
            if (管子已经完全经过小鸟 && 还没被计分) {
                // 计分！
                score += 1;
                检测完美穿越、惊险穿越、Fever...
            }
        }
    }
    break;

    case 3:  // 每 3000ms（动态调整）执行一次
    {
        // 生成一根新管子
        CColumn* newCol = new CColumn;
        newCol->InitColumn();
        m_ColumnBox.m_ColLst.push_back(newCol);
    }
    break;

    case 4:  // 每 100ms 执行一次
    {
        // 切换小鸟动画帧（0→1→2→...→7→0→...）
        m_PlayerBird.m_mode = (m_PlayerBird.m_mode + 1) % 8;
    }
    break;

    case 6:  // 每 30ms 执行一次
    {
        // 应用重力，更新小鸟位置
        m_PlayerBird.MovePlayerBird();
        // 检测是否掉到地面
        if (小鸟底部 >= 地面Y坐标) { 游戏结束; }
    }
    break;
    }
}
```

---

## 第九章：小鸟——物理、动画与渲染

### 9.1 小鸟的物理模型

```cpp
// CPlayerBird 类的核心属性
int m_x;       // 小鸟左上角的 X 坐标
int m_y;       // 小鸟左上角的 Y 坐标
int m_speed;   // 垂直速度（正数=向下，负数=向上）
int m_a;       // 加速度（重力）
int m_mode;    // 当前动画帧（0~7）
```

**物理公式：**

```
每 30ms 执行一次：
  速度 = 速度 + 加速度    （重力让速度越来越快地往下）
  位置 = 位置 + 速度      （根据速度更新位置）

点击拍翅膀时：
  速度 = -10              （给一个向上的初始速度）
```

用代码表示：

```cpp
void CPlayerBird::MovePlayerBird()
{
    m_speed += m_a;                    // 加上重力（m_a = 1）
    if (m_speed > BIRD_MAX_DOWN_SPEED) // 限制最大下落速度（11）
        m_speed = BIRD_MAX_DOWN_SPEED;
    m_y += m_speed;                    // 更新位置
}
```

**物理直觉：**

```
时刻 0:  点击拍翅膀 → speed = -10（向上飞）
时刻 1:  speed = -10 + 1 = -9   （还在上升，但开始减速）
时刻 2:  speed = -9 + 1 = -8    （继续减速）
...
时刻 10: speed = -10 + 10 = 0   （到达最高点，速度为 0）
时刻 11: speed = 0 + 1 = 1      （开始下落）
时刻 12: speed = 1 + 1 = 2      （下落越来越快）
...
时刻 21: speed = 10 + 1 = 11    （达到最大下落速度，不再加速）
```

这就是为什么小鸟的运动轨迹是一条「抛物线」——先上升、减速、到顶、然后加速下落。

### 9.2 拍翅膀的实现

```cpp
void CBirdAPP::TryFlap()
{
    if (m_GameStatus != 1) return;  // 只在游戏中才能拍
    if (m_Paused) return;           // 暂停时不能拍

    // 冷却机制：防止拍得太快
    int cooldown = (m_doubleJumpTicks > 0) ? 1 : 4;  // 二段跳时冷却更短
    if (m_flapCooldownTicks > 0) return;               // 还在冷却中
    m_flapCooldownTicks = cooldown;

    // 设置向上的速度
    int speed = (m_doubleJumpTicks > 0) ? -12 : -10;  // 二段跳时飞得更高
    m_PlayerBird.m_speed = speed;
}
```

### 9.3 小鸟的动画

小鸟有 8 帧动画，每 100ms 切换一帧，形成「拍翅膀」的视觉效果：

```cpp
// 定时器 4 的处理（每 100ms）
m_PlayerBird.m_mode = (m_PlayerBird.m_mode + 1) % 8;
// mode: 0→1→2→3→4→5→6→7→0→1→2→...
```

### 9.4 小鸟的渲染

```cpp
void CPlayerBird::ShowPlayerBird(int displayW, int displayH)
{
    // 第一步：用遮罩图做「扣除」操作（SRCPAINT = 或运算）
    putimage(m_x, m_y, displayW, displayH, &m_imgPlayer[m_mode], 0, 0, SRCPAINT);
    // 第二步：用原图做「填充」操作（SRCAND = 与运算）
    putimage(m_x, m_y, displayW, displayH, &m_imgPlayerFront[m_mode], 0, 0, SRCAND);
}
```

这两行代码实现了「透明贴图」效果，详见 [第十五章](#第十五章渲染技巧透明贴图的两层绘制法)。

---

## 第十章：管道障碍物——生成、移动与碰撞检测

### 10.1 管子的生成

```cpp
// CColumn::InitColumn()
void CColumn::InitColumn()
{
    loadimage(&m_imgColumn, "./res/column.png");          // 加载遮罩图
    loadimage(&m_imgColumnFront, "./res/columnFront.png"); // 加载颜色图

    m_x = WINDOW_WIDTH + 5;                                // 从屏幕右边外开始
    m_y = (-CBirdAPP::m_rand % 340) - 120;                // 随机垂直位置
    m_IfPass = false;                                       // 还没被穿过

    // 25% 概率生成「移动管子」
    m_IfMoving = ((CBirdAPP::m_rand % 100) < 25);
    m_vy = (CBirdAPP::m_rand % 2 == 0) ? 1 : -1;          // 随机初始移动方向
}
```

**`m_y` 为什么是负数？** 因为管子的图片有 1200 像素高，但窗口只有 740 像素。`m_y` 是管子图片左上角的 Y 坐标。当 `m_y` 为负数时，图片的上半部分被裁掉了，只露出下半部分的管口。通过改变 `m_y`，就改变了管口在屏幕上的位置，也就是改变了「缝隙」的位置。

### 10.2 管子的移动

```cpp
void CColumn::MoveColumn(int step)
{
    m_x -= step;  // 每次向左移动 step 个像素

    if (m_IfMoving)  // 如果是移动管子
    {
        m_y += m_vy;  // 垂直方向也移动
        if (m_y <= -480) { m_y = -480; m_vy = 1; }   // 到达上限，改为向下
        else if (m_y >= -80) { m_y = -80; m_vy = -1; } // 到达下限，改为向上
    }
}
```

### 10.3 碰撞检测 —— AABB 矩形碰撞

这是游戏开发中最基础的碰撞检测算法：

```cpp
bool CColumn::IfTouchBirdBox(int birdX, int birdY, int birdW, int birdH)
{
    // 第一步：检查水平方向是否重叠
    //  管子左边缘 <= 小鸟右边缘  &&  管子右边缘 >= 小鸟左边缘
    if (m_x <= birdX + birdW * 3/4 && m_x + IMG_COLUMN_WIDTH >= birdX)
    {
        // 第二步：检查垂直方向是否碰撞
        // 小鸟的碰撞范围比实际图片小一点（上下各裁掉 1/4），让游戏更宽容
        int birdTop = birdY + birdH * 1/4;     // 小鸟碰撞区域顶部
        int birdBottom = birdY + birdH * 3/4;  // 小鸟碰撞区域底部

        int gapTop = m_y + IMG_HALF_COLUMN_HEIGHT;  // 缝隙顶部
        int gapBottom = m_y + IMG_COLUMN_DOWN_Y;     // 缝隙底部

        // 如果小鸟不在缝隙范围内，就碰撞了
        if (gapTop >= birdTop || gapBottom <= birdBottom)
        {
            return true;  // 碰撞！
        }
    }
    return false;  // 没有碰撞
}
```

**用图来理解：**

```
    ┌─────────┐  ← 管子顶部 (m_y)
    │  管子   │
    │  实心   │
    ├─────────┤  ← 缝隙顶部 (gapTop = m_y + 524)
    │         │
    │  缝隙   │  ← 小鸟必须在这个范围内才安全
    │         │
    ├─────────┤  ← 缝隙底部 (gapBottom = m_y + 673)
    │  管子   │
    │  实心   │
    │         │
    └─────────┘  ← 管子底部 (m_y + 1200)

    检测逻辑：
    如果小鸟的碰撞区域和管子的实心部分重叠 → 碰撞！
```

### 10.4 CColumnBox —— 管子容器

```cpp
class CColumnBox {
    list<CColumn*> m_ColLst;  // 用链表存储所有管子的指针
};
```

**为什么用 `list<CColumn*>` 而不是数组？**

- 管子会不断地被创建和删除（新管子从右边出现，旧管子从左边消失）
- `list`（链表）在中间插入和删除元素的效率很高（O(1)）
- 如果用数组，每次删除一个管子，后面所有管子都要往前挪一位（O(n)）

**管子的生命周期管理：**

```cpp
void CColumnBox::MoveAllColumn(int step)
{
    auto ite = m_ColLst.begin();
    while (ite != m_ColLst.end())
    {
        if (*ite)
        {
            (*ite)->MoveColumn(step);  // 移动管子

            // 如果管子已经完全移出屏幕左边
            if ((*ite)->m_x < -IMG_COLUMN_WIDTH)
            {
                delete *ite;           // 释放内存
                *ite = nullptr;
                ite = m_ColLst.erase(ite);  // 从链表中移除
                continue;
            }
        }
        ite++;
    }
}
```

**⚠️ 重要概念：内存管理！**

```cpp
CColumn* newCol = new CColumn;   // 「new」在堆上分配内存
// ...
delete *ite;                      // 「delete」释放内存
```

在 C++ 中，`new` 分配的内存必须手动 `delete`，否则会造成「内存泄漏」（内存被占用但没人用，最终导致程序越来越慢甚至崩溃）。

---

## 第十一章：地面与背景——滚动效果的实现

### 11.1 地面的无限滚动

```cpp
void CGround::MoveGround(int step)
{
    m_x -= step;                    // 向左移动
    if (m_x <= -IMG_GROUND_WIDTH)   // 如果移出了一整个地面的宽度
    {
        m_x = 0;                    // 回到初始位置
    }
}
```

**原理图：**

```
初始状态：   [地面图片][地面图片]
             ↑ m_x = 0

移动后：     [地面图片][地面图片]
                   ↑ m_x = -50

继续移动：   [地面图片][地面图片]
                         ↑ m_x = -300

快移出时：   [地面图片][地面图片]
                               ↑ m_x = -399

重置：       [地面图片][地面图片]
             ↑ m_x = 0  （看起来无缝衔接，因为两张图是一样的）
```

因为地面图片的左右两端是无缝衔接的，所以重置 `m_x = 0` 时，视觉上看起来是连续滚动的。

### 11.2 背景为什么不滚动？

```cpp
// CBackGround.cpp 中，移动代码被注释掉了
// void CBackGround::MoveBackGround(int step) { ... }
```

背景是静态的——因为 Flappy Bird 的背景本身就不需要滚动（只有地面和管子在动）。这让视觉效果更自然：背景是「远方」，不应该跟着动。

---

## 第十二章：计分系统——从简单到复杂的进化

### 12.1 基础计分

```cpp
// 当管子完全经过小鸟时
if (column->m_x + IMG_COLUMN_WIDTH <= m_PlayerBird.m_x && !column->m_IfPass)
{
    column->m_IfPass = true;  // 标记为已穿过（防止重复计分）
    m_score += 1;              // +1 分
}
```

**`m_IfPass` 的作用：** 管子从右往左移动，当管子的右边缘经过小鸟的左边缘时，说明小鸟成功穿过了。但这个条件在管子继续移动时会一直成立，所以需要一个标志位 `m_IfPass` 来确保只计分一次。

### 12.2 完美穿越 (PERFECT)

```cpp
int gapCenter = (gapTop + gapBottom) / 2;  // 缝隙的中心点
int birdCenter = m_PlayerBird.m_y + IMG_BIRD_HEIGHT / 2;  // 小鸟的中心点
int diff = birdCenter - gapCenter;  // 小鸟中心和缝隙中心的距离
if (diff < 0) diff = -diff;         // 取绝对值

if (diff <= m_perfectWindow)  // 如果距离在「完美窗口」内（默认 18 像素）
{
    m_perfectStreak++;  // 完美连击 +1
    int streak = min(m_perfectStreak, 5);  // 最高 5 倍
    int bonus = 2 * streak;                 // 奖励分 = 2 × 连击数
    m_score += bonus;
}
else
{
    m_perfectStreak = 0;  // 不够完美，连击归零
}
```

**奖励梯度：**

| 连击数 | 额外加分 | 累计额外 |
|--------|----------|----------|
| 第 1 次完美 | +2 | 2 |
| 连续 2 次 | +4 | 6 |
| 连续 3 次 | +6 | 12 |
| 连续 4 次 | +8 | 20 |
| 连续 5 次 | +10 | 30 |

### 12.3 惊险穿越 (NEAR MISS)

```cpp
int bTop = m_PlayerBird.m_y + birdH / 4;     // 小鸟碰撞区域顶部
int bBot = m_PlayerBird.m_y + birdH * 3 / 4;  // 小鸟碰撞区域底部
int nearTop = abs(bTop - gapTop);              // 小鸟顶部离缝隙顶部的距离
int nearBot = abs(gapBottom - bBot);           // 小鸟底部离缝隙底部的距离

if (nearTop <= m_nearMissDist || nearBot <= m_nearMissDist)  // 距离 ≤ 8 像素
{
    m_score += 1;  // 额外 +1 分
    m_floatMsg = "NEAR MISS +1";
}
```

**设计哲学：** 这个机制奖励「大胆的玩家」。离管子越近越危险，但也越有利可图。这是风险与回报的平衡。

### 12.4 Fever 系统

```cpp
m_passStreak++;  // 每穿过一根管子，连击数 +1

if (m_passStreak >= 10)  // 连续穿过 10 根
{
    m_passStreak = 0;     // 连击归零
    m_feverTicks = 250;   // Fever 持续 250 个 tick（约 5 秒）
    m_floatMsg = "FEVER!";
}

// 在计分时：
m_score += 1 * GetScoreMultiplier();
// GetScoreMultiplier() 在 Fever 期间返回 2
```

### 12.5 难度递增

```cpp
void CBirdAPP::UpdateDifficulty()
{
    // 每 5 分，速度 +1（最快 10）
    m_speedStep = min(TIMER_GROUND_MOVE_STEP + m_score / 5, 10);

    // 每 5 分，管子生成间隔 -200ms（最快 1200ms）
    m_columnInterval = max(TIMER_NEW_COLUMN_INTERVAL - (m_score / 5) * 200, 1200);

    // 如果间隔变了，重新设置定时器
    if (intervalChanged && m_GameStatus == 1 && !m_Paused)
    {
        KillTimer(hwnd, TIMER_NEW_COLUMN_TIMERID);
        SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, m_columnInterval, nullptr);
    }
}
```

**难度曲线：**

| 分数 | 速度 | 管子间隔 |
|------|------|----------|
| 0-4  | 3    | 3000ms   |
| 5-9  | 4    | 2800ms   |
| 10-14| 5    | 2600ms   |
| 15-19| 6    | 2400ms   |
| 20-24| 7    | 2200ms   |
| 25-29| 8    | 2000ms   |
| 30+  | 10   | 1200ms   |

---

## 第十三章：道具系统——护盾、金币、强化

### 13.1 道具的通用模式

所有道具都遵循相同的生命周期模式：

```
生成 → 移动 → 检测拾取 → 应用效果 → 到期消失
Spawn → Move → CheckPickup → Apply → Expire
```

以护盾为例：

```cpp
// 1. 生成条件
void SpawnShieldIfNeeded()
{
    if (m_hasShield || m_shieldOnMap) return;  // 已经有护盾或地图上有护盾
    if (m_score % 8 != 0) return;               // 每 8 分生成一次
    if (m_lastShieldSpawnScore == m_score) return; // 防止同分重复生成

    m_shieldOnMap = true;
    m_shieldX = WINDOW_WIDTH + 30;              // 从屏幕右边外开始
    m_shieldY = 50 + rand() % (地面高度 - 130); // 随机高度
}

// 2. 移动
void UpdateShield()
{
    if (!m_shieldOnMap) return;
    m_shieldX -= m_speedStep;                   // 跟地面同速向左移动
    if (m_shieldX < -40) m_shieldOnMap = false; // 移出屏幕就消失
}

// 3. 拾取检测（AABB 碰撞）
bool CheckShieldPickup()
{
    if (!m_shieldOnMap) return false;

    // 护盾是一个圆形，用它的外接矩形来检测
    bool hit = !(盾右 < 鸟左 || 盾左 > 鸟右 || 盾底 < 鸟顶 || 盾顶 > 鸟底);
    if (hit) {
        m_shieldOnMap = false;  // 地图上的护盾消失
        m_hasShield = true;     // 小鸟获得护盾
        return true;
    }
    return false;
}

// 4. 效果应用（在碰撞检测中）
if (碰撞了 && m_hasShield) {
    m_hasShield = false;          // 消耗护盾
    m_PlayerBird.m_speed = -10;   // 给小鸟一个向上的速度（弹开效果）
    m_passStreak = 0;             // 连击归零
    m_perfectStreak = 0;          // 完美连击归零
}
```

### 13.2 四种随机道具

| 道具 | 代码标记 | 效果 | 持续时间 |
|------|----------|------|----------|
| Ghost（无敌） | `G` | 穿过管子和地面不碰撞 | 250 tick ≈ 7.5s |
| Shrink（缩小） | `K` | 小鸟碰撞体积缩小 30% | 300 tick ≈ 9s |
| SlowMo（减速） | `T` | 世界移动速度减半 | 250 tick ≈ 7.5s |
| 2X Score（双倍） | `X` | 所有得分翻倍 | 250 tick ≈ 7.5s |

```cpp
void CBirdAPP::ApplyPower(int type)
{
    if (type == 0) { m_ghostTicks = 250; m_floatMsg = "GHOST"; }
    if (type == 1) { m_shrinkTicks = 300; m_floatMsg = "SHRINK"; }
    if (type == 2) { m_slowMoTicks = 250; m_floatMsg = "SLOWMO"; }
    if (type == 3) { m_scoreMultiplierTicks = 250; m_floatMsg = "2X SCORE"; }
}
```

### 13.3 道具的绘制

道具在地图上显示为彩色圆圈 + 字母：

```cpp
void CBirdAPP::DrawShield()
{
    if (!m_shieldOnMap) return;
    int r = 14;
    setlinecolor(RGB(255, 180, 0));       // 金色边框
    circle(m_shieldX, m_shieldY, r);      // 外圈
    circle(m_shieldX, m_shieldY, r - 2);  // 内圈（形成环形效果）
    // 在圆圈中心写一个 "S" 字母
    ::drawtext("S", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
```

---

## 第十四章：商店系统——持久化存储

### 14.1 商店 UI

商店在开始画面显示，点击 SHOP 按钮展开：

```cpp
void CBirdAPP::DrawShop()
{
    // 画 SHOP 按钮
    rectangle(20, 650, 160, 700);
    ::drawtext(m_shopOpen ? "BACK" : "SHOP", ...);

    if (!m_shopOpen) return;

    // 画三个商品
    rectangle(20, 520, 380, 560);  // Start Shield 20 金币
    rectangle(20, 565, 380, 605);  // Perfect+ 15 金币
    rectangle(20, 610, 380, 650);  // DJ+ 25 金币
}
```

### 14.2 购买逻辑

```cpp
void CBirdAPP::BuyStartShield()
{
    const int cost = 20;
    if (!m_upgradeStartShield && m_coins >= cost) {  // 没买过 + 金币够
        m_coins -= cost;            // 扣金币
        m_upgradeStartShield = true; // 标记已购买
        SavePlayerSave();            // 保存到文件
    }
    // ... 错误提示 ...
}
```

### 14.3 文件持久化

```cpp
// 保存
void CBirdAPP::SavePlayerSave()
{
    ofstream out("player_save.txt", ios::trunc);  // 打开文件（清空原内容）
    out << m_coins << " "
        << (m_upgradeStartShield ? 1 : 0) << " "
        << m_perfectWindow << " "
        << m_upgradeDjBonusTicks;
    // 文件内容示例："5 0 18 0"
}

// 加载
void CBirdAPP::LoadPlayerSave()
{
    ifstream in("player_save.txt");  // 打开文件
    int coins, startShield, perfectWindow, djBonus;
    in >> coins >> startShield >> perfectWindow >> djBonus;  // 读取四个数
    m_coins = coins;
    m_upgradeStartShield = (startShield != 0);
    m_perfectWindow = perfectWindow;
    m_upgradeDjBonusTicks = djBonus;
}
```

**文件格式很简单：** 四个数字用空格分隔。

```
player_save.txt 内容：5 0 18 0
                       │ │ │  └── DJ+ 额外 ticks（0 = 未购买）
                       │ │ └──── 完美窗口大小（18 = 默认）
                       │ └────── 是否有开局护盾（0 = 没有）
                       └──────── 金币数量
```

---

## 第十五章：渲染技巧——透明贴图的两层绘制法

### 15.1 问题：图片有白色背景怎么办？

在早期的图形编程中，PNG 的透明通道（alpha channel）支持不好。如果你直接把一张有白色背景的小鸟图片画到蓝色的游戏背景上，你会看到一个白色的方块——小鸟周围全是白色。

### 15.2 解决方案：遮罩图 + 原图

每张图片准备两个版本：

**遮罩图 (Mask)** —— 黑色部分是要显示的，白色部分是透明的：

```
遮罩图示意：
┌────────────┐
│ 白白白白白白 │ ← 白色 = 透明（保留背景）
│ 白白白白白白 │
│ 白┌────┐白 │
│ 白│黑黑│白 │ ← 黑色 = 不透明（画上小鸟）
│ 白│黑黑│白 │
│ 白└────┘白 │
│ 白白白白白白 │
└────────────┘
```

**原图 (Front)** —— 黑色背景 + 彩色小鸟：

```
原图示意：
┌────────────┐
│ 黑黑黑黑黑黑 │ ← 黑色 = 透明部分（配合遮罩图使用）
│ 黑黑黑黑黑黑 │
│ 黑┌────┐黑 │
│ 黑│彩色│黑 │ ← 彩色 = 小鸟的实际颜色
│ 黑│小鸟│黑 │
│ 黑└────┘黑 │
│ 黑黑黑黑黑黑 │
└────────────┘
```

### 15.3 两步绘制过程

```cpp
// 第一步：SRCPAINT（或运算）
// 遮罩图的白色部分（11111111）和背景做或运算 → 保持白色（被后续步骤处理）
// 遮罩图的黑色部分（00000000）和背景做或运算 → 保持背景色
putimage(x, y, w, h, &mask, 0, 0, SRCPAINT);

// 第二步：SRCAND（与运算）
// 原图的彩色部分和第一步的结果做与运算 → 显示彩色
// 原图的黑色部分（00000000）和第一步的结果做与运算 → 变成黑色（被遮罩覆盖）
putimage(x, y, w, h, &front, 0, 0, SRCAND);
```

**用数学理解：**

```
设：B = 背景像素, M = 遮罩像素, F = 原图像素

遮罩图中：透明区域 M=白色(11111111)，不透明区域 M=黑色(00000000)
原图中：  透明区域 F=黑色(00000000)，不透明区域 F=彩色

第一步：R1 = B OR M
  透明区域：R1 = B OR 11111111 = 11111111（白色）
  不透明区域：R1 = B OR 00000000 = B（背景色）

第二步：R2 = R1 AND F
  透明区域：R2 = 11111111 AND 00000000 = 00000000（黑色 → 被遮罩图挡住）
  不透明区域：R2 = B AND F = F（显示小鸟颜色）

等等，透明区域变成黑色了？没关系！因为遮罩图在第一步已经把透明区域变成了白色，
而原图在透明区域是黑色，两者 AND 后是黑色——但这个黑色会被遮罩图的白色区域「盖住」。

实际效果：
  - 小鸟部分显示彩色
  - 其他部分显示背景色
  → 完美透明效果！
```

### 15.4 现代替代方案

现在有更简单的方法：直接使用带 Alpha 通道的 PNG 图片，用 `AlphaBlend` 函数。但这个项目用的是经典的两层绘制法，这也是学习 EasyX 的一个很好的练习。

---

## 第十六章：设计哲学总结——从这个项目我们能学到什么？

### 16.1 框架与实现分离

```
CGameFrame（框架层）        CBirdAPP（实现层）
┌─────────────────────┐    ┌─────────────────────┐
│ 创建窗口             │    │ 加载小鸟图片         │
│ 消息路由             │    │ 设置游戏定时器       │
│ 双缓冲绘制           │    │ 管子碰撞检测         │
│ 定时器管理           │    │ 计分逻辑             │
│ 生命周期管理         │    │ 道具系统             │
└─────────────────────┘    └─────────────────────┘
     通用、可复用                具体、不可复用
```

**设计原则：** 把「不变的东西」（框架）和「会变的东西」（具体游戏逻辑）分离开。这样如果要做新游戏，框架可以直接复用。

### 16.2 组合优于继承

```
CBirdAPP 不是继承自 CPlayerBird 或 CColumn
而是「拥有」它们（组合关系）

CBirdAPP has-a CPlayerBird    // 小鸟是游戏的一部分
CBirdAPP has-a CColumnBox     // 管子容器是游戏的一部分
CBirdAPP has-a CGround        // 地面是游戏的一部分
```

### 16.3 单一职责原则

每个类只负责一件事：

| 类 | 职责 |
|----|------|
| `CPlayerBird` | 小鸟的位置、速度、动画、渲染 |
| `CColumn` | 单根管子的位置、移动、碰撞 |
| `CColumnBox` | 管子集合的管理（创建、遍历、删除） |
| `CGround` | 地面的滚动和渲染 |
| `CBackGround` | 背景的加载和渲染 |
| `CBirdAPP` | 把以上所有组件组装起来，实现游戏逻辑 |

### 16.4 状态驱动的行为

```cpp
// 同一个函数，在不同状态下行为不同
void On_Paint() {
    if (状态 == 0) 画开始画面();
    if (状态 == 1) 画游戏画面();
    if (状态 == 2) 画结束画面();
}

void On_WM_LBUTTONDOWN() {
    if (状态 == 0) 处理开始画面的点击();
    if (状态 == 1) 让小鸟飞();
    if (状态 == 2) 处理结束画面的点击();
}
```

### 16.5 数据驱动的难度设计

难度不是硬编码的，而是根据分数动态计算：

```cpp
速度 = 基础速度 + 分数 / 5;     // 线性增长
间隔 = 基础间隔 - 分数 / 5 × 200; // 线性递减
```

这样做的好处是：调整难度只需要改公式，不用手动设置每个分数段的参数。

### 16.6 从这个项目我们可以看到一个游戏的「骨架」

```
┌──────────────────────────────────────────────────┐
│                    main()                         │
│              创建游戏 → 初始化 → 等待结束          │
└───────────────────────┬──────────────────────────┘
                        │
            ┌───────────┴───────────┐
            ▼                       ▼
    ┌──────────────┐       ┌──────────────┐
    │  消息驱动     │       │  定时器驱动   │
    │  (用户输入)   │       │  (自动更新)   │
    │              │       │              │
    │ 鼠标点击     │       │ 每20ms: 移动  │
    │ 键盘按下     │       │ 每30ms: 物理  │
    │ 窗口关闭     │       │ 每100ms: 动画 │
    └──────┬───────┘       │ 每3000ms: 生成│
           │               └──────┬───────┘
           │                      │
           └──────────┬───────────┘
                      ▼
              ┌──────────────┐
              │   渲染绘制    │
              │  (每帧重画)   │
              │              │
              │ 背景→管子→地面│
              │ →道具→小鸟→UI │
              └──────────────┘
```

---

## 附录：如何自己编译运行这个项目

### 方法一：直接运行

项目根目录已经有编译好的 `FlappyBird.exe`，双击即可运行。

**前提条件：** 需要 Windows 系统，且 `res/` 文件夹和 `.exe` 在同一目录。

### 方法二：自己编译

#### 步骤 1：安装 Visual Studio

下载 [Visual Studio 2022 Community](https://visualstudio.microsoft.com/)（免费），安装时勾选「使用 C++ 的桌面开发」工作负载。

#### 步骤 2：安装 EasyX

访问 [EasyX 官网](https://easyx.cn/)，下载安装程序。它会自动集成到 Visual Studio 中。

#### 步骤 3：创建项目

1. 打开 Visual Studio → 创建新项目 → 选择「空项目」
2. 项目名称：`FlappyBird`
3. 平台：x86（32 位）

#### 步骤 4：添加源文件

将项目中所有 `.cpp` 和 `.h` 文件添加到项目中：

```
GameFrame/main.cpp
GameFrame/frame.h
GameConfig/GameConfig.h
BirdAPP/CBirdAPP.h
BirdAPP/CBirdAPP.cpp
PlayerBird/CPlayerBird.h
PlayerBird/CPlayerBird.cpp
Column/CColumn.h
Column/CColumn.cpp
Column/CColumnBox.h
Column/CColumnBox.cpp
BackGround/CBackGround.h
BackGround/CBackGround.cpp
Ground/CGround.h
Ground/CGround.cpp
BeforeGame/BeforeGame.h
BeforeGame/BeforeGame.cpp
AfterGame/CAfterGame.h
AfterGame/CAfterGame.cpp
```

#### 步骤 5：设置工作目录

项目属性 → 调试 → 工作目录 → 设置为项目根目录（包含 `res/` 的那个目录）。

#### 步骤 6：编译运行

按 `F5` 或点击「本地 Windows 调试器」。

### 方法三：修改和实验

编译成功后，试着修改这些参数来理解代码：

```cpp
// GameConfig.h —— 试试改这些值：

// 改变窗口大小
#define IMG_BACKGROUND_WIDTH 600      // 改大一点会怎样？
#define IMG_BACKGROUND_HEIGHT 900

// 改变物理参数
#define BIRD_STARTSPEED  -15          // 飞得更高？
#define BIRD_A           2            // 重力更大？
#define BIRD_MAX_DOWN_SPEED  20       // 下落更快？

// 改变定时器间隔
#define TIMER_GROUND_MOVE_INTERVAL  10   // 移动更快？
#define TIMER_BIRD_POSITION_INTERVAL 60  // 物理更新更慢？
```

**建议实验顺序：**

1. 先改 `BIRD_STARTSPEED` 为 `-20`，感受一下超强弹跳
2. 改 `BIRD_A` 为 `3`，感受重力变强
3. 改 `TIMER_GROUND_MOVE_INTERVAL` 为 `10`，感受加速世界
4. 尝试修改 `CColumn::InitColumn()` 中的概率，让所有管子都变成移动管子

---

## 结语

这个项目虽然只有 9 个源文件、约 2000 行代码，但它包含了游戏开发的所有核心概念：

- **程序架构**：框架模式、消息驱动、状态机
- **游戏物理**：重力模拟、速度/加速度
- **图形渲染**：双缓冲、透明贴图、动画
- **碰撞检测**：AABB 矩形碰撞
- **内存管理**：new/delete、容器管理
- **数据持久化**：文件读写
- **游戏设计**：计分、道具、难度曲线、风险回报

理解了这些，你就具备了理解任何 2D 游戏的基础。下一步可以尝试：

1. 给游戏添加音效（EasyX 支持 `mciSendString` 播放音乐）
2. 用 PNG Alpha 通道替代两层绘制法
3. 添加粒子效果（爆炸、星星）
4. 实现在线排行榜
5. 用这个游戏框架做一个「俄罗斯方块」或「打砖块」

**最重要的学习方法：读代码 → 改代码 → 看效果 → 理解为什么。** 不要怕改坏，改坏了大不了重新来过。
