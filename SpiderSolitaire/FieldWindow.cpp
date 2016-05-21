#include "FieldWindow.h"
#include "resource.h"
#include <algorithm>
#include <random>

CFieldWindow::CFieldWindow()
{
	handle = 0;
	tickCount = 0;
	isMovingBack = false;
	startPositions = std::vector<int>();
	values = std::vector<int>();
	tableauPiles = std::vector<std::unique_ptr<CTableauPile> >();
	movingCards = std::vector<std::unique_ptr<CCardWindow> >();
	tickCountForMovingCards = std::deque<int>();
	dragPile = 0;
	prevPile = 0;
	stockPile = new CStockPile();
	wastePile = new CWastePile();
	steps = std::vector<std::unique_ptr<CState> >();
}

CFieldWindow::~CFieldWindow()
{
	delete stockPile;
	delete wastePile;
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
	window.hCursor = LoadCursor(0, IDC_ARROW);
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

void CFieldWindow::HandOut()
{
	for( int i = 0; i < 10; ++i ) {
		if( tableauPiles[i].get()->GetSize() == 0 ) {
			return;
		}
	}
	SetTimer(handle, 0, 30, handingOutCardsProc);
}

void CFieldWindow::SetDragPile( std::unique_ptr<CDragPile>& _dragPile )
{
	dragPile.reset( _dragPile.get() );
	_dragPile.release();
}

void CFieldWindow::DropPile( std::unique_ptr<CDragPile>& _dragPile, RECT& rect )
{
	int bottomCardX = rect.left;
	int bottomCardY = rect.top;
	CTableauPile* target = 0;
	int overlapArea = 0;
	dragPile.reset( _dragPile.get() );
	_dragPile.release();
	for( int i = 0; i < 10; ++i ) {
		int x, y;
		int width = 0;
		int height = 0;
		tableauPiles[i].get()->GetTopCardPos( x, y );
		if( bottomCardX <= x && x <= bottomCardX + 71 ) {
			width = bottomCardX + 71 - x;
		}
		if( x <= bottomCardX && bottomCardX <= x + 71 ) {
			width = x + 71 - bottomCardX;
		}
		if( bottomCardY <= y && y <= bottomCardY + 71 ) {
			height = bottomCardY + 71 - y;
		}
		if( y <= bottomCardY && bottomCardY <= y + 71 ) {
			height = y + 71 - bottomCardY;
		}
		if( height * width > overlapArea && height * width >= 852 && 
			tableauPiles[i].get()->CheckForDrop( dragPile.get()->GetId() ) ) {
			target = tableauPiles[i].get();
		}
	}
	prevPile = dynamic_cast< CTableauPile* >( dragPile->GetPrevPile() );
	if( target == 0 || target == prevPile ) {
		SetTimer(handle, 0, 10, backDropProc);
	} else {
		std::unique_ptr<CState> ptr = std::unique_ptr<CState>( new CState() );
		ptr.get()->SetPrevPile( prevPile );
		ptr.get()->SetCurPile( target );
		ptr.get()->SetCurPileIndex( target->GetSize() );
		ptr.get()->SetWasPrevPileClosed( !prevPile->IsOpen() );
		steps.push_back( std::move( ptr ) );
		dragPile->PopPile( movingCards );
		prevPile->CheckForOpen();
		target->AddPile( movingCards );
		dragPile.release();
		prevPile->SetIsDragging( false );
		target->SetIsDragging( false );
		if( target->CheckForComplete() ) {
			prevPile = target;
			Complete();
		}
	}
}

void CFieldWindow::Complete()
{
	SetTimer( handle, 0, 30, completeProc );
}

int myRandom (int i) 
{ 
	return std::rand() % i;
}

void CFieldWindow::InitField( bool isRestart )
{
	for( int i = 0; i < tableauPiles.size(); ++i ) {
		tableauPiles[i].get()->Clear();
	}
	steps.clear();
	tableauPiles.clear();
	stockPile->Clear();
	wastePile->Clear();
	values.clear();
	InvalidateRect( handle, 0, true );
	if( !isRestart ) {
		for( int i = 0; i < 8; ++i ) {
			for( int j = 0; j < 13; ++j ) {
				values.push_back( j );
			}
		}
		std::random_shuffle( values.begin(), values.end(), myRandom );
		startPositions = values;
	} else {
		values = startPositions;
	}
	stockPile->SetWindow( this );
	wastePile->SetWindow( this );
	for( int i = 0; i < 10; ++i ) {
		std::unique_ptr<CTableauPile> ptr(new CTableauPile());
		tableauPiles.push_back( std::move(ptr) );
		tableauPiles[i].get()->SetWindow( this );
		tableauPiles[i].get()->CreateEmptyBox( handle );
	}
	for( int i = 0; i < 11; ++i ) {
		std::unique_ptr<CCardWindow> ptr(new CCardWindow());
		ptr.get()->Create(handle);
		ptr.get()->SetPile( stockPile );
		stockPile->AddCard( std::move(ptr) );
	}
	SendMessage(handle, WM_SIZE, 0, 0);
	RedrawWindow( handle, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN );
	SetTimer(handle, 0, 30, startHandingOutCardsProc);
}

void CFieldWindow::OnNCCreate(HWND & _handle)
{
	handle = _handle;
}

void CFieldWindow::OnCreate()
{
	CCardWindow::RegisterClass();
	InitField(false);
}

void CFieldWindow::OnGetMinMaxInfo(LPARAM lParam)
{
	LPMINMAXINFO lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 500;
}

void CFieldWindow::OnCommand( WPARAM wParam )
{
	switch( LOWORD( wParam ) ) {
		case ID_NEW_GAME:
		{
			InitField( false );
			break;
		}
		case ID_RESTART:
		{
			InitField( true );
			break;
		}
		case ID_STEP_BACK:
		{
			stepBack();
			break;
		}
		case ID_RULES:
		{
			showRules();
			break;
		}
	}
}

void CFieldWindow::OnSize()
{
	RECT rect;
	::GetClientRect(handle, &rect);
	int width = (rect.right - rect.left) / 10;
	int left = (width - 71) / 2;
	if( left < 0 ) {
		left = 0;
	}
	stockPile->SetBottomCardPos( rect.right - left - 71, rect.bottom - 106 );
	for( int i = 0; i < 10; ++i ) {
		tableauPiles[i].get()->SetBottomCardPos( left + width * i, 10 );
	}
	wastePile->SetBottomCardPos( left, rect.bottom - 106 );
}

void CFieldWindow::OnDestroy()
{
	PostQuitMessage(0);
}

void CFieldWindow::showRules()
{
	wchar_t title[10];
	::LoadString(GetModuleHandle(0), IDS_TITLE, title, sizeof(title) / sizeof(wchar_t));
	::SetWindowText(handle, title);
	HMODULE module = GetModuleHandle(0);
	HRSRC customResource = FindResource(module, MAKEINTRESOURCE(IDR_RULES), L"RULES");
	HGLOBAL resourceData = LoadResource(module, customResource);
	DWORD size = ::SizeofResource(module, customResource);
	wchar_t* data = static_cast<wchar_t*>(::LockResource(resourceData));
	wchar_t* text = new wchar_t[size / sizeof(wchar_t) + 1];
	memcpy(text, data, size);
	text[size / sizeof(wchar_t)] = L'\0';
	MessageBox( handle, text, L"Правила", MB_OK );
}

void CFieldWindow::stepBack()
{
	if( !isMovingBack && !steps.empty() ) {
		isMovingBack = true;
		std::unique_ptr<CState> ptr = std::move( steps.back() );
		prevPile = ptr.get()->GetPrevPile();
		steps.pop_back();
		if( ptr.get()->GetWasPrevPileClosed() ) {
			prevPile->CloseTop();
		}
		CTableauPile* curPile = ptr.get()->GetCurPile();
		curPile->PopPile( ptr.get()->GetCurPileIndex() );
		curPile->SendDragPileToWindow();
		SetTimer( handle, 0, 30, backDropProc );
	}
}

void CFieldWindow::handingOutCardsProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime )
{
	CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	int stockPileTopCardX, stockPileTopCardY;
	windowPtr->steps.clear();
	windowPtr->stockPile->GetTopCardPos( stockPileTopCardX, stockPileTopCardY );
	if( windowPtr->tickCount % 2 == 0 && windowPtr->tickCount / 2 < 10 ) {
		int index = windowPtr->tickCount / 2;
		std::unique_ptr<CCardWindow> ptr( 
			new CCardWindow( windowPtr->values.back() ) );
		ptr.get()->Create( handle );
		windowPtr->values.pop_back();
		ptr.get()->Open();
		ptr.get()->SetPile( windowPtr->tableauPiles[index % 10].get() );
		windowPtr->movingCards.push_back( std::move( ptr ) );
		windowPtr->tickCountForMovingCards.push_back( windowPtr->tickCount);
	}
	if( windowPtr->tickCount == 18 ) {
		windowPtr->stockPile->PopCard();
	}
	for( size_t i = 0; i < windowPtr->movingCards.size(); ++i ) {
		int tableauPileNextCardX, tableauPileNextCardY;
		windowPtr->movingCards[i].get()->GetPile()->
			GetNextCardPos( tableauPileNextCardX, tableauPileNextCardY );
		SetWindowPos(windowPtr->movingCards[i].get()->GetHandle(), HWND_TOP, 
			stockPileTopCardX + (tableauPileNextCardX - stockPileTopCardX) * 
			(windowPtr->tickCount - windowPtr->tickCountForMovingCards[i]) / 10, 
			stockPileTopCardY + (tableauPileNextCardY - stockPileTopCardY) * 
			(windowPtr->tickCount - windowPtr->tickCountForMovingCards[i]) / 10, 
			71, 96, 0);
	}
	if( windowPtr->tickCount - windowPtr->tickCountForMovingCards[0] == 10 ) {
		windowPtr->movingCards[0]->GetPile()->
			AddCard( std::move( windowPtr->movingCards[0] ) );
		windowPtr->movingCards.erase(windowPtr->movingCards.begin());
		windowPtr->tickCountForMovingCards.pop_front();
	}
	windowPtr->tickCount++;
	if( windowPtr->movingCards.empty() ) {
		KillTimer( handle, 0 );
		windowPtr->tickCount = 0;
		windowPtr->tickCountForMovingCards.clear();
		for( int i = 0; i < 10; ++i ) {
			if( windowPtr->tableauPiles[i].get()->CheckForComplete() ) {
				windowPtr->prevPile = windowPtr->tableauPiles[i].get();
				windowPtr->Complete();
			}
		}
	}
}

void CFieldWindow::backDropProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime )
{
	CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	int dragPileBottomCardX, dragPileBottomCardY;
	windowPtr->dragPile.get()->GetBottomCardPos( dragPileBottomCardX, dragPileBottomCardY );
	int targetNextCardX, targetNextCardY;
	windowPtr->prevPile->GetNextCardPos( targetNextCardX, targetNextCardY );
	if( windowPtr->tickCount % 2 == 0 && windowPtr->tickCount / 2 < 6 ) {
		int time = windowPtr->tickCount / 2;
		windowPtr->dragPile.get()->SetBottomCardPos(
			dragPileBottomCardX + ( targetNextCardX - dragPileBottomCardX ) / 5 * time,
			dragPileBottomCardY + ( targetNextCardY - dragPileBottomCardY ) / 5 * time );
		if( time == 5 ) {
			std::vector< std::unique_ptr<CCardWindow> > cards;
			windowPtr->prevPile->SetIsDragging( false );
			windowPtr->dragPile.get()->PopPile( cards );
			windowPtr->prevPile->AddPile( cards );
			windowPtr->dragPile.release();
			windowPtr->tickCount = 0;
			if( windowPtr->isMovingBack ) {
				windowPtr->isMovingBack = false;
			}
			KillTimer( handle, 0 );
			return;
		}
	}
	windowPtr->tickCount++;
}

void CFieldWindow::completeProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime )
{
	CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	windowPtr->steps.clear();
	int tableauPileTopCardX, tableauPileTopCardY;
	windowPtr->prevPile->GetTopCardPos( tableauPileTopCardX, tableauPileTopCardY );
	if( windowPtr->tickCount % 2 == 0 && windowPtr->tickCount / 2 < 13 ) {
		int index = windowPtr->tickCount / 2;
		windowPtr->movingCards.push_back( std::move( windowPtr->prevPile->PopCard() ) );
		windowPtr->tickCountForMovingCards.push_back( windowPtr->tickCount );
		if( index == 12 ) {
			windowPtr->prevPile->CheckForOpen();
		}
	}
	for( size_t i = 0; i < windowPtr->movingCards.size(); ++i ) {
		int wastePileNextCardX, wastePileNextCardY;
		windowPtr->wastePile->
			GetNextCardPos( wastePileNextCardX, wastePileNextCardY );
		SetWindowPos(windowPtr->movingCards[i].get()->GetHandle(), HWND_TOP, 
			tableauPileTopCardX + (wastePileNextCardX - tableauPileTopCardX) * 
			(windowPtr->tickCount - windowPtr->tickCountForMovingCards[i]) / 10, 
			tableauPileTopCardY + (wastePileNextCardY - tableauPileTopCardY) * 
			(windowPtr->tickCount - windowPtr->tickCountForMovingCards[i]) / 10, 
			71, 96, 0);
	}
	if( windowPtr->tickCount - windowPtr->tickCountForMovingCards[0] == 10 ) {
		windowPtr->wastePile->AddCard( std::move( windowPtr->movingCards[0] ) );
		windowPtr->movingCards.erase(windowPtr->movingCards.begin());
		windowPtr->tickCountForMovingCards.pop_front();
	}
	windowPtr->tickCount++;
	if( windowPtr->movingCards.empty() ) {
		KillTimer( handle, 0 );
		windowPtr->tickCount = 0;
		windowPtr->tickCountForMovingCards.clear();
		if( windowPtr->wastePile->GetSize() == 104 ) {
			int result = MessageBox( handle, L"Поздравляем, вы выиграли! Начать новую игру?", L"Победа!", MB_YESNO );
			if( result == IDYES ) {
				windowPtr->InitField( false );
			}
		}
	}
}

void CFieldWindow::startHandingOutCardsProc( HWND handle, UINT message, UINT_PTR idEvent, DWORD dwTime )
{
	CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	int stockPileTopCardX, stockPileTopCardY;
	windowPtr->stockPile->GetTopCardPos( stockPileTopCardX, stockPileTopCardY );
	if( windowPtr->tickCount % 2 == 0 && windowPtr->tickCount / 2 < 54 ) {
		int index = windowPtr->tickCount / 2;
		std::unique_ptr<CCardWindow> ptr( 
			new CCardWindow( windowPtr->values.back() ) );
		ptr.get()->Create( handle );
		windowPtr->values.pop_back();
		if( index > 43 ) {
			ptr.get()->Open();
		}
		ptr.get()->SetPile( windowPtr->tableauPiles[index % 10].get() );
		windowPtr->movingCards.push_back( std::move( ptr ) );
		windowPtr->tickCountForMovingCards.push_back( windowPtr->tickCount);
		if( index % 10 == 3 ) {
			windowPtr->stockPile->PopCard();
		}
	}
	for( size_t i = 0; i < windowPtr->movingCards.size(); ++i ) {
		int tableauPileNextCardX, tableauPileNextCardY;
		windowPtr->movingCards[i].get()->GetPile()->
			GetNextCardPos( tableauPileNextCardX, tableauPileNextCardY );
		SetWindowPos(windowPtr->movingCards[i].get()->GetHandle(), HWND_TOP, 
			stockPileTopCardX + (tableauPileNextCardX - stockPileTopCardX) * 
			(windowPtr->tickCount - windowPtr->tickCountForMovingCards[i]) / 10, 
			stockPileTopCardY + (tableauPileNextCardY - stockPileTopCardY) * 
			(windowPtr->tickCount - windowPtr->tickCountForMovingCards[i]) / 10, 
			71, 96, 0);
	}
	if( windowPtr->tickCount - windowPtr->tickCountForMovingCards[0] == 10 ) {
		windowPtr->movingCards[0]->GetPile()->
			AddCard( std::move( windowPtr->movingCards[0] ) );
		windowPtr->movingCards[0].release();
		windowPtr->movingCards.erase(windowPtr->movingCards.begin());
		windowPtr->tickCountForMovingCards.pop_front();
	}
	windowPtr->tickCount++;
	if( windowPtr->movingCards.empty() ) {
		KillTimer( handle, 0 );
		windowPtr->tickCount = 0;
		windowPtr->tickCountForMovingCards.clear();
	}
}

LRESULT CFieldWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CFieldWindow* windowPtr = reinterpret_cast<CFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
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
			windowPtr->OnCreate();
			break;
		}
		case WM_COMMAND:
		{
			windowPtr->OnCommand(wParam);
			break;
		}
		case WM_SIZE:
		{
			windowPtr->OnSize();
			break;
		}
		case WM_GETMINMAXINFO:
		{
			windowPtr->OnGetMinMaxInfo(lParam);
			break;
		}
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
