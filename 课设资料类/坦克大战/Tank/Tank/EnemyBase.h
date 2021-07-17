#pragma once
#include "TankClass.h"
#include "struct.h"
#include "TimeClock.h"

/************** �л�̹�� ************
* һ���л�ʵ����һ������
* �ṩ�л����(�Ƿ��ǵ���̹��), �л�����[0-3]
* GameControl �ڵл�������, ���Ὣ��� EnemyList ���Ƴ�, ��Ϊ�л��ӵ����ܻ������˶�.��Ϸ�������������ʾ����ͷŵл�������Դ
************************************/
class EnemyBase
{
public:
	EnemyBase(TANK_KIND kind, byte level, BoxMarkStruct*);	// kind[0-1]; level [0-4]
	virtual ~EnemyBase();
	void Init();

	Star_State ShowStar(const HDC& center_hdc, int& total );		// ��ʾ��˸�Ľ���, true-������ʾ, false-��ʾ���
	void TankMoving(const HDC& center_hdc);		// �л��ƶ�
	virtual void DrawTank(const HDC&) {}			// ������̹��, ���า����
	void DrawBullet(const HDC&);			// �����ӵ�, ��Ҫ���ӵ��ƶ��ֿ�,
	bool ShootBullet();			// �����ӵ�
	BulletShootKind BulletMoving( );			// �ӵ��ƶ�
	void Bombing(const HDC&);
	virtual bool BeKill(bool killanyway);			// �л�������, ��� SignBox ���, ������ʾ��һ�õ���ֱ��kill��BigestTank
	bool Blasting(const HDC& );		// ��ʾ̹�˱�ըͼ, true ��ʶ��ը��,GameControl ��ȡ����ֵȻ�󽫸õл�ɾ��

	/*�� GameControl ������, ���� mPause, Ȼ�� ShootBullet() ���ֹͣ�����ӵ�*/
	static void SetPause(bool);

	int GetId();				// ���صл� id
	TANK_KIND GetKind();		// ���صл�����, �Ƿ��ǵ���̹��
	byte GetLevel();

private:
	void SignBox_8(int, int, int value);
	void SignBox_4(int x, int y, int value);		// ��ǻ�ȡ�� 4*4 ��С�ĸ���Ϊ̹��;

	/*
	* ����ӵ�ͷ���ڵ�һ�� 4 * 4 ����
	* �������ӵ�ͼƬ���Ͻ�����
	*/
	void SignBullet(int, int, byte dir, int val);

	bool CheckBox_8();	// ���ĳ�� box_8 �Ƿ���Է���̹��, ������ 16*16 ���ӵ����ĵ�, ��̹�����������ͬ
	bool CheckMoveable();			// 
	void RejustDirPosition();		// ���¶�λ̹�˷���, ����̹��λ��, �����ڸ�����
	BulletShootKind CheckBomb();				// �ƶ��ӵ�
	void ShootWhat(int, int);		// ������к���

	/*��TankMoving()�ڵ���*/
	void ShootBack();		// ��ͷ���

protected:
	int mEnemyId;				// ����л���л�
	TANK_KIND mEnemyTankKind;		// �л����, ����̹�˺���̹ͨ������, [0-1]
	byte mEnemyTankLevel : 2;	// �л�̹��4������ [0-3]
	bool mDied;					// �Ƿ񱻱�����, �����к�����Ϊ true, �л�����ֵ�����ƶ�
	BoxMarkStruct* bms;			// ָ����ӱ�ǽṹ, �� GameControl ���ݽ���

	int mTankX, mTankY;			// ̹������, ̹�˵����ĵ�
	byte mTankDir : 2;			// ̹�˷���
	byte mTankImageIndex : 6;	// ̹���ƶ��л�ͼƬ
	int mStep;					// ��ǰ�����ƶ��Ĳ���, һ����������������ϰ���任�������¼���;
	static int mDevXY[4][2];	// �ĸ������ƫ����
	int mSpeed[4];					// mSpeed * mDevXY �õ��˶��ٶ�, �±��Ӧ mPlayerTankLevel, ��ͬ�����ٶȲ�һ��

	static bool mPause;				// �� GameControl ����, Ȼ���ڷ����ʱ�������ֵ, ����л�����ͣ��ֹͣ�����ӵ�
	//bool mTankNumberReduce;		// ���Ľ��ǿ�ʼ, true-̹��������һ,Ȼ�����ֵ=false, ֻ��һ��

	BulletStruct mBulletStruct;
	int mShootCounter;		// ����һ�������, �������������ӵ�

	BombStruct mBombS;			// ��ը�ṹ��
	BlastStruct mBlast;			// ̹�˱�ը�ṹ
	StarClass mStar;			// �Ľ�����˸��

	TimeClock mTankTimer;		// �л��ƶ��ٶ�
	TimeClock mBulletTimer;		// �ӵ��ƶ��ٶ�
	TimeClock mShootTimer;		// �����ӵ�Ƶ��
	TimeClock mBombTimer;		// �ӵ���ը�ٶ�
	//TimeClock mBlastTimer;		// ̹�˱�ը�ٶ�

	TimeClock mShootBackTimer;	// ̹�˻�ͷ���,Ȼ��̾����ٴα���

	IMAGE mScoreImage[4];			// 100\200.. ��, �л���ը�����ʾ��
};

// ǰ������̹ͨ��
class CommonTank : public EnemyBase
{
public:
	CommonTank(byte level, BoxMarkStruct* bm);
	~CommonTank();
	void DrawTank(const HDC&);				// ������̹��

	TankInfo* mTank;			// ��ɫ̹��
};

// ǰ���ֵ���̹��
class PropTank : public EnemyBase
{
public:
	PropTank(byte level, BoxMarkStruct* bm);
	~PropTank();
	void DrawTank(const HDC&);		// ������̹��

	TankInfo* mTank[2];				// �洢��ɫ�ͺ�ɫ��̹��
	byte index_counter : 6;		// �±�������ɫ
};

// ���������̹�� (�����Ǻ�ƻ�, ��ͨ���̻ƻ�)
class BigestTank : public EnemyBase
{
public:
	BigestTank(TANK_KIND kind, BoxMarkStruct* bm);
	~BigestTank();
	void DrawTank(const HDC&);	// ������̹��
	bool BeKill(bool killanyway);

	TankInfo* mTank[4];			// ��,���,��
	byte index_counter : 6;		// �±�������ɫ
	int hp;				// ����̹����Ҫ�����ĴβŻᱬը
};
