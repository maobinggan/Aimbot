#include "pch.h"

//���壺�������������Ϊ20������������ٶ�Ӱ��ϴ�
int g_AntiShake = 10;

//���壺ɨ�跶Χ
int row_range = 200;
int col_range = 100;

//���壺��ͼ����
PBYTE g_pBitmapBuffer;
int g_BitmapBufferSize;
int g_ScrennX;
int g_ScrennY;
int g_LineWidth;

//������һ���߳�����������¼����������Hook�ص������з�������¼���������
DWORD WINAPI AimbotThread(LPVOID lpThreadParameter)
{
	int Key = VK_LBUTTON;
	BOOL START = FALSE;

	while (true)
	{
		//����
		if (START && (GetAsyncKeyState(Key)&0x8000))
		{
			AimBot_Player();
		}

		//���Ҽ��л�
		if (GetAsyncKeyState(VK_F8) & 1)
		{
			Key = (Key == VK_LBUTTON) ? VK_RBUTTON : VK_LBUTTON;
			Beep(2500, 200);
		}
		//ֹͣ����
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			START = FALSE;
			Beep(6000, 200);
		}
		//�������飨��ɫ��
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			START = TRUE;
			printf("�������������顿����=%d ��ɫģʽ=%s \n", g_AntiShake, "[��ɫ]");
			Beep(2000, 200);

			//[��ɫģʽ] (��׼=255,56,0)
			g_Player_rangeR_max = 255;//*
			g_Player_rangeR_min = 212;

			g_Player_rangeG_max = 68;//*
			g_Player_rangeG_min = 53;

			g_Player_rangeB_max = 25;
			g_Player_rangeB_min = 0;//*
		}

		//�������飨��ɫ��
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			START = TRUE;
			printf("�������������顿����=%d ��ɫģʽ=%s \n", g_AntiShake, "[��ɫ]");
			Beep(2000, 200);

			//[��ɫģʽ](��׼=255,0,38)
			g_Player_rangeR_max = 255;//*
			g_Player_rangeR_min = 215;

			g_Player_rangeG_max = 31;
			g_Player_rangeG_min = 0;//*

			g_Player_rangeB_max = 67;
			g_Player_rangeB_min = 37;//*
		}

		//������������
		if (GetAsyncKeyState(VK_SUBTRACT) & 1)
		{
			printf("������ -1��g_AntiShake=%d \n", --g_AntiShake);
		}
		if (GetAsyncKeyState(VK_ADD) & 1)
		{
			printf("������ +1��g_AntiShake=%d \n", ++g_AntiShake);
		}

		//�������CPUռ�ã��ͷ�һЩδ�õ�ʱ��Ƭ�������̻߳����ʹ��
		Sleep(1);
	}
}

//��ʼ����ͼ�������
VOID InitCapture()
{
	//��ͼ�������ݳ�ʼ��
	g_ScrennX = GetSystemMetrics(SM_CXSCREEN);							//��ȡ��ǰ�ֱ���
	g_ScrennY = GetSystemMetrics(SM_CYSCREEN);							//��ȡ��ǰ�ֱ���
	g_LineWidth = (((g_ScrennX * 24) + 31) & (0x7FFFFFFF - 31)) >> 3;	//ÿ��������ռ���ֽ�����ÿ����4�ֽ�(32λ)���룬һ�����ݵĳ��Ȳ��ܱ�4����ʱ����ÿ�е�ĩβ���ֵΪ0���ֽ�ʹ֮�ܱ�4���������磺���ڿ�5���ص�λͼÿ������ռ16���ֽڣ�ǰ15���ֽ�ÿ3���ֽڱ���1��������ɫ��������1���ֽڡ����ڿ�10���ص�λͼÿ������ռ32���ֽڣ�ǰ30���ֽ�ÿ3���ֽڱ���1��������ɫ��������2���ֽڡ�
	g_BitmapBufferSize = g_LineWidth * g_ScrennY;						//��Ļλͼ��ռ�����ֽ���
	g_pBitmapBuffer = (BYTE*)malloc(g_BitmapBufferSize);				//���뻺�����������ȡ��������������
	memset(g_pBitmapBuffer, 0, g_BitmapBufferSize);
}
//��ʼ������
VOID InitOthers()
{
	//Switches focus to the specified window and brings it to the foreground.
	HWND hwnd = FindWindowA(0, TEXT("�����ȷ�"));
	if (hwnd != NULL) SwitchToThisWindow(hwnd, TRUE);//��2������Ϊ�棬��ʾ���Ӵ��л�ģʽ���л�����
}


void main()
{
	//��ʼ����ͼ�������
	InitCapture();

	//��ʼ������
	InitOthers();

	//���������߳�
	HANDLE hThread = CreateThread(NULL, 0, AimbotThread, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
}



