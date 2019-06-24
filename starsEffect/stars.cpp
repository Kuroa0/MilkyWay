//=============================================================================
//
// ���͏��� [stars.cpp]
// Author : �� �F��
//
//=============================================================================
#include "stars.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVtxStars(int no);
void SetVtxStars(int no);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	starsTexture;
STARS_BASE			starsBase[STARS_MAX];
STARS_SEC			starsSecondary[STARS_SEC_MAX];

//=============================================================================
// ������
//=============================================================================
HRESULT InitStars(void)
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(device,	// �f�o�C�X�ւ̃|�C���^
	STARS_TEXTURE,						// �t�@�C���̖��O
	&starsTexture);						// �ǂݍ��ރ������[

	// �p�����[�^�̐ݒ�
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
		
		// ���_�t�H�[�}�b�g(���S���W)
		MakeVtxStars(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStars(void)
{
	RELEASE_TEXTURE(starsTexture)
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStars(void)
{
	// �p�����[�^�̐ݒ�
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
			// �_�ł̑��x(8160=255*32)
			float tmpAlphaSpd = (360.0f / 8160.0f);
			starsSecondary[tmpSecNo].alpha += tmpAlphaSpd;
			if (starsSecondary[tmpSecNo].alpha > 360.0f)
			{
				starsSecondary[tmpSecNo].alpha -= 360.0f;
				starsBase[i].use = false;
			}

			// �����g�œ_�ŏ���
			starsSecondary[tmpSecNo].col.a = sinf(starsSecondary[tmpSecNo].alpha);
		}

		starsSecondary[tmpSecNo].rot.z += starsSecondary[tmpSecNo].rotSpd;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStars(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, starsTexture);

	for (int i = 0; i < STARS_MAX; i++)
	{
		// ���_���W�̐ݒ�(���S���W)
		SetVtxStars(i);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &starsBase[i].vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�ݒ�(���S���W)
//=============================================================================
void SetVtxStars(int no)
{
	// �p�����[�^�̐ݒ�
	int tmpSecNo = (no / 3);

	// ���_���W�̐ݒ�
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

	// ���ˌ��̐ݒ�
	starsBase[no].vertexWk[0].diffuse = starsSecondary[tmpSecNo].col;
	starsBase[no].vertexWk[1].diffuse = starsSecondary[tmpSecNo].col;
	starsBase[no].vertexWk[2].diffuse = starsSecondary[tmpSecNo].col;
	starsBase[no].vertexWk[3].diffuse = starsSecondary[tmpSecNo].col;

}

//=============================================================================
// ���_�t�H�[�}�b�g(���S���W)
//=============================================================================
void MakeVtxStars(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	STARS_BASE *base = GetStarsBase(no);

	int tmpSecNo = (no / 3);

	STARS_SEC *sec = GetStarsSec(tmpSecNo);

	// ���_���W�̐ݒ�
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

	// rhw�̐ݒ�
	base->vertexWk[0].rhw =
	base->vertexWk[1].rhw =
	base->vertexWk[2].rhw =
	base->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	base->vertexWk[0].diffuse = sec->col;
	base->vertexWk[1].diffuse = sec->col;
	base->vertexWk[2].diffuse = sec->col;
	base->vertexWk[3].diffuse = sec->col;

	// �e�N�X�`�����W�̐ݒ�
	base->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	base->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	base->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	base->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���̃x�[�X�����擾
//=============================================================================
STARS_BASE *GetStarsBase(int no)
{
	return &starsBase[no];
}

//=============================================================================
// ����Sec�����擾
//=============================================================================
STARS_SEC *GetStarsSec(int no)
{
	return &starsSecondary[no];
}
