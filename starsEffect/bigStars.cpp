//=============================================================================
//
// ���̃G�t�F�N�g���� [bigStars.cpp]
// Author : �� �F��
//
//=============================================================================
#include "bigStars.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVtxBigStars(void);
void SetVtxBigStars(void);

//=============================================================================
// �O���[�o���ϐ�:
//=============================================================================
BIG_STARS_BASE bigStarsBase;

//=============================================================================
// ������
//=============================================================================
HRESULT InitBigStars (void)
{
	bigStarsBase.col	= BIG_STARS_COL;
	bigStarsBase.pos	= BIG_STARS_POS;
	bigStarsBase.size	= BIG_STARS_SIZE;
	
	// ���_�t�H�[�}�b�g(���S���W)
	MakeVtxBigStars();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBigStars(void)
{
	// ���ɂȂ�
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBigStars(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBigStars(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GetStarsTexture());

	// ���_���W�̐ݒ�(���S���W)
	SetVtxBigStars();

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &bigStarsBase.vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�t�H�[�}�b�g(���S���W)
//=============================================================================
void MakeVtxBigStars(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
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

	// rhw�̐ݒ�
	bigStarsBase.vertexWk[0].rhw =
	bigStarsBase.vertexWk[1].rhw =
	bigStarsBase.vertexWk[2].rhw =
	bigStarsBase.vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	bigStarsBase.vertexWk[0].diffuse = bigStarsBase.col;
	bigStarsBase.vertexWk[1].diffuse = bigStarsBase.col;
	bigStarsBase.vertexWk[2].diffuse = bigStarsBase.col;
	bigStarsBase.vertexWk[3].diffuse = bigStarsBase.col;

	// �e�N�X�`�����W�̐ݒ�
	bigStarsBase.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bigStarsBase.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	bigStarsBase.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	bigStarsBase.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_�ݒ�(���S���W)
//=============================================================================
void SetVtxBigStars(void)
{
	// ���_���W�̐ݒ�
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
