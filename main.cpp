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

int ItemWood;
int ItemStone;

BOOL canGetItem;


int MenuStringLeft;
int MenuStringRight;

//コンフィグ関連
MENU test[2];

//メニュー画面のサンプル
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


//★★★ゲーム共通のプロトタイプ宣言★★★
BOOL GameLoad(VOID);	//データを読込
VOID GameInit(VOID);	//データの初期化
VOID GameDelete(VOID);	//データを削除

VOID TitleInit(VOID);
VOID PlayInit(VOID);
VOID BattleInit(VOID);
VOID EndInit(VOID);
VOID GameOverInit(VOID);

VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

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

VOID CreateItem(EVENT* events);					//アイテム作成
VOID CreateItemDraw(EVENT* events);				//アイテム作成時の必要数表示

VOID GetItemSystem(EVENT* events, EVENT* tools);				//採取の個数変動
VOID GetItemDraw(EVENT* events);				//採取時の個数表示

VOID SetEventUpdate();							//イベントの場所更新

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

MAP_DATA map1;
MAP_DATA map2;

//イベントマス
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

EVENT GetMainItem[MAIN_ITEM_KIND];	//石、木材、金属

//ロゴなどの画像
IMAGE TitleImg;
IMAGE EndImg;
IMAGE GameoverImg;

IMAGE PushEnter;

BOOL MenuScreen;
BOOL MenuScreenInventory;

BOOL MenuRight;


//戦闘画面
CHARA_DATA Battleenemy[ENEMY_MAX];
CHARA_DATA PlayChara;

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
	if (LoadImageMem(&TitleImg, ".\\Image\\脱出_タイトル.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&EndImg, ".\\Image\\脱出_ゲームクリア.png") == FALSE) { return FALSE; }
	if (LoadImageMem(&GameoverImg, ".\\Image\\脱出_ゲームオーバー.png") == FALSE) { return FALSE; }
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

	//フォントデータを削除
	FontDelete();

	//画像を削除
	DeleteGraph(TitleImg.handle);
	DeleteGraph(EndImg.handle);
	DeleteGraph(GameoverImg.handle);
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

	PushEnter.IsDraw = TRUE;
	PushEnter.x = 0;
	PushEnter.y = 0;

	return;
}

VOID PlayInit(VOID)
{
	//サンプルプレイヤー初期化
	samplePlayerImg.speed = 9;
	samplePlayerImg.x = 20;
	samplePlayerImg.y = 10;
	//samplePlayerImg.x = MAP1_YOKO_MAX * map2.width / 2 - samplePlayerImg.width / 2;
	//samplePlayerImg.y = MAP1_TATE_MAX * map2.height / 2 - samplePlayerImg.height / 2;

	muki = muki_none;

	//サンプルイベントの作成
	//CreateEventMultiMass(9, 1, 11, 2, &sampleevent, map2);

	//イベントの作成
	SetEventUpdate();

	//時間制限
	GameTimeLimit = GameTimeLimitMax;

	//アイテムの個数の初期化
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

	strcpyDx(sampleGetItem[0].string, "アイテムてすと1");
	strcpyDx(sampleGetItem[1].string, "アイテムてすと2");
	strcpyDx(sampleGetItem[2].string, "アイテムてすと3");
	strcpyDx(sampleGetItem[3].string, "アイテムてすと4");

	for (int i = 0; i < 4; i++)
	{
		sampleGetItemMenu[i].Cnt = 0;
	}

	//メニューを開いていない
	MenuScreen = FALSE;
	MenuScreenInventory = FALSE;

	MenuStringLeft = 0;
	MenuStringRight = 0;

	MenuRight = FALSE;

	//コンフィグを初期化

	OpenVolumecfg = FALSE;

	//項目増やすテストのやつの初期化
	strcpyDx(DrawConfig.string, "表示数");
	DrawConfig.Cnt = 1;
	DrawConfig.can = FALSE;

	for (int i = 0; i < 3; i++)
	{
		test[i].Cnt = 0;
		test[i].can = FALSE;
	}
	strcpyDx(test[0].string, "関数てすと1");
	strcpyDx(test[1].string, "関数てすと2");
	strcpyDx(test[2].string, "関数てすと3");

	//イベントの初期化
	for (int i = 0; i < MAIN_ITEM_KIND; i++)
	{
		GetMainItem[i].Cnt = 0;
		GetMainItem[i].can = FALSE;

	}

	//イベントの文字列コピー
	strcpyDx(GetMainItem[0].string, "石");
	strcpyDx(GetMainItem[1].string, "木材");
	strcpyDx(GetMainItem[2].string, "金属");

	//取得したアイテムの種類の個数の初期化
	sampleGetItemMenuCnt = 0;
	GetMainItemCnt = 0;

	samplePlayerImg.screenX = 0;
	samplePlayerImg.screenY = 0;
	MapInit(&map2);
	
	return;
}

VOID BattleInit()
{
	//敵初期化
	enemyImg1.x = GAME_WIDTH / 2 - enemyImg1.width / 2;
	enemyImg1.y = GAME_HEIGHT / 2 - enemyImg1.height / 2 - 50;	//マジックナンバー
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
	//エンド画面の初期化
	EndImg.IsDraw = TRUE;
	EndImg.x = 0;
	EndImg.y = 0;

	return;
}

VOID GameOverInit(VOID)
{
	//ゲームオーバー画面の初期化
	GameoverImg.IsDraw = TRUE;
	GameoverImg.x = 0;
	GameoverImg.y = 0;

	return;
}

/// <summary>
/// イベントの座標の更新
/// </summary>
VOID SetEventUpdate()
{
	//イベント
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
	//セーブデータ系サンプル
	{
		//セーブデータ削除サンプル
		if (KeyDown(KEY_INPUT_Q)) { DeleteScoreData(); }

		//セーブデータ保存サンプル
		if (KeyDown(KEY_INPUT_E))
		{
			//スコアデータを設定
			score_data.Score1 = (int)GetGameTime();
			strcpyDx(score_data.Name1, "Taro");
			strcpyDx(score_data.Name2, "Jiro");
			strcpyDx(score_data.Name3, "Saburo");

			//スコアデータの並び替え

			//スコアデータ保存
			SaveScoreData();
		}
	}


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
		PlayInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	PlayAudio(titleBGM);	//BGMを鳴らす

	//プレイヤーの動作サンプル
	/*
	{
		muki = muki_none;	//最初は向きを無しにする
		if (KeyDown(KEY_INPUT_W)) { muki = muki_ue; samplePlayerImg.y--; }
		else if (KeyDown(KEY_INPUT_S)) { muki = muki_shita; samplePlayerImg.y++; }
		if (KeyDown(KEY_INPUT_A)) { muki = muki_hidari; samplePlayerImg.x--; }
		else if (KeyDown(KEY_INPUT_D)) { muki = muki_migi; samplePlayerImg.x++; }
		CollUpdateDivImage(&samplePlayerImg);	//当たり判定の更新
	}
	*/

	

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{

	/*
	DrawImage(sampleImg);				//サンプル画像の描画
	DrawDivImage(&sampleDivImg);		//サンプル分割画像の描画

	//プレイヤーの動作サンプル
	{
		DrawDivImageChara(&samplePlayerImg);//サンプル分割画像の描画
	}

	//ゲーム内時間
	DrawFormatString(500, 50, GetColor(0, 0, 0), "TIME:%3.2f", GetGameTime());

	//ゲーム内時間
	DrawFormatString(500, 30, GetColor(0, 0, 0), "残り:%3.2f", GameTimeLimit);

	//現在の日付と時刻
	DrawFormatString(500, 70, GetColor(0, 0, 0), "DATE:%4d/%2d/%2d %2d:%2d:%2d", fps.NowDataTime.Year, fps.NowDataTime.Mon, fps.NowDataTime.Day, fps.NowDataTime.Hour, fps.NowDataTime.Min, fps.NowDataTime.Sec);

	//フォントのサンプル
	DrawStringToHandle(100, 100, "MS ゴシックだよ", GetColor(0, 0, 0), sampleFont1.handle);
	
	//フォントのサンプル
	DrawStringToHandle(100, 150, "昔々ふぉんとだよ", GetColor(0, 0, 0), sampleFont3.handle);

	//数値を出したいとき
	DrawFormatStringToHandle(800, 200, GetColor(0, 0, 0), sampleFont2.handle, "残り:%3.2f",GameTimeLimit);
	
	//読み込んだデータを描画
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		DrawFormatString(300, 300 + i * 20, GetColor(0, 0, 0), "%s,%2d,%2d,%2d"
			, enemy[i].Name, enemy[i].HP, enemy[i].ATK, enemy[i].DEF);
	}
	*/

	DrawImage(TitleImg);
	DrawImage(PushEnter);

	if(GAME_DEBUG)DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
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

	


	
	//Xを押したときの関数反転
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

	//メニューを開いていない(通常時)時
	if (MenuScreen == FALSE)
	{
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
			//画面端にいった場合
			/*if (samplePlayerImg.y < map2.y[0][0]) { samplePlayerImg.y = map2.y[0][0]; }	//未確認
			if (samplePlayerImg.y > map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] - 20)
			{
				samplePlayerImg.y = map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] - 20;	//マジックナンバー
			}

			if (samplePlayerImg.x < map2.x[0][0]) { samplePlayerImg.x = map2.x[0][0]; }
			if (samplePlayerImg.x > map2.x[MAP1_TATE_MAX-1][MAP1_YOKO_MAX-1])
			{
				samplePlayerImg.x = map2.x[MAP1_TATE_MAX-1][MAP1_YOKO_MAX-1];
			}*/

			//移動した後のやつ
			DIVIMAGE dummy3 = samplePlayerImg;

			//プレイヤーの幅の考慮無し,要修正
			if (samplePlayerImg.x > GAME_WIDTH / 2
				&& map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_WIDTH && dummy2.x != samplePlayerImg.x)
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
				&& map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] > GAME_HEIGHT && dummy2.y != samplePlayerImg.y)
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
			CollMapUpdate(&map2);		//マップの当たり判定更新
			SetEventUpdate();		//イベントの座標の更新

			CollUpdateDivImage(&samplePlayerImg);	//当たり判定の更新

			//イベントマスに当たっているか
			if (CheckCollRectToRect(samplePlayerImg.coll, Goal.coll) == TRUE && CreateKey.Cnt > 0)
			{
				//ゲームデータの初期化
				GameInit();
				EndInit();

				//マップ移動のフラグ
				//sampleevent.can = TRUE;

				//音楽を止める
				StopAudio(&playBGM);

				//プレイ画面に切り替え
				ChangeScene(GAME_SCENE_END);
			}

			CreateItem(&CreatePickaxe);
			CreateItem(&CreateAxe);
			CreateItem(&CreateKey);

			GetItemSystem(&GetItem, NULL);
			GetItemSystem(&GetWood, &CreateAxe);
			GetItemSystem(&GetStone, &CreatePickaxe);

			//アイテムを追加で項目増やすサンプルイベント
			if (CheckCollRectToRect(samplePlayerImg.coll, CreateItems.coll))
			{
				if (KeyClick(KEY_INPUT_Z))
				{
					//描画数を増やす
					DrawConfig.Cnt++;
				}
			}

			//イベントを配列で管理するサンプル
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
								//文字列コピー
								strcpyDx(sampleGetItemMenu[sampleGetItemMenuCnt].string, sampleGetItem[i].string);
								sampleGetItemMenu[sampleGetItemMenuCnt].Cnt++;
								sampleGetItemMenuCnt++;
								break;
							}
						}

						
					}
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
						for (int j = 0; j < MAIN_ITEM_KIND; j++)
						{
							//探査(strcmpDxの仕様良くわかっていない、とりあえず動作したのでこのまま)
							if (!strcmpDx(GetMainItemMenu[j].string, GetMainItem[i].string))
							{
								GetMainItemMenu[j].Cnt++;
								break;
							}
							//存在してなく、最後まで行った場合
							else if (j == MAIN_ITEM_KIND - 1)
							{
								//文字列コピー
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
			//ゲームデータの初期化
			GameInit();
			GameOverInit();

			//音楽を止める
			StopAudio(&playBGM);

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_GAMEOVER);
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

			//左
			//左の項目を上限を超えて上に行ったとき(のちにdefine化)
			if (MenuStringLeft < 0)
			{
				MenuStringLeft = 5;
			}
			//左の項目を上限を超えて下に行ったとき
			else if (MenuStringLeft > 5)
			{
				MenuStringLeft = 0;
			}
			//左の一項目目の時
			if (MenuStringLeft == 0)
			{
				//右
				//↑上限超えたら(個別にdefine用意推奨)
				if (MenuStringRight < 0)
				{
					MenuStringRight = 2;
				}
				//↓上限超えたら
				else if (MenuStringRight > 2)
				{
					MenuStringRight = 0;
				}
			}
			//左の二項目目の時
			if (MenuStringLeft == 1)
			{
				//右
				if (MenuStringRight < 0)
				{
					MenuStringRight = 2;
				}
				else if (MenuStringRight > 2)
				{
					MenuStringRight = 0;
				}
			}
			//左の三項目目の時
			if (MenuStringLeft == 2)
			{
				//右
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
				//右
				//試験的にバーで項目数を増やせるように
				if (MenuStringRight < 0)
				{
					MenuStringRight = DrawConfig.Cnt - 1;
				}
				else if (MenuStringRight > DrawConfig.Cnt - 1)
				{
					MenuStringRight = 0;
				}
			}
			//左の五項目目の時
			if (MenuStringLeft == 4)
			{
				//右
				if (MenuStringRight < 0)
				{
					MenuStringRight = sampleGetItemMenuCnt-1;
				}
				else if (MenuStringRight > sampleGetItemMenuCnt-1)
				{
					MenuStringRight = 0;
				}
			}
			//左の六項目目の時
			if (MenuStringLeft == 5)
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

			//決定ボタンを押したとき
			if (KeyClick(KEY_INPUT_Z))
			{
				//左項目にいるとき
				if (MenuRight == FALSE)
				{
					//右に移動
					MenuRight = TRUE;
				}
				//右項目にいるとき
				else
				{


					//三行目
					if (MenuStringLeft == 2)
					{
						//三行目
						if (MenuStringRight == 2)
						{
							//決定を押したときにBOOL型を反転
							if (OpenVolumecfg == FALSE)
								OpenVolumecfg = TRUE;
							else
								OpenVolumecfg = FALSE;
						}
					}
					//四行目
					if (MenuStringLeft == 3)
					{
						//一行目
						if (MenuStringRight == 0)
						{
							//決定を押したときにBOOL型を反転
							if (DrawConfig.can == FALSE)
								DrawConfig.can = TRUE;
							else
								DrawConfig.can = FALSE;
						}
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

			//項目増やすテスト
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
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//マップのサンプル
	DrawMap(map2);

	//当たり判定の描画
	//DrawHitBox(&sampleevent);

	DrawHitBox(&CreatePickaxe);
	DrawHitBox(&CreateAxe);
	DrawHitBox(&CreateKey);

	DrawHitBox(&GetItem);
	DrawHitBox(&GetWood);
	DrawHitBox(&GetStone);

	DrawHitBox(&Goal);

	//サンプル
	DrawHitBox(&CreateItems);
	for (int i = 0; i < 4; i++) { DrawHitBox(&sampleGetItem[i]); }

	for (int i = 0; i < MAIN_ITEM_KIND; i++) { DrawHitBox(&GetMainItem[i]); }



	//数値を出したいとき
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(930, GAME_HEIGHT - 90, GAME_WIDTH, GAME_HEIGHT, GetColor(50, 50, 50), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatStringToHandle(950, GAME_HEIGHT - 75, GetColor(0, 0, 0), sampleFont2.handle, "残り:%3.2f", GameTimeLimit);

	DrawFormatStringToHandle(650, 40, GetColor(0, 0, 0), sampleFont1.handle, "ピッケル:%d\n斧　　　:%d\n鍵　　　:%d\n\n木:%d　石:%d",CreatePickaxe.Cnt,CreateAxe.Cnt,CreateKey.Cnt,ItemWood,ItemStone);

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
			//左ブロック
			if (MenuRight == FALSE)
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);
			else
			{
				DrawBox(GAME_WIDTH / 6 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH / 3 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}


			if (MenuStringLeft != 0)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "てすと1", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 20, "てすと1", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 1)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "てすと2", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 40, "てすと2", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 2)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "てすと3", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 60, "てすと3", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 3)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "てすと4", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 80, "てすと4", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 4)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 100, "てすと5", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 100, "てすと5", GetColor(200, 200, 200), FALSE);

			if (MenuStringLeft != 5)
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 120, "素材アイテム", GetColor(100, 100, 100), FALSE);
			else
				DrawString(GAME_WIDTH / 6 + 20, GAME_HEIGHT / 6 + 120, "素材アイテム", GetColor(200, 200, 200), FALSE);


			//右ブロック
			if (MenuRight == FALSE)
			{
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(150, 150, 150), TRUE);
			}
			else
				DrawBox(GAME_WIDTH / 3 + 10, GAME_HEIGHT / 6 + 10, GAME_WIDTH * 5 / 6 - 10, GAME_HEIGHT * 5 / 6 - 10, GetColor(170, 170, 170), TRUE);

			//右側の文字 汚いので要修正
			if (MenuStringLeft == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					//for文で表示させるテスト
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(200, 200, 200), "てすと%d", i + 4);
					else
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(100, 100, 100), "てすと%d", i + 4);
				}
			}
			if (MenuStringLeft == 1)
			{
				for (int i = 0; i < 3; i++)
				{
					//配列の関数を表示させるテスト
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), test[i].string, GetColor(200, 200, 200), FALSE);
					else
						DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), test[i].string, GetColor(100, 100, 100), FALSE);
				}

			}
			if (MenuStringLeft == 2)
			{
				if (MenuStringRight == 0 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "てすと10", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20, "てすと10", GetColor(100, 100, 100), FALSE);

				if (MenuStringRight == 1 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 40, "マップ拡大率", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 40, "マップ拡大率", GetColor(100, 100, 100), FALSE);

				if (MenuStringRight == 2 && MenuRight == TRUE)
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 60, "音量", GetColor(200, 200, 200), FALSE);
				else
					DrawString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 60, "音量", GetColor(100, 100, 100), FALSE);



				//音量調節のバー
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
				//入手した順にアイテムを追加していくサンプル
				for (int i = 0; i < sampleGetItemMenuCnt; i++)
				{
					if (MenuStringRight == i && MenuRight == TRUE)
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(200, 200, 200), "%16s x%d", sampleGetItemMenu[i].string, sampleGetItemMenu[i].Cnt);
					else
						DrawFormatString(GAME_WIDTH / 3 + 20, GAME_HEIGHT / 6 + 20 + (i * 20), GetColor(100, 100, 100), "%16s x%d", sampleGetItemMenu[i].string, sampleGetItemMenu[i].Cnt);
				}
			}
			//素材アイテム表示
			if (MenuStringLeft == 5)
			{
				//入手した順にアイテムを追加していく
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
	//敵の描画
	DrawImage(enemyImg1);

	//敵のバー表示
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
	//MP未実装


	//味方のウィンドウ+バー
	DrawBox(180, GAME_HEIGHT - 150, GAME_WIDTH - 200, GAME_HEIGHT - 50, GetColor(230, 230, 230), TRUE);
	DrawString(190, GAME_HEIGHT - 135, "HP", GetColor(200, 0, 0), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 135, GetColor(0, 0, 0), "%2d/%2d", PlayChara.HP,PlayChara.MAX_HP);

	DrawString(190, GAME_HEIGHT - 85, "MP", GetColor(0, 0, 200), FALSE);
	DrawFormatString(220, GAME_HEIGHT - 85, GetColor(0, 0, 0), "%2d/%2d", PlayChara.MP, PlayChara.MAX_MP);

	DrawBox(495, GAME_HEIGHT - 145, GAME_WIDTH - 205, GAME_HEIGHT - 55, GetColor(200, 200, 200), TRUE);
	//DrawString(520, GAME_HEIGHT - 130, "てすと", GetColor(0, 0, 0), FALSE);

	DrawBox(280, GAME_HEIGHT - 150 + 10, 480, GAME_HEIGHT - 150 + 40, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 10, 280+ ((PlayChara.HP * 100 / PlayChara.MAX_HP) * 2), GAME_HEIGHT - 150 + 40, GetColor(255, 0, 0), TRUE);

	DrawBox(280, GAME_HEIGHT - 150 + 60, 480, GAME_HEIGHT - 150 + 90, GetColor(150, 150, 150), TRUE);
	DrawBox(280, GAME_HEIGHT - 150 + 60, 280 + ((PlayChara.MP * 100 / PlayChara.MAX_MP) * 2), GAME_HEIGHT - 150 + 90, GetColor(0, 0, 255), TRUE);

	//ログ表示
	DrawBox(GAME_WIDTH - 350, 50, GAME_WIDTH - 50, 190, GetColor(200, 200, 200), TRUE);

	if (PlayerTurn)
		DrawString(520, GAME_HEIGHT - 130, "プレイヤーのこうげき！", GetColor(0, 0, 0), FALSE);
	if(PlayerResult)
		DrawString(520, GAME_HEIGHT - 110, "敵に１ダメージ！", GetColor(0, 0, 0), FALSE);
	if (EnemyTurn)
		DrawString(520, GAME_HEIGHT - 130, "敵のこうげき！", GetColor(0, 0, 0), FALSE);
	if(EnemyResult)
		DrawString(520, GAME_HEIGHT - 110, "プレイヤーに１ダメージ！", GetColor(0, 0, 0), FALSE);


	//ターン表示 (デバッグ限定予定)
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

	//数値を出したいとき
	DrawFormatStringToHandle(900, 620, GetColor(0, 0, 0), sampleFont2.handle, "時間:%3.2f", GameTimeLimitMax - GameTimeLimit);

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

	//数値を出したいとき
	DrawFormatStringToHandle(900, 620, GetColor(0, 0, 0), sampleFont2.handle, "時間:%3.2f", GameTimeLimitMax - GameTimeLimit);

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
/// <param name = "volume">ボリューム(0～255)</param>
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
/// <param name = "volume">ボリューム(0～255)</param>
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
/// <param name = "volume">ボリューム(0～255)</param>
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
/// アイテム作成
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
/// クラフト時のアイテム必要個数の描画
/// </summary>
VOID CreateItemDraw(EVENT* events)
{
	//クラフト
	{
		if (events->can == TRUE)
		{
			DrawFormatStringToHandle(900, 40, GetColor(0, 0, 0), sampleFont1.handle,
				"クラフト必要個数\n木: %d / %d\n石: %d / %d", ItemWood, events->Wood, ItemStone, events->Stone);
		}
	}

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

/// <summary>
/// 採取時のアイテムの加算(簡易)
/// </summary>
/// <param name="events">採取イベント</param>
/// <param name="tools">必要ツールのイベント関数</param>
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
/// 採取アイテム作成
/// </summary>
/// <param name="events">採取イベント</param>
VOID GetItemDraw(EVENT* events)
{
	{
		if (events->can == TRUE)
		{
			if (canGetItem == TRUE)
			{
				DrawFormatStringToHandle(900, 40, GetColor(0, 0, 0), sampleFont1.handle,
					"採取個数\n木: %d\n石: %d", events->Wood, events->Stone);
			}
			else
			{
				DrawFormatStringToHandle(900, 40, GetColor(0, 0, 0), sampleFont1.handle,
					"採取不可");
			}
		}
	}

	return;
}

