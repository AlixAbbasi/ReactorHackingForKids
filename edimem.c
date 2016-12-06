/*
 * ./editmem 1234
 foo() 0x0804855b
 Monitor() 0x08048514
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>


//char code[] = "\xc9\x90\xc3\x90";
 /*
   push %ebp        ;Creating stakc
   mov  %esp,%ebp   ;Creating stakc
   ..
   ..
   leave            ;Cleanup stack
   ret              ;return to caller function
 */
 char code[] = "\x55\x90\x90\x90\x90\x90\x90\x8b\xec\x90\xc9\x90\xc3\x90";

 /*
    hook monitor() function to stop it report changed/malicious value
 */
int
hook_monitor(int pid, unsigned char *entry, unsigned char *shllcode){

    int i, res;

    res= ptrace(PTRACE_ATTACH, pid, 0, 0);//Attach to process
    if(res < 0){
        printf("Failed\n");
    }
    else{
        printf("\tDone %d\n", res);
    }
    

    printf(" [+] Injecting code ");
    /*
        Loop to read our shllcode and patch the target process
    */
    for (i=0; i<strlen(shllcode); i++){
        res=ptrace (PTRACE_POKETEXT, pid, entry + i, *(int*) (shllcode + i));
        //printf("\t0x%x %x\n", entry+i, *(int*) (shllcode+i));
    }

    if(res < 0){
        printf(" FAILED%d \n", res);
    }
    else{
        printf(" Done\n");
    }
    //Detach from target process and leave him continue....
    ptrace(PTRACE_DETACH,pid,0,0);

    return 0;
}

//help function :D
int 
f1(){
    printf (" [-] Usage: ./editmem <pid>\n");
    return 0;
}

int
main(int argc, char **argv) {
        int dangerousvalue;
        int i, c, size, pid, res, res2= 0;
        struct user_regs_struct reg;
        char *buff;
        //long ins;
        if(argc < 2){
            f1();
            exit(1);
        }
        dangerousvalue == strtol(argv[2], NULL, 16));
        pid= atoi( argv[1] );

        size= sizeof( code );
        buff= (char*)malloc( size );
        memset( buff, 0x0, size);
        memcpy( buff, code, sizeof(code) );

        //0x08048514 is address of monitor() function in memory,
        //its hardcoded, but of course we should find all addresses at runtime.
        if(argv[2]){
            printf(" [+] Preparing hooks");
            hook_monitor(pid, 0x08048514, code);
        }
        
 		
 		printf(" [+] Attaching to %d \n", pid);
        ptrace(PTRACE_ATTACH, pid, 0, 0);//attach to target process
        wait( (int*)0 );
 
        ptrace(PTRACE_GETREGS, pid, 0, &reg);//get registers
        printf(" [+] EIP @ 0x%.8x, process %d\n", reg.eip, pid);

        //Read current value from memory addr
        res2= ptrace(PTRACE_PEEKDATA, pid, 0x804a02c, NULL);
        if(res > 0){
            printf(" [+] Current value is: %d\n", res2);
        }
        else{
            printf(" [-] FAILED to read value\n");
        }

        //Overwrite our value to memory addr, change it to 5
        res= ptrace(PTRACE_POKEDATA, pid, 0x804a02c, 0x00000005);
        if(res < 0){
            printf(" [-] FAILED: %d \n", res);
        }
        else{
            printf(" [+] Changed\n");
        }

        ptrace(PTRACE_DETACH,pid,0,0);//detach from target process
        printf(" [+] Deattaching from %d %d\n", pid, i);
            
        free(buff);//free the allocated buffer
        printf(" [*] Done.\n");

    return 0;

}
