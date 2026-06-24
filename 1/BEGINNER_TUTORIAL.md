# Flappy-Bird-EasyX 新手教程（从 C 基础到读懂完整项目）

面向读者：第一次做项目、会 C 的指针/数组/链表等基础，刚接触 C++ 和面向对象。  
目标：用这个 Flappy-Bird-EasyX 项目学会“看懂代码 → 能改一点 → 能 Debug → 能加小功能”。

---

## 0. 先说一句话：这个项目在做什么？

这个项目用 EasyX 在 Windows 上画一个 Flappy Bird。核心循环是：

- **开局界面（Start）**：展示开始图，点开始进入游戏
- **游戏进行中（Playing）**：小鸟下落，点一下往上跳；柱子不断从右往左移动；撞到柱子/地面就失败；越过柱子加分
- **结束界面（GameOver）**：展示结束图，点“重开”回到开局

现在项目还额外支持一些“更像游戏”的功能（你可以当作学习目标）：

- 空格/↑：跳跃
- P：暂停/继续（仅游戏中）
- R：随时回到开局（重置）
- 难度会随分数变高（地面/柱子移动更快，柱子生成更频繁）
- 会保存最高分到 `best_score.txt`
- 25% 概率出现“会上下摆动的柱子”（更考验反应）
- “护盾道具 S”：吃到后可抵挡 1 次撞柱/触地
- “完美穿越 PERFECT +2”：穿过缺口中心会额外加分
- “金币 C”：吃到金币会累积（不随死亡清空）；游戏中按 E 花 5 金币购买一段时间的“双跳强化”
- “双跳道具 D”：吃到后直接获得一段时间“双跳强化”（跳得更有力、连点更快）
- “连击加成”：连续触发 PERFECT 会越来越赚（PERFECT x2/x3…）
- “开始界面商店”：按 1/2/3 用金币购买永久升级（重开后仍保留）
- “Fever/狂热”：连续无失误过 10 个管道触发 FEVER（约 5 秒），期间分数 x2
- “Near-miss/擦边”：过管时如果贴近上/下边缘，会额外 +1 分并更快涨 STREAK 条
- “道具池”：随机刷出 G/K/T/X 道具
  - G = Ghost：短时间无视撞柱/触地
  - K = Shrink：短时间变小（更不容易撞）
  - T = SlowMo：短时间世界变慢
  - X = 2x：短时间分数 x2

两个你最该立刻体验的玩法：

- 连击：连续 PERFECT 时会显示 `PERFECT xN +B`，连得越多奖励越高
- 商店：在开始界面点击左下角 `SHOP` 打开面板，然后用鼠标点击购买（也支持按 1/2/3）；金币会保存到 `player_save.txt`

项目中最关键的文件只有 3 个：

- 游戏主控：`CBirdAPP`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp)）
- 事件/窗口框架：`CGameFrame + Wndproc`（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h)、[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp)）
- 参数配置：`GameConfig.h`（[GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h)）

你只要能读懂这三个，就能“抓住整个项目”。

---

## 1. 你要先认识的 6 个概念（全是从 C 延伸来的）

### 1) C++ 的 class 就是“把数据 + 函数打包”

比如 `CPlayerBird`：

- 数据（成员变量）：位置、速度、图片数组（[CPlayerBird.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.h#L6-L14)）
- 函数（成员函数）：初始化/移动/绘制（[CPlayerBird.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.h#L16-L21)）

你可以把它当成 C 里常见的写法：

- `struct Bird { ... }`
- `void Bird_Init(struct Bird* b)`, `void Bird_Move(struct Bird* b)`…

只不过 C++ 把这些“粘在一起”，写成 `b.InitPlayerBird()`。

### 2) 成员访问 `.` 和 `->`

你会看到两种：

- `obj.field`：对象本身（栈上/成员变量）  
  例：`m_PlayerBird.ShowPlayerBird();`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L75-L76)）
- `ptr->field`：指针指向的对象（堆上 `new` 出来的）  
  例：`(*ite)->IfTouchBird(&m_PlayerBird)`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L144-L148)）

### 3) `new/delete` 就像 `malloc/free`（但一定要配对）

项目里生成柱子是这样：

- `CColumn* newCol = new CColumn;`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L166)）
- 用完要 `delete`（在 `CColumnBox::MoveAllColumn` 出屏时删）（[CColumnBox.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.cpp#L47-L53)）

你会在 Debug 部分学到：这里有“新手最容易踩的内存泄漏坑”。

### 4) `std::list` 是 C++ 的链表（你会的链表概念能直接用）

`CColumnBox` 里用：

- `list<CColumn*> m_ColLst;`（[CColumnBox.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.h#L6-L10)）

这相当于你 C 里写的“链表节点指针”，只是 C++ 帮你维护了指针链接。

### 5) “定时器”就是“每隔一段时间自动调用一次更新”

项目用了 Windows `SetTimer`：

- 在不同状态设置不同 timer（现在逻辑集中在 `StartTimersForStatus()`，由 `On_SetTimer()` 调用）
- 定时器消息来了以后，走 `On_WM_TIMER`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L130-L201)）

你可以把它理解为：

- 每 20ms 调一次“地面和柱子移动”
- 每 3000ms 调一次“生成新柱子”
- 每 30ms 调一次“小鸟下落”

这些参数都在 [GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L25-L44)。

### 6) “状态机”就是 `m_GameStatus`

`m_GameStatus` 是一个数字：

- `0`：开始界面
- `1`：游戏进行中
- `2`：结束界面

它控制：

- 画什么（[On_Paint](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L67-L93)）
- 设哪些 timer（[On_SetTimer](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L22-L42)）
- 点哪里会发生什么（[On_WM_LBUTTONDOWN](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L108-L127)）

如果你只记住一句话：**状态决定“现在运行哪套逻辑”**。

---

## 2. 从 0 开始读懂项目：推荐阅读顺序（别乱翻）

按这个顺序读，最省脑子：

1. `GameFrame/main.cpp`：程序入口 + 消息分发（[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp#L60-L76)）
2. `GameFrame/frame.h`：游戏框架 `CGameFrame`（重点看 `InitGame` 和 `PaintGame`）（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L92-L124)）
3. `BirdAPP/CBirdAPP.cpp`：游戏主逻辑（重点看 `On_Init / On_Paint / On_WM_TIMER / On_WM_LBUTTONDOWN`）
4. `GameConfig/GameConfig.h`：所有参数（timer、图片尺寸、按钮范围）
5. 业务对象：
   - `CPlayerBird`：小鸟怎么画、怎么下落（[CPlayerBird.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.cpp#L47-L64)）
   - `CColumn/CColumnBox`：柱子怎么生成、移动、碰撞（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L17-L49)）
   - `CGround`：地面滚动、触地（[CGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.cpp#L26-L43)）
   - `CBeforeGame/CAfterGame`：开始/结束画面（[BeforeGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BeforeGame/BeforeGame.cpp#L3-L16)、[CAfterGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/AfterGame/CAfterGame.cpp#L3-L15)）

---

## 3. 你必须读懂的 3 个函数（抓主线）

### 3.1 `CBirdAPP::On_Init()`：初始化（根据状态初始化不同东西）

看这里：([CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L45-L64))

- 当 `m_GameStatus==0`：
  - 分数清零
  - 清柱子对象（现在用 `m_ColumnBox.ClearAll()`）
  - 初始化背景/地面/小鸟/开始界面/结束界面资源
  - 设定 timer
- 当 `m_GameStatus==1`：
  - 更新难度（分数越高越快）
  - 设定 timer

新手建议：先别纠结“这里写得是否完美”，你先理解“它在做初始化”。

### 3.2 `CBirdAPP::On_Paint()`：渲染（根据状态画不同东西）

看这里：([CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L67-L93))

三段 if 对应三种状态：

- 状态 1（游戏中）：背景→柱子→地面→分数→鸟
- 状态 0（开始）：背景→地面→开始图→鸟
- 状态 2（结束）：背景→柱子→地面→鸟→结束图→分数

顺序很重要：后画的会盖住先画的。

### 3.3 `CBirdAPP::On_WM_TIMER()`：更新逻辑（移动/生成/碰撞/计分）

看这里：([CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L130-L201))

你只需要先看 3 个 case：

- `TIMER_GROUND_MOVE_TIMERID`：每 20ms 移动地面和柱子 + 做柱子碰撞/计分
- `TIMER_NEW_COLUMN_TIMERID`：每 3000ms new 一个柱子并加入链表
- `TIMER_BIRD_POSITION_TIMERID`：每 30ms 让鸟下落 + 检测触地

如果你能把这 3 个 case 用自己的话讲出来，你就已经读懂 60% 了。

---

## 4. 新手练习（从“能跑”到“能改”）

每个练习都很小，你做完会越来越自信。

### 练习 1：改游戏速度（最推荐）

打开 [GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L26-L44)：

- 想让柱子/地面更快：改 `TIMER_GROUND_MOVE_STEP`（默认 3）
- 想让柱子生成更频繁：改 `TIMER_NEW_COLUMN_INTERVAL`（默认 3000ms）
- 想让鸟下落更慢：改 `BIRD_A`（默认 1）或 `BIRD_MAX_DOWN_SPEED`

### 练习 2：让分数颜色变明显

看 [ShowScore](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L215-L227)：

- 改 `settextcolor(RGB(...))`
- 改字体大小 `settextstyle(60, 0, ...)`

### 练习 3：让点击上跳更“有手感”

点击时现在是：

- `m_PlayerBird.m_speed = BIRD_STARTSPEED;`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L111-L114)）

你可以试试：

- 把 `BIRD_STARTSPEED` 从 -10 改成 -12 或 -8

### 练习 4：给 GameOver 增加“按任意键重开”

思路：

- `frame.h` 已经映射了 `WM_KEYDOWN`（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L60-L68)）
- 在 `CBirdAPP` 里重写 `On_WM_KEYDOWN`，当 `m_GameStatus==2` 时把状态设回 0 并 `On_Init()`

现在项目已经实现了键盘逻辑（[CBirdAPP::On_WM_KEYDOWN](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L240-L294)）：

- 空格/↑：跳
- P：暂停/继续
- R：回到开局

你可以基于它继续扩展，比如加 `ESC` 退出、加 `S` 静音等。

---

## 5. Debug 新手路线：只学 3 招就够用

### 招 1：断点从“主控入口”打起

最有用的断点位置：

- [On_WM_TIMER](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L130) 看更新是不是在跑
- [On_WM_LBUTTONDOWN](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L108) 看点击是不是进来了
- [CColumn::IfTouchBird](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L37) 看碰撞为什么判定

### 招 2：观察变量（你需要看的变量清单）

碰撞/运动常用：

- 鸟：`m_PlayerBird.m_x / m_y / m_speed / m_mode`
- 柱子：`(*ite)->m_x / m_y / m_IfPass`
- 地面：`m_Ground.m_y`
- 状态：`m_GameStatus`
- 分数：`m_score`

### 招 3：遇到“看不见的问题”就画出来

比如碰撞不准：你可以临时用 `rectangle()` 把碰撞盒画出来（调试用，不要长期保留）。

---

## 6. 新手最容易踩的坑：这个项目里就有一个（内存泄漏）

如果你用 C 写过链表，你一定听过一句话：**new 了就要 delete**。

这个项目里柱子是 `new` 出来的（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L172-L178)），而且存进了 `list<CColumn*>`（[CColumnBox.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.h#L6-L10)）。

以前如果直接 `m_ColLst.clear()`，会只清掉链表节点，不会 delete 指针指向的对象，容易泄漏。  
现在项目已经加了 `CColumnBox::ClearAll()` 来统一删除并清空（[CColumnBox.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.cpp#L60-L74)）。

你要学会的规则是：  
**“谁拥有对象（容器/管理者），谁负责统一释放。”**

---

## 7. 下一步你该学什么？（按这条路线就能进步很快）

1. 把 `m_GameStatus` 的 0/1/2 三套逻辑完全讲明白（状态机）
2. 把 `On_WM_TIMER` 的三个关键 case 讲明白（更新循环）
3. 把 `new CColumn` 到 `delete` 的生命周期讲明白（内存与容器）
4. 做一个小扩展：例如“按键重开”或“加一个暂停状态”

---

## 8. 给你的建议（你是小白也完全可以学会）

- 不要试图一次性看懂所有文件，先抓主线：`CBirdAPP` + `GameConfig` + `On_WM_TIMER`
- 每天只做一个小改动：改速度、改按钮热区、改分数显示
- 每次改完就用断点确认：你改的那行代码确实执行到了

如果你愿意，我可以按你的节奏带你做第一次“加功能”：  
你更想先做哪个？

- A. “按空格也能跳”
- B. “按 R 直接重开”
- C. “加一个暂停（P）”
- D. “把柱子生成速度做成逐渐变难”
