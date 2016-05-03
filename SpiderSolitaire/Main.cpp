// Описание: Главный исполняемый файл с точкой входа. 
// Запускаем игру Пасьянс "Паук"

#include "FieldWindow.h"
#include "resource.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance,
	LPSTR commandLine, int cmdShow)
{
	if( !CFieldWindow::RegisterClass() ) {
		std::cout << "Window Registration Failed!";
		return 0;
	}
	CFieldWindow window = CFieldWindow();
	if( !window.Create() ) {
		std::cout << "Window Creation Failed!";
		return 0;
	}
	window.Show(cmdShow);
	MSG message;
	BOOL getMessageResult = 0;
	while( (getMessageResult = GetMessage(&message, 0, 0, 0)) != 0 ) {
		if( getMessageResult == -1 ) {
			return -1;
		}
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return 0;
}