
#include <iostream>
#include <memory>
#include <iostream>
#include <vector> 
#include <chrono>
#include <thread>
#include <future>
#include <queue>
using namespace std;
using namespace chrono_literals;

struct node
    {
        node(float in) :contents(in) { dist = INFINITY; visited = false; };
        float contents;
        vector<std::pair<int,int>> conections;
        float dist = INFINITY;
        bool visited = false;
        node* prev = nullptr;
    };

class graph
{
    
    

public:
    std::vector<node> nodes;
    template<typename ... t2>
    void addnode(float contents, t2...conections)
    {
        nodes.emplace_back(contents);
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

   /* void addlink(int first,int second)
    {
        nodes[first].conections.push_back(second);
    }
   
    void addlink2(int first, int second)
    {
        nodes[first].conections.push_back(second);
        nodes[second].conections.push_back(first);
    }*/
    struct compare
    {
        bool operator()(node lhs, node rhs)
        {
            if (lhs.dist > rhs.dist) return true;
            else return false;
        }
    };
    std::vector<node>* FindRoute(int start, int end)
    {
        std::vector<node>* UnvisitedNodes = new std::vector<node>(nodes);
        UnvisitedNodes->at(start).dist = 0;
        while (std::find_if(UnvisitedNodes->begin(), UnvisitedNodes->end(), [](node n) {return !n.visited; }) != UnvisitedNodes->end())
        {
            
            node* temp = new node(100);
            node* u = temp;
            for (unsigned int i=0;i<UnvisitedNodes->size();i++)
            {
                if (UnvisitedNodes->at(i).dist < u->dist && !UnvisitedNodes->at(i).visited) u = &UnvisitedNodes->at(i);
            }
            delete temp;
            u->visited = true;
            for (unsigned int n = 0; n < u->conections.size(); n++)
            {
                node* v = &UnvisitedNodes->at(u->conections[n].first);
                if (!v->visited) //if the Node hasnt been visted already
                {
                    float alt = u->dist + u->conections[n].second;
                    if (alt < v->dist)
                    {
                        v->dist = alt;
                        v->prev = u;
                    }
                }
            }
        }
        return UnvisitedNodes;
    }

};




template<typename T = int, size_t N>
void insertsort(T (&array)[N])
{
    for (unsigned int i = 1; i < N; i++)
    {
        int j = i;
        while (j >= 0 && array[j - 1] > array[j])
        {
            std::swap(array[j], array[j - 1]);
            j -= 1;
        }
    }
}
void insertsort(int* array,size_t N)
{
    for (unsigned int i = 1; i < N; i++)
    {
        int j = i;
        while (j >= 0 && array[j - 1] > array[j])
        {
            std::swap(array[j], array[j - 1]);
            j -= 1;
        }
    }
}
int* merge(  int* a,   int* b,   int as,   int bs)
{
    //also bad cos result never freed :c
      int ia = 0;
      int ib = 0;
      int s = as + bs;
      int* result = new   int[s];
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
int* mergesort(  int* a, size_t sa)
{
    //this is horrible garbage no good very bad cos left + right never freed :c
    if (sa <= 1) return &a[0];
      int il = 0;
      int ir = 0;
      int* left = new   int[(sa + 1)/2];
      int* right = new   int[sa / 2];
   
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
void multimergesort(  int* a, size_t sa, std::promise<  int*> && ret)
{
    //this is horrible garbage no good very bad cos left + right never freed :c
    if (sa <= 1) { ret.set_value(a); return; };
    int il = 0;
    int ir = 0;
    int* left = new   int[(sa + 1) / 2];
    int* right = new   int[sa / 2];

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
        std::promise<  int*> leftp;
        std::promise<  int*> rightp;
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
bool VerifySort(int* arr, size_t N)
{
    if (N == 1 || N == 0) return 1;
    if (arr[N-1] > arr[N - 2]) return 0;

    return VerifySort(arr, N - 1);
}
typedef std::pair<int, int> p; //first is node connection second is weight

int main()
{
    graph g;
    g.addnode(1, p(1,2));
    g.addnode(11, p(0, 2), p(2,2));
    g.addnode(3, p(1, 1));
    g.addnode(12, p(2, 3));



    std::vector<node>* ns = g.FindRoute(0, 2);
    int n = 2;
    node* target = &ns->at(2);
    while (target != nullptr)
    {
        
        std::cout << target->contents << std::endl;
        target = target->prev;
    }

    return 0;
    /*{
        int size = 30000;
        srand(time(NULL));
        int* array = new int[size];
        for (unsigned int i = 0; i < size; i++)
        {
            array[i] = rand();
        }

        auto t1 = std::chrono::high_resolution_clock::now();

        //insertsort(array,size);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "non parralel time: ";
        std::cout << (t2 - t1).count() << "\n";
        std::cout << std::endl;
        for (int i = 0; i < size; i++)
        {
            //std::cout << array[i] << " ";
        }
        std::cout << std::endl;


        t1 = std::chrono::high_resolution_clock::now();
        std::promise<  int*> d;
        std::future f_d = d.get_future();
        multimergesort(array, size, std::move(d));
        int* res = f_d.get();

        t2 = std::chrono::high_resolution_clock::now();
        std::cout << std::endl;
        std::cout << "parallel time: ";
        std::cout << (t2 - t1).count() << std::endl;
        std::cout << std::endl;
        //  for (unsigned int i = 0; i < size; i++)std::cout << res[i] << " ";
        return 0;
    }
    */
}
