#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"Winmm.lib")


#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_SHOT		16
#define	CMD_CHANGE		32

DWORD WINAPI ThreadFunc(LPVOID);//发射子弹线程
DWORD WINAPI ThreadFunc2(LPVOID);//释放技能线程
DWORD WINAPI ThreadFunc3(LPVOID);//画布运动线程

void loadbackground();
void gameindicate();
int GetCommand();
void playgame();
void bianjie();
void getjinbi();
int xiaomiediren();
IMAGE yanmatu(IMAGE *img);
void drawjinbi();//障碍函数


typedef struct dongxi
{
	IMAGE img;
	POINT p;
	int i;//是否活着
}Dongxi;
Dongxi jinbi[200];

typedef struct zidan
{
	IMAGE *img;
	POINT p;
}Zidan;


typedef struct jineng
{
	IMAGE img;
	int count;
	int jinengchufa;//技能被触发
}Jineng;


typedef struct Feiji
{
	POINT p;//飞机的坐标
	IMAGE *imgj;//精灵图
	IMAGE *imgy;//掩码图   (82*63)
	zidan shoot;
	Jineng jn;//装备技能
	int shengming;
	int ch;//按键
	int fenshu;
	int jinbicount;
	int speed;//敌机的速度
}airplane;

airplane MyAir;
airplane difangAir[5];

IMAGE imap1;//画布
int scrx,scry=-13300;//画布的起点

IMAGE tietu[5];
IMAGE bushang[200];
POINT pbaocun[200];

IMAGE bibang[2];

void main()
{

	initgraph(800,700);
	IMAGE img;
	loadbackground();
	getch();
	closegraph();

}

void loadbackground()
{
	IMAGE img;
	IMAGE img1;
	IMAGE img2;
	IMAGE img3;
	IMAGE img4;
	loadimage(&img,_T("IMAGE"),_T("background"));
	putimage(0,0,&img);
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTCYAN);
	getimage(&img1,260,300,300,40);
	getimage(&img2,260,380,300,40);
	getimage(&img3,260,460,300,40);
	getimage(&img4,0,0,800,220);
	settextstyle(40,30,_T("宋体"));
	outtextxy(260,220,"飞机大战");
	outtextxy(260,300,"开始游戏");
	outtextxy(260,380,"游戏说明");
	outtextxy(260,460,"关于作者");
	MOUSEMSG m;
	while(true)
	{
		m=GetMouseMsg();
		if(m.uMsg==WM_LBUTTONDOWN)
		{
			if(m.x<500 && m.x>260 && m.y<340 && m.y>300)
			{
				
				putimage(0,0,&img4);
				putimage(260,300,&img1);
				setlinecolor(YELLOW);
				rectangle(260,300,500,340);
				Sleep(200);
				outtextxy(260,300,"开始游戏");
				playgame();
				break;
		
			}
			if(m.x<500 && m.x>260 && m.y>380 &&m.y<420)
			{
				
				putimage(260,380,&img2);
				setlinecolor(YELLOW);
				rectangle(260,380,500,420);
				Sleep(200);
				outtextxy(260,380,"游戏说明");
				Sleep(100);
				gameindicate();
			}
			if(m.x<500 && m.x>260 && m.y>460 && m.y<500)
			{
				putimage(0,0,&img4);
				putimage(260,460,&img3);
				setlinecolor(YELLOW);
				rectangle(260,460,500,500);
				Sleep(200);
				outtextxy(260,460,"关于作者");
				Sleep(100);
				MessageBox(NULL,"开发者:IT爱好者|QQ群302641304","我爱游戏",MB_OK|MB_ICONASTERISK);
			}
		}
	}
	graphdefaults();
	cleardevice();
	setbkmode(TRANSPARENT);
	settextstyle(60,30,_T("宋体"));
	settextcolor(RED);
	outtextxy(200,200,_T("生命值不够!"));
	outtextxy(200,400,_T("您输啦!"));
	Sleep(5000);
	cleardevice();
	
	settextcolor(RED);
	char str[20];
	sprintf(str,"你的得分为%d",MyAir.fenshu);
	outtextxy(100,200,str);
	outtextxy(100,500,_T("Game Over!!!"));
	settextstyle(20,10,_T("宋体"));
	settextcolor(YELLOW);
	outtextxy(550,600,_T("-----小志作品"));
}

void gameindicate()
{
	settextstyle(20,10,_T("宋体"));
	outtextxy(30,50,"A:控制飞机左移");
	outtextxy(30,100,"D:控制飞机右移");
	outtextxy(30,150,"W:控制飞机上移");
	outtextxy(30,200,"S:控制飞机下移");
	outtextxy(550,100,"J:放子弹");
	outtextxy(550,150,"K:使用技能");
	settextstyle(40,30,_T("宋体"));
}

int GetCommand()
{
	int c = 0;

	if (GetAsyncKeyState('A') & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)	    c |= CMD_RIGHT;
	if (GetAsyncKeyState('W') & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState('S') & 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState('J') & 0x8000)	    c |= CMD_SHOT;
	if (GetAsyncKeyState('K') & 0x8000)	    c |= CMD_CHANGE;
	return c;
}

//线程发射子弹
DWORD WINAPI ThreadFunc(LPVOID n)
{
	BeginBatchDraw();
	while(MyAir.shoot.p.y>=0)
	{
		putimage(MyAir.shoot.p.x,MyAir.shoot.p.y,MyAir.shoot.img,SRCPAINT);
		MyAir.shoot.p.y-=10;
		Sleep(10);
	}
	if(MyAir.shoot.p.y==0)
	{
		MyAir.shoot.p.y=MyAir.p.y;
	}
	
	FlushBatchDraw();
	return 0;
}

//线程释放技能
DWORD WINAPI ThreadFunc2(LPVOID n)
{
	BeginBatchDraw();
	while(MyAir.shoot.p.y>=0)
	{
		putimage(MyAir.shoot.p.x,MyAir.shoot.p.y,MyAir.shoot.img,SRCPAINT);
		MyAir.shoot.p.y-=10;
	}
	if(MyAir.shoot.p.y==0)
	{
		MyAir.shoot.p.y=MyAir.p.y;
	}
	FlushBatchDraw();
	MyAir.jn.jinengchufa=1;
	MyAir.jn.count--;
	return 0;
}

void bianjie()
{
	char s[8][20];
	
	
	settextstyle(20,10,_T("宋体"));

	sprintf(s[0],"分数:%d",MyAir.fenshu);
	outtextxy(620,50,s[0]);

	sprintf(s[1],"技能值:%d",MyAir.jn.count);
	outtextxy(620,100,s[1]);

	sprintf(s[2],"生命值:%d",MyAir.shengming);
	outtextxy(620,150,s[2]);

	sprintf(s[3],"金币数:%d",MyAir.jinbicount);
	outtextxy(620,200,s[3]);
	
	sprintf(s[4],"生命值X%d",difangAir[0].shengming);
	outtextxy(680,270,s[4]);

	sprintf(s[5],":生命值X%d",difangAir[1].shengming);
	outtextxy(680,340,s[5]);
	
	sprintf(s[6],":生命值X%d",difangAir[2].shengming);
	outtextxy(680,410,s[6]);

	sprintf(s[7],":生命值X%d",difangAir[3].shengming);
	outtextxy(680,480,s[7]);

}

void DispatchCommand(int _cmd)
{
	bianjie();
	getjinbi();

	//线程部分	
	HANDLE hThread,hThread2;
	DWORD exitCode1 = 0;
	DWORD threadId;

	putimage(scrx,scry,&imap1);
	for(int i=0;i<4;i++)
	{
		getimage(&tietu[i],difangAir[i].p.x,difangAir[i].p.y,difangAir[i].imgy->getwidth(),difangAir[i].imgy->getheight());
		if(difangAir[i].shengming>0)
		{
			putimage(difangAir[i].p.x,difangAir[i].p.y,difangAir[i].imgy,SRCPAINT);
			difangAir[i].p.y+=difangAir[i].speed;
		}
	}

	for(i=0;i<4;i++)
	{
		if(difangAir[i].p.y>=700 && difangAir[i].shengming>=0 || difangAir[i].shengming<=0)
		{
			difangAir[i].p.x=rand()%550;
			difangAir[i].p.y=-difangAir[i].imgy->getheight();
			difangAir[i].speed=rand()%4+3;
			difangAir[i].shengming=i*2+4;
		}
		
	}
	scry+=2;

	if (_cmd & CMD_UP)
	{
		MyAir.p.y-=8;
	}
	if (_cmd & CMD_DOWN)
	{
		MyAir.p.y+=8;
	}
	if (_cmd & CMD_LEFT)
	{
		MyAir.p.x-=8;
	}
	if (_cmd & CMD_RIGHT)
	{
		MyAir.p.x+=8;
	}

	if (_cmd & CMD_SHOT)
	{
		hThread = CreateThread(NULL,0,ThreadFunc,(LPVOID)1,0,&threadId);
		GetExitCodeThread(hThread,&exitCode1);
	}

	if(_cmd & CMD_CHANGE)
	{
		if(MyAir.jn.count>=0)
		{
			hThread2 = CreateThread(NULL,0,ThreadFunc2,(LPVOID)1,0,&threadId);
			GetExitCodeThread(hThread,&exitCode1);
		}
	}

	if(MyAir.p.x<=-16)
	{
		MyAir.p.x=-16;
	}
	if(MyAir.p.x>=524)
	{
		MyAir.p.x=524;
	}
	if(MyAir.p.y<=0)
	{
		MyAir.p.y=0;
	}
	if(MyAir.p.y>=647)
	{
		MyAir.p.y=647;
	}
	if(scry>=0)
	{
		scry=0;
	}
	
	if(MyAir.shoot.p.y<=0)
	{
		MyAir.shoot.p.x=MyAir.p.x+29;
		MyAir.shoot.p.y=MyAir.p.y-50;
	}
	
}

int xiaomiediren()
{               

	for(int i=0;i<4;i++)
	{
		if(difangAir[i].shengming >0 && fabs(MyAir.shoot.p.y-difangAir[i].p.y)<=difangAir[i].imgy->getheight()-10 && fabs(MyAir.shoot.p.x-difangAir[i].p.x)<=difangAir[i].imgy->getwidth()-10)
		{
			difangAir[i].shengming--;
			MyAir.fenshu++;
		}
		
		//释放技能敌人消失
		if(MyAir.jn.jinengchufa==1 && fabs(MyAir.p.x-difangAir[i].p.x)<=difangAir[i].imgy->getwidth()-10 &&MyAir.p.y>difangAir[i].p.y)
		{
			difangAir[i].shengming--;
			MyAir.fenshu++;
		}

		//射击敌人之后，敌人就消失
		if(difangAir[i].shengming<=0)
		{
			putimage(difangAir[i].p.x,difangAir[i].p.y,&tietu[i]);
		}
		

		//判断是否受到伤害
		if(fabs(MyAir.p.x-difangAir[i].p.x)<=difangAir[i].imgy->getwidth()-10 && fabs(MyAir.p.y-difangAir[i].p.y)<=difangAir[i].imgy->getheight()-10 && difangAir[i].shengming>0)
		{
			MyAir.shengming--;
		}
	}

	if(scry==0)
	{
		getimage(&tietu[4],difangAir[4].p.x,difangAir[4].p.y,difangAir[4].imgy->getwidth(),difangAir[4].imgy->getheight());
		putimage(difangAir[4].p.x,difangAir[4].p.y,difangAir[4].imgy,SRCPAINT);
		difangAir[4].p.y+=difangAir[4].speed;	
		if(difangAir[4].p.y>=700 || difangAir[i].shengming<=0)
		{
			difangAir[4].p.x=rand()%550;
			difangAir[4].p.y=-difangAir[4].imgy->getheight();
			difangAir[4].speed=rand()%8+8;
			difangAir[4].shengming=20;
		}
		//每受到boss的伤害（即受到石头的撞击）掉5滴血

		if(fabs(MyAir.p.x-difangAir[4].p.x)<=difangAir[4].imgy->getwidth()-10 && fabs(MyAir.p.y-difangAir[4].p.y)<=difangAir[4].imgy->getheight()-10)
		{
			MyAir.shengming-=5;
		}
		//只有释放技能才能打死石头
		if(MyAir.jn.jinengchufa==1 && fabs(MyAir.p.x-difangAir[4].p.x)<=difangAir[4].imgy->getwidth()-10 &&MyAir.p.y>difangAir[4].p.y)
		{
			difangAir[4].shengming--;
			MyAir.fenshu+=5;
		}
		if(difangAir[4].shengming<=0)
		{
			putimage(difangAir[4].p.x,difangAir[4].p.y,&tietu[4]);
		}
	}
	return 0;
}


void getjinbi()
{
	for(int i=0;i<200;i++)
	{
		jinbi[i].p.y+=2;
		if(jinbi[i].i==0 && fabs(MyAir.p.x-jinbi[i].p.x)<=jinbi[i].img.getwidth()-10 && fabs(MyAir.p.y-jinbi[i].p.y)<=jinbi[i].img.getheight()-10)
		{
			MyAir.jinbicount++;
			MyAir.jn.count+=10;
			jinbi[i].i=-1;
			SetWorkingImage(&imap1);
			putimage(pbaocun[i].x,pbaocun[i].y,&bushang[i]);
			SetWorkingImage(NULL);
		}
	}
}


void drawjinbi()
{
	srand(time(0));
	
	SetWorkingImage(&imap1);

	//初始化金币
	int random[20];
	for(int i=0;i<20;i++)
	{
		random[i]=rand()%560;
	}

	for(i=0;i<200;i++)
	{
		loadimage(&jinbi[i].img,_T("IMAGE"),_T("jinbi"));
		
		pbaocun[i].x=jinbi[i].p.x=random[i/(rand()%10+4)];	
		pbaocun[i].y=jinbi[i].p.y=i*67+rand()%20+700;

		getimage(&bushang[i],pbaocun[i].x,pbaocun[i].y,jinbi[i].img.getwidth(),jinbi[i].img.getheight());
		putimage(jinbi[i].p.x,jinbi[i].p.y,&jinbi[i].img,SRCPAINT);
		jinbi[i].p.y-=13400;	
	}
	
	SetWorkingImage(NULL);
}


void playgame()
{
	cleardevice();
	
	IMAGE bianjie[4];
	loadimage(&bianjie[0],_T("IMAGE"),_T("bianjieone"));
	loadimage(&bianjie[1],_T("IMAGE"),_T("bianjietwo"));
	loadimage(&bianjie[2],_T("IMAGE"),_T("bianjiethree"));
	loadimage(&bianjie[3],_T("IMAGE"),_T("bianjiefour"));
	putimage(620,255,&bianjie[0]);
	putimage(620,320,&bianjie[1]);
	putimage(620,395,&bianjie[2]);
	putimage(620,470,&bianjie[3]);
	
	//游戏说明
	rectangle(610,550,790,670);
	settextstyle(12,6,_T("宋体"));
	outtextxy(620,560,_T("指点迷津:"));
	outtextxy(622,580,_T("碰到敌机障碍物会减生命值"));
	outtextxy(622,600,_T("敌机障碍物可用射击(j键)清除"));
	outtextxy(622,620,_T("下降石头只能用技能(K键)清除"));
	outtextxy(622,640,_T("得金币可以增加技能值"));
	outtextxy(622,680,_T("扫清障碍可得分"));
	
	setbkmode(OPAQUE);
	setfillcolor(RED);
	solidrectangle(602,0,608,800);
	

	//初始化飞机
	IMAGE imgy,imgj;
	MyAir.shengming=100;
	MyAir.fenshu=0;
	MyAir.p.x=270;
	MyAir.p.y=640;

	IMAGE imgzidan;
	loadimage(&imgzidan,_T("IMAGE"),_T("zidan1"));
	MyAir.shoot.img=&imgzidan;
	MyAir.shoot.p.x=MyAir.p.x+29;
	MyAir.shoot.p.y=MyAir.p.y-50;
	
	loadimage(&imgy,_T("IMAGE"),_T("play"));
	MyAir.imgy=&yanmatu(&imgy);
	loadimage(&imgj,_T("IMAGE"),_T("play"));
	MyAir.imgj=&imgj;
	MyAir.jn.count=500;
	
	putimage(MyAir.p.x,MyAir.p.y,MyAir.imgy,SRCAND);
	putimage(MyAir.p.x,MyAir.p.y,MyAir.imgj,SRCPAINT);
	
	loadimage(&imap1,_T("IMAGE"),_T("BG2"));
	putimage(scrx,scry,&imap1);
	drawjinbi();


	//初始化敌机
	IMAGE img[5];
	char s[30];
	for(int i=0;i<4;i++)
	{
		sprintf(s,"DIREN%d",i+1);
		loadimage(&img[i],_T("IMAGE"),s);
		difangAir[i].imgy=&img[i];
		difangAir[i].p.x=rand()%550;
		difangAir[i].p.y=-difangAir[i].imgy->getheight();
		difangAir[i].speed=rand()%3+3;
	}
	
	difangAir[3].shengming=10;
	difangAir[2].shengming=8;
	difangAir[1].shengming=6;
	difangAir[0].shengming=4;
	
	//初始化boss，即下落的石头
	loadimage(&img[4],_T("IMAGE"),_T("diren5"));
	difangAir[4].imgy=&img[4];
	difangAir[4].p.x=rand()%550;
	difangAir[4].p.y=-difangAir[4].imgy->getheight();
	difangAir[4].speed=rand()%5+5;


	//操作飞机
	while(MyAir.shengming>=0)
	{
		BeginBatchDraw();
		MyAir.ch=GetCommand();
		DispatchCommand(MyAir.ch);
		xiaomiediren();
		putimage(MyAir.p.x,MyAir.p.y,MyAir.imgy,SRCAND);
		putimage(MyAir.p.x,MyAir.p.y,MyAir.imgj,SRCPAINT);
		FlushBatchDraw();
		MyAir.jn.jinengchufa=0;
		Sleep(18);
	}


	
}

IMAGE yanmatu(IMAGE *img)
{
	COLORREF c;
	DWORD *p=GetImageBuffer(img);
	c=p[0];
	int w=(*img).getwidth();
	int h=(*img).getheight();
	for(int i=0;i<w*h;i++)
	{
		if(c==p[i])
			p[i]=WHITE;
		else
			p[i]=BLACK;
	}
	return *img;
}
