#pragma once
#include "pch.h"

//============================= Ѫ��������� =======================================
//�����Ѫ����ʼλ�õ�����ƫ����
extern int g_HealBar_CenterOff_X;
extern int g_HealBar_CenterOff_Y;
//ѡ�����Ҽ�����
extern int chooseMouseButtonDown;
extern int chooseMouseButtonUp;
//���ؿ���
extern BOOL ISPRESS;
extern BOOL ISOPEN;
//��׼�����ƫ����
extern POINT AimPtOffset;
//===================================================================================

//============================= ����������� =======================================
//��ɫģ������
extern int g_Player_rangeR_max;
extern int g_Player_rangeR_min;
extern int g_Player_rangeG_max;
extern int g_Player_rangeG_min;
extern int g_Player_rangeB_max;
extern int g_Player_rangeB_min;
//===================================================================================

//============================= ���в��� =========================================
//ɨ�跶Χ
extern int row_range;
extern int col_range;
//��������
extern int g_AntiShake;
//��ͼ���ݻ�����
extern PBYTE g_pBitmapBuffer;
extern int g_BitmapBufferSize;
extern int g_ScrennX;
extern int g_ScrennY;
extern int g_LineWidth;
//������������ı��� 
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
