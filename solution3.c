#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>
// #include <sys/time.h>
#include <semaphore.h>
// pthread_t* tid;

pthread_mutex_t lock;
long sum = 0;
long primeCounter = 0;
long numOfRandomNumbers;

int randomPivot;
long Thread_num;
int x,y;

void foundPrime(int num) {
    sum = sum + num;
    primeCounter++;
}
int checkPrime(int num){

   if (num == 0 || num == 1)
    {
        return 0;
    }
    if ((!(num%6==1||num%6==5))||num%5==0) // took from wiki
    {
        return 0;
    }

    for (int i = 7; i <= (int)sqrt((double)num); i +=6)
    {
        if ((num % i == 0 ||(num % (i + 4) == 0)))
        {
            return 0;
        }
    }
    return num;
}
int isPrime(int num)
{
    // checkPrime(num);
    if(checkPrime(num)) return 0;
    pthread_mutex_lock(&lock);
    foundPrime(num);
    pthread_mutex_unlock(&lock);
    return 1;
}




void *sum_count_prime1(void *arg)
{
    int num = 0;
    printf("t1\n");
    for (int i = 0; i < x+y; i++)
    {
        /* code */
        num = rand();
        isPrime(num);
    }
    return NULL;
}

void *sum_count_prime(void *arg)
{
    int num = 0;
    printf("t2\n");
    
    for (int i = 0; i < x; i++)
    {
        /* code */
        num = rand();
        isPrime(num);
    }
    return NULL;
}

// int main(int argc, char *argv[])
// {
//     if (argc != 3)
//     {
//         printf("Too few arguments ");
//         printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
//         exit(0);
//     }
//     pthread_mutex_init(&lock, NULL);
//     int randomPivot = atoi(argv[1]);
//     numOfRandomNumbers = atoi(argv[2]);

//     Thread_num = get_nprocs_conf();

//     pthread_t tid[Thread_num];

//     int random1 = rand();
//     srand(randomPivot);

//     x = numOfRandomNumbers / Thread_num;
//     y = numOfRandomNumbers % Thread_num;
//     printf("\nx= %d , y= %d , x+y= %d\n",x,y,x+y);
//     // pthread_create(&(tid[0]), NULL, &sum_count_prime1, NULL);
//     for (int i = 0; i < Thread_num ; i++)
//     {
//         pthread_create(&(tid[i]), NULL, &sum_count_prime, NULL);
//     }

//     // pthread_join(tid[0], NULL);
//     for (int i = 0; i < Thread_num ; i++)
//     {

//         pthread_join(tid[i], NULL);
//     }

//     printf("%ld,%ld\n", sum, primeCounter);
//     pthread_mutex_destroy(&lock);
    
//     exit(0);
// }

int main(int argc, char *argv[])
{
    pthread_mutex_init(&lock, NULL);

    if (argc != 3)
    {
        printf("Too few arguments ");
        printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
        exit(0);
    }
    int randomPivot = atoi(argv[1]);
    numOfRandomNumbers = atoi(argv[2]);

    Thread_num = get_nprocs_conf();

    pthread_t tid[Thread_num];

    int random1 = rand();
    srand(randomPivot);

    x = numOfRandomNumbers / Thread_num ;

    for (int i = 0; i < Thread_num ; i++)
    {

         pthread_create(&(tid[i]), NULL, &sum_count_prime, NULL);
    
    }

    for (int i = 0; i < Thread_num ; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("%ld,%ld\n", sum, primeCounter);
    pthread_mutex_destroy(&lock);
    
    exit(0);
}