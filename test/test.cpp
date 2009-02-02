#include <srllib.h>
#include <dxxxlib.h>
#include <stdio.h>
void main(){
	int BoardCnt = 0;
	int bddev = 0;
	char boardName[20];
	char chanName[20];
	
	if( sr_getboardcnt(DEV_CLASS_VOICE,&BoardCnt) == -1 ){
		printf("sr_getboardcnt() Error\n");
		return ;
	}else{
		printf("The number of boards are: %d\n",BoardCnt);
	}

	for (int brd=1; brd<=BoardCnt; brd++){
		sprintf(boardName,"dxxxB%d",brd);//格式化语音卡名称
		printf("Board name: %s\n",boardName);
		if( (bddev = dx_open(boardName,0)) == -1 ) continue;//打开语音卡
		
		int subDeviceSum = ATDV_SUBDEVS(bddev);//board 子设备数
		for ( int chCnt =1; chCnt<=subDeviceSum; chCnt++ ){
			sprintf(chanName,"%sC%d",boardName,chCnt);
			printf("%s ---> %s\n",boardName,chanName);
		}
		dx_close(bddev);
	}
}