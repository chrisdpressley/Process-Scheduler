#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "user/user.h"

#define NPROC 64
/*
    Prints the current processes inside the process table
    getpinfo() is used to get process data from the kernel
*/
void main()
{
    char *array[6] = {"UNUSED  ","USED    ","SLEEPING","RUNNABLE","RUNNING ","ZOMBIE  "};   //Used for display of enum
    struct pstat *p = malloc(sizeof(struct pstat));
    getpinfo(p);
    printf("NAME\t\tPID\t\tSTATUS\t\tPRIORITY\n");
    for(int i = 0;i < NPROC;i++)
    {
        if(p->state[i] == 0)
        {
           break;
        }
        printf("%s\t\t%d\t\t%s\t%d\n",p->name[i],p->pid[i],array[p->state[i]],p->real_priority[i]);
    }
}