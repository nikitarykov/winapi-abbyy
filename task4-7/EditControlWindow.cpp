// ��������: ���������� ������� ������ �EditControlWindow

#include "EditControlWindow.h"

static const int ID_EDITCHILD = 100;

CEditControlWindow::CEditControlWindow()
{
	handle = 0;
}

CEditControlWindow::~CEditControlWindow()
{
}

bool CEditControlWindow::Create(HWND& handleParent)
{
	handle = CreateWindowEx(0, L"Edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE, 
		CW_USEDEFAULT, NULL, CW_USEDEFAULT, NULL, 
		handleParent, HMENU(ID_EDITCHILD),
		HINSTANCE(GetModuleHandle(0)), this);
	return handle != NULL;
}

HWND CEditControlWindow::GetHandle()
{
	return handle;
}

void CEditControlWindow::InitFont()
{
	HFONT font = CreateFont(20, 0, 0, 0, FW_DONTCARE, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");
	SendMessage(handle, WM_SETFONT, reinterpret_cast<WPARAM>(font), true);
}

void CEditControlWindow::DestroyFont()
{
	HFONT font = reinterpret_cast<HFONT>(SendMessage(handle, WM_GETFONT, 0, 0));
	SendMessage(handle, WM_SETFONT,
		reinterpret_cast<WPARAM>(GetStockObject(SYSTEM_FONT)), true);
	DeleteObject(font);
}

int CEditControlWindow::GetFontSize()
{
	HFONT oldFont = reinterpret_cast<HFONT>(SendMessage(handle, WM_GETFONT, 0, 0));
	LOGFONT fontAttributes;
	::GetObject(oldFont, sizeof(fontAttributes), &fontAttributes);
	return fontAttributes.lfHeight;
}

void CEditControlWindow::SetFontSize(int size)
{
	HFONT oldFont = reinterpret_cast<HFONT>(SendMessage(handle, WM_GETFONT, 0, 0));
	LOGFONT fontAttributes;
	::GetObject(oldFont, sizeof(fontAttributes), &fontAttributes);
	fontAttributes.lfHeight = size;
	HFONT font = ::CreateFontIndirect(&fontAttributes);
	SendMessage(handle, WM_SETFONT, reinterpret_cast<WPARAM>(font), true);
	DeleteObject(oldFont);
}

void CEditControlWindow::OnNCCreate(HWND& _handle)
{
	handle = _handle;
}

void CEditControlWindow::OnCreate()
{
}

void CEditControlWindow::OnDestroy()
{
	PostQuitMessage(0);
}

LRESULT CEditControlWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) {
	case WM_NCCREATE:
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		CEditControlWindow* windowPtr = reinterpret_cast<CEditControlWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
		windowPtr->OnNCCreate(handle);
		return true;
	}
	case WM_CREATE:
	{
		CEditControlWindow* windowPtr = reinterpret_cast<CEditControlWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnCreate();
		break;
	}
	case WM_DESTROY:
	{
		CEditControlWindow* windowPtr = reinterpret_cast<CEditControlWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnDestroy();
		break;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return false;
}


