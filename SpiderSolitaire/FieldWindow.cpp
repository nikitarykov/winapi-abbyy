#include "FieldWindow.h"
#include "resource.h"

CFieldWindow::CFieldWindow()
{
	handle = 0;
	cards.resize(10);
}

CFieldWindow::~CFieldWindow()
{
}

bool CFieldWindow::RegisterClass()
{
	WNDCLASSEX window;
	::ZeroMemory(&window, sizeof(window));
	window.cbSize = sizeof(window);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = windowProc;
	window.hbrBackground = CreateSolidBrush(RGB(0, 127, 64));
	window.hInstance = GetModuleHandle(0);
	window.lpszClassName = L"FieldWindow";
	window.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	window.hIcon = static_cast<HICON>(::LoadImage(window.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		32, 32, LR_DEFAULTCOLOR));
	window.hIconSm = static_cast<HICON>(::LoadImage(window.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		16, 16, LR_DEFAULTCOLOR));
	return (::RegisterClassEx(&window) != 0);
}

bool CFieldWindow::Create()
{
	handle = CreateWindowEx(0, L"FieldWindow", NULL,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, NULL,
		CW_USEDEFAULT, NULL, HWND(NULL), NULL,
		HINSTANCE(GetModuleHandle(0)), this);
	return (handle != 0);
}

void CFieldWindow::Show(int commandShow)
{
	wchar_t title[20];
	::LoadString(GetModuleHandle(0), IDS_TITLE, title, sizeof(title) / sizeof(wchar_t));
	::SetWindowText(handle, title);
	ShowWindow(handle, commandShow);
}

void CFieldWindow::OnNCCreate(HWND & _handle)
{
	handle = _handle;
}

void CFieldWindow::OnCreate()
{
	CCardWindow::RegisterClass();
	for( int i = 0; i < cards.size(); ++i ) {
		cards[i].Create(handle);
	}
}

void CFieldWindow::OnSize()
{
	RECT rect;
	::GetClientRect(handle, &rect);
	int width = (rect.right - rect.left) / cards.size();
	int left = (width - 71) / 2;
	if( left < 0 ) {
		left = 0;
	}
	for( int i = 0; i < cards.size(); ++i ) {
		SetWindowPos(cards[i].GetHandle(), 0, left + width * i, 10,
			71, 96, 0);
	}
}

void CFieldWindow::OnClose()
{
}

void CFieldWindow::OnDestroy()
{
	PostQuitMessage(0);
}

LRESULT CFieldWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) {
		case WM_NCCREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
			windowPtr->OnNCCreate(handle);
			return true;
		}
		case WM_CREATE:
		{
			CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnCreate();
			break;
		}
		case WM_SIZE:
		{
			CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnSize();
			break;
		}
		/*case WM_CLOSE:
		{
			CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnClose();
			break;
		}*/
		case WM_DESTROY:
		{
			CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnDestroy();
			break;
		}
		default:
			return DefWindowProc(handle, message, wParam, lParam);
	}
	return false;
}
