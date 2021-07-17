#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

int x,y;//表示飞机位置 
int xx,yy;//表示子弹位置 
int high,width;
int enemy_x,enemy_y; 
int speed;
int score;//得分
int ctrl;//控制器控制速度,与speed有关， 
int temp;//统计每增加10分 



void gotoxy(int x,int y){							//清屏作用 
	HANDLE handle =GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos); 
}

void HideCursor(){
	CONSOLE_CURSOR_INFO cursor_info ={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

void startup(){
	high=20,width=30;
	x=high-3,y=width/2;
	xx=-1,yy=-1;
	enemy_x=-1;enemy_y=width/2;
	speed=0; 
	score=0;
	ctrl=10;
	temp=0;
}

void show(){
	gotoxy(0,0);
	for(int i=0;i<high;i++){
		for(int j=0;j<width;j++){//由于飞机的形状定义的if语句早于敌机，所以敌机碰到飞机机翼是敌机会消失，因为要先显示飞机 
			if(i==x&&j==y)	printf("^");		//飞机头部 
			else 
				if(i==x+1&&(j>=y-2&&j<=y+2))	printf("*");
			else 
				if(i==x+2&&(j==y-1||j==y+1))	printf("*");		
			else 
				if(i==xx&&j==yy)	printf("|");
			else 
				if(i==enemy_x&&j==enemy_y)	printf("Y");
			else printf(" ");
		}
		
		printf("\n");
	}
	printf("YOUR SCORE:%3d",score);
}

void updateWithoutinput(){
	
	if(xx>-1)	xx--;
	if(xx==enemy_x&&yy==enemy_y)	{//即子弹与飞机重合表示击中敌机 
		printf("\a");
		score++;temp++;
		enemy_x=-1;
		enemy_y=rand()%(width-4)+2; 
		xx=-1;					//子弹打完无效了 
	}
	if(enemy_x>high)	{
		enemy_x=-1;
		enemy_y=rand()%(width-4)+2;
	}
	if(speed<ctrl)	speed++;//此处不用Sleep函数试一下 
	else if(speed==ctrl){
		enemy_x++;
		speed=0;
	}
	if(temp==10){//导致得分越高敌机速度越快 
		ctrl--;
		temp=0;
	}
	
	//控制防止飞机头出界 
	if(x+2>=high)	x=high-3;
	if(x<0)	x=0;
	if(y+2>=width)	y=width-3;
	if(y-2<0) y=2;
}

void updateWithinput(){
	char input; 
	if(kbhit()){
		input=getch();
		switch(input){
		case 'w':x--;break;
		case 's':x++;break;
		case 'a':y--;break;
		case 'd':y++;break;
		case 27: getchar();break;
		default:;
			}
		if(input==' ')	{
			xx=x-1;yy=y;
		}
	}
}

int main(){
	startup();
	srand((unsigned)time(NULL));//生成随机种子 
	
	while(1){
		HideCursor();
		show();
		updateWithoutinput();
		updateWithinput();
	}
	return 0;
}
