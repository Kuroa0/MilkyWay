//=============================================================================
//
// ���̃G�t�F�N�g���� [bigStars.cpp]
// Author : �� �F��
//
//=============================================================================
#ifndef _BIG_STARS_H_
#define _BIG_STARS_H_

#include "main.h"
#include "stars.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define BIG_STARS_COL	(D3DCOLOR_RGBA(255, 0, 0, 255))
#define BIG_STARS_POS	(D3DXVECTOR3(300.0f,100.0f,0.0f))
#define BIG_STARS_SIZE	(D3DXVECTOR2(100.0f,100.0f))

//=============================================================================
// �\����
//=============================================================================
typedef struct
{
	VERTEX_2D			vertexWk[NUM_VERTEX];
	D3DXVECTOR3			pos;
	D3DXVECTOR2			size;
	D3DXCOLOR			col;
}BIG_STARS_BASE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitBigStars(void);
void UninitBigStars(void);
void UpdateBigStars(void);
void DrawBigStars(void);

#endif
