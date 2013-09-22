/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013, Sandaruwan Silva <c-h [-a-t] users [-dot-] sf [-dot-] net>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

#include <windows.h>

LRESULT CALLBACK callback_window_main(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 

#define application_title L"Soundbot"

HINSTANCE application_instance = 0;
HWND hwnd_main = 0;


/* main */


int wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nCmdShow) 
{ 
	WNDCLASS            wc; 
	HWND                hwnd; 
	MSG                 msg; 
	
	application_instance = hInst;

	wc.style         = CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc   = callback_window_main; 
	wc.cbClsExtra    = 0; 
	wc.cbWndExtra    = 0; 
	wc.hInstance     = hInst; 
	wc.hIcon         = LoadIcon(hInst,(LPCTSTR)1); 
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0xd9, 0xe3, 0xec)); 
	wc.lpszMenuName  = NULL; 
	wc.lpszClassName = application_title; 

	if(!RegisterClass(&wc)) return 0; 

	hwnd_main = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_APPWINDOW, application_title, application_title, 
		WS_THICKFRAME | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPED, 
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 500, NULL, NULL, hInst, NULL); 

	if (!hwnd_main) return 0; 

	ShowWindow(hwnd_main, nCmdShow); 
	UpdateWindow(hwnd_main); 

 
	while (GetMessage(&msg,NULL,0,0) > 0) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 

	return 0;
} 


LRESULT CALLBACK callback_window_main(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
	switch (msg) 
	{ 
		case WM_PAINT: 
		{ 
			PAINTSTRUCT ps; 
			HDC hdc; 
			RECT r; 
			
			GetClientRect(hwnd,&r); 
			hdc = BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
			break; 
		} 

		case WM_DESTROY: 
			PostQuitMessage(0); 
			break; 

		case WM_LBUTTONDOWN:
			
			break;

		case WM_LBUTTONUP:
			
			break;

		case WM_MOUSEMOVE:
			
			break;


		default: 
			return DefWindowProc(hwnd, msg, wparam, lparam); 
	} 
	return 0; 
} 
