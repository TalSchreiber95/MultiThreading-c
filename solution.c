#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sched.h>
#include <semaphore.h>
#include "itay.c"


#define t1_STACK 10000
#define t2_STACK 10000
#define NUM_THREADS 3
pthread_t p1, p2, p3, p4;
_Atomic long N, empty, full;
sem_t mutex;

_Atomic long sum;
_Atomic long primeCounter;
int randomPivot, numOfRandomNumbers;
struct Stack *stk;


int isPrime(int num){ // prime=1 , non prime = 0
	int i;
	if(num == 0) return 0;
    if (num==2){return 0;}
	for (i = 3;i<sqrt((double)num);i+=2){ // skip even numbers check
		if (num%i == 0){return 0;}
	}
    if (num%(int)sqrt((double)num) == 0){return 0;}

    return 1;
}
int storePrimes(){
	printf("store Prime\n");
 	// check if the number is prime
	 
	long random;
	while(!isEmpty(stk)){

		// are there any numbers left? 
		if(N-empty >= NUM_THREADS) { // N-empty = full slots
			random = pop(stk);
			atomic_store(&empty, empty - 1);
		} else {
			printf("Less items than #thread LOOP\n");
			continue;	// rerun loop
			// pthread_join(p1, NULL);
		}

		printf("pop() next number\n");// Thread may not finish its job, but return and kill itself.
		if(random == INT_MIN){ printf("Stack is empty -- WAIT\n"); return -1;}//sem_wait(&mutex); printf("Stack is empty -- WAIT\n");} //wait()!! /*return -1;*/}// recheck if stack is empty so threads will not interrupt each other.
		if (isPrime(random)){
			//if do, add up it's sum, and increment counter
			printf("Found a prime!~~~~~~~~~~~~~~~~~\n");
			atomic_store(&sum, sum+random);
			atomic_store(&primeCounter,primeCounter+1);
		}
		
		// printf("not empty\n");
	}
	printf("Stack is empty! -- FINISH -- POST -- STORE_PRIMES\n");
	// sem_post(&mutex);
	return 0;
}

int randomNum(){ 
  	// atomic_store(&sum, 5);
	printf("randNum() \n");
	// int random = rand();
	int random;
    // srand(randomPivot); // to avoid non truly random numbers
	// if (empty > NUM_THREADS/*Thread count*/) {
	// 	printf("RAND_NUM -- %d > %d, WAITING...", empty, NUM_THREADS);
	// 	sem_wait(&mutex);
	// }
	//generate random numbers

	for (int i=0;i<N;i++){
		random = rand();
		push(stk,random);
		// printf("random num %d = %d \n",i,random);
    }
	printf("finished  randNum() -- POST\n");
	// storePrimes();
	// sem_post(&mutex);
	return 0;
}


int main(int argc, char *argv[])
{
	if(argc != 3) { // should check what is means?
	    printf("Too few arguments ");
	    printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
            exit(0);
    }
	char stack1[t1_STACK];
	char stack2[t2_STACK];
	randomPivot = atoi(argv[1]); // atoi() is a function that convert string to int 
	numOfRandomNumbers = atoi(argv[2]);

	// Global Variables:
	N = numOfRandomNumbers; 
	empty = N; 				/* Counts empty stack slots*/
	// full = 0;  				/* Count full stack slots */
	sem_init(&mutex, 0, 1);
	stk=createStack(numOfRandomNumbers);
	int random = rand();
	srand(randomPivot); // to avoid non truly random numbers
	push(stk, random);

	atomic_init(&empty, N-1); // because of push() in main
    atomic_init(&sum, 0);
    atomic_init(&primeCounter, 0);

	printf("before\n");
	// randomNum();
	// int proc1= clone(randomNum, stack1+t1_STACK, CLONE_PARENT, 0);
	// int proc2= clone(storePrimes, stack2+t2_STACK, CLONE_UNTRACED, 0);
	
	pthread_create(&p1, NULL, (void*)randomNum, NULL);
	pthread_create(&p2, NULL, (void*)storePrimes, NULL);
	pthread_create(&p3, NULL, (void*)storePrimes, NULL);
	pthread_create(&p4, NULL, (void*)storePrimes, NULL);

	printf("main pid: %d\n", getpid());
	// printf("p1 pid: %d\n", proc1);
	// printf("p2 pid: %d\n", proc2);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);
	// join(&proc1);
	// wait(&proc2);
	printf("\nsum = %d\nprimeCounter = %d\n", (int)sum, (int)primeCounter);
	printf("Finished!\n");
}
