#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

// create stuct 
struct Array_attr {

  long long ary_size;
  double* ary;

};


void* split_insertion_thread(void* arg);

void* single_insertion_thread(void* arg)
{
	// copy entire array into a different array
	
	printf("MADE IT TO THE THREEEEAD");
	pthread_exit(0);
	return NULL;
	
	
}


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
  
  ///////////////////////////////////////////// After checks are complete
 
  //create arrays
  double* ary = (double*)malloc(ary_size * sizeof(double)); //main array
  double* aryB = (double*)malloc(ary_size * sizeof(double)); // one thread array
  double* aryC = (double*)malloc(ary_size * sizeof(double)); // two thread array finished
  double* AfirstHalf = (double*)malloc(ary_size * sizeof(double)); // two thread array first half
  double* AsecondHalf = (double*)malloc(ary_size * sizeof(double)); // two thread array second half
  
  // create random seed
  srand(time(NULL));
  
  // loop through the array and create random floats for each value
  for (int i=0; i < ary_size; i++)
  {
  	double rand_num = fRand(50.0, 500.0);
  	ary[i] = rand_num;
  	printf("%f\n", ary[i]);
  }
		
	//create structs to hold arrays
	struct Array_attr aryB_attr;
	aryB_attr.ary = aryB;
	

		
	//Create thread ids
	pthread_t tid_single; 	
	
	//create thread function CHECK TO SEE IF IT WORKED BY CHECKING THE ERROR RETURNED
	int tid_single_return_status = 
		pthread_create(&tid_single, NULL, single_insertion_thread, &aryB_attr);
		
	printf("Return status is: %d", tid_single_return_status);	
	
	pthread_join(tid_single, NULL);

	
  
  

	// free up resources
	free(ary);
	free(aryB);
	free(aryC);
	free(AfirstHalf);
	free(AsecondHalf);
  return 0;
  

}


