if(speed<ctrl)	speed++;//此处不用Sleep函数试一下 
	else if(speed==ctrl){
		enemy_x++;
		speed=0;
	}
