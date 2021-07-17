#include "stdafx.h"
#include "EnemyBase.h"
#include "MciSound.h"

/////////////////////////////////////////////////////////
// �л�̹�˿���

bool EnemyBase::mPause = false;
int EnemyBase::mDevXY[4][2] = { { -1, 0 },{ 0, -1 },{ 1, 0 },{ 0, 1 } };	// ������������

// ����ĳ����𼶱�ĵл�
EnemyBase::EnemyBase(TANK_KIND kind, byte level, BoxMarkStruct* b)
{
	mEnemyTankKind = kind;
	mEnemyTankLevel = level;
	mDied = false;
	bms = b;

	int tempx[3] = {BOX_SIZE, 13 * BOX_SIZE, 25 * BOX_SIZE};	// ̹��������ֵ�����λ�� x ����
	mTankX = tempx[rand() % 3];
	mTankY = BOX_SIZE;
	mTankDir = DIR_DOWN;
	mTankImageIndex = 0;

	mStep = rand() % 200;					// ��ǰ��������ƶ��Ĳ���

	// ��ͬ����̹���ƶ��ٶ�ϵ��
	int temp[4] = { 1, 1, 1, 1 };
	for (int i = 0; i < 4; i++)
		mSpeed[i] = temp[i];

	//mTankNumberReduce = true;				// ̹��������һ��־

	// �ӵ���ʼ��
	// .. ���������� PlayerBase ������
	mBulletStruct.x = SHOOTABLE_X;
	mBulletStruct.y = -100;
	mBulletStruct.dir = DIR_DOWN;
	for (int i = 0; i < 4; i++)
		mBulletStruct.speed[i] = 3;		// ���ܳ��� 4
	mBulletStruct.mKillId = 0;			// ��¼�������̹�˵�id

	mShootCounter = rand() % 100 + 100;	// �����������ӵ�
	
	// ��ըͼƬ
	mBombS.mBombX = -100;
	mBombS.mBombY = -100;
	mBombS.canBomb = false;
	mBombS.counter = 0;

	// ��ͬ����л��ƶ�ʱ����
	int movelevel[4] = { 30, 15, 27, 25};
	mTankTimer.SetDrtTime(movelevel[mEnemyTankLevel]);

	// �ӵ��ƶ�ʱ����
	mBulletTimer.SetDrtTime(30);

	// �����ӵ�Ƶ��
	mShootTimer.SetDrtTime( rand() % 1000 + 700 );

	// �ӵ���ը�ٶ�
	mBombTimer.SetDrtTime(37);

	// ̹�˱�ը����
	//mBlastTimer.SetDrtTime(37);

	// ���û�ͷ���Ƶ��
	mShootBackTimer.SetDrtTime( rand()%5000 + 9000 );

	loadimage(&mScoreImage[0], _T("./res/big/100.gif"));
	loadimage(&mScoreImage[1], _T("./res/big/200.gif"));
	loadimage(&mScoreImage[2], _T("./res/big/300.gif"));
	loadimage(&mScoreImage[3], _T("./res/big/400.gif"));
}

EnemyBase::~EnemyBase()
{
	//if (mEnemyTank != NULL)
		//delete mEnemyTank;
}

void EnemyBase::Init()
{
}

// ��ʾ̹��
Star_State EnemyBase::ShowStar(const HDC& center_hdc, int& remainnumber)
{
	// ̹���Ѿ�����,������˸,ֱ�ӷ���
	/*if (mStar.IsStop() == true)
		return Star_State::Tank_Out;

	// һ��ʱ������ʾ�Ľ���, ֮ǰ����
	if (mStar.mTankOutAfterCounter-- > 0)
		return Star_State::Star_Showing;

	// ���̹�˳��ֵ�λ�ñ�������, �ȴ���һ�����ѭ���ڳ���
	if (CheckBox_8() == false)
	{
		// ����ѡ�����λ��
		int tempx[3] = { BOX_SIZE, 13 * BOX_SIZE, 25 * BOX_SIZE };
		mTankX = tempx[rand() % 3];

		mStar.mTankOutAfterCounter = rand() % 100 + 10;
		return Star_State::Star_Showing;
	}*/


	Star_State result = mStar.EnemyShowStar(center_hdc, mTankX, mTankY, bms);
	switch (result)
	{
		// ��ǰ���ڼ�ʱ, δ��ʾ
		case Star_State::Star_Timing:
			break;

		// ��ǰλ����ʾ�Ľ���ʧ��
		case Star_State::Star_Failed:
			{
				// ����ѡ�����λ��
				int tempx[3] = { BOX_SIZE, 13 * BOX_SIZE, 25 * BOX_SIZE };
				mTankX = tempx[rand() % 3];
			}
			break;

		// �Ľ��ǿ�ʼ����
		case Star_State::Star_Out:
			SignBox_4(mTankX, mTankY, STAR_SIGN);	// ���Ϊ STAR_SIGN = 2000, 2000 ����̹�˲��ܴ��еı�־
			break;

		// �Ľ������ڳ���
		case Star_State::Star_Showing:
			break;

		// �Ľ���ֹͣ
		case Star_State::Star_Stop:
			// �Ľ�����ʧ. �л�����, ʣ��̹����-1;
			mEnemyId = TOTAL_ENEMY_NUMBER - remainnumber;
			remainnumber -= 1;

			SignBox_4(mTankX, mTankY, ENEMY_SIGN + 1000 * mEnemyTankLevel + 100 * mEnemyTankKind + mEnemyId);		// ̹�˳���, ���Ľ��Ǳ�Ǹ�Ϊ̹�˱��
			break;

		// ̹���Ѿ�����
		case Star_State::Tank_Out:
			break;
	}
	return result;
}

void EnemyBase::TankMoving(const HDC& center_hdc)
{
	if (!mStar.IsStop() || mDied || mTankTimer.IsTimeOut() == false )
		return;
	
	// �ƶ�ǰȡ�����
	SignBox_4(mTankX, mTankY, _EMPTY);

	// �ڲ���ʱ, һ��ʱ����ͷ���
	ShootBack();

	// �ض���
	if (mStep-- < 0)
		RejustDirPosition();
	
	// ���ƶ�
	if (CheckMoveable())
	{
		mTankX += mDevXY[mTankDir][0] * mSpeed[mEnemyTankLevel];
		mTankY += mDevXY[mTankDir][1] * mSpeed[mEnemyTankLevel];
	}

	// �����ƶ�,�ض���
	else
	{
		RejustDirPosition();
	}

	// ����λ�����±��
	SignBox_4(mTankX, mTankY, ENEMY_SIGN + mEnemyTankLevel * 1000 + mEnemyTankKind * 100 + mEnemyId);
}

// 
void EnemyBase::DrawBullet(const HDC& center_hdc)
{
	// ����ӵ�û���ƶ����ߵл�����
	if (mBulletStruct.x == SHOOTABLE_X /*|| mDied*/)
		return;
	int dir = mBulletStruct.dir;

	TransparentBlt(center_hdc, mBulletStruct.x, mBulletStruct.y, BulletStruct::mBulletSize[dir][0],
		BulletStruct::mBulletSize[dir][1], GetImageHDC(&BulletStruct::mBulletImage[dir]),
		0, 0, BulletStruct::mBulletSize[dir][0], BulletStruct::mBulletSize[dir][1], 0x000000);
}

//
bool EnemyBase::ShootBullet()
{
	if ( mPause || mBulletStruct.x != SHOOTABLE_X || mShootTimer.IsTimeOut() == false || mDied || mStar.IsStop() == false )
		return false;

	// �ӵ����������
	mBulletStruct.x = mTankX + BulletStruct::devto_tank[mTankDir][0];
	mBulletStruct.y = mTankY + BulletStruct::devto_tank[mTankDir][1];
	mBulletStruct.dir = mTankDir;

	SignBullet(mBulletStruct.x, mBulletStruct.y, mBulletStruct.dir, E_B_SIGN);
	return true;
}

//
BulletShootKind EnemyBase::BulletMoving()
{
	// ����ӵ�û���ƶ����ߵл�����
	if (mBulletStruct.x == SHOOTABLE_X/* || mDied*/ || !mBulletTimer.IsTimeOut() )
		return BulletShootKind::None;
	
	// ����ӵ��ڱ�ը
	BulletShootKind result = CheckBomb();
	switch (result)
	{
	case BulletShootKind::Camp:
	case BulletShootKind::Other:
	case BulletShootKind::Player_1:
	case BulletShootKind::Player_2:
		return result;

	case BulletShootKind::None:
		break;
	default:
		break;
	}

	int dir = mBulletStruct.dir;
	SignBullet(mBulletStruct.x, mBulletStruct.y, dir, _EMPTY );

	mBulletStruct.x += mDevXY[dir][0] * mBulletStruct.speed[mEnemyTankLevel];
	mBulletStruct.y += mDevXY[dir][1] * mBulletStruct.speed[mEnemyTankLevel];

	SignBullet(mBulletStruct.x, mBulletStruct.y, dir, E_B_SIGN );
	return BulletShootKind::None;
}

//
void EnemyBase::Bombing(const HDC & center_hdc)
{
	int index[3] = { 0,1,2 };
	if (mBombS.canBomb)
	{
		TransparentBlt(center_hdc, mBombS.mBombX - BOX_SIZE, mBombS.mBombY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
			GetImageHDC(&BombStruct::mBombImage[index[mBombS.counter % 3]]), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
		if (mBombTimer.IsTimeOut())
		{
			if (mBombS.counter++ >= 3)
			{
				mBombS.counter = 0;
				mBombS.canBomb = false;
			}
		}
	}
}

/* ��Gamecontrol�ڼ��, Ȼ�����
* BigestTank.class ��Ҫ�����������, 
* ��Ϊ����Ҫ����Ĵβ���ɱ��
*/
bool EnemyBase::BeKill(bool killanyway)
{
	// ����л���û�г���
	if (mStar.IsStop() == false || mBlast.IsBlasting() || mDied == true)
		return false;

	MciSound::_PlaySound(S_ENEMY_BOMB);
	mDied = true;
	SignBox_4(mTankX, mTankY, _EMPTY);

	// ���ÿ�ʼ��ը����
	mBlast.SetBlasting(mTankX, mTankY);

	return true;
}

// ��ʾ̹�˱�ըЧ��, GameControl ��ѭ�����
bool EnemyBase::Blasting(const HDC& center_hdc)
{
	switch (mBlast.EnemyBlasting(center_hdc, &mScoreImage[mEnemyTankLevel]))
	{
	case BlastState::NotBlast:
		break;

	case BlastState::Blasting:
		break;

	case BlastState::BlastEnd:
		return true;

	default:
		break;
	}
	return false;
}

//.bool EnemyBase::IsShootCamp()
//.{
	//.return mIsShootCamp;
//.}

//
void EnemyBase::SetPause(bool val)
{
	mPause = val;
}
/*.
// ���ر�������� id ���� 0 û�л���
int EnemyBase::IsShootToPlayer()
{
	int temp = mShootedPlayerID;
	mShootedPlayerID = -1;			// ��ȡ���NONE! ��Ȼһֱ����ǻ��и����, ���������0,1 ��ֵ��ͬ!!
	return temp;
}
*/
int EnemyBase::GetId()
{
	return mEnemyId;
}

TANK_KIND EnemyBase::GetKind()
{
	return mEnemyTankKind;
}

byte EnemyBase::GetLevel()
{
	return mEnemyTankLevel;
}

//----------------- ˽�к��� ------------------------
/*
void EnemyBase::SetPause(bool val)
{
	mIsPause = val;
}*/

// x,y �� 16*16 ���ĵ�����
void EnemyBase::SignBox_8(int x, int y, int value)
{
	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = y / BOX_SIZE - 1;
	int jx = x / BOX_SIZE - 1;
	for (int i = iy; i < iy + 2; i++)
	{
		for (int j = jx; j < jx + 2; j++)
		{
			bms->box_8[i][j] = value;
		}
	}
}

// ��ǻ�ȡ��̹�����ڵ� 4*4 = 16 ������
void EnemyBase::SignBox_4(int cx, int cy, int val)
{
	// ���ҵ��� cs,cy ��ռ�ݰٷֱ����� 16 �� 4*4 �ĸ�������
	if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
	{
		if (cx > (cx / SMALL_BOX_SIZE) * SMALL_BOX_SIZE + SMALL_BOX_SIZE / 2)	// ����ǿ����ұ߽ڵ�, 
		{
			cx = (cx / SMALL_BOX_SIZE + 1) * SMALL_BOX_SIZE;
		}
		else {
			cx = (cx / SMALL_BOX_SIZE) * SMALL_BOX_SIZE;					// �����������ϵ���߽ڵ�
		}
	}
	// ����
	else
	{
		if (cy > (cy / SMALL_BOX_SIZE) * SMALL_BOX_SIZE + SMALL_BOX_SIZE / 2)	// ����ǿ��������±߽ڵ�,
			cy = (cy / SMALL_BOX_SIZE + 1) * SMALL_BOX_SIZE;
		else
			cy = (cy / SMALL_BOX_SIZE) * SMALL_BOX_SIZE;					// �����������ϵ��ϱ߽ڵ�
	}

	// ��̹����������ת�����Ͻ��Ǹ��� ��������
	int iy = cy / SMALL_BOX_SIZE - 2;
	int jx = cx / SMALL_BOX_SIZE - 2;
	for (int i = iy; i < iy + 4; i++)
	{
		for (int j = jx; j < jx + 4; j++)
		{
			bms->box_4[i][j] = val;
		}
	}
}

void EnemyBase::SignBullet(int lx, int ty, byte dir, int val)
{
	// ת����ͷ����
	int hx = lx + BulletStruct::devto_head[dir][0];
	int hy = ty + BulletStruct::devto_head[dir][1];

	// ת���� 4*4 �����±�����
	int b4i = hy / SMALL_BOX_SIZE;
	int b4j = hx / SMALL_BOX_SIZE;
	if (b4i > 51 || b4j > 51 || b4i < 0 || b4j < 0)
	{
		//printf("adad��ˮ��%d, %d\n", b4i, b4j);
		return;
	}

	bms->bullet_4[b4i][b4j] = val;
}

// ���ĳ��16*16λ�ÿ��Է�̹����, x,y 16*16�����ĵ�
bool EnemyBase::CheckBox_8()
{
	// ��ȡ̹�����Ͻǵ� 4*4 �±� 
	int iy = mTankY / SMALL_BOX_SIZE - 2;
	int jx = mTankX / SMALL_BOX_SIZE - 2;
	for (int i = iy; i < iy + 4; i++)
	{
		for (int j = jx; j < jx + 4; j++)
		{
			// ����Ľ���, ���,�л�,
			if (bms->box_4[i][j] != STAR_SIGN && bms->box_4[i][j] > _FOREST)
				return false;
		}
	}
	return true;
}

//
bool EnemyBase::CheckMoveable()
{
	// ̹����������
	int tempx = mTankX + mDevXY[mTankDir][0] * mSpeed[mEnemyTankLevel];
	int tempy = mTankY + mDevXY[mTankDir][1] * mSpeed[mEnemyTankLevel];

	if (tempx < BOX_SIZE || tempy < BOX_SIZE || tempy > CENTER_WIDTH - BOX_SIZE || tempx > CENTER_HEIGHT - BOX_SIZE)
	{
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy / BOX_SIZE) * BOX_SIZE;	break;
		default:														break;
		}
		return false;
	}

	// ת�����ص����ڵ� xy[26][26] �±�
	int index_i = (int)tempy / BOX_SIZE;
	int index_j = (int)tempx / BOX_SIZE;

	// ��� 8*8 �ϰ���
	int dev[4][2][2] = { { { -1,-1 },{ 0,-1 } },{ { -1,-1 },{ -1,0 } },{ { -1,1 },{ 0,1 } },{ { 1,-1 },{ 1,0 } } };

	// ���̹�� 4*4 ����
	// �ĸ�������Ҫ�������� 4*4 �ĸ�����̹���������� 4*4 ���ӵ��±�ƫ����
	int  dev_4[4][4][2] = { { { -2,-2 },{ 1,-2 },{ -1,-2 },{ 0,-2 } },{ { -2,-2 },{ -2,1 },{ -2,-1 },{ -2,0 } },
							{ { -2, 2 },{ 1, 2 },{ -1, 2 },{ 0, 2 } },{ { 2, -2 },{ 2, 1 },{ 2, -1 },{ 2, 0 } } };

	// ת���� [52][52] �±�
	int index_4i = tempy / SMALL_BOX_SIZE;
	int index_4j = tempx / SMALL_BOX_SIZE;

	// -1, 0, 1, 2 �������ƶ�
	bool tank1 = bms->box_4[index_4i + dev_4[mTankDir][0][0]][index_4j + dev_4[mTankDir][0][1]] <= _ICE;
	bool tank2 = bms->box_4[index_4i + dev_4[mTankDir][1][0]][index_4j + dev_4[mTankDir][1][1]] <= _ICE;
	bool tank3 = bms->box_4[index_4i + dev_4[mTankDir][2][0]][index_4j + dev_4[mTankDir][2][1]] <= _ICE;
	bool tank4 = bms->box_4[index_4i + dev_4[mTankDir][3][0]][index_4j + dev_4[mTankDir][3][1]] <= _ICE;

	// �����ϰ���
	if (bms->box_8 [index_i + dev[mTankDir][0][0]][index_j + dev[mTankDir][0][1]] > 2 ||
		bms->box_8 [index_i + dev[mTankDir][1][0]][index_j + dev[mTankDir][1][1]] > 2 )
	{
		// ��������ϰ���,��̹�������������������. ��Ȼ̹�˺��ϰ�����м������ص���
		switch (mTankDir)
		{
		case DIR_LEFT:	mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;	break;	// mTankX �� tempx ֮���Խ�˸���, ��̹�˷ŵ�mTankX���ڵĸ�������
		case DIR_UP:	mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_RIGHT: mTankX = (tempx  / BOX_SIZE) * BOX_SIZE;	break;
		case DIR_DOWN:	mTankY = (tempy  / BOX_SIZE) * BOX_SIZE;	break;
		default:													break;
		}
		return false;
	}
	// ����̹�˲��õ���
	else if (!tank1 || !tank2 || !tank3 || !tank4)
		return false;
	return true;
}

//
void EnemyBase::RejustDirPosition()
{
	mStep = rand() % 250;

	// ԭ���ұ����·���
	if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
	{
		if (mTankX > (mTankX / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2 - 1)	// ����ǿ����������ϵ��ұ߽ڵ�, -1������
			mTankX = (mTankX / BOX_SIZE + 1) * BOX_SIZE;
		else
			mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;					// �����������ϵ���߽ڵ�
	}
	// ���±�����
	else
	{
		if (mTankY > (mTankY / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2 - 1)	// ����ǿ����������ϵ��±߽ڵ�, -1������
			mTankY = (mTankY / BOX_SIZE + 1) * BOX_SIZE;
		else
			mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;					// �����������ϵ��ϱ߽ڵ�
	}

	/* �ض���, �������λ�ú�������÷���
	* ����̹�������ƶ��ļ��ʴ�Щ*/
	if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
	{
		bool val = rand() % 100 < 70;
		if (val)
			mTankDir = DIR_DOWN;
		else
			mTankDir = rand() % 4;
	}
	else
		mTankDir = rand() % 4;
}

//
BulletShootKind EnemyBase::CheckBomb()
{
	int dir = mBulletStruct.dir;

	// �ӵ�ͷ�Ӵ����ϰ�����Ǹ���, ���ҷ��������, ���·��������
	int bombx = mBulletStruct.x + BulletStruct::devto_head[dir][0];
	int bomby = mBulletStruct.y + BulletStruct::devto_head[dir][1];

	bool flag = false;
	int adjust_x = 0, adjust_y = 0;		// ��������ͼƬ��ʾ������

	// ������ bombx ���� mBulletStruct[i].x,����Ḳ���ϰ���ļ��
	if (mBulletStruct.x < 0 && dir == DIR_LEFT)
	{
		flag = true;
		adjust_x = 5;					// ����ըͼƬ������һ��
	}
	else if (mBulletStruct.y < 0 && dir == DIR_UP)
	{
		flag = true;
		adjust_y = 5;
	}

	// �����ȥ�ӵ��Ŀ� 4, ��Ȼ�ӵ�Խ��, �����⵼�� box_8 �±�Խ��
	else if (mBulletStruct.x >= CENTER_WIDTH - 4 && dir == DIR_RIGHT)
	{
		flag = true;
		adjust_x = -4;
	}
	else if (mBulletStruct.y >= CENTER_HEIGHT - 4 && dir == DIR_DOWN)
	{
		flag = true;
		adjust_y = -4;
	}
	if (flag)
	{
		// �趨��ը����, ������ը�������ڵĸ���,���һ�����ƫ��һ������֮���..
		mBulletStruct.x = SHOOTABLE_X;
		mBombS.canBomb = true;
		mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[dir][0]) * SMALL_BOX_SIZE;
		mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[dir][1]) * SMALL_BOX_SIZE;
		mBombS.counter = 0;
		return BulletShootKind::Other;
	}

	int tempi, tempj;

	// �������� 8*8 ���ӵ�����
	int b8i = bomby / BOX_SIZE;
	int b8j = bombx / BOX_SIZE;

	// 4*4 ��������
	int bi = bomby / SMALL_BOX_SIZE;
	int bj = bombx / SMALL_BOX_SIZE;

	//if ( bi>51 || bj > 51 || bi < 0 || bj < 0)
	//printf("%d, %d\n", bi, bj);

	// �����������ӵ�
	if (bms->bullet_4[bi][bj] == P_B_SIGN + 0 * 10 + 0 ||
		bms->bullet_4[bi][bj] == P_B_SIGN + 0 * 10 + 1 ||
		bms->bullet_4[bi][bj] == P_B_SIGN + 1 * 10 + 0 ||
		bms->bullet_4[bi][bj] == P_B_SIGN + 1 * 10 + 1 )
	{
		mBulletStruct.x = SHOOTABLE_X;
		bms->bullet_4[bi][bj] = WAIT_UNSIGN;		// �ȱ���м�ֵ, �ȴ������е��ӵ���⵽��ֵ��,�ٲ����ñ��
		return BulletShootKind::Other;
	}
	else if (bms->bullet_4[bi][bj] == WAIT_UNSIGN)
	{
		mBulletStruct.x = SHOOTABLE_X;
		bms->bullet_4[bi][bj] = _EMPTY;
		return BulletShootKind::Other;
	}

	switch (dir)
	{
	// ���Ҽ���ӵ�ͷ���ڵ�4*4���Ӻ����������ڵ��Ǹ�
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		int temp[2][2] = { { 0, 0 },{ -1, 0 } };
		for (int n = 0; n < 2; n++)
		{
			// 8*8 ����, �ж��Ƿ����
			tempi = b8i + temp[n][0];
			tempj = b8j + temp[n][1];
			if (bms->box_8[tempi][tempj] == CAMP_SIGN)
			{
				mBombS.counter = 0;
				mBulletStruct.x = SHOOTABLE_X;
				//.mIsShootCamp = true;
				SignBox_8(13 * BOX_SIZE, 25 * BOX_SIZE, _EMPTY);


				/*int iy = (25 * BOX_SIZE / BOX_SIZE)*2 - 2;
				int jx = (13 * BOX_SIZE / BOX_SIZE)*2 - 2;
				for (int i = iy; i < iy + 4; i++)
				{
					for (int j = jx; j < jx + 4; j++)
					{
						printf("%d, \n", bms->box_4[i][j]);
					}
				}*/

				return BulletShootKind::Camp;
			}

			// 4*4 ���
			tempi = bi + temp[n][0];
			tempj = bj + temp[n][1];
			if (bms->box_4[tempi][tempj] == _WALL || bms->box_4[tempi][tempj] == _STONE )
			{
				// �趨��ը����, ������ը�������ڵĸ���,
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;
				ShootWhat(bombx, bomby);
				return BulletShootKind::Other;
			}
			// 4*4 ��Ҹ���
			else if (bms->box_4[tempi][tempj] == PLAYER_SIGN || bms->box_4[tempi][tempj] == PLAYER_SIGN + 1)
			{
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;
				return (BulletShootKind)bms->box_4[tempi][tempj];
			}
		}
	}
	break;

	// ����ֻ����������ڵ���������
	case DIR_UP:
	case DIR_DOWN:
	{
		int temp[2][2] = { { 0, 0 },{ 0, -1 } };
		for (int n = 0; n < 2; n++)
		{
			// 8*8 ����, �ж��Ƿ������
			tempi = b8i + temp[n][0];
			tempj = b8j + temp[n][1];
			if (bms->box_8[tempi][tempj] == CAMP_SIGN)
			{
				mBombS.counter = 0;
				mBulletStruct.x = SHOOTABLE_X;
				//.mIsShootCamp = true;
				SignBox_8(13 * BOX_SIZE, 25 * BOX_SIZE, _EMPTY);
				return BulletShootKind::Camp;
			}

			// 4*4 ���
			tempi = bi + temp[n][0];
			tempj = bj + temp[n][1];
			if (bms->box_4[tempi][tempj] == _WALL || bms->box_4[tempi][tempj] == _STONE)
			{
				// �趨��ը����, ������ը�������ڵĸ���
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;
				ShootWhat(bombx, bomby);
				return BulletShootKind::Other;
			}
			// 4*4 ���С����
			else if (bms->box_4[tempi][tempj] == PLAYER_SIGN || bms->box_4[tempi][tempj] == PLAYER_SIGN + 1)
			{
				mBulletStruct.x = SHOOTABLE_X;
				mBombS.canBomb = true;				// ָʾ i bomb ��ը
				mBombS.mBombX = (bombx / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][0]) * SMALL_BOX_SIZE;
				mBombS.mBombY = (bomby / SMALL_BOX_SIZE + BulletStruct::bomb_center_dev[mBulletStruct.dir][1]) * SMALL_BOX_SIZE;
				mBombS.counter = 0;
				return (BulletShootKind)bms->box_4[tempi][tempj];
			}
		}
	}
	break;
	default:
		break;
	}
	return BulletShootKind::None;
}

void EnemyBase::ShootWhat(int bulletx, int bullety)
{
	int boxi = bullety / SMALL_BOX_SIZE;
	int boxj = bulletx / SMALL_BOX_SIZE;
	int tempx, tempy;
	switch (mBulletStruct.dir)
	{
	case DIR_LEFT:
	case DIR_RIGHT:
	{
		// ���ڵ��ĸ� 4*4 ����, ˳���ܱ�, �����õ��±��ж�
		int temp[4][2] = { { -2, 0 },{ -1, 0 },{ 0, 0 },{ 1, 0 } };
		for (int i = 0; i < 4; i++)
		{
			tempx = boxi + temp[i][0];
			tempy = boxj + temp[i][1];
			if (bms->box_4[tempx][tempy] == _WALL )
				bms->box_4[tempx][tempy] = _CLEAR;

			// ת�� tempx,tempy���ڵ� 8*8 ��������
			int n = tempx / 2;
			int m = tempy / 2;

			// ��� 8*8 �����ڵ�4�� 4*4 ��С�����Ƿ�ȫ�������,
			bool isClear = true;
			for (int a = 2 * n; a < 2 * n + 2; a++)
			{
				for (int b = 2 * m; b < 2 * m + 2; b++)
				{
					if (bms->box_4[a][b] != _CLEAR)
						isClear = false;
				}
			}
			if (isClear)
			{
				bms->box_8[n][m] = _EMPTY;
			}
		}
	}
	break;

	case DIR_UP:
	case DIR_DOWN:
	{
		// ���ڵ��ĸ� 4*4 ����, ˳���ܱ�, �����õ��±��ж�
		int temp[4][2] = { { 0, -2 },{ 0, -1 },{ 0, 0 },{ 0, 1 } };
		for (int i = 0; i < 4; i++)
		{
			tempx = boxi + temp[i][0];
			tempy = boxj + temp[i][1];
			if (bms->box_4[tempx][tempy] == _WALL )
				bms->box_4[tempx][tempy] = _CLEAR;

			// ת�� tempx,tempy���ڵ� 8*8 ��������
			int n = tempx / 2;
			int m = tempy / 2;

			// ��� 8*8 �����ڵ�4�� 4*4 ��С�����Ƿ�ȫ�������,
			bool isClear = true;
			for (int a = 2 * n; a < 2 * n + 2; a++)
			{
				for (int b = 2 * m; b < 2 * m + 2; b++)
				{
					if (bms->box_4[a][b] != _CLEAR)
						isClear = false;
				}
			}

			// 4 �� 4*4 ��ɵ� 8*8 ���ӱ������
			if (isClear)
			{
				bms->box_8[n][m] = _EMPTY;
			}
		}
	}
	break;

	default:
		break;
	}
}

/*
* ���õл���ͷ�˶���С��һ�ξ���
*/
void EnemyBase::ShootBack()
{
	if (!mShootBackTimer.IsTimeOut())
		return;

	int back_dir[4] = {DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_UP};

	// ԭ���ұ����·���
	if (mTankDir == DIR_LEFT || mTankDir == DIR_RIGHT)
	{
		if (mTankX > (mTankX / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2 - 1)	// ����ǿ����������ϵ��ұ߽ڵ�, -1������
			mTankX = (mTankX / BOX_SIZE + 1) * BOX_SIZE;
		else
			mTankX = (mTankX / BOX_SIZE) * BOX_SIZE;					// �����������ϵ���߽ڵ�
	}
	// ���±�����
	else
	{
		if (mTankY > (mTankY / BOX_SIZE) * BOX_SIZE + BOX_SIZE / 2 - 1)	// ����ǿ����������ϵ��±߽ڵ�, -1������
			mTankY = (mTankY / BOX_SIZE + 1) * BOX_SIZE;
		else
			mTankY = (mTankY / BOX_SIZE) * BOX_SIZE;					// �����������ϵ��ϱ߽ڵ�
	}

	mStep = rand() % 30 + 30;
	mTankDir = back_dir[mTankDir];
}

////////////////////////////////////////////////////////

CommonTank::CommonTank( byte level, BoxMarkStruct* bms) :
	EnemyBase(TANK_KIND::COMMON, level, bms)
{
	mTank = new TankInfo(GRAY_TANK, level, true);
}

CommonTank::~CommonTank()
{
	delete mTank;
	printf("~CommonTank::CommonTank() .. \n");
}

// 
void CommonTank::DrawTank(const HDC& center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&mTank->GetTankImage(mTankDir, mTankImageIndex++ / 3 % 2)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

//////////////////////////////////////////////////////////////////////////////


PropTank::PropTank(byte level, BoxMarkStruct* bms) :
	EnemyBase(TANK_KIND::PROP, level, bms)
{
	mTank[0] = new TankInfo(GRAY_TANK, level, true);
	mTank[1] = new TankInfo(RED_TANK, level, true);
}

PropTank::~PropTank()
{
	// ����ֱ�� delete[] mTank??
	for (int i = 0; i < 2; i++)
		delete mTank[i];
	printf("~PropTank::PropTank()..\n");
}

// 
void PropTank::DrawTank(const HDC& center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return; //printf("%d..\n", mTankImageIndex);
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&mTank[ index_counter++ / 6 % 2 ]->GetTankImage(mTankDir, mTankImageIndex++ / 3 % 2)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

//////////////////////////////////////////////////////////////

BigestTank::BigestTank(TANK_KIND kind, BoxMarkStruct * bms):
	EnemyBase(kind, 3, bms)
{
	mTank[GRAY_TANK] = new TankInfo(GRAY_TANK, 3, true);
	mTank[YELLOW_TANK] = new TankInfo(YELLOW_TANK, 3, true);
	mTank[RED_TANK] = new TankInfo(RED_TANK, 3, true);
	mTank[GREEN_TANK] = new TankInfo(GREEN_TANK, 3, true);

	hp = 4;
}

BigestTank::~BigestTank()
{
	for (int i = 0; i < 4; i++)
		delete mTank[i];
	printf("~BigestTank::BigestTank()\n");
}

void BigestTank::DrawTank(const HDC & center_hdc)
{
	if (!mStar.mIsOuted || mDied)
		return;

	// ����̹�˺���̹ͨ�˱�ɫ����
	TankInfo* temp[2] = { mTank[GRAY_TANK], mTank[GRAY_TANK] };

	switch (mEnemyTankKind)
	{
	case TANK_KIND::PROP:
		switch(hp)
		{
			case 4:
				temp[0] = mTank[RED_TANK];
				temp[1] = mTank[GRAY_TANK];
				break;
			case 3:
				temp[0] = mTank[RED_TANK];
				temp[1] = mTank[YELLOW_TANK];
				break;
			case 2:
				temp[0] = mTank[YELLOW_TANK];
				temp[1] = mTank[GRAY_TANK];
				break;
			default:
				break;
		}
		break;
	case TANK_KIND::COMMON:
		switch (hp)
		{
		case 4:
			temp[0] = mTank[GREEN_TANK];
			temp[1] = mTank[GRAY_TANK];
			break;
		case 3:
			temp[0] = mTank[GRAY_TANK];
			temp[1] = mTank[YELLOW_TANK];
			break;
		case 2:
			temp[0] = mTank[YELLOW_TANK];
			temp[1] = mTank[GREEN_TANK];
			break;
		default:
			break;
		}
		break;
	}

	if (temp == NULL)
	{
		printf("����!. EnemyBase.cpp");
		return;
	}
	TransparentBlt(center_hdc, (int)mTankX - BOX_SIZE, (int)mTankY - BOX_SIZE, BOX_SIZE * 2, BOX_SIZE * 2,
		GetImageHDC(&temp[index_counter++ / 7 % 2]->GetTankImage(mTankDir, mTankImageIndex++ / 3 % 2)), 0, 0, BOX_SIZE * 2, BOX_SIZE * 2, 0x000000);
}

bool BigestTank::BeKill(bool killanyway)
{
	if (mStar.mIsOuted == false || mBlast.IsBlasting() || mDied == true)
		return false;

	MciSound::_PlaySound(S_BIN);
	if (--hp <= 0 || killanyway)
	{
		hp = 0;
		return this->EnemyBase::BeKill(killanyway);
	}
	return false;
}
