//Savona Shrestha
//2330777
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

double Pi = 0;
struct PiRange{
    int start;
    int end;
};
//function to calculate pi
void *calculate_pi(void *ptr) { 
    struct PiRange *range = (struct PiRange *)ptr;
    int start_l = range->start;
    int end_l = range->end;
    int num;
    //calculating pi
	for(num = start_l; num <= end_l; num++) {
        Pi += (((pow(-1, num)) * 4)/((2 * num) + 1));//Leibniz formula
    }
    pthread_exit(0);
}
//Main function
void main(int argc, char **argv) {
    int n_Iterations;//Number of Iterations
    int n_Threads;//Number of Threads
    int i;

    printf("Enter the number of iterations: ");
    scanf("%d", &n_Iterations);
    printf("Enter the number of threads: ");
    scanf("%d", &n_Threads);
    
    int remainder = n_Iterations % n_Threads;
    //array to store iterations assigned to each thread
    int List[n_Threads];
    for(i = 0; i < n_Threads; i++) {
        List[i] = n_Iterations / n_Threads;
    }
    for(i = 0; i < remainder; i++){
        List[i] = List[i] + 1;
    }
    for (i = 0; i < n_Threads; i++) {
        printf("%dth Thread : %d\n", i + 1, List[i]);
    }

    int start[n_Threads];
    int end[n_Threads];
    //calculate the start and end indices for each thread
    for(i = 0; i < n_Threads; i++) {
        if(i == 0) {
            start[i] = 0;
        } else {
            start[i] = end[i - 1] + 1;
        }
        end[i] = start[i] + List[i] - 1;
    }
    //store threads Id
    pthread_t threadId[n_Threads];

    struct PiRange pi_ranges[n_Threads];
    for (i = 0; i < n_Threads; i++) {
        pi_ranges[i].start = start[i];
        pi_ranges[i].end = end[i];
        pthread_create(&threadId[i], NULL, calculate_pi, &pi_ranges[i]);
    }
    //all threads to finish execution
    for(i = 0; i < n_Threads; i++) {
        pthread_join(threadId[i],NULL);
    }       
    printf("The calculated value of Pi is: %lf\n", Pi);
}
