#pragma once

//-�w�b�_�[�t�@�C��-

//�Q�[���̃��C���w�b�_�t�@�C��
#include "game.h"


//-�}�N����`-

//�t�H���g�n
#define FONT_JIYU_PATH		TEXT(".\\Font\\Jiyucho.ttf")				//�t�H���g�̏ꏊ
#define FONT_JIYU_NAME		TEXT("���䂤���傤�t�H���g")				//�t�H���g�̖��O
#define FONT_MUKASI_PATH	TEXT(".\\font\\gomarice_mukasi_mukasi.ttf")	//�t�H���g�̏ꏊ
#define FONT_MUKASI_NAME	TEXT("�́X�ӂ����")						//�t�H���g�̖��O

#define FONT_MSGOTH_NAME	TEXT("MS �S�V�b�N")				//�t�H���g���iMS �S�V�b�N�j

#define DEF_FONT_NAME	FONT_JIYU_NAME				//�f�t�H���g�̃t�H���g��
#define DEF_FONT_SIZE	20							//�f�t�H���g�̃T�C�Y
#define DEF_FONT_THINCK	1							//�f�t�H���g�̑���
#define DEF_FONT_TYPE	DX_FONTTYPE_ANTIALIASING	//�f�t�H���g�̃^�C�v

//�G���[���b�Z�[�W
#define FONT_INSTALL_ERR_TITLE	TEXT("�t�H���g�C���X�g�[���G���[")
#define FONT_CREATE_ERR_TITLE	TEXT("�t�H���g�쐬�G���[")


//-�\����-

//�t�H���g�\����
struct FONT
{
	int handle;			//�t�H���g�n���h��
	int Size;			//�t�H���g�̃T�C�Y
	int Thinck;			//�t�H���g�̑���
	int Type = DX_FONTTYPE_ANTIALIASING;	//�t�H���g�̃^�C�v
	char Name[STR_MAX];	//�t�H���g�̖��O
};


//-�O���[�o���֐�-

//�t�H���g�֘A
extern FONT fontdef;		//�f�t�H���g�̃t�H���g

extern FONT sampleFont1;	//�T���v���̃t�H���g(PC�ɓ����Ă������ȃt�H���g)
extern FONT sampleFont2;	//�T���v���̃t�H���g(�t�H���g�t�@�C������ǂݍ��񂾃t�H���g)
extern FONT sampleFont3;	//�T���v���̃t�H���g(�t�H���g�t�@�C������ǂݍ��񂾃t�H���g)


//-�v���g�^�C�v�錾-

extern BOOL FontAdd(VOID);				//�t�H���g�̓Ǎ�
extern BOOL FontCreate(VOID);			//�t�H���g�쐬
extern VOID SetFont(FONT* f, const char* name, int size, int thinck, int type);	//�t�H���g�ɐݒ肷��
extern BOOL FontCreateHandle(FONT* f);	//�t�H���g�n���h�����쐬
extern VOID SetDefaultFont(FONT f);		//�t�H���g��S�̂ɔ��f������
extern VOID FontDelete(VOID);			//�t�H���g�폜
extern VOID FontRemove(VOID);			//�t�H���g�̍폜