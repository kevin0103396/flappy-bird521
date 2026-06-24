#pragma once

// 定义UNICODE以支持中文显示
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include<easyx.h>
#include<map>
using namespace std;

/* -------------------------初始化消息映射列表的宏----------------------- */
#define INIT_MSGMAP(MSGID,MSGTYPE)\
		m_msgmap[MSGID].msg_type = MSGTYPE;\
		m_msgmap[MSGID].p_fun_##MSGTYPE = &CGameFrame::On_##MSGID;

/* ------------------------------回调函数的声明------------------------------*/
LRESULT Wndproc(HWND unnamedParam1, UINT unnamedParam2, WPARAM unnamedParam3, LPARAM unnamedParam4);


/* ---------------------------------------------------------------------------------------------------- */
class CGameFrame
{
private:
	/* -----声明友元函数以便直接使用类中的私有成员----- */
	friend LRESULT Wndproc(HWND unnamedParam1, UINT unnamedParam2, WPARAM unnamedParam3, LPARAM unnamedParam4);
	// 声明为回调函数以便使用私有的 map

	bool m_end; // 判断是否退出

	typedef void(CGameFrame::* P_FUN_MOUSE)(POINT&);
	typedef void(CGameFrame::* P_FUN_WINDOW)(WPARAM, LPARAM);
	typedef void (CGameFrame::* P_FUN_KEY)(WPARAM);
	typedef void (CGameFrame::* P_FUN_CHAR)(WPARAM);

	/* ----声明消息映射表的 value 对应的类型和函数---- */
	struct Fun_Type  // 该结构体用来存放消息映射表的value 对应的类型和函数
	{
		// Message Category
		#define EM_MOUSE	1
		#define EM_KEY	2
		#define EM_CHAR	4
		#define EM_WINDOW	8
		int msg_type; // 消息类型为 int 型 代表调用的函数
		union
		{
			P_FUN_MOUSE p_fun_EM_MOUSE; // 指向成员函数的指针
			P_FUN_WINDOW p_fun_EM_WINDOW;
			P_FUN_KEY p_fun_EM_KEY;
			P_FUN_CHAR p_fun_EM_CHAR;
		};
	};
	void InitMsgMap()
	{
			// 添加鼠标消息
			/*m_msgmap[WM_LBUTTONDOWN].msg_type = EM_MOUSE;
			m_msgmap[WM_LBUTTONDOWN].p_fun_mouse = &CGameFrame::On_WM_LBUTTONDOWN;*/
			/* 采用直接优化成宏 函数封装*/
			INIT_MSGMAP(WM_LBUTTONDOWN, EM_MOUSE)

			// 添加 window 类型的消息
			/*m_msgmap[WM_CLOSE].msg_type = EM_WINDOW;
			m_msgmap[WM_CLOSE].p_fun_window = &CGameFrame::On_WM_CLOSE;*/
			INIT_MSGMAP(WM_CLOSE, EM_WINDOW)

			// 添加 key 类型的消息
			INIT_MSGMAP(WM_KEYDOWN, EM_KEY)

			// 添加 char 类型的消息
			INIT_MSGMAP(WM_CHAR, EM_CHAR)

			//
			INIT_MSGMAP(WM_TIMER, EM_WINDOW)
	}

	/* ---------------------消息映射表-----------------------*/
	map<UINT, Fun_Type> m_msgmap;  // 用来将消息分类，key值为msgid, value为对应的类型和函数
				//	将 msgid 作为 key , 可自动对应同一个函数
				// 所有的消息在初始化阶段就已经注册

public:
	HWND hwnd; // 窗口句柄，可能会用到
	CGameFrame() :m_end(0)
	{

	}
	virtual ~CGameFrame()//虚析构函数
	{

	}

	/* -------判断是否结束--------- */
	bool IfEndGame()
	{
		return m_end;
	}

	/* --------游戏的初始化-------- */
	void InitGame(const TCHAR* title,int width=400 ,int height=400 ,int x=50, int y=50)
	{
		/* ------窗口的初始化------- */

		hwnd = initgraph(width, height);//窗口初始化及绘图界面
		MoveWindow(hwnd, x, y, width, height, TRUE);//改变窗口的位置
		SetWindowText(hwnd,title);
		setbkcolor(RGB(255, 255, 255));//设置背景颜色，设置当前设备的绘图背景色
		cleardevice();//这个函数使用当前背景色清空绘图设备。（即设置背景色之后调用cleardevice不会改变已有颜色，因为只会改变背景色的值）之后如果执行绘图填充，调用 outtextxy（则会使用你设置的文本颜色值），如果需要修改全部背景颜色，可以设置背景色并执行 cleardevice 来实现

		/* ------初始化消息列表中的消息------ */
		InitMsgMap(); // 窗口的初始化放到最后面

		//TODO:窗口的初始化;
		On_Init();

		/* ----设定窗口的回调,重新设定窗口的回调函数---- */
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&Wndproc);
		// 这个第一个参数是窗口句柄的地址，第二个参数是设置的值
	}

	/* --------游戏的重绘窗口-------- */
	void PaintGame()
	{
		BeginBatchDraw();
		cleardevice();

		//todo:窗口的游戏显示
		On_Paint();

		EndBatchDraw();
	}

	/* --------游戏的运行-------- */
	//void RunGame()
	//{
	//	ExMessage msg;//该结构体用来保存获取的消息，自动获取
	//	while (1)
	//	{
	//		getmessage(&msg); //获取窗口产生的消息  //这个函数在获取一条消息时，如果当前消息池中没有，则一直等待；
	//		if (msg.message == WM_LBUTTONDOWN)//鼠标左键按下消息
	//		{
	//			POINT po;
	//			po.x = msg.x;
	//			po.y = msg.y;
	//			On_WM_LBUTTONDOWN(po);
	//		}
	//	}
	//}

	// 无法捕获windows的所有消息（getmessage获取的消息种类不多），重构到win32,已经移到InitGame中的回调函数里

	/* --------游戏的结束-------- */
	void EndGame()
	{
		On_Close(); // 调用游戏的结束

		closegraph();
		m_end = true; // 退出标识
	}


	/* ------------------定义游戏的虚函数----------------------- */


	// 定义为虚函数，子类一定要重写这3个
	virtual void On_Init() = 0;
	virtual void On_Paint() = 0;
	virtual void On_Close() = 0;

	virtual void On_WM_LBUTTONDOWN(POINT& po)
	{
	}

	void On_WM_CLOSE(WPARAM wParam, LPARAM lParam)
	{
		EndGame();
	}

	virtual void On_WM_KEYDOWN(WPARAM wparam)
	{

	}

	virtual void On_WM_CHAR(WPARAM wparam)
	{

	}

	virtual void On_WM_TIMER(WPARAM wParam, LPARAM lParam)
	{

	}

};

/* ---项目的具体定义  来替换上面的定义  在窗口的main.cpp中使用--- */
#define WND_PARAM(POS_X,POS_Y,WIDTH,HEIGHT,TITLE)\
	int WND_POS_X = POS_X;\
	int WND_POS_Y = POS_Y;\
	int WND_WIDTH = WIDTH;\
	int WND_HEIGHT = HEIGHT;\
	const TCHAR* WND_TITLE = TITLE; // 窗口项目的名称

/* ----动态new创建对象的宏函数的封装---- */
#define DYNAMIC_CREATE(CLASS_OBJECT)\
CGameFrame* CreatObject(){ \
	return new CLASS_OBJECT; \
}
