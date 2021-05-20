#define _GNU_SOURCE
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/time.h> // gettimeofday()
#include <sched.h>
#include "itay.c"
#define stk_size 10000

_Atomic long sum;
_Atomic long primeCounter;
int randomPivot, numOfRandomNumbers;
int NumOfThread;
// struct Stack *stk;
int isPrime(int num);
int randomNum();



int main() {

    atomic_init(&sum, 0);
    atomic_init(&primeCounter, 0);
    NumOfThread = numOfRandomNumbers/2;
    char stk[stk_size];
    int p1 = clone(randomNum, stk + stk_size, CLONE_PARENT, 0);
    int p2 = clone(randomNum, stk + stk_size, CLONE_PARENT, 0);

    printf("p1 pid: %d\n", p1);
    printf("p2 pid: %d\n", p2);

    wait(&p1);
    wait(&p2);

    printf("Finished!\n");
    return 0;
}




int isPrime(int num) { // prime=0 , non prime = 1
	int i;
    if (num==2){return 0;}
	for (i = 3;i<sqrt((double)num);i+=2){ // skip even numbers check
		if (num%i == 0){return 0;}
	}
    if (num%(int)sqrt((double)num) == 0){return 0;}

    return 1;
}
int randomNum(){ 
  	// atomic_store(&sum, 5);
	printf("random num \n");
	int random = rand();
    	srand(randomPivot); // to avoid non truly random numbers

	//generate random numbers
	for (int i=0;i<NumOfThread;i++){
		random = rand();
        printf("trying %d\n", random);

		// push(stk,random);
        if(isPrime(random)){
            printf("found a prime number!\n");
            atomic_store(&sum, sum+random);
			atomic_store(&primeCounter,primeCounter++);
            printf("sum = %d\ncount = %d\n", (int)random, (int)primeCounter);

        }
		printf("random num %d = %d \n",i,random);	
    }
	return 0;
}