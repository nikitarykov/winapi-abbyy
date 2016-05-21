#include "CardWindow.h"
#include "Pile.h"
#include "resource.h"
#include <strsafe.h>
#include <windowsx.h>

static const int ID_CARD_CHILD = 100;

CCardWindow::CCardWindow()
{
	id = 13;
	handle = 0;
	isOpen = false;
	pile = 0;
	pileIndex = 0;
	isDragged = false;
	mousePosX = 0;
	mousePosY = 0;
}

CCardWindow::CCardWindow(int _id)
{
	id = _id;
	handle = 0;
	isOpen = false;
	pile = 0;
	pileIndex = 0;
	isDragged = false;
}

CCardWindow::~CCardWindow()
{
}

bool CCardWindow::RegisterClass()
{
	WNDCLASSEX window;
	::ZeroMemory(&window, sizeof(window));
	window.cbSize = sizeof(window);
	window.style = CS_PARENTDC;
	window.lpfnWndProc = windowProc;
	window.hInstance = GetModuleHandle(0);
	window.lpszClassName = L"CardWindow";
	return (::RegisterClassEx(&window) != 0);
}

bool CCardWindow::Create(HWND& handleParent)
{
	handle = CreateWindowEx(0, L"CardWindow", NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, -71, -96,
		71, 96, handleParent, HMENU(ID_CARD_CHILD),
		HINSTANCE(GetModuleHandle(0)), this);
	return (handle != 0);
}

void CCardWindow::Show(int commandShow)
{
	ShowWindow(handle, commandShow);
}

HWND& CCardWindow::GetHandle()
{
	return handle;
}

bool CCardWindow::IsOpen()
{
	return isOpen;
}

void CCardWindow::Open()
{
	isOpen = true;
}

void CCardWindow::Close()
{
	isOpen = false;
	RedrawWindow( handle, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN );
}

void CCardWindow::SetPile( CPile* _pile )
{
	pile = _pile;
}

CPile * CCardWindow::GetPile()
{
	return pile;
}

int CCardWindow::GetId()
{
	return id;
}
	
void CCardWindow::SetPileIndex( int index )
{
	pileIndex = index;
}

int CCardWindow::GetPileIndex()
{
	return pileIndex;
}

void CCardWindow::SetDragged()
{
	isDragged = true;
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

void CCardWindow::OnPaint()
{
	RECT rect;
	PAINTSTRUCT paintStruct;
	::GetClientRect(handle, &rect);
	HDC handleDC = ::BeginPaint(handle, &paintStruct);
	HDC memoryDC = CreateCompatibleDC(handleDC);
	HBITMAP memoryBuffer = CreateCompatibleBitmap(handleDC, rect.right - rect.left,
		rect.bottom - rect.top);
	HGDIOBJ oldMemoryBuffer = SelectObject(memoryDC, memoryBuffer);
	HGDIOBJ oldBitmap;
	BITMAP bitmap;
	if( isOpen ) {
		GetObject(frontBitMaps[id], sizeof(bitmap), &bitmap);
		oldBitmap = SelectObject(memoryDC, frontBitMaps[id]);
	} else {
		GetObject(backBitMap, sizeof(bitmap), &bitmap);
		oldBitmap = SelectObject(memoryDC, backBitMap);
	}
	BitBlt(handleDC, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, memoryDC, 0, 0, SRCCOPY);
	SelectObject(memoryDC, oldMemoryBuffer);
	DeleteObject(memoryBuffer);
	DeleteDC(memoryDC);
	EndPaint(handle, &paintStruct);
}

void CCardWindow::OnButtonDown( LPARAM lParam )
{
	mousePosX = LOWORD(lParam);
    mousePosY = HIWORD(lParam);
	if( pile != 0 ) {
		pile->OnButtonDown( pileIndex );
	}
}

void CCardWindow::OnMouseMove( LPARAM lParam )
{
	if( isDragged ) {
		//RECT rect;
		//GetClientRect(handle ,&rect);
		int x = GET_X_LPARAM( lParam );
        int y = GET_Y_LPARAM(lParam);
		POINT point = { x, y };
		MapWindowPoints( handle, GetParent( handle ), reinterpret_cast<LPPOINT>( &point ), 1 );
		pile->OnMouseMove( point.x - mousePosX, point.y - mousePosY);
	}
}

void CCardWindow::OnButtonUp( LPARAM lParam )
{ 
	if( isDragged ) {
		ReleaseCapture();
	}
}

void CCardWindow::OnCaptureChanged( LPARAM lParam )
{
	if( isDragged ) {
		isDragged = false;
		RECT rect;
		GetClientRect( handle, &rect );
		MapWindowPoints( handle, GetParent( handle ), reinterpret_cast<LPPOINT>( &rect ), 2 );
		pile->OnCaptureChanged( rect );
	}
}

void CCardWindow::OnDestroy()
{
	//PostQuitMessage(0);
}

void CCardWindow::drawCard( HDC handleDC )
{
	BITMAP bitmap;
	if( isOpen ) {
		GetObject(frontBitMaps[id], sizeof(bitmap), &bitmap);
		HGDIOBJ oldBitmap = SelectObject(handleDC, frontBitMaps[id]);
	} else {
		GetObject(backBitMap, sizeof(bitmap), &bitmap);
		HGDIOBJ oldBitmap = SelectObject(handleDC, backBitMap);
	}
}

LRESULT CCardWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	switch( message ) {
		case WM_NCCREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast< CREATESTRUCT* >( lParam );
			CCardWindow* windowPtr = reinterpret_cast< CCardWindow* >( pCreate->lpCreateParams );
			SetWindowLongPtr( handle, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( windowPtr ) );
			windowPtr->OnNCCreate( handle );
			return true;
		}
		case WM_CREATE:
		{
			//CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnCreate();
			break;
		}
		case WM_DESTROY:
		{
			//CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnDestroy();
			break;
		}
		case WM_PAINT:
		{
			//CCardWindow* windowPtr = reinterpret_cast<CCardWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
			windowPtr->OnPaint();
			break;
		}
		case WM_LBUTTONDOWN:
		{
			windowPtr->OnButtonDown( lParam );
			break;
		}
		
		case WM_MOUSEMOVE:
		{
			windowPtr->OnMouseMove( lParam );
			break;
		}
		case WM_LBUTTONUP:
		{
			windowPtr->OnButtonUp( lParam );
			break;
		}
		case WM_CAPTURECHANGED:
		{
			windowPtr->OnCaptureChanged( lParam );
		}
		/*
		case WM_NCHITTEST: {
				LRESULT hit = DefWindowProc( handle, message, wParam, lParam );
				if( hit == HTCLIENT ) {
					hit = HTCAPTION;
				}
				return hit;
			
		}*/
		default:
			return DefWindowProc(handle, message, wParam, lParam);
	}
	return false;
}

const HBITMAP CCardWindow::frontBitMaps[14] = { 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_ACE ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_TWO ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_THREE ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_FOUR ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_FIVE ) ),
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_SIX ) ),
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_SEVEN ) ),
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_EIGHT ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_NINE ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_TEN ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_JACK ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_QUEEN ) ), 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_KING ) ),
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_EMPTY ) ) };

const HBITMAP CCardWindow::backBitMap = 
	LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_BACK ) );
