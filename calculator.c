#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
//Mert Türker 280201097
typedef struct data{
    float* arr;
    int thread_num;
    int num_term;
    int start_index;
} data;


void calculate(int x,int y ,int arrx[]){//x equals to total term y equals to thread number
    int i;
    
    int z = x/y;
    
    
    
    if(x != y*z){//if not equally divisble condition
        for ( i = 0; i < x-(y*z); i++)
        {
            arrx[i]= z+1;//fill the array with biggest portion
        }
        for ( ; i < y; i++)
        {
            arrx[i] = z;
        }    
    }
    else
    {
        for ( i = 0; i < y; i++)
        {
            arrx[i] = z;
        }
        }
        
    
    
}
void* pisum(void* p){
    data* ptr = (data*)p;
    int n = ptr->thread_num;
    int startIdx = ptr->start_index;
    int numOfTerms = ptr->num_term;
    // Declare sum dynamically to return to join:
    float* thread_sum = (float*) calloc(1, sizeof(float));
    printf("Thread %d: [%d, %d]. thread \n ",n,startIdx,numOfTerms+startIdx);

    
    for(int i = startIdx ; i < startIdx+numOfTerms; i++){
        thread_sum[0] = thread_sum[0] + ptr->arr[i];
    }
    
    
    pthread_exit(thread_sum);
}

int main(int argc, char** argv){
    
    
    // It is good to check if the arguments look okay:
    if (argc != 3) {
    printf("Usage: %s a b\n", argv[0]);
    printf("such that a and b are integers.\n");
    return 1;
    }

    int threadCount = atoi(argv[1]); // parse the string

    int termCount = atoi(argv[2]);
    
    printf("%d threads will work together to calculate pi (using %d terms) \n"
    , threadCount, termCount);

    
    float x = -1.0;
    float y = 0.0;
    float* int_arr = (float*) calloc(termCount, sizeof(float));
    int_arr[0] = 1;
    for(int i = 1; i < termCount; i++){
        y++;
        int_arr[i] = ( 1 / (2*y + 1) ) * x;
        
        x= x*(-1);
    }
     for(int i = 0; i < termCount; i++){
        
        //printf("%f ",int_arr[i]); 
        
    }
    int arrx[termCount];       
    calculate(termCount,threadCount,arrx);
    
    
    // Declare arguments for all threads:
    data thread_data[threadCount];
    int m,total = 0;
    
    while(m<threadCount){
        
        thread_data[m].thread_num = m;
        thread_data[m].arr = int_arr;
        thread_data[m].num_term = arrx[m];//more workload for threads at the start
        thread_data[m].start_index=total;
        total = total + arrx[m];
        m++;
    }
    
    
    // Declare thread IDs:
    pthread_t tid[threadCount];
    
    // Start all threads:
    int j =0;
    while(j < threadCount){
        
        pthread_create(&tid[j], NULL, pisum, (void*)&thread_data[j]);

        
        j++;
    }
    
    // Declare space for sum:
    
    
   
    float* sum;
    float* approximation = (float*) calloc(1, sizeof(float));
    
    for (int i = 0; i < threadCount; i++)
    {
        pthread_join(tid[i], (void**)&sum);
        
        *approximation += *sum;
    }
    
   
    
        
    
    printf("Pi approximation:  %f\n", 4 * (*approximation ));
     
    return 0;
}
