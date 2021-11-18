//ヘッダファイル読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理
#include "mouse.h"		//マウスの処理
#include "shape.h"		//図形の処理
#include "font.h"		//フォントの処理
#include "Data.h"		//データの処理

//独自のマクロ定義

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


//★★★ゲーム共通のプロトタイプ宣言★★★
BOOL GameLoad(VOID);	//データを読込
VOID GameInit(VOID);	//データの初期化
VOID GameDelete(VOID);	//データを削除

VOID TitleInit(VOID);
VOID PlayInit(VOID);
VOID EndInit(VOID);
VOID GameOverInit(VOID);

VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)

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

//★★★ゲーム共通のプロトタイプ宣言★★★

//サンプルの素材
IMAGE sampleImg;
DIVIMAGE sampleDivImg;
DIVIMAGE samplePlayerImg;
MUKI muki = muki_shita;		//サンプル向き

AUDIO sampleBGM;
AUDIO titleBGM;
AUDIO playBGM;
AUDIO endBGM;
AUDIO gameoverBGM;

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

//ロゴなどの画像
IMAGE TitleImg;
IMAGE EndImg;
IMAGE GameoverImg;

IMAGE PushEnter;


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
	if (LoadCSVChara(CSV_PATH_ENEMY, &enemy[0], ENEMY_MAX, TRUE) == FALSE) { return FALSE; }

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
	if (LoadAudio(&endBGM, ".\\audio\\ブルーボトル.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }
	if (LoadAudio(&gameoverBGM, ".\\audio\\拭えない悲しみと迫り来る明日.mp3", 128, DX_PLAYTYPE_LOOP) == FALSE) { return FALSE; }


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
	DeleteMusicMem(endBGM.handle);
	DeleteMusicMem(gameoverBGM.handle);

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
	samplePlayerImg.speed = 2;
	samplePlayerImg.x = 10;
	samplePlayerImg.y = 10;
	//samplePlayerImg.x = MAP1_YOKO_MAX * map2.width / 2 - samplePlayerImg.width / 2;
	//samplePlayerImg.y = MAP1_TATE_MAX * map2.height / 2 - samplePlayerImg.height / 2;

	muki = muki_none;

	//サンプルイベントの作成
	//CreateEventMultiMass(9, 1, 11, 2, &sampleevent, map2);

	//イベントの作成
	CreateEventMass(3, 9, &CreatePickaxe, map2);
	CreateEventMass(16, 9, &CreateAxe, map2);
	CreateEventMass(16, 4, &CreateKey, map2);

	CreateEventMultiMass(8, 15, 12, 18, &GetItem, map2);
	CreateEventMultiMass(2, 16, 4, 18, &GetWood, map2);
	CreateEventMultiMass(16, 16, 19, 18, &GetStone, map2);

	CreateEventMultiMass(9, 1, 11, 2, &Goal, map2);

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

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//ゲームデータの初期化
		GameInit();
		EndInit();

		//音楽を止める
		StopAudio(&playBGM);

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_END);

		return;
	}

	//音楽を再生
	PlayAudio(playBGM);

	//マップの当たり判定
	{
		muki = muki_none;					//最初は向きなし
		DIVIMAGE dummy = samplePlayerImg;	//当たり判定のダミー
		DIVIMAGE dummy2 = samplePlayerImg;	//当たり判定のダミー
		if (KeyDown(KEY_INPUT_W)) { muki = muki_ue; dummy.y -= samplePlayerImg.speed; }
		else if (KeyDown(KEY_INPUT_S)) { muki = muki_shita; dummy.y += samplePlayerImg.speed; }
		if (KeyDown(KEY_INPUT_A)) { muki = muki_hidari; dummy.x -= samplePlayerImg.speed; }
		else if (KeyDown(KEY_INPUT_D)) { muki = muki_migi; dummy.x += samplePlayerImg.speed; }

		CollUpdateDivImage(&dummy);	//当たり判定の更新

		if (CollMap(dummy.coll, map2) == FALSE)
		{
			samplePlayerImg = dummy;	//ダミーの情報を戻す
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

		if (samplePlayerImg.x>=GAME_WIDTH/2 && map2.x[0][0]<=0
			&& map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1]>GAME_WIDTH && dummy2.x != samplePlayerImg.x)
		{
			samplePlayerImg.screenX = samplePlayerImg.speed;
			samplePlayerImg.x = dummy2.x;	//ダミーの情報を戻す
			MapMove(&map2);
		}
		else if (samplePlayerImg.x <= GAME_WIDTH / 2 && map2.x[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] >= GAME_WIDTH
			&& map2.x[0][0]<0 && dummy2.x != samplePlayerImg.x)
		{
			samplePlayerImg.screenX = -samplePlayerImg.speed;
			samplePlayerImg.x = dummy2.x;	//ダミーの情報を戻す
			MapMove(&map2);
		}
		else
		{
			samplePlayerImg.screenX = 0;
		}

		if (samplePlayerImg.y>=GAME_HEIGHT/2 && map2.y[0][0]<=0
			&& map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1]>GAME_HEIGHT && dummy2.y != samplePlayerImg.y)
		{
			samplePlayerImg.screenY = samplePlayerImg.speed;
			samplePlayerImg.y = dummy2.y;	//ダミーの情報を戻す
			MapMove(&map2);
		}
		else if (samplePlayerImg.y <= GAME_HEIGHT / 2 && map2.y[MAP1_TATE_MAX - 1][MAP1_YOKO_MAX - 1] >= GAME_HEIGHT
			&& map2.y[0][0]<0 && dummy2.y != samplePlayerImg.y)
		{
			samplePlayerImg.screenY = -samplePlayerImg.speed;
			samplePlayerImg.y = dummy2.y;	//ダミーの情報を戻す
			MapMove(&map2);
		}
		else
		{
			samplePlayerImg.screenY = 0;
		}



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

		GetItemSystem(&GetItem,NULL);
		GetItemSystem(&GetWood,&CreateAxe);
		GetItemSystem(&GetStone,&CreatePickaxe);
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

	//数値を出したいとき
	DrawFormatStringToHandle(900, GAME_HEIGHT - 100, GetColor(0, 0, 0), sampleFont2.handle, "残り:%3.2f", GameTimeLimit);

	DrawFormatStringToHandle(650, 40, GetColor(0, 0, 0), sampleFont1.handle, "ピッケル:%d\n斧　　　:%d\n鍵　　　:%d\n\n木:%d　石:%d",CreatePickaxe.Cnt,CreateAxe.Cnt,CreateKey.Cnt,ItemWood,ItemStone);

	CreateItemDraw(&CreatePickaxe);
	CreateItemDraw(&CreateAxe);
	CreateItemDraw(&CreateKey);

	GetItemDraw(&GetItem);
	GetItemDraw(&GetWood);
	GetItemDraw(&GetStone);
	
	if (GAME_DEBUG)DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
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

