#pragma once


//-�w�b�_�[�t�@�C���ǂݍ���-

//�Q�[���̃��C���w�b�_�[�t�@�C��
#include "game.h"


//-�}�N����`-

//�G���[���b�Z�[�W
#define CSV_LOAD_ERR_TITLE		TEXT("CSV�ǂݍ��݃G���[")
#define CSV_DATANUM_ERR_TITLE	TEXT("CSV�f�[�^���G���[")


//�L�����f�[�^�̃p�X
#define CSV_PATH_ENEMY		TEXT(".\\Data\\enemy.csv")

//�f�[�^�̏����w��q
//�L�����N�^�[�̏����w��q
#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d")

//�G�f�[�^
#define ENEMY_MAX	8

//�G���[���b�Z�[�W
#define SCORE_CREATE_ERR_TITLE	TEXT("�X�R�A�f�[�^�쐬�G���[")
#define SCORE_LOAD_ERR_TITLE	TEXT("�X�R�A�f�[�^�Ǎ��G���[")
#define SCORE_CREATE_TITLE		TEXT("�X�R�A�f�[�^�쐬OK!!")
#define SCORE_SAVE_TITLE		TEXT("�X�R�A�f�[�^�ۑ�OK!!")
#define SCORE_DELETE_TITLE		TEXT("�X�R�A�f�[�^�폜OK!!")
#define SCORE_DELETE_ERR_TITLE	TEXT("�X�R�A�f�[�^�폜�G���[")

//�X�R�A�f�[�^�̃p�X
#define SCORE_DATA_PATH	TEXT(".\\data\\monika.chr")	//ANSI�̕����R�[�h

//�f�[�^�̏����w��q
#define SCORE_DATA_FORMAT	TEXT("%[^,],%d,%[^,],%d,%[^,],%d")	//�X�R�A�f�[�^�̏����w��q

//�}�b�v�`�b�v�̉摜�̃t�@�C���p�X
#define IMG_PATH_MAP1				TEXT(".\\map\\mapchip.png")

//�}�b�v�`�b�v�̉摜�̃t�@�C���p�X
#define CSV_PATH_MAP1_SHITA			TEXT(".\\map\\map_sita.csv")
#define CSV_PATH_MAP1_NAKA			TEXT(".\\map\\map_naka.csv")
#define CSV_PATH_MAP1_KAGU			TEXT(".\\map\\map_kagu.csv")
#define CSV_PATH_MAP1_NAKA_ATARI	TEXT(".\\map\\map_naka_atari.csv")
#define CSV_PATH_MAP1_UE			TEXT(".\\map\\map_ue.csv")

#define CSV_PATH_MAP2_SHITA			TEXT(".\\map\\dassyutu_sita.csv")
#define CSV_PATH_MAP2_NAKA			TEXT(".\\map\\dassyutu_naka.csv")
#define CSV_PATH_MAP2_KAGU			TEXT(".\\map\\dassyutu_kagu.csv")
#define CSV_PATH_MAP2_NAKA_ATARI	TEXT(".\\map\\dassyutu_naka_atari.csv")
#define CSV_PATH_MAP2_UE			TEXT(".\\map\\dassyutu_ue.csv")

#define CSV_PATH_MAINMAP_SHITA			TEXT(".\\map\\MainStage_wide_sita.csv")
#define CSV_PATH_MAINMAP_NAKA			TEXT(".\\map\\MainStage_wide_naka.csv")
#define CSV_PATH_MAINMAP_KAGU			TEXT(".\\map\\MainStage_wide_kagu.csv")
#define CSV_PATH_MAINMAP_NAKA_ATARI		TEXT(".\\map\\MainStage_wide_naka_atari.csv")
#define CSV_PATH_MAINMAP_UE				TEXT(".\\map\\MainStage_wide_ue.csv")

//�}�b�v�̏����w��q
#define CSV_MAP_FORMAT		TEXT("%d,")

#define MAP1_YOKO_MAX			64	//�}�b�v�̉��T�C�Y
#define MAP1_TATE_MAX			64	//�}�b�v�̏c�T�C�Y

//#define MAP1_YOKO_MAX			20	//�}�b�v�̉��T�C�Y
//#define MAP1_TATE_MAX			20	//�}�b�v�̏c�T�C�Y

#define MAP1_YOKO_DIV			60	//�}�b�v�̉�������
#define MAP1_TATE_DIV			60	//�}�b�v�̏c������

//#define MAP1_YOKO_DIV			60	//�}�b�v�̉�������
//#define MAP1_TATE_DIV			60	//�}�b�v�̏c������

#define MAP_MOVE_ID				56	//�ʂ��}�b�vID
#define MAP_STOP_ID				57	//�ʂ�Ȃ��}�b�vID
#define MAP_NONE_ID				59	//�����Ȃ��}�b�vID

//�L�����f�[�^�\����(�����܂ŃV���v��)
struct CHARA_DATA
{
	int No;					//No
	char Name[STR_MAX];		//���O

	//���ۂɎg���l
	int HP;					//HP
	int ATK;				//�U����
	int DEF;				//�h���
	
	//�ő�l
	int MAX_HP;				//HP
	int MAX_ATK;			//�U����
	int MAX_DEF;			//�h���
};

struct SCORE_DATA
{
	char path[PATH_MAX];	//�p�X
	char Name1[STR_MAX];	//���O1
	int Score1;				//�X�R�A1
	char Name2[STR_MAX];	//���O2
	int Score2;				//�X�R�A2
	char Name3[STR_MAX];	//���O3
	int Score3;				//�X�R�A3
};

struct MAP_DATA
{
	//CSV�f�[�^(�����摜��ID)
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_kagu[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//�}�b�v�̃n���h��
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//�}�b�v�̏ꏊ
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int divMax;
	
	BOOL IsDraw = FALSE;

	//�����蔻��
	RECT coll[MAP1_TATE_MAX][MAP1_YOKO_MAX];
};


//-�O���̃O���[�o���֐�-
extern CHARA_DATA enemy[ENEMY_MAX];		//�G�f�[�^
extern SCORE_DATA score_data;			//�X�R�A�f�[�^
extern SCORE_DATA score_dataInit;		//�X�R�A�f�[�^�������p


//-�v���g�^�C�v�錾-

extern BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);
extern BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader);

extern BOOL LoadCSVMap(
	const char* ImgPath,
	const char* shitaPath,
	const char* nakaPath,
	const char* kaguPath,
	const char* nakaatariPath,
	const char* uePath,
	MAP_DATA* map,
	int divYoko,
	int divTate);

extern BOOL CreateScoreData(VOID);
extern BOOL SaveScoreData(VOID);
extern BOOL DeleteScoreData(VOID);
extern const char* GetScoreDataPath(VOID);
extern VOID SetScoreDataPath(const char* path);

extern BOOL CollMap(RECT rect, MAP_DATA map);
extern VOID CollMapUpdate(MAP_DATA* map);
extern VOID DrawMap(MAP_DATA map);
extern VOID CreateEventMass(int x, int y, EVENT* events, MAP_DATA map);
extern VOID CreateEventMultiMass(int x1, int y1, int x2, int y2, EVENT* events, MAP_DATA map);
extern VOID MapMove(MAP_DATA* map);