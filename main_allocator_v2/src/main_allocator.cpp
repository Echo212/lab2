#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Allocator.h"

using namespace std;



class Class1
{
    DECLARE_ALLOCATOR

    private:
    char v1[1000];
};


int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<void*> class1_vec;
    vector<void*> class2_vec;

    cout << "DEBUG: sizeof(Class1) = " << sizeof(Class1) << endl;

    
    const int num_allocations = 100000;

    Allocator allocatorHeapBlocks(100, 100);
    
    for (int i = 0; i < num_allocations; ++i)
    {
        cout << "#" << i << endl;
        void* ptr = allocatorHeapBlocks.Allocate(sizeof(Class1));
        if (ptr)
        {
            class1_vec.push_back(ptr);
            // cout << "added another allocated block...\n";
        }

        else
        {
            cerr << "if ptr == false!!" << endl;
            // return 1;
        }
        
        if (!class1_vec.empty() && rand() % 2 == 0)
        {
            int index = rand() % class1_vec.size();
            allocatorHeapBlocks.Deallocate(class1_vec[index]);
            // cout << "freed another allocated block...\n";
            class1_vec.erase(class1_vec.begin() + index);
        }
    }
    
    for (void* ptr : class1_vec)
    {
        allocatorHeapBlocks.Deallocate(ptr);
    }
    return 0;
}
