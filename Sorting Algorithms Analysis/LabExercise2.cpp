#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <fstream>
using namespace std;
typedef chrono::milliseconds MSEC;
typedef chrono::high_resolution_clock HRC;
using namespace std;
//n^2 algorithms
void selectionSort(int arr[], int s);
void bubbleSort(int arr[], int s);
void insertionSort(int arr[], int s);
//nlogn algorithms
void mergeSort(int a[],int start,int end);
void mergeArr(int a[],int start, int mid, int end);
void quickSort(int a[],int start, int end);

int main()
{
    fstream myFile;

    srand(time(NULL));
    int s = 10000;
    for(int i = s; i < 210000; i+=10000)
    {
        int arr[i];
        for(int j = 0; j < i; j++)
        {
            arr[j] = rand()%100;
        }
        auto start = HRC::now();
        /*
        myFile.open("BubbleSortRuntimes.txt", ios::app);
        bubbleSort(arr, i);
        myFile.open("MergeSortRuntimes.txt", ios::app);
        mergeSort(arr, 0, i);
        myFile.open("SelectionSortRuntimes.txt", ios::app);
        selectionSort(arr, i);
        myFile.open("InsertionSortRuntimes.txt", ios::app);
        insertionSort(arr, i);
        myFile.open("QuickSortRuntimes.txt", ios::app);

        quickSort(arr, 0, i);*/

        auto end = HRC::now();
        auto executeTime =
            chrono::duration_cast<MSEC>(end - start);
        cout << "Execution Time: " << ((double)executeTime.count()/1000.00) << " secs" << endl;
        //return 0;
        myFile << ((double)executeTime.count()/1000.00) << endl;
        myFile.close();
    }
    return 0;
}

void bubbleSort(int a[], int s)
{
    int i=0;
    bool swp = 1;

    while (swp)
    {
        swp = 0;
        for (int j=s - 1;j>i;j--)
        {
            if (a[j]<a[j-1])
            {
                swap (a[j],a[j-1]);
                swp = 1;
            }
        }
        i++;
    }

}

void selectionSort(int a[], int s)
{
    int small,idx_small;

    for (int i=0; i< s - 1;i++)
    {
        idx_small = i;
        for (int j=i+1;j < s;j++)
        {
            if (a[j]<a[idx_small])
            {
                idx_small = j;
            }
        }
    swap(a[i],a[idx_small]);
    }

}


void insertionSort(int a[], int s)
{
     int key,j;
     for (int i=0;i<s;i++)
     {
        key=a[i];
        j=i-1;
        while(key<a[j]&&j>=0)
        {
            a[j+1]=a[j];
            j--;
        }
        a[j+1] = key;
     }
     /*for (int idx=0;idx<s;idx++)
            cout<<a[idx]<<",";*/
        //cout<<endl;
}



void quickSort(int a[],int start, int end)
{
    int i, j, pivot;
    if (start<end)
    {
        pivot = a[start];
        i=start+1;
        while(a[i]<pivot)
            i++;
        j=end;
        while(a[j]>pivot)
            j--;
        while(i<j)
        {
            swap(a[i],a[j]);
            i++;
            while(a[i]<pivot)
                i++;
            j--;
            while(a[j]>pivot)
                j--;
        }
     swap(a[start],a[j]);
    quickSort(a,start,j-1);
    quickSort(a,j+1,end);
    }
}


void mergeSort(int a[],int start,int end)
{
    //cout << "function entered";
    int mid;
    if(start<end)
    {
        mid = (start+end)/2;
        //cout<<start<<","<<mid<<",";

        mergeSort(a,start,mid);

        mergeSort(a,mid+1,end);

        mergeArr(a,start,mid,end);

    }
}

void mergeArr(int a[],int start, int mid, int end)
{
    int s,ptr1,ptr2;
    int i,j;

    s = end - start;
    int temp[end];
    ptr1=start;
    ptr2 =mid+1;

    j=0;
    /* The while loop below would execute  equal to the
     minimum of mid and (n-mid)
    */
    while(ptr1<mid+1&&ptr2<end+1)
    {
        if (a[ptr1]<a[ptr2])
        {
            temp[j]=a[ptr1];
            ptr1++;
            j++;
        }
        else
        {
            temp[j]=a[ptr2];
            ptr2++;
            j++;
        }
    }

    if(ptr1<mid+1)
    {
        for (i=j;i<s+1;i++)
        {
                temp[i]=a[ptr1];
                ptr1++;
        }
    }

    if(ptr2<end+1)
    {
            for (i=j;i<s+1;i++)
            {
                temp[i]=a[ptr2];
                ptr2++;
            }
    }

        for (j=0;j<s+1;j++)
            a[start+j]=temp[j];
}
