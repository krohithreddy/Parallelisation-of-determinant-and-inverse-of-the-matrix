#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <atomic>                   //required libraries
#include <pthread.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

using namespace std;
int n,m;
double ** global_matrix;
double Result = 0.0;
pthread_mutex_t lock1;
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

// thread calls this function to find the prime numbers with in the limit
//impliments the static allocation method
void * Writer(void* ttid)
{
  long id = (long ) ttid;   //thread id


  long  j , k;
  double **matrix2;

  matrix2 = new double*[n-1];
 for (int k1 = 0; k1 < (n - 1); k1++)
 {

	matrix2[k1] = new double[n-1];
  }
  double res = 0;
  // for each thread the value assigned to check prime or not is k*m + id
  for ( j = id , k = 1 ; j < n; j = k*m + id , k++) {


                               pthread_mutex_lock(&lock1);
                                cout<<"thread"<<j<<"\n";
                               pthread_mutex_unlock(&lock1);
				for (int i = 1; i < n; i++) {
					int j2 = 0;
					for (int j1 = 0; j1 < n; j1++) {
						if (j1 != j)
						{
						matrix2[i - 1][j2] = global_matrix[i][j1];
						j2++;
                                               }
					}
				}



				res = res + (pow(-1.0, 1.0 + j + 1.0) * global_matrix[0][j]* determinant(matrix2, n - 1));



	}
   				for(int i = 0; i < n-1; ++i) {
        				delete[] matrix2[i];
    				}
    					//Free the array of pointers
   					 delete[] matrix2;
                       pthread_mutex_lock(&lock1);
                        Result = Result + res;
                       pthread_mutex_unlock(&lock1);
}


int main () {

  ifstream ifs("inp-params.txt");   //opening the file for reading the input params
  string content( (istreambuf_iterator<char>(ifs) ),(istreambuf_iterator<char>()    ) );

  istringstream iss(content);
  iss >> n >> m;    // seperating the integers

  srand (time(NULL));
    global_matrix = new double*[n];

     for(int i=0; i<n; i++)
     {
        global_matrix[i] = new double[n];
     }

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			global_matrix[i][j] = rand() % 10 + 1;;
		}
	}

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<< global_matrix[i][j]<<" ";
		}
			cout<<"\n";
	}
  if (n == 1) // trivial case 1x1 matrix
	{
		cout<<"determinant value:"<<global_matrix[0][0];
                return 0;
	}
  if(n == 2)
  {
      cout <<"determinant value:"<<(global_matrix[0][0] * global_matrix[1][1] - global_matrix[1][0] *global_matrix[0][1]);
      return 0;
   }
  if (m <= n)
  {

        double Seq_Result;
        struct timeval start1 , end1 ;
  	gettimeofday(&start1,NULL);                  //getting the system time before executing the Sequential method
		Seq_Result = determinant(global_matrix,n);
        cout<<"Sequential Result:"<<Seq_Result<<"\n";
        gettimeofday(&end1,NULL);    //getting the time after executing the SAM method
   	cout <<"Time taken by Sequential method :" << (end1.tv_sec * 1000000 + end1.tv_usec)- (start1.tv_sec * 1000000 + start1.tv_usec)<<endl;

   	if (pthread_mutex_init(&lock1, NULL) != 0)
    	{                                                      //initializing the parameters
        	printf("\n mutex init failed\n");
    	}
  pthread_t tid[m];    //for m threads


  struct timeval start , end ;
  gettimeofday(&start,NULL);                  //getting the system time before executing the Parallal method
  Result = 0;
  for (int i = 0; i < m; i++) {
  		pthread_attr_t attr; 							//allocated the attribute to thread
		pthread_attr_init(&attr);
        pthread_create(&tid[i], 0, Writer, (void *)(intptr_t)(i));   //creating m threads

    }
   for (int i = 0; i < m; i++)
   {
       pthread_join(tid[i], NULL);    //joining the m thread with the main thread
   }
   cout<<"The det of the matrix :"<<Result<<endl;
   gettimeofday(&end,NULL);    //getting the time after executing the SAM method
   cout <<"Time taken by Parallel method :" << (end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)<<endl;
   if(Result == Seq_Result)
   {
        cout<<"Yes same..........\n";
   }
   else
   {
        cout<<"No Not same..................\n";
    }
   }
   else
   {
      cout<<"number of threads must be less than the matrix size";
    }

  return 0;
}
