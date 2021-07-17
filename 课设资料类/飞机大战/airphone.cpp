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

DWORD WINAPI ThreadFunc(LPVOID);//�����ӵ��߳�
DWORD WINAPI ThreadFunc2(LPVOID);//�ͷż����߳�
DWORD WINAPI ThreadFunc3(LPVOID);//�����˶��߳�

void loadbackground();
void gameindicate();
int GetCommand();
void playgame();
void bianjie();
void getjinbi();
int xiaomiediren();
IMAGE yanmatu(IMAGE *img);
void drawjinbi();//�ϰ�����


typedef struct dongxi
{
	IMAGE img;
	POINT p;
	int i;//�Ƿ����
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
	int jinengchufa;//���ܱ�����
}Jineng;


typedef struct Feiji
{
	POINT p;//�ɻ�������
	IMAGE *imgj;//����ͼ
	IMAGE *imgy;//����ͼ   (82*63)
	zidan shoot;
	Jineng jn;//װ������
	int shengming;
	int ch;//����
	int fenshu;
	int jinbicount;
	int speed;//�л����ٶ�
}airplane;

airplane MyAir;
airplane difangAir[5];

IMAGE imap1;//����
int scrx,scry=-13300;//���������

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
	settextstyle(40,30,_T("����"));
	outtextxy(260,220,"�ɻ���ս");
	outtextxy(260,300,"��ʼ��Ϸ");
	outtextxy(260,380,"��Ϸ˵��");
	outtextxy(260,460,"��������");
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
				outtextxy(260,300,"��ʼ��Ϸ");
				playgame();
				break;
		
			}
			if(m.x<500 && m.x>260 && m.y>380 &&m.y<420)
			{
				
				putimage(260,380,&img2);
				setlinecolor(YELLOW);
				rectangle(260,380,500,420);
				Sleep(200);
				outtextxy(260,380,"��Ϸ˵��");
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
				outtextxy(260,460,"��������");
				Sleep(100);
				MessageBox(NULL,"������:IT������|QQȺ302641304","�Ұ���Ϸ",MB_OK|MB_ICONASTERISK);
			}
		}
	}
	graphdefaults();
	cleardevice();
	setbkmode(TRANSPARENT);
	settextstyle(60,30,_T("����"));
	settextcolor(RED);
	outtextxy(200,200,_T("����ֵ����!"));
	outtextxy(200,400,_T("������!"));
	Sleep(5000);
	cleardevice();
	
	settextcolor(RED);
	char str[20];
	sprintf(str,"��ĵ÷�Ϊ%d",MyAir.fenshu);
	outtextxy(100,200,str);
	outtextxy(100,500,_T("Game Over!!!"));
	settextstyle(20,10,_T("����"));
	settextcolor(YELLOW);
	outtextxy(550,600,_T("-----С־��Ʒ"));
}

void gameindicate()
{
	settextstyle(20,10,_T("����"));
	outtextxy(30,50,"A:���Ʒɻ�����");
	outtextxy(30,100,"D:���Ʒɻ�����");
	outtextxy(30,150,"W:���Ʒɻ�����");
	outtextxy(30,200,"S:���Ʒɻ�����");
	outtextxy(550,100,"J:���ӵ�");
	outtextxy(550,150,"K:ʹ�ü���");
	settextstyle(40,30,_T("����"));
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

//�̷߳����ӵ�
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

//�߳��ͷż���
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
	
	
	settextstyle(20,10,_T("����"));

	sprintf(s[0],"����:%d",MyAir.fenshu);
	outtextxy(620,50,s[0]);

	sprintf(s[1],"����ֵ:%d",MyAir.jn.count);
	outtextxy(620,100,s[1]);

	sprintf(s[2],"����ֵ:%d",MyAir.shengming);
	outtextxy(620,150,s[2]);

	sprintf(s[3],"�����:%d",MyAir.jinbicount);
	outtextxy(620,200,s[3]);
	
	sprintf(s[4],"����ֵX%d",difangAir[0].shengming);
	outtextxy(680,270,s[4]);

	sprintf(s[5],":����ֵX%d",difangAir[1].shengming);
	outtextxy(680,340,s[5]);
	
	sprintf(s[6],":����ֵX%d",difangAir[2].shengming);
	outtextxy(680,410,s[6]);

	sprintf(s[7],":����ֵX%d",difangAir[3].shengming);
	outtextxy(680,480,s[7]);

}

void DispatchCommand(int _cmd)
{
	bianjie();
	getjinbi();

	//�̲߳���	
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
		
		//�ͷż��ܵ�����ʧ
		if(MyAir.jn.jinengchufa==1 && fabs(MyAir.p.x-difangAir[i].p.x)<=difangAir[i].imgy->getwidth()-10 &&MyAir.p.y>difangAir[i].p.y)
		{
			difangAir[i].shengming--;
			MyAir.fenshu++;
		}

		//�������֮�󣬵��˾���ʧ
		if(difangAir[i].shengming<=0)
		{
			putimage(difangAir[i].p.x,difangAir[i].p.y,&tietu[i]);
		}
		

		//�ж��Ƿ��ܵ��˺�
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
		//ÿ�ܵ�boss���˺������ܵ�ʯͷ��ײ������5��Ѫ

		if(fabs(MyAir.p.x-difangAir[4].p.x)<=difangAir[4].imgy->getwidth()-10 && fabs(MyAir.p.y-difangAir[4].p.y)<=difangAir[4].imgy->getheight()-10)
		{
			MyAir.shengming-=5;
		}
		//ֻ���ͷż��ܲ��ܴ���ʯͷ
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

	//��ʼ�����
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
	
	//��Ϸ˵��
	rectangle(610,550,790,670);
	settextstyle(12,6,_T("����"));
	outtextxy(620,560,_T("ָ���Խ�:"));
	outtextxy(622,580,_T("�����л��ϰ���������ֵ"));
	outtextxy(622,600,_T("�л��ϰ���������(j��)���"));
	outtextxy(622,620,_T("�½�ʯͷֻ���ü���(K��)���"));
	outtextxy(622,640,_T("�ý�ҿ������Ӽ���ֵ"));
	outtextxy(622,680,_T("ɨ���ϰ��ɵ÷�"));
	
	setbkmode(OPAQUE);
	setfillcolor(RED);
	solidrectangle(602,0,608,800);
	

	//��ʼ���ɻ�
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


	//��ʼ���л�
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
	
	//��ʼ��boss���������ʯͷ
	loadimage(&img[4],_T("IMAGE"),_T("diren5"));
	difangAir[4].imgy=&img[4];
	difangAir[4].p.x=rand()%550;
	difangAir[4].p.y=-difangAir[4].imgy->getheight();
	difangAir[4].speed=rand()%5+5;


	//�����ɻ�
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
