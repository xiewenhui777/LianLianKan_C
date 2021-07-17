#include<stdio.h>
#include<stdlib.h> 
#include<conio.h>
#include<windows.h>
#include<time.h>

#define high 20
#define width 30

int canvas[high][width]={0};
int moveDirection;//С���ƶ�����1��2��3��4�ֱ������������ 
int score;
int food_x,food_y;	//��ʾʳ��λ�� 
int newhead_i,newhead_j;//��ͷ���� 
int oldtail_i,oldtail_j;//��β������ 
int num;


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

void moveSnake(){
	void end();
	
	for(int i=1;i<high-1;i++){
		for(int j=0;j<width-1;j++){
			if(canvas[i][j]>0)	canvas[i][j]++;
		}
	}
	int oldhead_i,oldhead_j;
	int max=0;
	
	for(int i=1;i<high-1;i++){
		for(int j=0;j<width-1;j++){
			if(canvas[i][j]>max){	//���������ǲ��ظ��ģ������ֵ��ֻ��һ�� 
				max=canvas[i][j];
				oldtail_i=i;
				oldtail_j=j;
			}
			if(canvas[i][j]==2){
				oldhead_i=i;
				oldhead_j=j;
			}	
		}
	}
	
	if(moveDirection==1){
		newhead_i=oldhead_i-1;
		newhead_j=oldhead_j;
	}
	if(moveDirection==2){
		newhead_i=oldhead_i+1;
		newhead_j=oldhead_j;
	}
	if(moveDirection==3){
		newhead_i=oldhead_i;
		newhead_j=oldhead_j-1;
	}
	if(moveDirection==4){
		newhead_i=oldhead_i;
		newhead_j=oldhead_j+1;
	}
	
	if(canvas[newhead_i][newhead_j]==-2){
		score++;num++;
		canvas[food_x][food_y]=0;
		food_x=rand()%(high-2)+1;
		food_y=rand()%(width-2)+1;
		canvas[food_x][food_y]=-2;
	}
	if(num!=5)	//��ʾÿ��5�������ӳ��� 
		canvas[oldtail_i][oldtail_j]=0;
	if(num==5)	num=0;
		
	if(canvas[newhead_i][newhead_j]==-1||canvas[newhead_i][newhead_j]>0){
		atexit(end);
		system("pause");
		exit(0);
	}
	else
		canvas[newhead_i][newhead_j]=1; 
} 

void startup(){
	srand((unsigned)time(NULL));
	score=0;
	num=0;
	
	for(int i=0;i<width;i++){
		canvas[0][i]=-1;
		canvas[high-1][i]=-1;
	}
	for(int i=0;i<high;i++){
		canvas[i][0]=-1;
		canvas[i][width-1]=-1;
	}
	
	moveDirection=3;	//��ʼ������ 
	canvas[high/2][width/2]=1;//��ʼ����ͷ 
	for(int i=1;i<=4;i++){
		canvas[high/2][width/2+i]=i+1;	//����ֱ�Ϊ2345 
	}	
	
	food_x=rand()%(high-2)+1;
	food_y=rand()%(width-2)+1;
	canvas[food_x][food_y]=-2;	//-2��ʾʳ�� 
}

void show(){
	gotoxy(0,0);
	HideCursor();
	
	for(int i=0;i<high;i++){
		for(int j=0;j<width;j++){
			if(canvas[i][j]==0)	printf(" ");
			else if(canvas[i][j]==-2){
				if(score%5==0&&score!=0) printf("$");	//����һ���� 
				else printf("F");	//���ʳ�� 	
			} 
			else if(canvas[i][j]==-1)	printf("#");	//����߿� 
			else if(canvas[i][j]==1)	printf("@");	//�����ͷ 
			else if(canvas[i][j]>=1)	printf("*");	//������� 
		}
		printf("\n");
	}
	printf("Your score:%3d\n",score);
}

void updateWithoutinput(){
	moveSnake();
	Sleep(150);
}

void updateWithinput(){
	char input;
	if(kbhit()){
		input=getch();
		switch(input){            
			case 'w':	moveDirection=1;moveSnake();break;
			case 's':	moveDirection=2;moveSnake();break;
			case 'a':	moveDirection=3;moveSnake();break;
			case 'd':	moveDirection=4;moveSnake();break;
		} 
	}
}

void end(){
	if(canvas[newhead_i][newhead_j]==-1)	printf("Your snake has stricken the wall!\n");
	else if(canvas[newhead_i][newhead_j]>0)	printf("Your snake bites itself!\n");
	printf("Failure!\n");
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
