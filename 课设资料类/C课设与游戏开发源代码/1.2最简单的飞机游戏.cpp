#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
int main(){
	int x=5;
	int n=0;
	int y=10;
	char input;
	int fire=0;
	int ny=10;
	int kill=0; 

	while(1){
		system("cls");
		printf("score:%3d\n",n);
		if(kill==0){
			for(int i=0;i<ny;i++){
				printf(" ");
			}
			printf("+\n");
			Sleep(50);//放慢靶子速度 
		}
		ny++;
		if(ny==50)	ny=10;
		
		if(fire==0){
			for(int i=0;i<x;i++)
				printf("\n");
		}else
			if(fire==1){
				for(int i=0;i<x;i++){
					for(int j=0;j<y;j++)
						printf(" ");
			 		printf("  |\n");
			}
			
			if(y+2==ny&&kill==0)//此处要注意，应把Fire设置为1是才能打掉把子,所以把它套在里面 
				{
					kill=1;
					n++;
				}
				
			fire=0;
		}
		
		//设计飞机图形 
		for(int i=0;i<y;i++){
			printf(" ");
		}
		printf("  *\n");
		for(int i=0;i<y;i++){
			printf(" ");		}
		printf("*****\n");
		for(int i=0;i<y;i++){
			printf(" ");
		}
		printf(" * * \n"); 
		
		
		if(kbhit()){
			input=getch();
			switch(input){
				case 'w':x--;break;
				case 's':x++;break;
				case 'a':y--;break;
				case 'd':y++;break;
				default:;
			}
			if(input==' ')	{
				fire=1;
				printf("\a");//加音效 
			}
		}
	}
	return  0;
} 
