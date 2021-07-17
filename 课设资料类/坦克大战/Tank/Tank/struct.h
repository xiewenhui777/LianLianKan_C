#pragma once
#include "TimeClock.h"

#define WINDOW_WIDTH	512		// ���ڴ�С������������޸ģ�
#define WINDOW_HEIGHT	448
#define CANVAS_WIDTH	256		// ���� image ��С������ı䣬���һ�ͼ���� image �ϲ�����Ȼ��һ���Ի��Ƶ�������
#define CANVAS_HEIGHT	224
#define CENTER_WIDTH	208		// �м��ɫ��Ϸ����
#define CENTER_HEIGHT	208
#define CENTER_X		16		// ��ɫ��Ϸ����������Ͻǵ�����
#define CENTER_Y		9

// GameOver ������С
#define GAMEOVER_WIDTH	31
#define GAMEOVER_HEIGHT	15

// <<box_8>>, <<box_4>> ͬʱ���
#define _CLEAR		-1		// ����ϰ��ﱻ�����ĸ���, ����ɨ��ʱ�� 0 ���ֿ�, Ȼ����ƺ�ɫͼƬ, ����� 0 Ҳ���ж������
#define _EMPTY		0		// �յ�
#define _FOREST		1		// ɭ��
#define _ICE		2		// ��
// ------ ���� < 3 ̹�˿��Դ���
#define _WALL		3		// ǽ
#define _RIVER		4		// ����
#define _STONE		5		// ʯͷ

//* <<box_8 ���>> */
#define CAMP_SIGN	200			// ��Ӫ���
#define STAR_SIGN	2000		// �л������Ľ��Ǳ��, ��ʱ�л����ܽ���

/* <<box_4 ���>> */
#define PLAYER_SIGN	100

/*  <<box_4 ���>> 
* ���¶������:
	�涨����� 10000 + 1000*mEnemyTankLevel + 100*menemyKind + enemy_id 
*/
#define ENEMY_SIGN	10000		// 4*4 �� 8*8 ���ӱ��, ���ϵл����� id�ٱ��

/* 
* <<bullet_4 ���>>
* �л��ӵ����
* box_4[i][j] = E_B_SIGN + enemy_id
* ֻ��ǵ�ͷ�������ڵ���һ�� 4*4 ����
*/
#define E_B_SIGN	300

/* 
* <<bullet_4>> ���
* ����ӵ����
* bullet_4[i][j] = P_B_SIGN + player_id*10 + bullet_id 
* ֻ��ǵ�ͷ�������ڵ���һ�� 4*4 ����
*/
#define P_B_SIGN	400

/*
* <<bullet_4>> ���
�ӵ������ӵ�,����������box_4 ���Ϊ wait_unsign, �ȴ��Է�����
*/
#define WAIT_UNSIGN	444

// prop_8 ���
#define PROP_SIGN	3000		// ������ prop_8 �ı��


#define BOX_SIZE					8		// 26*26 �ĸ���
#define SMALL_BOX_SIZE				4		// 52*52 ���Ӵ�С
#define BLACK_NUMBER_SIZE			7		// ��ɫ���ִ�С

#define ENEMY_TANK_ICO_SIZE			7		// �л�ͼ���С
#define PLAYER_TANK_ICO_SIZE_X		7		// ���ͼ���С
#define PLAYER_TANK_ICO_SIZE_Y		8
#define PLAYER_12_ICO_SIZE_X		14		// 1P\2P ͼ���С
#define PLAYER_12_ICO_SIZE_Y		7
#define FLAG_ICO_SIZE_X				16		// ���Ӵ�С
#define FLAG_ICO_SIZE_Y				15
#define PLAYER_LIFE_NUMBER_SIZE		7		// ����������ִ�С

#define DIR_LEFT	0
#define DIR_UP		1
#define DIR_RIGHT	2
#define DIR_DOWN	3

#define GRAY_TANK	0		// ��ɫ̹��
#define RED_TANK	1		// ��ɫ̹��
#define YELLOW_TANK	2		// 
#define GREEN_TANK	3


#define SHOOTABLE_X		-100	// �涨�ӵ����� x = -100 �ӵ����Է���

// �ӵ����е��ϰ��������, ���� EnemyBase::CheckBomb ����ֵ
enum BulletShootKind {None, Player_1 = PLAYER_SIGN, Player_2 = PLAYER_SIGN + 1, Camp, Other};

/* ��Ӧ��ֵ
* 	#defien _EMPTY		0
#define _WALL		3
#define _FOREST		1
#define _ICE		2
#define _RIVER		4
#define _STONE		5
-------------------------------
* ��Ӧ�����ϵ
box[0][0-25] : ��һ��( y=0; x=[0-25] )
box[1][0-25] : �ڶ���( y=1; x=[0-25] )
...
box[y/BOX_SIZE][x/BOX_SIZE]
-------------------------------
*/
struct BoxMarkStruct
{
	int box_8[26][26];			// 8*8 ���ӵı��, ̹���ƶ�,�����øø��Ӽ��
	int box_4[52][52];			// 4*4 ���ӵı��, ǽ�������øñ�Ǽ��
	int prop_8[26][26];			// ���ߵĸ��ӱ��
	int bullet_4[52][52];		// �ӵ�����
};

#define TOTAL_ENEMY_NUMBER	20
#define SHOWING_STAR	true		// ������ʾ�Ľ���
#define STOP_SHOW_STAR	false		// �Ľ�����ʾ����

// ָʾ�л����͵���̹�˺���̹ͨ��
enum TANK_KIND { PROP, COMMON };

struct BulletStruct
{
	int x, y;					// �ӵ�ͼƬ���Ͻ�����, ���ݲ�ͬ����ָ����ͬ��λ�ô��� x,y, ָ�� x=SHOOTABLE_X ��ʱ����Է����ӵ�
	int dir;					// �ӵ�����
	int speed[4];				// �ӵ��ٶ�, ����̹�˼�����費ͬ�ٶ�ϵ��. ÿ���ƶ����ܳ���4�����ص�!! ��Ȼ���Խ 4*4 �ĸ���!!���bug

	/* �����һ��ел�,���ø�ֵΪ�л���id, GameControl ѭ���ڼ���ֵ, Ȼ��ɾ���õл�
	* ����ǵл��������, ���ø�ֵΪ���id, ��Ϸѭ�����ټ���ֵ�����������*/
	int mKillId;				// ��¼�ӵ����еĵл�/��� id

	static IMAGE mBulletImage[4];		// ͼƬ
	static int mBulletSize[4][2];		// {{4,3},{3,4},{4,3},{3,4}} �ߴ�: ����-3*4 / ����-4*3
	static int devto_tank[4][2];		// �涨�ӵ������������tank���ĵ��ƫ����
	static int devto_head[4][2];		// �涨�ӵ�ͼƬ���Ͻ�����ڵ�ͷ�����ƫ����;���·���ͷ�����ڵ�ͷ���ұ�;�������ڵ�ͷ�������Ǹ���
	static int bomb_center_dev[4][2];	// ��ը����������ӵ�ͷ��ƫ����
};

struct BombStruct
{
	static IMAGE mBombImage[3];				// �ӵ���ըͼ
	int mBombX, mBombY;						// ��ը����������
	bool canBomb;							// �Ƿ�ʼ��ʾ��ըͼƬ flag
	int counter;						// ȡģ������, ���ٴ�ѭ������һ��ͼƬ
};

// ̹�˱�ը״̬
enum BlastState {Blasting, BlastEnd, NotBlast};
class BlastStruct
{
public:
	BlastStruct();
	~BlastStruct() {}
	void Init();

	BlastState CampBlasting(const HDC&);

	/*PlayerBase �ڵ���, ѭ�����̹�˱�ը
	��ը��Ϸ��� BLAST_END
	����������� */
	BlastState Blasting(const HDC&);

	/*�л���ը��һ��, ��Ҫ��ʾ����*/
	BlastState EnemyBlasting(const HDC&, IMAGE*);

	/*���ÿ�ʼ��ը��־ �� ��������*/
	void SetBlasting(int tankx, int tanky);

	/*���ص�ǰ�Ƿ���ڱ�ը*/
	bool IsBlasting();


	static IMAGE image[5];
	int blastx, blasty;			// ��������, 32*32
	bool canBlast;
	int counter;				// �������ٴλ�һ��ͼƬ

	TimeClock timer;
};


// �л�������ҵ��Ľ�����˸��������
enum Star_State { 
	Star_Timing,		// ֻ���ڵл�, ��ʾ��ǰ��û�г����Ľ���, ���ڼ�ʱ֮��
	Star_Failed,		// ���ڵл�, ��ʾ��ǰλ�õл�����ʧ��
	Star_Out,			// �Ľ��Ǹոճ���
	Star_Showing,		// ������˸�Ľ���
	Star_Stop,			// �ո�ֹͣ��˸
	Tank_Out };			// ̹���Ѿ�����

class StarClass
{
public:
	StarClass();
	void Init();

	/*PlayerBase  �ڵ���, ������̹������*/
	Star_State ShowStar(const HDC&, int, int );
	bool IsStop();

	/*EnemyBase �ڵ���*/
	Star_State EnemyShowStar(const HDC&, int, int, const BoxMarkStruct* );

	static IMAGE mStarImage[4];	// �Ľ���ͼƬ, ����Ҹ�ֵ
	int mStarIndexDev;			// �����ı仯��, -1, 1  -1��star��С���, 1 ��star�ɴ��С
	byte mStarIndex : 2;		// �Ľ����±������仯���� 0-1-2-3-2-1-0-1-2-3-...
	int mStarCounter;			// ����,���ٴα��һ��ͼ��
	bool mIsOuted;				// �Ľ�����ʧ��ʱ��̹�˳���, ֹͣ�����Ľ�����˸ͼ

	// Enemy ר��! Player �ò���
	int mTankOutAfterCounter;	// һ���������֮��, �Ľ��ǿ�ʼ��˸,̹�˳���
	bool mStarOuted;		// �Ľ��Ǹճ���, ֻ���ڵл�
};

// ���̹�˳��ֵ�ʱ����ʾ����˸��״����Ȧ
class RingClass
{
public:
	RingClass();
	void Init();
	bool ShowRing(const HDC&, int centerx, int centery);		//

	/*���ÿ�����ʾ��״, PlayerBase �ڻ�ȡ���ߺ����
	����������ʾ���, ������ʾ�϶�, ��õ��ߺ���ʾ�ϳ�*/
	void SetShowable(long);				// 

	static IMAGE image[2];
	bool canshow;			// �Ƿ������ʾ��״
	int index_counter;		// �仯�±�����

	TimeClock timer;
};

// ��������
#define ADD_PROP	0	// �ӻ�
#define STAR_PROP	1	// �����
#define TIME_PROP	2	// ʱ��
#define BOMB_PROP	3	// ����
#define SHOVEL_PROP	4	// ����
#define CAP_PROP	5	// ñ��

/*** ������,
* �� PlayerBase ʵ������Աָ��
* PlayerBase ���캯���ڵ��ø���һ���������� BoxMarkStuct* ����
* �������к������� PlayerBase �ڵ���
***/
class PropClass
{
	void SignPropBox(int val);			// ��� prop_8 ����

public:
	PropClass();
	void Init(BoxMarkStruct * b);
	void ShowProp(const HDC&);			// GameControl ��ѭ�����ú���

	/*���������Ͻǵ�����*/
	void StartShowProp(int x, int y);	// ����̹�˱����к���øú���
	void StopShowProp(bool);			// ֹͣ��ʾ����, ��ʱ���߱���һ��, ����ָʾ����һ�û��ǳ�ʱ

private:
	int score_counter;		// ������ʾ���
	bool show_score;
	IMAGE score;				// 500 ����

	BoxMarkStruct* bms;
	int leftx, topy;			// �������ĵ�����
	int index_counter = 0;			// �±�任����
	/*static */IMAGE image[6];
	/*static */int prop_kind;			// ��������
	bool can_show;				// �Ƿ������ʾ����
	//TimeClock mTimer;
};

/*
* ÿһ��ʤ��\ʧ�ܺ���ʾ�ķ������
* PlayerBase ��ʵ����
* һ�����һ������
*/
class ScorePanel
{
public:
	ScorePanel(int player_id);
	~ScorePanel();
	bool show(const HDC&);

	/*��PlayerBase �ڵ���, PlayerBase����GameControl �ڵ���
	* ����ɱ����, �����
	*/
	void ResetData(const int * nums, const int&, const int& stage);				// ÿ����ʾǰ��Ҫ����

	static IMAGE background;

private:

	static IMAGE yellow_number;
	static IMAGE number;
	IMAGE player;
	IMAGE pts;
	static IMAGE bunds;		// bunds 1000pts ����

	// ����ĸ���ҷ�����,����ʾ���ı�
	static int who_bunds[2];			// ��ʾ�����һ������Ҷ���

	static int player_num;		// �����; ��ʼ��һ��������� +1
	static bool line_done_flag[2];	// ÿһ������������Ҷ���ʾ�����

	byte player_id;
	int player_x, player_y;
	int pts_x, pts_y;
	int kill_num[4], kill_num2[4];		// 4�ֵл�ɱ����, kill_num = -1 �����Ȳ���ʾ

	// x[..][0] �Ƿ���, x[..][1] ��ɱ����
	int x[4][2];	// һ���Ƿ���, һ����ɱ����, �������� ɱ����*100,200,400...
	int y[4][2];

	static int cur_line;		// ��ǰ���ڸ�����ʾ������������

	int total_kill_numm;		// ��ɱ����
	int total_kill_x, total_kill_y;	// ����

	static int end_counter;		// ���������ʾ��ȫ������ת

	int total_score_x, total_score_y;
	int total_score;			// �ܷ�
	int stage;
};

/*ĳ����ұ����𵥶���ʾ�� gameover ����*/
class PlayerGameover
{
public:
	PlayerGameover();
	~PlayerGameover() {}
	void Init(int p_id);
	void SetShow();
	void Show(const HDC& center_hdc);

	TimeClock mGameoverTimer;		//
	int mGameover_Dev;			// X ���ƶ�, ���һ����������, ��Ҷ��Ǹ�������
	int mGameoverX, mGameoverY;
	int mGameover_end_x;			// ͼƬֹͣ�� x ����
	static IMAGE mGameoverImage;		// �������ֵ����, ��ʾһ��ˮƽ�ƶ��� GAMEOVER ����
	int mGameoverCounter;			// ͼƬֹͣ������ʧ
	bool mShowGameover;
};