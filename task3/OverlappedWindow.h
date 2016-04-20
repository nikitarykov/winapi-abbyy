// Описание: Класс СOverlappedWindow. Представляет собой C++ оболочку
// над классом окна WinAPI.

#pragma once

#include <windows.h>
#include "EllipseWindow.h"
#include <vector>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();

	// Создать экземпляр окна
	bool Create();
	// Показать окно
	void Show(int cmdShow);

protected:
	// Функция, вызываемая при создании неклиентской области окна (сообщение WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// Функция, вызываемая при создании клиентской области окна (сообщение WM_CREATE)
	void OnCreate();
	// Функция, вызываемая при изменении размеров окна (сообщение WM_SIZE)
	void OnSize();
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // хэндл окна
	std::vector<CEllipseWindow> children; // вектор дочерних окон

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

