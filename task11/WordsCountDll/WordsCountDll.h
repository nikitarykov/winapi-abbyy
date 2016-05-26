#pragma once

#ifdef WORDSCOUNTDLL_EXPORTS
#define MYAPI __declspec(dllexport) 
#else
#define MYAPI __declspec(dllimport) 
#endif


extern "C" MYAPI int WordsCount( const wchar_t* text );