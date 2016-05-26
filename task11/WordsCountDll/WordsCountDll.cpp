#include "WordsCountDll.h"

int WordsCount( const wchar_t* text )
{
	int count = 0;
	int last = 0;
	while( *text != '\0' ) {
		while( *text != '\0' && *text != '\n' && *text == ' ' ) {
			text++;
		}
		if( *text == '\0') {
			break;
		}
		text++;
		count++;
		last = 0;
		while( *text != '\0' && *text != '\n' && *text != ' ' ) {
			text++;
			last++;
		}
	}
	return count;
}