// Описание: Класс СOverlappedWindow. Представляет собой C++ оболочку
// над классом окна WinAPI.

#pragma once

#include <windows.h>

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
	// Функция, вызываемая при перерисовке окна (сообщение WM_PAINT)
	void OnPaint();
	// Функция, вызываемая при срабатывании таймера (сообщение WM_TIMER)
	void OnTimer();
	// Функция, вызываемая при уничтожении окна (сообщение WM_DESTROY)
	void OnDestroy();

	// Перерисовка эллипса
	void drawEllipse(HDC& memoryDC, RECT& rect);
private:
	HWND handle; // хэндл окна
	UINT_PTR timer; // таймер для перерисовки
	int xSpeed; // скорость эллипса по оси x
	int ySpeed; // скорость эллипса по оси y
	int x; // координата центра эллипса по оси x
	int y; // координата центра эллипса по оси y
	int tickCount; // количество сообщений от таймера
	int paintCount; // количество перерисовок
	static const int aAxis = 100; // главная ось эллипса
	static const int bAxis = 60; // побочная ось эллипса

	// методы для перерасчета координат центра эллипса
	void calcX(const RECT& rect);
	void calcY(const RECT& rect);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

