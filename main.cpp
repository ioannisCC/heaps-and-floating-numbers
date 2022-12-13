#include <iostream>
#include <functional>
#include <random>
#include <cmath>

using namespace std;

bool compare_float(float x, float y, float epsilon = 0.01f);

template<class T>
class MaxHeap{
public:
    MaxHeap(int MaxSize = 10);
    ~MaxHeap() { delete[] heap; }
    int Size() const { return CurrentSize; }
    MaxHeap<T>& InsertMax(const T& x);
    MaxHeap<T>& DeleteMax(T& x);
    void Initialize(T a[], int size, int ArraySize);
private:
    int CurrentSize,MaxSize;
    T* heap;
};

template<class T>
class MinHeap{
public:
    MinHeap(int MinSize = 10);
    ~MinHeap() { delete[] heapm; }
    int Size() const { return CurrentSize; }
    T Min(int arraysize)
    {
        if (CurrentSize==0)
            throw out_of_range("error");
        return heapm[arraysize];
    }
    MinHeap<T>& InsertMin(const T& x);
    MinHeap<T>& DeleteMin(T& x);
    void Initialize(T a[], int size, int ArraySize);
private:
    int CurrentSize,MinSize;
    T* heapm;
};

int main() {
    int n;

    cout<<"provide size of the heap: "<<endl;
    cin>>n;
    while(n<=0)
    {
        cout<<"provide a valid size of the heap (>0): "<<endl;
        cin>>n;
    }

    default_random_engine generator;
    chi_squared_distribution<float> my_distribution(0.5);
    auto random_num = bind(my_distribution, generator);

    float* heap_min;
    heap_min = new float[n+1];  //create dynamic array

    for (int i = 1; i < n+1; i++) {
        heap_min[i] = random_num()* 10000.0;  //assign values to the dynamic array
        //cout<<heap_min[i]<<endl;
    }

    float* heap_max;
    heap_max = new float[n+1];  //create dynamic array

    for (int i = 1; i< n+1; i++)
    {
        heap_max[i] = heap_min[i];  //copy each value form heap_min to heap_max
    }

    //create the two heap
    MaxHeap<float> maxheap;
    MinHeap<float> minheap;

    //initialize the two heap by passing the heap_max & heap_min as arguments and theiw sizes
    maxheap.Initialize(heap_max,n,n);
    minheap.Initialize(heap_min,n,n);


    float addMin1,addMin2,SumMin;
    while(true)
    {
        minheap.DeleteMin(addMin1); //deletes the min element and assigns it to addMin1 and then it restructures the heap
        minheap.DeleteMin(addMin2); //deletes the min element and assigns it to addMin2 and then it restructures the heap

        SumMin = addMin1 + addMin2; //add the two extracted numbers
        minheap.InsertMin(SumMin);  //inserts the sum and restructures the heap

        if (minheap.Size() == 1)
        {
            minheap.DeleteMin(SumMin);  //assigns the last number to SumMin which is the Sum of all numbers added together and deletes that number
            cout<<fixed;  //in order to show numbers in full length and not in scientific representation
            cout<<"sum of min heap is: "<<SumMin<<endl;
            break;
        }
    }

    float addMax1,addMax2,SumMax;
    while(true)
    {
        maxheap.DeleteMax(addMax1); //deletes the max element and assigns it to addMax1 and then it restructures the heap
        maxheap.DeleteMax(addMax2); //deletes the max element and assigns it to addMax2 and then it restructures the heap

        SumMax = addMax1 + addMax2; //add the two extracted numbers
        maxheap.InsertMax(SumMax);  //inserts the sum and restructures the heap

        if (maxheap.Size() == 1)
        {
            maxheap.DeleteMax(SumMax);  //assigns the last number to SumMax which is the Sum of all numbers added together and deletes that number
            cout<<fixed;  //in order to show numbers in full length and not in scientific representation
            cout<<"sum of max heap is: "<<SumMax<<endl;
            break;
        }
    }

    cout<<(compare_float(SumMin,SumMax,0.001f) ? "they are equal" : "they are not equal")<<endl;

    delete [] heap_min;
    delete [] heap_max;

    return 0;
}

template<class T>
MaxHeap<T>::MaxHeap(int MaxSize)
{   //constructor
    this->MaxSize = MaxSize;
    heap = new T[MaxSize+1];
    CurrentSize = 0;
}

template<class T>
MinHeap<T>::MinHeap(int MinSize)
{   //constructor
    this->MinSize = MinSize;
    heapm = new T[MinSize+1];
    CurrentSize = 0;
}

template<class T>
MaxHeap<T>& MaxHeap<T>::InsertMax(const T& x)
{   //insert x into the max heap
    if (CurrentSize == MaxSize)
        throw out_of_range("no memory left");

    //find place for x
    //'i' starts at new leaf and moves up tree
    int i = ++CurrentSize;
    while (i != 1 && x > heap[i>>1])
    {
        //cannot put x in heap[i]
        heap[i] = heap[i>>1];  //move element down (i>>1 we shift the bit by one so is same as dividing by 2)
        i /= 2;   //move to parent
    }

    heap[i] = x;
    return *this;
}

template<class T>
MinHeap<T>& MinHeap<T>::InsertMin(const T& x)
{   //insert x into the min heap
    if (CurrentSize == MinSize)
        throw out_of_range("no memory left");

    //find place for x
    //'i' starts at new leaf and moves up tree
    int i = ++CurrentSize;
    while (i != 1 && x < heapm[i>>1])
    {
        //cannot put x in heap[i]
        heapm[i] = heapm[i>>1];  //move element down
        i /= 2;  //move to parent
    }

    heapm[i] = x;
    return *this;
}

template<class T>
MaxHeap<T>& MaxHeap<T>::DeleteMax(T& x)
{   //set x to max element and delete
    //max element from heap
    //check if heap is empty
    if (CurrentSize == 0)
        throw out_of_range("error"); // empty
    x = heap[1]; // max element
    //restructure heap
    T y = heap[CurrentSize--]; // last element
    //find place for y starting at root
    int i = 1, // current node of heap
    ci = 2; // child of i
    while (ci <= CurrentSize) {
        //heap[ci] should be largest child of i
        if (ci < CurrentSize && heap[ci] < heap[ci + 1])
            ci++;
        //can we put y in heap[i]?
        if (y >= heap[ci]) break; // yes
        //no
        heap[i] = heap[ci]; // move child up
        i = ci;
        ci *= 2; // move down a level
    }
    heap[i] = y;
    return *this;
}

template<class T>
MinHeap<T>& MinHeap<T>::DeleteMin(T& x)
{   //set x to min element and delete
    //max element from heap
    //check if heap is empty
    if (CurrentSize == 0)
        throw out_of_range("error"); // empty
    x = heapm[1]; // min element
    //restructure heap
    T y = heapm[CurrentSize--]; // last element
    //find place for y starting at root
    int i = 1, // current node of heap
    ci = 2; // child of i
    while (ci <= CurrentSize) {
        //heap[ci] should be largest child of i
        if (ci < CurrentSize && heapm[ci] > heapm[ci + 1])
            ci++;
        //can we put y in heap[i]?
        if (y <= heapm[ci]) break; // yes
        //no
        heapm[i] = heapm[ci]; // move child up
        i = ci;
        ci *= 2; // move down a level
    }
    heapm[i] = y;
    return *this;
}

template<class T>
void MaxHeap<T>::Initialize(T a[], int size, int ArraySize)
{   //initialize max heap to array a
    delete [] heap;
    heap = a;
    CurrentSize = size;
    MaxSize = ArraySize;
    //make into a max heap
    for (int i = CurrentSize>>1; i >= 1; i--) {
        T y = heap[i]; // root of subtree
        //find place to put y
        int c = 2*i; // parent of c is target
        //location for y
        while (c <= CurrentSize) {
            //heap[c] should be larger sibling
            if (c < CurrentSize && heap[c] < heap[c+1])
                c++;
            //can we put y in heap[c/2]?
            if (y >= heap[c]) break; // yes
            //no
            heap[c>>1] = heap[c]; // move child up
            c *= 2; // move down a level
        }
        heap[c>>1] = y;
    }
}

template<class T>
void MinHeap<T>::Initialize(T a[], int size, int ArraySize)
{   //initialize min heap to array a
    delete [] heapm;
    heapm = a;
    CurrentSize = size;
    MinSize = ArraySize;
    //make into a min heap
    for (int i = CurrentSize>>1; i >= 1; i--) {
        T y = heapm[i]; // root of subtree
        //find place to put y
        int c = 2*i; // parent of c is target
        //location for y
        while (c <= CurrentSize) {
            //heap[c] should be σμαλλερ sibling
            if (c < CurrentSize && heapm[c] > heapm[c+1])
                c++;
            //can we put y in heap[c/2]?
            if (y <= heapm[c]) break; // yes
            //no
            heapm[c>>1] = heapm[c]; // move child up
            c *= 2; // move down a level
        }
        heapm[c>>1] = y;
    }
}

//function that compares the two sums by checking if the absolute value of their differnece
//is less than epsilon defined as 0.1 (from math analysis definition)
bool compare_float(float x, float y, float epsilon)
{
    return ((fabs(x-y) < epsilon) ? true : false);
    //return true if they are the same, otherwise return false
}