// AutoAim_FindColor.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#pragma comment(lib, "Winmm.lib") //播放wav音乐 

//设置自瞄按键
int chooseMouseButtonDown = WM_LBUTTONDOWN;
int chooseMouseButtonUp = WM_LBUTTONUP;

//定义：瞄准的坐标偏移（距上一次鼠标坐标的相对偏移）
POINT AimPtOffset;

 //血条颜色模糊区间（血条半透明）
int destR_min = 185;
int destR_max = 218;
int destG_min = 0;
int destG_max = 0;
int destB_min = 18;
int destB_max = 19;

//血条的准心偏移量（相对于血条起始点）	
int g_HealBar_CenterOff_X = 50;
int g_HealBar_CenterOff_Y = 38;


/**
 * 寻找血条
 */
BOOL FindBloodBar()
{
	//截图所需数据初始化
	int dwX = GetSystemMetrics(SM_CXSCREEN);					 //获取当前分辨率
	int dwY = GetSystemMetrics(SM_CYSCREEN);					 //获取当前分辨率
	int g_LineWidth = (((dwX * 24) + 31) & (0x7FFFFFFF - 31)) >> 3;//每行像素所占的字节数：每行以4字节(32位)对齐，一行数据的长度不能被4整除时就在每行的末尾填充值为0的字节使之能被4整除。例如：对于宽5象素的位图每行数据占16个字节，前15个字节每3个字节保存1个象素颜色，最后填充1个字节。对于宽10象素的位图每行数据占32个字节，前30个字节每3个字节保存1个象素颜色，最后填充2个字节。
	int byteSize = g_LineWidth * dwY;								 //屏幕位图所占的总字节数
	BYTE* pBitmaps = (BYTE*)malloc(byteSize);					 //申请缓冲区，存放提取出来的像素数据
	memset(pBitmaps, 0, byteSize);

	//截图	(Windows不允许程序员直接访问硬件，它对屏幕的操作是通过环境设备,也就是DC来完成的。屏幕上的每一个窗口都对应一个DC,可以把DC想象成一个视频缓冲区，对这这个缓冲区的操作，会表现在这个缓冲区对应的屏幕窗口上。在窗口的DC之外，可以建立自己的DC，就是说它不对应窗口，这个方法就是CreateCompatibleDC，这个DC就是一个内存缓冲区)
	HDC screenDC = GetDC(NULL);										//获取整个屏幕的DC
	HDC hMemDC = CreateCompatibleDC(screenDC);						//创建一个与屏幕窗口兼容的DC
	HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, 1920, 1080); //创建一个与屏幕兼容的位图
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);	//将位图选入到缓冲区DC中 (关联起来，那么对hMemDC的操作也就是对hBitmap操作)
	BitBlt(hMemDC, 0, 0, dwX, dwY, screenDC, 0, 0, SRCCOPY);		//拷贝整个屏幕的图像到hMemDC中 

	//BitmapInfo初始化
	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = dwX;			//以像素为单位，给出该结构所描述位图的宽度；
	bitmapInfo.bmiHeader.biHeight = dwY;		//以像素为单位，给出该结构所描述位图的高度；
	bitmapInfo.bmiHeader.biPlanes = 1;			//必须为1
	bitmapInfo.bmiHeader.biBitCount = 24;		//以24位存储一个像素
	bitmapInfo.bmiHeader.biCompression = BI_RGB;//BI_RGB表示图像数据没有经过压缩处理；
	bitmapInfo.bmiHeader.biSizeImage = 0;		//以字节为单位，给出图像数据的大小，若图像为BI_RGB位图，则该成员的值必须设为0；

	//提取数据，存入BYTE缓冲区
	GetDIBits(hMemDC, hBitmap, 0, dwY, OUT pBitmaps, IN &bitmapInfo, DIB_RGB_COLORS);

	//获取当前鼠标坐标，左上角是(0,0)点，右下角是(1920,1080)
	POINT pt;
	GetCursorPos(&pt);

	//开始扫描血条
	if (pt.x > 200 && pt.x < 1720 && pt.y > 200 && pt.y < 880)
	{//防一手扫描坐标越界

		//row_off和col_off是相对于鼠标的行列偏移，例如鼠标上方10像素的点，其row_off= -10
		int row_flag = 1;
		for (int row_off = 0; row_off < row_range; )
		{
			//从y=0处开始扫描，以上下上下的顺序扫描
			row_flag = -row_flag;
			if (row_flag == 1)
			{
				row_off++;
			}
			for (int col_off = -col_range; col_off < col_range; col_off++)
			{
				//将二维数组矩阵坐标( [Top][Left]，左上角是0,0点 )转为一维字节缓冲区的偏移
				int Top = pt.y + row_off * row_flag;
				int Left = pt.x + col_off;
				int bufferOffset = ((dwY - Top - 1)*g_LineWidth) + (Left * 3);

				//转化为RGB颜色
				BYTE B = pBitmaps[bufferOffset + 0];
				BYTE G = pBitmaps[bufferOffset + 1];
				BYTE R = pBitmaps[bufferOffset + 2];
				COLORREF color = RGB(B, G, R);

				//判断是否为血条颜色
				if (R >= destR_min && R <= destR_max
					&& G >= destG_min && G <= destG_max
					&& B >= destB_min && B <= destB_max)
				{
					//要相对移动的位置=血条起始位置+血条中心偏移
					AimPtOffset.x = col_off + g_HealBar_CenterOff_X;
					AimPtOffset.y = row_off * row_flag + g_HealBar_CenterOff_Y;
					//防抖：限制每次鼠标移动的幅度（最大为20，否则受鼠标速度影响较大）
					if (AimPtOffset.x > g_AntiShake)
					{
						AimPtOffset.x = g_AntiShake;
					}
					if (AimPtOffset.x < -g_AntiShake)
					{
						AimPtOffset.x = -g_AntiShake;
					}
					if (AimPtOffset.y > g_AntiShake)
					{
						AimPtOffset.y = g_AntiShake;
					}
					if (AimPtOffset.y < -g_AntiShake)
					{
						AimPtOffset.y = -g_AntiShake;
					}
					//释放资源
					DeleteObject(hBitmap);
					DeleteDC(hMemDC);
					DeleteDC(screenDC);
					free(pBitmaps);
					return TRUE;
				}
			}
		}
	}
	//释放资源
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(screenDC);
	free(pBitmaps);
	return FALSE;
}


