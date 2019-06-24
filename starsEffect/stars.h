//=============================================================================
//
// ���͏��� [stars.h]
// Author : �� �F��
//
//=============================================================================
#ifndef _STARS_H_
#define _STARS_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define STARS_TEXTURE	("data/star.png")
#define STARS_MAX		(180)
#define STARS_SEC_MAX	(STARS_MAX/2)

#define STARS_MID_POS	D3DXVECTOR3(SCREEN_CENTER_X,0.0f,0.0f)
//#define STARS_SIZE		D3DXVECTOR2(20.0f,20.0f)

//=============================================================================
// �\����
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

}STARS_SEC;				// 3�́���1�̃p�����[�^�����L����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitStars(void);		// ������
void	UninitStars(void);		// �I������
void	UpdateStars(void);		// �X�V����
void	DrawStars(void);		// �`�揈��

STARS_BASE *GetStarsBase(int no);
STARS_SEC *GetStarsSec(int no);

#endif