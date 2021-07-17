#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

int high,width;	//�����С 
int bird_x,bird_y;//С������� 
int bar1_y,bar1_xDown,bar1_xTop;//�ϰ��� 
int score;//��¼��������С�񴩹��ϰ��Ĵ��� 
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
	bar1_xDown=high/2;	//��ʾ��϶�ײ�		//ע�⣡����д��ȫ���˵���Ӱ����� 
	bar1_xTop=high/3;	//��ʾ��϶���� 
	int score=0;
	speed=1;
}

void show(){
	gotoxy(0,0);
	HideCursor();
	
	for(int i=0;i<high;i++){
		for(int j=0;j<width;j++){
			if(i==bird_x&&j==bird_y)	printf("@");
			else if(j==bar1_y&&(i<bar1_xTop||i>bar1_xDown))	printf("#");//��Ϊ��0��ʼ�����Դ˴��ô��ںź�С�ں� 
			else printf(" ");
		}
		printf("\n");
	}
	printf("SCORE:%3d\n",score);
}

void updateWithoutinput(){
	void end();
	
	if(bird_x%8==0)	{		//��ȡ�˼��������ֶΣ��������⣬��ֱ��ɾȥ��β�������д 
		speed++;
	}
	if(bird_x+speed>=high) 	bird_x=high-1;	 //��ֹ����� 
	else bird_x+=speed;
	bar1_y--;
	
	if(bird_x>=high)	bird_x=high-1;
	if(bird_x<=0)	bird_x=0;
	
	srand((unsigned)time(NULL));
	if(bar1_y<=0){
		bar1_y=width; 
		int temp;
		temp=rand()%(int)(0.8*high);	//ǧ��������ǿ������ת�� 
		bar1_xDown=temp+high/10;	 
		bar1_xTop=temp-high/10;	//��֤�˷�϶�Ŀ��Ӵ�СΪhigh/5 
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
