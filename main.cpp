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
#define CRAFTING_ITEM_KIND 3

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
int MenuStringLeft;
int MenuStringRight;

//�R���t�B�O�֘A

//���j���[��ʂ̃T���v��
MENU GetMainItemMenu[MAIN_ITEM_KIND];
int GetMainItemCnt;

MENU GetCraftingItemMenu[CRAFTING_ITEM_KIND];	//�C���x���g��
MENU CraftingItemMenu[CRAFTING_ITEM_KIND];		//�N���t�g���
int CraftingItemCnt;							//���j���[�̃J�E���g

int Volumecfg = 0;
BOOL OpenVolumecfg;

int turn;

BOOL PlayerTurn;
BOOL EnemyTurn;
BOOL PlayerResult;
BOOL EnemyResult;

int Damage;


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

VOID SetEventUpdate();							//�C�x���g�̏ꏊ�X�V
VOID StatusUpdate(MENU tool);					//�X�e�[�^�X�̕ϓ��p
VOID SetCharaCmd(CHARA_CMD* cmd,const char name[255],int cost,float multi,BOOL myself);		//�Z�̐ݒ�

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
AUDIO enterSE;
AUDIO returnSE;
AUDIO cursorSE;
AUDIO collectSE;
AUDIO beepSE;

//�퓬SE
AUDIO BattleSE[6];

MAP_DATA map1;
MAP_DATA map2;

//�C�x���g�}�X
EVENT CreateItems;

EVENT GetMainItem[MAIN_ITEM_KIND];	//�΁A�؍ށA����

//���S�Ȃǂ̉摜
IMAGE TitleImg;
IMAGE TitleLogo;
IMAGE BattleImg;
IMAGE EndImg;
IMAGE EndLogo;
IMAGE GameoverImg;
IMAGE GameoverLogo;

IMAGE PushEnter;

BOOL MenuScreen;
BOOL MenuScreenInventory;
BOOL MenuScreenCrafting;

BOOL MenuRight;

int GetItemCnt;
int GetItemCntMax;

//�퓬���
CHARA_DATA Battleenemy[ENEMY_MAX];
CHARA_DATA PlayChara;

char BattleLog[4][255];
char BattleText[255];

//�Z�̑I��
BOOL BattleMenuUp;
BOOL BattleMenuLeft;
int BattlePlayCmd;
BOOL MPsmall;

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
	if (LoadAudio(&enterSE, ".\\SE\\�V�X�e�����艹_3.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&returnSE, ".\\SE\\�L�����Z��2.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&cursorSE, ".\\SE\\�J�[�\���ړ�8.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&collectSE, ".\\SE\\����A�{�^������29.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&beepSE, ".\\SE\\�r�[�v��.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }

	if (LoadAudio(&BattleSE[0], ".\\SE\\BattleSE\\�_���[�W��01.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[1], ".\\SE\\BattleSE\\�d���p���`2.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[2], ".\\SE\\BattleSE\\�Ō�6.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[3], ".\\SE\\BattleSE\\���Ŏa��1.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[4], ".\\SE\\BattleSE\\�Ή����@1.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[5], ".\\SE\\BattleSE\\�񕜖��@1.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }

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
	if (LoadImageMem(&TitleImg, ".\\Image\\�^�C�g���w�i.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&TitleLogo, ".\\Image\\�^�C�g�����S.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&BattleImg, ".\\Image\\field_d.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&EndImg, ".\\Image\\GAME_CLEAR�w�i.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&EndLogo, ".\\Image\\�Q�[���N���A���S.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&GameoverImg, ".\\Image\\GAME_OVER�w�i.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&GameoverLogo, ".\\Image\\�Q�[���I�[�o�[���S.png") == FALSE) { return FALSE; }
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
	DeleteMusicMem(enterSE.handle);
	DeleteMusicMem(returnSE.handle);
	DeleteMusicMem(cursorSE.handle);
	DeleteMusicMem(collectSE.handle);

	for (int i = 0; i < 6; i++)
	{
		DeleteMusicMem(BattleSE[i].handle);
	}

	//�t�H���g�f�[�^���폜
	FontDelete();

	//�摜���폜
	DeleteGraph(TitleImg.handle);
	DeleteGraph(TitleLogo.handle);
	DeleteGraph(BattleImg.handle);
	DeleteGraph(EndImg.handle);
	DeleteGraph(EndLogo.handle);
	DeleteGraph(GameoverImg.handle);
	DeleteGraph(GameoverLogo.handle);
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

	TitleLogo.IsDraw = TRUE;
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;
	TitleLogo.y = TitleLogo.height / 6;

	PushEnter.IsDraw = TRUE;
	PushEnter.x = 0;
	PushEnter.y = 0;

	//��̃X�e�[�^�X�ɏ�����
	PlayChara.MAX_HP = PlayChara.DEFAULT_HP;
	PlayChara.MAX_MP = PlayChara.DEFAULT_MP;
	PlayChara.MAX_ATK = PlayChara.DEFAULT_ATK;
	PlayChara.MAX_DEF = PlayChara.DEFAULT_DEF;

	//�X�e�[�^�X������
	PlayChara.HP = PlayChara.MAX_HP;
	PlayChara.MP = PlayChara.MAX_MP;
	PlayChara.ATK = PlayChara.MAX_ATK;
	PlayChara.DEF = PlayChara.MAX_DEF;

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
	ItemEventInit(&CreateItems, 0, 0, NULL);

	//���j���[���J���Ă��Ȃ�
	MenuScreen = FALSE;
	MenuScreenInventory = FALSE;

	MenuStringLeft = 0;
	MenuStringRight = 0;

	MenuRight = FALSE;

	//�R���t�B�O��������

	OpenVolumecfg = FALSE;

	//�C�x���g�̏�����
	for (int i = 0; i < MAIN_ITEM_KIND; i++)
	{
		GetMainItem[i].Cnt = 0;
		GetMainItem[i].can = FALSE;
	}

	//�C�x���g�̕�����R�s�[
	strcpyDx(GetMainItem[0].string, "�؍�");
	strcpyDx(GetMainItem[1].string, "��");
	strcpyDx(GetMainItem[2].string, "����");

	//�C�x���g�̕����񏉊���
	for (int i = 0; i < MAIN_ITEM_KIND; i++)
	{
		strcpyDx(GetMainItemMenu[i].string,GetMainItem[i].string);
		GetMainItemMenu[i].Cnt = 0;
	}

	//�N���t�g�̃��j���[�̏�����
	for (int i = 0; i < CRAFTING_ITEM_KIND; i++)
	{
		CraftingItemMenu[i].Cnt = 0;
		CraftingItemMenu[i].can = FALSE;
	}

	strcpyDx(CraftingItemMenu[0].string, "�s�b�P��");
	strcpyDx(CraftingItemMenu[1].string, "�A�b�N�X");
	strcpyDx(CraftingItemMenu[2].string, "�\�[�h");

	//�N���t�g�K�v�f��(�̂��Ɋ֐���)
	CraftingItemMenu[0].Wood = 1;
	CraftingItemMenu[0].Stone = 3;
	CraftingItemMenu[0].Metal = 0;

	CraftingItemMenu[1].Wood = 3;
	CraftingItemMenu[1].Stone = 2;
	CraftingItemMenu[1].Metal = 1;

	CraftingItemMenu[2].Wood = 1;
	CraftingItemMenu[2].Stone = 1;
	CraftingItemMenu[2].Metal = 5;

	//����̃X�e�[�^�X
	CraftingItemMenu[0].HP = 3;
	CraftingItemMenu[0].MP = 1;
	CraftingItemMenu[0].ATK = 0;
	CraftingItemMenu[0].DEF = 0;

	CraftingItemMenu[1].HP = 0;
	CraftingItemMenu[1].MP = 1;
	CraftingItemMenu[1].ATK = 0;
	CraftingItemMenu[1].DEF = 3;

	CraftingItemMenu[2].HP = 0;
	CraftingItemMenu[2].MP = 1;
	CraftingItemMenu[2].ATK = 3;
	CraftingItemMenu[2].DEF = 0;

	//�C���x���g��
	for (int i = 0; i < CRAFTING_ITEM_KIND; i++)
	{
		strcpyDx(GetCraftingItemMenu[i].string, "");
		GetCraftingItemMenu[i].Cnt = 0;
		GetCraftingItemMenu[i].can = FALSE;
		GetCraftingItemMenu[i].HP = 0;
		GetCraftingItemMenu[i].MP = 0;
		GetCraftingItemMenu[i].ATK = 0;
		GetCraftingItemMenu[i].DEF = 0;
	}

	//�擾�����A�C�e���̎�ނ̌��̏�����
	GetMainItemCnt = 0;
	CraftingItemCnt = 0;

	samplePlayerImg.screenX = 0;
	samplePlayerImg.screenY = 0;
	MapInit(&map2);

	//�̎��
	GetItemCnt = 0;
	GetItemCntMax = 30;
	
	return;
}

VOID BattleInit()
{
	//�w�i������
	BattleImg.IsDraw = TRUE;
	BattleImg.x = 0;
	BattleImg.y = 0;

	//�G������
	enemyImg1.x = GAME_WIDTH / 2 - enemyImg1.width / 2;
	enemyImg1.y = GAME_HEIGHT / 2 - enemyImg1.height / 2 - 50;	//�}�W�b�N�i���o�[
	enemyImg1.IsDraw = TRUE;

	CollUpdateImage(&enemyImg1);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Battleenemy[i].MAX_HP = Battleenemy[i].DEFAULT_HP;
		Battleenemy[i].MAX_MP = Battleenemy[i].DEFAULT_MP;
		Battleenemy[i].HP = Battleenemy[i].MAX_HP;
		Battleenemy[i].MP = Battleenemy[i].MAX_MP;
	}

	PlayChara.HP = PlayChara.MAX_HP;
	PlayChara.MP = PlayChara.MAX_MP;
	PlayChara.ATK = PlayChara.MAX_ATK;
	PlayChara.DEF = PlayChara.MAX_DEF;

	turn = 0;

	PlayerTurn = FALSE;
	PlayerResult = FALSE;
	EnemyTurn = FALSE;
	EnemyResult = FALSE;

	Damage = 0;

	strcpyDx(BattleLog[0], "�Ă���1");
	strcpyDx(BattleLog[1], "�Ă���2");
	strcpyDx(BattleLog[2], "�Ă���3");
	strcpyDx(BattleLog[3], "�Ă���4");

	SetCharaCmd(&PlayChara.cmd[0], "��������", 0, 1, FALSE);
	SetCharaCmd(&PlayChara.cmd[1], "���肳��", 1, 1.2, FALSE);
	SetCharaCmd(&PlayChara.cmd[2], "�t�@�C�A", 4, 2, FALSE);
	SetCharaCmd(&PlayChara.cmd[3], "�q�[��", 5, 2, TRUE);

	BattleMenuUp = TRUE;
	BattleMenuLeft = TRUE;
	BattlePlayCmd = 0;
	MPsmall = FALSE;

	return;
}

VOID EndInit(VOID)
{
	//�G���h��ʂ̏�����
	EndImg.IsDraw = TRUE;
	EndImg.x = 0;
	EndImg.y = 0;

	EndLogo.IsDraw = TRUE;
	EndLogo.x = GAME_WIDTH / 2 - EndLogo.width / 2;
	EndLogo.y = GAME_HEIGHT / 2 - EndLogo.height / 2;

	return;
}

VOID GameOverInit(VOID)
{
	//�Q�[���I�[�o�[��ʂ̏�����
	GameoverImg.IsDraw = TRUE;
	GameoverImg.x = 0;
	GameoverImg.y = 0;

	GameoverLogo.IsDraw = TRUE;
	GameoverLogo.x = GAME_WIDTH / 2 - GameoverLogo.width / 2;
	GameoverLogo.y = GAME_HEIGHT / 2 - GameoverLogo.height / 2;

	return;
}

/// <summary>
/// �C�x���g�̍��W�̍X�V
/// </summary>
VOID SetEventUpdate()
{
	//�C�x���g
	{
		CreateEventMass(41, 29, &CreateItems, map2);

		CreateEventMultiMass(46, 8, 49, 9, &GetMainItem[0], map2);
		CreateEventMultiMass(41, 8, 43, 9, &GetMainItem[1], map2);
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
	CollUpdateImage(&TitleLogo);

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

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawImage(TitleImg);
	DrawImage(TitleLogo);
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
	
	//X���������Ƃ��̊֐����] + a
	if (KeyClick(KEY_INPUT_X))
	{
		if(OpenVolumecfg==TRUE)
		{
			OpenVolumecfg = FALSE;
			PlayAudio(returnSE);
		}
		else if (MenuScreen == TRUE && MenuRight == FALSE && MenuScreenInventory==TRUE)
		{
			MenuScreen = FALSE;
			MenuScreenInventory = FALSE;
			MenuStringLeft = 0;
			PlayAudio(returnSE);
		}
		else if (MenuScreen == TRUE && MenuRight == FALSE && MenuScreenCrafting==TRUE)
		{
			MenuScreen = FALSE;
			MenuScreenCrafting = FALSE;
			MenuStringLeft = 0;
			PlayAudio(returnSE);
		}
		else if (MenuRight == TRUE)
		{
			MenuRight = FALSE;
			PlayAudio(returnSE);
		}
		else if (MenuScreen == FALSE)
		{
			MenuScreen = TRUE;
			MenuScreenInventory = TRUE;
			PlayAudio(enterSE);
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

			//�v���C���[�̕��̍l������,�v�C��
			if (samplePlayerImg.x > GAME_WIDTH / 2
				&& map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_WIDTH - map2.width && dummy2.x != samplePlayerImg.x)
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
				&& map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_HEIGHT - map2.height && dummy2.y != samplePlayerImg.y)
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

			if (map2.x[0][0] > 0)
			{
				samplePlayerImg.screenX = map2.x[0][0];
			}
			else if (map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] < GAME_WIDTH - map2.width)
			{
				samplePlayerImg.screenX = -(GAME_WIDTH - map2.width - map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1]);
			}
			else
			{
				samplePlayerImg.screenX = 0;
			}

			if (map2.y[0][0] > 0)
			{
				samplePlayerImg.screenY = map2.y[0][0];
			}
			else if (map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] < GAME_HEIGHT - map2.height)
			{
				samplePlayerImg.screenY = -(GAME_HEIGHT - map2.height - map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1]);
			}
			else
			{
				samplePlayerImg.screenY = 0;
			}

			MapMove(&map2);		//�}�b�v�̈ړ�

			CollMapUpdate(&map2);		//�}�b�v�̓����蔻��X�V
			SetEventUpdate();		//�C�x���g�̍��W�̍X�V

			CollUpdateDivImage(&samplePlayerImg);	//�����蔻��̍X�V

			//�N���t�g�C�x���g
			if (CheckCollRectToRect(samplePlayerImg.coll, CreateItems.coll))
			{
				if (KeyClick(KEY_INPUT_Z))
				{
					PlayAudio(enterSE);
					MenuScreen = TRUE;
					MenuScreenCrafting = TRUE;
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
						PlayAudio(collectSE);
						for (int j = 0; j < MAIN_ITEM_KIND; j++)
						{
							//�T��(strcmpDx�̎d�l�ǂ��킩���Ă��Ȃ��A�Ƃ肠�������삵���̂ł��̂܂�)
							if (!strcmpDx(GetMainItemMenu[j].string, GetMainItem[i].string))
							{
								GetMainItemMenu[j].Cnt++;
								GetItemCnt++;
								break;
							}
						}
					}
				}
			}
		}

		if (GetItemCnt == GetItemCntMax)
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
				PlayAudio(cursorSE);
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
				PlayAudio(cursorSE);
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
				MenuStringLeft = 3;
			}
			//���̍��ڂ�����𒴂��ĉ��ɍs�����Ƃ�
			else if (MenuStringLeft > 3)
			{
				MenuStringLeft = 0;
			}
			//����1���ږڂ̎�
			if (MenuStringLeft == GAME_MENU_MATERIAL)
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
			//����2���ږڂ̎�
			if (MenuStringLeft == GAME_MENU_CRAFTITEM)
			{
				//�E
				if (MenuStringRight < 0)
				{
					MenuStringRight = CraftingItemCnt-1;
				}
				else if (MenuStringRight > CraftingItemCnt-1)
				{
					MenuStringRight = 0;
				}
			}
			//����3���ږڂ̎�(����悤�Ȃ̂œ����Ȃ�)
			if (MenuStringLeft == GAME_MENU_STATUS)
			{

			}
			//����4���ږڂ̎�
			if (MenuStringLeft == GAME_MENU_CONFIG)
			{
				//�E
				if (MenuStringRight < 0)
				{
					MenuStringRight = 0;
				}
				else if (MenuStringRight > 0)
				{
					MenuStringRight = 0;
				}
			}

			//����{�^�����������Ƃ�
			if (KeyClick(KEY_INPUT_Z))
			{
				PlayAudio(enterSE);
				//�����ڂɂ���Ƃ�
				if (MenuRight == FALSE)
				{
					//�E�Ɉړ�
					MenuRight = TRUE;
				}
				//�E���ڂɂ���Ƃ�
				else
				{
					PlayAudio(enterSE);
					//4�s��
					if (MenuStringLeft == GAME_MENU_CONFIG)
					{
						//������������Ƃ���BOOL�^�𔽓]
						if (OpenVolumecfg == FALSE)
							OpenVolumecfg = TRUE;
						else
							OpenVolumecfg = FALSE;
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
		}
		//�N���t�g���
		else if (MenuScreenCrafting == TRUE)
		{
			//��
			if (KeyClick(KEY_INPUT_DOWN))
			{
				PlayAudio(cursorSE);
				if (MenuRight == FALSE)
				{
					MenuStringLeft++;
					MenuStringRight = 0;
				}
				if (MenuRight == TRUE)
					MenuStringRight++;
			}
			//��
			else if (KeyClick(KEY_INPUT_UP))
			{
				PlayAudio(cursorSE);
				if (MenuRight == FALSE)
				{
					MenuStringLeft--;
					MenuStringRight = 0;
				}
				if (MenuRight == TRUE)
					MenuStringRight--;
			}

			//��
			//���̍��ڂ�����𒴂��ď�ɍs�����Ƃ�
			if (MenuStringLeft < 0)
			{
				MenuStringLeft = CRAFTING_ITEM_KIND - 1;
			}
			//���̍��ڂ�����𒴂��ĉ��ɍs�����Ƃ�
			else if (MenuStringLeft > CRAFTING_ITEM_KIND - 1)
			{
				MenuStringLeft = 0;
			}

			//����L�[���������Ƃ�
			if (KeyClick(KEY_INPUT_Z))
			{
				if (CraftingItemMenu[MenuStringLeft].Wood <= GetMainItemMenu[0].Cnt
					&& CraftingItemMenu[MenuStringLeft].Stone <= GetMainItemMenu[1].Cnt
					&& CraftingItemMenu[MenuStringLeft].Metal <= GetMainItemMenu[2].Cnt
					&& CraftingItemMenu[MenuStringLeft].Cnt == 0)
				{
					PlayAudio(enterSE);
					{
						//������R�s�[
						strcpyDx(GetCraftingItemMenu[CraftingItemCnt].string, CraftingItemMenu[MenuStringLeft].string);
						GetCraftingItemMenu[CraftingItemCnt].Cnt++;
						CraftingItemMenu[MenuStringLeft].Cnt++;

						//�f�ޏ���
						GetMainItemMenu[0].Cnt -= CraftingItemMenu[MenuStringLeft].Wood;
						GetMainItemMenu[1].Cnt -= CraftingItemMenu[MenuStringLeft].Stone;
						GetMainItemMenu[2].Cnt -= CraftingItemMenu[MenuStringLeft].Metal;

						//�X�e�[�^�X���ړ�
						GetCraftingItemMenu[CraftingItemCnt].HP = CraftingItemMenu[MenuStringLeft].HP;
						GetCraftingItemMenu[CraftingItemCnt].MP = CraftingItemMenu[MenuStringLeft].MP;
						GetCraftingItemMenu[CraftingItemCnt].ATK = CraftingItemMenu[MenuStringLeft].ATK;
						GetCraftingItemMenu[CraftingItemCnt].DEF = CraftingItemMenu[MenuStringLeft].DEF;

						//�X�e�ϓ�
						StatusUpdate(GetCraftingItemMenu[CraftingItemCnt]);

						//�c�[���̎�ރv���X
						CraftingItemCnt++;
					}
				}
				else
				{
					PlayAudio(beepSE);
				}
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

	for (int i = 0; i < MAIN_ITEM_KIND; i++) { DrawHitBox(&GetMainItem[i]); }

	//���l���o�������Ƃ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(930, GAME_HEIGHT - 90, GAME_WIDTH, GAME_HEIGHT, GetColor(50, 50, 50), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatStringToHandle(950, GAME_HEIGHT - 75, GetColor(0, 0, 0), sampleFont2.handle, "�c��:%3d��", GetItemCntMax - GetItemCnt);

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

			if (MenuStringLeft != GAME_MENU_MATERIAL)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "�f�ރA�C�e��", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "�f�ރA�C�e��", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != GAME_MENU_CRAFTITEM)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "�c�[��", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "�c�[��", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != GAME_MENU_STATUS)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "�X�e�[�^�X", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "�X�e�[�^�X", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != GAME_MENU_CONFIG)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "�ݒ�", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "�ݒ�", GetColor(200, 200, 200), FALSE);


			//�E�u���b�N
			if (MenuRight == FALSE)
			{
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}
			else
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			//�f�ރA�C�e���\��
			if (MenuStringLeft == GAME_MENU_MATERIAL)
			{
				//���肵�����ɃA�C�e����ǉ����Ă���
				for (int i = 0,j = 0; i < MAIN_ITEM_KIND; i++)
				{
					if (GetMainItemMenu[i].Cnt > 0)
					{
						if (MenuStringRight == i && MenuRight == TRUE)
						{
							DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + ((j + 1) * 20), GetColor(200, 200, 200), "%-14s x%d", GetMainItemMenu[i].string, GetMainItemMenu[i].Cnt);
						}
						else
						{
							DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + ((j + 1) * 20), GetColor(100, 100, 100), "%-14s x%d", GetMainItemMenu[i].string, GetMainItemMenu[i].Cnt);
						}
						j++;
					}
				}
			}
			//����h��ȂǃN���t�g���\��
			if (MenuStringLeft == GAME_MENU_CRAFTITEM)
			{
				//���肵�����ɃA�C�e����ǉ����Ă���
				for (int i = 0,j = 0; i < CRAFTING_ITEM_KIND; i++)
				{
					if (GetCraftingItemMenu[i].Cnt > 0)
					{
						if (MenuStringRight == i && MenuRight == TRUE)
						{
							DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + ((j + 1) * 20), GetColor(200, 200, 200), "%-14s x%d", GetCraftingItemMenu[i].string, GetCraftingItemMenu[i].Cnt);
						}
						else
						{
							DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + ((j + 1) * 20), GetColor(100, 100, 100), "%-14s x%d", GetCraftingItemMenu[i].string, GetCraftingItemMenu[i].Cnt);
						}
						j++;
					}
				}
			}

			if (MenuStringLeft == GAME_MENU_STATUS)
			{
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, GetColor(100, 100, 100), "Name : %s", PlayChara.Name);
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 60, GetColor(100, 100, 100), "HP   : %d / %d", PlayChara.HP, PlayChara.MAX_HP);
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 80, GetColor(100, 100, 100), "MP   : %d / %d", PlayChara.MP, PlayChara.MAX_MP);
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 100, GetColor(100, 100, 100), "ATK  : %d", PlayChara.ATK);
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 120, GetColor(100, 100, 100), "DEF  : %d", PlayChara.DEF);
			}

			if (MenuStringLeft == GAME_MENU_CONFIG)
			{
				if (MenuStringRight == 0 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "����", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "����", GetColor(100, 100, 100), FALSE);

				//���ʒ��߂̃o�[
				if (OpenVolumecfg == TRUE)
				{
					DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 80, GetColor(0, 0, 0), "%3d / 255", Volumecfg);
					DrawBox(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 100, GAME_WIDTH / 3 + 275, GAME_HEIGHT / 6 + 120, GetColor(0, 0, 0), TRUE);
					DrawBox(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 100, GAME_WIDTH / 3 + 20 + Volumecfg, GAME_HEIGHT / 6 + 120, GetColor(0, 0, 255), TRUE);
				}
			}
		}
		else if (MenuScreenCrafting == TRUE)
		{
			//���u���b�N
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			for (int i = 0; i < CRAFTING_ITEM_KIND; i++)
			{
				if (MenuStringLeft != i)
					DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + (20 * (i + 1)), CraftingItemMenu[i].string, GetColor(100, 100, 100), FALSE);
				else
					DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + (20 * (i + 1)), CraftingItemMenu[i].string, GetColor(200, 200, 200), FALSE);
			}

			//�E�u���b�N
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			
			//�ϐ��ɂł������H
			int MenuHeightCnt=0;
			if (CraftingItemMenu[MenuStringLeft].Wood > 0)
			{
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + (20 * (MenuHeightCnt + 1)), GetColor(100, 100, 100), 
					"%-5s %2d/%2d",GetMainItemMenu[0].string, GetMainItemMenu[0].Cnt, CraftingItemMenu[MenuStringLeft].Wood);
				MenuHeightCnt++;
			}
			if (CraftingItemMenu[MenuStringLeft].Stone > 0)
			{
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + (20 * (MenuHeightCnt + 1)), GetColor(100, 100, 100), 
					"%-5s %2d/%2d",GetMainItemMenu[1].string, GetMainItemMenu[1].Cnt, CraftingItemMenu[MenuStringLeft].Stone);
				MenuHeightCnt++;
			}
			if (CraftingItemMenu[MenuStringLeft].Metal > 0)
			{
				DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + (20 * (MenuHeightCnt + 1)), GetColor(100, 100, 100), 
					"%-5s %2d/%2d",GetMainItemMenu[2].string, GetMainItemMenu[2].Cnt, CraftingItemMenu[MenuStringLeft].Metal);
				MenuHeightCnt++;
			}

			//�ޗ�������Ă���̂Ȃ�(����̐�����X�ݒ肵����)(���͏�����)
			if (CraftingItemMenu[MenuStringLeft].Wood <= GetMainItemMenu[0].Cnt
				&& CraftingItemMenu[MenuStringLeft].Stone <= GetMainItemMenu[1].Cnt
				&& CraftingItemMenu[MenuStringLeft].Metal <= GetMainItemMenu[2].Cnt
				&& CraftingItemMenu[MenuStringLeft].Cnt == 0)
			{
				DrawString(GAME_WIDTH * 5 / 6 - 100, GAME_HEIGHT * 5 / 6 - 40, "�쐬�\", GetColor(100, 100, 100), FALSE);
			}
			else if (CraftingItemMenu[MenuStringLeft].Cnt > 0)
			{
				DrawString(GAME_WIDTH * 5 / 6 - 100, GAME_HEIGHT * 5 / 6 - 40, "�쐬�ς�", GetColor(100, 100, 100), FALSE);
			}
			else
			{
				DrawString(GAME_WIDTH * 5 / 6 - 100, GAME_HEIGHT * 5 / 6 - 40, "�쐬�s��", GetColor(100, 100, 100), FALSE);
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

	if (PlayerTurn == FALSE && EnemyTurn == FALSE)
	{
		if (KeyClick(KEY_INPUT_UP) || KeyClick(KEY_INPUT_DOWN))
		{
			BattleMenuUp=!BattleMenuUp;
			PlayAudio(cursorSE);
		}
		if (KeyClick(KEY_INPUT_LEFT) || KeyClick(KEY_INPUT_RIGHT))
		{
			BattleMenuLeft=!BattleMenuLeft;
			PlayAudio(cursorSE);
		}

		if (BattleMenuUp)
		{
			if(BattleMenuLeft){BattlePlayCmd = 0;}
			else{BattlePlayCmd = 1;}
		}
		else
		{
			if (BattleMenuLeft){BattlePlayCmd = 2;}
			else{BattlePlayCmd = 3;}
		}
	}

	if (KeyClick(KEY_INPUT_Z))
	{
		if (PlayChara.HP <= 0)
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

		if (Battleenemy[0].HP <= 0)
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


		if (MPsmall == TRUE)
		{
			MPsmall = FALSE;
		}
		else if (PlayerTurn == TRUE && PlayerResult == TRUE)
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
			if (PlayChara.MP >= PlayChara.cmd[BattlePlayCmd].cost)
				PlayerTurn = TRUE;
			else
			{
				MPsmall = TRUE;
				PlayAudio(beepSE);
			}
		}

		if (PlayerTurn && !PlayerResult)
		{
			switch (BattlePlayCmd)
			{
			case 0:
				PlayAudio(BattleSE[1]);
				break;
			case 1:
				PlayAudio(BattleSE[3]);
				break;
			case 2:
				PlayAudio(BattleSE[4]);
				break;
			case 3:
				PlayAudio(BattleSE[5]);
				break;
			}
		}
		if (EnemyTurn && !EnemyResult)
		{
			if ((turn + 1) % 3 == 0)
			{
				PlayAudio(BattleSE[2]);
			}
			else
			{
				PlayAudio(BattleSE[1]);
			}
		}

		if (PlayerResult == TRUE)
		{
			if (PlayChara.cmd[BattlePlayCmd].Myself == FALSE)
			{
				Damage = PlayChara.ATK * PlayChara.cmd[BattlePlayCmd].DamageMultiplier - Battleenemy[0].DEF;
				PlayChara.MP -= PlayChara.cmd[BattlePlayCmd].cost;
				if (Damage < 1)
				{
					Damage = 0;
				}
				Battleenemy[0].HP -= Damage;
				if (Battleenemy[0].HP < 0)
				{
					Battleenemy[0].HP = 0;
				}
				PlayAudio(BattleSE[0]);
			}
			else
			{
				Damage = PlayChara.ATK * PlayChara.cmd[BattlePlayCmd].DamageMultiplier;
				PlayChara.MP -= PlayChara.cmd[BattlePlayCmd].cost;
				PlayChara.HP += Damage;
				if (PlayChara.HP > PlayChara.MAX_HP)
				{
					Damage -= PlayChara.HP - PlayChara.MAX_HP;
					PlayChara.HP = PlayChara.MAX_HP;
				}
			}
		}
		if (EnemyResult == TRUE)
		{
			if ((turn + 1) % 3 == 0)
			{
				Damage = Battleenemy[0].ATK * 2 - PlayChara.DEF;
			}
			else
			{
				Damage = Battleenemy[0].ATK - PlayChara.DEF;
			}
			if (Damage < 0)
			{
				Damage = 0;
			}
			PlayChara.HP -= Damage;
			if (PlayChara.HP < 0)
			{
				PlayChara.HP = 0;
			}
			PlayAudio(BattleSE[0]);
		}
	}

	return;
}

VOID BattleDraw()
{
	DrawImage(BattleImg);

	//�G�̕`��
	DrawImage(enemyImg1);

	//�G�̃o�[�\��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(enemyImg1.x + enemyImg1.width, enemyImg1.y + enemyImg1.height / 2 - 35, enemyImg1.x + enemyImg1.width + 280, enemyImg1.y + enemyImg1.height / 2 + 35, GetColor(230, 230, 230), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//HP
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(enemyImg1.x + enemyImg1.width+75, enemyImg1.y + enemyImg1.height / 2 - 25, enemyImg1.x + enemyImg1.width + 275, enemyImg1.y + enemyImg1.height / 2 - 10, GetColor(150, 150, 150), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(180, GAME_HEIGHT - 150, GAME_WIDTH - 200, GAME_HEIGHT - 50, GetColor(230, 230, 230), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(190, GAME_HEIGHT - 135, "HP", GetColor(200, 0, 0), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 135, GetColor(0, 0, 0), "%2d/%2d", PlayChara.HP,PlayChara.MAX_HP);

	DrawString(190, GAME_HEIGHT - 85, "MP", GetColor(0, 0, 200), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 85, GetColor(0, 0, 0), "%2d/%2d", PlayChara.MP, PlayChara.MAX_MP);

	//�Z�Ƃ��̃E�B���h�E
	DrawBox(495, GAME_HEIGHT - 145, GAME_WIDTH - 205, GAME_HEIGHT - 55, GetColor(200, 200, 200), TRUE);

	DrawBox(280, GAME_HEIGHT - 150 + 10, 480, GAME_HEIGHT - 150 + 40, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 10, 280+ ((PlayChara.HP * 100 / PlayChara.MAX_HP) * 2), GAME_HEIGHT - 150 + 40, GetColor(255, 0, 0), TRUE);

	DrawBox(280, GAME_HEIGHT - 150 + 60, 480, GAME_HEIGHT - 150 + 90, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 60, 280 + ((PlayChara.MP * 100 / PlayChara.MAX_MP) * 2), GAME_HEIGHT - 150 + 90, GetColor(0, 0, 255), TRUE);

	//�Z�\��
	if (!PlayerTurn && !EnemyTurn)
	{
		if (!MPsmall)
		{
			if (BattlePlayCmd == 0)
				DrawFormatString(520, GAME_HEIGHT - 130, GetColor(150, 150, 150), "%s", PlayChara.cmd[0].Name);
			else
				DrawFormatString(520, GAME_HEIGHT - 130, GetColor(0, 0, 0), "%s", PlayChara.cmd[0].Name);
			if (BattlePlayCmd == 1)
				DrawFormatString(800, GAME_HEIGHT - 130, GetColor(150, 150, 150), "%s", PlayChara.cmd[1].Name);
			else
				DrawFormatString(800, GAME_HEIGHT - 130, GetColor(0, 0, 0), "%s", PlayChara.cmd[1].Name);
			if (BattlePlayCmd == 2)
				DrawFormatString(520, GAME_HEIGHT - 85, GetColor(150, 150, 150), "%s", PlayChara.cmd[2].Name);
			else
				DrawFormatString(520, GAME_HEIGHT - 85, GetColor(0, 0, 0), "%s", PlayChara.cmd[2].Name);
			if (BattlePlayCmd == 3)
				DrawFormatString(800, GAME_HEIGHT - 85, GetColor(150, 150, 150), "%s", PlayChara.cmd[3].Name);
			else
				DrawFormatString(800, GAME_HEIGHT - 85, GetColor(0, 0, 0), "%s", PlayChara.cmd[3].Name);
		}
		else
			DrawString(520, GAME_HEIGHT - 130, "MP������܂���", GetColor(0, 0, 0), FALSE);
	}


	//���O�\��
	/*
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(GAME_WIDTH - 350, 10, GAME_WIDTH - 50, 110, GetColor(200, 200, 200), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < 4; i++)
	{
		DrawString(GAME_WIDTH - 340, 80 - (i * 20), BattleLog[i], GetColor(150, 150, 150));
	}
	*/
	if (PlayerTurn)
		DrawFormatString(520, GAME_HEIGHT - 130, GetColor(0, 0, 0), "%s �� %s�I",PlayChara.Name,PlayChara.cmd[BattlePlayCmd].Name);
	if (PlayerResult)
	{
		if(!PlayChara.cmd[BattlePlayCmd].Myself)
			DrawFormatString(520, GAME_HEIGHT - 110, GetColor(0, 0, 0), "%s �� %d �_���[�W�I", Battleenemy[0].Name, Damage);
		else
			DrawFormatString(520, GAME_HEIGHT - 110, GetColor(0, 0, 0), "%s �� %d �񕜁I", Battleenemy[0].Name, Damage);
	}
	if (EnemyTurn)
		DrawFormatString(520, GAME_HEIGHT - 130, GetColor(0, 0, 0), "%s �̂��������I", Battleenemy[0].Name);
	if(EnemyResult)
		DrawFormatString(520, GAME_HEIGHT - 110, GetColor(0, 0, 0), "%s �� %d �_���[�W�I", PlayChara.Name, Damage);


	//�^�[���\�� (�f�o�b�O����)
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
	DrawImage(EndLogo);

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
	DrawImage(GameoverLogo);

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


VOID StatusUpdate(MENU tool)
{
	PlayChara.MAX_HP += tool.HP;
	PlayChara.MAX_MP += tool.MP;
	PlayChara.MAX_ATK += tool.ATK;
	PlayChara.MAX_DEF += tool.DEF;

	PlayChara.HP = PlayChara.MAX_HP;
	PlayChara.MP = PlayChara.MAX_MP;
	PlayChara.ATK = PlayChara.MAX_ATK;
	PlayChara.DEF = PlayChara.MAX_DEF;
}

VOID SetCharaCmd(CHARA_CMD* cmd, const char name[255], int cost, float multi, BOOL myself)
{
	strcpyDx(cmd->Name, name);
	cmd->cost = cost;
	cmd->DamageMultiplier = multi;
	cmd->Myself = myself;

	return;
}