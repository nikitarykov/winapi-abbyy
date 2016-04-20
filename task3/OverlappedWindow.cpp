// Описание: Реализация методов класса СOverlappedWindow

#include "OverlappedWindow.h"

COverlappedWindow::COverlappedWindow()
{
	handle = 0;
	children.resize(4);
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
	handle = CreateWindowEx(0, L"OverlappedWindow", L"Window",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, NULL,
		CW_USEDEFAULT, NULL, HWND(NULL), NULL,
		HINSTANCE(GetModuleHandle(0)), this);
	return handle != NULL;
}

void COverlappedWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	for( size_t i = 0; i < children.size(); ++i ) {
		children[i].Show(cmdShow);
	}
}

void COverlappedWindow::OnNCCreate(HWND& _handle)
{
	handle = _handle;
}

void COverlappedWindow::OnCreate()
{
	CEllipseWindow::RegisterClass();
	for( size_t i = 0; i < children.size(); ++i ) {
		children[i].Create(handle);
	}
}

void COverlappedWindow::OnSize()
{
	RECT rect;
	::GetClientRect(handle, &rect);
	int width = (rect.right - rect.left) / 2;
	int height = (rect.bottom - rect.top) / 2;
	SetWindowPos(children[0].getHandle(), 0, rect.left, rect.top, width, height, 0);
	SetWindowPos(children[1].getHandle(), 0, rect.left + width, rect.top, width, height, 0);
	SetWindowPos(children[2].getHandle(), 0, rect.left, rect.top + height, width, height, 0);
	SetWindowPos(children[3].getHandle(), 0, rect.left + width, rect.top + height, width, height, 0);
}

void COverlappedWindow::OnDestroy()
{
	PostQuitMessage(0);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_NCCREATE:
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
		windowPtr->OnNCCreate(handle);
		return true;
	}
	case WM_CREATE:
	{
		COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnCreate();
		break;
	}
	case WM_SIZE:
	{
		COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnSize();
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
