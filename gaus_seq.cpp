#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;

double det=1;
int flag=0;

void determinant(int n,float **a){
  int i,j,k;
  for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
      for (k=i+1;k<n;k++)
          {
              double t=a[k][i]/a[i][i];
              for (j=0;j<n;j++)
                  a[k][j]=a[k][j]-t*a[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
          }
}
int main()
{
    int n,i,j,k;
    cout.precision(4);        //set precision
    cout.setf(ios::fixed);
    cout<<"\nEnter the order(n): \n";
    cin>>n;                //input the no. of equations
  //  float a[n][n];        //declare an array to store the elements of augmented-matrix
      srand (time(NULL));
      float** a = new float*[n];

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

    cout<<"\nThe matrix after Pivotisation is:\n";
    for (i=0;i<n;i++)            //print the new matrix
    {
        for (j=0;j<n;j++)
            cout<<a[i][j]<<setw(16);
        cout<<"\n";
    }
    determinant(n,a);
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
