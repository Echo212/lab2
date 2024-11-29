#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Allocator.h"

using namespace std;


int main()
{
    cout << "IT'S ALLOCATOR STATIC POOL\n";
    srand(static_cast<unsigned int>(time(nullptr)));

    static const int MAX_BLOCKS_1 = 100000;
    // static const int MAX_BLOCKS_2 = 100000;
    static const int MAX_BLOCK_SIZE_1 = 4096;
    // static const int MAX_BLOCK_SIZE_2 = 256;

    static AllocatorPool<char[MAX_BLOCK_SIZE_1], MAX_BLOCKS_1> allocatorStaticPool;
    static Allocator allocatorHeapBlocks(MAX_BLOCK_SIZE_1);

    vector<void*> allocations;
    
    for (int i = 0; i < MAX_BLOCKS_1; ++i)
    {
        // cout << "#" << i << endl;
        size_t block_size = rand() % MAX_BLOCK_SIZE_1 + 1;
        void* ptr = allocatorStaticPool.Allocate(block_size);
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
