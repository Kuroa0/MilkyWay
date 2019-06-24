//=============================================================================
//
// 入力処理 [stars.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#include "stars.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVtxStars(int no);
void SetVtxStars(int no);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9	starsTexture;
STARS_BASE			starsBase[STARS_MAX];
STARS_SEC			starsSecondary[STARS_SEC_MAX];

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitStars(void)
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(device,	// デバイスへのポインタ
	STARS_TEXTURE,						// ファイルの名前
	&starsTexture);						// 読み込むメモリー

	// パラメータの設定
	for (int i = 0; i < STARS_SEC_MAX; i++)
	{
		starsSecondary[i].col = D3DCOLOR_RGBA(rand() % 20 + 235, 0, 0, 0);
		starsSecondary[i].col.g = starsSecondary[i].col.r;
		starsSecondary[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		starsSecondary[i].size = D3DXVECTOR2(rand() % 50 + 25, 0.0f);
		starsSecondary[i].size.y = starsSecondary[i].size.x;
		starsSecondary[i].angle = 1;
		starsSecondary[i].alpha = 0;
		starsSecondary[i].rotSpd = 1.0f / (rand() % 300 + 200);
	}
	for (int i = 0; i < STARS_MAX; i++)
	{
		int tmpSecNo = (i / 3);

		starsBase[i].pos = STARS_MID_POS;
		starsBase[i].use = false;
		starsBase[i].pos.x += rand() % 200 - 100;
		starsBase[i].pos.y += rand() % 700;
		
		// 頂点フォーマット(中心座標)
		MakeVtxStars(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStars(void)
{
	RELEASE_TEXTURE(starsTexture)
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStars(void)
{
	// パラメータの設定
	for (int i = 0; i < STARS_MAX; i++)
	{
		int tmpSecNo = (i / 3);

		starsBase[i].pos.y += 5;

		if (starsBase[i].pos.y > SCREEN_HEIGHT + starsSecondary[tmpSecNo].size.y)
		{
			starsBase[i].pos.y = -starsSecondary[tmpSecNo].size.y;
		}

		if (!starsBase[i].use)
		{
			if ((rand() % 10) == 0)
			{
				starsBase[i].use = true;
			}
		}

		if (starsBase[i].use)
		{
			// 点滅の速度(8160=255*32)
			float tmpAlphaSpd = (360.0f / 8160.0f);
			starsSecondary[tmpSecNo].alpha += tmpAlphaSpd;
			if (starsSecondary[tmpSecNo].alpha > 360.0f)
			{
				starsSecondary[tmpSecNo].alpha -= 360.0f;
				starsBase[i].use = false;
			}

			// 正弦波で点滅処理
			starsSecondary[tmpSecNo].col.a = sinf(starsSecondary[tmpSecNo].alpha);
		}

		starsSecondary[tmpSecNo].rot.z += starsSecondary[tmpSecNo].rotSpd;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStars(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, starsTexture);

	for (int i = 0; i < STARS_MAX; i++)
	{
		// 頂点座標の設定(中心座標)
		SetVtxStars(i);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &starsBase[i].vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点設定(中心座標)
//=============================================================================
void SetVtxStars(int no)
{
	// パラメータの設定
	int tmpSecNo = (no / 3);

	// 頂点座標の設定
	starsBase[no].vertexWk[0].vtx.x = starsBase[no].pos.x - cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[0].vtx.y = starsBase[no].pos.y - sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[0].vtx.z = 0.0f;

	starsBase[no].vertexWk[1].vtx.x = starsBase[no].pos.x + cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[1].vtx.y = starsBase[no].pos.y - sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[1].vtx.z = 0.0f;

	starsBase[no].vertexWk[2].vtx.x = starsBase[no].pos.x - cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[2].vtx.y = starsBase[no].pos.y + sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[2].vtx.z = 0.0f;

	starsBase[no].vertexWk[3].vtx.x = starsBase[no].pos.x + cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[3].vtx.y = starsBase[no].pos.y + sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[3].vtx.z = 0.0f;

	// 反射光の設定
	starsBase[no].vertexWk[0].diffuse = starsSecondary[tmpSecNo].col;
	starsBase[no].vertexWk[1].diffuse = starsSecondary[tmpSecNo].col;
	starsBase[no].vertexWk[2].diffuse = starsSecondary[tmpSecNo].col;
	starsBase[no].vertexWk[3].diffuse = starsSecondary[tmpSecNo].col;

}

//=============================================================================
// 頂点フォーマット(中心座標)
//=============================================================================
void MakeVtxStars(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	STARS_BASE *base = GetStarsBase(no);

	int tmpSecNo = (no / 3);

	STARS_SEC *sec = GetStarsSec(tmpSecNo);

	// 頂点座標の設定
	starsBase[no].vertexWk[0].vtx.x = starsBase[no].pos.x - cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[0].vtx.y = starsBase[no].pos.y - sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[0].vtx.z = 0.0f;

	starsBase[no].vertexWk[1].vtx.x = starsBase[no].pos.x + cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[1].vtx.y = starsBase[no].pos.y - sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[1].vtx.z = 0.0f;

	starsBase[no].vertexWk[2].vtx.x = starsBase[no].pos.x - cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[2].vtx.y = starsBase[no].pos.y + sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) - starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[2].vtx.z = 0.0f;

	starsBase[no].vertexWk[3].vtx.x = starsBase[no].pos.x + cosf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.x;
	starsBase[no].vertexWk[3].vtx.y = starsBase[no].pos.y + sinf(D3DXToDegree(starsSecondary[tmpSecNo].angle) + starsSecondary[tmpSecNo].rot.z) * starsSecondary[tmpSecNo].size.y;
	starsBase[no].vertexWk[3].vtx.z = 0.0f;

	// rhwの設定
	base->vertexWk[0].rhw =
	base->vertexWk[1].rhw =
	base->vertexWk[2].rhw =
	base->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	base->vertexWk[0].diffuse = sec->col;
	base->vertexWk[1].diffuse = sec->col;
	base->vertexWk[2].diffuse = sec->col;
	base->vertexWk[3].diffuse = sec->col;

	// テクスチャ座標の設定
	base->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	base->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	base->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	base->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 星のベース情報を取得
//=============================================================================
STARS_BASE *GetStarsBase(int no)
{
	return &starsBase[no];
}

//=============================================================================
// 星のSec情報を取得
//=============================================================================
STARS_SEC *GetStarsSec(int no)
{
	return &starsSecondary[no];
}
