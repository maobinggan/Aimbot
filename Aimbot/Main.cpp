#include "pch.h"

//定义：防抖参数（最大为20，否则受鼠标速度影响较大）
int g_AntiShake = 10;

//定义：扫描范围
int row_range = 200;
int col_range = 100;

//定义：截图参数
PBYTE g_pBitmapBuffer;
int g_BitmapBufferSize;
int g_ScrennX;
int g_ScrennY;
int g_LineWidth;

//单独开一个线程来发送鼠标事件。若在鼠标Hook回调函数中发送鼠标事件将卡死。
DWORD WINAPI AimbotThread(LPVOID lpThreadParameter)
{
	int Key = VK_LBUTTON;
	BOOL START = FALSE;

	while (true)
	{
		//自瞄
		if (START && (GetAsyncKeyState(Key)&0x8000))
		{
			AimBot_Player();
		}

		//左右键切换
		if (GetAsyncKeyState(VK_F8) & 1)
		{
			Key = (Key == VK_LBUTTON) ? VK_RBUTTON : VK_LBUTTON;
			Beep(2500, 200);
		}
		//停止自瞄
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			START = FALSE;
			Beep(6000, 200);
		}
		//开启自瞄（橘色）
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			START = TRUE;
			printf("【开启人物自瞄】防抖=%d 颜色模式=%s \n", g_AntiShake, "[橘色]");
			Beep(2000, 200);

			//[橘色模式] (标准=255,56,0)
			g_Player_rangeR_max = 255;//*
			g_Player_rangeR_min = 212;

			g_Player_rangeG_max = 68;//*
			g_Player_rangeG_min = 53;

			g_Player_rangeB_max = 25;
			g_Player_rangeB_min = 0;//*
		}

		//开启自瞄（红色）
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			START = TRUE;
			printf("【开启人物自瞄】防抖=%d 颜色模式=%s \n", g_AntiShake, "[红色]");
			Beep(2000, 200);

			//[红色模式](标准=255,0,38)
			g_Player_rangeR_max = 255;//*
			g_Player_rangeR_min = 215;

			g_Player_rangeG_max = 31;
			g_Player_rangeG_min = 0;//*

			g_Player_rangeB_max = 67;
			g_Player_rangeB_min = 37;//*
		}

		//调整防抖参数
		if (GetAsyncKeyState(VK_SUBTRACT) & 1)
		{
			printf("【防抖 -1】g_AntiShake=%d \n", --g_AntiShake);
		}
		if (GetAsyncKeyState(VK_ADD) & 1)
		{
			printf("【防抖 +1】g_AntiShake=%d \n", ++g_AntiShake);
		}

		//大幅降低CPU占用，释放一些未用的时间片给其他线程或进程使用
		Sleep(1);
	}
}

//初始化截图所需参数
VOID InitCapture()
{
	//截图所需数据初始化
	g_ScrennX = GetSystemMetrics(SM_CXSCREEN);							//获取当前分辨率
	g_ScrennY = GetSystemMetrics(SM_CYSCREEN);							//获取当前分辨率
	g_LineWidth = (((g_ScrennX * 24) + 31) & (0x7FFFFFFF - 31)) >> 3;	//每行像素所占的字节数：每行以4字节(32位)对齐，一行数据的长度不能被4整除时就在每行的末尾填充值为0的字节使之能被4整除。例如：对于宽5象素的位图每行数据占16个字节，前15个字节每3个字节保存1个象素颜色，最后填充1个字节。对于宽10象素的位图每行数据占32个字节，前30个字节每3个字节保存1个象素颜色，最后填充2个字节。
	g_BitmapBufferSize = g_LineWidth * g_ScrennY;						//屏幕位图所占的总字节数
	g_pBitmapBuffer = (BYTE*)malloc(g_BitmapBufferSize);				//申请缓冲区，存放提取出来的像素数据
	memset(g_pBitmapBuffer, 0, g_BitmapBufferSize);
}
//初始化其他
VOID InitOthers()
{
	//Switches focus to the specified window and brings it to the foreground.
	HWND hwnd = FindWindowA(0, TEXT("守望先锋"));
	if (hwnd != NULL) SwitchToThisWindow(hwnd, TRUE);//第2个参数为真，表示用视窗切换模式来切换窗口
}


void main()
{
	//初始化截图所需参数
	InitCapture();

	//初始化其他
	InitOthers();

	//创建自瞄线程
	HANDLE hThread = CreateThread(NULL, 0, AimbotThread, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
}



