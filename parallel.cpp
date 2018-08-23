#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include <mutex>
using namespace std;
int n;

mutex mtx;

class Mat {
public:
  double** matrix;
  int i;
  Mat(double**);
};
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

void * writer_func(void *args) {    // function
    Mat *arg_mat = (Mat *)args;
    int tid = arg_mat->i ;
  //  cout << tid << endl;
    double** matrix1 = new double*[n-1];

     for(int i=0; i<n-1; i++)
     {
        matrix1[i] = new double[n-1];
     }
     int k = 0 ,l = 0 ;
  for(int i=1;i<n;i++)
  {
    l=0;
    for(int j=0;j<n;j++)
    {
      if(j==tid){

      }
      else{
        matrix1[i-1][l] =arg_mat->matrix[i][j];
        l++;
      }
    }
  }
double det = determinant(matrix1,n-1);
  mtx.lock();
  cout << det << endl ;
  for(int i=0;i<n-1;i++)
  {
  	for(int j=0;j<n-1;j++)
  	{
  		cout<< matrix1[i][j]<<" ";
  	}
  		cout<<"\n";
  }
  mtx.unlock();
  pthread_exit(NULL);
}



Mat::Mat(double** Matrix) {
  matrix = Matrix;
}

int main()
{
    cout<<"enter the n value:";
    cin>>n;
    srand (time(NULL));
  double**  matrix = new double*[n];
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
    mtx.lock();
    for(int i=0;i<n;i++)
    {
      for(int j=0;j<n;j++)
      {
        cout<< matrix[i][j]<<" ";
      }
        cout<<"\n";
    }
    mtx.unlock();
    // double** matrix = new double*[n];


    pthread_t writer[2] ;       // Declaring n threads where each will update particular array element

	// for(int i=0;i<n;i++)
	// {
	// 	for(int j=0;j<n;j++)
	// 	{
	// 		cout<< matrix[i][j]<<" ";
	// 	}
	// 		cout<<"\n";
	// }
  for(long i = 0 ; i < 2 ; i++) {
    Mat *m = new Mat(matrix);
    m->i = i;
    pthread_create(&writer[i] , NULL , writer_func , m) ;   // Each thread calling write function
  }
  for(int i = 0 ; i < 2 ; i++)
      pthread_join(writer[i] , NULL) ; //Joining writer threads


	//cout << determinant(matrix,n);
	return 0;
}
