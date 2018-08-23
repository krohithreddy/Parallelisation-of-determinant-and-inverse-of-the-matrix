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

double det=1;
int flag=0;
int n,m ;
float **a;
float **b,**c,**d;
mutex lock1;
atomic <float*> Level;
 atomic <int> size,broke ,levelquantity ,level,sum ;
//int broke,level;


void diagonal(int n,float **a,float **b,int row,int column){
  int i,j,k;
      i = row ;
      k =column;

              double t=a[k][i]/a[i][i];
              if(i!=k)
                for (j=0;j<n;j++){
                  a[k][j]=a[k][j]-t*a[i][j];
                  b[k][j]=b[k][j]-t*b[i][j];
                }
      size-- ;

}
void unit(int n,float **a,float **b,int row,int column){
  int i,j,k;
  // for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
      i = row ;
      k =column;
              if(i==k)
                {
                  double t =a[i][i];
                  for (j=0;j<n;j++){
                    a[k][j]=a[k][j]/t;
                    b[k][j]=b[k][j]/t;
                  }

                }

                //make the elements below the pivot elements equal to zero or elimnate the variables
      //    }

}
void  Seq_inversion(){ //  sequential computation
  //int i=0;
  for (int i=0;i<n;i++)
        for(int j=0;j<n;j++)
          diagonal(n,c,d,i,j);
  for(int j=0;j<n;j++)
    unit(n,c,d,j,j);
}
void * Writer(void* ttid)
{
  long id = (long ) ttid;   //thread id
  for (int i=0;i<n;i++)
  {
    if(!(i==0)){
    while(!(levelquantity==i*m)){
      if(i<level)
        break;
    }
  }
  level=i+1;
        for(int j=id,k=1;j<n;j = k*m + (id) , k++){     //assign each thread few rows
         diagonal(n,a,b,i,j);
       }
      levelquantity++;
}
  for(int j=id,k=1;j<n;j = k*m + (id) , k++){   // assign each thread few diagonal elements
    unit(n,a,b,j,j);
  }

  pthread_exit(NULL);

}



int main()
{

    int i,j,k;
    cout.precision(4);        //set precision
    cout.setf(ios::fixed);
    cout<<"\nEnter the order(n): \n";
    cin>>n;                //input the no. of equations
    cout<<"\nEnter number of threads (m): \n";
    cin>>m;                //input the no. of equations
  //  float a[n][n];        //declare an array to store the elements of augmented-matrix

      srand (time(NULL));
       a = new float*[n];
       b = new float*[n];
       c = new float*[n];
       d = new float*[n];
       Level = new float[n+1];
       sum=(m*(m+1))/2;
       Level[0]=0;



       for(int i=0; i<n; i++)
       {
          a[i] = new float[n];
          b[i] = new float[n];
          c[i] = new float[n];
          d[i] = new float[n];
          Level[i+1]=0;
       }
    for (i=0;i<n;i++)
        for (j=0;j<n;j++){
          a[i][j] = rand() % 10 + 1;;   //input the elements of array
          c[i][j] = a[i][j];
        }

          for (i=0;i<n;i++)                 // identity matrix
              for (j=0;j<n;j++){
                if(i==j){
                  b[i][j] = 1;
                  d[i][j]=1;
                }
                else{
                  b[i][j] = 0;
                  d[i][j]=0;
              }
            }

    for (i=0;i<n;i++)                    //Pivotisation
        for (k=i+1;k<n;k++)
            if (abs(a[i][i])<abs(a[k][i])){
            	flag++;
            	for (j=0;j<n;j++){
                    double temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                    double temp1=c[i][j];
                    c[i][j]=c[k][j];
                    c[k][j]=temp1;
                }
			}
      for (i=0;i<n;i++)                    //Pivotisation
          for (k=i+1;k<n;k++)
              if (abs(b[i][i])<abs(b[k][i])){
                flag++;
                for (j=0;j<n;j++){
                      double temp=b[i][j];
                      b[i][j]=b[k][j];
                      b[k][j]=temp;
                      double temp1=d[i][j];
                      d[i][j]=d[k][j];
                      d[k][j]=temp1;
                  }
        }
   struct timeval start , end ;
   gettimeofday(&start,NULL);
   Seq_inversion();
   gettimeofday(&end,NULL);
    size = n;
    broke = -1;
    levelquantity=0;
    level =0;
    pthread_t tid[m];    //for m threads
    pthread_attr_t attr; 							//allocated the attribute to thread
    pthread_attr_init(&attr);
    struct timeval start1 , end1 ;
    gettimeofday(&start1,NULL);
    for (int i = 0; i < m; i++) {

          pthread_create(&tid[i], 0, Writer, (void *)(intptr_t)(i));   //creating m threads

      }

     for (int i = 0; i < m; i++)
     {
         pthread_join(tid[i], NULL);    //joining the m thread with the main thread
     }
     gettimeofday(&end1,NULL);
   cout <<"Time taken by Parallel method :" << (end1.tv_sec * 1000000 + end1.tv_usec)- (start1.tv_sec * 1000000 + start1.tv_usec)<<endl;
  cout <<"Time taken by Sequential method :" << (end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)<<endl;
    return 0;
}
