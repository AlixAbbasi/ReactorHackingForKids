/*
Value addr: 0x804a02c
*/
#include <stdio.h>
#include <dlfcn.h>

int Value= 1;

/*
	Writing status to monitor.out, 
	'monitor' program will read later from this file for current status
*/
int
Monitor(int val){
	FILE *F;
	F= fopen("monitor.out", "w");
		fprintf(F, "%d", val);
	fclose(F);
}
/*
	This is where our software read value from memory!
*/
void Logic()
{
 	if(Value > 1){
 		printf("Current Status is: %c[1;31m |||||||| BOOOOOOOM!!! YOU FUCKED UP! %d\n", 
 					27, Value);	
 	}
 	printf("%c[0mLogic\n", 27);
}

int main()
{
	int i ;

 	printf("//My PID is: PID:%d PPID:%d Value:%p \n\n\n", getpid(), getppid(), &Value);

	for(i=0; i < 10; i++){

 		if(Value == 1){
	 		printf("Current Status is: %c[1;32m |||||||| Everything is ok, Life is good!!%d\n", 
 						27, Value);
 			printf("%c[0m", 27);
 	}
	getchar(); 

	Logic();
	Monitor(Value);//Writing latest status to files
}
	//to see if after the patch program can continue running and return to main function and then OS.s
	printf("%c[0mReturned\n", 27);
 return 0;
}
