#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include <sys/time.h>
using namespace std;

double determinant(double **matrix1, int level) {
	double det = 0;
	double res;
	double **matrix2;

	if (level == 1) // trivial case 1x1 matrix
	{
		res = matrix1[0][0];
	}

	else
	{
		if (level == 2) // base case 2x2 matrix
		{
			res = matrix1[0][0] * matrix1[1][1] - matrix1[1][0] *matrix1[0][1];
		}
		else
		{ // NxN matrix
			res = 0;
			matrix2 = new double*[level-1];
				for (int k = 0; k < (level - 1); k++)
					{

						matrix2[k] = new double[level-1];
					}
			for (int j1 = 0; j1 < level; j1++) {


				for (int i = 1; i < level; i++) {
					int j2 = 0;
					for (int j = 0; j < level; j++) {
						if (j == j1)
							{
							   continue;
							}
						matrix2[i - 1][j2] = matrix1[i][j];
						j2++;
					}
				}

				res =res + (pow(-1.0, 1.0 + j1 + 1.0) * matrix1[0][j1]* determinant(matrix2, level - 1));
			}
			 for(int i = 0; i < level-1; ++i) {
        				delete[] matrix2[i];
    				}
    					//Free the array of pointers
   					 delete[] matrix2;
				}
	}
	return res;
	}


int main()
{

    int n = 0;
    cout<<"enter the n value:";
    cin>>n;
    srand (time(NULL));
    double** matrix = new double*[n];

     for(int i=0; i<n; i++)
     {
        matrix[i] = new double[n];
     }

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			matrix[i][j] = rand() % 10 + 1;;
		}
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<< matrix[i][j]<<" ";
		}
			cout<<"\n";
	}
        struct timeval start , end ;
  	gettimeofday(&start,NULL);                  //getting the system time before executing the Sequential method
	cout << determinant(matrix,n);
        gettimeofday(&end,NULL);    //getting the time after executing the SAM method
   	cout <<"Time taken by Sequential method :" << (end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)<<endl;
	return 0;
}
