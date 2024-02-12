//Savona Shrestha

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Variables
clock_t start, end;
double e_time;
int *arr, count = 0;
FILE *fileptr;
pthread_mutex_t mutex;

//structure to define range
struct calculate{
    int start;
    int end;
};
//function to check if a num is prime 
int check_prime(int n, int i){
    if (n <= 2)
        return (n == 2) ? 1 : 0;
    if (n % i == 0)
        return 0;
    if (i * i > n)
        return 1;
    return check_prime(n, i + 1);
}
//function to find prime num in range
void *f_prime(void *ptr){
    struct calculate *arg = ptr;
    int s = arg->start, e = arg->end;
    int i, j;
    for (i = s; i <= e; i++){
        int num = *(arr + i);
        if (check_prime(num, 2) == 1){
        	//lock mutex before updating
            pthread_mutex_lock(&mutex);
            count++;
            fprintf(fileptr, "%d\n", num);
            pthread_mutex_unlock(&mutex); //unlock mutex after updating
        }
    }
}

void main()
{
    int i, n = 0, j = 0, num_file = 3;
    FILE *fptr;
    char filename[15], ch;
    int nThreads;
    //count total lines from all files
    for (i = 1; i <= num_file; i++)
    {
        sprintf(filename, "PrimeData%d.txt", i);
        fptr = fopen(filename, "r");
        ch = getc(fptr);
        while (ch != EOF)
        {
            if (ch == '\n')
            {
                if (ch != ' ')
                    n++;
            }
            ch = getc(fptr);
        }
        fclose(fptr);
    }

    printf("Total lines in all files is:(%d).\nUse multithredding to make process faster.", n);
    printf("\nEnter the number of threads: ");
    scanf("%d", &nThreads);
    
    //array to store numbers
    arr = (int *)malloc(n * sizeof(int));
    for (i = 1; i <= num_file; i++){
    	//read num from files
        sprintf(filename, "PrimeData%d.txt", i);
        fptr = fopen(filename, "r");

        while (!feof(fptr))
        {
            fscanf(fptr, "%d\n", (arr + j));
            j++;
        }
        fclose(fptr);
    }

    int sliceList[nThreads];
    for (i = 0; i < nThreads; i++)
        sliceList[i] = n / nThreads;
        
    int reminder = n % nThreads;
    for (i = 0; i < reminder; i++)
        sliceList[i]++;
    //calculate start and end
    int start_l[nThreads], end_l[nThreads];
    for (i = 0; i < nThreads; i++)
    {
        if (i == 0)
            start_l[i] = 0;
        else
            start_l[i] = end_l[i - 1] + 1;
        end_l[i] = start_l[i] + sliceList[i] - 1;
    }
    
    fileptr = fopen("Prime_Numbers.txt", "w");
    
    start = clock();
    pthread_mutex_init(&mutex, NULL);
    //create threads
    pthread_t threads[nThreads];
    struct calculate divider[nThreads];
    for (i = 0; i < nThreads; i++)
    {
        divider[i].start = start_l[i];
        divider[i].end = end_l[i];
        pthread_create(&threads[i], NULL, f_prime, &divider[i]);
    }

    for (i = 0; i < nThreads; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mutex);
    end = clock();
    //calculate time
    e_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Total Time taken = %lfs\n", e_time);

    printf("Total Prime Count: %d\n", count);
    fprintf(fptr, "There are total of %d prime numbers.", count);
    free(arr);
    printf("Prime Numbers has been saved in new file 'Prime_Numbers.txt'\n");
    fclose(fileptr);//close file
}
