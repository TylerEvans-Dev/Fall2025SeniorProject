/* Tyler Evans U1313811
 * testing timing function program written with the help of gpt
 * :)
*/
 #define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>

/*
 * @function check_run_time
 *  @param void (*func)(void) -> function being passed in
 *  @param const char* name -> string detaling test
 */
void check_run_time(int (*func)(int), int arg, const char* name){
	//helper for converting ns to seconds
	double ns = 1e9;
	//get the start end, and timespec structs from the libn
	struct timespec start, end;
	//sets the start clock
	clock_gettime(CLOCK_MONOTONIC, &start);
	//runs the function
	func(arg);
	//once complete gets the timing by seeing time passed.
 	 clock_gettime(CLOCK_MONOTONIC, &end);
	double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / ns;
	printf("%s completed in %.9f seconds.\n", name, elapsed);
}
