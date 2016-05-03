#pragma once

#include <windows.h>

class CCardWindow
{
public:
	CCardWindow();
	~CCardWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass();
	// Создать экземпляр окна
	bool Create(HWND& handleParent);
	// Показать окно
	void Show(int commandShow);
	// Геттер для хэндла окна
	HWND GetHandle();
	
protected:
	// Функция, вызываемая при создании неклиентской области окна (сообщение WM_NCCREATE)
	void OnNCCreate(HWND& _handle);
	// Функция, вызываемая при создании клиентской области окна (сообщение WM_CREATE)
	void OnCreate();
	// Функция, вызываемая при перерисовке фона карты (сообщение WM_ERASEBKGND)
	void OnDrawBackground();
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

private:
	HWND handle; // хэндл окна

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};


