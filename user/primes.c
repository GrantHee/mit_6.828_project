#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"stddef.h"

void redirect(int n,int p[])
{
    close(n);  // n can only be 0 or 1, 0 present read,1 present write
    dup(p[n]);
    close(p[0]);
    close(p[1]);
}

void prime()
{
    int pp[2];
    int first,next;
    if(read(0,&first,sizeof(int)))
    {
        printf("prime %d\n",first);
        pipe(pp); // create a pipeline
        if(fork() == 0)
        {
            //child process redirect write to 1
            redirect(1,pp);
            while(read(0,&next,sizeof(int)))
            {
                if(next%first != 0)
                {
                    write(1,&next,sizeof(int));

                }
            }
        }else
        {
            // father process
            wait(NULL);
            redirect(0,pp);   //redirect IO to this pipeline instead of the last one
            prime();
        }
    }
}

int
main(int argc,char*argv[])
{
    if(argc != 1)
    {
        write(2,"No parameters required\n",sizeof("No parameters required\n"));
        exit(1);
    }
    int p[2];
    pipe(p);
    if(fork() == 0)
    {
        redirect(1,p);
        for(int i = 2;i<=35;i++)
        {
            write(1,&i,sizeof(int));    //child process will read from 1
        }
    }
    else
    {
        wait(NULL);
        redirect(0,p);
        prime();
    }
    exit(0);
}