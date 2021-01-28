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

//��ʼ��
void WheelInit(HWND hwnd)
{


	if (LogiSteeringInitializeWithWindow(true, hwnd))
	{

		printf("init secuss\n");
		//printf("%d\n", hwnd);
		while (LogiUpdate() && LogiIsConnected(0))//�豸0
		{
			//printf("connect secuss\n");
			Sleep(100);//100
			DIJOYSTATE2 * wheel = LogiGetState(0);//�豸0
			
			//����Ƕȣ����ţ�ɲ����Ϣ
			printf("�����̽Ƕ� = %d  ���� = %d  ɲ�� = %d\n", wheel->lX, wheel->lY, wheel->lRz);
			Sleep(1000);
		}

	}
	else
	{
		printf("init faild");
	}

}


// ����Ҫ����ǰ������  
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
	// ����  
	TCHAR cls_Name[] = TEXT("My Class");//TCHAR cls_Name[] = L"My Class";
	// ��ƴ�����  
	WNDCLASS wc = { sizeof(WNDCLASS) };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = cls_Name;
	wc.hInstance = hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	// ע�ᴰ����  
	RegisterClass(&wc);

	// ��������
	HWND hwnd = CreateWindow(
		cls_Name,           //������Ҫ�͸ղ�ע���һ��  
		TEXT("������Demo"),          //���ڱ������� L"������Demo"
		WS_OVERLAPPEDWINDOW,        //���������ʽ  
		38,             //��������ڸ�����X����  
		20,             //��������ڸ�����Y����  
		500,                //���ڵĿ��  
		500,                //���ڵĸ߶�  
		NULL,               //û�и����ڣ�ΪNULL  
		NULL,               //û�в˵���ΪNULL  
		hInstance,          //��ǰӦ�ó����ʵ�����  
		NULL);              //û�и������ݣ�ΪNULL  
	if (hwnd == NULL)                //��鴰���Ƿ񴴽��ɹ�  
		return 0;

	// ��ʾ����  
	ShowWindow(hwnd, SW_SHOW);

	// ���´���  
	UpdateWindow(hwnd);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //���ô�����ǰ��
	thread GetWheelData(WheelInit, hwnd);
	GetWheelData.detach();
	//MessageBox(0, "������WinMain����", "���ԣ�", 0);


	// ��Ϣѭ��  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// ��WinMain��ʵ��  
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


