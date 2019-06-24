//=============================================================================
//
// Main処理 [main.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "stars.h"
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		_T("AppClass")			// ウインドウのクラス名
#define WINDOW_NAME		_T("SMASH STARS")		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(HWND hWnd);
void Draw(HWND hWnd);

void ReInitGame(void);

#ifdef _DEBUG
// デバッグ表示処理
void DrawDebugFont(HWND hWnd);

#endif

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				D3D = NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		D3DDevice = NULL;		// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
LPD3DXFONT				D3DXFont = NULL;		// フォントへのポインタ
int						CountFPS;				// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						100,																		// ウィンドウの左座標
						100,																		// ウィンドウの上座標
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// ウィンドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if(FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	
	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				CountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	// FPSを測定
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

				Update(hWnd);		// 更新処理
				Draw(hWnd);				// 描画処理
				
				dwFrameCount++;		// 処理回数のカウントを加算
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	srand((unsigned)time(NULL));

	// Direct3Dオブジェクトの作成
	D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(D3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定

	if(bWindow)
	{// ウィンドウモード
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
									D3DDEVTYPE_HAL,								// ディスプレイタイプ
									hWnd,										// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
									&d3dpp,										// デバイスのプレゼンテーションパラメータ
									&D3DDevice)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp,
										&D3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp,
											&D3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// カリングを行わない
	D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// α値で透明処理を行う

#ifdef _DEBUG
																			// 情報表示用フォントを設定
	D3DXCreateFont(D3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &D3DXFont);
#endif
	
	// 初期化
	InitStars();
	

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// 入力処理の終了処理
	UninitInput();

	if (D3DDevice != NULL)
	{// デバイスの開放
		D3DDevice->Release();
	}

	if (D3D != NULL)
	{// Direct3Dオブジェクトの開放
		D3D->Release();
	}

	// 終了処理
	UninitStars();

}

//=============================================================================
// 更新処理
//=============================================================================
void Update(HWND hWnd)
{
	// 入力の更新処理
	UpdateInput();
	
	// 更新処理
	UpdateStars();

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(HWND hWnd)
{
	// バックバッファ＆Ｚバッファのクリア
	D3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(D3DDevice->BeginScene()))
	{
		// 描画処理
		DrawStars();

#ifdef _DEBUG
		// デバッグ表示
		DrawDebugFont(hWnd);
#endif
		
		// Direct3Dによる描画の終了
		D3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	D3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイス取得関数
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return D3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// デバッグ表示処理
//=============================================================================
void DrawDebugFont(HWND hWnd)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	char str[256];

	// テキスト描画
	sprintf(str, _T("FPS:%d"), CountFPS);
	D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// マウス座標
	rect.top += 20;
	sprintf(str, _T("mouse pos:%f %f %f"), GetMousePos(hWnd).x, GetMousePos(hWnd).y, GetMousePos(hWnd).z);
	D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// マウス座標
	rect.top += 20;
	sprintf(str, _T("stars_max:%d"), STARS_MAX);
	D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// 星の情報
	for (int i = 0; i < 33; i++)
	{
		rect.top += 20;
		sprintf(str, _T("stars %d pos:%d %d"), i, (int)GetStarsBase(i)->pos.x, (int)GetStarsBase(i)->pos.y);
		D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	rect.top = 40;
	rect.left += 200;
	for (int i = 33; i < 66; i++)
	{
		rect.top += 20;
		sprintf(str, _T("stars %d pos:%d %d"), i, (int)GetStarsBase(i)->pos.x, (int)GetStarsBase(i)->pos.y);
		D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	rect.top = 40;
	rect.left += 200;
	for (int i = 66; i < 99; i++)
	{
		rect.top += 20;
		sprintf(str, _T("stars %d pos:%d %d"), i, (int)GetStarsBase(i)->pos.x, (int)GetStarsBase(i)->pos.y);
		D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	rect.top = 40;
	rect.left += 200;
	for (int i = 99; i < 132; i++)
	{
		rect.top += 20;
		sprintf(str, _T("stars %d pos:%d %d"), i, (int)GetStarsBase(i)->pos.x, (int)GetStarsBase(i)->pos.y);
		D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	rect.top = 40;
	rect.left += 200;
	for (int i = 132; i < 165; i++)
	{
		rect.top += 20;
		sprintf(str, _T("stars %d pos:%d %d"), i, (int)GetStarsBase(i)->pos.x, (int)GetStarsBase(i)->pos.y);
		D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	rect.top = 40;
	rect.left += 200;
	for (int i = 165; i < 180; i++)
	{
		rect.top += 20;
		sprintf(str, _T("stars %d pos:%d %d"), i, (int)GetStarsBase(i)->pos.x, (int)GetStarsBase(i)->pos.y);
		D3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

}
#endif

//=============================================================================
// ゲームの再初期化処理処理
// 戻り値：無し
//=============================================================================
void ReInitGame(void)
{
	{// 再初期化	 

	}
}