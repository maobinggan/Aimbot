#include "pch.h"

/**
 * CaptureFullScreen捕获全屏画面的数据
 * 参数 pBuffer	保存的文件路径
 * 参数 bufferSize	存放bitmap的字节缓冲区
 */
VOID CaptureFullScreen()
{
	//初始化缓冲区
	//int dwX = GetSystemMetrics(SM_CXSCREEN);						//获取当前分辨率
	//int dwY = GetSystemMetrics(SM_CYSCREEN);						//获取当前分辨率
	//int g_LineWidth = (((dwX * 24) + 31) & (0x7FFFFFFF - 31)) >> 3;	//每行像素所占的字节数：每行以4字节(32位)对齐，一行数据的长度不能被4整除时就在每行的末尾填充值为0的字节使之能被4整除。例如：对于宽5象素的位图每行数据占16个字节，前15个字节每3个字节保存1个象素颜色，最后填充1个字节。对于宽10象素的位图每行数据占32个字节，前30个字节每3个字节保存1个象素颜色，最后填充2个字节。
	//int byteSize = g_LineWidth * dwY;								//屏幕位图所占的总字节数
	//BYTE* pBitmaps = (BYTE*)malloc(byteSize);						//申请缓冲区，存放提取出来的像素数据
	//memset(pBitmaps, 0, byteSize);

	memset(g_pBitmapBuffer, 0, g_BitmapBufferSize);

	//截图	(Windows不允许程序员直接访问硬件，它对屏幕的操作是通过环境设备,也就是DC来完成的。屏幕上的每一个窗口都对应一个DC,可以把DC想象成一个视频缓冲区，对这这个缓冲区的操作，会表现在这个缓冲区对应的屏幕窗口上。在窗口的DC之外，可以建立自己的DC，就是说它不对应窗口，这个方法就是CreateCompatibleDC，这个DC就是一个内存缓冲区)
	HDC screenDC = GetDC(NULL);										//获取整个屏幕的DC
	HDC hMemDC = CreateCompatibleDC(screenDC);						//创建一个与屏幕窗口兼容的DC
	HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, g_ScrennX, g_ScrennY);	//创建一个与屏幕兼容的位图
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);	//将位图选入到缓冲区DC中 (关联起来，那么对hMemDC的操作也就是对hBitmap操作)
	BitBlt(hMemDC, 0, 0, g_ScrennX, g_ScrennY, screenDC, 0, 0, SRCCOPY);		//拷贝整个屏幕的图像到hMemDC中 

	//BitmapInfo初始化
	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = g_ScrennX;			//以像素为单位，给出该结构所描述位图的宽度；
	bitmapInfo.bmiHeader.biHeight = g_ScrennY;		//以像素为单位，给出该结构所描述位图的高度；
	bitmapInfo.bmiHeader.biPlanes = 1;			//必须为1
	bitmapInfo.bmiHeader.biBitCount = 24;		//以24位存储一个像素
	bitmapInfo.bmiHeader.biCompression = BI_RGB;//BI_RGB表示图像数据没有经过压缩处理；
	bitmapInfo.bmiHeader.biSizeImage = 0;		//以字节为单位，给出图像数据的大小，若图像为BI_RGB位图，则该成员的值必须设为0；

	//提取数据，存入BYTE缓冲区
	GetDIBits(hMemDC, hBitmap, 0, g_ScrennY, OUT g_pBitmapBuffer, IN &bitmapInfo, DIB_RGB_COLORS);

	//释放资源
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(screenDC);
	//free(pBitmaps);
	return;
}



/**
 * 将捕获的截图数据存为JPG图片
 * 参数 FilePath	保存的文件路径
 * 参数 pBitmaps	存放bitmap的字节缓冲区
 * 参数 byteSize	缓冲区大小
 * 参数 bitmapInfo  扫描图片结构体
 * eg:
 * CHAR filepath[] = "C:\\Users\\41388\\Desktop\\test.jpg";
 * CaptureSave(filepath, (DWORD)pBitmaps, byteSize, bitmapInfo);
 */
void MemToJPG(CHAR* FilePath, DWORD pBitmaps, DWORD byteSize, BITMAPINFO bitmapInfo)
{
	// 创建一个文件来保存文件截图
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

	// 设置 Offset 偏移至位图的位(bitmap bits)实际开始的地方
	bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	// 文件大小	(将图片头(headers)的大小, 加上位图的大小来获得整个文件的大小)
	bmpFileHeader.bfSize = byteSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// 位图的 bfType 必须是字符串 "BM"
	bmpFileHeader.bfType = 0x4D42; //BM

	//写入文件
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)pBitmaps, byteSize, &dwBytesWritten, NULL);

	// 关闭文件句柄
	CloseHandle(hFile);
	return;

}






/**
 * 正方形矩阵的螺旋遍历算法 (不支持步长)
 */
void  SpiralMatrix()
{
	//初始化方向
	DIRECTION up;		up.rowOff = -1;		up.columnOff = 0;		up.flag = UP;
	DIRECTION left;		left.rowOff = 0;	left.columnOff = -1;	left.flag = LEFT;
	DIRECTION right;	right.rowOff = 0;	right.columnOff = +1;	right.flag = RIGHT;
	DIRECTION down;		down.rowOff = +1;	down.columnOff = 0;		down.flag = DOWN;

	//正方形矩阵
	int matrix[5][5] =
	{
		1, 2, 3, 4, 5,  //第0行
		6, 7, 8, 9, 10,	//第1行
		11,12,13,14,15, //第2行
		16,17,18,19,20,
		21,22,23,24,25
	};
	//矩阵元素个数
	int num = 5 * 5;
	//总遍历次数
	int count = 0;
	//当前方向上的遍历次数
	int distance = 1;
	//当前方向
	DIRECTION currentDir = up;
	//起始点坐标
	int begin_row = 2;
	int begin_column = 2;

	//开始遍历
	printf("起始点：%d \n", matrix[begin_row][begin_column]);
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
