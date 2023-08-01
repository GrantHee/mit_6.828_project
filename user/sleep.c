#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

int 
main(int argc,char* agrv[])
{
    //check the number of input agrv
    if(argc != 2)
    {
        // print error info
        write(2,"Need:sleep time(positive integer)\n",strlen("Need:sleep time(positive integer)\n"));
        exit(1);
    }
    int time = atoi(agrv[1]);
    sleep(time); //system call
    exit(0);
}