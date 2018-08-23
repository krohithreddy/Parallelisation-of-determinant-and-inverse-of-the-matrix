#define N 30

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[])

{
	int	i,j,k,u,v;
	int	maxi;
	double	a[N][2*N];
	double	tmp;
	double	pivot;
	double	multiple;
	double	trace;
        double  delta;
        struct timeval start, end;

	// initialize the augmented matrix
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) {
			a[i][j]=(double)((N+i-j)%N+1);
		} //end for
	} //end for
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) {
			a[i][N+j]=(i==j)?1:0;
		} //end for
	} //end for

	// print the augmented matrix if it is 5x10 or smaller
	if (N<=50) {
		printf("Original Matrix:\n");
		printf("==============\n\n");
		for (i=0;i<N;i++) {
			for (j=0;j<2*N;j++) {
				printf("%6.3lf ",a[i][j]);
			} //end for
			printf("\n");
		} //end for
		printf("\n\n");
	}

	// Perform Gaussian elimination on the matrix
	i=0;
	j=0;
        gettimeofday(&start, NULL);
        
	while ((i<N) && (j<N)) {
		// Find pivot in column j, starting in row i
		maxi=i;
		for (k=i+1;k<N;k++) {
			if (abs(a[k][j]) > abs(a[maxi][j])) {
				maxi=k;
			}// end if
		} // end for

		if (a[maxi][j]==0) {
			printf("Matrix is singular - aborting.\n");
			exit(1);
		} else {
			// Swap contents of rows i and maxi in column j and on
			for (u=j;u<2*N;u++) {
				tmp=a[i][u];
				a[i][u]=a[maxi][u];
				a[maxi][u]=tmp;
			} // end for

			// a[i,j] is now the pivot and we want to
			// divide each entry in row i by a[i,j]
			pivot=a[i][j];
			for (u=j;u<2*N;u++) {
				a[i][u]=a[i][u]/pivot;
			} // end for

			// a[i,j] is now 1 and we will use it to zero out
			// the elements in column j below row i
#pragma omp parallel for private(multiple,v)
			for (u=i+1;u<N;u++) {
				// Subtract a[u,j] * row i from row u
				multiple=a[u][j];
				for (v=j;v<2*N;v++) {
					a[u][v]=a[u][v]-multiple*a[i][v];
				}
			} // end for
			i=i+1;
		} // end if
		j=j+1;
	} // end while

	// now zero out the top half of A to form A_inv
	for (i=1;i<N;i++) {
		// we already have '1' in A[i,i] so use it to knock out
		// values above it
#pragma omp parallel for private(multiple,u)
	for (j=i-1;j>=0;j--) {
			multiple=a[j][i];
			for (u=i;u<2*N;u++) {
				a[j][u]=a[j][u]-multiple*a[i][u];
			} // end for
		} // end for
	} // end for
        gettimeofday(&end, NULL);
	// print the result matrix if it is 5x10 or smaller
	if (N<=50) {
		printf("Result Matrix:\n");
		printf("=============\n\n");
		for (i=0;i<N;i++) {
			for (j=0;j<2*N;j++) {
				printf("%6.3lf ",a[i][j]);
			} //end for
			printf("\n");
		} //end for
		printf("\n\n");
	} else {
        trace=0.0;
        for (i=0;i<N;i++)
        	trace+=a[i][i];
        printf("\nThe trace of A is %lf.\n",trace);
		printf("\n\n");
	} // end if
        delta =  (end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec);
        printf("The time required for the %d x %d matrix inversion is : %d\n",N,N,delta);
} // end main
