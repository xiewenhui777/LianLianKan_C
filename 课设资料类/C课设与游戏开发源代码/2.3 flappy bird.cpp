#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

int high,width;	//画面大小 
int bird_x,bird_y;//小鸟的坐标 
int bar1_y,bar1_xDown,bar1_xTop;//障碍物 
int score;//记录分数，即小鸟穿过障碍的次数 
int speed; 

void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void HideCursor(){
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
	
}
void startup(){
	high=15;
	width=20;
	bird_x=0;
	bird_y=width/3;
	bar1_xDown=high/2;	//表示缝隙底部		//注意！书上写的全反了但不影响程序 
	bar1_xTop=high/3;	//表示缝隙顶部 
	int score=0;
	speed=1;
}

void show(){
	gotoxy(0,0);
	HideCursor();
	
	for(int i=0;i<high;i++){
		for(int j=0;j<width;j++){
			if(i==bird_x&&j==bird_y)	printf("@");
			else if(j==bar1_y&&(i<bar1_xTop||i>bar1_xDown))	printf("#");//因为从0开始，所以此处用大于号和小于号 
			else printf(" ");
		}
		printf("\n");
	}
	printf("SCORE:%3d\n",score);
}

void updateWithoutinput(){
	void end();
	
	if(bird_x%8==0)	{		//采取了加速下落手段，若不满意，可直接删去这段并照书上写 
		speed++;
	}
	if(bird_x+speed>=high) 	bird_x=high-1;	 //防止鸟出界 
	else bird_x+=speed;
	bar1_y--;
	
	if(bird_x>=high)	bird_x=high-1;
	if(bird_x<=0)	bird_x=0;
	
	srand((unsigned)time(NULL));
	if(bar1_y<=0){
		bar1_y=width; 
		int temp;
		temp=rand()%(int)(0.8*high);	//千万不能忘记强制类型转换 
		bar1_xDown=temp+high/10;	 
		bar1_xTop=temp-high/10;	//保证了缝隙的口子大小为high/5 
	}
	
	if(bar1_y==bird_y){
		if(bird_x>=bar1_xTop&&bird_x<=bar1_xDown)
			score++;
		else {
			atexit(end);
			system("pause");
			exit(0);
		}
	}
	Sleep(200);
}

void updateWithinput(){
	char input;
	if(kbhit()){
		input=getch();
		if(input==' ')	bird_x-=speed+1;
	}
	
}

void end(){
	printf("your final score is %d\n",score);
	printf("Keep fighting!");
}

int main(){
	startup();
	
	while(1){
		show();
		updateWithoutinput();
		updateWithinput();
	}
	
	return 0;
} 
