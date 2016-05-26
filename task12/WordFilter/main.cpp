#include <windows.h>
#include <sstream>
#include <string>
#include <strsafe.h>

const std::wstring dictionary[5] = { L"читал", L"ты", L"это", L"на", L"видишь" };

void filter( wchar_t* text )
{
	std::wstring filteredText;
	std::wstring word;
	wchar_t* res = text;
	while( *text != '\0' ) {
		while( *text != '\0' && *text != '\n' && *text != ' ' ) {
			word += *text;
			text++;
		}
		for( std::wstring dictWord : dictionary ) {
			if( word == dictWord ) {
				filteredText += word;
			}
		}
		filteredText += *text;
		if( *text == '\0') {
			break;
		}
		text++;
		word.clear();
	}
	text = res;
	memcpy(text, filteredText.c_str(), (filteredText.size() + 1) * sizeof(wchar_t));
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevInstance,
	LPSTR commandLine, int cmdShow )
{
	wchar_t newTextEventName[50];
	wchar_t textProceededEventName[50];
	wchar_t fileName[50];
	wchar_t destroyEventName[50];
	int processId = GetCurrentProcessId();
	swprintf_s(newTextEventName, L"NewText%d", processId);
	swprintf_s(fileName, L"File%d", processId);
	swprintf_s(textProceededEventName, 
		L"TextProceeded%d", processId);
	swprintf_s(destroyEventName, L"Destroy%d", processId);
	HANDLE waitEvents[2];
	waitEvents[0] = CreateEvent( 0, false, false, newTextEventName );
	waitEvents[1] = CreateEvent( 0, false, false, destroyEventName );
	HANDLE readyEvent = OpenEvent( EVENT_ALL_ACCESS, false, L"Ready" );
	SetEvent( readyEvent );
	CloseHandle( readyEvent );
	while( true ) {
		int object = WaitForMultipleObjects( 2, waitEvents, false, INFINITE );
		if( object == WAIT_OBJECT_0 ) {
			HANDLE mapHandle = OpenFileMapping( FILE_MAP_ALL_ACCESS, false, fileName );
			wchar_t* buffer = static_cast< wchar_t* >( MapViewOfFile( mapHandle,
				FILE_MAP_ALL_ACCESS, 0, 0, 0 ) );
			filter( buffer );
			CloseHandle( mapHandle );
			HANDLE textProceededEvent = OpenEvent( EVENT_ALL_ACCESS, false, textProceededEventName );
			SetEvent( textProceededEvent );
			CloseHandle( textProceededEvent );
		} else {
			break;
		}
	}
	CloseHandle(waitEvents[0]);
	CloseHandle(waitEvents[1]);
}

