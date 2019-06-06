#pragma once
#include "pch.h"

//============================= 血条自瞄参数 =======================================
//相对于血条起始位置的中心偏移量
extern int g_HealBar_CenterOff_X;
extern int g_HealBar_CenterOff_Y;
//选择左右键自瞄
extern int chooseMouseButtonDown;
extern int chooseMouseButtonUp;
//开关控制
extern BOOL ISPRESS;
extern BOOL ISOPEN;
//瞄准的鼠标偏移量
extern POINT AimPtOffset;
//===================================================================================

//============================= 人物自瞄参数 =======================================
//颜色模糊区间
extern int g_Player_rangeR_max;
extern int g_Player_rangeR_min;
extern int g_Player_rangeG_max;
extern int g_Player_rangeG_min;
extern int g_Player_rangeB_max;
extern int g_Player_rangeB_min;
//===================================================================================

//============================= 共有参数 =========================================
//扫描范围
extern int row_range;
extern int col_range;
//防抖参数
extern int g_AntiShake;
//截图数据缓冲区
extern PBYTE g_pBitmapBuffer;
extern int g_BitmapBufferSize;
extern int g_ScrennX;
extern int g_ScrennY;
extern int g_LineWidth;
//螺旋遍历矩阵的变量 
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
struct DIRECTION
{
	int flag;
	int rowOff;
	int columnOff;
};
//===================================================================================


//===========================Function====================================================
BOOL FindBloodBar();
VOID InitCapture();

VOID AimBot_Player();
VOID CaptureFullScreen();
BOOL FindNearestPlayer(OUT POINT& nearestPos, IN POINT mousePos);
VOID CaculatePlayerModel(OUT int& edge_left, OUT int& edge_right, OUT int& edge_top, IN POINT& nearestPos, IN POINT mousePos);
POINT GetAimOffset(IN int edge_left,IN int edge_right,IN int edge_top, IN POINT mousePos);
