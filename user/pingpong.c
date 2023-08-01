#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

int
main(int argc,char* argv[])
{
    if(argc != 1)
    {
        write(2,"No parameters required\n",strlen("No parameters required\n"));
        exit(1);
    }
    int f[2]; // father
    int s[2]; // son
    char buffer[8];
    pipe(f);  // father to son
    pipe(s);  // son to father
    if(fork() == 0)
    {
        close(f[1]);
        close(s[0]); // in order to keep safe of pipe
        read(f[0],buffer,8);
        printf("%d: received %s\n",getpid(),buffer);
        write(s[1],"pong",strlen("pong"));

    }
    else
    {
        close(f[0]);
        close(s[1]);
        write(f[1],"ping",strlen("ping"));
        read(s[0],buffer,8);
        printf("%d: received %s\n",getpid(),buffer);
    }
    exit(0);
}