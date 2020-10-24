#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define buffer 10000               
typedef struct{
       int **a;
       int row;
       int col;
       int x;
       int y;
}para;

int valid[11]={0};                 // array that is visible to all 11 worker threads
                                   // if the ith worker thread thinks the solution is valid index i set to 1

void *workc(void *data){                  //function to check if all columns have all the digits
       para* dt=(para*)data;
       int ret=0;
       int** ar=(int**)dt->a;
       int roww=(int)dt->row;
       int coll=(int)dt->col;
       int i,j;
       int check[9];
       for(i=0;i<coll;i++){
              for(j=0;j<roww;j++){
                     check[j]=0;
              }
              for(j=0;j<roww;j++){
                     check[ar[j][i]-1]=1;
              }
              for(j=0;j<roww;j++){
                     if(check[j]!=1)
                            return (void*)ret;
              }
       }
       ret=1;
       return (void *)ret;
}

void *workr(void *data){                  //function to check if all rows have all the digits
       para* dt=(para*)data;
       int ret=0;
       int** ar=(int**)dt->a;
       int roww=(int)dt->row;
       int coll=(int)dt->col;
       int i,j;
       int check[9];
       for(i=0;i<roww;i++){
              for(j=0;j<coll;j++){
                     check[j]=0;
              }
              for(j=0;j<coll;j++){
                     check[ar[i][j]-1]=1;
              }
              for(j=0;j<coll;j++){
                     if(check[j]!=1)
                     {return (void*)ret;}
              }
       }
       ret=1;
       return (void *)ret;
}

void *workgrid(void *data){               //function to check if the subgrid is valid
       para *dt=(para*)data;
       int ret=0;
       int** ar=(int**)dt->a;
       int roww=(int)dt->row;
       int coll=(int)dt->col;
       roww=roww/3;
       coll=coll/3;
       int i,j;
       int check[9]={0};
       for(i=0;i<roww;i++){
              for(j=0;j<coll;j++){
                     check[ar[dt->y+i][dt->x+j]-1]=1;
              }
       }
       for(i=0;i<9;i++){
              if(check[i]!=1)
                     return (void*)ret;
       }
       ret=1;
       return (void *)ret;
}

int main(int argc,char*argv[]){
       if(argc<2 || argc>2){
              printf("Incorrect number of arguments\n");
              return -1;
       }
      int r,c,i,j,cnt;
      para *data;
      data=(para*)malloc(sizeof(para));
      r=9;    //total rows
      c=9;          //total columns
      data->row=r;
      data->col=c;
      pthread_t tid[11];           //11 thread ids
      pthread_attr_t attr;
      pthread_attr_init(&attr);           //common set of attributes initialised
      int **arr;                          //This matrix will store the Sudoku as an integer matrix
      arr=(int**)malloc(r*sizeof(int*));         
      for(i=0;i<r;i++){
             arr[i]=(int*)malloc(c*sizeof(int));
      }
      char d[buffer];
      int temp;
      FILE *fptr;
      if((fptr=fopen(argv[1],"r"))==NULL){
             printf("Error");
             exit(1);
      }
      j=0;
      cnt=0;
      while(fgets(d,buffer,fptr)!=NULL){
             for(i=0;i<=strlen(d);i++){
                    temp=(int)(d[i]-'0') ;
                   if(temp<=9 && temp >=0)
                     {arr[cnt][j]=temp;
                     j++;
                     }
             }
             cnt++;
             j=0;
      }
      data->a=arr;
      int status;
      pthread_create(&tid[0],&attr,workc,data);         //thread 0 assigned to check columns
      pthread_create(&tid[1],&attr,workr,data);         //thread 1 assigned to check rows
       cnt=2;
       for(i=0;i<r;i=i+3){
              data->y=i;
              for(j=0;j<c;j=j+3){
                     data->x=j;
                     pthread_create(&tid[cnt],&attr,workgrid,data);          //thread 2-10 assigned to check individual subgrids
                     cnt++;
              }
       }
       for(i=0;i<11;i++){
              pthread_join(tid[i],&valid[i]);                  //waiting for all threads, and updating the array with their results
       }
      for(i=0;i<11;i++){
             if(valid[i]==0)                           //if one of them is 0 then the solution is invalid
              {
                     printf("Not valid\n");
                     fclose(fptr);
                     return 0;
              }
      }
      printf("Valid\n");           //if not then valid
      fclose(fptr);
      return 0;
}