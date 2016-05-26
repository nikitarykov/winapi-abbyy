// ŒÔËÒ‡ÌËÂ: –Â‡ÎËÁ‡ˆËˇ ÏÂÚÓ‰Ó‚ ÍÎ‡ÒÒ‡ —SettingsDialog

#include "SettingsDialog.h"
#include "resource.h"
#include "CommCtrl.h"
#include "OverlappedWindow.h"
#include "EditControlWindow.h"
#include <cmath>

CSettingsDialog::CSettingsDialog()
{
	handle = 0;
	isPreview = false;
	oldFontSize = 0;
	fontSize = 0;
	oldTransparency = 0;
	transparency = 0;
	oldFontColor = RGB(0, 0, 0);
	fontColor = RGB(0, 0, 0);
	oldBackgroundColor = RGB(255, 255, 255);
	backgroundColor = RGB(255, 255, 255);
	::ZeroMemory(custColors, 16 * sizeof(COLORREF));
}

CSettingsDialog::~CSettingsDialog()
{
}

HWND CSettingsDialog::GetHandle()
{
	return handle;
}

bool CSettingsDialog::Create(HWND& windowHandle)
{
	COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
	CreateDialogParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), windowHandle, dialogProc,
		reinterpret_cast<LPARAM>(this));
	ShowWindow(handle, SW_SHOW);
	return (handle != 0);
}

void CSettingsDialog::OnCheckPreview()
{
	HWND parentHandle = GetParent(handle);
	COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	isPreview = IsDlgButtonChecked(handle, IDC_CHECK1) == BST_CHECKED;
	if( isPreview ) {
		window->SetFontColor(fontColor);
		window->SetBackgroundColor(backgroundColor);
		window->SetFontSize(fontSize);
		SetLayeredWindowAttributes(parentHandle, 0, transparency, LWA_ALPHA);
	} else {
		window->SetFontColor(oldFontColor);
		window->SetBackgroundColor(oldBackgroundColor);
		window->SetFontSize(oldFontSize);
		SetLayeredWindowAttributes(parentHandle, 0, oldTransparency, LWA_ALPHA);
	}
	InvalidateRect(parentHandle, 0, true);

}

void CSettingsDialog::OnInitDialog(HWND& _handle)
{
	isPreview = false;
	handle = _handle;
	HWND parentHandle = GetParent(handle);
	COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	oldFontSize = window->GetFontSize();
	oldBackgroundColor = window->GetBackgroundColor();
	oldFontColor = window->GetFontColor();
	BYTE buff;
	GetLayeredWindowAttributes(GetParent(handle), 0, &buff, 0);
	oldTransparency = buff;
	SendDlgItemMessage(handle, IDC_SLIDER1, TBM_SETRANGE, 1, MAKELONG(8, 72));
	SendDlgItemMessage(handle, IDC_SLIDER1, TBM_SETPOS, 1, oldFontSize);
	SendDlgItemMessage(handle, IDC_SLIDER2, TBM_SETRANGE, 1, MAKELONG(0, 255));
	SendDlgItemMessage(handle, IDC_SLIDER2, TBM_SETPOS, 1, oldTransparency);
	fontSize = oldFontSize;
	transparency = oldTransparency;
	backgroundColor = oldBackgroundColor;
	fontColor = oldFontColor;
}

void CSettingsDialog::OnFontChange()
{
	fontSize = SendDlgItemMessage(handle, IDC_SLIDER1, TBM_GETPOS, 0, 0);
	HWND parentHandle = GetParent(handle);
	COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	if( isPreview ) {
		window->SetFontSize(fontSize);
	}
}

void CSettingsDialog::OnTransparencyChange()
{
	transparency = SendDlgItemMessage(handle, IDC_SLIDER2, TBM_GETPOS, 0, 0);
	if( isPreview ) {
		SetLayeredWindowAttributes(GetParent(handle), 0, transparency, LWA_ALPHA);
	}
}

void CSettingsDialog::OnFontColorChange()
{
	CHOOSECOLOR chooseColor;
	::ZeroMemory(&chooseColor, sizeof(CHOOSECOLOR));
	chooseColor.lStructSize = sizeof(CHOOSECOLOR);
	chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	chooseColor.hwndOwner = handle;
	chooseColor.lpCustColors = custColors;
	chooseColor.rgbResult = fontColor;
	if (::ChooseColor(&chooseColor)) {
		HWND parentHandle = GetParent(handle);
		COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
		fontColor = chooseColor.rgbResult;
		if( isPreview ) {
			window->SetFontColor(fontColor);
			window->SetFontSize(fontSize);
		} else {
			window->SetFontColor(oldFontColor);
			window->SetFontSize(oldFontSize);
		}
	}
}

void CSettingsDialog::OnBackgroundColorChange()
{
	CHOOSECOLOR chooseColor;
	::ZeroMemory(&chooseColor, sizeof(CHOOSECOLOR));
	chooseColor.lStructSize = sizeof(CHOOSECOLOR);
	chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	chooseColor.hwndOwner = handle;
	chooseColor.lpCustColors = custColors;
	chooseColor.rgbResult = backgroundColor;
	if( ::ChooseColor(&chooseColor) ) {
		HWND parentHandle = GetParent(handle);
		COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
		backgroundColor = chooseColor.rgbResult;
		if( isPreview ) {
			window->SetBackgroundColor(backgroundColor);
			window->SetFontSize(fontSize);
		} else {
			window->SetBackgroundColor(oldBackgroundColor);
			window->SetFontSize(oldFontSize);
		}
	}
}

void CSettingsDialog::OnOk(LPARAM wParam)
{
	HWND parentHandle = GetParent(handle);
	COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	window->SetFontColor(fontColor);
	window->SetBackgroundColor(backgroundColor);
	window->SetFontSize(fontSize);
	SetLayeredWindowAttributes(parentHandle, 0, transparency, LWA_ALPHA);
	EndDialog(handle, wParam);
	window->ResetHasDialog();
}

void CSettingsDialog::OnCancel(LPARAM wParam)
{
	HWND parentHandle = GetParent(handle);
	COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(parentHandle, GWLP_USERDATA));
	window->SetFontColor(oldFontColor);
	window->SetBackgroundColor(oldBackgroundColor);
	window->SetFontSize(oldFontSize);
	SetLayeredWindowAttributes(parentHandle, 0, oldTransparency, LWA_ALPHA);
	EndDialog(handle, wParam);
	window->ResetHasDialog();
}

INT_PTR CSettingsDialog::dialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) {
		case WM_INITDIALOG:
		{
			CSettingsDialog* dialogPtr = reinterpret_cast<CSettingsDialog*>(lParam);
			SetWindowLongPtr(handle, DWLP_USER, reinterpret_cast<LONG_PTR>(dialogPtr));
			dialogPtr->OnInitDialog(handle);
			break;

		}
		case WM_HSCROLL:
		{
			CSettingsDialog* dialogPtr = reinterpret_cast<CSettingsDialog*>(GetWindowLongPtr(handle, DWLP_USER));
			int sliderId = GetDlgCtrlID(reinterpret_cast<HWND>(lParam));
			if( sliderId == IDC_SLIDER1 ) {
				dialogPtr->OnFontChange();
			} else {
				dialogPtr->OnTransparencyChange();
			}
			break;
		}
		case WM_COMMAND:
		{
			CSettingsDialog* dialogPtr = reinterpret_cast<CSettingsDialog*>(GetWindowLongPtr(handle, DWLP_USER));
			switch( LOWORD(wParam) ) {
				case IDOK:
				{
					dialogPtr->OnOk(wParam);
					return true;
				}
				case IDCANCEL:
				{
					dialogPtr->OnCancel(wParam);
					return true;
				}
				case IDC_CHECK1:
				{
					dialogPtr->OnCheckPreview();
					break;
				}
				case IDC_BUTTON1:
				{
					dialogPtr->OnBackgroundColorChange();
					break;
				}
				case IDC_BUTTON2:
				{
					dialogPtr->OnFontColorChange();
					break;
				}
			}
		}
	}
	return false;
}