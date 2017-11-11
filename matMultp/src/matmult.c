/*
 ============================================================================
 Name        : matMultp.c
 Author      : Omar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <ctype.h>
#include <string.h>
/**
 * open the given files.
 */
void open_files(char *argv[]);
/**
 * get x,y,z(dimensions of the given matrices).
 *
 */
void get_dimen();
/**
 * extract a variable from the line.
 * the line in the file should be in this format(row=xx col=yy).
 */
void extract_var(char** line, long* var);
/**
 * read the matrix and file the global 2D array
 */
void read_mat();
void initialize_out_mat();
/*
 *  This function generates array of threads (size
 of array equals to number of rows of the first
 matrix).
 *  Each thread will call a function to calculate
 the elements of the row.
 *  The index of the thread in the array is the
 number of the row it executed .
 * 	The answer is filled in an matrix after
 joining each thread and then the matrix will
 copied to a file.
 */
void method1();
/**
 * This function generates array of threads (size
 of array equals to number of rows of the firstmatrix * number of columns of the second
 matrix).
 * Each thread will call a function to calculate
 an element in a given row and column.
 * Information about the element is passed in a
 struct.
 * The answer is filled in an matrix after
 joining each thread and then the matrix will
 copied to a file.
 */
void method2();
FILE *file_a, *file_b, *file_out1, *file_out2;
long **matA, **matB, **matC;
long x = 0, y = 0, z = 0;
int main(int argc, char *argv[]) {

	if (argc > 1)
		open_files(argv);
	else
		open_files(NULL);

	get_dimen();
	read_mat();

	initialize_out_mat();
	method1();
	for (long i = 0; i < x; i++) {
		for (int j = 0; j < z; j++) {
			fprintf(file_out1, "%ld\t", matC[i][j]);
		}
		fprintf(file_out1, "\n");
	}
	fclose(file_out1);
	initialize_out_mat();
	method2();
	for (long i = 0; i < x; i++) {
		for (int j = 0; j < z; j++) {
			fprintf(file_out2, "%ld\t", matC[i][j]);
		}
		fprintf(file_out2, "\n");
	}
	free(matA);
	free(matB);
	free(matC);
	fclose(file_out2);
}
int file_num = 1;
void open_files(char *argv[]) {
	if (argv == NULL) {
		file_a = fopen("a.txt", "r");
		file_b = fopen("b.txt", "r");
		file_out1 = fopen("c_1", "w");
		file_out2 = fopen("c_2", "w");
		if (file_out2 == NULL || file_out1 == NULL
				|| file_a == NULL || file_b == NULL)
			printf("ERROR\n"), exit(1);
	} else {
		file_a = fopen(argv[1], "r");
		file_b = fopen(argv[2], "r");
		file_out1 = fopen(strcat(argv[3], "_1"), "w");
		file_out2 = fopen(strcat(argv[3], "_2"), "w");
		if (file_out2 == NULL || file_out1 == NULL
				|| file_a == NULL || file_b == NULL)
			printf("ERROR\n"), exit(1);
	}
}

void get_dimen() {
	char *line = malloc(20);
	if (fgets(line, 20, file_a) != NULL) {
		extract_var(&line, &x);
		extract_var(&line, &y);
	}
	line = malloc(20);
	if (fgets(line, 20, file_b) != NULL) {
		long temp = y;
		extract_var(&line, &y);
		if (y != temp) {
			printf(
					"ERROR number of columns of first matrix doesn't equal "
							"number of rows of second matrix.\n");
			exit(-1);
		}
		extract_var(&line, &z);
	}
}
void extract_var(char** line, long* var) {
	char *p = *line;
	while (*p) { // While there are more characters to process...
		if (isdigit(*p)) { // Upon finding a digit, ...
			long val = strtol(p, &p, 10); // Read a number, ...
			*var = val;
			*line = p;
			return;
		} else { // Otherwise, move on to the next character.
			p++;
		}
	}
	*line = p;
}

void read_mat() {
	matA = malloc(x * sizeof(long*));
	for (int i = 0; i < x; ++i)
		matA[i] = malloc(y * sizeof(long));
	matB = malloc(y * sizeof(long*));
	for (int i = 0; i < y; ++i)
		matB[i] = malloc(z * sizeof(long));
	//reading the first matrix.
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (!fscanf(file_a, "%ld", &matA[i][j]))
				break;
		}
	}
	//reading the second matrix.
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < z; j++) {
			if (!fscanf(file_b, "%ld", &matB[i][j]))
				break;
		}
	}
	fclose(file_a);
	fclose(file_b);
}
void initialize_out_mat() {
	if (matC) {
		free(matC);
	}
	matC = malloc(x * sizeof(long*));
	for (int i = 0; i < x; ++i)
		matC[i] = malloc(z * sizeof(long));
}
/**
 * calculate an element(cell) in the matrix.
 */
long get_elem(long row, long col) {
	long elem = 0;
	for (int i = 0; i < y; i++) {
		elem += (matA[row][i] * matB[i][col]);
	}
	return elem;
}
/**
 * calculate a row in the matrix.
 */
void* calc_row(void *i) {
	long row = (long) i;
	for (int j = 0; j < z; j++) {
		matC[row][j] = get_elem(row, (long) j);
	}
	pthread_exit(NULL);
}
void method1() {
	struct timeval stop, start;
	printf("Method 1 takes %ld threads\n", x);
	gettimeofday(&start, NULL); //start checking time
	pthread_t threads[x];
	for (long i = 0; i < x; i++) {
		if (pthread_create(&threads[i], NULL, calc_row,
				(void *) i)) {
			printf("Can not create a thread\n");
			exit(1);
		}
	}
	for (long i = 0; i < x; i++) {
		pthread_join(threads[i], NULL);
	}
	gettimeofday(&stop, NULL); //end checking time

	printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
	printf("Microseconds taken: %lu\n",
			stop.tv_usec - start.tv_usec);
}
struct element {
	long row;
	long col;
};
long n = 1000000;
/**
 * get information about the element to be executed by the thread.
 */
void *elem_info(void* elem) {
	long element = (long) elem;
	long r = element % n, c = element / n;
	matC[r][c] = get_elem(r, c);
	pthread_exit(NULL);
}
long ii, jj;
void method2() {
	struct timeval stop, start;
	printf("===================================\n"
			"Method 2 takes %ld threads\n", x * z);
	gettimeofday(&start, NULL); //start checking time

	pthread_t threads[x * z];
	int idx = 0;
	for (ii = 0; ii < x; ii++) {
		for (jj = 0; jj < z; jj++) {
			long element = ii + (jj % n) * n; //combining the two indexes into one variable.
			if (pthread_create(&threads[idx++], NULL, elem_info,
					(void*) element)) {
				printf("Can not create a thread\n");
				exit(1);
			}
		}
	}
	for (long i = 0; i < x * z; i++) {
		pthread_join(threads[i], NULL);
	}

	gettimeofday(&stop, NULL); //end checking time
	printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
	printf("Microseconds taken: %lu\n",
			stop.tv_usec - start.tv_usec);
}
