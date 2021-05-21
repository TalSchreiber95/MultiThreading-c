#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>
// #include <sys/time.h>
#include <semaphore.h>
#define NUM_T 1
#define NUM_T_L 1
// pthread_t* tid;
pthread_mutex_t lock;
long sum = 0;
long primeCounter = 0;
long numOfRandomNumbers;

int randomPivot;
long Thread_num;
int x;

int isPrime(int num)
{
    if (num == 0 || num == 1)
    {
        return 0;
    }

    if ((!(num%6==1||num%6==5))||num%5==0)
    {
        return 0;
    }

    int i;
    int r = (int)sqrt((double)num);
    for (i = 7; i <= r; i +=6)
    {
     
    
        if ((num % i == 0 ||(num % (i + 4) == 0)))
        {
            return 0;
        }
    }
    pthread_mutex_lock(&lock);
    sum = sum + num;
    primeCounter++;
    pthread_mutex_unlock(&lock);
    return 1;
}




void *sum_count_prime(void *arg)
{
    int num = 0;
    for (int i = 0; i < x; i++)
    {
        /* code */
        num = rand();
        isPrime(num);
    }
}

int main(int argc, char *argv[])
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("erro");
        return 1;
    }

    if (argc != 3)
    {
        printf("Too few arguments ");
        printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
        exit(0);
    }
    int e;
    int randomPivot = atoi(argv[1]);
    numOfRandomNumbers = atoi(argv[2]);

    Thread_num = get_nprocs_conf();

    pthread_t tid[Thread_num];

    int random1 = rand();
    srand(randomPivot);

    x = numOfRandomNumbers / (Thread_num * NUM_T);

    for (int i = 0; i < Thread_num * NUM_T; i++)
    {

        e = pthread_create(&(tid[i]), NULL, &sum_count_prime, NULL);
        //check if the number is prime
        if (e != 0)
        {
            printf("erro");
        }
    }

    for (int i = 0; i < Thread_num * NUM_T; i++)
    {
        pthread_join(tid[i], NULL);
    }

    //keep the out format as this!!
    printf("%ld,%ld\n", sum, primeCounter);
    pthread_mutex_destroy(&lock);
    
    exit(0);
}