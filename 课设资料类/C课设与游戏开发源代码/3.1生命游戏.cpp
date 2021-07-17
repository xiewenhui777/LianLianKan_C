#include<stdio.h>
#include<conio.h>
#include<windows.h>

#define high 25
#define width 50

int cells[high][width];

void gotoxy(int x,int y){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}

void startup(){	
	
	for(int i=0;i<=high-1;i++){
		for(int j=0;j<=width-1;j++){
			cells[i][j]=1;
		}
	}
}

void show(){
	gotoxy(0,0);
	
	for(int i=1;i<high-1;i++){
		for(int j=1;j<width-1;j++){
			if(cells[i][j]==1)	printf("*");
			else printf(" ");
		}
		printf("\n");	//别忘记每行输完输一个空格 
	}
	Sleep(50);
}

void updateWithoutinput(){
	int neighbor=0;
	int newcells[high][width];
	
	for(int i=1;i<high-1;i++){
		for(int j=1;j<width-1;j++){
			neighbor=cells[i-1][j-1]+cells[i-1][j]+cells[i-1][j+1]+cells[i][j-1]+
			cells[i][j+1]+cells[i+1][j-1]+cells[i+1][j]+cells[i+1][j+1];//此处不用循环是因为循环嵌套层数过多会拖慢运行速度 
		
			if(neighbor==3)	newcells[i][j]=1;
			else if(neighbor==2)	newcells[i][j]=cells[i][j];
			else	newcells[i][j]=0; 
		}
	}
	 for(int i=1;i<high-1;i++){
	 	for(int j=0;j<width-1;j++){
	 		cells[i][j]=newcells[i][j];
		 }
	 }

}

void updateWithinput(){
	
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
