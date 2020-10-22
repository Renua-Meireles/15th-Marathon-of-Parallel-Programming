#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define MAXNUM 100
#define MAXSTR 50
#define MAXRESULTS 1000

char firstHalf[MAXNUM][MAXSTR];
char secondHalf[MAXNUM][MAXSTR];
char strToTest[MAXSTR];
long int result[MAXRESULTS];
int numPrimes=0;

int isprime(long int value)
{ long int root;
  long int factor=2;
  root = sqrtl(value);
  int not_prime = 0;
  #pragma omp parallel for num_threads(root+1)
  for (factor=2; factor<=root; factor++){
    if (!(fmod((double)value,(double) factor) > 0.0))
      {
        #pragma omp critical
        not_prime++;
      }
  }
  return !not_prime;
  /*int prime = 1;
  while ((factor<=root) && (prime))
        { prime = fmod((double)value,(double) factor) > 0.0;
		  factor++;
	    }
  return prime;*/
}

void quicksort(long int *primes,int first,int last)
{ int i, j, pivot;
  long int temp;
  if (first<last)
     { pivot=first;
       i=first;
       j=last;
       while(i<j)
	     { while (primes[i]<=primes[pivot]&&i<last)
             i++;
           while(primes[j]>primes[pivot])
             j--;
           if (i<j)
		      { temp=primes[i];
                primes[i]=primes[j];
                primes[j]=temp;
              }
         }
       temp=primes[pivot];
       primes[pivot]=primes[j];
       primes[j]=temp;

       #pragma omp task
       quicksort(primes,first,j-1);

       #pragma omp task
       quicksort(primes,j+1,last);
       
     }
}


int main(int argc,char** argv)
{ 
  //double init = omp_get_wtime();
  FILE *primesFile;
  int i=0,j=0,numResults=0;
  long int primeToTest;
  primesFile=stdin;
  fscanf(primesFile,"%d\n",&numPrimes);
  for (i=0;i<numPrimes;i++)
      fscanf(primesFile,"%s\n",firstHalf[i]);

  for (i=0;i<numPrimes;i++)
      fscanf(primesFile,"%s\n",secondHalf[i]);
  fclose(primesFile);

  #pragma omp parallel for collapse(2) private(strToTest, primeToTest) shared(numResults)
  for (i=0;i<numPrimes;i++)
	  for (j=0;j<numPrimes;j++)
	      { strcpy(strToTest,firstHalf[i]);
	        strcat(strToTest,secondHalf[j]);
			primeToTest=atol(strToTest);
			if (isprime(primeToTest)) 
			   {
           result[numResults++]=primeToTest;
			   }
	      }
  quicksort(result,0,numResults-1);
  for (i=0;i<numResults;i++)
      printf("%ld\n",result[i]);
  //printf("%lf\n", omp_get_wtime() - init);
  return 0;
}