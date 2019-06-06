#include "pch.h"

/**
 * CaptureFullScreen����ȫ�����������
 * ���� pBuffer	������ļ�·��
 * ���� bufferSize	���bitmap���ֽڻ�����
 */
VOID CaptureFullScreen()
{
	//��ʼ��������
	//int dwX = GetSystemMetrics(SM_CXSCREEN);						//��ȡ��ǰ�ֱ���
	//int dwY = GetSystemMetrics(SM_CYSCREEN);						//��ȡ��ǰ�ֱ���
	//int g_LineWidth = (((dwX * 24) + 31) & (0x7FFFFFFF - 31)) >> 3;	//ÿ��������ռ���ֽ�����ÿ����4�ֽ�(32λ)���룬һ�����ݵĳ��Ȳ��ܱ�4����ʱ����ÿ�е�ĩβ���ֵΪ0���ֽ�ʹ֮�ܱ�4���������磺���ڿ�5���ص�λͼÿ������ռ16���ֽڣ�ǰ15���ֽ�ÿ3���ֽڱ���1��������ɫ��������1���ֽڡ����ڿ�10���ص�λͼÿ������ռ32���ֽڣ�ǰ30���ֽ�ÿ3���ֽڱ���1��������ɫ��������2���ֽڡ�
	//int byteSize = g_LineWidth * dwY;								//��Ļλͼ��ռ�����ֽ���
	//BYTE* pBitmaps = (BYTE*)malloc(byteSize);						//���뻺�����������ȡ��������������
	//memset(pBitmaps, 0, byteSize);

	memset(g_pBitmapBuffer, 0, g_BitmapBufferSize);

	//��ͼ	(Windows���������Աֱ�ӷ���Ӳ����������Ļ�Ĳ�����ͨ�������豸,Ҳ����DC����ɵġ���Ļ�ϵ�ÿһ�����ڶ���Ӧһ��DC,���԰�DC�����һ����Ƶ����������������������Ĳ�����������������������Ӧ����Ļ�����ϡ��ڴ��ڵ�DC֮�⣬���Խ����Լ���DC������˵������Ӧ���ڣ������������CreateCompatibleDC�����DC����һ���ڴ滺����)
	HDC screenDC = GetDC(NULL);										//��ȡ������Ļ��DC
	HDC hMemDC = CreateCompatibleDC(screenDC);						//����һ������Ļ���ڼ��ݵ�DC
	HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, g_ScrennX, g_ScrennY);	//����һ������Ļ���ݵ�λͼ
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);	//��λͼѡ�뵽������DC�� (������������ô��hMemDC�Ĳ���Ҳ���Ƕ�hBitmap����)
	BitBlt(hMemDC, 0, 0, g_ScrennX, g_ScrennY, screenDC, 0, 0, SRCCOPY);		//����������Ļ��ͼ��hMemDC�� 

	//BitmapInfo��ʼ��
	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = g_ScrennX;			//������Ϊ��λ�������ýṹ������λͼ�Ŀ�ȣ�
	bitmapInfo.bmiHeader.biHeight = g_ScrennY;		//������Ϊ��λ�������ýṹ������λͼ�ĸ߶ȣ�
	bitmapInfo.bmiHeader.biPlanes = 1;			//����Ϊ1
	bitmapInfo.bmiHeader.biBitCount = 24;		//��24λ�洢һ������
	bitmapInfo.bmiHeader.biCompression = BI_RGB;//BI_RGB��ʾͼ������û�о���ѹ������
	bitmapInfo.bmiHeader.biSizeImage = 0;		//���ֽ�Ϊ��λ������ͼ�����ݵĴ�С����ͼ��ΪBI_RGBλͼ����ó�Ա��ֵ������Ϊ0��

	//��ȡ���ݣ�����BYTE������
	GetDIBits(hMemDC, hBitmap, 0, g_ScrennY, OUT g_pBitmapBuffer, IN &bitmapInfo, DIB_RGB_COLORS);

	//�ͷ���Դ
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(screenDC);
	//free(pBitmaps);
	return;
}



/**
 * ������Ľ�ͼ���ݴ�ΪJPGͼƬ
 * ���� FilePath	������ļ�·��
 * ���� pBitmaps	���bitmap���ֽڻ�����
 * ���� byteSize	��������С
 * ���� bitmapInfo  ɨ��ͼƬ�ṹ��
 * eg:
 * CHAR filepath[] = "C:\\Users\\41388\\Desktop\\test.jpg";
 * CaptureSave(filepath, (DWORD)pBitmaps, byteSize, bitmapInfo);
 */
void MemToJPG(CHAR* FilePath, DWORD pBitmaps, DWORD byteSize, BITMAPINFO bitmapInfo)
{
	// ����һ���ļ��������ļ���ͼ
	HANDLE hFile = CreateFile(
		FilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	BITMAPFILEHEADER bmpFileHeader;

	// ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
	bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	// �ļ���С	(��ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С)
	bmpFileHeader.bfSize = byteSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// λͼ�� bfType �������ַ��� "BM"
	bmpFileHeader.bfType = 0x4D42; //BM

	//д���ļ�
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)pBitmaps, byteSize, &dwBytesWritten, NULL);

	// �ر��ļ����
	CloseHandle(hFile);
	return;

}






/**
 * �����ξ�������������㷨 (��֧�ֲ���)
 */
void  SpiralMatrix()
{
	//��ʼ������
	DIRECTION up;		up.rowOff = -1;		up.columnOff = 0;		up.flag = UP;
	DIRECTION left;		left.rowOff = 0;	left.columnOff = -1;	left.flag = LEFT;
	DIRECTION right;	right.rowOff = 0;	right.columnOff = +1;	right.flag = RIGHT;
	DIRECTION down;		down.rowOff = +1;	down.columnOff = 0;		down.flag = DOWN;

	//�����ξ���
	int matrix[5][5] =
	{
		1, 2, 3, 4, 5,  //��0��
		6, 7, 8, 9, 10,	//��1��
		11,12,13,14,15, //��2��
		16,17,18,19,20,
		21,22,23,24,25
	};
	//����Ԫ�ظ���
	int num = 5 * 5;
	//�ܱ�������
	int count = 0;
	//��ǰ�����ϵı�������
	int distance = 1;
	//��ǰ����
	DIRECTION currentDir = up;
	//��ʼ������
	int begin_row = 2;
	int begin_column = 2;

	//��ʼ����
	printf("��ʼ�㣺%d \n", matrix[begin_row][begin_column]);
	while (count < num - 1)
	{
		for (int i = 1; i <= distance; i++)
		{
			begin_row = begin_row + currentDir.rowOff;
			begin_column = begin_column + currentDir.columnOff;
			printf("%d \n", matrix[begin_row][begin_column]);
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
	return;
}
