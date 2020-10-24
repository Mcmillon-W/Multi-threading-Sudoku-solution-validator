#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_PID 5000               //Can try out different range of PID //Try 500 here to see and input very high number of threads too see that the threads wait
#define MIN_PID 300                                                                                                                  //for PID to get free

int s=MAX_PID-MIN_PID+1;
int latest;

pid_t pid_arr[MAX_PID-MIN_PID+1];

int allocate_map(){
       int i;                                                         // Initiallising the PID array which keeps track of which PIDs are free to allocate
       for(i=0;i<s;i++)
              pid_arr[i]=0;                                    //0 indicates free PID slot
       latest=-1;  //to show no pids have been allocated yet initially
       if(pid_arr[s-1]==0)
              return 1;
       else
              return -1;
       
}

int allocate_pid(){
       int i=0;
       int next,pnext;             // To store where the free PID slot might be
       if(latest==s-1)
       next=0;
       else
       next=latest+1;
       pnext=next;                 //To store the initial potential free PID slot
       while(pid_arr[next]!=0){           //until we allocate PID the thread will stay in this function
              next=pnext;
       while(i<s && pid_arr[next]==1){
              i++;
              next++;
              if(next>s-1)
                     next=0;
       }
       if(i>=s)
              {
              printf("Waiting for PID to be released\n");      //The thread will wait till one of the other threads releases its pid
              sleep(1);            //wait for 1 second, for a PID to be free
              i=0;                        //Start over and see if any PIDs are free
              }
       }
       pid_arr[next]=1;                   //PID assigned
       latest=next;
       return latest +300;
}

void release_pid(int pid){
       if(pid_arr[pid-MIN_PID]==0){
              printf("PID not in use\n");
              return;
       }
       pid_arr[pid-MIN_PID]=0;                          //PID freed
       printf("PID %d released ,Process terminated\n",pid);
}

void *work(void *duration)
{      int id;
       clock_t slept,ex;
       double time_exe,time_sleep;
       int i=(int)duration;
       srand(time(0));
       ex=clock();              //Start the clock for seeing how long it takes to execute
       id=allocate_pid();          //Allocating pid to the thread
       printf("PID %d allocated\n",id);
       slept=clock();        //Start the clock too see how long the thread slept
       sleep((double)rand()/(double)RAND_MAX*1);        //Random Sleeping time between 0 to 1 seconds if *1 , Sleeping time between 0 to 100 if *100, Can try other values as well
       release_pid(id);                   //Done working so release PID
       slept=clock()-slept;   //Stop the clock too see its sleeping time
       ex=clock()-ex;                        //Stop the clock for seeing how long it taken to execute
       printf("The Thread %d got %d as its pid and took %fs to execute out of which %fs was sleeping time\n",i,id,(double)ex/CLOCKS_PER_SEC,(double)slept/CLOCKS_PER_SEC);
       //The thread has finish its execution and is terminated
       pthread_exit(0);
}

int main(){
       printf("Allocation of the required pids:\n");
       if(allocate_map()==1)                                   //First initialize the array of PIDs
              printf("Successful\n");
       else
              {printf("Failed\n");
              return 0;
              }
       int p;
       printf("Number of threads to be created:\n");
       scanf("%d",&p);                    //User input of how many threads exist
       pthread_t tid;
       pthread_attr_t attr;
       pthread_attr_init(&attr);
       int i;
       for(i=0;i<p;i++){       
       pthread_create(&tid,&attr,work,(void *)i);              // Creating threads
       pthread_detach(tid);                             //As soon as the thread is done sleeping, automatically results are yield .i.e PID is released
       }                                                      //Allocation and releasing is done simultaneously within this loop
      pthread_exit(0);
}