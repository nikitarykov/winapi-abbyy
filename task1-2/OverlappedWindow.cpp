// ŒÔËÒ‡ÌËÂ: –Â‡ÎËÁ‡ˆËˇ ÏÂÚÓ‰Ó‚ ÍÎ‡ÒÒ‡ —OverlappedWindow

#include "OverlappedWindow.h"

COverlappedWindow::COverlappedWindow() 
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

COverlappedWindow::~COverlappedWindow()
{
}

bool COverlappedWindow::RegisterClass() 
{
	WNDCLASSEX window;
	::ZeroMemory(&window, sizeof(window));
	window.cbSize = sizeof(window);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = windowProc;
	window.hInstance = GetModuleHandle(0);
	window.lpszClassName = L"OverlappedWindow";
	return (::RegisterClassEx(&window) != 0);
}

bool COverlappedWindow::Create()
{
	handle = CreateWindowEx(0, L"OverlappedWindow",	L"Window",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,	NULL,
		CW_USEDEFAULT, NULL, HWND(NULL), NULL,
		HINSTANCE(GetModuleHandle(0)), this);
	return handle != NULL;
}

void COverlappedWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

void COverlappedWindow::OnNCCreate(HWND& _handle)
{
	handle = _handle;
}

void COverlappedWindow::OnCreate()
{
	SetTimer(handle, timer, 50, 0);
}



void COverlappedWindow::OnPaint()
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

void COverlappedWindow::OnTimer()
{
	RECT rect;
	tickCount++;
	::GetClientRect(handle, &rect);
	if( x == 0 && y == 0 ) {
		x = rect.left + aAxis;
		y = rect.top + bAxis;
	} else {
		calcX(rect);
		calcY(rect);
	}
	InvalidateRect(handle, &rect, FALSE);
}

void COverlappedWindow::OnDestroy()
{
	KillTimer(handle, timer);
	PostQuitMessage(0);

}

void COverlappedWindow::drawEllipse(HDC & memoryDC, RECT & rect)
{
	FillRect(memoryDC, &rect, static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)));
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	HPEN hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(memoryDC, hBrush));
	HPEN oldPen = static_cast<HPEN>(SelectObject(memoryDC, hPen));
	Ellipse(memoryDC, x - aAxis, y - bAxis, x + aAxis, y + bAxis);
	SelectObject(memoryDC, oldBrush);
	SelectObject(memoryDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void COverlappedWindow::calcX(const RECT & rect)
{
	x += xSpeed;
	if( x >= rect.right - aAxis ) {
		x = rect.right - aAxis;
		xSpeed = -xSpeed;
	}
	else if( x <= rect.left + aAxis ) {
		x = rect.left + aAxis;
		xSpeed = -xSpeed;
	}
}

void COverlappedWindow::calcY(const RECT & rect)
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

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) {
		case WM_NCCREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
			//windowPtr->OnNCCreate(handle);
			return true;
		}
		case WM_CREATE:
		{
			COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnCreate();
			break;
		}
		case WM_PAINT:
		{
			COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnPaint();
			break;
		}
		case WM_TIMER:
		{
			COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnTimer();
			break;
		}
		case WM_DESTROY:
		{
			COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnDestroy();
			break;
		}
		default:
			return DefWindowProc(handle, message, wParam, lParam);
	}
	return false;
}
