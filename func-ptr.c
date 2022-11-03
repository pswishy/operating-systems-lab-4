#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "process.h"
#include "util.h"

#define DEBUG 0			//change this to 1 to enable verbose output
// command for my program : ./task1/qsort task1/test-data/test1.csv
// command for answer program: ./task1/qsort_ref task1/test-data/test1.

/**
 * Signature for an function pointer that can compare
 * You need to cast the input into its actual 
 * type and then compare them according to your
 * custom logic
 */
typedef int (*Comparer) (const void *a, const void *b);

/**
 * compares 2 processes
 * You can assume: 
 * - Process ids will be unique
 * - No 2 processes will have same arrival time
 */
int my_comparer(const void *this, const void *that)
{
	Process process1 = *(Process *)this;
	Process process2 = *(Process *)that;

   // qsort() expects the comparison function to return:
   // 
   //    a negative result if process 1 go before process2
   //    0 if process is same
   //    a positive result if process1 go after process 2

	// process1 priority is higher then process 1 should go BEFORE
	if ((process1.priority) > (process2.priority)){
		return -1;
	}
	// process2 priority is higher then process1 should go AFTER 
	else if ((process2.priority) > (process1.priority)){
		return 1;
	}
	// triggers if process1 and process 2 have same priority 
	else{
			// process1 has smaller arrival time it should be BEFORE process 2
			if ((process1.arrival_time) < (process2.arrival_time)){
				return -1;
			}
			// process1 has bigger arrival time it should be AFTER process 2
			else if ((process2.arrival_time) < (process1.arrival_time)){
				return 1;
			}
	}
	return 0;
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		   fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	}

	/*******************/
	/* Parse the input */
	/*******************/
	FILE *input_file = fopen(argv[1], "r");
	if (!input_file) {
		   fprintf(stderr, "Error: Invalid filepath\n");
		   fflush(stdout);
		   return 1;
	}

	Process *processes = parse_file(input_file);

	/*******************/
	/* sort the input  */
	/*******************/
	Comparer process_comparer = &my_comparer;

#if DEBUG
	for (int i = 0; i < P_SIZE; i++) {
		    printf("%d (%d, %d) ",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
	}
	printf("\n");
#endif

	qsort(processes, P_SIZE, sizeof(Process), process_comparer);

	/**************************/
	/* print the sorted data  */
	/**************************/

	for (int i = 0; i < P_SIZE; i++) {
		    printf("%d (%d, %d)\n",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
	}
	fflush(stdout);
	fflush(stderr);

	/************/
	/* clean up */
	/************/
	free(processes);
	fclose(input_file);
	return 0;
}
