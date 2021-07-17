#include<stdio.h>
#include<stdlib.h> 
#include<time.h>		//srand头文件 
#include<conio.h>
#include<windows.h>
#include<stdbool.h>

typedef struct block{
	int x,y; 
}BLOCK;

BLOCK B[6];		//!!!!千万注意此处括号里的数与下面的block_n一致，若不一致会导致程序错误 
int high,width;
int ball_x,ball_y;//球位置 
int vx,vy;//球速 
int position_x,position_y;//板中点坐标 
int radius;
int left,right;//板左端和右端的y的坐标 
int score;
int ball_n;//反弹数
int block_n;	//同一屏幕上砖块的数量 


void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void startup(){
	high=12,width=17;
	ball_x=0,ball_y=width/2;
	vx=1,vy=1;
	position_x=0,position_y=width/2;
	radius=5;
	left=position_y-radius,right=position_y+radius;
	score=0;
	ball_n=0;
	block_n=6;
	
	for(int i=0;i<block_n;i++){
		B[i].x=0;
	}
	
	srand((unsigned)time(NULL));//随机生成种子 
	//以下为了产生block_n个互不相同的随机数 
	int k=0;	//只是一个临时的变量 
	while(k<block_n){
		int i=0;
		B[k].y=rand()%(width-1);
		for(i=0;i<k;i++){
			if(B[i].y==B[k].y)	break;
		}
		if(i<k)	continue;	//直接跳过k++ 
		
		k++;
	}
}


void show(){
	gotoxy(0,0);		//将光标移到(0，0)处 
	
	bool produceblock(int x,int y);		//对下面要用到的函数进行声明 
	
	for(int i=0;i<=high+1;i++){	//注意此处用了等于号，因为要设置边框 
		for(int j=0;j<=width;j++){
			if(i==ball_x&&j==ball_y)	printf("O");
			else if(i==high+1)	printf("~");
			else if(j==width)	printf("|");
			else if(i==high&&j>=left&&j<=right)	printf("*");
			else if(produceblock(i,j))	printf("#");//表示一块砖 
			else printf(" ");
		}
		printf("\n");//注意换行 
	}
	printf("BALL NUMBER:%3d\n",ball_n);
	printf("YOUR  SCORE:%3d",score);
}

void updateWithoutinput(){
	void end();
	
	ball_x+=vx;
	ball_y+=vy;
	if(ball_x==0||ball_x==high-1)	{
		vx=-vx;
	}
	if(ball_y==0||ball_y==width-1)	{
		vy=-vy;
	}
	if(ball_x==high-1)	{ //注意此处ball_x==high-1一定要单列 
		if(ball_y>=left&&ball_y<=right){
			ball_n++;
			printf("\a");
		} 
		else{
			atexit(end);
			system("pause");
			exit(0);
		}
	}	
	
	for(int i=0;i<block_n;i++){
		if(ball_x==B[i].x&&ball_y==B[i].y){
			score++;
			int flag=0;
			B[i].y=rand()%(width-1);
		}
	}
	
	Sleep(150);
} 

void updateWithinput(){
	char input;
	if(kbhit()){
		input=getch();
		if(input=='a'){
			position_y--;
			left--,right--;
		}
		if(input=='d'){
			position_y++;
			left++,right++;
		}
		if(input=='n')	ball_y--;	//n使小球左移 
		if(input=='m')	ball_y++;	//m使小球左移
	}
}
bool produceblock(int x,int y){
	for(int i=0;i<block_n;i++){
		if(x==B[i].x&&y==B[i].y)	return true;
	}
	return false;
}

void end(){
	if(score>=1)	printf("Win!!!\n");
	else printf("Failure!\n");
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
