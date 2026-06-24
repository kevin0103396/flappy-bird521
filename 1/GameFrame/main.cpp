#include<easyx.h>
#include"frame.h"
#include<windowsx.h>
#include<iostream>
using namespace std;
CGameFrame* P_G = nullptr;

/* -----------回调函数的声明------------ */
LRESULT Wndproc(HWND hwnd, UINT msgid, WPARAM wparam, LPARAM lparam)
{				/* 第一个参数是窗口句柄，第二个是消息ID，后面是消息ID携带的参数 */
	/* ------声明这些直接优化了，因为是固定的------ */
	if (P_G->m_msgmap.count(msgid)) // 如果消息列表中有 msgid 对应的消息处理
	{
		switch (P_G->m_msgmap[msgid].msg_type)
		{
		case EM_MOUSE: // 如果鼠标类类型的 msgid
		{
			POINT po = { 0 };// 创建一个结构体，有long x ,long y
			po.x = GET_X_LPARAM(lparam); // windowsx.h  鼠标左键按下WM_LBUTTONDOWN
			po.y = GET_Y_LPARAM(lparam); // windowsx.h
			(P_G->*(P_G->m_msgmap[msgid].p_fun_EM_MOUSE))(po); // 调用成员函数的指针
		}
		break;
		case EM_WINDOW:
		{
			(P_G->*(P_G->m_msgmap[msgid].p_fun_EM_WINDOW))(wparam, lparam);
		}
		break;
		case EM_KEY:
		{
			(P_G->*(P_G->m_msgmap[msgid].p_fun_EM_KEY))(wparam);
		}
		break;
		case EM_CHAR:
		{
			(P_G->*(P_G->m_msgmap[msgid].p_fun_EM_CHAR))(wparam);
		}
		break;
		}

		if (!P_G->IfEndGame())
		{
			// 消息处理完刷新窗口
			P_G->PaintGame();
		}
	}
	return DefWindowProc(hwnd, msgid, wparam, lparam); // 调用一下windows默认的回调函数，处理上面没有处理的选择范围内的消息
}


// 这些变量为声明，在外部文件中的定义
CGameFrame* CreatObject();
extern int WND_POS_X;
extern int WND_POS_Y;
extern int WND_WIDTH;
extern int WND_HEIGHT;
extern const TCHAR* WND_TITLE;


int main()
{
	// 设置工作目录为exe所在目录，确保资源文件路径正确
	TCHAR exePath[MAX_PATH];
	GetModuleFileName(NULL, exePath, MAX_PATH);
	// 去掉文件名，保留目录
	TCHAR* lastSlash = _tcsrchr(exePath, _T('\\'));
	if (lastSlash)
	{
		*lastSlash = _T('\0');
	}
	SetCurrentDirectory(exePath);

	P_G = CreatObject();// 有纯虚函数因为是抽象类，无法实例化
	P_G->InitGame(WND_TITLE,WND_WIDTH, WND_HEIGHT, WND_POS_X, WND_POS_Y);
	//P_G->RunGame();
	P_G->PaintGame(); // 初始化游戏时重绘一次

	while (!P_G->IfEndGame()) {

		Sleep(200);
	}

	delete P_G;
	P_G = nullptr;

	return 0;
}


