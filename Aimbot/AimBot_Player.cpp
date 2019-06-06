#include "pch.h"

//����ȫ�ֱ���
int g_Player_rangeR_max = 0;
int g_Player_rangeR_min = 0;
int g_Player_rangeG_max = 0;
int g_Player_rangeG_min = 0;
int g_Player_rangeB_max = 0;
int g_Player_rangeB_min = 0;

BOOL FindNearestPlayer(OUT POINT& nearestPos, IN POINT mousePos)
{
	//��ʼ������
	DIRECTION up;		up.rowOff = -1;		up.columnOff = 0;		up.flag = UP;
	DIRECTION left;		left.rowOff = 0;	left.columnOff = -1;	left.flag = LEFT;
	DIRECTION right;	right.rowOff = 0;	right.columnOff = +1;	right.flag = RIGHT;
	DIRECTION down;		down.rowOff = +1;	down.columnOff = 0;		down.flag = DOWN;

	//����Ԫ�ظ���
	int num = 150 * 150;
	//�ѱ�������
	int count = 0;
	//��ǰ�����ϵı�������
	int distance = 1; //
	//��ǰ����
	DIRECTION currentDir = up;
	//��ʼ������
	int begin_row = mousePos.y;
	int begin_column = mousePos.x;

	//��ʼ��������
	while (count < num)
	{
		for (int i = 1; i <= distance; i++)
		{
			begin_row = begin_row + currentDir.rowOff;
			begin_column = begin_column + currentDir.columnOff;
			int Top = begin_row;
			int Left = begin_column;
			int bufferOffset = ((g_ScrennY - Top - 1)*g_LineWidth) + (Left * 3);
			//printf("begin_row=%d begin_column=%d distance=%d count=%d\n", begin_row, begin_column, distance, count);
			//ת��ΪRGB��ɫ
			BYTE B = g_pBitmapBuffer[bufferOffset + 0];
			BYTE G = g_pBitmapBuffer[bufferOffset + 1];
			BYTE R = g_pBitmapBuffer[bufferOffset + 2];
			COLORREF color = RGB(B, G, R);
			if (R >= g_Player_rangeR_min && R <= g_Player_rangeR_max
				&& G >= g_Player_rangeG_min && G <= g_Player_rangeG_max
				&& B >= g_Player_rangeB_min && B <= g_Player_rangeB_max)
			{
				nearestPos.x = Left;
				nearestPos.y = Top;
				return TRUE;
			}
			count++;
		}
		switch (currentDir.flag)
		{
		case UP:
			currentDir = right;
			break;
		case RIGHT:
			currentDir = down;
			distance++;
			break;
		case DOWN:
			currentDir = left;
			break;
		case LEFT:
			currentDir = up;
			distance++;
			break;
		default:
			break;
		}
	}
	return FALSE;
}

//��������ģ�͵�����������      
VOID CaculatePlayerModel(OUT int& edge_left, OUT int& edge_right, OUT int& edge_top, IN POINT& nearestPos, IN POINT mousePos)
{
	int bufferOffset = 0;
	BYTE B = 0;
	BYTE G = 0;
	BYTE R = 0;

	edge_left = 0;
	edge_right = 0;
	edge_top = mousePos.y;

	for (int tempLeft = nearestPos.x - 50; tempLeft <= nearestPos.x; tempLeft++)
	{
		bufferOffset = ((g_ScrennY - nearestPos.y - 1)*g_LineWidth) + (tempLeft * 3);
		B = g_pBitmapBuffer[bufferOffset + 0];
		G = g_pBitmapBuffer[bufferOffset + 1];
		R = g_pBitmapBuffer[bufferOffset + 2];
		if (R >= g_Player_rangeR_min && R <= g_Player_rangeR_max
			&& G >= g_Player_rangeG_min && G <= g_Player_rangeG_max
			&& B >= g_Player_rangeB_min && B <= g_Player_rangeB_max)
		{
			edge_left = tempLeft; break;
		}
	}
	for (int tempLeft = nearestPos.x + 50; tempLeft >= nearestPos.x; tempLeft--)
	{
		bufferOffset = ((g_ScrennY - nearestPos.y - 1)*g_LineWidth) + (tempLeft * 3);
		B = g_pBitmapBuffer[bufferOffset + 0];
		G = g_pBitmapBuffer[bufferOffset + 1];
		R = g_pBitmapBuffer[bufferOffset + 2];
		if (R >= g_Player_rangeR_min && R <= g_Player_rangeR_max
			&& G >= g_Player_rangeG_min && G <= g_Player_rangeG_max
			&& B >= g_Player_rangeB_min && B <= g_Player_rangeB_max)
		{
			edge_right = tempLeft; break;
		}
	}

	int center_x = (edge_left + edge_right) / 2;

	//ɨ����������ĵ�������+/-100��Χ��������������������±߽�
	for (int tempTop = nearestPos.y - 100; tempTop <= nearestPos.y; tempTop++)
	{
		int Left = center_x;
		bufferOffset = ((g_ScrennY - tempTop - 1)*g_LineWidth) + (Left * 3);
		B = g_pBitmapBuffer[bufferOffset + 0];
		G = g_pBitmapBuffer[bufferOffset + 1];
		R = g_pBitmapBuffer[bufferOffset + 2];
		if (R >= g_Player_rangeR_min && R <= g_Player_rangeR_max
			&& G >= g_Player_rangeG_min && G <= g_Player_rangeG_max
			&& B >= g_Player_rangeB_min && B <= g_Player_rangeB_max)
		{
			edge_top = tempTop; break;
		}
	}

	return;
}



POINT GetAimOffset(IN int edge_left, IN int edge_right, IN int edge_top, IN POINT mousePos)
{
	//��������ͷ�����������ƫ��
	POINT AimPtOffset = { 0 };
	AimPtOffset.x = (edge_left + edge_right) / 2 - mousePos.x;//���������ĵ������ľ���
	AimPtOffset.y = edge_top - mousePos.y + 2;	//ͷ�����������ľ���
	//printf("ԭƫ��=%d,%d ", AimPtOffset.x, AimPtOffset.y);

	
	//�����������������ƫ����
	const float constant = 0.116f;	//0.090 seems to work better sometimes
	float mouseSensitivity = 15.0f;		//found in game menu
	float modifier = mouseSensitivity * constant;	//modifier = sens*const.
	AimPtOffset.x = (int)((float)AimPtOffset.x / modifier);
	AimPtOffset.y = (int)((float)AimPtOffset.y / modifier);

	//����
	if (abs(AimPtOffset.x) > g_AntiShake) { AimPtOffset.x = (AimPtOffset.x > 0) ? g_AntiShake : -g_AntiShake; }
	if (abs(AimPtOffset.y) > g_AntiShake) { AimPtOffset.y = (AimPtOffset.y > 0) ? g_AntiShake : -g_AntiShake; }
	printf("x=%d,y=%d \n", AimPtOffset.x, AimPtOffset.y);

	return AimPtOffset;
}

VOID AimBot_Player()
{
	//ȫ����ͼ
	CaptureFullScreen();

	//��ȡ��ǰ������꣬���Ͻ���(0,0)�㣬���½���(1920,1080)
	POINT pt;
	GetCursorPos(&pt);

	//��������Խ��
	if (pt.x > 200 && pt.x < 1720 && pt.y > 200 && pt.y < 880)
	{
		//���Ҿ������������������ص�λ��
		POINT nearstPos;
		if (FindNearestPlayer(OUT nearstPos, IN pt))
		{
			//��������ģ�͵��ϸ߶ȡ����ҿ��
			int edge_left = 0;
			int edge_right = 0;
			int edge_top = 0;
			CaculatePlayerModel(OUT edge_left, OUT edge_right, OUT edge_top, IN nearstPos, IN pt);

			//��������¼��ƶ���ƫ����
			POINT AimPtOffset = GetAimOffset(IN edge_left, IN edge_right, IN edge_top, IN pt);

			//�ƶ���꣺����ƶ�(���ϴ������������ƫ��,������ƶ��ٶ�Ӱ��)
			mouse_event(MOUSEEVENTF_MOVE, AimPtOffset.x, AimPtOffset.y, 0, 0);
			//printf("�����ƶ���꣺x=%d ,y=%d \n", AimPtOffset.x, AimPtOffset.y);
		}
	}
}

