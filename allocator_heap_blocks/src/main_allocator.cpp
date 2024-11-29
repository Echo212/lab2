#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Allocator.h"

using namespace std;


int main()
{
    cout << "IT'S ALLOCATOR HEAP BLOCKS\n";
    srand(static_cast<unsigned int>(time(nullptr)));

    static const int MAX_BLOCKS = 10000;
    static const int MAX_BLOCK_SIZE = 4096;

    // static AllocatorPool<char[MAX_BLOCK_SIZE], MAX_BLOCKS*2> allocatorStaticPool;
    static Allocator allocatorHeapBlocks(MAX_BLOCK_SIZE);
    vector<void*> allocations;
    
    for (int i = 0; i < MAX_BLOCKS; ++i)
    {
        // cout << "#" << i << endl;
        size_t block_size = rand() % MAX_BLOCK_SIZE + 1;
        void* ptr = allocatorHeapBlocks.Allocate(block_size);
        if (ptr)
        {
            allocations.push_back(ptr);
            // cout << "added another allocated block...\n";
        }

        else
        {
            cerr << "if ptr = false!!" << endl;
            // return 1;
        }
        
        if (!allocations.empty() && rand() % 2 == 0)
        {
            int index = rand() % allocations.size();
            allocatorHeapBlocks.Deallocate(allocations[index]);
            // cout << "freed another allocated block...\n";
            allocations.erase(allocations.begin() + index);
        }
    }
    
    for (void* ptr : allocations)
    {
        allocatorHeapBlocks.Deallocate(ptr);
    }
    return 0;
}
