#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


struct test{

  long long num;

};


void* split_insertion_thread(void* arg);

void* single_insertion_thread(void* arg);


double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}



int main(int argc, char** argv)
{

  //check for one command line argument
  if (argc != 2) 
  { 
    printf( "Expected 1 argument: you have %d\n", argc-1 ); 
    exit(0);
  }
  
  //check to see if argument is a non negative integer
  // loop through input and check all char to make sure all are int
  for (int i=0; argv[1][i] != '\0'; i++)
  {
    // using ctype.h
    if (isdigit(argv[1][i]) == 0)
    {
      printf("Argument must be a non-negative integer\n");
      exit(0);
    }
  }
  
  // change str into int
  long long ary_size = atoll(argv[1]);
  
  
 
  //create array
  double* ary = (double*)malloc(ary_size * sizeof(double));
  
  // create random seed
  srand(time(NULL));
  
  
  
  for (int i=0; i < ary_size; i++)
  {
  	double rand_num = fRand(50.0, 500.0);
  	ary[i] = rand_num;
  	printf("%f\n", ary[i]);
  }
		
  
  

	// free up resources
	free(ary);
  return 0;
  

}


