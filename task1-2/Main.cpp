// Описание: Главный исполняемый файл с точкой входа. 
// Демонстрирует жизненный цикл окна COverlappedWindow

#include "OverlappedWindow.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance,
	LPSTR commandLine, int cmdShow)
{
	COverlappedWindow window = COverlappedWindow();
	if (!window.RegisterClass())
	{
		std::cout << "Window Registration Failed!";
		return 0;
	}
	if (!window.Create())
	{
		std::cout << "Window Creation Failed!";
		return 0;
	}
	window.Show(cmdShow);
	MSG message;
	BOOL getMessageResult = 0;
	while ((getMessageResult = GetMessage(&message, 0, 0, 0)) != 0) {
		if (getMessageResult == -1) {
			return -1;
		}
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return 0;
}