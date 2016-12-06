#include <stdio.h>
/*
	Reading from monitor.out which include current status.
*/
int
Monitor(){
	FILE *F;
	char ch;

	F= fopen("monitor.out", "r");
		fread(&ch, sizeof(ch), 1, F);
		printf("Im the operator on my SCADA everything is fine... :-)\nValue: %c\n", ch);
	fclose(F);
}

int 
main(int argc, char *argv[]){

	//Cleaning terminal
	printf("\033[2J\033[1;1H");
	//Infinite loop to read new status and show it on screen
	while(1){
		Monitor();
		sleep(1);
		printf("\033[2J\033[1;1H");
	}

	return 0;
}
