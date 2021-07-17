#include<stdio.h>
#include<stdlib.h> 
#include<time.h>		//srandͷ�ļ� 
#include<conio.h>
#include<windows.h>
#include<stdbool.h>

typedef struct block{
	int x,y; 
}BLOCK;

BLOCK B[6];		//!!!!ǧ��ע��˴�����������������block_nһ�£�����һ�»ᵼ�³������ 
int high,width;
int ball_x,ball_y;//��λ�� 
int vx,vy;//���� 
int position_x,position_y;//���е����� 
int radius;
int left,right;//����˺��Ҷ˵�y������ 
int score;
int ball_n;//������
int block_n;	//ͬһ��Ļ��ש������� 


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
	
	srand((unsigned)time(NULL));//����������� 
	//����Ϊ�˲���block_n��������ͬ������� 
	int k=0;	//ֻ��һ����ʱ�ı��� 
	while(k<block_n){
		int i=0;
		B[k].y=rand()%(width-1);
		for(i=0;i<k;i++){
			if(B[i].y==B[k].y)	break;
		}
		if(i<k)	continue;	//ֱ������k++ 
		
		k++;
	}
}


void show(){
	gotoxy(0,0);		//������Ƶ�(0��0)�� 
	
	bool produceblock(int x,int y);		//������Ҫ�õ��ĺ����������� 
	
	for(int i=0;i<=high+1;i++){	//ע��˴����˵��ںţ���ΪҪ���ñ߿� 
		for(int j=0;j<=width;j++){
			if(i==ball_x&&j==ball_y)	printf("O");
			else if(i==high+1)	printf("~");
			else if(j==width)	printf("|");
			else if(i==high&&j>=left&&j<=right)	printf("*");
			else if(produceblock(i,j))	printf("#");//��ʾһ��ש 
			else printf(" ");
		}
		printf("\n");//ע�⻻�� 
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
	if(ball_x==high-1)	{ //ע��˴�ball_x==high-1һ��Ҫ���� 
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
		if(input=='n')	ball_y--;	//nʹС������ 
		if(input=='m')	ball_y++;	//mʹС������
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
