#include<iostream>
#include<omp.h>
#include<climits>
using namespace std;

int main()
{
    int n;
    cout<<"enter the number of element:";
    cin>>n;

    int arr[n];

    cout<<"enter the element:\n";
    for(int i = 0;i<n;i++)
    {
        cin>>arr[i];
    }

    int sum = 0;
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for(int i = 0;i<n;i++)
    {
        sum += arr[i];

        if(arr[i] < min_val)
            min_val = arr[i];

        if(arr[i] > max_val)
            max_val = arr[i];
    }

    float avg = (float)sum / n;

    cout<<"sum:"<<sum<<endl;
    cout<<"min_val:"<<min_val<<endl;
    cout<<"max_val:"<<max_val<<endl;
    cout<<"avg:"<<avg<<endl;

    return 0;
}