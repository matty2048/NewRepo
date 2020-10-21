
#include <iostream>
#include <memory>
#include <iostream>
#include <vector> 
#include <chrono>
#include <thread>
#include <future>
using namespace std;
using namespace chrono_literals;



class graph
{
    
    struct node
    {
        node(float in) :contents(in) {};
        float contents;
        vector<int> conections;
    };

public:
    std::vector<node> nodes;
    template<typename ... t2>
    void addnode(float contents, t2...conections)
    {
        nodes.push_back(contents);
        size_t s = sizeof...(t2);
        if (s != 0) addneighbour(&(*(nodes.end() - 1)), &conections...);
    };
    
    void addneighbour(node* n) { return; };
    template<typename T = int, typename ... T2>
    void addneighbour(node* n, T t1, T2 ... t2)
    {
        n->conections.emplace_back(*t1);
        addneighbour(n, t2...);
    }

    void addlink(int first,int second)
    {
        nodes[first].conections.push_back(second);
    }

    void addlink2(int first, int second)
    {
        nodes[first].conections.push_back(second);
        nodes[second].conections.push_back(first);
    }
};

template<typename T = int, size_t N>
void insertsort(T (&array)[N])
{
    for (unsigned int i = 1; i < N; i++)
    {
        int j = i;
        int target = array[i];
        while (j >= 0 && array[j - 1] > array[j])
        {
            array[j] = array[j - 1];
            array[j - 1] = target;
            j -= 1;
        }
    }
}

void insertsort(int* array,size_t N)
{
    for (unsigned int i = 1; i < N; i++)
    {
        int j = i;
        int target = array[i];
        while (j >= 0 && array[j - 1] > array[j])
        {
            array[j] = array[j - 1];
            array[j - 1] = target;
            j -= 1;
        }
    }
}


int* merge(int* a, int* b,int as,int bs)
{
    //also bad cos result never freed :c
    int ia = 0;
    int ib = 0;
    int s = as + bs;
    int* result = new int[s];
    int ir = 0;
    while(ir <= as+bs)
    {
        if (a[ia] <= b[ib] && (ia <= as-1))
        {
            result[ir] = a[ia];
            ia++;
            ir++;
        }
        else if(ib <= bs - 1)
        {
            result[ir] = b[ib];
            ib++;
            ir++;
        }
        else break;
    }
    while (ia < as) {
        result[ir] = a[ia];
        ia++;
        ir++;
    }
    while (ib < bs)
    {
        result[ir] = b[ib];
        ib++;
        ir++;
    }

    return result;
}
int* mergesort(int* a, size_t sa)
{
    //this is horrible garbage no good very bad cos left + right never freed :c
    if (sa <= 1) return &a[0];
    int il = 0;
    int ir = 0;
    int* left = new int[(sa + 1)/2];
    int* right = new int[sa / 2];
   
    for (unsigned int i = 0; i < sa; i++)
    {
        if (i < (sa+1) / 2)
        {
            left[il] = a[i];
            il++;
        }
        else
        {
            right[ir] = a[i];
            ir++;
        }
    }

    left = mergesort(left, (sa + 1) / 2);
    right = mergesort(right, sa / 2);
    return merge(left, right,(sa+1)/2,sa/2);
}
void multimergesort(int* a, size_t sa, std::promise<int*> && ret)
{
    //this is horrible garbage no good very bad cos left + right never freed :c
    if (sa <= 1) { ret.set_value(a); return; };
    int il = 0;
    int ir = 0;
    int* left = new int[(sa + 1) / 2];
    int* right = new int[sa / 2];

    for (unsigned int i = 0; i < sa; i++)
    {
        if (i < (sa + 1) / 2)
        {
            left[il] = a[i];
            il++;
        }
        else
        {
            right[ir] = a[i];
            ir++;
        }
    }
    if (sa > 800000/4)
    {
        std::promise<int*> leftp;
        std::promise<int*> rightp;
        auto f_left = leftp.get_future();
        auto f_right = rightp.get_future();
        std::thread l(&multimergesort,left, (sa + 1) / 2,std::move(leftp));
        std::thread r(&multimergesort, right, (sa ) / 2, std::move(rightp));

        l.join();
     
        r.join();
     
       
        left = f_left.get();
        right = f_right.get();
        
        
    }
    else
    {

        left = mergesort(left, (sa + 1) / 2);
        right = mergesort(right, sa / 2);
    }
    int* result = merge(left, right, (sa + 1) / 2, sa / 2);
    ret.set_value(result);
    return;
}




template<typename T, size_t as, size_t bs>
int* merge(T (&a)[as], T (&b)[bs])
{
    int ia = 0;
    int ib = 0;
    int ir = 0;
    int s = as + bs;
    int* result = new int[s];
    while (ir <= as + bs)
    {
        if (a[ia] <= b[ib] && (ia <= as - 1))
        {
            result[ir] = a[ia];
            ia++;
            ir++;
        }
        else if (ib <= bs - 1)
        {
            result[ir] = b[ib];
            ib++;
            ir++;
        }
        else break;
    }
    while (ia < as) {
        result[ir] = a[ia];
        ia++;
        ir++;
    }
    while (ib < bs)
    {
        result[ir] = b[ib];
        ib++;
        ir++;
    }
    return result;
}




int main()
{
    
    int size = 500000;
    
    int* array = new int[size];
    for (unsigned int i = 0; i < size; i++)
    {
        int z = rand();
        memcpy(&(z), &(array[i]), sizeof(int));
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    mergesort(array,size);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < sizeof(array) / sizeof(int); i++)
    {
        //std::cout << array[i] << " ";
    }
    std::cout << (t2 - t1).count() << "\n" ;
    int arr[] = { 1,3,2,5,7,4,6,8,90,81,56,32 };

    int a[] = { 3,4,5,6,7,8 };
    int b[] = { 2 };
    //int* c = merge(a,b, int(sizeof(a)/sizeof(a[0])), sizeof(b)/sizeof(b[0])); //merges two sorted lists
    
    int* c = merge(a, b);
    //for (unsigned int i = 0; i < sizeof(a) / sizeof(a[0]) + sizeof(b) / sizeof(b[0]); i++) std::cout << c[i] << " ";

    int e[] = { 2,1,3,4,99,98,87 };
    t1 = std::chrono::high_resolution_clock::now();
    std::promise<int*> d;
    std::future f_d = d.get_future();
    multimergesort(array,size,std::move(d));
    int* res = f_d.get();

    t2 = std::chrono::high_resolution_clock::now();
    std::cout << (t2 - t1).count(); 
    std::cout << "\n";
    //for (unsigned int i = 0; i < size; i++)std::cout << res[i] << " ";
    return 0;
}
