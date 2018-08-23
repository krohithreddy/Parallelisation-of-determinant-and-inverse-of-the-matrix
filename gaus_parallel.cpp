#include<iostream>
#include<iomanip>
#include<cmath>
#include<atomic>
#include <chrono>
#include <thread>
#include<mutex>

using namespace std;
using namespace std::chrono;

double det=1;
int flag=0;
int n,m ;
float **a;
mutex lock1;
// atomic <int> count =0  ;
int size,broke;

void determinant(int n,float **a,int row,int column){
  int i,j,k;
  // for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
      i = row ;
      k =column;
      // for (k=i+1;k<n;k++)
      //     {
              double t=a[k][i]/a[i][i];
              for (j=0;j<n;j++)
                  a[k][j]=a[k][j]-t*a[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
      //    }
      lock1.lock();
      size-- ;
        //cout<<"decount"<<size<<"for"<<column<<"\n";
        lock1.unlock();
}

void * Writer(void* ttid)
{
  long id = (long ) ttid;   //thread id
  for (int i=0;i<n-1;i++)  {
    for(int j=i+1+id,k=1;j<n;j = k*m + (id+i+1) , k++){
    //  lock1.lock();
      //  cout<<"thread "<<id+1<<"value "<< j<<"\n";
      //  lock1.unlock();
     determinant(n,a,i,j);
   }
   while(size >0){             // wait for all threads to finish a column
      if(broke==i){
          break;
      }
     if(i==n-2){
         cout<<"final round "<<id<<"by II "<<i<<"\n";
           break;
       }

   }
   // pthread_mutex_lock(&lock1);
   lock1.lock();
   if (size<1&&size!=-2) {
     if(i==n-2){
       cout<<"last one "<<id<<"by I "<<i<<"\n";
       size=-2;
       broke=i;
     }
    else{
      size = n-i-2;
      broke=i;
      //  cout<<"set count"<<size<<"by i "<<i<<"\n";
    }

   }
   // pthread_mutex_unlock(&lock1);
   lock1.unlock();
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
  size = n-1;
  broke = -1;
      srand (time(NULL));
       a = new float*[n];

       for(int i=0; i<n; i++)
       {
          a[i] = new float[n];
       }
    for (i=0;i<n;i++)
        for (j=0;j<n;j++)
          a[i][j] = rand() % 10 + 1;;   //input the elements of array

    for (i=0;i<n;i++)                    //Pivotisation
        for (k=i+1;k<n;k++)
            if (abs(a[i][i])<abs(a[k][i])){
            	flag++;
            	for (j=0;j<n;j++){
                    double temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                }
			}

    // cout<<"\nThe matrix after Pivotisation is:\n";
    // for (i=0;i<n;i++)            //print the new matrix
    // {
    //     for (j=0;j<n;j++)
    //         cout<<a[i][j]<<setw(16);
    //     cout<<"\n";
    // }

    pthread_t tid[m];    //for m threads
    for (int i = 0; i < m; i++) {
        pthread_attr_t attr; 							//allocated the attribute to thread
      pthread_attr_init(&attr);
          pthread_create(&tid[i], 0, Writer, (void *)(intptr_t)(i));   //creating m threads

      }
     for (int i = 0; i < m; i++)
     {
         pthread_join(tid[i], NULL);    //joining the m thread with the main thread
     }

    // for (i=0;i<n-1;i++)
    //   determinant(n,a,i);

    cout<<"\n\nThe matrix after gauss-elimination is as follows:\n";

    for (i=0;i<n;i++)            //print the new matrix
    {
        for (j=0;j<n;j++)
            cout<<a[i][j]<<setw(16);
        cout<<"\n";
    }

  for(i=0;i<n;i++){
    det=det*a[i][i];
  }
  if (flag%2==0){
    det=det;
  }else{
    det=-det;
  }
  cout<<"\n\n The determinant is: "<<det << endl;

    return 0;
}
