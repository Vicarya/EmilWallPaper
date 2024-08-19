#include <windows.h>
#include <stdio.h>
#include <string>

#define MSG(m) {\
	MessageBoxA(NULL,m,NULL,MB_OK);}
#define FRAME_NO 2 // デフォルトフレーム数
#define FRAME_TIME 50  // フレーム長さ

// ウィンドウハンドル
HWND hwnd;
// インスタンスハンドル
HINSTANCE hinst;
// 画像パス取得
std::string GetPictPath(int);

// 画像枚数
int gif_frame_max = 0;
// 画像ディレクトリ読込
void InitPictures();

// メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::string filename; // ファイルパスバフ
	InitPictures(); // 画像読込
	int flg = (gif_frame_max == 0) ? FRAME_NO : gif_frame_max; // 画像切り替えフラグ

	while (TRUE) {
		// 背景画像設定
		filename = GetPictPath(flg);
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (char*)filename.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		//PostMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)("ProxyServer"));
		Sleep(FRAME_TIME);
		--flg;
		if (flg < 0) flg = (gif_frame_max == 0) ? FRAME_NO : gif_frame_max;

		// 終了処理
		if(GetAsyncKeyState(VK_ESCAPE)) exit(0);
	}
	return 0;
}

#include <vector>

// 実行ファイルのパスを取得
using namespace std;
// デフォルト画像
const string picture[] = { "defpic/test1.bmp", "defpic/test2.bmp", "defpic/test3.bmp" }; // "defpic/元画像/emil.bmp"　→　https://sp.nicovideo.jp/watch/sm32503857
// 読み込み画像
vector<string> pictures_path;
string GetModulePath()
{
	// 実行ファイルのパス
	string modulePath = "";
	// ドライブ名、ディレクトリ名、ファイル名、拡張子
	char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

	// 実行ファイルのファイルパスを取得
	if (::GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
	{
		// ファイルパスを分割
		::_splitpath_s(path, drive, dir, fname, ext);
		// ドライブとディレクトリ名を結合して実行ファイルパスとする
		modulePath = string(drive) + string(dir);
	}
	return modulePath;
}

// 画像読込部
void InitPictures() {
	string dir = GetModulePath() + "gifpic/";
	WIN32_FIND_DATA win32fd;
	HANDLE hfind;
	if (INVALID_HANDLE_VALUE==(hfind = FindFirstFile((dir + "*.bmp").c_str(), &win32fd))) return; // picturesをNULLで返す。
	pictures_path.push_back(dir + win32fd.cFileName);
	while (FindNextFile(hfind, &win32fd)) {
		++gif_frame_max;
		pictures_path.push_back(dir + win32fd.cFileName);
	}
	FindClose(hfind);
}

// 画像パス取得部
string GetPictPath(int i) {
	if(0 < gif_frame_max)return pictures_path[i];
	return GetModulePath() + picture[i]; // 自分で画像を入れていないときのデフォ
}
