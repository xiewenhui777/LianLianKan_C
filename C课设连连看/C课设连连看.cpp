// C课设连连看.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include <stdio.h>
#include<conio.h>
#include<graphics.h>
#include<stdlib.h>
#include<time.h> 
#include<string.h>
#include<windows.h>
#include<stdbool.h>
#include<math.h>
#pragma comment(lib,"Winmm.lib")
#pragma warning(disable:4996)；

#define _CRT_SECURE_NO_DEPRECATE；
#define High 600
#define Width 800
#define PI acos(-1)						//精确地表示π

struct BALL {
	int x;
	int y;
	int needle_x1, needle_y1;			//表示针距离球远的一端的坐标
	int needle_x2, needle_y2;			//表示针距离球远的一端的坐标
	int radius;
	bool isbullet;						//判断谁当下准备开火,有开火权，但被ruin后仍为true，表示有过炮针的痕迹，便于进行连线,另外一方面可以冲充当副球们是否遍历的情况
	int bullet_x1, bullet_y1, bullet_x2, bullet_y2;	//表示炮弹的坐标
	int bullet_angle;			//炮弹的角度
	bool ruin;
	int vx, vy;						//小球速度
}ball[10];

typedef struct file {
	int score;
	char name[10];
	struct file *next;
}F;

struct getrank {
	char name[10];
	int score;
	char info[20];						//用于输出
	TCHAR userinfo[20];
}rank[100];



void updateInfo(clock_t	*time1, clock_t *time2, clock_t *timepause, clock_t *timeend, int *jetlag, int *oclock, int *ball_x, int *ball_y, int *radius, int *needle_x1, int *needle_y1, int *needle_x2, int *needle_y2, int *bullet_x1, int *bullet_y1, int *bullet_x2, int *bullet_y2, int *needle_length, int *bullet_length, double *bullet_angle, double *angle, int *n, int *state, int *score, int *lineNum, bool *ruin, bool *reborn, int toline[], int *degree) {
	srand((unsigned)time(NULL));


	*needle_length = 20;
	*radius = 40;
	*ball_x = rand() % (Width - 2 * (*radius) - 2 * 40) + *radius + 40;
	*ball_y = rand() % (High - 2 * (*radius + *needle_length) - 2 * 40) + *radius + *needle_length + 40;	//防止小针出界或圆不完整
	*needle_x1 = *ball_x;
	*needle_y1 = *ball_y - *radius;
	*needle_x2 = *ball_x;
	*needle_y2 = *needle_y1 - *needle_length;
	*angle = 0;
	*state = 0;
	*oclock = 60;
	*ruin = false;
	*reborn = false;
	*lineNum = 0;
	*score = 0;


	*time1 = 0; *time2 = 0;
	*timepause = 0;
	*timeend = 0;
	*timepause = 0;
	*jetlag = 0;


	*bullet_x1 = -1; *bullet_y1 = -1;
	*bullet_x2 = -1; *bullet_y2 = -1;
	*bullet_length = 20;
	*bullet_angle = 0;

	
	//先让其随机生成，在遍历一遍结构体数组判断是否范围有重复（即交叉）,倘若有再重新随机生成
	for (int i = 0; i < 20; i++) {
		toline[i] = -1;							//数组记得要清0，不用0的原因是防止下标为0的小球来乱连线，防止多次玩游戏产生乱连线问题
	}

	switch (*degree) {
	case 1:	*n = 3; break;
	case 2: *n = 6; break;
	case 3: *n = 10; break;
	}
	for (int i = 0; i < *n; i++) {
		ball[i].radius = 40;
		while (1) {
			int flag = 0;
			ball[i].x = rand() % (Width - 2 * ball[i].radius - 2 * 40) + ball[i].radius + 40;
			ball[i].y = rand() % (High - 2 * ball[i].radius - 2 * 40) + ball[i].radius + 40;
			if (i == 0) {
				if (!(ball[i].x - 2 * ball[i].radius >= *ball_x || ball[i].x + 2 * ball[i].radius <= *ball_x || ball[i].y - 2 * ball[i].radius >= *ball_y || ball[i].y + 2 * ball[i].radius <= *ball_y)) {
					flag = 1;
				}
			}
			for (int j = 0; j < i; j++) {
				if (!((ball[i].x - 2 * ball[i].radius >= ball[j].x || ball[i].x + 2 * ball[i].radius <= ball[j].x || ball[i].y - 2 * ball[i].radius >= ball[j].y || ball[i].y + 2 * ball[i].radius <= ball[j].y) && (ball[i].x - 2 * ball[i].radius >= *ball_x || ball[i].x + 2 * ball[i].radius <= *ball_x || ball[i].y - 2 * ball[i].radius >= *ball_y || ball[i].y + 2 * ball[i].radius <= *ball_y))) {
					flag = 1;
					break;
				}
			}
			if (flag != 1)	break;
		}

		ball[i].vx = 0;
		ball[i].vy = 0;
		ball[i].bullet_angle = 0;
		ball[i].bullet_x1 = -1, ball[i].bullet_y1 = -1, ball[i].bullet_x2 = -1, ball[i].bullet_y2 = -1;
		ball[i].needle_x1 = -1; ball[i].needle_y1 = -1;
		ball[i].needle_x2 = -1; ball[i].needle_y2 = -1;
		ball[i].isbullet = false;
		ball[i].ruin = false;
	}
}

void startup(IMAGE *img_bk, IMAGE *img_pause1, IMAGE *img_pause2, IMAGE *img_ball1, IMAGE *img_ball2, IMAGE *img_needle1, IMAGE *img_needle2, IMAGE *img_enter, IMAGE *img_pausemenu, IMAGE *img_mainmenu, IMAGE *img_mainmenu1, IMAGE *img_return1, IMAGE *img_return2, IMAGE *img_win, IMAGE *img_defeat, IMAGE *img_difficulty, IMAGE *img_instruction) {


	loadimage(&*img_enter, _T("material\\enter.jpg"));
	loadimage(&*img_bk, _T("material\\background.jpg"));
	loadimage(&*img_pause1, _T("material\\pause1_mask.bmp"));
	loadimage(&*img_pause2, _T("material\\pause1.bmp"));
	loadimage(&*img_ball1, _T("material\\ball1.jpg"));
	loadimage(&*img_ball2, _T("material\\ball2.jpg"));
	loadimage(&*img_needle1, _T("material\\needle_mask.bmp"));
	loadimage(&*img_needle2, _T("material\\needle.bmp"));
	loadimage(&*img_pausemenu, _T("material\\pausemenu.jpg"));
	loadimage(&*img_mainmenu, _T("material\\mainmenu.jpg"));
	loadimage(&*img_mainmenu1, _T("material\\mainmenu1.jpg"));
	loadimage(&*img_return1, _T("material\\return_mask.bmp"));
	loadimage(&*img_return2, _T("material\\return.bmp"));
	loadimage(&*img_win, _T("material\\win.jpg"));
	loadimage(&*img_defeat, _T("material\\defeat.jpg"));
	loadimage(&*img_difficulty, _T("material\\difficulty.jpg"));
	loadimage(&*img_instruction, _T("material\\instruction.jpg"));


	initgraph(Width, High);					//使用initgraph(Width, High, SHOWCONSOLE);可以进行调试
	// 获取窗口句柄
	HWND hwnd = GetHWnd();
	// 设置窗口标题文字
	SetWindowText(hwnd, _T(">_< 连连看 >_<"));

	BeginBatchDraw();

	mciSendString(_T("open material\\mainmenumusic.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
}


void show(int *oclock, IMAGE *img_bk, IMAGE *img_pause1, IMAGE *img_pause2, IMAGE *img_ball1, IMAGE *img_ball2, IMAGE *img_needle1, IMAGE *img_needle2, IMAGE *img_enter, IMAGE *img_pausemenu, IMAGE *img_mainmenu, IMAGE *img_mainmenu1, IMAGE *img_return1, IMAGE *img_return2, IMAGE *img_win, IMAGE *img_defeat, IMAGE *img_difficulty, IMAGE *img_instruction, int *ball_x, int *ball_y, int *radius, int *needle_x1, int *needle_y1, int *needle_x2, int *needle_y2, int *bullet_x1, int *bullet_y1, int *bullet_x2, int *bullet_y2, int *score, int *n, int toline[], int *scene) {
	void scene0(IMAGE *img_enter);
	void scene1(IMAGE *img_mainmenu);
	void scene2(int *oclock, IMAGE *img_bk, IMAGE *img_pause1, IMAGE *img_pause2, IMAGE *img_ball1, IMAGE *img_ball2, int *ball_x, int *ball_y, int *radius, int *needle_x1, int *needle_y1, int *needle_x2, int *needle_y2, int *bullet_x1, int *bullet_y1, int *bullet_x2, int *bullet_y2, int *score, int *n, int toline[]);
	void scene3(IMAGE *img_pausemenu);
	void scene5(IMAGE *img_return1, IMAGE *img_return2, IMAGE *img_difficulty);
	void scene6(IMAGE *img_mainmenu1, IMAGE *img_return1, IMAGE *img_return2);
	void scene7(IMAGE *img_instruction, IMAGE *img_return1, IMAGE *img_return2);
	void scene8(IMAGE *img_win, int *score);
	void scene9(IMAGE *img_defeat, int *score);

	switch (*scene) {
	case 0:scene0(img_enter); break;
	case 1:scene1(img_mainmenu); break;
	case 2:scene2(oclock, img_bk, img_pause1, img_pause2, img_ball1, img_ball2, ball_x, ball_y, radius, needle_x1, needle_y1, needle_x2, needle_y2, bullet_x1, bullet_y1, bullet_x2, bullet_y2, score, n, toline); break;
	case 3:scene3(img_pausemenu); break;
	case 5:scene5(img_return1, img_return2, img_difficulty); break;
	case 6:scene6(img_mainmenu1, img_return1, img_return2); break;
	case 7:scene7(img_instruction, img_return1, img_return2); break;
	case 8:scene8(img_win, score); break;
	case 9:scene9(img_defeat, score); break;
	}
}

//以下的8个scene函数都只在show函数中展示
void scene0(IMAGE *img_enter) {		//即登入图
	cleardevice();					//把加载的图片全部清除
	putimage(0, 0, img_enter);
	FlushBatchDraw();					//关键因素

}
void scene1(IMAGE *img_mainmenu) {//即startmenu()
	cleardevice();
	putimage(0, 0, img_mainmenu);
	FlushBatchDraw();
}
void scene2(int *oclock, IMAGE *img_bk, IMAGE *img_pause1, IMAGE *img_pause2, IMAGE *img_ball1, IMAGE *img_ball2, int *ball_x, int *ball_y, int *radius, int *needle_x1, int *needle_y1, int *needle_x2, int *needle_y2, int *bullet_x1, int *bullet_y1, int *bullet_x2, int *bullet_y2, int *score, int *n, int toline[]) {//即正常运行
	cleardevice();

	putimage(0, 0, img_bk);
	putimage(0, 0, img_pause1, SRCAND);
	putimage(0, 0, img_pause2, SRCPAINT);

	setfillcolor(MAGENTA);
	setcolor(RED);
	TCHAR  SCORE[5];

	swprintf_s(SCORE, _T("%d"), *score);
	setcolor(BLACK);							//将倒计时文字设为黑色
	setbkmode(TRANSPARENT);							//将文字设为透明的不带底色
	settextstyle(16, 0, _T("宋体"));
	outtextxy((int)(Width * 7 / 8), 50, _T("Score:"));
	outtextxy((int)(Width * 7 / 8), 70, SCORE);

	putimage(*ball_x - *radius, *ball_y - *radius, img_ball1, NOTSRCERASE);
	putimage(*ball_x - *radius, *ball_y - *radius, img_ball2, SRCINVERT);


	for (int i = 0; i < *n; i++) {
		putimage(ball[i].x - *radius, ball[i].y - ball[i].radius, img_ball1, NOTSRCERASE);
		putimage(ball[i].x - *radius, ball[i].y - ball[i].radius, img_ball2, SRCINVERT);
	}

	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
	setlinecolor(BROWN);
	line(*needle_x1, *needle_y1, *needle_x2, *needle_y2);
	for (int i = 0; i < *n; i++) {
		line(ball[i].needle_x1, ball[i].needle_y1, ball[i].needle_x2, ball[i].needle_y2);
	}

	if (*bullet_x2 != -1 && *bullet_y2 != -1) {				//解决子弹有夹角问题
		setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
		setlinecolor(GREEN);
		line(*bullet_x1, *bullet_y1, *bullet_x2, *bullet_y2);
	}
	for (int i = 0; i < *n; i++) {
		if (ball[i].bullet_x2 != -1 && ball[i].bullet_y2 != -1) {
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
			setlinecolor(GREEN);
			line(ball[i].bullet_x1, ball[i].bullet_y1, ball[i].bullet_x2, ball[i].bullet_y2);
		}
	}

	setcolor(WHITE);
	if (ball[toline[0]].isbullet == true) {
		line(*ball_x, *ball_y, ball[toline[0]].x, ball[toline[0]].y);
	}
	for (int i = 1; i < *n; i++) {		//！！！这里疑似出现问题，因为最开始初始化时toline数组均变为0，导致后期哪个下标是0的小球可能出现了神经病
		if (toline[i] >= 0) {
			if (ball[toline[i]].isbullet == true)	line(ball[toline[i - 1]].x, ball[toline[i - 1]].y, ball[toline[i]].x, ball[toline[i]].y);
		}

	}

	TCHAR  sclock[5];						//用于保存倒计时的字符串

	if (*oclock >= 0) {
		swprintf_s(sclock, _T("%d"), *oclock);
		setcolor(DARKGRAY);							//将倒计时文字设为灰色
		setbkmode(TRANSPARENT);							//将文字设为透明的不带底色
		settextstyle(16, 0, _T("宋体"));
		outtextxy((int)(Width * 7 / 8), 10, _T("剩余时间:"));
		outtextxy((int)(Width * 7 / 8), 30, sclock);
	}

	FlushBatchDraw();
	Sleep(25);							//缓冲一下在隐藏，注意sleep函数可以控制旋转的速度
}
void scene3(IMAGE *img_pausemenu) {//即pausemenu()
	putimage(Width / 2 - 150, High / 2 - 200, img_pausemenu);

	FlushBatchDraw();
}
void scene5(IMAGE *img_return1, IMAGE *img_return2, IMAGE *img_difficulty) {				//游戏难度
	cleardevice();
	putimage(0, 0, img_difficulty);
	putimage(0, 0, img_return1, SRCAND);
	putimage(0, 0, img_return2, SRCPAINT);
	FlushBatchDraw();
}
void scene6(IMAGE *img_mainmenu1, IMAGE *img_return1, IMAGE *img_return2) {		//游戏排行榜
	void userrank();

	cleardevice();
	putimage(0, 0, img_mainmenu1);
	putimage(0, 0, img_return1, SRCAND);
	putimage(0, 0, img_return2, SRCPAINT);

	userrank();
	FlushBatchDraw();
}
void scene7(IMAGE *img_instruction, IMAGE *img_return1, IMAGE *img_return2) {	//游戏帮助
	cleardevice();
	putimage(0, 0, img_instruction);
	putimage(0, 0, img_return1, SRCAND);
	putimage(0, 0, img_return2, SRCPAINT);
	FlushBatchDraw();
}
void scene8(IMAGE *img_win, int *score) {	//成功界面
	putimage(Width / 2 - 150, High / 2 - 200, img_win);
	TCHAR  SCORE[5];

	swprintf_s(SCORE, _T("%d"), *score);
	setcolor(BLACK);							//将倒计时文字设为黑色
	setbkmode(TRANSPARENT);							//将文字设为透明的不带底色
	settextstyle(16, 0, _T("宋体"));
	outtextxy(Width / 2 - 150 + 80, High / 2 - 200 + 150, _T("Your Score is:"));
	outtextxy(Width / 2 - 150 + 200, High / 2 - 200 + 150, SCORE);

	FlushBatchDraw();
}
void scene9(IMAGE *img_defeat, int *score) {	//失败界面    可以考虑把失败界面和成功界面合并
	putimage(Width / 2 - 150, High / 2 - 200, img_defeat);

	char s[5];
	TCHAR  SCORE[5];
	swprintf_s(SCORE, _T("%d"), *score);
	sprintf_s(s, "%d", *score);
	setcolor(BLACK);							//将倒计时文字设为黑色
	setbkmode(TRANSPARENT);							//将文字设为透明的不带底色
	settextstyle(16, 0, _T("宋体"));
	outtextxy(Width / 2 - 150 + 80, High / 2 - 200 + 150, _T("Your Score is:"));
	outtextxy(Width / 2 - 150 + 200, High / 2 - 200 + 150, SCORE);

	FlushBatchDraw();
}

void TcharToChar(const TCHAR * tchar, char * _char)		//问学长如何解决字串TCHAR
{
	int iLength;
	//获取字节长度    
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char     
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}


void record(F * h) {
	char s[5];
	FILE *fp;								//记录分数的文件

	while (h->next != NULL) {
		h = h->next;
	}
	sprintf_s(s, "%d", h->score);


	fp = fopen("RECORD.txt", "a+");
	fprintf(fp, "%-10s%s\n", h->name, s);
	fclose(fp);
}

F *create() {				//创建头结点
	F *f = (F *)malloc(sizeof(F));
	f->next = NULL;
	return f;
}
void adduser(F * h, char name[]) {
	F *f = (F *)malloc(sizeof(F));
	strcpy(f->name, name);
	f->next = NULL;

	/*h = head;*/
	while (h->next != NULL) {			//发生了访问冲突,使用了被删除的结点，思考如何修改
		h = h->next;
	}
	h->next = f;
}
void addscore(F * h, int *score) {
	while (h->next != NULL) {
		h = h->next;
	}
	h->score = *score;
}
void free_user(F * h) {			//倘若用户不保存成绩,则销毁当前结点
	F *p = h->next;
	while (p->next != NULL) {
		h = p;
		p = p->next;
	}
	h->next = p->next;
	free(p);
}
void readfileandsort() {							//读取文件并排序
	FILE *fp;								//记录分数的文件

	int readnum = 0;							//表示每次读取文件时的下标
	fp = fopen("RECORD.txt", "r");
	char a[10], b[10];							//吃掉前面的垃圾 
	fscanf(fp, "%s%s", a, b);


	int e = 0;		//!feof(fp)
	while (!feof(fp)) {//判断是否到文件末尾		//出现访问问题			//可以先试一下到20
		fscanf(fp, "%s%d", rank[readnum].name, &rank[readnum].score);
		sprintf_s(rank[readnum].info, sizeof(rank[readnum].info), "%-10s%d", rank[readnum].name, rank[readnum].score);
		readnum++;
		/*e++;*/
	}
	//文件读取到一段奇怪的数字,可能读取的只是地址而非里面的值
	fclose(fp);
	/*strcpy(rank[readnum].info, rank[readnum].name);
	sprintf_s(rank[readnum].info, "%d", rank[readnum].score);*/
	struct getrank temp;
	for (int j = 0; j < readnum - 1; j++) {
		for (int k = j + 1; k < readnum; k++) {
			if (rank[j].score < rank[k].score) {
				temp = rank[j];
				rank[j] = rank[k];
				rank[k] = temp;
			}
		}
	}
}

void userrank() {
	readfileandsort();

	setcolor(RED);
	setbkmode(TRANSPARENT);							//将文字设为透明的不带底色
	settextstyle(35, 0, _T("宋体"));
	outtextxy(Width / 2 - 120, 60, _T("USERS     SCORE"));

	setcolor(BLACK);
	for (int i = 0; i < 10; i++) {		//将1换成10
		CharToTchar(rank[i].info, rank[i].userinfo);
		/*swprintf_s(rank[i].userinfo, _T("%s"), rank[i].info);*/
		outtextxy(Width / 2 - 120, 60 + 35 * (i + 2), rank[i].userinfo);
	}

}


void updateWithoutinput(clock_t *time1, clock_t *time2, clock_t *timepause, clock_t *timeend, int *jetlag, int *oclock, double *bullet_angle, double *angle, int *bullet_x1, int *bullet_y1, int *bullet_x2, int *bullet_y2, int *needle_x1, int *needle_y1, int *bullet_length, int *needle_length, int *needle_x2, int *needle_y2, int *ball_x, int *ball_y, int *radius, int *n, int *state, int *score, int *lineNum, int *scene, bool *ruin, bool *reborn, int toline[]) {

	if (*scene == 2) {

		mciSendString(_T("open material\\gamemusic.mp3 alias bkmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic"), NULL, 0, NULL);

		*jetlag += *timeend - *timepause;
		*timeend = 0; *timepause = 0;		//更新为0
		*time2 = clock();
		*oclock = (int)(60 - ((*time2 - *jetlag - *time1) / 1000));

		if (*ruin == false) {						//主球还在转动
			for (int i = 0; i < *n; i++) {			//主球第一次出击去连接
				if ((*bullet_x2 - ball[i].x)*(*bullet_x2 - ball[i].x) + (*bullet_y2 - ball[i].y)*(*bullet_y2 - ball[i].y) <= ball[i].radius * ball[i].radius) {//可能需要改动，可能击不中
					if (ball[i].isbullet == true)	continue;				//返回连接时不受影响

					ball[i].needle_x1 = ball[i].x;
					ball[i].needle_y1 = ball[i].y - ball[i].radius;
					ball[i].needle_x2 = ball[i].x;
					ball[i].needle_y2 = ball[i].needle_y1 - *needle_length;
					ball[i].isbullet = true;
					*score += 20;

					toline[(*lineNum)++] = i;//按照顺序保存后就方便连线了,表示被打到的圆按顺序存进toline数组中
					//ball[i].bullet_angle = angle;

					*ruin = true;
					*needle_x1 = -1; *needle_y1 = -1;//上一个球的针消失
					*needle_x2 = -1; *needle_y2 = -1;
					break;			//防止一次击中两个球
				}
			}
		}
		else {
			for (int i = 0; i < *n; i++) {
				if (ball[i].isbullet == true && ball[i].ruin == false) {
					for (int j = 0; j < *n; j++) {
						if (j == i) continue;

						if ((ball[i].bullet_x2 - ball[j].x)*(ball[i].bullet_x2 - ball[j].x) + (ball[i].bullet_y2 - ball[j].y)*(ball[i].bullet_y2 - ball[j].y) <= ball[j].radius * ball[j].radius) {
							if (ball[j].isbullet == true)	continue;				//返回连接时不受影响
							ball[j].needle_x1 = ball[j].x;
							ball[j].needle_y1 = ball[j].y - ball[j].radius;
							ball[j].needle_x2 = ball[j].x;
							ball[j].needle_y2 = ball[j].needle_y1 - *needle_length;
							ball[j].isbullet = true;
							*score += 20;

							ball[i].needle_x1 = -1; ball[i].needle_y1 = -1;//上一个球的针消失
							ball[i].needle_x2 = -1; ball[i].needle_y2 = -1;
							ball[i].ruin = true;
							toline[(*lineNum)++] = j;
							break;			//可写可不写，只是加快跳出
						}
					}
					break;
				}
			}
		}


		for (int i = 0; i < *n; i++) {				//返回去连接主球
			if (ball[i].isbullet == true && ball[i].ruin == false) {
				if ((ball[i].bullet_x2 - *ball_x)*(ball[i].bullet_x2 - *ball_x) + (ball[i].bullet_y2 - *ball_y)*(ball[i].bullet_y2 - *ball_y) <= (*radius) * (*radius)) {

					//设置只有最后连成环的时候才能成功，否则之前返回去连接属于失败
					//如果全部遍历到表示成功，否则则跳出
					int flag = 1;
					for (int j = 0; j < *n; j++) {
						if (ball[j].isbullet == false)	flag = 0;			//代表着还没有遍历完成
					}
					if (flag) {
						*scene = 8;//表示游戏的成功
						mciSendString(_T("close bkmusic"), NULL, 0, NULL);
						/*score = oclock * 10;*/
						ball[i].needle_x1 = -1; ball[i].needle_y1 = -1;//上一个球的针消失
						ball[i].needle_x2 = -1; ball[i].needle_y2 = -1;
						ball[i].ruin = true;
						*score += 20;//表示连到最后一个球加20分
						*score += *oclock * 5;//表示游戏结束时的时间奖励分

						*reborn = true;			//宣告了主球的假复活,reborn可以考虑删去

						setcolor(WHITE);
						if (*reborn == true)	line(ball[toline[(*lineNum) - 1]].x, ball[toline[(*lineNum) - 1]].y, *ball_x, *ball_y);
					}
				}
				break;
			}
		}

		if (!(*state))	*angle += PI / 32;//点击的时候瞬间停一下//角度旋转一次可以尽可能的小，这样可以保证子弹会击中目标
		if (!(*ruin)) {			//摧毁了变便再无小针
			*needle_x1 = *ball_x + (int)(*radius * sin(*angle));
			*needle_y1 = *ball_y - (int)(*radius * cos(*angle));
			*needle_x2 = *ball_x + (int)((*needle_length + *radius) * sin(*angle));
			*needle_y2 = *ball_y - (int)((*needle_length + *radius) * cos(*angle));
		}

		for (int i = 0; i < *n; i++) {
			if (ball[i].isbullet == true && ball[i].ruin == false) {
				ball[i].needle_x1 = ball[i].x + (int)(ball[i].radius * sin(*angle));
				ball[i].needle_y1 = ball[i].y - (int)(ball[i].radius * cos(*angle));
				ball[i].needle_x2 = ball[i].x + (int)((*needle_length + ball[i].radius) * sin(*angle));
				ball[i].needle_y2 = ball[i].y - (int)((*needle_length + ball[i].radius) * cos(*angle));
			}
		}

		if (*bullet_x2 >= Width || *bullet_y2 <= 0 || *bullet_x2 <= 0 || *bullet_y2 >= High) {
			*bullet_x1 = -1; *bullet_y1 = -1;
			*bullet_x2 = -1; *bullet_y2 = -1;
			*bullet_angle = 0;
		}
		for (int i = 0; i < *n; i++) {
			if (ball[i].bullet_x2 >= Width || ball[i].bullet_y2 <= 0 || ball[i].bullet_x2 <= 0 || ball[i].bullet_y2 >= High) {
				ball[i].bullet_x1 = -1; ball[i].bullet_y1 = -1;
				ball[i].bullet_x2 = -1; ball[i].bullet_y2 = -1;
				ball[i].bullet_angle = 0;
			}
		}

		//小球移动,每个球都控制在一个小区域内
		for (int i = 0; i < *n; i++) {
			int k = rand() % 4;
			switch (k) {	//解决小球会碰撞问题
			case 0: ball[i].vx = 5; ball[i].vy = 0;	break;			//处理小球重叠问题
			case 1: ball[i].vx = 0; ball[i].vy = 5; break;
			case 2: ball[i].vx = -5; ball[i].vy = 0; break;
			case 3: ball[i].vx = 0; ball[i].vy = -5; break;
			}
		}


		if (*bullet_x2 != -1 && *bullet_y2 != -1) {			//子弹的上升
			*bullet_x1 += (int)(40 * sin(*bullet_angle));
			*bullet_y1 -= (int)(40 * cos(*bullet_angle));
			*bullet_x2 += (int)(40 * sin(*bullet_angle));
			*bullet_y2 -= (int)(40 * cos(*bullet_angle));
		}

		for (int i = 0; i < *n; i++) {
			if (ball[i].bullet_x2 != -1 && ball[i].bullet_y2 != -1) {
				ball[i].bullet_x1 += (int)(40 * sin(ball[i].bullet_angle));
				ball[i].bullet_y1 -= (int)(40 * cos(ball[i].bullet_angle));
				ball[i].bullet_x2 += (int)(40 * sin(ball[i].bullet_angle));
				ball[i].bullet_y2 -= (int)(40 * cos(ball[i].bullet_angle));
			}
			int flag = 0;
			for (int j = 0; j < *n; j++) {
				if (j == i)	continue;
				if (!((ball[i].x - 2 * ball[i].radius + ball[i].vx >= ball[j].x || ball[i].x + 2 * ball[i].radius + ball[i].vx <= ball[j].x || ball[i].y - 2 * ball[i].radius + ball[i].vy >= ball[j].y || ball[i].y + 2 * ball[i].radius + ball[i].vy <= ball[j].y) && (ball[i].x - 2 * ball[i].radius + ball[i].vx >= *ball_x || ball[i].x + 2 * ball[i].radius + ball[i].vx <= *ball_x || ball[i].y - 2 * ball[i].radius + ball[i].vy >= *ball_y || ball[i].y + 2 * ball[i].radius + ball[i].vy <= *ball_y))) {
					flag = 1;
					//倘若两球相碰撞时让他们反着走
					ball[i].x -= ball[i].vx;
					ball[i].y -= ball[i].vy;
				}
			}

			if (flag == 0) {
				if (ball[i].x >= ball[i].radius + 5 && ball[i].x <= Width - ball[i].radius - 5 && ball[i].y >= ball[i].radius + 5 && ball[i].y <= High - ball[i].radius - 5) {
					ball[i].x += ball[i].vx;
					ball[i].y += ball[i].vy;
				}
				else {
					//使小球不走到控制区域
					if (ball[i].x - ball[i].radius <= 40 && ball[i].y - ball[i].radius <= 40) {
						ball[i].x += 5;
						ball[i].y += 5;
					}
					else if (ball[i].x + ball[i].radius >= Width * 7 / 8 && ball[i].y - ball[i].radius <= 100) {
						ball[i].x -= 5;
						ball[i].y += 5;
					}
					else {
						if (ball[i].x <= ball[i].radius + 5)	ball[i].x += 5;
						if (ball[i].x >= Width - ball[i].radius - 5)  ball[i].x -= 5;
						if (ball[i].y <= ball[i].radius + 5)	ball[i].y += 5;
						if (ball[i].y >= High - ball[i].radius - 5)	ball[i].y -= 5;
					}
				}
			}
		}

		*state = 0;	    									//状态复原

		if (*oclock == -1) {
			*scene = 9;
			mciSendString(_T("close bkmusic"), NULL, 0, NULL); // 关闭音乐
			/*score = 0;*/
		}
	}
}


void updateWithinput(clock_t *time1, clock_t *time2, clock_t *timepause, clock_t *timeend, int *jetlag, int *oclock, double *bullet_angle, double *angle, int *bullet_x1, int *bullet_y1, int *bullet_x2, int *bullet_y2, int *needle_x1, int *needle_y1, int *bullet_length, int *ball_x, int *ball_y, int *needle_x2, int *needle_y2, int *radius, int *needle_length, int *n, int *state, int *degree, int *score, int *lineNum, int *scene, bool *ruin, bool *reborn, int toline[]) {
	MOUSEMSG m;
	TCHAR username[10];
	static char name[10];		//先放在全局变量，应为游戏中用户名不能变，可以尝试用一下static
	static F * head;								//头结点

	char input;
	if (_kbhit()) {
		input = _getch();
		if (*scene == 0 && input == 32) {
			InputBox(username, 10, _T("请输入用户名(非中文"));		//解决名字问题
			TcharToChar(username, name);
			head = create();
			mciSendString(_T("close bkmusic"), NULL, 0, NULL);
			*scene = 1;
		}
	}

	while (MouseHit()) {		//如果检查到按键弹起的消息，发射子弹
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP) {
			F *h = head;			//将h座位头结点传入之后的函数中，防止head头结点可能遭到修改
			if (*scene == 1) {
				if (m.x <= 480 && m.x >= 300 && m.y <= 150 && m.y >= 110) {
					*scene = 2;
					updateInfo(time1, time2, timepause, timeend, jetlag, oclock, ball_x, ball_y, radius, needle_x1, needle_y1, needle_x2, needle_y2, bullet_x1, bullet_y1, bullet_x2, bullet_y2, needle_length, bullet_length, bullet_angle, angle, n, state, score, lineNum, ruin, reborn, toline, degree);
					adduser(h, name);

					*time1 = clock();			//计时的起点
					*time2 = clock();

					mciSendString(_T("close bkmusic"), NULL, 0, NULL);
				}
				else if (m.x <= 480 && m.x >= 300 && m.y <= 240 && m.y >= 200) {
					*scene = 5;
				}
				else if (m.x <= 460 && m.x >= 320 && m.y <= 320 && m.y >= 280) {
					*scene = 6;
				}
				else if (m.x <= 480 && m.x >= 300 && m.y <= 410 && m.y >= 365) {
					*scene = 7;
				}

				else if (m.x <= 480 && m.x >= 300 && m.y <= 500 && m.y >= 460) {
					exit(0);
				}
				break;
			}

			else if (*scene == 2) {
				if (m.x <= 40 && m.x >= 0 && m.y <= 40 && m.y >= 0) {
					*scene = 3;
					mciSendString(_T("pause bkmusic"), NULL, 0, NULL); // 暂停播放
					*timepause = clock();
					break;
				}

				if (!(*ruin)) {								//表示未被ruin时生成子弹
					*bullet_angle = (int)(*angle);		//改成double类型
					*bullet_x1 = *needle_x1;
					*bullet_y1 = *needle_y1;
					*bullet_x2 = *needle_x1 + (int)(*bullet_length) * sin(*bullet_angle);
					*bullet_y2 = *needle_y1 - (int)(*bullet_length) * cos(*bullet_angle);
				}
				for (int i = 0; i < *n; i++) {				//表示且isbullet是true时未被ruin时生成子弹
					if (ball[i].isbullet == true && ball[i].ruin == false) {	//双重标准严格的很好的进行了控制
						ball[i].bullet_angle = *angle;
						ball[i].bullet_x1 = ball[i].needle_x1;
						ball[i].bullet_y1 = ball[i].needle_y1;
						ball[i].bullet_x2 = ball[i].needle_x1 + *bullet_length * sin(ball[i].bullet_angle);
						ball[i].bullet_y2 = ball[i].needle_y1 - *bullet_length * cos(ball[i].bullet_angle);
						break;
					}
				}
				*state = 1;									//让圆的转动暂停一下
			}

			else if (*scene == 3) {
				if (m.x <= Width / 2 - 150 + 220 && m.x >= Width / 2 - 150 + 80 && m.y <= High / 2 - 200 + 170 && m.y >= High / 2 - 200 + 140) {
					*scene = 2;
					mciSendString(_T("resume bkmusic"), NULL, 0, NULL); // 恢复播放 
					*timeend = clock();
				}
				else if (m.x <= Width / 2 - 150 + 230 && m.x >= Width / 2 - 150 + 65 && m.y <= High / 2 - 200 + 255 && m.y >= High / 2 - 200 + 230) {
					free_user(h);
					*scene = 1;
				}
				break;
			}

			else if (*scene == 5) {
				if (m.x <= 80 && m.x >= 0 && m.y <= 80 && m.y >= 0) {
					*scene = 1;
				}
				else if (m.x <= 510 && m.x >= 275 && m.y <= 195 && m.y >= 135) {//增加难度的图片，在设置按钮，点完难度后直接要调到scene1去
					*degree = 1;
					*scene = 1;
				}
				else if (m.x <= 510 && m.x >= 275 && m.y <= 305 && m.y >= 250) {
					*degree = 2;
					*scene = 1;
				}
				else if (m.x <= 510 && m.x >= 275 && m.y <= 420 && m.y >= 365) {
					*degree = 3;
					*scene = 1;
				}
				break;
			}

			else if (*scene == 6) {
				if (m.x <= 80 && m.x >= 0 && m.y <= 80 && m.y >= 0) {
					*scene = 1;
				}
				break;
			}

			else if (*scene == 7) {
				if (m.x <= 80 && m.x >= 0 && m.y <= 80 && m.y >= 0) {
					*scene = 1;
				}
				break;
			}

			else if (*scene == 8) {
				if (m.x <= Width / 2 - 150 + 220 && m.x >= Width / 2 - 150 + 90 && m.y <= High / 2 - 200 + 240 && m.y >= High / 2 - 200 + 210) {//保存成绩
					addscore(h, score);
					record(h);
				}
				else if (m.x <= Width / 2 - 150 + 240 && m.x >= Width / 2 - 150 + 70 && m.y <= High / 2 - 200 + 310 && m.y >= High / 2 - 200 + 280) {
					free_user(h);
					*scene = 1;
				}
				break;
			}

			else if (*scene == 9) {
				if (m.x <= Width / 2 - 150 + 220 && m.x >= Width / 2 - 150 + 90 && m.y <= High / 2 - 200 + 240 && m.y >= High / 2 - 200 + 210) {//保存成绩
					addscore(h, score);
					record(h);
				}
				else if (m.x <= Width / 2 - 150 + 240 && m.x >= Width / 2 - 150 + 70 && m.y <= High / 2 - 200 + 310 && m.y >= High / 2 - 200 + 275) {
					free_user(h);
					*scene = 1;
				}
				break;
			}
		}
	}
}


int main()
{
	IMAGE img_bk;							//背景图片
	IMAGE img_pause1;						//暂停图标遮罩图
	IMAGE img_pause2;						//暂停图标
	IMAGE img_ball1;						//球的遮罩图
	IMAGE img_ball2;						//旋转球
	IMAGE img_needle1;						//针的遮罩图
	IMAGE img_needle2;						//针
	IMAGE img_enter;						//登入封面
	IMAGE img_pausemenu;					//暂停界面
	IMAGE img_mainmenu;						//主菜单
	IMAGE img_mainmenu1;					//主菜单的背景
	IMAGE img_return1, img_return2;			//返回键
	IMAGE img_win, img_defeat;				//成功与失败
	IMAGE img_difficulty;					//难度
	IMAGE img_instruction;					//游戏说明


	int oclock;								//倒计时
	//TCHAR  sclock[5];						//用于保存倒计时的字符串
	clock_t	time1, time2;					//用于保存时间
	clock_t	timepause, timeend;				//用于计算游戏过程中暂停时流逝的时间,记一个间隔差
	int jetlag;								//表示时差

	int ball_x, ball_y;						//表示最开始的主球坐标
	int needle_x1, needle_y1;				//表示主针距离球近的一端的坐标
	int needle_x2, needle_y2;				//表示主针距离球远的一端的坐标
	int radius;								//表示主球的半径
	int needle_length;						//表示共同的针的长度
	double angle;							//表示转动的角度
	int bullet_x1, bullet_y1, bullet_x2, bullet_y2;	//表示炮弹的坐标
	int  bullet_length;	double bullet_angle;		//炮弹的角度和长度(公用)

	int lineNum;							//表示连线下标
	int state;								//表示状态
	int score;								//记录最终得分
	int degree = 1;							//表示难度信息，1-简单，2-适宜，3-困难
	int n;									//用于改变游戏中小球的个数

	int scene = 0;							//表示场景的切换,0—登入封图，1—主菜单，2—游戏运行界面，3—游戏暂停，5—游戏难度，6—游戏排行榜，7—游戏帮助，8—成功界面，9—失败界面(8，9都代表了游戏结束
	bool ruin;								//主球的摧毁,此处的ruin要注意，是指射中别人后自己不再旋转，然后就摧毁
	bool reborn;							//主球重生（假性）
	int toline[20];							//保存要连线的球的下标

	loadimage(&img_enter, _T("material\\enter.jpg"));
	putimage(0, 0, &img_enter);
	startup(&img_bk, &img_pause1, &img_pause2, &img_ball1, &img_ball2, &img_needle1, &img_needle2, &img_enter, &img_pausemenu, &img_mainmenu, &img_mainmenu1, &img_return1, &img_return2, &img_win, &img_defeat, &img_difficulty, &img_instruction);

	while (1) {
		show(&oclock, &img_bk, &img_pause1, &img_pause2, &img_ball1, &img_ball2, &img_needle1, &img_needle2, &img_enter, &img_pausemenu, &img_mainmenu, &img_mainmenu1, &img_return1, &img_return2, &img_win, &img_defeat, &img_difficulty, &img_instruction, &ball_x, &ball_y, &radius, &needle_x1, &needle_y1, &needle_x2, &needle_y2, &bullet_x1, &bullet_y1, &bullet_x2, &bullet_y2, &score, &n, toline, &scene);
		updateWithoutinput(&time1, &time2, &timepause, &timeend, &jetlag, &oclock, &bullet_angle, &angle, &bullet_x1, &bullet_y1, &bullet_x2, &bullet_y2, &needle_x1, &needle_y1, &bullet_length, &needle_length, &needle_x2, &needle_y2, &ball_x, &ball_y, &radius, &n, &state, &score, &lineNum, &scene, &ruin, &reborn, toline);
		updateWithinput(&time1, &time2, &timepause, &timeend, &jetlag, &oclock, &bullet_angle, &angle, &bullet_x1, &bullet_y1, &bullet_x2, &bullet_y2, &needle_x1, &needle_y1, &bullet_length, &ball_x, &ball_y, &needle_x2, &needle_y2, &radius, &needle_length, &n, &state, &degree, &score, &lineNum, &scene, &ruin, &reborn, toline);
	}
	_getch();
	return 0;
}
