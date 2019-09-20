#include <stdio.h>
#include <stdlib.h>
#include <atlimage.h>
#include <time.h>
#include <WinUser.h>
#include <iostream>
#include <direct.h>
#include <Windows.h>
#include <winbase.h>
#include <tchar.h>
using namespace std;
int mouse2;//滑鼠數值容器
static CString Path = L"\\\\10.224.22.219\\Screen\\"; //檔案路徑變數

//隱藏console
void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

//創建時間資料夾拍攝桌面存入
void scrshot() {
	TCHAR szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(szBuffer, &dwSize); //Get the computer name
	CString PCName = szBuffer;//PC名稱變數
	CImage image;//加入圖像模塊
	time_t t = time(0);   // 取得現在時間
	struct tm* now = localtime(&t);
	char timefolder[80];
	strftime(timefolder, 80, "%m_%d_%H", now);//資料夾時間
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d %H_%M_%S", now);//時間格式
	CString type = ".jpg";//檔案類型變
	CString Filename = buffer;//檔案名稱變數
	CString Totlename = Path + PCName + "\\" + timefolder + "\\" + Filename + type;//檔案名稱加上路徑
	CString TotlenameTime = Path + PCName + "\\" + timefolder;//加上時間變數資料夾
	CW2A pszConvertedAnsichar(TotlenameTime);//型別轉換
	//確定時間資料夾是否存在自動創建
	if (_mkdir(pszConvertedAnsichar) == 0)
	{
		printf("Directory '\\\\10.224.22.219\\Screen\\PCName\\timefolder' was successfully created\n");
	}
	else {
		printf("Problem creating directory '\\\\10.224.22.219\\Screen\\PCName\\timefolder'\n");
	}
	// 獲取窗口設備上下文（Device Contexts）
	HDC GetDC(NULL); // 要截圖的窗口控制代碼，空值為全螢幕
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbitmap(NULL);
	int width = 3840;
	int heigth = 1080;
	//int width = GetDeviceCaps(hdc, HORZRES);// 螢幕圖片寬度
	//int heigth = GetDeviceCaps(hdc, VERTRES);// 螢幕圖片高度
	hbitmap = CreateCompatibleBitmap(hdc, width, heigth);
	SelectObject(hdcMem, hbitmap);
	BitBlt(hdcMem, 0, 0, width, heigth, hdc, 0, 0, SRCCOPY);

	// 獲取螢幕相關訊息的尺寸大小
	int nBitPerPixel = GetDeviceCaps(hdcMem, BITSPIXEL);
	// 創建圖像，設置高度，像素
	image.Create(width, heigth, nBitPerPixel);

	//讀取滑鼠圖繪製到hdcMem上
	HICON hIcon;
	hIcon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
		NULL,             // hInstance must be NULL when loading from a file
		L"C:\\screenshotc++\\cursor.ico",   // the icon file name
		IMAGE_ICON,       // specifies that the file is an icon
		0,                // width of the image (we'll specify default later on)
		0,                // height of the image
		LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
		LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
		LR_SHARED         // let the system release the handle when it's no longer used
	);
	POINT pt;//滑鼠座標指令
	BOOL bReturn = GetCursorPos(&pt);//滑鼠座標回傳確認變數
	DrawIcon(hdcMem, pt.x, pt.y, hIcon);

	////// 對指定的源設備環境區域中的像素進行位塊（bit_block）轉換
	BitBlt(
		image.GetDC(),  // 保存到的目標 圖片對象 上下文
		0, 0,     // 起始 x, y 座標
		width, heigth,  // 截圖寬高
		hdcMem,      // 擷取對象的 控制代碼
		0, 0,           // 指定源矩形區域左上角的 X, y 邏輯坐標
		SRCCOPY);

	// 釋放 DC控制代碼
	ReleaseDC(NULL, hdcMem);
	// 釋放圖片
	image.ReleaseDC();
	//image.Save(L"c:\\ScreenShot.jpg", Gdiplus::ImageFormatJPEG);//圖片路徑,圖片格式
	image.Save(Totlename, Gdiplus::ImageFormatJPEG);//圖片路徑,圖片格式
	printf("\nSAVE Location ");
	printf("X: %lu ", pt.x);
	printf("Y: %lu ", pt.y);
}

//偵測滑鼠移動
int mouseposition() {
	POINT pt;//滑鼠座標指令
	BOOL bReturn = GetCursorPos(&pt);//滑鼠座標回傳確認變數
	int x = pt.x;
	return x;
}

//主程式
int main()
{
	//HideConsole();隱藏
	system("NET USE \\\\10.224.22.219 /user:fileserver19\\administrator !qaz2wsx");//登入NAS取的存取權限
	TCHAR szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(szBuffer, &dwSize); //Get the computer name
	CString PCName = szBuffer;//PC名稱變數
	CString Totlename = Path + PCName;//電腦名稱資料夾位置
	//CT2CA pszConvertedAnsiString(Totlename);//型別轉換CString to string
	//std::string folder(pszConvertedAnsiString);
	CW2A pszConvertedAnsichar(Totlename);//型別轉換CString to char
	//判斷電腦名稱資料夾是否存在不存在自動創建
	if (_mkdir(pszConvertedAnsichar) == 0)
	{
		printf("Directory '\\\\10.224.22.219\\Screen\\' was successfully created\n");
		SetFileAttributes(Totlename, FILE_ATTRIBUTE_HIDDEN);
	}
	else {
		printf("Problem creating directory '\\\\10.224.22.219\\Screen\\'\n");
		SetFileAttributes(Totlename, FILE_ATTRIBUTE_HIDDEN);
	}
	//判斷使用者是否動作呼叫scrshot函數
	while (true)
	{
		int mouse1 = mouseposition();
		if (mouse1 == mouse2)
		{
			printf("\nSTOP");
			printf("%d", mouse1);
			Sleep(1000);
		}
		else
		{
			scrshot();
			mouse2 = mouse1;
			Sleep(2000);
		}
	}
}
