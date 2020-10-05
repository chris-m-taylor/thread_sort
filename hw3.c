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

////////////////////////// single thread
void* single_insertion_thread(void* arg)
{
	
	struct Array_attr* arg_struct = (struct Array_attr*) arg;	
	
	// Get variables out of struct
	long long ary_size = arg_struct->ary_size;
	double* ary = arg_struct->ary;
	
	printf("ary size is: %lld\n", ary_size);
	
	//sort array using standard insertion sort algorithm
	for (int i=1; i<ary_size; i++)
	{
		double key = ary[i];
		int j = i-1;
		
		while (j >= 0 && ary[j] > key)
		{
			ary[j+1] = ary[j];
			j= j-1;
		}
		ary[j+1] = key;
	}
	printf("Done sorting");
	
	
	
	pthread_exit(0);
	return NULL;
	
	
}

// Helper Function
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}


// MAIN
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
	aryB_attr.ary_size = ary_size;
	
	//Copy main array to arrayB
	for (int i=0; i<ary_size; i++)
	{
		aryB[i] = ary[i];
	}

	//Create thread ids
	pthread_t tid_single; 	
	
	//create thread function CHECK TO SEE IF IT WORKED BY CHECKING THE ERROR RETURNED
	int tid_single_return_status = 
		pthread_create(&tid_single, NULL, single_insertion_thread, &aryB_attr);
		
	if (tid_single_return_status != 0)
	{ 
		printf("Problem with creating thread error: %d\n", tid_single_return_status);
	}
	
	// wait for threads to finish
	pthread_join(tid_single, NULL);
	
	
	// print out aryB after thread
	for (int i=0; i<ary_size; i++)
	{
		if (i==0){ printf("sorted array is: "); }
		printf("%f ", aryB[i]);
		if (i==ary_size-1){ printf("\n"); }
	}
	
	
	

	
  
  

	// free up resources
	free(ary);
	free(aryB);
	free(aryC);
	free(AfirstHalf);
	free(AsecondHalf);
  return 0;
  

}


