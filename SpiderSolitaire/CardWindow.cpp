#include "CardWindow.h"
#include "resource.h"
#include <strsafe.h>

static const int ID_CARD_CHILD = 100;

CCardWindow::CCardWindow()
{
	
}

CCardWindow::~CCardWindow()
{
}

bool CCardWindow::RegisterClass()
{
	WNDCLASSEX window;
	::ZeroMemory(&window, sizeof(window));
	window.cbSize = sizeof(window);
	window.style = CS_OWNDC;
	window.lpfnWndProc = windowProc;
	window.hbrBackground = 0;
	window.hInstance = GetModuleHandle(0);
	window.lpszClassName = L"CardWindow";
	return (::RegisterClassEx(&window) != 0);
}

bool CCardWindow::Create(HWND& handleParent)
{
	handle = CreateWindowEx(0, L"CardWindow", NULL,
		WS_CHILD | WS_VISIBLE, 10, 10,
		71, 96, handleParent, HMENU(ID_CARD_CHILD),
		HINSTANCE(GetModuleHandle(0)), this);
	return (handle != 0);
}

void CCardWindow::Show(int commandShow)
{
	ShowWindow(handle, commandShow);
}

HWND CCardWindow::GetHandle()
{
	return handle;
}

void CCardWindow::OnNCCreate(HWND& _handle)
{
	handle = _handle;
}

void CCardWindow::OnCreate()
{
}

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

void CCardWindow::OnDrawBackground()
{
	PAINTSTRUCT paintStruct;
	HDC hdc = BeginPaint(handle, &paintStruct);
	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP hBack = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BACK));
	BITMAP bitmap;
	GetObject(hBack, sizeof(bitmap), &bitmap);
	HGDIOBJ oldBitmap = SelectObject(memoryDC, hBack);
	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, memoryDC, 0, 0, SRCCOPY);
	SelectObject(memoryDC, oldBitmap);
	DeleteDC(memoryDC);
	EndPaint(handle, &paintStruct);
}

void CCardWindow::OnDestroy()
{
	PostQuitMessage(0);
}

LRESULT CCardWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) {
		case WM_NCCREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
			windowPtr->OnNCCreate(handle);
			return true;
		}
		case WM_CREATE:
		{
			CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnCreate();
			break;
		}
		case WM_DESTROY:
		{
			CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnDestroy();
			break;
		}
		case WM_ERASEBKGND:
		{
			CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnDrawBackground();
			return false;
		}
		default:
			return DefWindowProc(handle, message, wParam, lParam);
	}
	return false;
}
