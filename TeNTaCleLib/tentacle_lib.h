#pragma once
#define _WINSOCKAPI_

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "xinput.lib" )
#pragma comment( lib, "xaudio2.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "zlibstatic.lib" )
#pragma comment( lib, "assimp.lib" )
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2-static.lib" )
#pragma comment( lib, "Effekseer.lib" )
#pragma comment( lib, "EffekseerRendererDX11.lib" )
#pragma comment( lib, "xinput.lib" )

#pragma comment( lib, "Crypt32.lib" )
#pragma comment( lib, "libssl.lib" )
#pragma comment( lib, "libcrypto.lib" )

#include <unordered_map>
#include <windows.h>

#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/TextSprite.h"
#include "Timer.h"
#include "Audio.h"
#include "Input.h"
#include "Console.h"
#include "XMFLOAT.h"

namespace TentacleLib
{
	static struct SETTING
	{
		LPCWSTR title;
		UINT syncInterval = 1;
		bool showFPS = false;
	} setting;
	static HWND hWnd;

	HWND Init(LPCWSTR, HINSTANCE, LONG width, LONG height, LPWSTR cmd_line, INT cmd_show, WNDPROC);
	void Finalize();

	void SetSyncInterval(UINT syncInterval);
	void SetShowFPS(bool showFPS);

	void Update();
	void Draw();

	bool isFocus();
	bool isShowCursor();

	static Graphics& graphics = Graphics::Instance();
	static TentacleLib::Timer& timer = TentacleLib::Timer::Instance();
	static Audio& audio = Audio::Instance();
	static Input& input = Input::Instance();
	static Console& console = Console::Instance();
	static ResourceManager& resource = ResourceManager::Instance();
	static TextSprite& textSprite = TextSprite::Instance();
}
#define T_GRAPHICS TentacleLib::graphics
#define T_TIMER TentacleLib::timer
#define T_AUDIO TentacleLib::audio
#define T_INPUT TentacleLib::input
#define T_RESOURCE TentacleLib::resource
#define T_TEXT TentacleLib::textSprite
#define T_CONSOLE TentacleLib::console
#define SCREEN_W TentacleLib::graphics.GetScreenWidth()
#define SCREEN_H TentacleLib::graphics.GetScreenHeight()
#define RADIAN1 0.01745329251f
#define CURSOR_OFF while (ShowCursor(false) >= 0)
#define CURSOR_ON while (ShowCursor(true) <= 0)
#define RANDOMI(l,h) (rand() % (h) + (l))
#define RANDOMF(l,h) ((l) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((h)-(l)))))