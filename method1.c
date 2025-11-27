
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h>
#include <sys/time.h>//this library needed for calculating time
typedef struct segment{
  long start;
  long end;
  long* array;
  long id;
}segment; //this structure is useful for sending arguments when we are creating threads
void merge(long l,long m,long h,long a[h-l+2])//this function will merge two parts of sorted array and merged array will also be sorted
{    
     long k=0,t,c[h-l+100],j=m+1,temp,i=l;
     while(1){
        t=j;
      	if(a[i]>=a[j])
      	{
      		c[k]=a[j];
      		j++;
      	} 
      	if(a[i]<a[t])
      	{
      		c[k]=a[i];
      		i++;
      	}
        k++;
      	if (i==m+1||j==h+1)
      	     break;
     }
     temp=j;
     if (i==m+1){ 
      	while(j!=h+1){
      		c[k]=a[j];
      		k++;
      		j++;
      	} 
     }
     j=temp;
     if (j==h+1)
     {
      	while(i!=m+1){
      	    c[k]=a[i];
      	    k++;
      	    i++;
      	}
     }
     for ( i = l; i <=h; ++i)
      	a[i]=c[i-l];
}
void sort(long l,long h,long a[h-l+2])//this function is for sorting array
{
  if(l>=h)
      return ;
  long m=(l+h)/2;
  sort(l,m,a);
  sort(m+1,h,a);
  merge(l,m,h,a);
}
void* enter(void* se){//the threads which we created for purpose of sorting will  execute this function
   segment* seg=(segment*)se;   
   sort(seg->start,seg->end,seg->array);   
}
long power(long x){//this function will return 2^x value
  long i,y=1;
  for(i=0;i<x;i++)
      y=2*y;
  return y;     
}
void print(long i,long j,long array[j-i+10],FILE *p){//this function is for printing array
   long k;
   for(k=i;k<=j;k++)
      fprintf(p,"%ld ",array[k]);
   fprintf(p,"\n");
}  
int main(int argc, char *argv[])
{ 
  FILE *fptr;
  if ((fptr = fopen("inp.txt","r")) == NULL){
       printf("Error! opening file");
       exit(1);
  }
  long sizeofarray,noofthreads,i,sizeofseg,j,k,rc,time_mult,diff;
  fscanf(fptr,"%ld",&sizeofarray);
  fscanf(fptr,"%ld",&noofthreads);
  fclose(fptr);
  noofthreads=power(noofthreads);//noofthreads=2^noofthreads
  sizeofarray=power(sizeofarray);//sizeofarray=2^sizeofarray
  sizeofseg=(sizeofarray)/(noofthreads);//variable sizeofseg denotes length of each segment
  long array[sizeofarray]; 
  srand(time(0)); 
  for(i=0;i<sizeofarray;i++){//assigning random values for array
      array[i]=rand();
  }
  segment* tseg[noofthreads];//tseg array is for storing adress of segment variables
  struct timeval start1;
  struct timeval end1;
  fptr = fopen("Output.txt","w");
  if(fptr == NULL)
  {
      printf("Error!");   
      exit(1);             
  }
  print(0,sizeofarray-1,array,fptr);    
  gettimeofday(&start1, NULL);//timer starts
  for(k=0;k<noofthreads;k++){
    tseg[k]=(segment*)malloc(sizeof(segment));//i am allocating memory and storing all adressess of memory in tseg array
  }
  pthread_t t[noofthreads];
  i=0;
  j=sizeofseg-1;
  k=0;  
  for(k=0;k<noofthreads;k++){
     tseg[k]->start=i;//staring location of array of (k+1)th segment will be stored 
     tseg[k]->end=j;//ending location of array of (k+1)th segment will be stored
     tseg[k]->array=array;//adress of array will be stored
     tseg[k]->id=k; //thread id will be stored
     pthread_create(&t[k],NULL,enter,tseg[k]);//thread will be created and enter function will be executed
     i=i+sizeofseg;//here we are moving to another segment so starting location is updated
     j=j+sizeofseg;//ending location will be updated
  }
  for(i=0;i<noofthreads;i++){//waiting for all slave threaeds to finish their tasks
    pthread_join(t[i],NULL);
  }
  ///////at this moment all slave threads finished their job and all segments are sorted
  long tempsegment=noofthreads;
  j=sizeofseg-1;
  k=2*j+1;
  while(tempsegment>1){//we will run this loop till whole array is sorted
    
    merge(0,j,k,array);//we will merge 0th to jth part of array and (j+1)th to kth part of array
    //after executing merge function array from 0th to kth part will be sorted 
    j=k; //j will be updated because we were including another segment in merging 
    k+=sizeofseg;//k will be updated because we were including another segment in merging   
    tempsegment--;
  }
  for(k=0;k<noofthreads;k++){
         free(tseg[k]); //releasing allocated memory
  }
  gettimeofday(&end1, NULL);//timer ends
  time_mult=(end1.tv_sec*1000000+end1.tv_usec)-(start1.tv_sec*1000000+start1.tv_usec);
  print(0,sizeofarray-1,array,fptr);
  fprintf(fptr,"Time taken : %ld Microseconds.\n",time_mult);
  fclose(fptr);
  return 0;
}
