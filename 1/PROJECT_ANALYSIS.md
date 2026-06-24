# Flappy-Bird-EasyX 项目深入解析（学习笔记）

下面用“读这个项目的人”而不是“写这个项目的人”的视角，把它拆成可理解、可扩展、可协作的结构，并穿插你真正会用到的 Debug 路径与团队落地方式。

---

## 1) 项目结构与模块划分（从 README + 类图 + 代码落点）

### 整体分层（建议你用这张心智图记）

- **框架层（Engine / Framework）**：负责窗口、消息分发、绘制节拍  
  - 核心：`CGameFrame`（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L16-L187)）+ `Wndproc`（[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp#L9-L48)）
- **应用层（Game App / Orchestrator）**：负责“把一堆对象组织成游戏”  
  - 核心：`CBirdAPP`（[CBirdAPP.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.h#L11-L37)、[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L45-L201)）
- **业务对象层（Game Objects）**：背景/地面/小鸟/柱子/开始界面/结束界面等  
  - `CBackGround`（[CBackGround.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/BackGround/CBackGround.h#L4-L18)、[CBackGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BackGround/CBackGround.cpp#L14-L24)）
  - `CGround`（[CGround.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.h#L6-L20)、[CGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.cpp#L14-L43)）
  - `CPlayerBird`（[CPlayerBird.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.h#L4-L21)、[CPlayerBird.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.cpp#L19-L64)）
  - `CColumn` / `CColumnBox`（[CColumn.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.h#L5-L20)、[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L17-L49)、[CColumnBox.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.h#L6-L16)、[CColumnBox.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.cpp#L26-L57)）
  - `CBeforeGame` / `CAfterGame`（[BeforeGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BeforeGame/BeforeGame.cpp#L3-L16)、[CAfterGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/AfterGame/CAfterGame.cpp#L3-L15)）
- **配置层（Config）**：窗口尺寸、资源尺寸、计时器参数、按钮热区  
  - 核心：宏定义集中在 [GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L4-L62)
- **资源层（Assets）**：`res/*.png`、`picture/*` 等，典型是“底图 + Front遮罩”成对（`SRCPAINT + SRCAND`）

### 每个核心模块职责是什么？

#### (A) `CGameFrame`：把 Win32/EasyX 的“消息驱动”包装成“可重写回调”

- `InitMsgMap()` 把 `WM_LBUTTONDOWN / WM_TIMER / WM_CLOSE ...` 映射到成员函数（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L47-L68)）
- `Wndproc` 做“根据消息类型分发到 CGameFrame 子类的 On_*” （[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp#L12-L38)）
- 你可以把它理解为一个极简“游戏框架骨架”：把平台复杂性隔离掉，让你只管实现 `On_Init / On_Paint / On_WM_TIMER / On_WM_LBUTTONDOWN`。

#### (B) `CBirdAPP`：游戏总导演（Orchestrator / Composition Root）

- 组合持有几乎所有对象（[CBirdAPP.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.h#L19-L28)）
- 维护全局状态：`m_GameStatus`（0开始前 / 1进行中 / 2结束）和 `m_score`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L11-L15)）
- 负责三件大事：
  - 初始化（按状态初始化对象+计时器）：[On_Init](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L45-L64)、[On_SetTimer](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L22-L42)  
  - 绘制编排（按状态调用各对象 `Show*()`）：[On_Paint](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L67-L93)  
  - Tick 更新 + 业务规则（移动、生成柱子、碰撞、计分、触地失败）：[On_WM_TIMER](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L130-L201)、[ShowScore](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L215-L227)

#### (C) `CPlayerBird`：只管“鸟自己”

- 数据：位置/速度/加速度/动画帧索引、资源数组（[CPlayerBird.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.h#L7-L14)）
- 行为：
  - `InitPlayerBird()`：加载 0~7 与 0Front~7Front（[CPlayerBird.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.cpp#L19-L45)）
  - `MovePlayerBird()`：重力模型（速度 + 加速度并封顶）（[CPlayerBird.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.cpp#L53-L64)）
  - `ShowPlayerBird()`：两次 `putimage` 做透明合成（[CPlayerBird.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.cpp#L47-L51)）

#### (D) `CColumn` / `CColumnBox`：障碍系统

- `CColumn`：单根柱子的资源、位置、是否已计分、碰撞判定（[CColumn.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.h#L8-L19)）
  - `InitColumn()`：柱子从屏幕右侧出生，y 用 `CBirdAPP::m_rand` 的随机数偏移（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L17-L24)）
  - `IfTouchBird()`：基于矩形近似 + 上下通道判定（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L37-L49)）
- `CColumnBox`：管理多根柱子的容器，负责批量 `Show/Move` 和出屏清理（[CColumnBox.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.cpp#L39-L56)）

#### (E) `CGround` / `CBackGround`：场景系统

- `CBackGround`：只负责加载+绘制背景（[CBackGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BackGround/CBackGround.cpp#L14-L24)）
- `CGround`：
  - `MoveGround(step)`：x 循环滚动（[CGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.cpp#L26-L34)）
  - `IfTouchGround(bird)`：判定触地失败（[CGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.cpp#L36-L43)）

#### (F) `CBeforeGame` / `CAfterGame`：UI 状态壳（Start/Over Screen）

- 两者都是“加载两张图，然后 SRCPAINT + SRCAND 合成渲染”  
  - Start：[BeforeGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BeforeGame/BeforeGame.cpp#L3-L16)  
  - GameOver：[CAfterGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/AfterGame/CAfterGame.cpp#L3-L15)

### 模块之间如何交互？`CBirdAPP` 怎么协调大家？

把 `CBirdAPP` 的每个回调当成一次“导演口令”：

1. **初始化阶段**
   - 点击开始后 `m_GameStatus=1`，重新 `On_Init()`（[On_WM_LBUTTONDOWN](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L108-L127)）
   - `On_Init()` 按状态初始化不同对象（[On_Init](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L45-L64)）
2. **游戏循环阶段（由计时器驱动）**
   - `WM_TIMER` → `On_WM_TIMER`（[On_WM_TIMER](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L130-L201)）
   - 在 `TIMER_GROUND_MOVE_TIMERID` 分支里：
     - 地面滚动：`m_Ground.MoveGround()`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L134-L138)）
     - 柱子整体移动：`m_ColumnBox.MoveAllColumn()`（同上）
     - 遍历柱子：碰撞则进入 GameOver，越过则计分（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L139-L156)）
3. **渲染阶段**
   - 按状态调用不同的 `Show*()` 顺序（[On_Paint](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L67-L93)）  
     这是典型的“渲染编排”：背景 → 障碍 → 地面 → UI/分数 → 鸟（顺序会影响遮挡关系）。

---

## 2) 面向对象封装思想（真实现状 + 你应该学到的原则）

结论：这个项目的类划分是 OOP 的，但封装强度偏弱（很多成员 `public`）。这对学习很友好：你能快速看懂数据流；但对长期维护/多人协作不够稳。

### 每个类封装了什么？对外暴露了什么接口？

你会看到一种非常一致的“对象接口套路”：

- `InitXxx()`：加载资源、初始化位置
- `ShowXxx()`：绘制
- `MoveXxx(step)`：更新位置（可选）
- `IfTouchXxx(other)`：碰撞/判定（可选）

例子：

- `CPlayerBird` 暴露 `Init/Move/Show`（[CPlayerBird.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.h#L16-L21)）
- `CGround` 暴露 `Init/Move/Show/IfTouchGround`（[CGround.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.h#L13-L20)）
- `CColumn` 暴露 `Init/Move/Show/IfTouchBird`（[CColumn.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.h#L14-L20)）
- `CBirdAPP` 暴露“框架回调”接口（[CBirdAPP.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.h#L29-L36)）

### 这种封装方式的好处

- 职责清晰：每个类都围绕一类对象的资源+行为。
- 可替换性：比如你想换 `Ground` 的滚动策略，理论上只改 `CGround`。
- 可编排：`CBirdAPP` 像导演一样串这些接口（`Init/Move/Show/IfTouch`）。

### 你也要看到它的“封装短板”

- 成员基本都 public：比如 `CPlayerBird::m_speed` 被 `CBirdAPP` 直接写（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L111-L114)）
- 跨模块强耦合：`CColumn` 直接 include `CBirdAPP.h` 只为了用 `CBirdAPP::m_rand`（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L1-L4)）

### 扩展新功能示例：道具系统

用现有风格最稳的设计是“照抄障碍系统的形状”：

- 新增：
  - `CProp`：单个道具（`Init/Show/Move/IfTouchBird`）
  - `CPropBox`：道具容器（`ShowAll/MoveAll/cleanup`）
- `CBirdAPP`：
  - 持有 `CPropBox m_PropBox;`
  - 在 `On_Init()` 里初始化道具系统（只在 playing 状态）
  - 在 `On_WM_TIMER()` 加 `TIMER_NEW_PROP_TIMERID` 生成道具（类似 [new CColumn](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L164-L169)）
  - 在移动分支里移动道具并检测拾取

道具效果建议不要写死在 `CBirdAPP`：让拾取返回“效果事件”，由 `CBirdAPP` 统一应用到 bird/score/timer 上。

---

## 3) 项目分工与合作（3-5 人团队模拟）

### 推荐分工（按边界清晰、低冲突）

- A：框架/循环负责人（Engine）  
  负责 `CGameFrame` + `Wndproc`（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L47-L68)、[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp#L12-L46)）
- B：玩法负责人（Gameplay / Orchestrator）  
  负责 `CBirdAPP` 状态机、计时器、计分、切换（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L22-L201)）
- C：角色与物理（Bird）  
  负责 `CPlayerBird`（[CPlayerBird.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/PlayerBird/CPlayerBird.cpp#L19-L64)）
- D：障碍与碰撞（Columns）  
  负责 `CColumn/CColumnBox`（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L17-L49)、[CColumnBox.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.cpp#L39-L56)）
- E：UI 与资源（Start/Over + Assets）  
  负责 `CBeforeGame/CAfterGame`、资源替换规范、按钮热区与配置（[GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L54-L62)）

### “接口契约”与协作保障

- 对象生命周期契约：先 `Init*()` 再 `Show*()/Move*()`
- 坐标系契约：同一屏幕坐标，尺寸来自 [GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L9-L24)
- 碰撞契约：`IfTouchBird(CPlayerBird*)` / `IfTouchGround(CPlayerBird*)` 返回 bool

团队规则建议：

- 跨模块访问只通过 public 方法，不直接写 public 字段
- 只允许 `CBirdAPP` 修改状态 `m_GameStatus`
- 资源路径统一用 `./res/...`，替换资源同步更新尺寸宏与按钮热区

---

## 4) 项目落地流程（从 0 到 1 的正确推进顺序）

1. 搭框架骨架：窗口、消息、回调（`CGameFrame + Wndproc`）
2. 画静态场景：`CBackGround` + `CGround`
3. 角色最小闭环：`CPlayerBird`（渲染 + 重力）+ 输入上抬（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L111-L114)）
4. 障碍闭环：生成（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L164-L169)）+ 移动清理（[CColumnBox.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumnBox.cpp#L39-L56)）
5. 规则：碰撞（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L37-L49)）+ 触地（[CGround.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.cpp#L36-L43)）+ 计分（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L149-L153)）
6. 状态 UI：Start/Over 贴图合成（[BeforeGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BeforeGame/BeforeGame.cpp#L12-L16)、[CAfterGame.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/AfterGame/CAfterGame.cpp#L11-L15)）
7. 调优与清理：参数收敛到 [GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L25-L44)

### 资源管理与替换素材

- 当前是“硬编码路径 + 宏定义尺寸 + 逻辑按尺寸判定”
- 替换图片通常只要换 `res/*.png`
- 但素材尺寸变更要同步改 [GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L9-L24)
- UI 按钮图替换要同步按钮热区（[GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L54-L62)）和点击判断（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L116-L126)）

---

## 5) Debug 思路与常见问题（带具体例子）

### 典型 Bug 1：碰撞检测“穿模/误判”

定位入口：

- 柱子碰撞：[CColumn::IfTouchBird](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L37-L49)
- 触地：[CGround::IfTouchGround](file:///e:/personal_website/Flappy-Bird-EasyX-main/Ground/CGround.cpp#L36-L43)

Debug 方法：

- 在 `IfTouchBird` 打断点观察 `m_x/m_y`、`player->m_x/m_y`、`IMG_*` 是否合理
- 画碰撞盒（用 `rectangle()`）把判定范围可视化
- 根因常见：
  - `IMG_HALF_COLUMN_HEIGHT / IMG_COLUMN_DOWN_Y` 与美术不匹配（[GameConfig.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameConfig/GameConfig.h#L22-L24)）
  - 判定里大量 `*3/4`、`*1/4` 系数（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L39-L44)）对换皮不友好

### 典型 Bug 2：内存泄漏（这个项目非常容易踩）

高风险点：`new CColumn` + `list<CColumn*>`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L164-L169)）

`CColumnBox` 析构会 delete，但多处直接 `clear()` 会把指针丢掉：

- `On_Init` 状态 0：`m_ColumnBox.m_ColLst.clear();`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L56-L58)）
- `On_Close`：`m_ColumnBox.m_ColLst.clear();`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L96-L105)）

Debug 方法：

- Visual Studio 启用内存诊断或 CRT Leak 检测
- 在 `new CColumn` 打断点，多次开始/结束/重开后退出，看是否有未释放对象

### 典型 Bug 3：画面闪烁/残影/刷新不一致

`PaintGame()` 使用 `BeginBatchDraw + cleardevice + EndBatchDraw`（[frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L115-L124)），但 `Wndproc` 处理消息后直接调用 `On_Paint()`（[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp#L41-L45)），绕过了批绘与清屏，容易产生残影/闪烁。

### 典型 Bug 4：计时器混乱（状态切换后还在跑旧逻辑）

`On_SetTimer()` 会在不同状态设置不同 timer（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L22-L42)），状态切换前未统一 kill，可能导致重复触发/刷新节拍混乱。

---

## 6) 代码优化建议 + 排行榜/音效扩展方式

### 最值得改的点（按收益排序）

1. 统一渲染入口：让所有重绘都走 `PaintGame()`，不要在 `Wndproc` 直接 `On_Paint()`（[main.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/main.cpp#L41-L45) vs [frame.h](file:///e:/personal_website/Flappy-Bird-EasyX-main/GameFrame/frame.h#L115-L124)）
2. 修正柱子容器内存管理：禁止 `list<CColumn*>` 直接 `clear()`  
   - 最小改法：`CColumnBox::ClearAndDelete()`  
   - 更工程化：改成 `list<std::unique_ptr<CColumn>>`
3. 状态机 int → enum，状态切换封装成函数，降低漏 kill timer/漏 init 的概率
4. 降耦合：`CColumn` 不依赖 `CBirdAPP::m_rand`，改为 `InitColumn(int randValue)` 或注入 RNG/Config
5. 资源加载优化：避免柱子每次生成都 loadimage（[CColumn.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/Column/CColumn.cpp#L19-L20)），做共享缓存

### 添加排行榜（不破坏结构）

新增 `ScoreManager`：

- 维护 `currentScore/bestScore`
- GameOver 时更新 best
- 从磁盘读写（例如 `best_score.txt`）

接入点：

- 得分仍在 `CBirdAPP`（[m_score++](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L149-L153)）
- 进入结束状态时 `scoreManager.OnGameOver(m_score)`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L144-L148)、[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L193-L197)）
- 绘制 best：在 `ShowScore()` 或结束界面绘制后叠加（[ShowScore](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L215-L227)）

### 添加音效（当前项目无音频依赖）

新增 `AudioManager`（封装 `PlayClick/PlayScore/PlayHit`），接入点：

- 点击起跳：`On_WM_LBUTTONDOWN`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L108-L127)）
- 得分：`m_score++`（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L149-L153)）
- 撞击/触地：进入 GameOver 分支（[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L144-L148)、[CBirdAPP.cpp](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L193-L197)）

---

如果你愿意继续深入，我建议下一步按“读懂一个完整游戏循环”的方式走一遍：只盯住三处——[On_Init](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L45-L64)、[On_WM_TIMER](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L130-L201)、[On_Paint](file:///e:/personal_website/Flappy-Bird-EasyX-main/BirdAPP/CBirdAPP.cpp#L67-L93)。

你想先从哪个角度切入做一次“带断点的逐帧讲解”：计时器驱动，还是碰撞/计分，还是渲染与资源合成（SRCPAINT+SRCAND）？

