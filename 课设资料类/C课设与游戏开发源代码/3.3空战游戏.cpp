#include<stdio.h>
#include<stdlib.h> 
#include<conio.h>
#include<windows.h>
#include<time.h>

#define high 25
#define width 50
#define Enemynum 5

int position_x,position_y;	//我机位置 
int canvas[high][width]={0};//0为空格，1为飞机*,2为子弹|，3为敌机@
int enemy_x[Enemynum],enemy_y[Enemynum];
int Bulletwidth;
int Enemyspeed;
int score;


void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void startup(){
	position_x=high/2;
	position_y=width/2;
	canvas[position_x][position_y]=1;
	Bulletwidth=0;
	Enemyspeed=20;
	
	for(int k=0;k<Enemynum;k++){
		enemy_x[k]=rand()%2;
		enemy_y[k]=rand()%width;
		canvas[enemy_x[k]][enemy_y[k]]=3;
	}
	
	score=0;
}

void show(){
	gotoxy(0,0);
	
	for(int i=0;i<high;i++){
		for(int j=0;j<width;j++){
			if(canvas[i][j]==1)	printf("*");
			else if(canvas[i][j]==2)	printf("|");
			else if(canvas[i][j]==3)	printf("@"); 
			else printf(" ");
		}
		printf("\n");
	}
	printf("Your Score:%3d\n",score);
	Sleep(20);
}

void updateWithoutinput(){
	void end();
	srand((unsigned)time(NULL));
	
	for(int i=0;i<high;i++){	//在整个画布范围内搜索 
		for(int j=0;j<width;j++){
			if(canvas[i][j]==2){
				for(int k=0;k<Enemynum;k++){
					if(i==enemy_x[k]&&j==enemy_y[k]){
						score++;
						if(score!=0&&score%5==0&&Enemyspeed>5)	Enemyspeed--;
						if(score%5==0&&Bulletwidth<=6)	Bulletwidth++;
						printf("\a");
						canvas[enemy_x[k]][enemy_y[k]]=0;
						enemy_x[k]=rand()%2;		//敌机再生 
						enemy_y[k]=rand()%width;
						canvas[enemy_x[k]][enemy_y[k]]=3;
						canvas[i][j]=0;		//子弹消失 
					}
				}
					canvas[i][j]=0;	//使子弹消失 
					if(i>0)	canvas[i-1][j]=2;	//子弹上移 	
			}
		}
	}
	//防止战斗机出边界 
	if(position_x<0)	{
		canvas[position_x][position_y]=0;
		position_x=0;
		canvas[position_x][position_y]=1;
	}
	if(position_x>high-1)	{
		canvas[position_x][position_y]=0;
		position_x=high;
		canvas[position_x][position_y]=1;
	}
	if(position_y>width-1){
		canvas[position_x][position_y]=0;
		position_y=width-1;
		canvas[position_x][position_y]=1;
	}
	if(position_y<0){
		canvas[position_x][position_y]=0;
		position_y=0;
		canvas[position_x][position_y]=1;
	}
	
	static int speed=0;
	if(speed<Enemyspeed)
		speed++;
	
	for(int k=0;k<Enemynum;k++){
		if(position_x==enemy_x[k]&&position_y==enemy_y[k]){
			atexit(end);
			system("pause");
			exit(0);	//表示正常退出 
		}
		
		if(enemy_x[k]>=high){
			canvas[enemy_x[k]][enemy_y[k]]=0;
			enemy_x[k]=rand()%2;
			enemy_y[k]=rand()%width;
			canvas[enemy_x[k]][enemy_y[k]]=3;
			score--;	//放走敌机减分 
		}
		
		
		if(speed==Enemyspeed){
			speed=0;
			for(int k=0;k<Enemynum;k++){
				canvas[enemy_x[k]][enemy_y[k]]=0;
				enemy_x[k]++;
				canvas[enemy_x[k]][enemy_y[k]]=3;
			}
		}
	}	
}

void updateWithinput(){
	char input;
	if(kbhit()){
		input=getch();
		switch(input){//此处一定要注意飞机位置的更新 
			case 'a':canvas[position_x][position_y]=0;position_y--;canvas[position_x][position_y]=1;break;
			case 'd':canvas[position_x][position_y]=0;position_y++;canvas[position_x][position_y]=1;break;
			case 'w':canvas[position_x][position_y]=0;position_x--;canvas[position_x][position_y]=1;break;
			case 's':canvas[position_x][position_y]=0;position_x++;canvas[position_x][position_y]=1;break;
			case ' ':
			{
				int left=position_y-Bulletwidth;
				int right=position_y+Bulletwidth;
				if(left<0)	left=0;
				if(right>width-1)	right=width-1;
				for(int k=left;k<=right;k++){
					canvas[position_x-1][k]=2;
				}
				break;
			}
		}
	}
}

void end(){
	printf("You plane is destroyed by the enemy!\n");
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
