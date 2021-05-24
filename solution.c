#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>

// Global variables
pthread_mutex_t lock;
long sum = 0;
long primeCounter = 0;
long numOfRandomNumbers;
int randomPivot;
int processors_num;
int thread_runs,main_runs;

void foundPrime(int num) {
    sum = sum + num;
    primeCounter++;
}

// This Prime check algorithm was taken from Wikipedia
// and works by some Number Threory Lemmas.
// https://en.wikipedia.org/wiki/Primality_test#C#_code
int checkPrime(int num){

    if (num <= 1) return 0;
    if (num == 2 || num == 3 || num == 5) return 1;
    else if ((num % 2)==0 || (num % 3)==0) return 0;
    if ((!(num%6==1||num%6==5))||num%5==0)
        return 0;

    for (int i = 7; i <= (int)sqrt((double)num); i +=6)
    {
        if ((num % i == 0 ||(num % (i + 4) == 0))) return 0;
    }
    return 1;
}
int isPrime(int num)
{
    if(!checkPrime(num)) return 0;
    pthread_mutex_lock(&lock);
    foundPrime(num);
    pthread_mutex_unlock(&lock);
    return 1;
}

void *sum_count_prime_main()
{
    int num = 0;
    for (int i = 0; i < main_runs; i++)
    {
        num = rand();
        isPrime(num);
    }
    return NULL;
}

void* sum_count_prime()
{
    int num = 0;
    for (int i = 0; i < thread_runs; i++)
    {
        num = rand();
        isPrime(num);
    }
    return NULL;
}

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

    processors_num = get_nprocs_conf(); // Number of processors

    pthread_t threads[processors_num];

    srand(randomPivot);

    thread_runs = numOfRandomNumbers / processors_num ;
    main_runs =numOfRandomNumbers % processors_num ;
    
    for (int i = 0; i < processors_num ; i++)
    {
        pthread_create(&(threads[i]), NULL, &sum_count_prime, NULL);
    }

    sum_count_prime_main();     // This runs the excess numbers that were not divieded by K threads (#Numbers % ThreadNumber)

    for (int i = 0; i < processors_num ; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("%ld,%ld\n", sum, primeCounter);
    pthread_mutex_destroy(&lock);
    
    exit(0);
}