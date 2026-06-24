                 评分：




《面向对象程序设计实践》课程
综合实训报告



题  目       Flappy Bird EasyX 游戏



学    院：       计算机学院
班    级：       计科专业
小组编号 ：         第 组
任课教师：
学    期：      2025-2026（1）

---

# Flappy Bird EasyX 游戏

Github仓库：Flappy-Bird-EasyX-main

## 1 系统分析

### 1.1 应用系统简介

本课设面向经典休闲游戏场景，设计并实现一套基于 C++ 与 EasyX 图形库的 "Flappy Bird" 游戏系统。系统围绕 "小鸟飞行—管道障碍—道具收集—商店升级"核心玩法链，支持玩家通过鼠标或键盘控制小鸟飞行、穿越管道障碍、收集道具增强能力、在商店中购买永久升级等功能，旨在通过面向对象方法完成游戏业务抽象、模块划分、类设计与数据持久化落地，并在可运行的游戏原型中体现封装、继承、多态等核心思想。

系统在需求层面具有如下约束：

（1）**玩法边界**：聚焦 Flappy Bird 核心飞行玩法，不覆盖联网对战、关卡编辑等外围功能；

（2）**数据一致性**：最高分记录、玩家金币与升级存档必须在文件中可持久化、可回溯；

（3）**状态约束**：开始画面、游戏中、游戏结束三种状态的操作边界清晰，定时器启停严格按状态管理；

（4）**可扩展性**：采用框架与实现分离的架构，预留游戏替换扩展点，便于后期将框架用于其他游戏类型。

### 1.2 系统需求（功能）分析

结合 Flappy Bird 游戏的典型玩法流程，系统需求分为玩家操作需求与系统机制需求两类：

#### 1）玩家操作需求

玩家需要完成游戏启动、小鸟飞行控制（鼠标点击/键盘空格/方向上键）、暂停与恢复（P键）、重新开始（R键）、查看当前分数与最高分、在开始画面浏览商店并购买升级、在游戏中购买二段跳道具（E键）等操作。玩家操作需实时响应，确保游戏体验的流畅性。

#### 2）系统机制需求

系统需要完成以下核心机制：

- **物理模拟**：模拟小鸟的重力下落与拍翅膀上升的抛物线运动轨迹；
- **管道生成与管理**：定时生成随机高度的管道障碍物，支持固定管道与上下移动管道（25%概率），管道离开屏幕后自动回收内存；
- **碰撞检测**：实时检测小鸟与管道、小鸟与地面的碰撞，碰撞时若持有护盾则消耗护盾，否则游戏结束；
- **计分系统**：基础穿越+1分，完美穿越（从缝隙中心穿过）额外加分并支持连击奖励，惊险穿越（贴近管壁）额外+1分，连续穿越10根管道触发Fever双倍得分模式；
- **道具系统**：地图上定时生成护盾（S）、金币（C）、二段跳（D）、随机道具（G/K/T/X），玩家控制小鸟接触道具即可拾取生效；
- **难度递增**：随分数提高，管道移动速度加快、生成间隔缩短；
- **商店系统**：开始画面提供商店界面，支持使用金币购买开局护盾、完美窗口加大、二段跳增强三项永久升级；
- **数据持久化**：最高分保存至 `best_score.txt`，玩家金币与升级存档保存至 `player_save.txt`；
- **透明渲染**：采用遮罩图+原图的两层绘制法实现精灵透明贴图效果。

#### 3）非功能需求

（1）**性能需求**：游戏画面刷新流畅，物理更新间隔30ms，主逻辑更新间隔20ms，确保无明显卡顿；

（2）**可用性需求**：操作简单直观，提供浮动消息提示（如"PERFECT x3 +6"、"FEVER!"、"SHIELD!"等），帮助玩家理解游戏状态；

（3）**可维护性需求**：采用分层架构与面向对象设计，每个游戏组件独立封装，职责清晰，便于后续功能扩展。

---

## 2 系统设计

### 2.1 总体设计

系统采用 "框架层 + 实现层" 的总体设计思路，将游戏开发中的通用机制与具体玩法逻辑分离为相对独立的层次，并通过继承与组合关系实现模块协作与解耦。整体架构划分为三层：

1. **框架层（GameFrame）**：负责窗口创建、消息路由、双缓冲绘制、定时器管理等通用游戏机制；
2. **游戏逻辑层（BirdAPP）**：负责游戏状态管理、物理模拟、碰撞检测、计分、道具、商店等具体玩法实现；
3. **组件层（PlayerBird/Column/ColumnBox/BackGround/Ground/BeforeGame/AfterGame）**：负责各游戏实体的数据封装与渲染。

功能模块划分如下：

**1）游戏框架模块（GameFrame）**
提供程序入口与游戏基类框架。`CGameFrame` 抽象基类定义了游戏的初始化（`On_Init`）、绘制（`On_Paint`）、关闭（`On_Close`）三个纯虚函数接口，以及鼠标点击、键盘按键、定时器等消息处理的虚函数接口。通过消息映射表机制将 Windows 消息路由到对应的成员函数，实现了消息驱动的游戏架构。`main.cpp` 作为程序入口，通过工厂宏 `DYNAMIC_CREATE` 创建具体游戏对象，调用框架初始化后进入消息等待循环。

**2）游戏配置模块（GameConfig）**
集中管理所有游戏常量，包括窗口尺寸（400×740）、图片尺寸（小鸟56×48、管子78×1200、地面400×354）、定时器ID与间隔、物理参数（初始速度-10、重力加速度1、最大下落速度11）、按钮碰撞区域坐标等。通过 `#define` 宏定义实现常量的统一管理，修改游戏参数只需调整此文件。

**3）游戏主控模块（BirdAPP）**
继承自 `CGameFrame`，是整个游戏的核心控制器。通过组合方式持有所有游戏组件对象（背景、地面、小鸟、管子容器、开始画面、结束画面），并实现全部游戏逻辑：游戏状态机管理（开始/游戏中/结束）、定时器驱动的主循环、碰撞检测与计分、道具生成与拾取、商店购买与存档、难度递增算法等。

**4）小鸟模块（PlayerBird）**
封装小鸟的位置、速度、加速度、动画帧等属性，提供物理移动（重力模拟）、透明渲染（遮罩图+原图两层绘制）、动画帧切换等功能。

**5）管子模块（Column/ColumnBox）**
`CColumn` 封装单根管子的位置、移动逻辑、碰撞检测算法（AABB矩形碰撞），支持固定管子与移动管子两种类型。`CColumnBox` 作为管子容器，使用 `list<CColumn*>` 链表管理所有管子的生命周期（创建、遍历、自动回收）。

**6）场景模块（BackGround/Ground/BeforeGame/AfterGame）**
分别负责静态背景渲染、滚动地面渲染、开始画面覆盖层渲染、结束画面覆盖层渲染。

### 2.2 详细设计

#### 2.2.1 数据结构设计

本系统采用文件存储方式对游戏数据进行持久化管理。围绕 Flappy Bird 游戏的核心数据，设计覆盖最高分记录与玩家存档两个数据文件。

**1）best_score.txt（最高分记录文件）**

存储格式：单行整数，记录玩家历史最高得分。

```
字段说明：
score    INT    当前历史最高分（主数据）
```

示例内容：`21`

**2）player_save.txt（玩家存档文件）**

存储格式：单行四个空格分隔的整数，记录玩家的永久资产与升级状态。

```
字段说明：
coins              INT    金币数量（主数据）
startShield        INT    是否拥有开局护盾（0=无，1=有）
perfectWindow      INT    完美穿越判定窗口像素大小（默认18，升级后26）
djBonusTicks       INT    二段跳额外持续时间（0=未升级，300=已升级）
```

示例内容：`5 0 18 0`（含义：5金币，无开局护盾，完美窗口18像素，无二段跳加成）

**一致性约束**：两个文件独立存储，通过 `CBirdAPP` 类在构造时统一加载、在关键操作（金币变化、升级购买、游戏结束）时统一保存，保证数据的完整性与可追溯性。

#### 2.2.2 主要类设计

本系统在实现过程中采用面向对象方法对游戏业务进行建模，根据系统功能与代码结构，将主要类划分为 **框架基类**、**游戏主控类**、**游戏实体类** 三类。各类之间职责边界清晰，通过继承与组合完成协作，从而提高系统的可维护性与扩展性。

**（1）框架基类（CGameFrame）**

框架基类作为游戏的抽象骨架，负责定义游戏生命周期的标准接口与消息路由机制。

**1）CGameFrame（游戏框架抽象基类）**

该类作为所有游戏的公共基类，负责完成窗口创建、消息映射表初始化、双缓冲绘制流程以及游戏生命周期管理。通过纯虚函数强制子类实现初始化、绘制与关闭逻辑，通过虚函数允许子类选择性地处理鼠标、键盘与定时器消息。

该类主要职责包括：

1. 定义纯虚接口 `On_Init()`、`On_Paint()`、`On_Close()`，强制子类实现游戏初始化、画面绘制与资源清理；
2. 维护消息映射表 `m_msgmap`（`map<UINT, Fun_Type>`），将 Windows 消息 ID 映射到对应的成员函数指针；
3. 提供 `InitGame()` 方法完成窗口创建与回调函数注册；
4. 提供 `PaintGame()` 方法实现双缓冲绘制流程（`BeginBatchDraw` → `cleardevice` → `On_Paint` → `EndBatchDraw`）；
5. 提供 `EndGame()` 方法完成资源清理与退出标志设置。

通过引入该抽象基类，实现了游戏框架与具体游戏逻辑的分离。若开发新游戏（如俄罗斯方块），只需继承 `CGameFrame` 并实现三个纯虚函数，`main.cpp` 无需任何修改。

**（2）游戏主控类（CBirdAPP）**

**2）CBirdAPP（Flappy Bird 游戏主控类）**

该类继承自 `CGameFrame`，是 Flappy Bird 游戏的核心控制器。通过组合方式持有所有游戏组件对象，集中管理游戏状态、计分、道具、商店等全部业务逻辑。

该类主要职责包括：

1. **状态管理**：维护 `m_GameStatus`（0=开始，1=游戏中，2=结束）和 `m_Paused`（暂停标志），控制定时器的启停与消息处理的分支逻辑；
2. **物理与碰撞**：在定时器回调中驱动小鸟物理更新、管子移动、碰撞检测（管子AABB碰撞+地面碰撞）；
3. **计分系统**：实现基础穿越计分、完美穿越加分与连击奖励、惊险穿越加分、Fever双倍模式，以及难度递增算法；
4. **道具系统**：管理护盾、金币、二段跳、随机道具（Ghost/Shrink/SlowMo/2X）的生成、移动、拾取检测与效果应用；
5. **商店系统**：提供开始画面的商店界面，支持三种永久升级的购买逻辑（开局护盾20金币、完美窗口加大15金币、二段跳增强25金币）；
6. **数据持久化**：提供 `LoadBestScore()`/`SaveBestScore()` 与 `LoadPlayerSave()`/`SavePlayerSave()` 方法，实现最高分与玩家存档的文件读写；
7. **渲染调度**：在 `On_Paint()` 中根据游戏状态调用各组件的绘制方法，实现分层渲染（背景→管子→地面→UI→道具→小鸟→浮动消息）。

该类与各游戏组件之间采用组合关系（has-a），与 `CGameFrame` 之间采用继承关系（is-a），体现了面向对象设计中组合优于继承的原则。

**（3）游戏实体类**

**3）CPlayerBird（小鸟类）**

该类封装小鸟精灵的全部属性与行为。功能：维护小鸟的位置坐标（m_x, m_y）、垂直速度（m_speed）、加速度（m_a）及当前动画帧（m_mode），提供物理移动与透明渲染接口。

主要接口包括：`InitPlayerBird()` 加载8帧动画的遮罩图与原图并初始化位置和物理参数；`MovePlayerBird()` 实现重力物理（`speed += acceleration; y += speed`，速度上限11）；`ShowPlayerBird(displayW, displayH)` 采用两层绘制法实现透明贴图（先 `SRCPAINT` 绘制遮罩图，再 `SRCAND` 绘制原图）。

**4）CColumn（管子类）**

该类封装单根管道障碍物的属性与行为。功能：维护管子的位置坐标（m_x, m_y）、是否已穿过标志（m_IfPass）、是否为移动管子标志（m_IfMoving）及垂直移动速度（m_vy），提供移动与碰撞检测接口。

主要接口包括：`InitColumn()` 加载管子图片，随机生成垂直位置（`m_y` 范围-120到-460），以25%概率决定是否为移动管子；`MoveColumn(step)` 实现水平左移与移动管子的垂直振荡（在y=-480到y=-80之间往复）；`IfTouchBirdBox(birdX, birdY, birdW, birdH)` 实现AABB矩形碰撞检测，水平方向检测管子与小鸟的重叠，垂直方向检测小鸟碰撞区域（上下各裁去1/4）是否在管子缝隙之外。

**5）CColumnBox（管子容器类）**

该类使用 `list<CColumn*>` 链表管理所有管子对象的生命周期。功能：提供管子集合的统一遍历、移动与绘制接口，并在管子移出屏幕左侧时自动释放内存。

主要接口包括：`ShowAllColumn()` 遍历链表调用每个管子的绘制方法；`MoveAllColumn(step)` 遍历链表移动所有管子，对 `m_x < -IMG_COLUMN_WIDTH` 的管子执行 `delete` 并从链表中 `erase`，实现内存的自动回收；`ClearAll()` 删除所有管子并清空链表，用于游戏重新开始时的资源清理。

**6）CBackGround（背景类）/ CGround（地面类）**

`CBackGround` 负责加载并绘制静态背景图片（400×740）。`CGround` 负责加载并绘制滚动地面图片（400×354），提供 `MoveGround(step)` 方法实现无限循环滚动（当 `m_x <= -地面宽度` 时重置为0），以及 `IfTouchGround()` 方法检测小鸟是否触地。

**7）CBeforeGame（开始画面类）/ CAfterGame（结束画面类）**

分别负责加载并绘制开始画面与结束画面的覆盖层图片，均采用遮罩图+原图的两层透明绘制法。

**（4）类设计的整体关系说明**

系统主要类之间通过继承与组合形成清晰的层次关系：

1. `CBirdAPP` 继承自 `CGameFrame`（框架复用），是系统的核心调度中心；
2. `CBirdAPP` 组合持有 `CBackGround`、`CGround`、`CPlayerBird`、`CColumnBox`、`CBeforeGame`、`CAfterGame` 六个组件对象（模块化组合）；
3. `CColumnBox` 持有 `list<CColumn*>` 管理多个 `CColumn` 对象（容器管理）；
4. 所有实体类均依赖 `GameConfig.h` 中的常量定义（配置集中管理）。

通过上述设计，系统在类结构层面形成了"框架—主控—组件"的三层架构，各层职责单一、接口清晰，为系统的功能实现与后期扩展奠定了良好基础。

---

## 3 系统实现

### 3.1 系统开发环境

本系统在 Windows 平台下完成开发与调试，采用 C++ 语言结合 EasyX 图形库实现游戏功能，使用文件存储实现数据持久化。系统整体开发环境配置如下。

在操作系统方面，系统运行于 Windows 桌面环境下，依托 Visual Studio 2022 提供的集成开发环境完成程序的编译、调试与运行。开发过程中使用 EasyX 图形库构建游戏窗口与图形界面，通过 Win32 定时器机制驱动游戏主循环，从而实现流畅的游戏体验。

在程序设计语言方面，系统核心逻辑采用 C++ 语言实现。通过面向对象方法对游戏业务进行建模，将系统划分为框架基类、游戏主控类以及各游戏实体类等模块，各模块之间职责清晰，便于维护与扩展。程序使用 Visual Studio 工程文件进行组织，支持多源文件协同编译。

在数据管理方面，系统采用纯文本文件（`best_score.txt` 和 `player_save.txt`）进行数据持久化存储，通过 C++ 标准库的 `fstream` 进行文件读写操作，实现了最高分记录与玩家存档的跨会话保存。

在版本管理方面，系统源代码通过 Git 进行集中管理，便于团队成员协同开发与版本迭代。

### 3.2 程序编码

从整体结构来看，这个 Flappy Bird 游戏以"框架初始化→消息驱动→定时器更新→状态渲染"为主线，围绕小鸟飞行、管道穿越、道具收集、商店升级四条玩法链路组织功能。

#### 3.2.1 程序入口与工厂模式

程序从 `main()` 函数开始运行。考虑到框架的可复用性，`main.cpp` 不直接创建具体的游戏对象，而是通过工厂宏 `DYNAMIC_CREATE` 生成工厂函数 `CreatObject()`，由该函数负责创建 `CBirdAPP` 对象。这样做的好处是：如果以后要做另一个游戏（如俄罗斯方块），只需写一个新的类并替换一行宏调用，`main.cpp` 完全不用修改。

```cpp
// CBirdAPP.cpp 中使用工厂宏
DYNAMIC_CREATE(CBirdAPP)
// 展开后等价于：
// CGameFrame* CreatObject(){ return new CBirdAPP; }

int main()
{
    P_G = CreatObject();  // 通过工厂函数创建游戏对象（多态）
    P_G->InitGame(WND_TITLE, WND_WIDTH, WND_HEIGHT, WND_POS_X, WND_POS_Y);
    P_G->PaintGame();     // 绘制初始画面

    while (!P_G->IfEndGame()) {
        Sleep(200);  // 等待游戏结束，真正的逻辑在定时器回调中
    }

    delete P_G;
    return 0;
}
```

#### 3.2.2 框架层：消息映射与回调路由

框架层的核心设计是消息映射表机制。`CGameFrame` 维护一个 `map<UINT, Fun_Type>` 映射表，将 Windows 消息 ID（如 `WM_LBUTTONDOWN`、`WM_KEYDOWN`、`WM_TIMER`）映射到对应的成员函数指针。当 Windows 发送消息时，回调函数 `Wndproc` 查找映射表并调用对应的处理函数。

```cpp
// frame.h 中的消息映射初始化宏
#define INIT_MSGMAP(MSGID, MSGTYPE)                              \
    m_msgmap[MSGID].msg_type = MSGTYPE;                          \
    m_msgmap[MSGID].p_fun_##MSGTYPE = &CGameFrame::On_##MSGID;

void InitMsgMap()
{
    INIT_MSGMAP(WM_LBUTTONDOWN, EM_MOUSE)   // 鼠标左键 → On_WM_LBUTTONDOWN
    INIT_MSGMAP(WM_CLOSE, EM_WINDOW)        // 关闭窗口 → On_WM_CLOSE
    INIT_MSGMAP(WM_KEYDOWN, EM_KEY)         // 键盘按下 → On_WM_KEYDOWN
    INIT_MSGMAP(WM_CHAR, EM_CHAR)           // 字符输入 → On_WM_CHAR
    INIT_MSGMAP(WM_TIMER, EM_WINDOW)        // 定时器   → On_WM_TIMER
}

// main.cpp 中的回调函数
LRESULT Wndproc(HWND hwnd, UINT msgid, WPARAM wparam, LPARAM lparam)
{
    if (P_G->m_msgmap.count(msgid))  // 查找映射表
    {
        switch (P_G->m_msgmap[msgid].msg_type)
        {
        case EM_MOUSE:
            (P_G->*(P_G->m_msgmap[msgid].p_fun_EM_MOUSE))(po);
            break;
        case EM_KEY:
            (P_G->*(P_G->m_msgmap[msgid].p_fun_EM_KEY))(wparam);
            break;
        case EM_WINDOW:
            (P_G->*(P_G->m_msgmap[msgid].p_fun_EM_WINDOW))(wparam, lparam);
            break;
        }
        if (!P_G->IfEndGame()) {
            P_G->PaintGame();  // 消息处理后重绘画面
        }
    }
    return DefWindowProc(hwnd, msgid, wparam, lparam);
}
```

通过引入消息映射表机制，实现了消息类型与处理函数的解耦。新增消息处理只需添加一行宏调用，无需修改回调函数代码。

#### 3.2.3 双缓冲绘制机制

为避免画面闪烁，框架层采用 EasyX 的双缓冲绘制机制。`PaintGame()` 方法在每次调用时先开启批量绘制（`BeginBatchDraw`），清空画布后调用子类的 `On_Paint()` 绘制具体内容，最后一次性提交显示（`EndBatchDraw`）。

```cpp
void CGameFrame::PaintGame()
{
    BeginBatchDraw();   // 开始批量绘制（先画到内存）
    cleardevice();      // 清空画布
    On_Paint();         // 调用子类绘制（由 CBirdAPP 实现）
    EndBatchDraw();     // 一次性显示（避免闪烁）
}
```

#### 3.2.4 定时器驱动的游戏主循环

与传统的 `while(true)` 游戏循环不同，本系统采用 Win32 定时器驱动架构。游戏在 `main()` 中不进行主动循环，而是通过六个定时器分别驱动不同的游戏逻辑：

```cpp
void CBirdAPP::StartTimersForStatus()
{
    StopAllTimers();
    if (m_Paused) return;

    if (m_GameStatus == 0)  // 开始画面
    {
        SetTimer(hwnd, TIMER_GROUND_MOVE_TIMERID, TIMER_GROUND_MOVE_INTERVAL, nullptr);  // 地面滚动
        SetTimer(hwnd, TIMER_BIRD_CHANGE_TIMERID, TIMER_BIRD_CHANGE_INTERVAL, nullptr);  // 小鸟动画
    }
    if (m_GameStatus == 1)  // 游戏中
    {
        SetTimer(hwnd, TIMER_GROUND_MOVE_TIMERID, TIMER_GROUND_MOVE_INTERVAL, nullptr);  // 主逻辑(20ms)
        SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, m_columnInterval, nullptr);              // 生成管子(3000ms)
        SetTimer(hwnd, TIMER_BIRD_CHANGE_TIMERID, TIMER_BIRD_CHANGE_INTERVAL, nullptr);  // 动画帧(100ms)
        SetTimer(hwnd, TIMER_BIRD_POSITION_TIMERID, TIMER_BIRD_POSITION_INTERVAL, nullptr); // 物理(30ms)
    }
    // 状态2（结束）：不启动任何定时器，画面静止
}
```

#### 3.2.5 游戏状态机实现

系统通过 `m_GameStatus` 变量实现三态状态机，在 `On_WM_LBUTTONDOWN` 中处理状态切换：

```cpp
void CBirdAPP::On_WM_LBUTTONDOWN(POINT& po)
{
    if (m_GameStatus == 1)  // 游戏中：点击让小鸟飞
    {
        TryFlap();
    }
    else if (m_GameStatus == 0)  // 开始画面
    {
        // 检测是否点击了商店按钮
        if (po.x >= 20 && po.x <= 160 && po.y >= 650 && po.y <= 700)
        {
            m_shopOpen = !m_shopOpen;
            return;
        }
        // 检测是否点击了商品...
        // 检测是否点击了开始按钮
        if (po.x >= BUTTON_POS_X_1 && po.x <= BUTTON_POS_X_2 &&
            po.y >= BUTTON_POS_Y_1 && po.y <= BUTTON_POS_Y_2)
        {
            m_GameStatus = 1;  // 切换到游戏中
            On_Init();
        }
    }
    else if (m_GameStatus == 2)  // 结束画面
    {
        // 检测是否点击了重新开始按钮
        if (po.x >= BUTTON2_POS_X_1 && po.x <= BUTTON2_POS_X_2 &&
            po.y >= BUTTON2_POS_Y_1 && po.y <= BUTTON2_POS_Y_2)
        {
            m_GameStatus = 0;  // 回到开始画面
            On_Init();
        }
    }
}
```

#### 3.2.6 小鸟物理模拟

小鸟的运动模型基于经典的欧拉积分法：每30ms执行一次重力更新，速度累加重力加速度后更新位置。点击拍翅膀时赋予向上的初始速度（-10），形成抛物线运动轨迹。

```cpp
// CPlayerBird.cpp
void CPlayerBird::MovePlayerBird()
{
    m_speed += m_a;                          // 速度 += 重力加速度(1)
    if (m_speed > BIRD_MAX_DOWN_SPEED)       // 限制最大下落速度(11)
        m_speed = BIRD_MAX_DOWN_SPEED;
    m_y += m_speed;                          // 更新位置
}

// CBirdAPP.cpp - 拍翅膀
void CBirdAPP::TryFlap()
{
    if (m_GameStatus != 1 || m_Paused) return;

    int cooldown = (m_doubleJumpTicks > 0) ? 1 : 4;  // 二段跳冷却更短
    if (m_flapCooldownTicks > 0) return;
    m_flapCooldownTicks = cooldown;

    int speed = (m_doubleJumpTicks > 0) ? -12 : -10; // 二段跳飞得更高
    m_PlayerBird.m_speed = speed;
}
```

#### 3.2.7 碰撞检测算法

管子与小鸟的碰撞检测采用 AABB（轴对齐包围盒）算法，为提高游戏宽容度，小鸟的碰撞区域在垂直方向上下各裁去1/4：

```cpp
// CColumn.cpp
bool CColumn::IfTouchBirdBox(int birdX, int birdY, int birdW, int birdH)
{
    // 水平方向：管子左边缘 <= 小鸟右边缘(3/4处) 且 管子右边缘 >= 小鸟左边缘
    if (m_x <= birdX + birdW * 3/4 && m_x + IMG_COLUMN_WIDTH >= birdX)
    {
        int birdTop = birdY + birdH * 1/4;      // 小鸟碰撞区域顶部（裁去上1/4）
        int birdBottom = birdY + birdH * 3/4;    // 小鸟碰撞区域底部（裁去下1/4）
        int gapTop = m_y + IMG_HALF_COLUMN_HEIGHT;    // 缝隙顶部
        int gapBottom = m_y + IMG_COLUMN_DOWN_Y;       // 缝隙底部

        // 如果小鸟碰撞区域不在缝隙范围内 → 碰撞
        if (gapTop >= birdTop || gapBottom <= birdBottom)
            return true;
    }
    return false;
}
```

#### 3.2.8 计分系统实现

计分系统包含基础分、完美穿越、惊险穿越和Fever四种得分机制：

```cpp
// CBirdAPP.cpp - 在定时器1的处理中
if ((*ite)->m_x + IMG_COLUMN_WIDTH <= m_PlayerBird.m_x && !(*ite)->m_IfPass)
{
    (*ite)->m_IfPass = true;
    m_passStreak++;
    m_score += 1 * GetScoreMultiplier();  // 基础分 +1（Fever时×2）

    // 完美穿越判定
    int gapCenter = (gapTop + gapBottom) / 2;
    int birdCenter = m_PlayerBird.m_y + IMG_BIRD_HEIGHT / 2;
    int diff = abs(birdCenter - gapCenter);
    if (diff <= m_perfectWindow)  // 默认18像素，升级后26像素
    {
        m_perfectStreak++;
        int bonus = 2 * min(m_perfectStreak, 5);  // 最高×5连击
        m_score += bonus * GetScoreMultiplier();
        m_floatMsg = "PERFECT x" + to_string(m_perfectStreak) + " +" + to_string(bonus);
    }

    // 惊险穿越判定
    if (nearTop <= m_nearMissDist || nearBot <= m_nearMissDist)  // 8像素内
    {
        m_score += 1 * GetScoreMultiplier();
        m_floatMsg = "NEAR MISS +1";
    }

    // Fever判定
    if (m_passStreak >= 10) {
        m_passStreak = 0;
        m_feverTicks = 250;  // 约5秒双倍得分
    }

    UpdateDifficulty();  // 难度递增
}
```

#### 3.2.9 道具系统实现

所有道具遵循统一的生命周期模式：`生成 → 移动 → 拾取检测 → 效果应用 → 到期消失`。以护盾为例：

```cpp
// 生成：每8分生成一次，从屏幕右侧出现
void CBirdAPP::SpawnShieldIfNeeded()
{
    if (m_hasShield || m_shieldOnMap) return;
    if (m_score <= 0 || m_score % 8 != 0) return;
    if (m_lastShieldSpawnScore == m_score) return;

    m_lastShieldSpawnScore = m_score;
    m_shieldOnMap = true;
    m_shieldX = WINDOW_WIDTH + 30;
    m_shieldY = 50 + rand() % (IMG_GROUND_POS_Y - 130);
}

// 移动：跟随世界速度向左移动
void CBirdAPP::UpdateShield()
{
    if (!m_shieldOnMap) return;
    m_shieldX -= m_speedStep;
    if (m_shieldX < -40) m_shieldOnMap = false;
}

// 拾取：AABB碰撞检测
bool CBirdAPP::CheckShieldPickup()
{
    if (!m_shieldOnMap) return false;
    int r = 14;
    bool hit = !(m_shieldX+r < birdLeft || m_shieldX-r > birdRight ||
                 m_shieldY+r < birdTop  || m_shieldY-r > birdBottom);
    if (hit) {
        m_shieldOnMap = false;
        m_hasShield = true;
        m_floatMsg = "GET SHIELD";
    }
    return hit;
}
```

#### 3.2.10 商店系统与数据持久化

商店系统通过文件读写实现数据的跨会话保存：

```cpp
// 保存玩家存档
void CBirdAPP::SavePlayerSave()
{
    ofstream out("player_save.txt", ios::trunc);
    out << m_coins << " " << (m_upgradeStartShield ? 1 : 0) << " "
        << m_perfectWindow << " " << m_upgradeDjBonusTicks;
}

// 加载玩家存档
void CBirdAPP::LoadPlayerSave()
{
    ifstream in("player_save.txt");
    int coins, startShield, perfectWindow, djBonus;
    in >> coins >> startShield >> perfectWindow >> djBonus;
    m_coins = coins;
    m_upgradeStartShield = (startShield != 0);
    m_perfectWindow = perfectWindow;
    m_upgradeDjBonusTicks = djBonus;
}

// 购买开局护盾
void CBirdAPP::BuyStartShield()
{
    const int cost = 20;
    if (!m_upgradeStartShield && m_coins >= cost) {
        m_coins -= cost;
        m_upgradeStartShield = true;
        SavePlayerSave();  // 立即保存
        m_shopMsg = "BUY START SHIELD";
    }
    else if (m_upgradeStartShield) {
        m_shopMsg = "ALREADY OWNED";
    }
    else {
        m_shopMsg = "NEED 20 COINS";
    }
}
```

#### 3.2.11 透明贴图渲染技术

EasyX 不直接支持 PNG 的 Alpha 通道透明，本系统采用经典的遮罩图+原图两层绘制法实现透明效果：

```cpp
// CPlayerBird.cpp
void CPlayerBird::ShowPlayerBird(int displayW, int displayH)
{
    // 第一步：遮罩图做 SRCPAINT（或运算）—— 白色区域保留背景，黑色区域清零
    putimage(m_x, m_y, displayW, displayH, &m_imgPlayer[m_mode], 0, 0, SRCPAINT);
    // 第二步：原图做 SRCAND（与运算）—— 彩色区域覆盖，黑色区域透明
    putimage(m_x, m_y, displayW, displayH, &m_imgPlayerFront[m_mode], 0, 0, SRCAND);
}
```

#### 3.2.12 难度递增算法

难度随分数线性增长，每5分提升一个等级：

```cpp
void CBirdAPP::UpdateDifficulty()
{
    int newStep = TIMER_GROUND_MOVE_STEP + (m_score / 5);  // 速度：3+分数/5
    newStep = min(newStep, 10);                              // 上限10

    int newInterval = TIMER_NEW_COLUMN_INTERVAL - (m_score / 5) * 200;  // 间隔：3000-200×(分数/5)
    newInterval = max(newInterval, 1200);                                // 下限1200ms

    m_speedStep = newStep;
    if (newInterval != m_columnInterval) {
        m_columnInterval = newInterval;
        // 动态重建定时器
        KillTimer(hwnd, TIMER_NEW_COLUMN_TIMERID);
        SetTimer(hwnd, TIMER_NEW_COLUMN_TIMERID, m_columnInterval, nullptr);
    }
}
```

---

## 4 系统测试

### 4.1 测试环境

系统测试环境与开发环境保持一致，具体配置如下：

1. 操作系统：Windows 11 桌面环境
2. 开发与运行环境：Visual Studio 2022 + EasyX 图形库
3. 测试数据来源：首次运行自动生成默认存档（`best_score.txt` 和 `player_save.txt`）

### 4.2 测试方法与原则

系统测试采用 **功能测试与流程测试相结合** 的方式，重点验证系统是否能够正确支持 Flappy Bird 游戏的核心玩法流程。测试遵循以下原则：

1. **以状态为主线进行测试**：分别测试开始画面、游戏中、游戏结束三种状态下的功能表现与状态切换正确性；
2. **以玩法闭环为测试目标**：重点测试"飞行控制—管道穿越—计分—道具—死亡—重开"这一完整玩法链；
3. **以数据持久化作为最终判据**：所有存档相关测试均通过界面操作与文件内容变化进行双重验证。

### 4.3 游戏启动与开始画面测试

#### 4.3.1 测试目的

验证系统能否正确启动并显示开始画面，商店界面能否正常打开与关闭。

#### 4.3.2 测试过程

1. 双击 `FlappyBird.exe` 启动游戏；
2. 观察窗口是否正确创建（400×740像素），背景与地面是否正常显示；
3. 观察小鸟是否在开始画面中播放拍翅膀动画；
4. 点击左下角"SHOP"按钮，验证商店界面是否展开显示三项商品；
5. 再次点击"BACK"按钮，验证商店界面是否收起。

#### 4.3.3 测试结果

游戏能够正确启动，窗口尺寸、背景图片、地面滚动均正常。小鸟动画在开始画面中持续播放。商店按钮响应正确，展开/收起逻辑正常。各商品显示当前购买状态（"CLICK TO BUY"或"OWNED"），金币数量正确读取自存档文件。测试结果符合设计预期。

### 4.4 小鸟飞行控制与物理模拟测试

#### 4.4.1 测试目的

验证小鸟的重力下落、拍翅膀上升、抛物线运动轨迹是否符合物理模型预期。

#### 4.4.2 测试过程

1. 点击开始按钮进入游戏状态；
2. 不进行任何操作，观察小鸟是否自然下落；
3. 连续快速点击，观察小鸟是否能保持在屏幕上方飞行；
4. 单次点击后等待，观察小鸟是否呈现先上升后下落的抛物线轨迹；
5. 观察小鸟下落速度是否有上限（不会瞬间掉到地面）。

#### 4.4.3 测试结果

小鸟在不操作时自然加速下落，符合重力模拟预期。点击后小鸟获得向上的初始速度（-10），形成平滑的抛物线运动轨迹。连续点击可以控制小鸟保持飞行高度。下落速度达到上限（11）后不再加速，避免了瞬间坠落的问题。物理模型表现真实自然，测试结果符合设计预期。

### 4.5 管道穿越与碰撞检测测试

#### 4.5.1 测试目的

验证管道的生成、移动、碰撞检测以及固定管道与移动管道的区别是否正确。

#### 4.5.2 测试过程

1. 进入游戏，等待管道从屏幕右侧生成并向左移动；
2. 控制小鸟成功穿越管道缝隙，验证是否正确计分；
3. 故意让小鸟撞到管道上方或下方的实心部分，验证是否触发游戏结束；
4. 让小鸟掉落到地面，验证是否触发游戏结束；
5. 观察是否有管道出现上下移动的现象（约25%概率）；
6. 观察管道移出屏幕左侧后是否被正确回收（不出现内存持续增长）。

#### 4.5.3 测试结果

管道从屏幕右侧定时生成，垂直位置随机，移动速度正确。碰撞检测准确：小鸟碰到管子实心部分或掉到地面均正确触发游戏结束。碰撞区域的1/4裁切设计使游戏体验更加宽容。约25%的管子呈现上下振荡移动，增加了游戏难度。管子移出屏幕后被正确删除，无内存泄漏。测试结果符合设计预期。

### 4.6 计分系统测试

#### 4.6.1 测试目的

验证基础计分、完美穿越加分、惊险穿越加分以及Fever双倍得分是否正确。

#### 4.6.2 测试过程

1. 进入游戏，穿越第一根管子，观察分数是否变为1；
2. 尝试从管子缝隙正中心穿过，观察是否出现"PERFECT x1 +2"提示并获得额外加分；
3. 连续多次从中心穿过，观察连击数是否递增（PERFECT x2、x3...），加分是否递增（+4、+6...）；
4. 尝试从缝隙边缘极近距离穿过，观察是否出现"NEAR MISS +1"提示；
5. 连续穿越10根管子不死亡，观察是否出现"FEVER!"提示并进入双倍得分模式；
6. 观察右上角状态栏是否正确显示当前速度、管子间隔等难度参数。

#### 4.6.3 测试结果

基础计分正确（每穿越一根管子+1）。完美穿越判定准确，连击奖励梯度正确（+2、+4、+6、+8、+10）。惊险穿越在8像素距离内正确触发+1分。Fever系统在连续10次穿越后正确激活，持续约5秒，期间所有得分翻倍。状态栏信息实时更新，测试结果符合设计预期。

### 4.7 道具系统测试

#### 4.7.1 测试目的

验证护盾、金币、二段跳、随机道具的生成、拾取与效果是否正确。

#### 4.7.2 测试过程

1. 游戏中分数达到3的倍数时，观察是否有金币（黄色圆圈"C"）从右侧出现；
2. 分数达到8的倍数时，观察是否有护盾（金色圆圈"S"）出现；
3. 分数达到10的倍数时，观察是否有二段跳（蓝色圆圈"D"）出现；
4. 分数达到7的倍数时，观察是否有随机道具（灰色圆圈，标记G/K/T/X之一）出现；
5. 控制小鸟接触各道具，验证拾取后是否出现对应提示文字；
6. 拾取护盾后撞到管子，验证是否消耗护盾而非游戏结束；
7. 拾取二段跳后，验证拍翅膀频率和跳跃高度是否增强；
8. 拾取Ghost道具后，验证穿越管子和地面是否不受碰撞影响；
9. 拾取Shrink道具后，验证小鸟碰撞体积是否缩小；
10. 拾取SlowMo道具后，验证世界移动速度是否减半。

#### 4.7.3 测试结果

各道具按设计的分数周期正确生成，外观标识清晰（颜色+字母）。拾取检测准确，效果应用正确：护盾消耗后抵挡一次碰撞、二段跳增强拍翅膀能力、Ghost实现无敌穿越、Shrink缩小碰撞体积、SlowMo减缓世界速度。各道具效果有时间限制，到期后正确消失。测试结果符合设计预期。

### 4.8 商店系统与数据持久化测试

#### 4.8.1 测试目的

验证商店购买逻辑、金币扣除、升级效果以及存档文件的正确性。

#### 4.8.2 测试过程

1. 在开始画面打开商店，确认当前金币数量显示正确；
2. 点击"Start Shield"商品（20金币），验证购买成功后显示"OWNED"；
3. 关闭游戏，重新打开，验证购买状态是否持久化；
4. 购买"Perfect+"升级（15金币）后进入游戏，验证完美穿越窗口是否加大；
5. 购买"DJ+"升级（25金币）后进入游戏，验证二段跳持续时间是否延长；
6. 在游戏中按E键购买二段跳（5金币），验证金币扣除与效果生效；
7. 打开 `player_save.txt` 文件，验证内容是否与游戏状态一致；
8. 打开 `best_score.txt` 文件，验证最高分是否正确记录。

#### 4.8.3 测试结果

商店界面响应正确，购买条件校验严格（金币不足提示"NEED X COINS"，已拥有提示"ALREADY OWNED"）。购买后金币正确扣除，升级效果正确生效。关闭游戏重新打开后，存档文件正确加载，金币数量、升级状态、最高分均与上次退出时一致。文件内容格式正确，测试结果符合设计预期。

### 4.9 完整游戏流程联通测试

#### 4.9.1 测试目的

以"启动→开始画面→商店购买→游戏中飞行→收集道具→穿越管道→触发Fever→难度递增→碰撞死亡→查看最高分→重新开始"为主线，验证系统在完整游戏流程中的稳定性与数据一致性。

#### 4.9.2 测试过程

1. 首次启动游戏，确认默认状态（0金币，0最高分）；
2. 开始游戏，飞行并穿越5根管道，确认分数为5，难度参数开始变化；
3. 收集途中的金币和护盾，确认状态栏正确显示"SHIELD"标识；
4. 继续飞行至分数15+，确认管子速度明显加快、生成间隔缩短；
5. 连续穿越10根管子触发Fever，确认双倍得分生效；
6. 故意撞到管子，确认护盾消耗而非直接死亡；
7. 再次碰撞（无护盾），确认游戏结束并显示结束画面；
8. 确认最高分正确更新；
9. 点击重新开始，确认回到开始画面，金币已保存；
10. 用积累的金币在商店购买升级，确认存档文件更新。

#### 4.9.3 测试结果

完整游戏流程运行顺畅，各功能模块衔接正确。状态切换（开始→游戏→结束→开始）无异常，定时器启停正确。计分、道具、难度递增、Fever等系统协同工作正常。数据持久化在每次关键操作后正确保存，重新启动游戏后状态完整恢复。未出现崩溃、内存泄漏或数据不一致问题。测试结果符合设计预期。

### 4.10 测试结果分析与总结

通过上述测试，系统各功能模块均能够按照设计要求正常运行。系统在游戏状态管理、物理模拟、碰撞检测、计分系统、道具系统、商店系统及数据持久化等方面形成了完整、稳定的游戏闭环。测试结果表明，系统功能实现正确，数据流转一致，能够满足 Flappy Bird EasyX 游戏的设计目标。

---

## 5 总结与体会

通过本次综合实训项目的设计与实现，小组完成了一套基于 C++ 与 EasyX 图形库的 Flappy Bird 游戏系统。系统围绕"小鸟飞行—管道穿越—道具收集—商店升级"的核心玩法流程展开，采用面向对象方法进行游戏建模，并通过框架与实现分离的架构设计，实现了较为完整的游戏功能闭环。

### 5.1 系统开发过程中遇到的问题及解决方法

在系统开发过程中，小组也遇到了一些具有代表性的问题，并在不断调试与改进中逐步解决。

**（1）游戏循环方式的选择问题**

在早期实现阶段，小组尝试使用 EasyX 自带的 `getmessage()` 函数进行消息循环，但发现这种方式无法正确接收 Windows 定时器消息（`WM_TIMER`），导致游戏无法实现定时更新。针对这一问题，小组将架构重构为 Win32 消息驱动模式，通过 `SetWindowLongPtr` 注册自定义回调函数 `Wndproc`，直接处理 Windows 消息。这一改动虽然增加了代码复杂度，但彻底解决了定时器驱动问题，使游戏能够通过多个定时器分别控制物理更新、动画切换和管子生成。

**（2）管子内存管理问题**

在游戏中，管子不断地被创建和销毁。初期实现时，管子从链表中移除后未正确释放内存，导致长时间游戏后内存持续增长。针对这一问题，小组在 `CColumnBox::MoveAllColumn()` 中加入了 `delete` 操作，在管子移出屏幕时同时释放内存并从链表中移除指针，确保了内存的正确回收。同时在 `ClearAll()` 方法中统一清理所有管子，用于游戏重新开始时的资源重置。

**（3）碰撞检测的精度与宽容度平衡问题**

初期的碰撞检测使用小鸟图片的完整矩形区域，导致玩家经常在"看起来没碰到"的情况下被判为碰撞，游戏体验较差。针对这一问题，小组将小鸟的碰撞区域在垂直方向上下各裁去1/4，使碰撞判定更加宽容。这一调整在不改变视觉效果的前提下显著提升了游戏的可玩性，体现了游戏设计中"对玩家宽容"的原则。

**（4）透明贴图的实现问题**

EasyX 不直接支持 PNG 图片的 Alpha 通道透明渲染。初期直接绘制带白色背景的小鸟图片，导致白色方块覆盖游戏画面。针对这一问题，小组采用了经典的遮罩图+原图两层绘制法：先用遮罩图（黑白）做 SRCPAINT 运算清除背景区域，再用原图做 SRCAND 运算填充小鸟颜色，最终实现了透明效果。虽然需要为每张图片准备两个版本，增加了一倍的美术资源量，但渲染效果清晰无瑕。

**（5）难度递增与定时器动态调整问题**

难度递增需要动态修改管子生成定时器的间隔，但 `SetTimer` 在定时器已存在时不会自动更新间隔。针对这一问题，小组在 `UpdateDifficulty()` 中先 `KillTimer` 销毁旧定时器，再用新的间隔 `SetTimer` 创建新定时器，确保了间隔参数的实时生效。

### 5.2 系统的优点分析

通过本次实训系统的设计与实现，小组总结出系统具有以下几个方面的优点：

首先，**架构设计合理**。系统采用框架层与实现层分离的架构，`CGameFrame` 抽象基类封装了通用的游戏框架机制（窗口管理、消息路由、双缓冲绘制），`CBirdAPP` 具体实现类专注于 Flappy Bird 的玩法逻辑。这种设计使得框架可以在不修改代码的情况下复用于其他游戏项目，体现了面向对象设计中开闭原则的思想。

其次，**面向对象特性运用充分**。系统通过继承实现框架复用（`CBirdAPP` 继承 `CGameFrame`），通过组合实现模块化（`CBirdAPP` 持有六个组件对象），通过多态实现消息分发（虚函数覆盖），通过封装实现各组件的独立性（每个类只管理自己的数据和行为）。这些面向对象特性的综合运用，使系统结构清晰、职责分明。

再次，**游戏玩法丰富**。在经典 Flappy Bird 基础上，系统增加了完美穿越、惊险穿越、Fever系统、四种随机道具、商店永久升级、难度递增等创新机制，显著提升了游戏的可玩性和深度。这些机制之间相互配合（如完美连击+Fever+2X道具的得分叠加），形成了丰富的策略空间。

此外，**数据持久化设计实用**。通过简洁的文本文件格式存储最高分和玩家存档，实现了游戏进度的跨会话保存。商店系统的引入为玩家提供了长期目标感，增强了游戏的留存价值。

### 5.3 系统的不足与改进方向

尽管系统已基本实现设计目标，但仍存在一些不足之处，有待在后续工作中进一步完善。

首先，**音效系统缺失**。当前游戏没有任何声音效果，玩家无法通过听觉获得操作反馈（如拍翅膀声、碰撞声、得分声）。后续可引入 `mciSendString` 或 FMOD 等音频库，为游戏添加丰富的音效与背景音乐。

其次，**图形渲染方式较为原始**。当前采用的遮罩图+原图两层绘制法虽然有效，但需要为每张图片准备两个版本，维护成本较高。后续可迁移到支持 Alpha 通道的渲染方案（如使用 `AlphaBlend` 函数或切换到 SFML/SDL 等现代图形库），简化美术资源管理。

再次，**排行榜与社交功能缺失**。当前游戏仅记录本地最高分，缺乏多人排行榜和成绩分享功能。后续可引入文件或数据库存储多人成绩，并实现排行榜界面。

此外，**游戏平衡性仍有优化空间**。难度递增曲线、道具生成概率、Fever触发条件等参数目前基于经验值设定，缺乏系统的平衡性测试。后续可通过收集玩家游戏数据，对这些参数进行数据驱动的调优。

### 5.4 个人收获与体会

通过本次实训项目，小组成员在以下几个方面获得了显著的成长：

**面向对象设计能力的提升**：从最初将所有代码写在一个文件中的"大杂烩"式设计，到逐步将系统拆分为框架层、主控层、组件层的三层架构，小组深刻体会到了面向对象设计中封装、继承、多态、抽象四大特性的实际价值。特别是"组合优于继承"原则的应用——`CBirdAPP` 通过组合持有各组件对象而非继承自它们——使系统结构更加灵活。

**问题定位与调试能力的提升**：游戏开发中的问题往往表现为视觉异常（如画面闪烁、碰撞误判、动画卡顿），需要从底层原理出发定位原因。通过反复调试，小组学会了从现象追溯到根本原因的系统化调试方法，例如通过打印小鸟坐标和管子坐标来排查碰撞检测问题，通过检查定时器启停状态来排查状态切换问题。

**工程思维的培养**：认识到编程不只是实现功能，还要考虑可维护性、可扩展性和用户体验。良好的代码组织（一个类一个文件夹）、清晰的命名规范（`m_` 前缀表示成员变量）、合理的注释密度，这些工程实践能够大大降低后续维护成本。

**游戏设计思维的启发**：通过实现计分系统、道具系统、难度曲线等游戏机制，小组体会到了游戏设计中"风险与回报"的平衡哲学——惊险穿越奖励鼓励大胆操作，Fever系统奖励持续稳定表现，商店系统提供长期目标感。这些设计思维不仅适用于游戏开发，对其他类型的软件设计也有启发意义。

### 参考文献：

[1] Bjarne Stroustrup. The C++ Programming Language (4th Edition). Addison-Wesley, 2013.

[2] EasyX 图形库官方文档 [EB/OL]. https://easyx.cn/

[3] Microsoft. Win32 API 参考手册 [EB/OL].

[4] Game Programming Patterns. Robert Nystrom, 2014.

---

| 学号 | 姓名 | 具体任务（分工） |
|------|------|------------------|
| xxx  | xxx  | 游戏框架搭建、核心游戏逻辑实现、报告撰写 |
| xxx  | xxx  | 组件类实现、道具系统开发、报告撰写 |
| xxx  | xxx  | 美术资源处理、商店系统实现、测试验证、报告撰写 |
