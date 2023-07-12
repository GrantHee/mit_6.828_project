#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/param.h"

int 
main(int argc,char*argv[])
{
    if(argc<2)
    {
        write(2,"xargs need more parameters",strlen("xargs need more parameters"));
        exit(1);
    }
    char* parameters[MAXARG];    //store the num of parameters
    char* p;             
    char block[32],buf[32];
    p = buf;
    int count = 0;               //work as a index in the buffer
    int m = 0;                   // work as a index in the while loop
    for(int i =1; i <argc;i++)
    {
        parameters[count] = argv[i];
        count++;                         //transfer the data intp the parameter array
    }

     // read data from the standard io
    int n;
    while((n = read(0,block,sizeof(block)))>0)
    {
        for(int i =0;i<n;i++)
        {
            if(block[i] == '\n')
            {
                buf[m] = 0;      //set it as end of the string
                parameters[count++]=p;
                parameters[count]= 0;   //as the end of parameter list
                m =0;                 //reset
                p = buf;
                count = argc-1;
                if(fork()==0)
                {
                    exec(argv[1],parameters);   //execute the command
                }
                wait(0);
            }else if(block[i] == ' ')
            {
                buf[m++] = 0;
                parameters[count++]=p;
                p =&buf[m];
            }else
            {
                buf[m++]=block[i];
            }
        }
    }
    exit(0);

}
