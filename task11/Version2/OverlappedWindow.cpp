#include "OverlappedWindow.h"
#include "EditControlWindow.h"
#include "SettingsDialog.h"
#include "resource.h"
#include <cmath>
#include <string>

COverlappedWindow::COverlappedWindow()
{
	handle = 0;
	isEmpty = true;
	editControl = CEditControlWindow();
	dialog = CSettingsDialog();
	fontColor = RGB(0, 0, 0);
	backgroundColor = RGB(255, 255, 255);
	hasDialog = false;
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
	window.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	window.hIcon = static_cast<HICON>(::LoadImage(window.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		32, 32, LR_DEFAULTCOLOR));
	window.hIconSm = static_cast<HICON>(::LoadImage(window.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		16, 16, LR_DEFAULTCOLOR));
	return (::RegisterClassEx(&window) != 0);
}

bool COverlappedWindow::Create()
{
	handle = CreateWindowEx(WS_EX_LAYERED, L"OverlappedWindow", NULL,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, NULL,
		CW_USEDEFAULT, NULL, HWND(NULL), NULL,
		HINSTANCE(GetModuleHandle(0)), this);
	SetLayeredWindowAttributes(handle, 0, 255, LWA_ALPHA);
	return handle != NULL;
}

void COverlappedWindow::Show(int cmdShow)
{
	wchar_t title[10];
	::LoadString(GetModuleHandle(0), IDS_TITLE, title, sizeof(title) / sizeof(wchar_t));
	::SetWindowText(handle, title);
	HMODULE module = GetModuleHandle(0);
	HRSRC customResource = FindResource(module, MAKEINTRESOURCE(IDR_CUSTOM1), L"CUSTOM");
	HGLOBAL resourceData = LoadResource(module, customResource);
	DWORD size = ::SizeofResource(module, customResource);
	wchar_t* data = static_cast<wchar_t*>(::LockResource(resourceData));
	wchar_t* text = new wchar_t[size / sizeof(wchar_t) + 1];
	memcpy(text, data, size);
	text[size / sizeof(wchar_t)] = L'\0';
	editControl.InitFont();
	::SetWindowText(editControl.GetHandle(), text);
	ShowWindow(handle, cmdShow);
}

HWND COverlappedWindow::GetHandle()
{
	return handle;
}

HWND COverlappedWindow::GetDialogHandle()
{
	return dialog.GetHandle();
}

int COverlappedWindow::GetFontSize()
{
	return editControl.GetFontSize();
}

COLORREF COverlappedWindow::GetFontColor()
{
	return fontColor;
}

COLORREF COverlappedWindow::GetBackgroundColor()
{
	return backgroundColor;
}

void COverlappedWindow::SetFontSize(int size)
{
	editControl.SetFontSize(size);
}

void COverlappedWindow::SetFontColor(COLORREF color)
{
	fontColor = color;
}

void COverlappedWindow::SetBackgroundColor(COLORREF color)
{
	backgroundColor = color;
}

void COverlappedWindow::ResetHasDialog()
{
	hasDialog = false;
}

void COverlappedWindow::OnNCCreate(HWND& _handle)
{
	handle = _handle;
}

void COverlappedWindow::OnCreate()
{
	editControl.Create(handle);
}

void COverlappedWindow::OnSize()
{
	RECT rect;
	::GetClientRect(handle, &rect);
	SetWindowPos(editControl.GetHandle(), 0, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top, 0);
}

LRESULT COverlappedWindow::OnControlColorEdit(HDC hdc)
{
	SetBkColor(hdc, backgroundColor);
	SetDCBrushColor(hdc, backgroundColor);
	SetTextColor(hdc, fontColor);
	return reinterpret_cast<LRESULT>(GetStockObject(DC_BRUSH));
}

void COverlappedWindow::OnTextChange()
{
	if( ::GetWindowTextLength(editControl.GetHandle()) == 0 ) {
		isEmpty = true;
	} else {
		isEmpty = false;
	}
}

void COverlappedWindow::OnCountWords()
{
	wchar_t textLength = SendMessage(editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
	wchar_t* text = new wchar_t[textLength + 1];
	SendMessage(editControl.GetHandle(), WM_GETTEXT, textLength + 1, (LPARAM)text);
	typedef int( CALLBACK*  WORDSCOUNT )( const wchar_t* );
	HINSTANCE dll = 0;
	dll = LoadLibrary( L"WordsCountDll.dll" );
	if( dll != 0 ) {
		WORDSCOUNT wordsCount = reinterpret_cast<WORDSCOUNT>(GetProcAddress(dll, "WordsCount"));
		if( wordsCount != 0 ) {
			int count = wordsCount( text );
			wchar_t buffer[256];
			swprintf_s(buffer, L"%d", count);
			MessageBox( handle, buffer, L"WordsCount", MB_OK );
		}
		FreeLibrary( dll );
	}
}

void COverlappedWindow::OnClose()
{
	if( !isEmpty ) {
		int messageBoxID = ::MessageBox(handle,
			L"You have unsaved text. Do you want to save it?", L"Warning", MB_YESNOCANCEL);
		switch( messageBoxID ) {
		case IDYES:
			saveFile();
		case IDNO:
			DestroyWindow(handle);
			break;
		default:
			return;
		}
	} else {
		DestroyWindow(handle);
	}
}

void COverlappedWindow::OnDestroy()
{
	editControl.DestroyFont();
	PostQuitMessage(0);
}

void COverlappedWindow::saveFile()
{
	OPENFILENAME fileNameStruct;
	WCHAR fileName[MAX_PATH] = L"";
	ZeroMemory(&fileNameStruct, sizeof(fileNameStruct));
	fileNameStruct.lStructSize = sizeof(fileNameStruct);
	fileNameStruct.hwndOwner = handle;
	fileNameStruct.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	fileNameStruct.lpstrFile = fileName;
	fileNameStruct.nMaxFile = MAX_PATH;
	fileNameStruct.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	fileNameStruct.lpstrDefExt = L"txt";
	GetSaveFileName(&fileNameStruct);
	HANDLE fileHandle = CreateFile(fileNameStruct.lpstrFile,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	wchar_t textLength = SendMessage(editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
	wchar_t* text = new wchar_t[textLength + 1];
	SendMessage(editControl.GetHandle(), WM_GETTEXT, textLength + 1, (LPARAM)text);
	DWORD bytesWritten;
	static const BYTE BOM[] = { 0xFF, 0xFE };
	WriteFile(fileHandle, BOM, sizeof(BOM), &bytesWritten, NULL);
	WriteFile(fileHandle, text, textLength*sizeof(wchar_t), &bytesWritten, NULL);
	delete[] text;
	CloseHandle(fileHandle);
}

void COverlappedWindow::showSettings()
{
	if( !hasDialog ) {
		dialog.Create(handle);
		hasDialog = true;
	} 
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	switch( message ) {
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
		windowPtr->OnCreate();
		break;
	}
	case WM_SIZE:
	{
		windowPtr->OnSize();
		break;
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = reinterpret_cast<HDC>(wParam);
		return windowPtr->OnControlColorEdit(hdc);
	}
	case WM_COMMAND:
	{
		if( HIWORD(wParam) == EN_CHANGE ) {
			windowPtr->OnTextChange();
			break;
		}
		switch( LOWORD(wParam) ) {
		case ID_FILE_SAVE:
		{
			windowPtr->saveFile();
			break;
		}
		case ID_FILE_EXIT:
		{
			windowPtr->OnClose();
			break;
		}
		case ID_VIEW_SETTINGS:
		{
			windowPtr->showSettings();
			break;
		}
		case ID_FILE_COUNTWORDS:
		{
			windowPtr->OnCountWords();
			break;
		}
		case ID_KILL:
		{
			COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnDestroy();
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		COverlappedWindow* windowPtr = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		windowPtr->OnClose();
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