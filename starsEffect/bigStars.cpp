//=============================================================================
//
// 星のエフェクト処理 [bigStars.cpp]
// Author : 蔡 友剛
//
//=============================================================================
#include "bigStars.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVtxBigStars(void);
void SetVtxBigStars(void);

//=============================================================================
// グローバル変数:
//=============================================================================
BIG_STARS_BASE bigStarsBase;

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitBigStars (void)
{
	bigStarsBase.col	= BIG_STARS_COL;
	bigStarsBase.pos	= BIG_STARS_POS;
	bigStarsBase.size	= BIG_STARS_SIZE;
	
	// 頂点フォーマット(中心座標)
	MakeVtxBigStars();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBigStars(void)
{
	// 特にない
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBigStars(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBigStars(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, GetStarsTexture());

	// 頂点座標の設定(中心座標)
	SetVtxBigStars();

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &bigStarsBase.vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点フォーマット(中心座標)
//=============================================================================
void MakeVtxBigStars(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	bigStarsBase.vertexWk[0].vtx.x = bigStarsBase.pos.x - bigStarsBase.size.x;
	bigStarsBase.vertexWk[0].vtx.y = bigStarsBase.pos.y - bigStarsBase.size.y;
	bigStarsBase.vertexWk[0].vtx.z = 0.0f;

	bigStarsBase.vertexWk[1].vtx.x = bigStarsBase.pos.x + bigStarsBase.size.x;
	bigStarsBase.vertexWk[1].vtx.y = bigStarsBase.pos.y - bigStarsBase.size.y;
	bigStarsBase.vertexWk[1].vtx.z = 0.0f;

	bigStarsBase.vertexWk[2].vtx.x = bigStarsBase.pos.x - bigStarsBase.size.x;
	bigStarsBase.vertexWk[2].vtx.y = bigStarsBase.pos.y + bigStarsBase.size.y;
	bigStarsBase.vertexWk[2].vtx.z = 0.0f;

	bigStarsBase.vertexWk[3].vtx.x = bigStarsBase.pos.x + bigStarsBase.size.x;
	bigStarsBase.vertexWk[3].vtx.y = bigStarsBase.pos.y + bigStarsBase.size.y;
	bigStarsBase.vertexWk[3].vtx.z = 0.0f;

	// rhwの設定
	bigStarsBase.vertexWk[0].rhw =
	bigStarsBase.vertexWk[1].rhw =
	bigStarsBase.vertexWk[2].rhw =
	bigStarsBase.vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	bigStarsBase.vertexWk[0].diffuse = bigStarsBase.col;
	bigStarsBase.vertexWk[1].diffuse = bigStarsBase.col;
	bigStarsBase.vertexWk[2].diffuse = bigStarsBase.col;
	bigStarsBase.vertexWk[3].diffuse = bigStarsBase.col;

	// テクスチャ座標の設定
	bigStarsBase.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bigStarsBase.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	bigStarsBase.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	bigStarsBase.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点設定(中心座標)
//=============================================================================
void SetVtxBigStars(void)
{
	// 頂点座標の設定
	bigStarsBase.vertexWk[0].vtx.x = bigStarsBase.pos.x - bigStarsBase.size.x;
	bigStarsBase.vertexWk[0].vtx.y = bigStarsBase.pos.y - bigStarsBase.size.y;
	bigStarsBase.vertexWk[0].vtx.z = 0.0f;

	bigStarsBase.vertexWk[1].vtx.x = bigStarsBase.pos.x + bigStarsBase.size.x;
	bigStarsBase.vertexWk[1].vtx.y = bigStarsBase.pos.y - bigStarsBase.size.y;
	bigStarsBase.vertexWk[1].vtx.z = 0.0f;

	bigStarsBase.vertexWk[2].vtx.x = bigStarsBase.pos.x - bigStarsBase.size.x;
	bigStarsBase.vertexWk[2].vtx.y = bigStarsBase.pos.y + bigStarsBase.size.y;
	bigStarsBase.vertexWk[2].vtx.z = 0.0f;

	bigStarsBase.vertexWk[3].vtx.x = bigStarsBase.pos.x + bigStarsBase.size.x;
	bigStarsBase.vertexWk[3].vtx.y = bigStarsBase.pos.y + bigStarsBase.size.y;
	bigStarsBase.vertexWk[3].vtx.z = 0.0f;
}
