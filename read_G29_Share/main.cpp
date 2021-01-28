#include <Windows.h>
#include <thread>
#include <stdio.h>
#include <iostream>
#include<winsock.h>

#include "LogitechSteeringWheelLib.h"
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "LogitechSteeringWheelLib.lib")
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

//初始化
void WheelInit(HWND hwnd)
{


	if (LogiSteeringInitializeWithWindow(true, hwnd))
	{

		printf("init secuss\n");
		//printf("%d\n", hwnd);
		while (LogiUpdate() && LogiIsConnected(0))//设备0
		{
			//printf("connect secuss\n");
			Sleep(100);//100
			DIJOYSTATE2 * wheel = LogiGetState(0);//设备0
			
			//输出角度，油门，刹车信息
			printf("方向盘角度 = %d  油门 = %d  刹车 = %d\n", wheel->lX, wheel->lY, wheel->lRz);
			Sleep(1000);
		}

	}
	else
	{
		printf("init faild");
	}

}


// 必须要进行前导声明  
LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	// 类名  
	TCHAR cls_Name[] = TEXT("My Class");//TCHAR cls_Name[] = L"My Class";
	// 设计窗口类  
	WNDCLASS wc = { sizeof(WNDCLASS) };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = cls_Name;
	wc.hInstance = hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	// 注册窗口类  
	RegisterClass(&wc);

	// 创建窗口
	HWND hwnd = CreateWindow(
		cls_Name,           //类名，要和刚才注册的一致  
		TEXT("方向盘Demo"),          //窗口标题文字 L"方向盘Demo"
		WS_OVERLAPPEDWINDOW,        //窗口外观样式  
		38,             //窗口相对于父级的X坐标  
		20,             //窗口相对于父级的Y坐标  
		500,                //窗口的宽度  
		500,                //窗口的高度  
		NULL,               //没有父窗口，为NULL  
		NULL,               //没有菜单，为NULL  
		hInstance,          //当前应用程序的实例句柄  
		NULL);              //没有附加数据，为NULL  
	if (hwnd == NULL)                //检查窗口是否创建成功  
		return 0;

	// 显示窗口  
	ShowWindow(hwnd, SW_SHOW);

	// 更新窗口  
	UpdateWindow(hwnd);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //设置窗口最前端
	thread GetWheelData(WheelInit, hwnd);
	GetWheelData.detach();
	//MessageBox(0, "调用了WinMain函数", "测试：", 0);


	// 消息循环  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// 在WinMain后实现  
LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	)
{

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		/*PAINTSTRUCT     ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		int length;
		TCHAR buff[1024];
		length = wsprintf(buff, TEXT("the angle is : %d"), x);
		TextOut(hdc, 20, 20, buff, length);
		EndPaint(hwnd, &ps);
		break;*/

	}
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


