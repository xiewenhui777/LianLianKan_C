#pragma once
#include "TankClass.h"
#include "struct.h"
#include "TimeClock.h"

/************* ��ҿ��� *************
* һ�����ʵ����һ������
* ���Թ����Լ�������
*/

class PlayerBase
{
public:
	PlayerBase(byte player, BoxMarkStruct* /*, PropClass**/);						// player [0-1]
	~PlayerBase();

	void Init();

	/*
	* GameControl ��ѭ������
	*/
	void DrawPlayerTankIco(const HDC& );

	/* ���̹����˸����*/
	bool ShowStar(const HDC& center_hdc);

	/*
	* GameControl ��ѭ������
	* ������̹�˲���
	*/
	void DrawPlayerTank( const HDC& );

	/*�����ӵ�,��Ҫ���ƶ��ֿ�, ��Ȼ��ѭ���ڻ�ʧ֡, ��Ϊ��һ��ÿ�λ�ͼ�����ƶ�*/
	void DrawBullet(const HDC&);

	/*
	* GameControl ��ѭ������
	* ��ⰴ��
	*/
	bool PlayerControl();

	/*
	* GameControl ��ѭ������,
	* mBulletStruct[i].x = SHOOTABLE_X �ӵ����˶�
	*/
	BulletShootKind BulletMoving(const HDC&);

	/*
	* GameControl ��ѭ������
	* �������Ƿ���ʾ��ըЧ���� ����ӵ����� �ĺ�������
	*/
	void Bombing(const HDC&);

	/*
	* GameControl �ڵ���, ͨ��������ȡ�����еĵл� id
	* a : 1 ���ӵ����ел��� id
	* b : 2 ���ӵ����ел��� id
	*/
	void GetKillEnemy(int& a, int& b);

	/*
	* GameControl ��ѭ������ֵ
	* �������Ƿ���д�Ӫ
	* �����ڵĳ�Ա mIsShootCamp �ڼ���ӵ����еĺ����и�ֵ
	*/
	//bool IsShootCamp();

	/*
	* �� GameControl �ڼ�����
	* GameControl ��ѭ������ EnemyBase->IsShootToPlayer() ��ȡ��������ҵ� id
	* Ȼ����ö�Ӧ��ҵ� BeKill ����
	* BeKill() ���Ʊ�ըͼ��ʾ
	*/
	void BeKill();

	/*
	* GameControl ��ѭ������
	* BeKill �����ú�����һ�� flag = true,
	* �ú��������� flag Ȼ����ʾ��ըͼ
	* ���� true ��������ù�
	*/
	bool Blasting(const HDC&);

	const int& GetID();

	/*GameControl �ڵ���, �������Ƿ���ʱ�����*/
	static bool IsGetTimeProp();

	// ���ӵ���ѵ��
	static bool IsGetShvelProp();
	bool IsGetBombProp();

	/*�� gameControl ��ѭ������ ��Ⲣ��ʾ��˸����*/
	static void ShowProp(const HDC&);

	/*GameCnotrol ��ѭ������, �����ڵ��ó�ԱScorePanel ������ʾ���*/
	bool ShowScorePanel(const HDC&);

	/*GameControl �ڼ���ӵ����н���жϵ���,
	���������ͣ�ƶ�*/
	void SetPause();

	/*
	* GameControl �� CheckKillEnemy() �ڵ���
	* ����ɹ�������ߵл�, ����ʾ����
	*/
	static void SetShowProp();

	/*GameControl ��CheckKillEnemy() �ڵ���,
	��¼�������ͬ�������͵ĵл�������*/
	void AddKillEnemyNum(byte enemy_level);

	/*GameControl ��ÿһ�ؽ�����ʾ��������ʱ�����*/
	void ResetScorePanelData(const int& player_num, const int& stage);

	/*ʤ��������һ��ʱ���, ��������Ҳ����³�ʼ��, ��һ�ز������*/
	bool IsLifeEnd();

	/*GameCOntrol ��ѭ������, �ж��������ֵ, ��ʾGAMEOVER ����*/
	void CheckShowGameOver(const HDC& );

	/*GameControl ����� blasting ���������*/
	void SetShowGameover();

private:

	/*
	* ����ӵ�ͷ���ڵ�һ�� 4*4 ����
	* �������ӵ�ͼƬ���Ͻ�����
	*/
	void SignBullet(int, int, byte dir, int val);

	/*������������³���*/
	void Reborn();

	/*��ȡ������, ̹���ƶ��ڼ�����*/
	void DispatchProp(int prop_kind);
	/*
	* PlayerControl() �ڵ���
	* �жϲ����ķ�������ƶ�
	* ͬʱ�������굽����������, 
	*/
	void Move(int new_dir);	

	/*
	* Move() �ڵ���
	* ��⵱ǰ�����ƶ��Ŀ�����
	*/
	bool CheckMoveable();

	/*
	* PlayerControl() �ڵ���
	* ���� id ���ӵ�[0,1]
	* ���� J �� 1 ������
	*/
	bool ShootBullet(int bullet_id);

	/*
	* BulletMoving() �ڵ���
	* ����ӵ��ƶ������Ƿ���ж���
	*/
	BulletShootKind CheckBomb(int);

	/*
	* CheckBomb() �ڻ����ϰ���ʱ����
	* id  : �ӵ� id
	* x,y : �ӵ�͹���������, ���ݲ�ͬ����λ�ò�һ��
	*/
	void ClearWallOrStone(int id, int x, int y);

	/*
	* ��� 4 �� 8*8 �ĸ���
	* x,y : 16*16 ���ӵ����ĵ�����
	* val : ��Ҫ��ǵ�ֵ
	*/
	void SignBox_8(int x, int y, int val);

	// ����̹����������, ���16�� 4*4 ����
	void SignBox_4(int, int, int);

	/*
	* ������ [26][26] ���±�����
	*/
	static bool CheckBox_8(int iy, int jx);

	/*�ӵ������ӵ�ʱ�����*/
	//void DisappearBullet(int sign);

	/**/
	static void ProtectCamp(int val);

public:
	static list<PlayerBase*>* mPList;		// ��GameControl�� ��ֵ

private:
	bool mDied;								// �����Ƿ�����,����
	byte player_id : 1;						// [0-1] ���
	PlayerTank* mPlayerTank;				// ̹����
	static BoxMarkStruct* bms;

	IMAGE m12PImage;						// 1P\2P ͼ��
	int m12PImage_x, m12PImage_y;			// ͼ������

	IMAGE mPlayerTankIcoImage;				// ���̹��ͼ��
	int   mPlayerTankIcoImage_x,			// ͼ������
		  mPlayerTankIcoImage_y;

	int mPlayerLife;						// �������, Ĭ��3
	int mPlayerLife_x, 						// ͼ������
		mPlayerLife_y;

	IMAGE mBlackNumberImage;				// 0123456789 ��ɫ����
	int mTankX, mTankY;					// ̹�����ĵ�����
	byte mPlayerTankLevel : 2;				// [0-3] ̹�˼���,��õ��ߺ�����̹��
	byte mTankDir : 2;						// ��ǰ̹�˷���0-��,1-��,2��,3-��
	static int mDevXY[4][2];				// �ĸ������ƫ����
	int mSpeed[4];							// mSpeed * mDevXY �õ��˶��ٶ�, �±��Ӧ mPlayerTankLevel, ��ͬ�����ٶȲ�һ��

	BulletStruct mBulletStruct[2];			// �����ӵ�
	int mBullet_1_counter;					// �ӵ� 1 �ļ���, �ӵ� 1 �����ú���ܷ����ӵ� 2
	bool mMoving;							// ָʾ̹���Ƿ��ƶ�, ���ݵ� GetTankImage() ��ȡ�ƶ���̹��

	BombStruct mBombS[2];			// ��ը�ṹ��
	BlastStruct mBlast;				// ̹�˱�ը�ṹ, 

	StarClass mStar;				// ̹�˳���ǰ���Ľ�����˸
	RingClass mRing;				// ����Ȧ��

	static PropClass mProp;		// ������

	/*GameControl ��ѭ������ֵ, Ȼ�������� EnemyBase ֹͣ�ƶ�*/
	static bool mTimeProp;			// ��¼�Ƿ��� ʱ�ӵ���
	bool mBombProp;					// ���׵���, �߼��� mTimeProp ��ͬ
	static bool mShovelProp;				// ����Ƿ�ӵ�в��ӵ���
	static int mShovelProp_counter;	// 

	TimeClock mTankTimer;			// ̹���ƶ���ʱ��
	TimeClock mBulletTimer;			// �ӵ��ƶ��ٶ�
	TimeClock mBombTimer;	//bug?		// �ӵ���ը�ٶ�
	//TimeClock mBlastTimer;			// ̹�˱�ը�ٶ�

	ScorePanel* mScorePanel;		// �ؿ���������ʾ�������

	int mKillEnemyNumber[4];		// ���ֵл�����,ɱ������

	bool mPause;			// ������һ����һ���, ��ͣһ��ʱ��
	int mPauseCounter;		// Move() �ڼ���ֹͣ�ƶ�, DrawTank() ��ȡģ����,ʵ��̹����˸

	bool mHasSendKillNumToScorePanel;		// ��ֹ ʤ�����д�Ӫ������.��η������ݵ��������

	/*
	* CheckMoveable() �ڼ��
	* ̹���Ƿ��� _ICE ��
	* ���� _ICE ��ʱ��������Ϊ true, ���ݸ� flag ���� mTankTimer ʱ���, �ӿ�̹���ƶ�
	* ���������� false, ����ԭ��ʱ���
	*/
	bool mOnIce;
	bool mAutoMove;
	int mAutoMove_Counter;
	int mRandCounter;			// ÿ���Զ��ƶ��������

	PlayerGameover mPlayerGameover;		// ��ұ�������ʧ gameover ����

	/*TimeClock mGameOverTimer;		//
	int mGameOver_Dev;			// X ���ƶ�, ���һ����������, ��Ҷ��Ǹ�������
	int mGameOverX, mGameOverY;	
	int mGameOver_end_x;			// ͼƬֹͣ�� x ����
	static IMAGE mGameOverImage;		// �������ֵ����, ��ʾһ��ˮƽ�ƶ��� GAMEOVER ����
	int mGameOverCounter;			// ͼƬֹͣ������ʧ
	bool mShowGameOver;*/

	static int mMoveSpeedDev[4];	// �ĸ�����̹���ƶ�ʱ����
	static int mBulletSpeedDev[4];	// ��ͬ�����ӵ�ʱ�����ٶ�
};