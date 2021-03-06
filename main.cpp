//ヘッダファイル読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理
#include "mouse.h"		//マウスの処理
#include "shape.h"		//図形の処理
#include "font.h"		//フォントの処理
#include "Data.h"		//データの処理

//独自のマクロ定義
#define MAIN_ITEM_KIND	3
#define CRAFTING_ITEM_KIND 6

//独自の列挙型の定義
enum MUKI
{
	muki_none,
	muki_ue,
	muki_hidari,
	muki_shita,
	muki_migi
};//向きのサンプル

//独自の構造体の定義


//★★★ゲーム共通のグローバル変数★★★
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回のゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン

//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

int fadeTimeMill = 2000;					//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;		//フェードアウトのカウンタMAX
//★★★ゲーム共通のグローバル変数★★★

float GameTimeLimit=0;
const float GameTimeLimitMax=60;


//独自のグローバル変数
//チュートリアルの文の格納
char TutorialString[16][255];

//チュートリアルの文の量
int TutorialCnt;
int TutorialCntMax;
BOOL TutorialSkip;

int MenuStringLeft;
int MenuStringRight;
BOOL ShiftDownSpeedUp;

//コンフィグ関連

//メニュー画面のサンプル
MENU GetMainItemMenu[MAIN_ITEM_KIND];
int GetMainItemCnt;

MENU GetCraftingItemMenu[CRAFTING_ITEM_KIND];	//インベントリ
MENU CraftingItemMenu[CRAFTING_ITEM_KIND];		//クラフト画面
int CraftingItemCnt;							//メニューのカウント

int Volumecfg = 0;
BOOL OpenVolumecfg;

int turn;

BOOL PlayerTurn;
BOOL EnemyTurn;
BOOL PlayerResult;
BOOL EnemyResult;

int Damage;


//★★★ゲーム共通のプロトタイプ宣言★★★
BOOL GameLoad(VOID);	//データを読込
VOID GameInit(VOID);	//データの初期化
VOID GameDelete(VOID);	//データを削除

VOID TitleInit(VOID);
VOID TutorialInit(VOID);
VOID PlayInit(VOID);
VOID BattleInit(VOID);
VOID EndInit(VOID);
VOID GameOverInit(VOID);

VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

VOID Tutorial(VOID);		//チュートリアル(操作説明)
VOID TutorialProc(VOID);	//チュートリアル(操作説明)(処理)
VOID TutorialDraw(VOID);	//チュートリアル(操作説明)(描画)

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)

VOID Battle(VOID);		//プレイ画面
VOID BattleProc(VOID);	//プレイ画面(処理)
VOID BattleDraw(VOID);	//プレイ画面(描画)

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面(処理)
VOID EndDraw(VOID);		//エンド画面(描画)

VOID GameOver(VOID);		//ゲームオーバー画面
VOID GameOverProc(VOID);	//ゲームオーバー画面(処理)
VOID GameOverDraw(VOID);	//ゲームオーバー画面(描画)

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面(処理)
VOID ChangeDraw(VOID);	//切り替え画面(描画)

VOID ChangeScene(GAME_SCENE scene);		//シーン切り替え
BOOL OnCollRect(RECT a, RECT b);		//矩形と矩形の当たり判定
VOID CollUpdateImage(IMAGE* img);		//画像の当たり判定の更新
VOID CollUpdateDivImage(DIVIMAGE* div);	//分割画像の当たり判定の更新
BOOL LoadImageMem(IMAGE* image, const char* path);										//ゲームの画像を読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType);				//ゲームの音楽を読み込み
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate);//ゲームの画像の分割読み込み

VOID PlayAudio(AUDIO audio);					//音楽再生
int GetVolumeAudio(AUDIO audio);				//音楽のボリューム取得
VOID SetVolumeAudio(AUDIO* audio, int vol);		//音楽のボリューム設定
VOID ChangeVolumeAudio(AUDIO* audio, int vol);	//音楽のボリューム変更
VOID StopAudio(AUDIO* audio);					//音楽停止

VOID DrawImage(IMAGE image);					//画像描画
VOID DrawRect(RECT r, unsigned int color, bool b);	//RECT描画
VOID DrawDivImage(DIVIMAGE* image);				//分割画像の描画
VOID DrawDivImageChara(DIVIMAGE* image);		//分割画像の描画

VOID DrawHitBox(EVENT* events);					//イベントの当たり判定の描画

VOID ItemEventInit(EVENT* events, int Wood, int Stone, int CntMax);	//アイテム初期化

VOID SetEventUpdate();							//イベントの場所更新
VOID StatusUpdate(MENU tool);					//ステータスの変動用
VOID SetCharaCmd(CHARA_CMD* cmd,const char name[255],int cost,float multi,BOOL myself);		//技の設定
VOID SetWeaponStatus(MENU* tool, const char name[255], int wood, int stone, int metal, int HP, int MP, int ATK, int DEF);

//★★★ゲーム共通のプロトタイプ宣言★★★

//サンプルの素材
IMAGE sampleImg;
DIVIMAGE sampleDivImg;
DIVIMAGE samplePlayerImg;
MUKI muki = muki_shita;		//サンプル向き

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

//戦闘SE
AUDIO BattleSE[6];

MAP_DATA map1;
MAP_DATA map2;

//イベントマス
EVENT CreateItems;

EVENT GetMainItem[MAIN_ITEM_KIND];	//石、木材、金属

//ロゴなどの画像
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

//戦闘画面
CHARA_DATA Battleenemy[ENEMY_MAX];
CHARA_DATA PlayChara;

char BattleLog[4][255];
char BattleText[255];

//技の選択
BOOL BattleMenuUp;
BOOL BattleMenuLeft;
int BattlePlayCmd;
BOOL MPsmall;

// プログラムは WinMain から始まります
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//フォント追加
	if (FontAdd() == FALSE) { return FALSE; }



	//ゲーム読み込み
	if (!GameLoad())
	{
		//データの読み込みに失敗したとき
		GameDelete();	//データ削除
		DxLib_End();	//DxLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();
	TitleInit();

	//無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//マウス入力の更新
		MouseUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_TUTORIAL:
			Tutorial();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_BATTLE:
			Battle();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_GAMEOVER:
			GameOver();			//ゲームオーバー画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//現在のマウスの情報を描画
		MouseDraw();

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//フォント削除
	FontRemove();

	//データ削除
	GameDelete();

	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad(VOID)
{
	//サンプル画像を読み込み
	if (LoadImageMem(&sampleImg, ".\\Image\\player.png") == FALSE) { return FALSE; }

	//サンプル分割画像を読み込み
	if (LoadImageDivMem(&sampleDivImg, ".\\Image\\baku1.png", 8, 2) == FALSE) { return FALSE; }

	//サンプル分割画像を読み込み
	if (LoadImageDivMem(&samplePlayerImg, ".\\Image\\charachip1.png", 4, 4) == FALSE) { return FALSE; }

	//サンプルBGMを読み込み
	if (LoadAudio(&sampleBGM, ".\\Audio\\ブリキのPARADE.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	
	//フォントを作成
	if (FontCreate() == FALSE) { return FALSE; }

	//サンプル敵データを読み込み
	//if (LoadCSVChara(CSV_PATH_ENEMY, &enemy[0], ENEMY_MAX, TRUE) == FALSE) { return FALSE; }

	//サンプルスコアデータを読み込み
	if (LoadScoreData(SCORE_DATA_PATH, &score_data, TRUE) == FALSE) { return FALSE; }

	//サンプルマップデータを読み込み
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

	//BGMを読み込み
	if (LoadAudio(&titleBGM, ".\\audio\\街をブラブラする的なBGM.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&playBGM, ".\\audio\\Just_a_really_bad_feeling.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&battleBGM, ".\\audio\\Final_Judgement_2.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&endBGM, ".\\audio\\ブルーボトル.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&gameoverBGM, ".\\audio\\拭えない悲しみと迫り来る明日.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }

	if (LoadAudio(&sceneEnterSE, ".\\SE\\システム決定音_9_2.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&enterSE, ".\\SE\\システム決定音_3.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&returnSE, ".\\SE\\キャンセル2.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&cursorSE, ".\\SE\\カーソル移動8.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&collectSE, ".\\SE\\決定、ボタン押下29.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&beepSE, ".\\SE\\ビープ音.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }

	if (LoadAudio(&BattleSE[0], ".\\SE\\BattleSE\\ダメージ音01.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[1], ".\\SE\\BattleSE\\重いパンチ2.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[2], ".\\SE\\BattleSE\\打撃6.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[3], ".\\SE\\BattleSE\\剣で斬る1.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[4], ".\\SE\\BattleSE\\火炎魔法1.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }
	if (LoadAudio(&BattleSE[5], ".\\SE\\BattleSE\\回復魔法1.mp3", 128, DX_PLAYTYPE_BACK) == FALSE) { return FALSE; }

	//マップデータを読み込み
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

	//ロゴ読み込み
	if (LoadImageMem(&TitleImg, ".\\Image\\タイトル背景.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&TitleLogo, ".\\Image\\タイトルロゴ.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&BattleImg, ".\\Image\\field_d.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&EndImg, ".\\Image\\GAME_CLEAR背景.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&EndLogo, ".\\Image\\ゲームクリアロゴ.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&GameoverImg, ".\\Image\\GAME_OVER背景.jpg") == FALSE) { return FALSE; }
	if (LoadImageMem(&GameoverLogo, ".\\Image\\ゲームオーバーロゴ.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&PushEnter, ".\\Image\\脱出_Enter.png") == FALSE) { return FALSE; }

	//敵画像を読み込み
	if (LoadImageMem(&enemyImg1, ".\\Image\\monster171_l.png") == FALSE) { return FALSE; }

	//敵データを読み込み
	if (LoadCSVChara(CSV_PATH_ENEMY, &Battleenemy[0], ENEMY_MAX, TRUE) == FALSE) { return FALSE; }

	//プレイヤーデータを読み込み
	if (LoadCSVChara(CSV_PATH_PLAYER, &PlayChara, 1, TRUE) == FALSE) { return FALSE; }


	return TRUE;	//全て読み込みた！
}

/// <summary>
/// ゲームのデータを削除
/// </summary>
VOID GameDelete(VOID)
{
	//サンプル画像を削除
	DeleteGraph(sampleImg.handle);

	//サンプル分割画像を削除
	for (int i = 0; i < sampleDivImg.DivMax; i++) { DeleteGraph(sampleDivImg.handle[i]); }

	//サンプル分割画像を削除
	for (int i = 0; i < samplePlayerImg.DivMax; i++) { DeleteGraph(samplePlayerImg.handle[i]); }

	//サンプル音楽を削除
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

	//フォントデータを削除
	FontDelete();

	//画像を削除
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
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//音楽の音量調整
	SetVolumeAudio(&sampleBGM, 100);
	SetVolumeAudio(&titleBGM, 200);
	SetVolumeAudio(&playBGM, 200);
	SetVolumeAudio(&battleBGM, 200);
	SetVolumeAudio(&endBGM, 200);
	SetVolumeAudio(&gameoverBGM, 200);

	//ゲーム内時間リセット
	ResetGameTime();

	//サンプルプレイヤー初期化

	return;
}

VOID TitleInit(VOID)
{
	//タイトルロゴの初期化
	TitleImg.IsDraw = TRUE;
	TitleImg.x = 0;
	TitleImg.y = 0;

	TitleLogo.IsDraw = TRUE;
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;
	TitleLogo.y = TitleLogo.height / 6;

	PushEnter.IsDraw = TRUE;
	PushEnter.x = 0;
	PushEnter.y = 0;

	//基のステータスに初期化
	PlayChara.MAX_HP = PlayChara.DEFAULT_HP;
	PlayChara.MAX_MP = PlayChara.DEFAULT_MP;
	PlayChara.MAX_ATK = PlayChara.DEFAULT_ATK;
	PlayChara.MAX_DEF = PlayChara.DEFAULT_DEF;

	//ステータス初期化
	PlayChara.HP = PlayChara.MAX_HP;
	PlayChara.MP = PlayChara.MAX_MP;
	PlayChara.ATK = PlayChara.MAX_ATK;
	PlayChara.DEF = PlayChara.MAX_DEF;

	return;
}

VOID TutorialInit(VOID)
{
	TutorialCnt = 0;
	TutorialCntMax = 12;

	TutorialSkip = FALSE;

	strcpyDx(TutorialString[0], "チュートリアルをスキップしますか？");
	strcpyDx(TutorialString[1], "");
	strcpyDx(TutorialString[2], "[Z]:決定、[X]:キャンセル,メニュー、[SHIFT]:ダッシュ");
	strcpyDx(TutorialString[3], "[矢印キー]:上下左右移動、選択肢の移動");
	strcpyDx(TutorialString[4], "マップ右上側で素材の採取");
	strcpyDx(TutorialString[5], "マップ中央付近で素材を消費してのクラフトができます");
	strcpyDx(TutorialString[6], "クラフトすることでステータスが上昇します");
	strcpyDx(TutorialString[7], "採取回数が０になったら戦闘へ移行します");
	strcpyDx(TutorialString[8], "戦闘に勝てばゲームクリア");
	strcpyDx(TutorialString[9], "戦闘に負けるとゲームオーバーです");
	strcpyDx(TutorialString[10], "それでは行ってらっしゃいませ");
	strcpyDx(TutorialString[11], "");

	return;
}

VOID PlayInit(VOID)
{
	//サンプルプレイヤー初期化
	samplePlayerImg.speed = 4;
	samplePlayerImg.x = 20;
	samplePlayerImg.y = 10;
	ShiftDownSpeedUp = FALSE;

	muki = muki_none;

	//サンプルイベントの作成
	//CreateEventMultiMass(9, 1, 11, 2, &sampleevent, map2);

	//イベントの作成
	SetEventUpdate();

	//時間制限
	GameTimeLimit = GameTimeLimitMax;

	//アイテムの個数の初期化
	ItemEventInit(&CreateItems, 0, 0, NULL);

	//メニューを開いていない
	MenuScreen = FALSE;
	MenuScreenInventory = FALSE;

	MenuStringLeft = 0;
	MenuStringRight = 0;

	MenuRight = FALSE;

	//コンフィグを初期化

	OpenVolumecfg = FALSE;

	//イベントの初期化
	for (int i = 0; i < MAIN_ITEM_KIND; i++)
	{
		GetMainItem[i].Cnt = 0;
		GetMainItem[i].can = FALSE;
	}

	//イベントの文字列コピー
	strcpyDx(GetMainItem[0].string, "木材");
	strcpyDx(GetMainItem[1].string, "石");
	strcpyDx(GetMainItem[2].string, "金属");

	//イベントの文字列初期化
	for (int i = 0; i < MAIN_ITEM_KIND; i++)
	{
		strcpyDx(GetMainItemMenu[i].string,GetMainItem[i].string);
		GetMainItemMenu[i].Cnt = 0;
	}

	//クラフトのメニューの初期化
	for (int i = 0; i < CRAFTING_ITEM_KIND; i++)
	{
		CraftingItemMenu[i].Cnt = 0;
		CraftingItemMenu[i].can = FALSE;
	}

	SetWeaponStatus(&CraftingItemMenu[0], "ピッケル", 1, 3, 0, 3, 1, 0, 0);
	SetWeaponStatus(&CraftingItemMenu[1], "アックス", 3, 2, 1, 0, 1, 0, 3);
	SetWeaponStatus(&CraftingItemMenu[2], "ソード", 1, 1, 5, 0, 1, 3, 0);
	SetWeaponStatus(&CraftingItemMenu[3], "魔術本", 5, 0, 1, 0, 5, 1, 0);
	SetWeaponStatus(&CraftingItemMenu[4], "バトルアックス", 5, 2, 5, 3, 0, 5, 0);
	SetWeaponStatus(&CraftingItemMenu[5], "グレートソード", 2, 5, 8, 2, 0, 7, 2);

	//インベントリ
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

	//取得したアイテムの種類の個数の初期化
	GetMainItemCnt = 0;
	CraftingItemCnt = 0;

	samplePlayerImg.screenX = 0;
	samplePlayerImg.screenY = 0;
	MapInit(&map2);

	//採取回数
	GetItemCnt = 0;
	GetItemCntMax = 30;
	
	return;
}

VOID BattleInit()
{
	//背景初期化
	BattleImg.IsDraw = TRUE;
	BattleImg.x = 0;
	BattleImg.y = 0;

	//敵初期化
	enemyImg1.x = GAME_WIDTH / 2 - enemyImg1.width / 2;
	enemyImg1.y = GAME_HEIGHT / 2 - enemyImg1.height / 2 - 50;	//マジックナンバー
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

	strcpyDx(BattleLog[0], "てすと1");
	strcpyDx(BattleLog[1], "てすと2");
	strcpyDx(BattleLog[2], "てすと3");
	strcpyDx(BattleLog[3], "てすと4");

	SetCharaCmd(&PlayChara.cmd[0], "こうげき", 0, 1, FALSE);
	SetCharaCmd(&PlayChara.cmd[1], "きりさく", 1, 1.2, FALSE);
	SetCharaCmd(&PlayChara.cmd[2], "ファイア", 4, 2, FALSE);
	SetCharaCmd(&PlayChara.cmd[3], "ヒール", 5, 2, TRUE);

	BattleMenuUp = TRUE;
	BattleMenuLeft = TRUE;
	BattlePlayCmd = 0;
	MPsmall = FALSE;

	return;
}

VOID EndInit(VOID)
{
	//エンド画面の初期化
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
	//ゲームオーバー画面の初期化
	GameoverImg.IsDraw = TRUE;
	GameoverImg.x = 0;
	GameoverImg.y = 0;

	GameoverLogo.IsDraw = TRUE;
	GameoverLogo.x = GAME_WIDTH / 2 - GameoverLogo.width / 2;
	GameoverLogo.y = GAME_HEIGHT / 2 - GameoverLogo.height / 2;

	return;
}

/// <summary>
/// イベントの座標の更新
/// </summary>
VOID SetEventUpdate()
{
	//イベント
	{
		CreateEventMass(41, 29, &CreateItems, map2);

		CreateEventMultiMass(46, 8, 49, 9, &GetMainItem[0], map2);
		CreateEventMultiMass(41, 8, 43, 9, &GetMainItem[1], map2);
		CreateEventMultiMass(53, 9, 55, 10, &GetMainItem[2], map2);
	}
}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	CollUpdateImage(&TitleLogo);

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//SEを流す
		PlayAudio(sceneEnterSE);
		//音楽を止める
		StopAudio(&titleBGM);

		//ゲームの初期化
		GameInit();
		TutorialInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_TUTORIAL);

		return;
	}

	PlayAudio(titleBGM);	//BGMを鳴らす

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	DrawImage(TitleImg);
	DrawImage(TitleLogo);
	DrawImage(PushEnter);

	if(GAME_DEBUG)DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// チュートリアル画面
/// </summary>
VOID Tutorial(VOID)
{
	TutorialProc();	//処理
	TutorialDraw();	//描画

	return;
}

/// <summary>
/// チュートリアル画面の処理
/// </summary>
VOID TutorialProc(VOID)
{
	if (GAME_DEBUG)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//シーン切り替え
			//次のシーンの初期化をここで行うと楽

			//SEを流す
			PlayAudio(sceneEnterSE);

			//ゲームの初期化
			GameInit();
			PlayInit();

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_PLAY);

			return;
		}
	}

	if (TutorialCnt == 0)
	{
		if (KeyClick(KEY_INPUT_LEFT) || KeyClick(KEY_INPUT_RIGHT))
		{
			PlayAudio(cursorSE);
			TutorialSkip = !TutorialSkip;
		}
	}

	if (KeyClick(KEY_INPUT_Z))
	{
		if (TutorialCnt >= TutorialCntMax - 2 || TutorialSkip)
		{
			//シーン切り替え
			//次のシーンの初期化をここで行うと楽

			//SEを流す
			PlayAudio(sceneEnterSE);

			//ゲームの初期化
			GameInit();
			PlayInit();

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_PLAY);

			return;
		}
		else
		{
			PlayAudio(enterSE);
			TutorialCnt += 2;
		}
	}

	return;
}

/// <summary>
/// チュートリアル画面の描画
/// </summary>
VOID TutorialDraw(VOID)
{
	//背景
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0),TRUE);

	//メッセージウィンドウ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(175, GAME_HEIGHT - 155, GAME_WIDTH - 195, GAME_HEIGHT - 45, GetColor(230, 230, 230), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(180, GAME_HEIGHT - 150, GAME_WIDTH - 200, GAME_HEIGHT - 50, GetColor(230, 230, 230), TRUE);

	//本文
	DrawString(200, GAME_HEIGHT - 130, TutorialString[TutorialCnt], GetColor(0, 0, 0), FALSE);
	DrawString(200, GAME_HEIGHT - 110, TutorialString[TutorialCnt + 1], GetColor(0, 0, 0), FALSE);

	if (TutorialCnt == 0)
	{
		if (TutorialSkip)
		{
			DrawString(240, GAME_HEIGHT - 90, "いいえ", GetColor(0, 0, 0), FALSE);
			DrawString(380, GAME_HEIGHT - 90, "→", GetColor(0, 0, 0), FALSE);
			DrawString(400, GAME_HEIGHT - 90, "はい", GetColor(100, 100, 100), FALSE);
		}
		else
		{
			DrawString(240, GAME_HEIGHT - 90, "いいえ", GetColor(100, 100, 100), FALSE);
			DrawString(220, GAME_HEIGHT - 90, "→", GetColor(0, 0, 0), FALSE);
			DrawString(400, GAME_HEIGHT - 90, "はい", GetColor(0, 0, 0), FALSE);
		}
	}

	if (GAME_DEBUG)DrawString(0, 0, "チュートリアル画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//処理
	PlayDraw();	//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	PlayAudio(playBGM);	//BGMを鳴らす
	if (Volumecfg == 0)
	{
		Volumecfg = GetVolumeAudio(playBGM);
	}
	SetVolumeAudio(&playBGM, Volumecfg);
	if (GAME_DEBUG)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//ゲームデータの初期化
			GameInit();
			BattleInit();

			//SEを流す
			PlayAudio(sceneEnterSE);
			//音楽を止める
			StopAudio(&playBGM);

			//戦闘画面に切り替え
			ChangeScene(GAME_SCENE_BATTLE);

			return;
		}
	}
	
	//Xを押したときの関数反転 + a
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

	//メニューを開いていない(通常時)時
	if (MenuScreen == FALSE)
	{
		if (KeyDown(KEY_INPUT_LSHIFT) || KeyDown(KEY_INPUT_RSHIFT))
		{
			if (!ShiftDownSpeedUp)
			{
				ShiftDownSpeedUp = TRUE;
				samplePlayerImg.speed *= 2;
			}
		}
		else
		{
			if (ShiftDownSpeedUp)
			{
				ShiftDownSpeedUp = FALSE;
				samplePlayerImg.speed /= 2;
			}
		}

		//マップの当たり判定
		{
			muki = muki_none;					//最初は向きなし
			DIVIMAGE dummy = samplePlayerImg;	//当たり判定のダミー
			DIVIMAGE dummy2 = samplePlayerImg;	//当たり判定のダミー
			if (KeyDown(KEY_INPUT_UP)) { muki = muki_ue; dummy.y -= samplePlayerImg.speed; }
			else if (KeyDown(KEY_INPUT_DOWN)) { muki = muki_shita; dummy.y += samplePlayerImg.speed; }

			CollUpdateDivImage(&dummy);	//当たり判定の更新
			CollMapUpdate(&map2);		//マップの当たり判定更新

			for(int i=0;i<samplePlayerImg.speed;i++)
			{
				//当たっていないのなら
				if (CollMap(dummy.coll, map2) == FALSE)
				{
					samplePlayerImg.y = dummy.y;	//ダミーの情報を戻す
					break;
				}
				//dummyが基の座標よりも上なら
				else if(dummy.y < dummy2.y)
				{
					//下にずらす
					dummy.y++;
				}
				//下なら
				else if (dummy.y > dummy2.y)
				{
					//上にずらす
					dummy.y--;
				}
				CollUpdateDivImage(&dummy);	//当たり判定の更新
			}

			dummy = samplePlayerImg;	//当たり判定のダミー

			if (KeyDown(KEY_INPUT_LEFT)) { muki = muki_hidari; dummy.x -= samplePlayerImg.speed; }
			else if (KeyDown(KEY_INPUT_RIGHT)) { muki = muki_migi; dummy.x += samplePlayerImg.speed; }

			CollUpdateDivImage(&dummy);	//当たり判定の更新
			CollMapUpdate(&map2);		//マップの当たり判定更新

			for (int i = 0; i < samplePlayerImg.speed; i++)
			{
				if (CollMap(dummy.coll, map2) == TRUE)
				{
					//当たっていた時、dummy2とplayerのy位置が違うなら向き修正
					if (samplePlayerImg.y != dummy2.y)
					{
						if (samplePlayerImg.y < dummy2.y) { muki = muki_ue; }
						else { muki = muki_shita; }
					}
				}
				//当たっていないなら
				else if (CollMap(dummy.coll, map2) == FALSE)
				{
					samplePlayerImg.x = dummy.x;	//ダミーの情報を戻す
					break;
				}

				//dummyが基の座標よりも左なら
				if (dummy.x < dummy2.x)
				{
					//右にずらす
					dummy.x++;
				}
				//右なら
				else if (dummy.x > dummy2.x)
				{
					//左にずらす
					dummy.x--;
				}
				CollUpdateDivImage(&dummy);	//当たり判定の更新
			}

			//プレイヤーの幅の考慮無し,要修正
			if (samplePlayerImg.x > GAME_WIDTH / 2
				&& map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_WIDTH - map2.width && dummy2.x != samplePlayerImg.x)
			{
				samplePlayerImg.screenX = samplePlayerImg.x - dummy2.x;
				samplePlayerImg.x = dummy2.x;	//ダミーの情報を戻す
			}
			else if (samplePlayerImg.x < GAME_WIDTH / 2
				&& map2.x[0][0] < 0 && dummy2.x != samplePlayerImg.x)
			{
				samplePlayerImg.screenX = samplePlayerImg.x - dummy2.x;
				samplePlayerImg.x = dummy2.x;	//ダミーの情報を戻す
			}
			else
			{
				samplePlayerImg.screenX = 0;
			}

			if (samplePlayerImg.y > GAME_HEIGHT / 2
				&& map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_HEIGHT - map2.height && dummy2.y != samplePlayerImg.y)
			{
				samplePlayerImg.screenY = samplePlayerImg.y - dummy2.y;
				samplePlayerImg.y = dummy2.y;	//ダミーの情報を戻す
			}
			else if (samplePlayerImg.y < GAME_HEIGHT / 2
				&& map2.y[0][0] < 0 && dummy2.y != samplePlayerImg.y)
			{
				samplePlayerImg.screenY = samplePlayerImg.y - dummy2.y;
				samplePlayerImg.y = dummy2.y;	//ダミーの情報を戻す
			}
			else
			{
				samplePlayerImg.screenY = 0;
			}

			MapMove(&map2);		//マップの移動

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

			MapMove(&map2);		//マップの移動

			CollMapUpdate(&map2);		//マップの当たり判定更新
			SetEventUpdate();		//イベントの座標の更新

			CollUpdateDivImage(&samplePlayerImg);	//当たり判定の更新

			//クラフトイベント
			if (CheckCollRectToRect(samplePlayerImg.coll, CreateItems.coll))
			{
				if (KeyClick(KEY_INPUT_Z))
				{
					PlayAudio(enterSE);
					MenuScreen = TRUE;
					MenuScreenCrafting = TRUE;
				}
			}

			//素材アイテム入手
			for (int i = 0; i < MAIN_ITEM_KIND; i++)
			{
				//当たり判定
				if (CheckCollRectToRect(samplePlayerImg.coll, GetMainItem[i].coll))
				{
					//Zを押したら
					if (KeyClick(KEY_INPUT_Z))
					{
						PlayAudio(collectSE);
						for (int j = 0; j < MAIN_ITEM_KIND; j++)
						{
							//探査(strcmpDxの仕様良くわかっていない、とりあえず動作したのでこのまま)
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
			//ゲームデータの初期化
			GameInit();
			BattleInit();

			//SEを流す
			PlayAudio(sceneEnterSE);
			//音楽を止める
			StopAudio(&playBGM);

			//戦闘画面に切り替え
			ChangeScene(GAME_SCENE_BATTLE);
		}
	}
	//メニューを開いているとき
	else if(MenuScreen==TRUE)
	{
		//メニューの操作
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

			//左
			//左の項目を上限を超えて上に行ったとき(のちにdefine化)
			if (MenuStringLeft < 0)
			{
				MenuStringLeft = 3;
			}
			//左の項目を上限を超えて下に行ったとき
			else if (MenuStringLeft > 3)
			{
				MenuStringLeft = 0;
			}
			//左の1項目目の時
			if (MenuStringLeft == GAME_MENU_MATERIAL)
			{
				//右
				if (MenuStringRight < 0)
				{
					MenuStringRight = GetMainItemCnt-1;
				}
				else if (MenuStringRight > GetMainItemCnt-1)
				{
					MenuStringRight = 0;
				}
			}
			//左の2項目目の時
			if (MenuStringLeft == GAME_MENU_CRAFTITEM)
			{
				//右
				if (MenuStringRight < 0)
				{
					MenuStringRight = CraftingItemCnt-1;
				}
				else if (MenuStringRight > CraftingItemCnt-1)
				{
					MenuStringRight = 0;
				}
			}
			//左の3項目目の時(見るようなので動きなし)
			if (MenuStringLeft == GAME_MENU_STATUS)
			{

			}
			//左の4項目目の時
			if (MenuStringLeft == GAME_MENU_CONFIG)
			{
				//右
				if (MenuStringRight < 0)
				{
					MenuStringRight = 0;
				}
				else if (MenuStringRight > 0)
				{
					MenuStringRight = 0;
				}
			}

			//決定ボタンを押したとき
			if (KeyClick(KEY_INPUT_Z))
			{
				PlayAudio(enterSE);
				//左項目にいるとき
				if (MenuRight == FALSE)
				{
					//右に移動
					MenuRight = TRUE;
				}
				//右項目にいるとき
				else
				{
					PlayAudio(enterSE);
					//4行目
					if (MenuStringLeft == GAME_MENU_CONFIG)
					{
						//決定を押したときにBOOL型を反転
						if (OpenVolumecfg == FALSE)
							OpenVolumecfg = TRUE;
						else
							OpenVolumecfg = FALSE;
					}
				}
			}

			//音量調整
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
		//クラフト画面
		else if (MenuScreenCrafting == TRUE)
		{
			//下
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
			//上
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

			//左
			//左の項目を上限を超えて上に行ったとき
			if (MenuStringLeft < 0)
			{
				MenuStringLeft = CRAFTING_ITEM_KIND - 1;
			}
			//左の項目を上限を超えて下に行ったとき
			else if (MenuStringLeft > CRAFTING_ITEM_KIND - 1)
			{
				MenuStringLeft = 0;
			}

			//決定キーを押したとき
			if (KeyClick(KEY_INPUT_Z))
			{
				if (CraftingItemMenu[MenuStringLeft].Wood <= GetMainItemMenu[0].Cnt
					&& CraftingItemMenu[MenuStringLeft].Stone <= GetMainItemMenu[1].Cnt
					&& CraftingItemMenu[MenuStringLeft].Metal <= GetMainItemMenu[2].Cnt
					&& CraftingItemMenu[MenuStringLeft].Cnt == 0)
				{
					PlayAudio(enterSE);
					{
						//文字列コピー
						strcpyDx(GetCraftingItemMenu[CraftingItemCnt].string, CraftingItemMenu[MenuStringLeft].string);
						GetCraftingItemMenu[CraftingItemCnt].Cnt++;
						CraftingItemMenu[MenuStringLeft].Cnt++;

						//素材消費
						GetMainItemMenu[0].Cnt -= CraftingItemMenu[MenuStringLeft].Wood;
						GetMainItemMenu[1].Cnt -= CraftingItemMenu[MenuStringLeft].Stone;
						GetMainItemMenu[2].Cnt -= CraftingItemMenu[MenuStringLeft].Metal;

						//ステータスを移動
						GetCraftingItemMenu[CraftingItemCnt].HP = CraftingItemMenu[MenuStringLeft].HP;
						GetCraftingItemMenu[CraftingItemCnt].MP = CraftingItemMenu[MenuStringLeft].MP;
						GetCraftingItemMenu[CraftingItemCnt].ATK = CraftingItemMenu[MenuStringLeft].ATK;
						GetCraftingItemMenu[CraftingItemCnt].DEF = CraftingItemMenu[MenuStringLeft].DEF;

						//ステ変動
						StatusUpdate(GetCraftingItemMenu[CraftingItemCnt]);

						//ツールの種類プラス
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
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//マップのサンプル
	DrawMap(map2);

	for (int i = 0; i < MAIN_ITEM_KIND; i++) { DrawHitBox(&GetMainItem[i]); }

	//数値を出したいとき
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(930, GAME_HEIGHT - 90, GAME_WIDTH, GAME_HEIGHT, GetColor(50, 50, 50), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatStringToHandle(950, GAME_HEIGHT - 75, GetColor(0, 0, 0), sampleFont2.handle, "残り:%3d回", GetItemCntMax - GetItemCnt);

	if(MenuScreen==TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawBox(GAME_WIDTH / 6, GAME_HEIGHT / 6, GAME_WIDTH * 5 / 6, GAME_HEIGHT * 5 / 6, GetColor(200, 200, 200), TRUE);

		if(MenuScreenInventory==TRUE)
		{
			//左ブロック
			if (MenuRight == FALSE)
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);
			else
			{
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}

			if (MenuStringLeft != GAME_MENU_MATERIAL)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "素材アイテム", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "素材アイテム", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != GAME_MENU_CRAFTITEM)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "ツール", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "ツール", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != GAME_MENU_STATUS)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "ステータス", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "ステータス", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != GAME_MENU_CONFIG)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "設定", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "設定", GetColor(200, 200, 200), FALSE);


			//右ブロック
			if (MenuRight == FALSE)
			{
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}
			else
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			//素材アイテム表示
			if (MenuStringLeft == GAME_MENU_MATERIAL)
			{
				//入手した順にアイテムを追加していく
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
			//武器防具などクラフト物表示
			if (MenuStringLeft == GAME_MENU_CRAFTITEM)
			{
				//入手した順にアイテムを追加していく
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
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "音量", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "音量", GetColor(100, 100, 100), FALSE);

				//音量調節のバー
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
			//左ブロック
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			for (int i = 0; i < CRAFTING_ITEM_KIND; i++)
			{
				if (MenuStringLeft != i)
					DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + (20 * (i + 1)), CraftingItemMenu[i].string, GetColor(100, 100, 100), FALSE);
				else
					DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + (20 * (i + 1)), CraftingItemMenu[i].string, GetColor(200, 200, 200), FALSE);
			}

			//右ブロック
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			
			//変数にできそう？
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

			//材料が足りているのなら(上限の数も後々設定したい)(今は上限一個)
			if (CraftingItemMenu[MenuStringLeft].Wood <= GetMainItemMenu[0].Cnt
				&& CraftingItemMenu[MenuStringLeft].Stone <= GetMainItemMenu[1].Cnt
				&& CraftingItemMenu[MenuStringLeft].Metal <= GetMainItemMenu[2].Cnt
				&& CraftingItemMenu[MenuStringLeft].Cnt == 0)
			{
				DrawString(GAME_WIDTH * 5 / 6 - 100, GAME_HEIGHT * 5 / 6 - 40, "作成可能", GetColor(100, 100, 100), FALSE);
			}
			else if (CraftingItemMenu[MenuStringLeft].Cnt > 0)
			{
				DrawString(GAME_WIDTH * 5 / 6 - 100, GAME_HEIGHT * 5 / 6 - 40, "作成済み", GetColor(100, 100, 100), FALSE);
			}
			else
			{
				DrawString(GAME_WIDTH * 5 / 6 - 100, GAME_HEIGHT * 5 / 6 - 40, "作成不可", GetColor(100, 100, 100), FALSE);
			}
		}
	}
	//else if(MenuScreen==FALSE)
	//{SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }

	
	if (GAME_DEBUG)DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
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

	if (GAME_DEBUG)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//ゲームデータの初期化
			GameInit();
			EndInit();

			//SEを流す
			PlayAudio(sceneEnterSE);
			//音楽を止める
			StopAudio(&battleBGM);

			//エンド画面に切り替え
			ChangeScene(GAME_SCENE_END);

			return;
		}
	}

	if (KeyClick(KEY_INPUT_SPACE) == TRUE)
	{
		//ゲームデータの初期化
		GameInit();
		GameOverInit();

		//SEを流す
		PlayAudio(sceneEnterSE);
		//音楽を止める
		StopAudio(&battleBGM);

		//ゲームオーバー画面に切り替え
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
			//ゲームデータの初期化
			GameInit();
			GameOverInit();

			//SEを流す
			PlayAudio(sceneEnterSE);
			//音楽を止める
			StopAudio(&battleBGM);

			//ゲームオーバー画面に切り替え
			ChangeScene(GAME_SCENE_GAMEOVER);

			return;
		}

		if (Battleenemy[0].HP <= 0)
		{
			//ゲームデータの初期化
			GameInit();
			EndInit();

			//SEを流す
			PlayAudio(sceneEnterSE);
			//音楽を止める
			StopAudio(&battleBGM);

			//エンド画面に切り替え
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

	//敵の描画
	DrawImage(enemyImg1);

	//敵のバー表示
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
	//MP未実装


	//味方のウィンドウ+バー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(180, GAME_HEIGHT - 150, GAME_WIDTH - 200, GAME_HEIGHT - 50, GetColor(230, 230, 230), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(190, GAME_HEIGHT - 135, "HP", GetColor(200, 0, 0), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 135, GetColor(0, 0, 0), "%2d/%2d", PlayChara.HP,PlayChara.MAX_HP);

	DrawString(190, GAME_HEIGHT - 85, "MP", GetColor(0, 0, 200), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 85, GetColor(0, 0, 0), "%2d/%2d", PlayChara.MP, PlayChara.MAX_MP);

	//技とかのウィンドウ
	DrawBox(495, GAME_HEIGHT - 145, GAME_WIDTH - 205, GAME_HEIGHT - 55, GetColor(200, 200, 200), TRUE);

	DrawBox(280, GAME_HEIGHT - 150 + 10, 480, GAME_HEIGHT - 150 + 40, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 10, 280+ ((PlayChara.HP * 100 / PlayChara.MAX_HP) * 2), GAME_HEIGHT - 150 + 40, GetColor(255, 0, 0), TRUE);

	DrawBox(280, GAME_HEIGHT - 150 + 60, 480, GAME_HEIGHT - 150 + 90, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 60, 280 + ((PlayChara.MP * 100 / PlayChara.MAX_MP) * 2), GAME_HEIGHT - 150 + 90, GetColor(0, 0, 255), TRUE);

	//技表示
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
			DrawString(520, GAME_HEIGHT - 130, "MPが足りません", GetColor(0, 0, 0), FALSE);
	}


	if (PlayerTurn)
		DrawFormatString(520, GAME_HEIGHT - 130, GetColor(0, 0, 0), "%s の %s！",PlayChara.Name,PlayChara.cmd[BattlePlayCmd].Name);
	if (PlayerResult)
	{
		if(!PlayChara.cmd[BattlePlayCmd].Myself)
			DrawFormatString(520, GAME_HEIGHT - 110, GetColor(0, 0, 0), "%s に %d ダメージ！", Battleenemy[0].Name, Damage);
		else
			DrawFormatString(520, GAME_HEIGHT - 110, GetColor(0, 0, 0), "%s に %d 回復！", Battleenemy[0].Name, Damage);
	}
	if (EnemyTurn)
		DrawFormatString(520, GAME_HEIGHT - 130, GetColor(0, 0, 0), "%s のこうげき！", Battleenemy[0].Name);
	if(EnemyResult)
		DrawFormatString(520, GAME_HEIGHT - 110, GetColor(0, 0, 0), "%s に %d ダメージ！", PlayChara.Name, Damage);


	//ターン表示 (デバッグ限定)
	if (GAME_DEBUG)
	{
		DrawFormatString(20, 40, GetColor(0, 0, 0), "たーん:%d", turn);

		DrawFormatString(20, 60, GetColor(0, 0, 0), "%d,%s,%d,%d,%d,%d", Battleenemy[0].No, Battleenemy[0].Name, Battleenemy[0].HP, Battleenemy[0].MP, Battleenemy[0].ATK, Battleenemy[0].DEF);
		
		DrawFormatString(20, 80, GetColor(0, 0, 0), "%d,%s,%d,%d,%d,%d", PlayChara.No, PlayChara.Name, PlayChara.HP, PlayChara.MP, PlayChara.ATK, PlayChara.DEF);

		DrawFormatString(20, 100, GetColor(0, 0, 0), "Player:%d/%d\nEnemy:%d/%d", PlayerTurn, PlayerResult, EnemyTurn, EnemyResult);
	}
	if (GAME_DEBUG)DrawString(0, 0, "戦闘画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();	//処理
	EndDraw();	//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	//音楽を再生
	PlayAudio(endBGM);

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//ゲームデータの初期化
		GameInit();
		TitleInit();

		//SEを流す
		PlayAudio(sceneEnterSE);
		//音楽を止める
		StopAudio(&endBGM);

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawImage(EndImg);
	DrawImage(EndLogo);

	if (GAME_DEBUG)DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// ゲームオーバー画面
/// </summary>
VOID GameOver(VOID)
{
	GameOverProc();	//処理
	GameOverDraw();	//描画

	return;
}

/// <summary>
/// ゲームオーバー画面の処理
/// </summary>
VOID GameOverProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//初期化
		GameInit();
		TitleInit();

		//SEを流す
		PlayAudio(sceneEnterSE);
		//音楽を止める
		StopAudio(&gameoverBGM);

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//音楽を再生
	PlayAudio(gameoverBGM);

	return;
}

/// <summary>
/// ゲームオーバー画面の描画
/// </summary>
VOID GameOverDraw(VOID)
{
	DrawImage(GameoverImg);
	DrawImage(GameoverLogo);

	if (GAME_DEBUG)DrawString(0, 0, "ゲームオーバー画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//処理
	ChangeDraw();	//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを増やす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;				//フェードアウト処理終了
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}


	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_TUTORIAL:
		TutorialDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_BATTLE:
		BattleDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	case GAME_SCENE_GAMEOVER:
		GameOverDraw();
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (GAME_DEBUG)DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新（画像）
/// </summary>
/// <param name="img">画像構造体のポインタ</param>
VOID CollUpdateImage(IMAGE* img)
{
	img->coll.left = img->x;
	img->coll.top = img->y;

	img->coll.right = img->x + img->width;
	img->coll.bottom = img->y + img->height;

	return;
}

/// <summary>
/// 当たり判定の領域更新（分割画像）
/// </summary>
/// <param name="img">画像構造体のポインタ</param>
VOID CollUpdateDivImage(DIVIMAGE* div)
{
	div->coll.left = div->x + 5;
	div->coll.top = div->y + 10;

	div->coll.right = div->x + div->width - 5;
	div->coll.bottom = div->y + div->height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>あたったらTRUE/あたらないならFALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right &&		//　矩形Aの左辺X座標 < 矩形Bの右辺X座標　かつ
		a.right > b.left &&		//　矩形Aの右辺X座標 > 矩形Bの左辺X座標　かつ
		a.top  < b.bottom &&	//　矩形Aの上辺Y座標 < 矩形Bの下辺Y座標　かつ
		a.bottom > b.top		//　矩形Aの下辺Y座標 > 矩形Bの上辺Y座標
		)
	{
		//あたっているとき
		return TRUE;
	}
	else
	{
		//あたっていないとき
		return FALSE;
	}
}


/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="bunkatuYoko">分割するときの横の数</param>
/// <param name="bunkatuTate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadImageDivMem(DIVIMAGE* div, const char* path, int bunkatuYoko, int bunkatuTate)
{
	int IsLoad = -1;	//画像が読み込みたか？

	//一時的に画像のハンドルを用意する
	int TempHandle = LoadGraph(path);

	//読み込みエラー
	if (TempHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	int Width = -1;		//幅
	int Height = -1;	//高さ
	GetGraphSize(TempHandle, &Width, &Height);

	//分割して読み込み
	IsLoad = LoadDivGraph(
		path,							//画像のパス
		bunkatuYoko * bunkatuTate,		//分割総数
		bunkatuYoko, bunkatuTate,					//横の分割,縦の分割
		Width / bunkatuYoko, Height / bunkatuTate,	//画像1つ分の幅,高さ
		div->handle									//連続で管理する配列の先頭アドレス
	);

	//分割エラー
	if (IsLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,					//本文
			"画像分割エラー",		//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//情報を設定
	div->DivYoko = bunkatuYoko;
	div->DivTate = bunkatuTate;
	div->DivMax = div->DivYoko * div->DivTate;
	div->x = 0;
	div->y = 0;
	GetGraphSize(div->handle[0], &div->width, &div->height);
	div->AnimCnt = 0;
	div->IsAnimLoop = FALSE;
	div->nowIndex = 0;

	//当たり判定の更新
	CollUpdateDivImage(div);

	//画像を描画する
	div->IsDraw = TRUE;

	//一時的に読み込んだハンドルを解放
	DeleteGraph(TempHandle);

	return TRUE;
}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="image">画像構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//ゴールの画像を読み込み
	strcpyDx(image->path, path);			//パスのコピー
	image->handle = LoadGraph(image->path);	//画像の読み込み

	//画像が読み込めなかったときは、エラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,			//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	//画像の位置を初期化
	image->x = 0; image->y = 0;

	//当たり判定の更新
	CollUpdateImage(image);

	//画像を描画する
	image->IsDraw = TRUE;

	//読み込めた
	return TRUE;
}

/// <summary>
/// 画像描画
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawImage(IMAGE image)
{
	//画像が描画できるときは
	if (image.IsDraw == TRUE)
	{
		DrawGraph(image.x, image.y, image.handle, TRUE);
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image.coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// 分割画像の描画
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImage(DIVIMAGE* image)
{
	//画像が描画できるときは
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		//アニメーションのカウントアップ
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{
			//すべてのアニメーションを描画し終わったら
			if (image->nowIndex == image->DivMax - 1)
			{
				//アニメーションを繰り返さないならば
				if (image->IsAnimLoop == FALSE)
				{
					image->IsDraw = FALSE;	//描画を止める
				}
				image->AnimCnt = 0;		//カウンタ0クリア
				image->nowIndex = 0;	//先頭に戻す
			}
			//次の分割画像があるときは
			else if (image->nowIndex < image->DivMax - 1)
			{
				image->nowIndex++; //次の画像へ
			}
			image->AnimCnt = 0;	//カウンタ0クリア
		}
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// 分割画像の描画(キャラなど)
/// </summary>
/// <param name="image">描画する画像構造体</param>
/// <returns></returns>
/// <returns></returns>
VOID DrawDivImageChara(DIVIMAGE* image)
{
	//画像が描画できるときは
	if (image->IsDraw == TRUE)
	{
		DrawGraph(image->x, image->y, image->handle[image->nowIndex], TRUE);

		if (muki == muki_none)
		{
			image->AnimCnt = image->AnimCntMAX - 1;
		}

		//アニメーションのカウントアップ
		if (image->AnimCnt < image->AnimCntMAX) { image->AnimCnt++; }
		else
		{
			//右向きのとき(画像によって数字が違うので、合わせること！)
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

			//左向きのとき(画像によって数字が違うので、合わせること！)
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

			//上向きのとき(画像によって数字が違うので、合わせること！)
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

			//下向きのとき(画像によって数字が違うので、合わせること！)
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

			image->AnimCnt = 0;	//カウンタ0クリア
		}

		//向き無しのときは、直前の向きの真ん中の画像にする(画像に合わせて決めてネ)
		if (muki == muki_none)
		{
			if (image->nowIndex >= 8 && image->nowIndex <= 11) { image->nowIndex = 8; }
			if (image->nowIndex >= 4 && image->nowIndex <= 7) { image->nowIndex = 4; }
			if (image->nowIndex >= 12 && image->nowIndex <= 15) { image->nowIndex = 12; }
			if (image->nowIndex >= 0 && image->nowIndex <= 3) { image->nowIndex = 0; }
		}
	}

	//デバッグモードのときは
	if (GAME_DEBUG == TRUE)
	{
		//当たり判定を描画
		DrawRect(image->coll, GetColor(255, 255, 255), FALSE);
	}
	return;
}

/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int Volume, int playType)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);					//パスのコピー
	audio->handle = LoadSoundMem(audio->path);		//音楽の読み込み

	//音楽が読み込めなかったときは、エラー(-1)が入る
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,			//メッセージ本文
			"音楽読み込みエラー！",		//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//その他の設定
	audio->Volume = Volume;
	audio->playType = playType;

	return TRUE;
}

/// <summary>
/// 音楽再生
/// </summary>
/// <param name="audio">再生する音楽構造体</param>
/// <returns></returns>
VOID PlayAudio(AUDIO audio)
{
	//ループ再生でなければ、そのまま再生する
	if (audio.playType == DX_PLAYTYPE_BACK)
	{
		PlaySoundMem(audio.handle, audio.playType);//音楽の再生
	}
	//ループ再生のときは、音楽が再生されていなければ、再生する
	else if (audio.playType == DX_PLAYTYPE_LOOP)
	{
		if (CheckSoundMem(audio.handle) == 0)
		{
			PlaySoundMem(audio.handle, audio.playType);//音楽の再生
		}
	}

	return;
}

/// <summary>
/// 音楽のボリューム取得
/// </summary>
/// <param name="audio">取得する音楽構造体</param>
/// <param name = "volume">ボリューム(0〜255)</param>
/// <returns></returns>
int GetVolumeAudio(AUDIO audio)
{
	//音量を取得
	return audio.Volume;
}

/// <summary>
/// 音楽のボリューム設定
/// </summary>
/// <param name="audio">設定する音楽構造体</param>
/// <param name = "volume">ボリューム(0〜255)</param>
/// <returns></returns>
VOID SetVolumeAudio(AUDIO* audio, int vol)
{
	//音量を設定
	audio->Volume = vol;
	ChangeVolumeSoundMem(audio->Volume, audio->handle);

	return;
}

/// <summary>
/// 音楽のボリューム変更
/// </summary>
/// <param name="audio">変更する音楽構造体</param>
/// <param name = "volume">ボリューム(0〜255)</param>
/// <returns></returns>
VOID ChangeVolumeAudio(AUDIO* audio, int vol)
{
	//音量を設定（構造体のボリュームは変更しない）
	ChangeVolumeSoundMem(vol, audio->handle);

	return;
}

/// <summary>
/// 音楽停止
/// </summary>
/// <param name="audio">停止する音楽構造体</param>
/// <returns></returns>
VOID StopAudio(AUDIO* audio)
{
	//音楽の停止
	StopSoundMem(audio->handle);

	return;
}

/// <summary>
/// イベントの当たり判定の描画
/// </summary>
/// <param name="events"></param>
VOID DrawHitBox(EVENT* events)
{
	if(GAME_DEBUG==TRUE)
		DrawBox(events->coll.left, events->coll.top, events->coll.right, events->coll.bottom, GetColor(0, 255, 0), FALSE);

	return;
}

/// <summary>
/// アイテム数の初期化(仮でアイテムと採取地点の関数同一化)
/// </summary>
/// <param name="events">イベントの関数</param>
/// <param name="Wood">木の数</param>
/// <param name="Stone">石の数</param>
/// <param name="CntMax">アイテムの最大所持数/NULLなら上限なし(仮)</param>
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

VOID SetWeaponStatus(MENU* tool, const char name[255], int wood, int stone, int metal, int HP, int MP, int ATK, int DEF)
{
	strcpyDx(tool->string, name);

	tool->Wood = wood;
	tool->Stone = stone;
	tool->Metal = metal;

	tool->HP = HP;
	tool->MP = MP;
	tool->ATK = ATK;
	tool->DEF = DEF;

	return;
}