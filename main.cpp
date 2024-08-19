#include <windows.h>
#include <stdio.h>
#include <string>

#define MSG(m) {\
	MessageBoxA(NULL,m,NULL,MB_OK);}
#define FRAME_NO 2 // �f�t�H���g�t���[����
#define FRAME_TIME 50  // �t���[������

// �E�B���h�E�n���h��
HWND hwnd;
// �C���X�^���X�n���h��
HINSTANCE hinst;
// �摜�p�X�擾
std::string GetPictPath(int);

// �摜����
int gif_frame_max = 0;
// �摜�f�B���N�g���Ǎ�
void InitPictures();

// ���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::string filename; // �t�@�C���p�X�o�t
	InitPictures(); // �摜�Ǎ�
	int flg = (gif_frame_max == 0) ? FRAME_NO : gif_frame_max; // �摜�؂�ւ��t���O

	while (TRUE) {
		// �w�i�摜�ݒ�
		filename = GetPictPath(flg);
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (char*)filename.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		//PostMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)("ProxyServer"));
		Sleep(FRAME_TIME);
		--flg;
		if (flg < 0) flg = (gif_frame_max == 0) ? FRAME_NO : gif_frame_max;

		// �I������
		if(GetAsyncKeyState(VK_ESCAPE)) exit(0);
	}
	return 0;
}

#include <vector>

// ���s�t�@�C���̃p�X���擾
using namespace std;
// �f�t�H���g�摜
const string picture[] = { "defpic/test1.bmp", "defpic/test2.bmp", "defpic/test3.bmp" }; // "defpic/���摜/emil.bmp"�@���@https://sp.nicovideo.jp/watch/sm32503857
// �ǂݍ��݉摜
vector<string> pictures_path;
string GetModulePath()
{
	// ���s�t�@�C���̃p�X
	string modulePath = "";
	// �h���C�u���A�f�B���N�g�����A�t�@�C�����A�g���q
	char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

	// ���s�t�@�C���̃t�@�C���p�X���擾
	if (::GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
	{
		// �t�@�C���p�X�𕪊�
		::_splitpath_s(path, drive, dir, fname, ext);
		// �h���C�u�ƃf�B���N�g�������������Ď��s�t�@�C���p�X�Ƃ���
		modulePath = string(drive) + string(dir);
	}
	return modulePath;
}

// �摜�Ǎ���
void InitPictures() {
	string dir = GetModulePath() + "gifpic/";
	WIN32_FIND_DATA win32fd;
	HANDLE hfind;
	if (INVALID_HANDLE_VALUE==(hfind = FindFirstFile((dir + "*.bmp").c_str(), &win32fd))) return; // pictures��NULL�ŕԂ��B
	pictures_path.push_back(dir + win32fd.cFileName);
	while (FindNextFile(hfind, &win32fd)) {
		++gif_frame_max;
		pictures_path.push_back(dir + win32fd.cFileName);
	}
	FindClose(hfind);
}

// �摜�p�X�擾��
string GetPictPath(int i) {
	if(0 < gif_frame_max)return pictures_path[i];
	return GetModulePath() + picture[i]; // �����ŉ摜�����Ă��Ȃ��Ƃ��̃f�t�H
}
