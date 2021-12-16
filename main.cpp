//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���
#include "mouse.h"		//�}�E�X�̏���
#include "shape.h"		//�}�`�̏���
#include "font.h"		//�t�H���g�̏���
#include "Data.h"		//�f�[�^�̏���

//�Ǝ��̃}�N����`
#define MAIN_ITEM_KIND	3

//�Ǝ��̗񋓌^�̒�`
enum MUKI
{
	muki_none,
	muki_ue,
	muki_hidari,
	muki_shita,
	muki_migi
};//�����̃T���v��

//�Ǝ��̍\���̂̒�`


//�������Q�[�����ʂ̃O���[�o���ϐ�������
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

int fadeTimeMill = 2000;					//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX
//�������Q�[�����ʂ̃O���[�o���ϐ�������

float GameTimeLimit=0;
const float GameTimeLimitMax=60;


//�Ǝ��̃O���[�o���ϐ�

int ItemWood;
int ItemStone;

BOOL canGetItem;


int MenuStringLeft;
int MenuStringRight;

//�R���t�B�O�֘A
MENU test[2];

//���j���[��ʂ̃T���v��
MENU sampleGetItemMenu[4];
int sampleGetItemMenuCnt;

MENU GetMainItemMenu[MAIN_ITEM_KIND];
int GetMainItemCnt;

MENU DrawConfig;

int Volumecfg = 0;
BOOL OpenVolumecfg;

int turn;

BOOL PlayerTurn;
BOOL EnemyTurn;
BOOL PlayerResult;
BOOL EnemyResult;


//�������Q�[�����ʂ̃v���g�^�C�v�錾������
BOOL GameLoad(VOID);	//�f�[�^��Ǎ�
VOID GameInit(VOID);	//�f�[�^�̏�����
VOID GameDelete(VOID);	//�f�[�^���폜

VOID TitleInit(VOID);
VOID PlayInit(VOID);
VOID BattleInit(VOID);
VOID EndInit(VOID);
VOID GameOverInit(VOID);

VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID Battle(VOID);		//�v���C���
VOID BattleProc(VOID);	//�v���C���(����)
VOID BattleDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID GameOver(VOID);		//�Q�[���I�[�o�[���
VOID GameOverProc(VOID);	//�Q�[���I�[�o�[���(����)
VOID GameOverDraw(VOID);	//�Q�[���I�[�o�[���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID ChangeScene(GAME_SCENE scene);		//�V�[���؂�ւ�
BOOL OnCollRect(RECT a, RECT b);		//��`�Ƌ�`�̓����蔻��
VOID CollUpdateImage(IMAGE* img);		//�摜�̓����蔻��̍X�V
VOID CollUpdateDivImage(DIVIMAGE* div);	//�����摜�̓����蔻��̍X�V
BOOL LoadImageMem(IMAGE* image, const char* path);										//�Q�[���̉摜��ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType);				//�Q�[���̉��y��ǂݍ���
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);//�Q�[���̉摜�̕����ǂݍ���

VOID PlayAudio(AUDIO audio);					//���y�Đ�
int GetVolumeAudio(AUDIO audio);				//���y�̃{�����[���擾
VOID SetVolumeAudio(AUDIO* audio, int vol);		//���y�̃{�����[���ݒ�
VOID ChangeVolumeAudio(AUDIO* audio, int vol);	//���y�̃{�����[���ύX
VOID StopAudio(AUDIO* audio);					//���y��~

VOID DrawImage(IMAGE image);					//�摜�`��
VOID DrawRect(RECT r, unsigned int color, bool b);	//RECT�`��
VOID DrawDivImage(DIVIMAGE* image);				//�����摜�̕`��
VOID DrawDivImageChara(DIVIMAGE* image);		//�����摜�̕`��

VOID DrawHitBox(EVENT* events);					//�C�x���g�̓����蔻��̕`��

VOID ItemEventInit(EVENT* events, int Wood, int Stone, int CntMax);	//�A�C�e��������

VOID CreateItem(EVENT* events);					//�A�C�e���쐬
VOID CreateItemDraw(EVENT* events);				//�A�C�e���쐬���̕K�v���\��

VOID GetItemSystem(EVENT* events, EVENT* tools);				//�̎�̌��ϓ�
VOID GetItemDraw(EVENT* events);				//�̎掞�̌��\��

VOID SetEventUpdate();							//�C�x���g�̏ꏊ�X�V

//�������Q�[�����ʂ̃v���g�^�C�v�錾������

//�T���v���̑f��
IMAGE sampleImg;
DIVIMAGE sampleDivImg;
DIVIMAGE samplePlayerImg;
MUKI muki = muki_shita;		//�T���v������

IMAGE enemyImg1;

AUDIO sampleBGM;
AUDIO titleBGM;
AUDIO playBGM;
AUDIO battleBGM;
AUDIO endBGM;
AUDIO gameoverBGM;

AUDIO sceneEnterSE;

MAP_DATA map1;
MAP_DATA map2;

//�C�x���g�}�X
EVENT sampleevent;

EVENT CreatePickaxe;
EVENT CreateAxe;
EVENT CreateKey;

EVENT GetItem;
EVENT GetWood;
EVENT GetStone;

EVENT Goal;

EVENT CreateItems;
EVENT sampleGetItem[4];

EVENT GetMainItem[MAIN_ITEM_KIND];	//�΁A�؍ށA����

//���S�Ȃǂ̉摜
IMAGE TitleImg;
IMAGE EndImg;
IMAGE GameoverImg;

IMAGE PushEnter;

BOOL MenuScreen;
BOOL MenuScreenInventory;

BOOL MenuRight;


//�퓬���
CHARA_DATA Battleenemy[ENEMY_MAX];
CHARA_DATA PlayChara;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�t�H���g�ǉ�
	if (FontAdd() == FALSE) { return FALSE; }



	//�Q�[���ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		GameDelete();	//�f�[�^�폜
		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();
	TitleInit();

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//�}�E�X���͂̍X�V
		MouseUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_BATTLE:
			Battle();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_GAMEOVER:
			GameOver();			//�Q�[���I�[�o�[���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//���݂̃}�E�X�̏���`��
		MouseDraw();

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�t�H���g�폜
	FontRemove();

	//�f�[�^�폜
	GameDelete();

	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�T���v���摜��ǂݍ���
	if (LoadImageMem(&sampleImg, ".\\Image\\player.png") == FALSE) { return FALSE; }

	//�T���v�������摜��ǂݍ���
	if (LoadImageDivMem(&sampleDivImg, ".\\Image\\baku1.png", 8, 2) == FALSE) { return FALSE; }

	//�T���v�������摜��ǂݍ���
	if (LoadImageDivMem(&samplePlayerImg, ".\\Image\\charachip1.png", 4, 4) == FALSE) { return FALSE; }

	//�T���v��BGM��ǂݍ���
	if (LoadAudio(&sampleBGM, ".\\Audio\\�u���L��PARADE.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	
	//�t�H���g���쐬
	if (FontCreate() == FALSE) { return FALSE; }

	//�T���v���G�f�[�^��ǂݍ���
	//if (LoadCSVChara(CSV_PATH_ENEMY, &enemy[0], ENEMY_MAX, TRUE) == FALSE) { return FALSE; }

	//�T���v���X�R�A�f�[�^��ǂݍ���
	if (LoadScoreData(SCORE_DATA_PATH, &score_data, TRUE) == FALSE) { return FALSE; }

	//�T���v���}�b�v�f�[�^��ǂݍ���
	if(LoadCSVMap(
		IMG_PATH_MAP1,
		CSV_PATH_MAP1_SHITA,
		CSV_PATH_MAP1_NAKA,
		CSV_PATH_MAP1_KAGU,
		CSV_PATH_MAP1_NAKA_ATARI,
		CSV_PATH_MAP1_UE,
		&map1,
		MAP1_YOKO_DIV,MAP1_TATE_DIV
	) == FALSE) {return FALSE; }

	//BGM��ǂݍ���
	if (LoadAudio(&titleBGM, ".\\audio\\�X���u���u������I��BGM.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&playBGM, ".\\audio\\Just_a_really_bad_feeling.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&battleBGM, ".\\audio\\Final_Judgement_2.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&endBGM, ".\\audio\\�u���[�{�g��.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&gameoverBGM, ".\\audio\\�@���Ȃ��߂��݂Ɣ��藈�閾��.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }

	if (LoadAudio(&sceneEnterSE, ".\\SE\\�V�X�e�����艹_9_2.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }

	//�}�b�v�f�[�^��ǂݍ���
	if(LoadCSVMap(
		IMG_PATH_MAP1,
		CSV_PATH_MAINMAP_SHITA,
		CSV_PATH_MAINMAP_NAKA,
		CSV_PATH_MAINMAP_KAGU,
		CSV_PATH_MAINMAP_NAKA_ATARI,
		CSV_PATH_MAINMAP_UE,
		&map2,
		MAP1_YOKO_DIV,MAP1_TATE_DIV
	) == FALSE) {return FALSE; }

	//���S�ǂݍ���
	if (LoadImageMem(&TitleImg, ".\\Image\\�E�o_�^�C�g��.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&EndImg, ".\\Image\\�E�o_�Q�[���N���A.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&GameoverImg, ".\\Image\\�E�o_�Q�[���I�[�o�[.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&PushEnter, ".\\Image\\�E�o_Enter.png") == FALSE) { return FALSE; }

	//�G�摜��ǂݍ���
	if (LoadImageMem(&enemyImg1, ".\\Image\\monster171_l.png") == FALSE) { return FALSE; }

	//�G�f�[�^��ǂݍ���
	if (LoadCSVChara(CSV_PATH_ENEMY, &Battleenemy[0], ENEMY_MAX, TRUE) == FALSE) { return FALSE; }

	//�v���C���[�f�[�^��ǂݍ���
	if (LoadCSVChara(CSV_PATH_PLAYER, &PlayChara, 1, TRUE) == FALSE) { return FALSE; }


	return TRUE;	//�S�ēǂݍ��݂��I
}

/// <summary>
/// �Q�[���̃f�[�^���폜
/// </summary>
VOID GameDelete(VOID)
{
	//�T���v���摜���폜
	DeleteGraph(sampleImg.handle);

	//�T���v�������摜���폜
	for (int i = 0; i < sampleDivImg.DivMax; i++) { DeleteGraph(sampleDivImg.handle[i]); }

	//�T���v�������摜���폜
	for (int i = 0; i < samplePlayerImg.DivMax; i++) { DeleteGraph(samplePlayerImg.handle[i]); }

	//�T���v�����y���폜
	DeleteMusicMem(sampleBGM.handle);
	DeleteMusicMem(titleBGM.handle);
	DeleteMusicMem(playBGM.handle);
	DeleteMusicMem(battleBGM.handle);
	DeleteMusicMem(endBGM.handle);
	DeleteMusicMem(gameoverBGM.handle);

	DeleteMusicMem(sceneEnterSE.handle);

	//�t�H���g�f�[�^���폜
	FontDelete();

	//�摜���폜
	DeleteGraph(TitleImg.handle);
	DeleteGraph(EndImg.handle);
	DeleteGraph(GameoverImg.handle);
	DeleteGraph(PushEnter.handle);

	return;
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//���y�̉��ʒ���
	SetVolumeAudio(&sampleBGM, 100);
	SetVolumeAudio(&titleBGM, 200);
	SetVolumeAudio(&playBGM, 200);
	SetVolumeAudio(&battleBGM, 200);
	SetVolumeAudio(&endBGM, 200);
	SetVolumeAudio(&gameoverBGM, 200);

	//�Q�[�������ԃ��Z�b�g
	ResetGameTime();

	//�T���v���v���C���[������

	return;
}

VOID TitleInit(VOID)
{
	//�^�C�g�����S�̏�����
	TitleImg.IsDraw = TRUE;
	TitleImg.x = 0;
	TitleImg.y = 0;

	PushEnter.IsDraw = TRUE;
	PushEnter.x = 0;
	PushEnter.y = 0;

	return;
}

VOID PlayInit(VOID)
{
	//�T���v���v���C���[������
	samplePlayerImg.speed = 9;
	samplePlayerImg.x = 20;
	samplePlayerImg.y = 10;
	//samplePlayerImg.x = MAP1_YOKO_MAX * map2.width / 2 - samplePlayerImg.width / 2;
	//samplePlayerImg.y = MAP1_TATE_MAX * map2.height / 2 - samplePlayerImg.height / 2;

	muki = muki_none;

	//�T���v���C�x���g�̍쐬
	//CreateEventMultiMass(9, 1, 11, 2, &sampleevent, map2);

	//�C�x���g�̍쐬
	SetEventUpdate();

	//���Ԑ���
	GameTimeLimit = GameTimeLimitMax;

	//�A�C�e���̌��̏�����
	ItemWood = 0;
	ItemStone = 0;

	canGetItem = TRUE;

	ItemEventInit(&GetItem,1,1,NULL);
	ItemEventInit(&GetWood,5,0,NULL);
	ItemEventInit(&GetStone,0,5,NULL);

	ItemEventInit(&CreatePickaxe, 1, 2, 1);
	ItemEventInit(&CreateAxe, 1, 3, 1);
	ItemEventInit(&CreateKey, 10, 10, 1);
	ItemEventInit(&CreateItems, 0, 0, NULL);

	strcpyDx(sampleGetItem[0].string, "�A�C�e���Ă���1");
	strcpyDx(sampleGetItem[1].string, "�A�C�e���Ă���2");
	strcpyDx(sampleGetItem[2].string, "�A�C�e���Ă���3");
	strcpyDx(sampleGetItem[3].string, "�A�C�e���Ă���4");

	for (int i = 0; i < 4; i++)
	{
		sampleGetItemMenu[i].Cnt = 0;
	}

	//���j���[���J���Ă��Ȃ�
	MenuScreen = FALSE;
	MenuScreenInventory = FALSE;

	MenuStringLeft = 0;
	MenuStringRight = 0;

	MenuRight = FALSE;

	//�R���t�B�O��������

	OpenVolumecfg = FALSE;

	//���ڑ��₷�e�X�g�̂�̏�����
	strcpyDx(DrawConfig.string, "�\����");
	DrawConfig.Cnt = 1;
	DrawConfig.can = FALSE;

	for (int i = 0; i < 3; i++)
	{
		test[i].Cnt = 0;
		test[i].can = FALSE;
	}
	strcpyDx(test[0].string, "�֐��Ă���1");
	strcpyDx(test[1].string, "�֐��Ă���2");
	strcpyDx(test[2].string, "�֐��Ă���3");

	//�C�x���g�̏�����
	for (int i = 0; i < MAIN_ITEM_KIND; i++)
	{
		GetMainItem[i].Cnt = 0;
		GetMainItem[i].can = FALSE;

	}

	//�C�x���g�̕�����R�s�[
	strcpyDx(GetMainItem[0].string, "��");
	strcpyDx(GetMainItem[1].string, "�؍�");
	strcpyDx(GetMainItem[2].string, "����");

	//�擾�����A�C�e���̎�ނ̌��̏�����
	sampleGetItemMenuCnt = 0;
	GetMainItemCnt = 0;

	samplePlayerImg.screenX = 0;
	samplePlayerImg.screenY = 0;
	MapInit(&map2);
	
	return;
}

VOID BattleInit()
{
	//�G������
	enemyImg1.x = GAME_WIDTH / 2 - enemyImg1.width / 2;
	enemyImg1.y = GAME_HEIGHT / 2 - enemyImg1.height / 2 - 50;	//�}�W�b�N�i���o�[
	enemyImg1.IsDraw = TRUE;

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Battleenemy[i].HP = Battleenemy[i].MAX_HP;
		Battleenemy[i].MP = Battleenemy[i].MAX_MP;

	}

	PlayChara.HP = PlayChara.MAX_HP;
	PlayChara.MP = PlayChara.MAX_MP;

	turn = 0;

	PlayerTurn = FALSE;
	PlayerResult = FALSE;
	EnemyTurn = FALSE;
	EnemyResult = FALSE;

	return;
}

VOID EndInit(VOID)
{
	//�G���h��ʂ̏�����
	EndImg.IsDraw = TRUE;
	EndImg.x = 0;
	EndImg.y = 0;

	return;
}

VOID GameOverInit(VOID)
{
	//�Q�[���I�[�o�[��ʂ̏�����
	GameoverImg.IsDraw = TRUE;
	GameoverImg.x = 0;
	GameoverImg.y = 0;

	return;
}

/// <summary>
/// �C�x���g�̍��W�̍X�V
/// </summary>
VOID SetEventUpdate()
{
	//�C�x���g
	{
		CreateEventMass(3, 9, &CreatePickaxe, map2);
		CreateEventMass(16, 9, &CreateAxe, map2);
		CreateEventMass(16, 4, &CreateKey, map2);
		CreateEventMass(41, 29, &CreateItems, map2);

		CreateEventMultiMass(8, 15, 12, 18, &GetItem, map2);
		CreateEventMultiMass(2, 16, 4, 18, &GetWood, map2);
		CreateEventMultiMass(16, 16, 19, 18, &GetStone, map2);

		CreateEventMultiMass(9, 1, 11, 2, &Goal, map2);

		for (int i = 0; i < 4; i++)
		{
			CreateEventMass(15 + 3 * i, 38 + 3 * i, &sampleGetItem[i], map2);
		}

		CreateEventMultiMass(41, 8, 43, 9, &GetMainItem[0], map2);
		CreateEventMultiMass(46, 8, 49, 9, &GetMainItem[1], map2);
		CreateEventMultiMass(53, 9, 55, 10, &GetMainItem[2], map2);

	}
}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	//�Z�[�u�f�[�^�n�T���v��
	{
		//�Z�[�u�f�[�^�폜�T���v��
		if (KeyDown(KEY_INPUT_Q)) { DeleteScoreData(); }

		//�Z�[�u�f�[�^�ۑ��T���v��
		if (KeyDown(KEY_INPUT_E))
		{
			//�X�R�A�f�[�^��ݒ�
			score_data.Score1 = (int)GetGameTime();
			strcpyDx(score_data.Name1, "Taro");
			strcpyDx(score_data.Name2, "Jiro");
			strcpyDx(score_data.Name3, "Saburo");

			//�X�R�A�f�[�^�̕��ёւ�

			//�X�R�A�f�[�^�ۑ�
			SaveScoreData();
		}
	}


	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//SE�𗬂�
		PlayAudio(sceneEnterSE);
		//���y���~�߂�
		StopAudio(&titleBGM);

		//�Q�[���̏�����
		GameInit();
		PlayInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	PlayAudio(titleBGM);	//BGM��炷

	//�v���C���[�̓���T���v��
	/*
	{
		muki = muki_none;	//�ŏ��͌����𖳂��ɂ���
		if (KeyDown(KEY_INPUT_W)) { muki = muki_ue; samplePlayerImg.y--; }
		else if (KeyDown(KEY_INPUT_S)) { muki = muki_shita; samplePlayerImg.y++; }
		if (KeyDown(KEY_INPUT_A)) { muki = muki_hidari; samplePlayerImg.x--; }
		else if (KeyDown(KEY_INPUT_D)) { muki = muki_migi; samplePlayerImg.x++; }
		CollUpdateDivImage(&samplePlayerImg);	//�����蔻��̍X�V
	}
	*/

	

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{

	/*
	DrawImage(sampleImg);				//�T���v���摜�̕`��
	DrawDivImage(&sampleDivImg);		//�T���v�������摜�̕`��

	//�v���C���[�̓���T���v��
	{
		DrawDivImageChara(&samplePlayerImg);//�T���v�������摜�̕`��
	}

	//�Q�[��������
	DrawFormatString(500, 50, GetColor(0, 0, 0), "TIME:%3.2f", GetGameTime());

	//�Q�[��������
	DrawFormatString(500, 30, GetColor(0, 0, 0), "�c��:%3.2f", GameTimeLimit);

	//���݂̓��t�Ǝ���
	DrawFormatString(500, 70, GetColor(0, 0, 0), "DATE:%4d/%2d/%2d %2d:%2d:%2d", fps.NowDataTime.Year, fps.NowDataTime.Mon, fps.NowDataTime.Day, fps.NowDataTime.Hour, fps.NowDataTime.Min, fps.NowDataTime.Sec);

	//�t�H���g�̃T���v��
	DrawStringToHandle(100, 100, "MS �S�V�b�N����", GetColor(0, 0, 0), sampleFont1.handle);
	
	//�t�H���g�̃T���v��
	DrawStringToHandle(100, 150, "�́X�ӂ���Ƃ���", GetColor(0, 0, 0), sampleFont3.handle);

	//���l���o�������Ƃ�
	DrawFormatStringToHandle(800, 200, GetColor(0, 0, 0), sampleFont2.handle, "�c��:%3.2f",GameTimeLimit);
	
	//�ǂݍ��񂾃f�[�^��`��
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		DrawFormatString(300, 300 + i * 20, GetColor(0, 0, 0), "%s,%2d,%2d,%2d"
			, enemy[i].Name, enemy[i].HP, enemy[i].ATK, enemy[i].DEF);
	}
	*/

	DrawImage(TitleImg);
	DrawImage(PushEnter);

	if(GAME_DEBUG)DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	PlayAudio(playBGM);	//BGM��炷
	if (Volumecfg == 0)
	{
		Volumecfg = GetVolumeAudio(playBGM);
	}
	SetVolumeAudio(&playBGM, Volumecfg);

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�Q�[���f�[�^�̏�����
		GameInit();
		BattleInit();

		//SE�𗬂�
		PlayAudio(sceneEnterSE);
		//���y���~�߂�
		StopAudio(&playBGM);

		//�퓬��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_BATTLE);

		return;
	}

	


	
	//X���������Ƃ��̊֐����]
	if (KeyClick(KEY_INPUT_X))
	{
		if(OpenVolumecfg==TRUE)
		{ OpenVolumecfg = FALSE;}
		else if(DrawConfig.can==TRUE)
		{ DrawConfig.can = FALSE;}
		else if (MenuScreen == TRUE && MenuRight == FALSE && MenuScreenInventory==TRUE)
		{
			MenuScreen = FALSE;
			MenuScreenInventory = FALSE;
		}
		else if (MenuRight == TRUE)
		{ MenuRight = FALSE;}
		else if (MenuScreen == FALSE)
		{
			MenuScreen = TRUE;
			MenuScreenInventory = TRUE;
		}
	}

	//���j���[���J���Ă��Ȃ�(�ʏ펞)��
	if (MenuScreen == FALSE)
	{
		//�}�b�v�̓����蔻��
		{
			muki = muki_none;					//�ŏ��͌����Ȃ�
			DIVIMAGE dummy = samplePlayerImg;	//�����蔻��̃_�~�[
			DIVIMAGE dummy2 = samplePlayerImg;	//�����蔻��̃_�~�[
			if (KeyDown(KEY_INPUT_UP)) { muki = muki_ue; dummy.y -= samplePlayerImg.speed; }
			else if (KeyDown(KEY_INPUT_DOWN)) { muki = muki_shita; dummy.y += samplePlayerImg.speed; }

			CollUpdateDivImage(&dummy);	//�����蔻��̍X�V
			CollMapUpdate(&map2);		//�}�b�v�̓����蔻��X�V

			for(int i=0;i<samplePlayerImg.speed;i++)
			{
				//�������Ă��Ȃ��̂Ȃ�
				if (CollMap(dummy.coll, map2) == FALSE)
				{
					samplePlayerImg.y = dummy.y;	//�_�~�[�̏���߂�
					break;
				}
				//dummy����̍��W������Ȃ�
				else if(dummy.y < dummy2.y)
				{
					//���ɂ��炷
					dummy.y++;
				}
				//���Ȃ�
				else if (dummy.y > dummy2.y)
				{
					//��ɂ��炷
					dummy.y--;
				}
				CollUpdateDivImage(&dummy);	//�����蔻��̍X�V
			}

			dummy = samplePlayerImg;	//�����蔻��̃_�~�[

			if (KeyDown(KEY_INPUT_LEFT)) { muki = muki_hidari; dummy.x -= samplePlayerImg.speed; }
			else if (KeyDown(KEY_INPUT_RIGHT)) { muki = muki_migi; dummy.x += samplePlayerImg.speed; }

			CollUpdateDivImage(&dummy);	//�����蔻��̍X�V
			CollMapUpdate(&map2);		//�}�b�v�̓����蔻��X�V

			for (int i = 0; i < samplePlayerImg.speed; i++)
			{
				if (CollMap(dummy.coll, map2) == TRUE)
				{
					//�������Ă������Adummy2��player��y�ʒu���Ⴄ�Ȃ�����C��
					if (samplePlayerImg.y != dummy2.y)
					{
						if (samplePlayerImg.y < dummy2.y) { muki = muki_ue; }
						else { muki = muki_shita; }
					}
				}
				//�������Ă��Ȃ��Ȃ�
				else if (CollMap(dummy.coll, map2) == FALSE)
				{
					samplePlayerImg.x = dummy.x;	//�_�~�[�̏���߂�
					break;
				}

				//dummy����̍��W�������Ȃ�
				if (dummy.x < dummy2.x)
				{
					//�E�ɂ��炷
					dummy.x++;
				}
				//�E�Ȃ�
				else if (dummy.x > dummy2.x)
				{
					//���ɂ��炷
					dummy.x--;
				}
				CollUpdateDivImage(&dummy);	//�����蔻��̍X�V
			}
			//��ʒ[�ɂ������ꍇ
			/*if (samplePlayerImg.y < map2.y[0][0]) { samplePlayerImg.y = map2.y[0][0]; }	//���m�F
			if (samplePlayerImg.y > map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] - 20)
			{
				samplePlayerImg.y = map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] - 20;	//�}�W�b�N�i���o�[
			}

			if (samplePlayerImg.x < map2.x[0][0]) { samplePlayerImg.x = map2.x[0][0]; }
			if (samplePlayerImg.x > map2.x[MAP1_TATE_MAX-1][MAP1_YOKO_MAX-1])
			{
				samplePlayerImg.x = map2.x[MAP1_TATE_MAX-1][MAP1_YOKO_MAX-1];
			}*/

			//�ړ�������̂��
			DIVIMAGE dummy3 = samplePlayerImg;

			//�v���C���[�̕��̍l������,�v�C��
			if (samplePlayerImg.x > GAME_WIDTH / 2
				&& map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_WIDTH && dummy2.x != samplePlayerImg.x)
			{
				samplePlayerImg.screenX = samplePlayerImg.x - dummy2.x;
				samplePlayerImg.x = dummy2.x;	//�_�~�[�̏���߂�
			}
			else if (samplePlayerImg.x < GAME_WIDTH / 2
				&& map2.x[0][0] < 0 && dummy2.x != samplePlayerImg.x)
			{
				samplePlayerImg.screenX = samplePlayerImg.x - dummy2.x;
				samplePlayerImg.x = dummy2.x;	//�_�~�[�̏���߂�
			}
			else
			{
				samplePlayerImg.screenX = 0;
			}

			if (samplePlayerImg.y > GAME_HEIGHT / 2
				&& map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_HEIGHT && dummy2.y != samplePlayerImg.y)
			{
				samplePlayerImg.screenY = samplePlayerImg.y - dummy2.y;
				samplePlayerImg.y = dummy2.y;	//�_�~�[�̏���߂�
			}
			else if (samplePlayerImg.y < GAME_HEIGHT / 2
				&& map2.y[0][0] < 0 && dummy2.y != samplePlayerImg.y)
			{
				samplePlayerImg.screenY = samplePlayerImg.y - dummy2.y;
				samplePlayerImg.y = dummy2.y;	//�_�~�[�̏���߂�
			}
			else
			{
				samplePlayerImg.screenY = 0;
			}

			MapMove(&map2);		//�}�b�v�̈ړ�
			CollMapUpdate(&map2);		//�}�b�v�̓����蔻��X�V
			SetEventUpdate();		//�C�x���g�̍��W�̍X�V

			CollUpdateDivImage(&samplePlayerImg);	//�����蔻��̍X�V

			//�C�x���g�}�X�ɓ������Ă��邩
			if (CheckCollRectToRect(samplePlayerImg.coll, Goal.coll) == TRUE && CreateKey.Cnt > 0)
			{
				//�Q�[���f�[�^�̏�����
				GameInit();
				EndInit();

				//�}�b�v�ړ��̃t���O
				//sampleevent.can = TRUE;

				//���y���~�߂�
				StopAudio(&playBGM);

				//�v���C��ʂɐ؂�ւ�
				ChangeScene(GAME_SCENE_END);
			}

			CreateItem(&CreatePickaxe);
			CreateItem(&CreateAxe);
			CreateItem(&CreateKey);

			GetItemSystem(&GetItem, NULL);
			GetItemSystem(&GetWood, &CreateAxe);
			GetItemSystem(&GetStone, &CreatePickaxe);

			//�A�C�e����ǉ��ō��ڑ��₷�T���v���C�x���g
			if (CheckCollRectToRect(samplePlayerImg.coll, CreateItems.coll))
			{
				if (KeyClick(KEY_INPUT_Z))
				{
					//�`�搔�𑝂₷
					DrawConfig.Cnt++;
				}
			}

			//�C�x���g��z��ŊǗ�����T���v��
			for (int i = 0; i < 4; i++)
			{
				if (CheckCollRectToRect(samplePlayerImg.coll, sampleGetItem[i].coll))
				{
					if (KeyClick(KEY_INPUT_Z))
					{
						for (int j = 0; j < 4; j++)
						{
							if (!strcmpDx(sampleGetItemMenu[j].string, sampleGetItem[i].string))
							{
								sampleGetItemMenu[j].Cnt++;
								break;
							}
							else if (j == 3)
							{
								//������R�s�[
								strcpyDx(sampleGetItemMenu[sampleGetItemMenuCnt].string, sampleGetItem[i].string);
								sampleGetItemMenu[sampleGetItemMenuCnt].Cnt++;
								sampleGetItemMenuCnt++;
								break;
							}
						}

						
					}
				}
			}

			//�f�ރA�C�e������
			for (int i = 0; i < MAIN_ITEM_KIND; i++)
			{
				//�����蔻��
				if (CheckCollRectToRect(samplePlayerImg.coll, GetMainItem[i].coll))
				{
					//Z����������
					if (KeyClick(KEY_INPUT_Z))
					{
						for (int j = 0; j < MAIN_ITEM_KIND; j++)
						{
							//�T��(strcmpDx�̎d�l�ǂ��킩���Ă��Ȃ��A�Ƃ肠�������삵���̂ł��̂܂�)
							if (!strcmpDx(GetMainItemMenu[j].string, GetMainItem[i].string))
							{
								GetMainItemMenu[j].Cnt++;
								break;
							}
							//���݂��ĂȂ��A�Ō�܂ōs�����ꍇ
							else if (j == MAIN_ITEM_KIND - 1)
							{
								//������R�s�[
								strcpyDx(GetMainItemMenu[GetMainItemCnt].string, GetMainItem[i].string);
								GetMainItemMenu[GetMainItemCnt].Cnt++;
								GetMainItemCnt++;
								break;
							}
						}

						
					}
				}
			}


		}

		GameTimeLimit -= fps.DeltaTime;
		if (GameTimeLimit <= 0)
		{
			//�Q�[���f�[�^�̏�����
			GameInit();
			GameOverInit();

			//���y���~�߂�
			StopAudio(&playBGM);

			//�v���C��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_GAMEOVER);
		}
	}
	//���j���[���J���Ă���Ƃ�
	else if(MenuScreen==TRUE)
	{
		//���j���[�̑���
		if(MenuScreenInventory==TRUE)
		{
			if (KeyClick(KEY_INPUT_DOWN))
			{
				if (MenuRight == FALSE)
				{
					MenuStringLeft++;
					MenuStringRight = 0;
				}
				if (MenuRight == TRUE)
					MenuStringRight++;
			}
			else if (KeyClick(KEY_INPUT_UP))
			{
				if (MenuRight == FALSE)
				{
					MenuStringLeft--;
					MenuStringRight = 0;
				}
				if (MenuRight == TRUE)
					MenuStringRight--;
			}

			//��
			//���̍��ڂ�����𒴂��ď�ɍs�����Ƃ�(�̂���define��)
			if (MenuStringLeft < 0)
			{
				MenuStringLeft = 5;
			}
			//���̍��ڂ�����𒴂��ĉ��ɍs�����Ƃ�
			else if (MenuStringLeft > 5)
			{
				MenuStringLeft = 0;
			}
			//���̈ꍀ�ږڂ̎�
			if (MenuStringLeft == 0)
			{
				//�E
				//�������������(�ʂ�define�p�Ӑ���)
				if (MenuStringRight < 0)
				{
					MenuStringRight = 2;
				}
				//�������������
				else if (MenuStringRight > 2)
				{
					MenuStringRight = 0;
				}
			}
			//���̓񍀖ږڂ̎�
			if (MenuStringLeft == 1)
			{
				//�E
				if (MenuStringRight < 0)
				{
					MenuStringRight = 2;
				}
				else if (MenuStringRight > 2)
				{
					MenuStringRight = 0;
				}
			}
			//���̎O���ږڂ̎�
			if (MenuStringLeft == 2)
			{
				//�E
				if (MenuStringRight < 0)
				{
					MenuStringRight = 2;
				}
				else if (MenuStringRight > 2)
				{
					MenuStringRight = 0;
				}
			}
			if (MenuStringLeft == 3)
			{
				//�E
				//�����I�Ƀo�[�ō��ڐ��𑝂₹��悤��
				if (MenuStringRight < 0)
				{
					MenuStringRight = DrawConfig.Cnt - 1;
				}
				else if (MenuStringRight > DrawConfig.Cnt - 1)
				{
					MenuStringRight = 0;
				}
			}
			//���̌܍��ږڂ̎�
			if (MenuStringLeft == 4)
			{
				//�E
				if (MenuStringRight < 0)
				{
					MenuStringRight = sampleGetItemMenuCnt-1;
				}
				else if (MenuStringRight > sampleGetItemMenuCnt-1)
				{
					MenuStringRight = 0;
				}
			}
			//���̘Z���ږڂ̎�
			if (MenuStringLeft == 5)
			{
				//�E
				if (MenuStringRight < 0)
				{
					MenuStringRight = GetMainItemCnt-1;
				}
				else if (MenuStringRight > GetMainItemCnt-1)
				{
					MenuStringRight = 0;
				}
			}

			//����{�^�����������Ƃ�
			if (KeyClick(KEY_INPUT_Z))
			{
				//�����ڂɂ���Ƃ�
				if (MenuRight == FALSE)
				{
					//�E�Ɉړ�
					MenuRight = TRUE;
				}
				//�E���ڂɂ���Ƃ�
				else
				{


					//�O�s��
					if (MenuStringLeft == 2)
					{
						//�O�s��
						if (MenuStringRight == 2)
						{
							//������������Ƃ���BOOL�^�𔽓]
							if (OpenVolumecfg == FALSE)
								OpenVolumecfg = TRUE;
							else
								OpenVolumecfg = FALSE;
						}
					}
					//�l�s��
					if (MenuStringLeft == 3)
					{
						//��s��
						if (MenuStringRight == 0)
						{
							//������������Ƃ���BOOL�^�𔽓]
							if (DrawConfig.can == FALSE)
								DrawConfig.can = TRUE;
							else
								DrawConfig.can = FALSE;
						}
					}
				}
			}

			//���ʒ���
			if (OpenVolumecfg == TRUE)
			{
				if (KeyDown(KEY_INPUT_LEFT))
				{
					Volumecfg--;
				}
				else if (KeyDown(KEY_INPUT_RIGHT))
				{
					Volumecfg++;
				}

				if (Volumecfg < 0)
					Volumecfg = 0;
				else if (Volumecfg > 255)
					Volumecfg = 255;
			}

			//���ڑ��₷�e�X�g
			if (DrawConfig.can == TRUE)
			{
				if (KeyClick(KEY_INPUT_LEFT))
				{
					DrawConfig.Cnt--;
				}
				else if (KeyClick(KEY_INPUT_RIGHT))
				{
					DrawConfig.Cnt++;
				}

				if (DrawConfig.Cnt < 0)
					DrawConfig.Cnt = 0;
				else if (DrawConfig.Cnt > 255)
					DrawConfig.Cnt = 255;
			}

		}
	}
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�}�b�v�̃T���v��
	DrawMap(map2);

	//�����蔻��̕`��
	//DrawHitBox(&sampleevent);

	DrawHitBox(&CreatePickaxe);
	DrawHitBox(&CreateAxe);
	DrawHitBox(&CreateKey);

	DrawHitBox(&GetItem);
	DrawHitBox(&GetWood);
	DrawHitBox(&GetStone);

	DrawHitBox(&Goal);

	//�T���v��
	DrawHitBox(&CreateItems);
	for (int i = 0; i < 4; i++) { DrawHitBox(&sampleGetItem[i]); }

	for (int i = 0; i < MAIN_ITEM_KIND; i++) { DrawHitBox(&GetMainItem[i]); }



	//���l���o�������Ƃ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(930, GAME_HEIGHT - 90, GAME_WIDTH, GAME_HEIGHT, GetColor(50, 50, 50), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatStringToHandle(950, GAME_HEIGHT - 75, GetColor(0, 0, 0), sampleFont2.handle, "�c��:%3.2f", GameTimeLimit);

	DrawFormatStringToHandle(650, 40, GetColor(0, 0, 0), sampleFont1.handle, "�s�b�P��:%d\n���@�@�@:%d\n���@�@�@:%d\n\n��:%d�@��:%d",CreatePickaxe.Cnt,CreateAxe.Cnt,CreateKey.Cnt,ItemWood,ItemStone);

	CreateItemDraw(&CreatePickaxe);
	CreateItemDraw(&CreateAxe);
	CreateItemDraw(&CreateKey);

	GetItemDraw(&GetItem);
	GetItemDraw(&GetWood);
	GetItemDraw(&GetStone);

	if(MenuScreen==TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawBox(GAME_WIDTH / 6, GAME_HEIGHT / 6, GAME_WIDTH * 5 / 6, GAME_HEIGHT * 5 / 6, GetColor(200, 200, 200), TRUE);

		if(MenuScreenInventory==TRUE)
		{
			//���u���b�N
			if (MenuRight == FALSE)
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);
			else
			{
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}


			if (MenuStringLeft != 0)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "�Ă���1", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "�Ă���1", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 1)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "�Ă���2", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "�Ă���2", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 2)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "�Ă���3", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "�Ă���3", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 3)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "�Ă���4", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "�Ă���4", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 4)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 100, "�Ă���5", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 100, "�Ă���5", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 5)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 120, "�f�ރA�C�e��", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 120, "�f�ރA�C�e��", GetColor(200, 200, 200), FALSE);


			//�E�u���b�N
			if (MenuRight == FALSE)
			{
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}
			else
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			//�E���̕��� �����̂ŗv�C��
			if (MenuStringLeft == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					//for���ŕ\��������e�X�g
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(200, 200, 200), "�Ă���%d", i + 4);
					else
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(100, 100, 100), "�Ă���%d", i + 4);
				}
			}
			if (MenuStringLeft == 1)
			{
				for (int i = 0; i < 3; i++)
				{
					//�z��̊֐���\��������e�X�g
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), test[i].string, GetColor(200, 200, 200), FALSE);
					else
						DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), test[i].string, GetColor(100, 100, 100), FALSE);
				}

			}
			if (MenuStringLeft == 2)
			{
				if (MenuStringRight == 0 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "�Ă���10", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "�Ă���10", GetColor(100, 100, 100), FALSE);

				if (MenuStringRight == 1 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 40, "�}�b�v�g�嗦", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 40, "�}�b�v�g�嗦", GetColor(100, 100, 100), FALSE);

				if (MenuStringRight == 2 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 60, "����", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 60, "����", GetColor(100, 100, 100), FALSE);



				//���ʒ��߂̃o�[
				if (OpenVolumecfg == TRUE)
				{
					DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 80, GetColor(0, 0, 0), "%3d / 255", Volumecfg);
					DrawBox(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 100, GAME_WIDTH / 3 + 275, GAME_HEIGHT / 6 + 120, GetColor(0, 0, 0), TRUE);
					DrawBox(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 100, GAME_WIDTH / 3 + 20 + Volumecfg, GAME_HEIGHT / 6 + 120, GetColor(0, 0, 255), TRUE);
				}

			}

			if (MenuStringLeft == 3)
			{
				for (int i = 0; i < DrawConfig.Cnt; i++)
				{
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), DrawConfig.string, GetColor(200, 200, 200), FALSE);
					else
						DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), DrawConfig.string, GetColor(100, 100, 100), FALSE);
				}

				if (DrawConfig.can == TRUE)
				{
					DrawFormatString(GAME_WIDTH / 3 + 220, GAME_HEIGHT / 6 + 20, GetColor(0, 0, 0), "%3d / 255", DrawConfig.Cnt);
					DrawBox(GAME_WIDTH / 3 + 220, GAME_HEIGHT / 6 + 40, GAME_WIDTH / 3 + 475, GAME_HEIGHT / 6 + 60, GetColor(0, 0, 0), TRUE);
					DrawBox(GAME_WIDTH / 3 + 220, GAME_HEIGHT / 6 + 40, GAME_WIDTH / 3 + 220 + DrawConfig.Cnt, GAME_HEIGHT / 6 + 60, GetColor(0, 0, 255), TRUE);
				}
			}
			if (MenuStringLeft == 4)
			{
				//���肵�����ɃA�C�e����ǉ����Ă����T���v��
				for (int i = 0; i < sampleGetItemMenuCnt; i++)
				{
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(200, 200, 200), "%16s x%d", sampleGetItemMenu[i].string, sampleGetItemMenu[i].Cnt);
					else
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(100, 100, 100), "%16s x%d", sampleGetItemMenu[i].string, sampleGetItemMenu[i].Cnt);
				}
			}
			//�f�ރA�C�e���\��
			if (MenuStringLeft == 5)
			{
				//���肵�����ɃA�C�e����ǉ����Ă���
				for (int i = 0; i < GetMainItemCnt; i++)
				{
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(200, 200, 200), "%-14s x%d", GetMainItemMenu[i].string, GetMainItemMenu[i].Cnt);
					else
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(100, 100, 100), "%-14s x%d", GetMainItemMenu[i].string, GetMainItemMenu[i].Cnt);
				}
			}
		}
	}
	//else if(MenuScreen==FALSE)
	//{SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }

	
	if (GAME_DEBUG)DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

VOID Battle()
{
	BattleProc();
	BattleDraw();

	return;
}

VOID BattleProc()
{
	PlayAudio(battleBGM);
	SetVolumeAudio(&battleBGM, Volumecfg);

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�Q�[���f�[�^�̏�����
		GameInit();
		EndInit();

		//SE�𗬂�
		PlayAudio(sceneEnterSE);
		//���y���~�߂�
		StopAudio(&battleBGM);

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		return;
	}

	if (KeyClick(KEY_INPUT_SPACE) == TRUE)
	{
		//�Q�[���f�[�^�̏�����
		GameInit();
		GameOverInit();

		//SE�𗬂�
		PlayAudio(sceneEnterSE);
		//���y���~�߂�
		StopAudio(&battleBGM);

		//�Q�[���I�[�o�[��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_GAMEOVER);

		return;
	}

	if (KeyClick(KEY_INPUT_Z))
	{
		if (PlayerTurn == TRUE && PlayerResult == TRUE)
		{
			PlayerTurn = FALSE;
			PlayerResult = FALSE;
			EnemyTurn = TRUE;
		}
		else if (EnemyTurn == TRUE && EnemyResult == TRUE)
		{
			EnemyTurn = FALSE;
			EnemyResult = FALSE;
			turn++;
		}
		else if (PlayerTurn == TRUE)
		{
			PlayerResult = TRUE;
		}
		else if (EnemyTurn == TRUE)
		{
			EnemyResult = TRUE;
		}
		else if (PlayerTurn == FALSE && EnemyTurn == FALSE)
		{
			PlayerTurn = TRUE;
		}

		if (PlayerResult == TRUE)
		{
			Battleenemy[0].HP--;
		}
		if (EnemyResult == TRUE)
		{
			Battleenemy[0].MP-=2;
			PlayChara.HP--;
		}
	}

	return;
}

VOID BattleDraw()
{
	//�G�̕`��
	DrawImage(enemyImg1);

	//�G�̃o�[�\��
	DrawBox(enemyImg1.x + enemyImg1.width, enemyImg1.y + enemyImg1.height / 2 - 35, enemyImg1.x + enemyImg1.width + 280, enemyImg1.y + enemyImg1.height / 2 + 35, GetColor(230, 230, 230), TRUE);

	//HP
	DrawBox(enemyImg1.x + enemyImg1.width+75, enemyImg1.y + enemyImg1.height / 2 - 25, enemyImg1.x + enemyImg1.width + 275, enemyImg1.y + enemyImg1.height / 2 - 10, GetColor(150, 150, 150), TRUE);
	DrawBox(enemyImg1.x + enemyImg1.width+75, enemyImg1.y + enemyImg1.height / 2 - 25, enemyImg1.x + enemyImg1.width + 75 + ((Battleenemy[0].HP*100/Battleenemy[0].MAX_HP)*2)
		, enemyImg1.y + enemyImg1.height / 2 - 10, GetColor(255, 0, 0), TRUE);
	DrawFormatString(enemyImg1.x + enemyImg1.width+10, enemyImg1.y + enemyImg1.height / 2 - 27, GetColor(0, 0, 0), "%2d/%2d", Battleenemy[0].HP, Battleenemy[0].MAX_HP);

	//MP
	DrawBox(enemyImg1.x + enemyImg1.width+75, enemyImg1.y + enemyImg1.height / 2 + 25, enemyImg1.x + enemyImg1.width + 275, enemyImg1.y + enemyImg1.height / 2 + 10, GetColor(150, 150, 150), TRUE);
	DrawBox(enemyImg1.x + enemyImg1.width+75, enemyImg1.y + enemyImg1.height / 2 + 25, enemyImg1.x + enemyImg1.width + 75 + ((Battleenemy[0].MP * 100 / Battleenemy[0].MAX_MP) * 2)
		, enemyImg1.y + enemyImg1.height / 2 + 10, GetColor(0, 0, 255), TRUE);
	DrawFormatString(enemyImg1.x + enemyImg1.width+10, enemyImg1.y + enemyImg1.height / 2 + 8, GetColor(0, 0, 0), "%2d/%2d", Battleenemy[0].MP, Battleenemy[0].MAX_MP);
	//MP������


	//�����̃E�B���h�E+�o�[
	DrawBox(180, GAME_HEIGHT - 150, GAME_WIDTH - 200, GAME_HEIGHT - 50, GetColor(230, 230, 230), TRUE);
	DrawString(190, GAME_HEIGHT - 135, "HP", GetColor(200, 0, 0), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 135, GetColor(0, 0, 0), "%2d/%2d", PlayChara.HP,PlayChara.MAX_HP);

	DrawString(190, GAME_HEIGHT - 85, "MP", GetColor(0, 0, 200), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 85, GetColor(0, 0, 0), "%2d/%2d", PlayChara.MP, PlayChara.MAX_MP);

	DrawBox(495, GAME_HEIGHT - 145, GAME_WIDTH - 205, GAME_HEIGHT - 55, GetColor(200, 200, 200), TRUE);
	//DrawString(520, GAME_HEIGHT - 130, "�Ă���", GetColor(0, 0, 0), FALSE);

	DrawBox(280, GAME_HEIGHT - 150 + 10, 480, GAME_HEIGHT - 150 + 40, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 10, 280+ ((PlayChara.HP * 100 / PlayChara.MAX_HP) * 2), GAME_HEIGHT - 150 + 40, GetColor(255, 0, 0), TRUE);

	DrawBox(280, GAME_HEIGHT - 150 + 60, 480, GAME_HEIGHT - 150 + 90, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 60, 280 + ((PlayChara.MP * 100 / PlayChara.MAX_MP) * 2), GAME_HEIGHT - 150 + 90, GetColor(0, 0, 255), TRUE);

	//���O�\��
	DrawBox(GAME_WIDTH - 350, 50, GAME_WIDTH - 50, 190, GetColor(200, 200, 200), TRUE);

	if (PlayerTurn)
		DrawString(520, GAME_HEIGHT - 130, "�v���C���[�̂��������I", GetColor(0, 0, 0), FALSE);
	if(PlayerResult)
		DrawString(520, GAME_HEIGHT - 110, "�G�ɂP�_���[�W�I", GetColor(0, 0, 0), FALSE);
	if (EnemyTurn)
		DrawString(520, GAME_HEIGHT - 130, "�G�̂��������I", GetColor(0, 0, 0), FALSE);
	if(EnemyResult)
		DrawString(520, GAME_HEIGHT - 110, "�v���C���[�ɂP�_���[�W�I", GetColor(0, 0, 0), FALSE);


	//�^�[���\�� (�f�o�b�O����\��)
	if (GAME_DEBUG)
	{
		DrawFormatString(20, 40, GetColor(0, 0, 0), "���[��:%d", turn);

		DrawFormatString(20, 60, GetColor(0, 0, 0), "%d,%s,%d,%d,%d,%d", Battleenemy[0].No, Battleenemy[0].Name, Battleenemy[0].HP, Battleenemy[0].MP, Battleenemy[0].ATK, Battleenemy[0].DEF);
		
		DrawFormatString(20, 80, GetColor(0, 0, 0), "%d,%s,%d,%d,%d,%d", PlayChara.No, PlayChara.Name, PlayChara.HP, PlayChara.MP, PlayChara.ATK, PlayChara.DEF);

		DrawFormatString(20, 100, GetColor(0, 0, 0), "Player:%d/%d\nEnemy:%d/%d", PlayerTurn, PlayerResult, EnemyTurn, EnemyResult);
	}
	if (GAME_DEBUG)DrawString(0, 0, "�퓬���", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	//���y���Đ�
	PlayAudio(endBGM);

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�Q�[���f�[�^�̏�����
		GameInit();
		TitleInit();

		//SE�𗬂�
		PlayAudio(sceneEnterSE);
		//���y���~�߂�
		StopAudio(&endBGM);

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawImage(EndImg);

	//���l���o�������Ƃ�
	DrawFormatStringToHandle(900, 620, GetColor(0, 0, 0), sampleFont2.handle, "����:%3.2f", GameTimeLimitMax - GameTimeLimit);

	if (GAME_DEBUG)DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �Q�[���I�[�o�[���
/// </summary>
VOID GameOver(VOID)
{
	GameOverProc();	//����
	GameOverDraw();	//�`��

	return;
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̏���
/// </summary>
VOID GameOverProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//������
		GameInit();
		TitleInit();

		//SE�𗬂�
		PlayAudio(sceneEnterSE);
		//���y���~�߂�
		StopAudio(&gameoverBGM);

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//���y���Đ�
	PlayAudio(gameoverBGM);

	return;
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̕`��
/// </summary>
VOID GameOverDraw(VOID)
{
	DrawImage(GameoverImg);

	//���l���o�������Ƃ�
	DrawFormatStringToHandle(900, 620, GetColor(0, 0, 0), sampleFont2.handle, "����:%3.2f", GameTimeLimitMax - GameTimeLimit);

	if (GAME_DEBUG)DrawString(0, 0, "�Q�[���I�[�o�[���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}


	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_BATTLE:
		BattleDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	case GAME_SCENE_GAMEOVER:
		GameOverDraw();
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (GAME_DEBUG)DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�摜�j
/// </summary>
/// <param name="img">�摜�\���̂̃|�C���^</param>
VOID CollUpdateImage(IMAGE* img)
{
	img->coll.left = img->x;
	img->coll.top = img->y;

	img->coll.right = img->x + img->width;
	img->coll.bottom = img->y + img->height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�����摜�j
/// </summary>
/// <param name="img">�摜�\���̂̃|�C���^</param>
VOID CollUpdateDivImage(DIVIMAGE* div)
{
	div->coll.left = div->x + 5;
	div->coll.top = div->y + 10;

	div->coll.right = div->x + div->width - 5;
	div->coll.bottom = div->y + div->height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`A</param>
/// <param name="b">��`B</param>
/// <returns>����������TRUE/������Ȃ��Ȃ�FALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right &&		//�@��`A�̍���X���W < ��`B�̉E��X���W�@����
		a.right > b.left &&		//�@��`A�̉E��X���W > ��`B�̍���X���W�@����
		a.top  < b.bottom &&	//�@��`A�̏��Y���W < ��`B�̉���Y���W�@����
		a.bottom > b.top		//�@��`A�̉���Y���W > ��`B�̏��Y���W
		)
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return FALSE;
	}
}


/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunkatuYoko">��������Ƃ��̉��̐�</param>
/// <param name="bunkatuTate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate)
{
	int IsLoad = -1;	//�摜���ǂݍ��݂����H

	//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
	int TempHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TempHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	int Width = -1;		//��
	int Height = -1;	//����
	GetGraphSize(TempHandle, &Width, &Height);

	//�������ēǂݍ���
	IsLoad = LoadDivGraph(
		path,							//�摜�̃p�X
		bunkatuYoko * bunkatuTate,		//��������
		bunkatuYoko, bunkatuTate,					//���̕���,�c�̕���
		Width / bunkatuYoko, Height / bunkatuTate,	//�摜1���̕�,����
		div->handle									//�A���ŊǗ�����z��̐擪�A�h���X
	);

	//�����G���[
	if (IsLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�����G���[",		//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//����ݒ�
	div->DivYoko = bunkatuYoko;
	div->DivTate = bunkatuTate;
	div->DivMax = div->DivYoko * div->DivTate;
	div->x = 0;
	div->y = 0;
	GetGraphSize(div->handle[0], &div->width, &div->height);
	div->AnimCnt = 0;
	div->IsAnimLoop = FALSE;
	div->nowIndex = 0;

	//�����蔻��̍X�V
	CollUpdateDivImage(div);

	//�摜��`�悷��
	div->IsDraw = TRUE;

	//�ꎞ�I�ɓǂݍ��񂾃n���h�������
	DeleteGraph(TempHandle);

	return TRUE;
}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�S�[���̉摜��ǂݍ���
	strcpyDx(image->path, path);			//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�摜�̈ʒu��������
	image->x = 0; image->y = 0;

	//�����蔻��̍X�V
	CollUpdateImage(image);

	//�摜��`�悷��
	image->IsDraw = TRUE;

	//�ǂݍ��߂�
	return TRUE;
}

/// <summary>
/// �摜�`��
/// </summary>
/// <param name="image">�`�悷��摜�\����</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawImage(IMAGE image)
{
	//�摜���`��ł���Ƃ���
	if (image.IsDraw == TRUE)
	{
		DrawGraph(image.x, image.y, image.handle, TRUE);
	}

	//�f�o�b�O���[�h�̂Ƃ���
	if (GAME_DEBUG == TRUE)
	{
		//�����蔻���`��
		DrawRect(image.coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// �����摜�̕`��
/// </summary>
/// <param name="image">�`�悷��摜�\����</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImage(DIVIMAGE* image)
{
	//�摜���`��ł���Ƃ���
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		//�A�j���[�V�����̃J�E���g�A�b�v
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{
			//���ׂẴA�j���[�V������`�悵�I�������
			if (image->nowIndex == image->DivMax - 1)
			{
				//�A�j���[�V�������J��Ԃ��Ȃ��Ȃ��
				if (image->IsAnimLoop == FALSE)
				{
					image->IsDraw = FALSE;	//�`����~�߂�
				}
				image->AnimCnt = 0;		//�J�E���^0�N���A
				image->nowIndex = 0;	//�擪�ɖ߂�
			}
			//���̕����摜������Ƃ���
			else if (image->nowIndex < image->DivMax - 1)
			{
				image->nowIndex++; //���̉摜��
			}
			image->AnimCnt = 0;	//�J�E���^0�N���A
		}
	}

	//�f�o�b�O���[�h�̂Ƃ���
	if (GAME_DEBUG == TRUE)
	{
		//�����蔻���`��
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// �����摜�̕`��(�L�����Ȃ�)
/// </summary>
/// <param name="image">�`�悷��摜�\����</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImageChara(DIVIMAGE* image)
{
	//�摜���`��ł���Ƃ���
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		if (muki == muki_none)
		{
			image->AnimCnt = image->AnimCntMAX - 1;
		}

		//�A�j���[�V�����̃J�E���g�A�b�v
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{
			//�E�����̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
			if (muki == muki_migi)
			{
				if (image->nowIndex >= 8 && image->nowIndex < 11 )
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 8;
				}
			}

			//�������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
			if (muki == muki_hidari)
			{
				if (image->nowIndex >= 4 && image->nowIndex < 7)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 4;
				}
			}

			//������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
			if (muki == muki_ue)
			{
				if (image->nowIndex >= 12 && image->nowIndex < 15)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 12;
				}
			}

			//�������̂Ƃ�(�摜�ɂ���Đ������Ⴄ�̂ŁA���킹�邱�ƁI)
			if (muki == muki_shita)
			{
				if (image->nowIndex >= 0 && image->nowIndex < 3)
				{
					image->nowIndex++;
				}
				else
				{
					image->nowIndex = 0;
				}
			}

			image->AnimCnt = 0;	//�J�E���^0�N���A
		}

		//���������̂Ƃ��́A���O�̌����̐^�񒆂̉摜�ɂ���(�摜�ɍ��킹�Č��߂ăl)
		if (muki == muki_none)
		{
			if (image->nowIndex >= 8 && image->nowIndex <= 11) { image->nowIndex = 8; }
			if (image->nowIndex >= 4 && image->nowIndex <= 7) { image->nowIndex = 4; }
			if (image->nowIndex >= 12 && image->nowIndex <= 15) { image->nowIndex = 12; }
			if (image->nowIndex >= 0 && image->nowIndex <= 3) { image->nowIndex = 0; }
		}
	}

	//�f�o�b�O���[�h�̂Ƃ���
	if (GAME_DEBUG == TRUE)
	{
		//�����蔻���`��
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);					//�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);		//���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,			//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//���̑��̐ݒ�
	audio->Volume = Volume;
	audio->playType = playType;

	return TRUE;
}

/// <summary>
/// ���y�Đ�
/// </summary>
/// <param name="audio">�Đ����鉹�y�\����</param>
/// <returns></returns>
VOID PlayAudio(AUDIO audio)
{
	//���[�v�Đ��łȂ���΁A���̂܂܍Đ�����
	if (audio.playType == DX_PLAYTYPE_BACK)
	{
		PlaySoundMem(audio.handle, audio.playType);//���y�̍Đ�
	}
	//���[�v�Đ��̂Ƃ��́A���y���Đ�����Ă��Ȃ���΁A�Đ�����
	else if (audio.playType == DX_PLAYTYPE_LOOP)
	{
		if (CheckSoundMem(audio.handle) == 0)
		{
			PlaySoundMem(audio.handle, audio.playType);//���y�̍Đ�
		}
	}

	return;
}

/// <summary>
/// ���y�̃{�����[���擾
/// </summary>
/// <param name="audio">�擾���鉹�y�\����</param>
/// <param name = "volume">�{�����[��(0�`255)</param>
/// <returns></returns>
int GetVolumeAudio(AUDIO audio)
{
	//���ʂ��擾
	return audio.Volume;
}

/// <summary>
/// ���y�̃{�����[���ݒ�
/// </summary>
/// <param name="audio">�ݒ肷�鉹�y�\����</param>
/// <param name = "volume">�{�����[��(0�`255)</param>
/// <returns></returns>
VOID SetVolumeAudio(AUDIO* audio, int vol)
{
	//���ʂ�ݒ�
	audio->Volume = vol;
	ChangeVolumeSoundMem(audio->Volume, audio->handle);

	return;
}

/// <summary>
/// ���y�̃{�����[���ύX
/// </summary>
/// <param name="audio">�ύX���鉹�y�\����</param>
/// <param name = "volume">�{�����[��(0�`255)</param>
/// <returns></returns>
VOID ChangeVolumeAudio(AUDIO* audio, int vol)
{
	//���ʂ�ݒ�i�\���̂̃{�����[���͕ύX���Ȃ��j
	ChangeVolumeSoundMem(vol, audio->handle);

	return;
}

/// <summary>
/// ���y��~
/// </summary>
/// <param name="audio">��~���鉹�y�\����</param>
/// <returns></returns>
VOID StopAudio(AUDIO* audio)
{
	//���y�̒�~
	StopSoundMem(audio->handle);

	return;
}

/// <summary>
/// �C�x���g�̓����蔻��̕`��
/// </summary>
/// <param name="events"></param>
VOID DrawHitBox(EVENT* events)
{
	if(GAME_DEBUG==TRUE)
		DrawBox(events->coll.left, events->coll.top, events->coll.right, events->coll.bottom, GetColor(0, 255, 0), FALSE);

	return;
}

/// <summary>
/// �A�C�e���쐬
/// </summary>
VOID CreateItem(EVENT* events)
{
	if (CheckCollRectToRect(samplePlayerImg.coll, events->coll) == TRUE && events->Cnt < events->CntMax)
	{
		events->can = TRUE;
		if (KeyClick(KEY_INPUT_Z))
		{
			if (ItemWood >= events->Wood && ItemStone >= events->Stone)
			{
				ItemWood -= events->Wood;
				ItemStone -= events->Stone;
				events->Cnt++;
			}
		}
	}
	else
	{
		events->can = FALSE;
	}

	return;
}

/// <summary>
/// �N���t�g���̃A�C�e���K�v���̕`��
/// </summary>
VOID CreateItemDraw(EVENT* events)
{
	//�N���t�g
	{
		if (events->can == TRUE)
		{
			DrawFormatStringToHandle(900, 40, GetColor(0, 0, 0), sampleFont1.handle,
				"�N���t�g�K�v��\n��: %d / %d\n��: %d / %d", ItemWood, events->Wood, ItemStone, events->Stone);
		}
	}

	return;
}

/// <summary>
/// �A�C�e�����̏�����(���ŃA�C�e���ƍ̎�n�_�̊֐����ꉻ)
/// </summary>
/// <param name="events">�C�x���g�̊֐�</param>
/// <param name="Wood">�؂̐�</param>
/// <param name="Stone">�΂̐�</param>
/// <param name="CntMax">�A�C�e���̍ő及����/NULL�Ȃ����Ȃ�(��)</param>
VOID ItemEventInit(EVENT* events,int Wood,int Stone,int CntMax)
{
	events->Wood = Wood;
	events->Stone = Stone;
	if (CntMax != NULL)
	{
		events->Cnt = 0;
		events->CntMax = CntMax;
	}
	return;
}

/// <summary>
/// �̎掞�̃A�C�e���̉��Z(�Ȉ�)
/// </summary>
/// <param name="events">�̎�C�x���g</param>
/// <param name="tools">�K�v�c�[���̃C�x���g�֐�</param>
VOID GetItemSystem(EVENT* events, EVENT* tools)
{
	if (CheckCollRectToRect(samplePlayerImg.coll, events->coll) == TRUE)
	{
		if (tools == NULL || tools->Cnt > 0)
		{
			events->can = TRUE;
			canGetItem = TRUE;

			if (KeyClick(KEY_INPUT_Z))
			{
				ItemWood += events->Wood;
				ItemStone += events->Stone;
			}
		}
		else
		{
			events->can = TRUE;
			canGetItem = FALSE;
		}
	}
	else
	{
		events->can = FALSE;
	}

	return;
}

/// <summary>
/// �̎�A�C�e���쐬
/// </summary>
/// <param name="events">�̎�C�x���g</param>
VOID GetItemDraw(EVENT* events)
{
	{
		if (events->can == TRUE)
		{
			if (canGetItem == TRUE)
			{
				DrawFormatStringToHandle(900, 40, GetColor(0, 0, 0), sampleFont1.handle,
					"�̎��\n��: %d\n��: %d", events->Wood, events->Stone);
			}
			else
			{
				DrawFormatStringToHandle(900, 40, GetColor(0, 0, 0), sampleFont1.handle,
					"�̎�s��");
			}
		}
	}

	return;
}

