#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

int x,y;//��ʾ�ɻ�λ�� 
int xx,yy;//��ʾ�ӵ�λ�� 
int high,width;
int enemy_x,enemy_y; 
int speed;
int score;//�÷�
int ctrl;//�����������ٶ�,��speed�йأ� 
int temp;//ͳ��ÿ����10�� 



void gotoxy(int x,int y){							//�������� 
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
		for(int j=0;j<width;j++){//���ڷɻ�����״�����if������ڵл������Եл������ɻ������ǵл�����ʧ����ΪҪ����ʾ�ɻ� 
			if(i==x&&j==y)	printf("^");		//�ɻ�ͷ�� 
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
	if(xx==enemy_x&&yy==enemy_y)	{//���ӵ���ɻ��غϱ�ʾ���ел� 
		printf("\a");
		score++;temp++;
		enemy_x=-1;
		enemy_y=rand()%(width-4)+2; 
		xx=-1;					//�ӵ�������Ч�� 
	}
	if(enemy_x>high)	{
		enemy_x=-1;
		enemy_y=rand()%(width-4)+2;
	}
	if(speed<ctrl)	speed++;//�˴�����Sleep������һ�� 
	else if(speed==ctrl){
		enemy_x++;
		speed=0;
	}
	if(temp==10){//���µ÷�Խ�ߵл��ٶ�Խ�� 
		ctrl--;
		temp=0;
	}
	
	//���Ʒ�ֹ�ɻ�ͷ���� 
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
	srand((unsigned)time(NULL));//����������� 
	
	while(1){
		HideCursor();
		show();
		updateWithoutinput();
		updateWithinput();
	}
	return 0;
}
