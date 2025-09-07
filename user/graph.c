#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
    > I finished this before it was taken out of the requirements
    > Forks three processes that all enter infinite loops
    > Each loop uses 100000 by 1000 2D array to slow down 
        print statements in the terminal
    > It's meant to be exited via the 'ctrl a x' command
*/

void main()
{
    int pid1, pid2, pid3,pid,prio;
    if((pid1 = fork()) == 0 )
    {
        pid = getpid();
        setPriority(-20);
        setEffectivePriority(pid,-20);
        prio = -20;
        for( ; ; )
        {
            
            for(int i =0;i<100000 ;i++ )
            {
                for(int j = 0; j<1000;j++)
                {
                    
                }
                if(i == 99999)
                {
                    printf("%d\n",prio);
                }
            }
        }
        
    }
    if((pid2 = fork()) == 0 )
    {
        pid = getpid();
        setPriority(0);
        setEffectivePriority(pid,0);
        prio = 0;
        for( ; ; )
        {
            for(int i = 0 ;i<100000 ;i++ )
            {
                for(int j = 0; j<1000;j++)
                {

                }
                if(i == 99999)
                {
                    printf("%d\n",prio);
                }
            }
        }
    }
    if((pid3 = fork()) == 0 )
    {
        pid = getpid();
        setPriority(20);
        setEffectivePriority(pid,20);
        prio = 20;
        for( ; ; )
        {
            for(int i = 0 ;i<100000 ;i++ )
            {
                for(int j = 0; j<1000;j++)
                {
                    
                }
                if(i == 99999)
                {
                    printf("%d\n",prio);
                }
            }
        }
    }
}
