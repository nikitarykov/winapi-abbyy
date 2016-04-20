// Описание: Реализация методов класса СEllipseWindow

#include "EllipseWindow.h"

CEllipseWindow::CEllipseWindow()
{
	handle = 0;
	timer = 0;
	xSpeed = 5;
	ySpeed = 10;
	x = 0;
	y = 0;
	tickCount = 0;
	paintCount = 0;
}

CEllipseWindow::~CEllipseWindow()
{
}

bool CEllipseWindow::RegisterClass()
{
	WNDCLASSEX window;
	::ZeroMemory(&window, sizeof(window));
	window.cbSize = sizeof(window);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = windowProc;
	window.hInstance = GetModuleHandle(0);
	window.lpszClassName = L"EllipseWindow";
	return (::RegisterClassEx(&window) != 0);
}

bool CEllipseWindow::Create(HWND& handleParent)
{
	handle = CreateWindowEx(0, L"EllipseWindow", L"ChildWindow",
		WS_CHILD | WS_BORDER, CW_USEDEFAULT, NULL,
		CW_USEDEFAULT, NULL, handleParent, NULL,
		HINSTANCE(GetModuleHandle(0)), this);
	return handle != NULL;
}

void CEllipseWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

HWND CEllipseWindow::getHandle()
{
	return handle;
}

void CEllipseWindow::OnNCCreate(HWND& _handle)
{
	handle = _handle;
}

void CEllipseWindow::OnCreate()
{
	SetTimer(handle, timer, 50, 0);
}



void CEllipseWindow::OnPaint()
{
	RECT rect;
	PAINTSTRUCT paintStruct;
	::GetClientRect(handle, &rect);
	HDC handleDC = ::BeginPaint(handle, &paintStruct);
	HDC memoryDC = CreateCompatibleDC(handleDC);
	HBITMAP memoryBuffer = CreateCompatibleBitmap(handleDC, rect.right - rect.left,
		rect.bottom - rect.top);
	HGDIOBJ oldMemoryBuffer = SelectObject(memoryDC, memoryBuffer);
	drawEllipse(memoryDC, rect);
	BitBlt(handleDC, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, memoryDC, 0, 0, SRCCOPY);
	SelectObject(memoryDC, oldMemoryBuffer);
	DeleteObject(memoryBuffer);
	DeleteDC(memoryDC);
	::EndPaint(handle, &paintStruct);
}

void CEllipseWindow::OnTimer()
{
	RECT rect;
	tickCount++;
	::GetClientRect(handle, &rect);
	if (x == 0 && y == 0) {
		x = rect.left + aAxis;
		y = rect.top + bAxis;
	}
	else {
		calcX(rect);
		calcY(rect);
	}
	InvalidateRect(handle, &rect, FALSE);
}

void CEllipseWindow::OnButtonDown()
{
	::SetFocus(handle);
}

void CEllipseWindow::OnDestroy()
{
	KillTimer(handle, timer);
	PostQuitMessage(0);

}

void CEllipseWindow::drawEllipse(HDC & memoryDC, RECT & rect)
{
	FillRect(memoryDC, &rect, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));
	HBRUSH hBrush;
	if( handle == ::GetFocus() ) {
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
	} else {
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
	}
	HPEN hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(memoryDC, hBrush));
	HPEN oldPen = static_cast<HPEN>(SelectObject(memoryDC, hPen));
	Ellipse(memoryDC, x - aAxis, y - bAxis, x + aAxis, y + bAxis);
	SelectObject(memoryDC, oldBrush);
	SelectObject(memoryDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void CEllipseWindow::calcX(const RECT & rect)
{
	x += xSpeed;
	if( x >= rect.right - aAxis ) {
		x = rect.right - aAxis;
		xSpeed = -xSpeed;
	} else if( x <= rect.left + aAxis ) {
		x = rect.left + aAxis;
		xSpeed = -xSpeed;
	}
}

void CEllipseWindow::calcY(const RECT & rect)
{
	y += ySpeed;
	if( y >= rect.bottom - bAxis ) {
		y = rect.bottom - bAxis;
		ySpeed = -ySpeed;
	} else if( y <= rect.top + bAxis ) {
		y = rect.top + bAxis;
		ySpeed = -ySpeed;
	}
}

LRESULT CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) {
	case WM_NCCREATE:
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		CEllipseWindow* windowPtr = reinterpret_cast<CEllipseWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
		windowPtr->OnNCCreate(handle);
		return true;
	}
	case WM_CREATE:
	{
		CEllipseWindow* windowPtr = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnCreate();
		break;
	}
	case WM_PAINT:
	{
		CEllipseWindow* windowPtr = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnPaint();
		break;
	}
	case WM_TIMER:
	{
		CEllipseWindow* windowPtr = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnTimer();
		break;
	}
	case WM_LBUTTONDOWN: 
	{
		CEllipseWindow* windowPtr = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnButtonDown();
		break;
	}
	case WM_DESTROY:
	{
		CEllipseWindow* windowPtr = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnDestroy();
		break;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return false;
}
