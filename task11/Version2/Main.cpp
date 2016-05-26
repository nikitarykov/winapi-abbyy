// Описание: Главный исполняемый файл с точкой входа. 
// Демонстрирует жизненный цикл окна COverlappedWindow

#include "OverlappedWindow.h"
#include "resource.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance,
	LPSTR commandLine, int cmdShow)
{
	if( !COverlappedWindow::RegisterClass() ) {
		std::cout << "Window Registration Failed!";
		return 0;
	}
	COverlappedWindow window = COverlappedWindow();
	if( !window.Create() ) {
		std::cout << "Window Creation Failed!";
		return 0;
	}
	window.Show(cmdShow);
	HACCEL acceleratorHandle;
	acceleratorHandle = LoadAccelerators(instance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	MSG message;
	BOOL getMessageResult = 0;
	while( (getMessageResult = GetMessage(&message, 0, 0, 0)) != 0 ) {
		if( getMessageResult == -1 ) {
			return -1;
		}
		if( !TranslateAccelerator(window.GetHandle(), acceleratorHandle, &message) &&
			!IsDialogMessage(window.GetDialogHandle(), &message) ) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	DestroyAcceleratorTable(acceleratorHandle);
	return 0;
}