//=============================================================================
//
// 入力処理 [stars.h]
// Author : 蔡 友剛
//
//=============================================================================
#ifndef _STARS_H_
#define _STARS_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define STARS_TEXTURE	("data/star.png")
#define STARS_MAX		(180)
#define STARS_SEC_MAX	(STARS_MAX/2)

#define STARS_MID_POS	D3DXVECTOR3(SCREEN_CENTER_X,0.0f,0.0f)
//#define STARS_SIZE		D3DXVECTOR2(20.0f,20.0f)

//=============================================================================
// 構造体
//=============================================================================
typedef struct
{
	VERTEX_2D			vertexWk[NUM_VERTEX];
	D3DXVECTOR3			pos;
	bool				use;

}STARS_BASE;

typedef struct
{
	D3DXVECTOR3			rot;
	D3DXVECTOR2			size;
	D3DXCOLOR			col;
	float				alpha;
	float				angle;
	float				rotSpd;

}STARS_SEC;				// 3つの☆を1つのパラメータを共有する

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitStars(void);		// 初期化
void	UninitStars(void);		// 終了処理
void	UpdateStars(void);		// 更新処理
void	DrawStars(void);		// 描画処理

STARS_BASE *GetStarsBase(int no);
STARS_SEC *GetStarsSec(int no);

#endif