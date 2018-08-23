
#include<math.h>
#include<stdio.h>

#include<iostream>
#include<iomanip>
#include<cmath>
#include<atomic>
#include <chrono>
#include <thread>
#include<mutex>
#include <time.h>
#include <sys/time.h>
using namespace std;
using namespace std::chrono;
float **D,**C,**s; //pointers for main matrix , inverse matrix by sequential,inverse matrix by parallel
int i,j,m,k;
int n; // store size
int t; //store number of threads
int flag=0;
mutex lock1;



void LU()
{
		int i,j,k;
		float x;
		for(k=0;k<=n-1;k++)
		{
				for(j=k+1;j<=n;j++)
				{
						x=D[j][k]/D[k][k];
						for(i=k;i<=n;i++)
					 	{
			 		 		D[j][i]=D[j][i]-x*D[k][i];
					 	}
						D[j][k]=x;
				}
		}

		}

void * Writer(void* ttid)
{
				long id = (long ) ttid;   //thread id
				float y[n+1];
				for(int e=0;e<=n;e++)
					y[i]=0;

				for(int p=id,k=1;p<=n;p = k*t + (id) , k++)
				{
					float x1;
					for(int q=0;q<=n;q++)
					{
						x1=0.0;
						for(int w=0;w<=q-1;w++)
							x1=x1+D[q][w]*y[w];

						if(q==p)
							y[q]=(1.0-x1);
						else
							y[q]=(0.0-x1);
					}
					for(int q=n;q>=0;q--)
					{
						x1=0.0;
						for(int w=i+1;w<=n;w++)
							x1=x1+D[q][w]*s[w][p];

						s[q][p]=(y[q]-x1)/D[q][q];
					 }
				}
				pthread_exit(NULL);
		}

int main()
{

		cout.precision(4);        //set precision
		cout.setf(ios::fixed);
		cout<<"\nEnter the order(n): \n";
		cin>>n;                //input the no. of equations
		cout<<"\nEnter number of threads (m): \n";
		cin>>t;                //input the no. of equations

		n=n-1;
		srand (time(NULL));

		 D = new float*[n+1];  // for creating n*n matrix
		 C = new float*[n+1];  // for storing inverse by sequential method
		 s = new float*[n+1];	 // for storing inverse by parallel method


		//void LU();
		for(int i=0; i<=n; i++)
		{
			 D[i] = new float[n+1];
			 C[i] = new float[n+1];
			 s[i] = new float[n+1];

		}

		for (i=0;i<=n;i++)
				for (j=0;j<=n;j++)
				{
					D[i][j] = rand() % 10 + 1;;   //input the elements of array
				//	c[i][j] = a[i][j];
				}
				for (i=0;i<=n;i++)                    //Pivotisation
						for (k=i+1;k<=n;k++)
								if (abs(D[i][i])<abs(D[k][i]))
								{
									flag++;
									for (j=0;j<=n;j++)
									{
												double temp=D[i][j];
												D[i][j]=D[k][j];
												D[k][j]=temp;
										}
									}


		/* Call a sub-function to calculate the LU decomposed matrix. Note that
		we pass the two dimensional array [D] to the function and get it back */

		  LU();

		// printf("The matrix LU decomposed \n");
		//       for(m=0;m<=n;m++)
		//       {
		// 				for(j=0;j<=n;j++)
		// 					cout << D[m][j]<<" ";
		//
		//       cout <<endl;
		//       }

		/*  TO FIND THE INVERSE */

		/* to find the inverse we solve [D][y]=[d] with only one element in
		the [d] array put equal to one at a time */

			pthread_t tid[t];    //for m threads
			pthread_attr_t attr; 							//allocated the attribute to thread
			pthread_attr_init(&attr);
			struct timeval start1 , end1 ;
			gettimeofday(&start1,NULL);
			for (m = 0; m < t; m++)
			{
						pthread_create(&tid[m], 0, Writer, (void *)(intptr_t)(m));   //creating m threads
				}

			 for (m = 0; m < t; m++)
			 {
					 pthread_join(tid[m], NULL);    //joining the m thread with the main thread
			 }
			  gettimeofday(&end1,NULL);


			/* Print the inverse matrix */
				// printf("The Inverse Matrix\n");
		    // for(m=0;m<=n;m++)
				// {
				// 	for(j=0;j<=n;j++)
				// 		cout << s[m][j]<<" ";
				// 	cout <<endl;
				// }
				float y[n+1];
     		for(m=0;m<=n;m++)
							y[m]=0;

				struct timeval start , end ;

				gettimeofday(&start,NULL);
				float x;
				for(int p=0;p<=n;p++)
				{
					for(i=0;i<=n;i++)
						{
							x=0.0;
							for(j=0;j<=i-1;j++)
									 {
										 x=x+D[i][j]*y[j];
									 }
							if(i==p)
								y[i]=(1.0-x);
							else
								y[i]=(0.0-x);

						 }

					for(i=n;i>=0;i--)
						{
							x=0.0;
						 	for(j=i+1;j<=n;j++)
									 {
										 x=x+D[i][j]*C[j][p];
									 }
						 	C[i][p]=(y[i]-x)/D[i][i];
						}
				}

				gettimeofday(&end,NULL);

				cout <<"Time taken by Parallel method :" << (end1.tv_sec * 1000000 + end1.tv_usec)- (start1.tv_sec * 1000000 + start1.tv_usec)<<endl;
			 	cout <<"Time taken by Sequential method :" << (end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)<<endl;


   }

/* The function that calcualtes the LU deomposed matrix.
Note that it receives the matrix as a two dimensional array
of pointers. Any change made to [D] here will also change its
value in the main function. So there is no need of an explicit
"return" statement and the function is of type "void". */
