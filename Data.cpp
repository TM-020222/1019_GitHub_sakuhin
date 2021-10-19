//-�w�b�_�[�t�@�C���ǂݍ���-
#include "game.h"
#include "Data.h"
#include "shape.h"	//�}�`�̏���


//-�O���[�o���֐�-
CHARA_DATA enemy[ENEMY_MAX];	//�G�f�[�^

SCORE_DATA score_data;
SCORE_DATA score_dataInit =		//�X�R�A�f�[�^
{ SCORE_DATA_PATH,"NAME1",-1,"NAME2",-1,"NAME3",-1 };//�X�R�A�f�[�^�������p

//MAP_DATA map1;			//�}�b�v1


//-�֐�-

/// <summary>
/// CSV�t�@�C����ǂݍ���
/// </summary>
/// <param name="path">CSV�t�@�C���̃p�X</param>
/// <param name="chara">�`�����f�[�^�\���̂̐擪�A�h���X</param>
/// <param name="DataMax">CSV�t�@�C���̍ő吔</param>
/// <param name="IsHeader">CSV�t�@�C���̈�s�ڂ��w�b�_�Ȃ�TRUE</param>
/// <returns>TRUE�͓ǂݍ��߂�/FALSE�͎��s����</returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader = FALSE)
{
	FILE* fp;				//�t�@�C���|�C���^
	char GetChar = '\0';	//�擾���镶��
	int LoopCnt = 0;		//���[�v�J�E���^
	errno_t FileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	int IsEOF = NULL;		//�t�@�C���̏I��肩�`�F�b�N

	//�t�@�C����ǂݍ��݃��[�h(r)�ŊJ��
	FileErr = fopen_s(&fp, path, "r");
	if (FileErr != NULL)
	{
		//�ǂݍ��݃G���[
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}


	//����ɓǂݍ��߂��牺��

	//CSV�̈�s�ڂ��w�b�_�̎�
	if (IsHeader == TRUE)
	{
		//�ꕶ�������o���A���s���q���g�Ɉ�s�ڂ�ǂݔ�΂�
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//���ۂɃf�[�^���i�[����
	while (IsEOF != EOF)	//End Of File(�t�@�C���̍Ō�)�łȂ��Ƃ��J��Ԃ�
	{
		//CSV�̃f�[�^�̈�s����ǂݎ��
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,
			&(chara + LoopCnt)->HP,
			&(chara + LoopCnt)->ATK,
			&(chara + LoopCnt)->DEF,
			&(chara + LoopCnt)->MAX_HP,
			&(chara + LoopCnt)->MAX_ATK,
			&(chara + LoopCnt)->MAX_DEF
			);

		//���̃f�[�^��
		LoopCnt++;
	}
	
	//�f�[�^�̐����Ⴄ�Ƃ�
	if (LoopCnt - 1 != DataMax)
	{
		//�ǂݍ��݃G���[
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);
		return FALSE;
	}

	fclose(fp);		//�t�@�C�������

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^��ǂݍ���
/// </summary>
/// <param name="path">�X�R�A�f�[�^�̃p�X</param>
/// <param name="score">�X�R�A�f�[�^�\���̂̐擪�A�h���X</param>
/// <param name="IsHeader">CSV�t�@�C���̈�s�ڂ��w�b�_�Ȃ�TRUE</param>
/// <returns>�ǂݍ��߂��Ȃ�TRUE</returns>
BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader = FALSE)
{
	FILE* fp;					//�t�@�C���|�C���^
	char GetChar = '\0';		//�擾���镶��
	errno_t FileErr = NULL;		//�t�@�C���ǂݍ��݃G���[���m�F
	int IsEOF = NULL;			//�t�@�C���̏I��肩�`�F�b�N

	//�p�X�̃R�s�[
	SetScoreDataPath(path);

	FileErr = fopen_s(&fp, path, "r");	//�t�@�C����ǂݍ��݃��[�h�ŊJ��
	if (FileErr != NULL)
	{
		//�X�R�A�f�[�^�쐬
		if (CreateScoreData() == FALSE) { return FALSE; }

		//�X�R�A�X�e�[�^�X�쐬����Ă���͂�

		FileErr = fopen_s(&fp, path, "r");
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), path, SCORE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
	}

	//-����ɓǂݍ��߂��Ȃ�-

	//CSV�̈�s�ڂ��w�b�_�̎�
	if (IsHeader == TRUE)
	{
		//1���������o���A��s�ڂ�ǂݔ�΂�
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//�f�[�^���i�[����
	while (IsEOF != EOF)	//End Of File �łȂ��Ƃ��J��Ԃ�
	{
		//CSV�̃f�[�^��1�s���ǂݎ��
		IsEOF = fscanf_s(
			fp,
			SCORE_DATA_FORMAT,	//�f�[�^���u1�s=1��v�ŏ�����Ă���
			//�������ǂݍ��ނƂ��́A���̈����ŕ�����ő吔���w�肷��
			&(score)->Name1, STR_MAX,
			&(score)->Score1,
			&(score)->Name2, STR_MAX,
			&(score)->Score2,
			&(score)->Name3, STR_MAX,
			&(score)->Score3
		);
	}

	fclose(fp);	//�t�@�C�������

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�쐬
/// </summary>
BOOL CreateScoreData(VOID)
{
	//�Ȃ�������X�R�A�f�[�^�����
	FILE* crefp;
	errno_t creFileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//�t�@�C�����������݃��[�h�ŊJ��
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�w�b�_����������
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//����̃X�R�A�f�[�^����������
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(crefp);	//�X�R�A�f�[�^�����

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�ۑ�
/// </summary>
BOOL SaveScoreData(VOID)
{
	//�Ȃ�������X�R�A�f�[�^�����
	FILE* crefp;
	errno_t creFileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//�t�@�C�����������݃��[�h�ŊJ��
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�w�b�_����������
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//�X�R�A�f�[�^����������
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_data.Name1, score_data.Score1,
		score_data.Name2, score_data.Score2,
		score_data.Name3, score_data.Score3
	);

	fclose(crefp);	//�X�R�A�f�[�^�����

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�̍폜
/// </summary>
/// <param name="path">�X�R�A�f�[�^�̃p�X</param>
BOOL DeleteScoreData(VOID)
{
	//�t�@�C���폜
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//�X�R�A�f�[�^���쐬
	return CreateScoreData();
}

/// <summary>
/// �X�R�A�f�[�^�̃p�X��Ԃ�
/// </summary>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// �X�R�A�f�[�^�̃p�X��ݒ肷��
/// </summary>
VOID SetScoreDataPath(const char* path)
{
	strcpyDx(score_data.path, path);
	return;
}

/// <summary>
/// CSV�t�@�C����ǂݍ���
/// </summary>
/// <param name="ImgPath">�}�b�v�`�b�v�̉摜�t�@�C���̃p�X</param>
/// <param name="shitaPath">���t�@�C���̃p�X</param>
/// <param name="nakaPath">���t�@�C���̃p�X</param>
/// <param name="kaguPath">�Ƌ�t�@�C���̃p�X</param>
/// <param name="nakaatariPath">�������蔻��t�@�C���̃p�X</param>
/// <param name="uePath">��t�@�C���̃p�X</param>
/// <param name="map">�}�b�v�f�[�^�\���̂̐擪�A�h���X</param>
/// <param name="divYoko">���̕�����</param>
/// <param name="divTate">�c�̕�����</param>
/// <returns>TRUE or FALSE</returns>
BOOL LoadCSVMap(
	const char* ImgPath,
	const char* shitaPath,
	const char* nakaPath,
	const char* kaguPath,
	const char* nakaatariPath,
	const char* uePath,
	MAP_DATA* map,
	int divYoko,
	int divTate)
{
	//�}�b�v�`�b�v��ǂݍ���
	{
		int IsLoad = -1;	//�摜��ǂݍ��߂���

		//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
		int TempHandle = LoadGraph(ImgPath);

		//�ǂݍ��݃G���[
		if (TempHandle == -1)
		{
			MessageBox
			(
				GetMainWindowHandle(),		//�E�B���h�E�n���h��
				ImgPath,					//�{��
				"�摜�ǂݍ��݃G���[",		//�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;	//�ǂݍ��ݎ��s
		}

		//�摜�̕��ƍ������擾
		int width = -1;		//��
		int height = -1;	//����
		GetGraphSize(TempHandle, &width, &height);

		//�������ēǂݍ���
		IsLoad = LoadDivGraph
		(
			ImgPath,							//�摜�̃p�X
			divYoko * divTate,					//��������
			divYoko, divTate,					//���̕����A�c�̕���
			width / divYoko, height / divTate,	//�摜����̕��A����
			map->handle							//�A���ŊǗ�����z��̐擪�A�h���X
		);

		if (IsLoad == -1)
		{
			MessageBox
			(
				GetMainWindowHandle(),		//�E�B���h�E�n���h��
				ImgPath,					//�{��
				"�摜�����G���[",			//�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;	//�ǂݍ��ݎ��s
		}

		//����ݒ�
		map->divMax = divYoko * divTate;
		GetGraphSize(map->handle[0], &map->width, &map->height);

		//�摜��`�悷��
		map->IsDraw = TRUE;

		//�ꎞ�I�ɓǂݍ��񂾃n���h�������
		DeleteGraph(TempHandle);
	}

	FILE* fp;					//�t�@�C���|�C���^
	char GetChar = '\0';		//�擾���镶��
	int LoopCnt = 0;			//���[�v�J�E���^
	errno_t FileErr = NULL;		//�t�@�C���ǂݍ��݃G���[���m�F
	int IsEOF = NULL;			//�t�@�C���̏I��肩�`�F�b�N

	//��CSV
	{
		FileErr = fopen_s(&fp, shitaPath, "r");		//�t�@�C����ǂݍ��݃��[�h�ŊJ��
		if (FileErr != NULL)
		{
			//�ǂݍ��݃G���[
			MessageBox
			(
				GetMainWindowHandle(),
				shitaPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//����ɓǂݍ��߂��Ȃ�

		//�f�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[����
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
			//[3][3]�̔z��̎��ALoopCnt��5�Ȃ�
			//[5/3=1][5%3=2]�ƂȂ�A1�����z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��XX,�ŏ�����Ă���
				&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}
		fclose(fp);	//�t�@�C�������
	}

	//��CSV
	{
		FileErr = fopen_s(&fp, nakaPath, "r");		//�t�@�C����ǂݍ��݃��[�h�ŊJ��
		if (FileErr != NULL)
		{
			//�ǂݍ��݃G���[
			MessageBox
			(
				GetMainWindowHandle(),
				nakaPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//����ɓǂݍ��߂��Ȃ�

		//�f�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[����
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
			//[3][3]�̔z��̎��ALoopCnt��5�Ȃ�
			//[5/3=1][5%3=2]�ƂȂ�A1�����z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��XX,�ŏ�����Ă���
				&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}
		fclose(fp);	//�t�@�C�������
	}

	//�Ƌ��CSV
	{
		FileErr = fopen_s(&fp, kaguPath, "r");		//�t�@�C����ǂݍ��݃��[�h�ŊJ��
		if (FileErr != NULL)
		{
			//�ǂݍ��݃G���[
			MessageBox
			(
				GetMainWindowHandle(),
				kaguPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//����ɓǂݍ��߂��Ȃ�

		//�f�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[����
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
			//[3][3]�̔z��̎��ALoopCnt��5�Ȃ�
			//[5/3=1][5%3=2]�ƂȂ�A1�����z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��XX,�ŏ�����Ă���
				&map->CSV_kagu[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}
		fclose(fp);	//�t�@�C�������
	}

	//�������蔻��CSV��ǂݎ��
	{
		FileErr = fopen_s(&fp, nakaatariPath, "r");		//�t�@�C����ǂݍ��݃��[�h�ŊJ��
		if (FileErr != NULL)
		{
			//�ǂݍ��݃G���[
			MessageBox
			(
				GetMainWindowHandle(),
				nakaatariPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//����ɓǂݍ��߂��Ȃ�

		//�f�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[����
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
			//[3][3]�̔z��̎��ALoopCnt��5�Ȃ�
			//[5/3=1][5%3=2]�ƂȂ�A1�����z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��XX,�ŏ�����Ă���
				&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}
		fclose(fp);	//�t�@�C�������
	}

	//��CSV
	{
		FileErr = fopen_s(&fp, uePath, "r");		//�t�@�C����ǂݍ��݃��[�h�ŊJ��
		if (FileErr != NULL)
		{
			//�ǂݍ��݃G���[
			MessageBox
			(
				GetMainWindowHandle(),
				uePath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//����ɓǂݍ��߂��Ȃ�

		//�f�[�^���i�[����
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//�t�@�C�����琔�l����ǂݍ��݁A�z��Ɋi�[����
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]��
			//[3][3]�̔z��̎��ALoopCnt��5�Ȃ�
			//[5/3=1][5%3=2]�ƂȂ�A1�����z��̈ʒu��񎟌��z��Ƃ��ĕϊ��ł���
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//�f�[�^��XX,�ŏ�����Ă���
				&map->CSV_ue[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//���̃f�[�^��
			LoopCnt++;
		}
		fclose(fp);	//�t�@�C�������
	}

	//�}�b�v�̓����蔻����쐬
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//�ʂ�Ȃ�ID�Ȃ�
				if (map->CSV_naka_atari[tate][yoko] == MAP_STOP_ID)
				{
					//�����蔻����쐬
					map->coll[tate][yoko].left = (yoko + 0) * map->width + 1;
					map->coll[tate][yoko].right = (yoko + 1) * map->width - 1;
					map->coll[tate][yoko].top = (tate + 0) * map->height + 1;
					map->coll[tate][yoko].bottom = (tate + 1) * map->height - 1;
				}
				else
				{
					map->coll[tate][yoko].left = 0;
					map->coll[tate][yoko].right = 0;
					map->coll[tate][yoko].top = 0;
					map->coll[tate][yoko].bottom = 0;
				}
				
				//�}�b�v�̏ꏊ���w��
				map->x[tate][yoko] = (yoko + 0) * map->width;
				map->y[tate][yoko] = (tate + 0) * map->height;
			}
		}
	}

	return TRUE;
}

/// <summary>
/// �}�b�v�����蔻��
/// </summary>
/// <returns>����������TRUE or ������Ȃ�������FALSE</returns>
BOOL CollMap(RECT rect, MAP_DATA map)
{
	//�����蔻��`�F�b�N
	for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
		{
			if (CheckCollRectToRect(rect, map.coll[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

/// <summary>
/// �}�b�v�`��
/// </summary>
/// <param name="map">�}�b�v�\����</param>
VOID DrawMap(MAP_DATA map)
{
	//�摜���`��ł���Ƃ���
	if (map.IsDraw == TRUE)
	{
		//�����C���[
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_shita[tate][yoko]], TRUE);
			}
		}

		//�����C���[
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_naka[tate][yoko]], TRUE);
			}
		}

		//�Ƌ�C���[
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_kagu[tate][yoko]], TRUE);
			}
		}

		//�v���C���[�̉摜��`��
		DrawDivImageChara(&samplePlayerImg);	//�T���v�������摜�̕`��

		//�ヌ�C���[
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_ue[tate][yoko]], TRUE);
			}
		}
	}

	//�f�o�b�O���[�h�̎���
	if (GAME_DEBUG == TRUE)
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//�����蔻��̕`��
				DrawRect(map.coll[tate][yoko], GetColor(255, 255, 255), FALSE);
			}
		}
	}

	return;
}

extern VOID CreateEventMass(int x, int y, EVENT* events, MAP_DATA map)
{
	events->x = x;
	events->y = y;

	events->coll.top = map.height * events->y - 1;
	events->coll.left = map.width * events->x - 1;
	events->coll.bottom = map.height * (events->y + 1) + 1;
	events->coll.right = map.width * (events->x + 1) + 1;

	return;
}

extern VOID CreateEventMultiMass(int x1, int y1, int x2, int y2, EVENT* events, MAP_DATA map)
{
	events->x = x1;
	events->y = y1;

	events->coll.top = map.height * y1 - 1;
	events->coll.left = map.width * x2 - 1;
	events->coll.bottom = map.height * (y2 + 1) + 1;
	events->coll.right = map.width * (x1 + 1) + 1;

	return;
}