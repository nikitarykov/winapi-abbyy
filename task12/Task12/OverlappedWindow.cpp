#include "OverlappedWindow.h"
#include "EditControlWindow.h"
#include "SettingsDialog.h"
#include "resource.h"
#include <cmath>
#include <sstream>
#include <strsafe.h>

COverlappedWindow::COverlappedWindow()
{
	handle = 0;
	isEmpty = true;
	editControl = CEditControlWindow();
	dialog = CSettingsDialog();
	fontColor = RGB(0, 0, 0);
	backgroundColor = RGB(255, 255, 255);
	hasDialog = false;
	for( int i = 0; i < 4; ++i ) {
		processIds[i] = 0;
	}
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

void COverlappedWindow::OnCommand( WPARAM wParam )
{
	if( HIWORD(wParam) == EN_CHANGE ) {
		OnTextChange();
		return;
	}
	switch( LOWORD(wParam) ) {
		case ID_FILE_OPEN:
		{
			openFile();
			break;
		}
		case ID_FILE_SAVE:
		{
			saveFile();
			break;
		}
		case ID_FILE_CLEAR:
		{
			filter();
			break;
		}
		case ID_FILE_EXIT:
		{
			OnClose();
			break;
		}
		case ID_VIEW_SETTINGS:
		{
			showSettings();
			break;
		}
		case ID_KILL:
		{
			OnDestroy();
			break;
		}
	}
}

LRESULT COverlappedWindow::OnControlColorEdit( WPARAM wParam )
{
	HDC hdc = reinterpret_cast<HDC>(wParam);
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
	for( int i = 0; i < 4; ++i ) {
		wchar_t destroyEventName[50];
		swprintf_s( destroyEventName,
			L"Destroy%d", processIds[i] );
		HANDLE destroyEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, destroyEventName );
		SetEvent( destroyEvent );
		CloseHandle( destroyEvent );
	}
	PostQuitMessage(0);
}

void COverlappedWindow::openFile()
{
	OPENFILENAME fileNameStruct;
	WCHAR fileName[MAX_PATH] = L"";
	ZeroMemory(&fileNameStruct, sizeof(fileNameStruct));
	fileNameStruct.lStructSize = sizeof(fileNameStruct);
	fileNameStruct.hwndOwner = handle;
	fileNameStruct.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	fileNameStruct.lpstrFile = fileName;
	fileNameStruct.nMaxFile = MAX_PATH;
	fileNameStruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	fileNameStruct.lpstrDefExt = L"txt";
	GetOpenFileName(&fileNameStruct);
	HANDLE fileHandle = CreateFile(fileNameStruct.lpstrFile,
		GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(fileHandle, 0);
	DWORD bytesRead;
	wchar_t* text = new wchar_t[fileSize / 2 + 1];
	text[fileSize / 2] = 0;
	ReadFile(fileHandle, text, fileSize, &bytesRead, NULL);
	CloseHandle(fileHandle);
	SetWindowText(editControl.GetHandle(), text);
	delete[] text;
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

void COverlappedWindow::filter()
{
	for( int i = 0; i < 4; ++i ) {
		if( processIds[i] == 0 ) {
			STARTUPINFO si;
			ZeroMemory( &si, sizeof( STARTUPINFO ) );
			si.cb = sizeof( STARTUPINFO );
			PROCESS_INFORMATION pi;
			HANDLE readyEvent = CreateEvent( 0, false, false, L"Ready" );
			CreateProcess( L"C:\\Users\\Nikita Rykov\\Documents\\Visual Studio 2015\\Projects\\Task12\\Debug\\WordFilter.exe", 0,
				0, 0, false, 0, 0, 0, &si, &pi );
			processIds[i] = pi.dwProcessId;
			WaitForSingleObject( readyEvent, INFINITE );
			CloseHandle( readyEvent );
		}
	}
	HANDLE textProceededEvents[4];
	for( int i = 0; i < 4; ++i ) {
		wchar_t textProceededEventName[50];
		swprintf_s( textProceededEventName,
			L"TextProceeded%d", processIds[i] );
		textProceededEvents[i] = CreateEvent( 0, false, false, textProceededEventName );
	}
	wchar_t textLength = SendMessage(editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
	wchar_t* text = new wchar_t[textLength + 1];
	SendMessage(editControl.GetHandle(), WM_GETTEXT, textLength + 1, (LPARAM)text);
	int partLength = textLength / 4;
	wchar_t* partPtrs[4];
	for( int i = 0; i < 4; ++i ) {
		partPtrs[i] = text + i * partLength;
	}
	for( int i = 1; i < 4; ++i ) {
		while( *partPtrs[i] != ' ' && *partPtrs[i] != '\n'
			&& *partPtrs[i] != '\0' ) {
			partPtrs[i]++;
		}
	}
	HANDLE mapHandles[4];
	wchar_t* parts[4];
	for( int i = 0; i < 4; ++i ) {
		wchar_t fileName[50];
		swprintf_s(fileName, L"File%d", processIds[i]);
		int partLength;
		if( i == 3 ) {
			partLength = partPtrs[0] + textLength - partPtrs[i];
		} else {
			partLength = partPtrs[i + 1] - partPtrs[i];
		}
		mapHandles[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 0, 
			PAGE_READWRITE, 0, (partLength + 1) * sizeof(wchar_t), fileName);
		parts[i] = static_cast<wchar_t*>(MapViewOfFile(mapHandles[i], 
			FILE_MAP_ALL_ACCESS, 0, 0, 0));
		memcpy(parts[i], partPtrs[i], partLength * sizeof(wchar_t));
		parts[i][partLength] = '\0';
	}
	for( int i = 0; i < 4; ++i ) {
		wchar_t newTextEventName[50];
		swprintf_s(newTextEventName, L"NewText%d", processIds[i]);
		HANDLE newTextEvent = OpenEvent(EVENT_ALL_ACCESS, false, newTextEventName);
		SetEvent(newTextEvent);
		CloseHandle(newTextEvent);
	}
	WaitForMultipleObjects( 4, textProceededEvents, true, INFINITE );
	wchar_t* ptr = text;
	for( int i = 0; i < 4; ++i ) {
		int partLength = wcslen(parts[i]);
		memcpy( ptr, parts[i], partLength * sizeof(wchar_t));
		ptr += partLength;
		CloseHandle(mapHandles[i]);
		CloseHandle(textProceededEvents[i]);
	}
	*ptr = '\0';
	SendMessage(editControl.GetHandle(), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
	delete[] text;
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
		return windowPtr->OnControlColorEdit(wParam);
	}
	case WM_COMMAND:
	{
		windowPtr->OnCommand(wParam);
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