#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

#define DEBUG 0

// create stuct 
struct Array_attr {

  long long ary_size;
  double* ary;

};

//create struct for two sorted lists to be merged
struct mergeSorted_S {
	long long ary1_size;
	long long ary2_size;
	double* ary1;
	double* ary2;
	double* aryC;

};



////////////////////////// single thread

void* mergeSorted(void* arg)
{
	struct mergeSorted_S* arg_struct = arg;
	
	long long ary1_size = arg_struct->ary1_size;
	long long ary2_size = arg_struct->ary2_size;
	double* ary1 = arg_struct->ary1;
	double* ary2 = arg_struct->ary2;
	double* aryC = arg_struct->aryC;
	
	// sort both array 1 and 2 into array C
	int i = 0;
	int j = 0;
	int k = 0;

	
	while (i<ary1_size && j<ary2_size)
	{

		// standard merging two sorted list algorithm
		
		// if the item in one list is less than the other, add it to the third array
		if (ary1[i] < ary2[j])
		{
			aryC[k++] = ary1[i++];
		}
		else
		{
			aryC[k++] = ary2[j++];
		}
	}
	
	// after one list is completly empty, add whichever list remains to the third array
	while (i<ary1_size)
	{
		aryC[k++] = ary1[i++];
	}
	
	while (j<ary2_size)
	{
		aryC[k++] = ary2[j++];
	}
	
	pthread_exit(0);
	
	return NULL;
}


// This thread function takes in a pointer to an Array_attr
void* single_insertion_thread(void* arg)
{
	
	struct Array_attr* arg_struct = (struct Array_attr*) arg;	
	
	// Get variables out of struct
	long long ary_size = arg_struct->ary_size;
	double* ary = arg_struct->ary;
	
	//printf("ary size is: %lld\n", ary_size);
	
	
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
  double* AfirstHalf = (double*)malloc(ary_size/2 * sizeof(double)); // two thread array first half
  double* AsecondHalf = (double*)malloc(ary_size/2 * sizeof(double)); // two thread array second half
  
  // create random seed
  srand(time(NULL));
  
  // loop through the array and create random floats for each value
  for (int i=0; i < ary_size; i++)
  {
  	double rand_num = fRand(50.0, 500.0);
  	ary[i] = rand_num;
  	//printf("%f\n", ary[i]);
  }
		
	// DOING THE PROBLEM USING ONE THREAD	
	//create structs to hold arrays
	struct Array_attr aryB_attr;
	aryB_attr.ary = aryB;
	aryB_attr.ary_size = ary_size;
	
	//Copy main array to arrayB
	for (int i=0; i<ary_size; i++)
	{
		aryB[i] = ary[i];
		//printf("%lf ", ary[i]);
	}
	
#if DEBUG == 1
	printf("aryB before sort:\n");
	for (int i=0; i<ary_size; i++)
	{
		printf("%lf ", aryB[i]);
	}
	printf("\n\n");
#endif
	
	//Create thread id
	pthread_t tid_single; 
	
	// get time before thread
	struct timespec ts_begin;
	struct timespec ts_end;
	clock_gettime(CLOCK_MONOTONIC, &ts_begin);
	
	//create thread function CHECK TO SEE IF IT WORKED BY CHECKING THE ERROR RETURNED
	int tid_single_return_status = 
		pthread_create(&tid_single, NULL, single_insertion_thread, &aryB_attr);
		
	if (tid_single_return_status != 0)
	{ 
		printf("Problem with creating thread error: %d\n", tid_single_return_status);
	}
	
	// wait for threads to finish
	pthread_join(tid_single, NULL);
	
	// get time after thread
	clock_gettime(CLOCK_MONOTONIC, &ts_end);
	
	double elapsed = ts_end.tv_sec - ts_begin.tv_sec;
	elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec)/ 1000000000.0;
	
	printf("Sorting is done in %lf ms when one thread is used\n", elapsed*1000);
	
	
	////////////// End thread one problem
	
	
	// Split the array A in half and then send to threads and rejoin at the end
	
	
	// copy first half of A to first array
	int count_1 = 0;
	for (int i=0; i<ary_size/2; i++)
	{	
		AfirstHalf[i] = ary[i];
		count_1++;
	}
	
	int count_2 = 0;
	for (int i=ary_size/2; i<ary_size; i++)
	{
		AsecondHalf[i-ary_size/2] = ary[i];
		count_2++;
	}
	
#if DEBUG == 1	
	printf("[%d]First half before sorting is:\n", count_1);
	for (int i=0; i<count_1; i++)
	{
		printf("%lf ", AfirstHalf[i]);
	}
	printf("\n\n");
	
	printf("[%d]second half before sorting is:\n", count_2);
	for (int i=0; i<count_1; i++)
	{
		printf("%lf ", AsecondHalf[i]);
	}
	printf("\n\n");
#endif
	
	// create thread id's
	pthread_t tid_1, tid_2;
	
	// create struct's for each thread to be run
	struct Array_attr firstHalf;
	struct Array_attr secondHalf;
	
	firstHalf.ary_size = count_1;
	secondHalf.ary_size = count_2;
	
	firstHalf.ary = AfirstHalf;
	secondHalf.ary = AsecondHalf;
	//////////////////////////////////////// TWO THREADS
	
	//get time before sort
	clock_gettime(CLOCK_MONOTONIC, &ts_begin);
	
	//create thread function CHECK TO SEE IF IT WORKED BY CHECKING THE ERROR RETURNED
	int tid_1_return_status = 
		pthread_create(&tid_1, NULL, single_insertion_thread, &firstHalf);
		
	if (tid_1_return_status != 0)
	{ 
		printf("Problem with creating thread error: %d\n", tid_1_return_status);
	}
	
	//create thread function CHECK TO SEE IF IT WORKED BY CHECKING THE ERROR RETURNED
	int tid_2_return_status = 
		pthread_create(&tid_2, NULL, single_insertion_thread, &secondHalf);
		
	if (tid_2_return_status != 0)
	{ 
		printf("Problem with creating thread error: %d\n", tid_2_return_status);
	}
	
	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	
#if DEBUG == 1	
	
	printf("\n[%d]First half after sorting is:\n", count_1);
	for (int i=0; i<count_1; i++)
	{
		printf("%lf ", AfirstHalf[i]);
	}
	printf("\n\n");
	
	printf("[%d]Second half after sorting is:\n", count_2);
	for (int i=0; i<count_2; i++)
	{
		printf("%lf ", AsecondHalf[i]);
	}
	printf("\n\n");
#endif


	// create struct to pass into mergeSorted thread
	struct mergeSorted_S merge;
	
	//assign values to struct
	
	merge.ary1 = AfirstHalf;
	merge.ary2 = AsecondHalf;
	merge.aryC = aryC;
	merge.ary1_size = count_1;
	merge.ary2_size = count_2;
	
	//create thread id
	pthread_t merge_id;
	
	// call thread to merge arrays in struct
	int merge_return_status = 
		pthread_create(&merge_id, NULL, mergeSorted, &merge);
		
	if (merge_return_status != 0)
	{ 
		printf("Problem with creating thread error: %d\n", merge_return_status);
	}
	
	// wait for thread
	pthread_join(merge_id, NULL);
	
#if DEBUG == 1	
	printf("Array sorted using two threads and then merged:\n");
	for (int i = 0; i<ary_size; i++)
	{
		printf("%lf ", aryC[i]);
	}
	printf("\n\n");
#endif
	
	//get time after sort
	clock_gettime(CLOCK_MONOTONIC, &ts_end);
	
	elapsed = ts_end.tv_sec - ts_begin.tv_sec;
	elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec)/ 1000000000.0;
	
	printf("Sorting is done in %lf ms when two threads are used\n", elapsed*1000);
	

	// free up resources
	free(ary);
	free(aryB);
	free(aryC);
	free(AfirstHalf);
	free(AsecondHalf);
  return 0;
  

}


