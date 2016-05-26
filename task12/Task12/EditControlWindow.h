// Описание: Класс СEditControlWindow. Представляет собой C++ оболочку
// над классом окна с Edit Control.

#pragma once

#include <windows.h>

class CEditControlWindow {
public:
	CEditControlWindow();
	~CEditControlWindow();

	// Создать экземпляр окна
	bool Create(HWND& handleParent);
	// Геттер для хэндла окна
	HWND GetHandle();
	// функция, создающая свой шрифт по системному шрифту
	void InitFont();
	// функция, удаляющая свой шрифт
	void DestroyFont();
	// Возвращает размер шрифта в EditControl
	int GetFontSize();
	// Устанавливает размер шрифта в EditControl
	void SetFontSize(int size);

private:
	HWND handle; // хэндл окна
		
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

