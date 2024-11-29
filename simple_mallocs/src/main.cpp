#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Allocator.h"

using namespace std;


int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<void*> allocations;

    cout << "IT'S SIMPLE MALLOCS\n";
    
    const int num_allocations_1 = 100000;
    const int max_block_size_1 = 4096;
    // const int max_block_size_2 = 250;


    
    for (int i = 0; i < num_allocations_1; ++i)
    {
        cout << "#" << i << endl;
        size_t block_size = rand() % max_block_size_1 + 1;
        void* ptr = malloc(block_size);
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
            free(allocations[index]);
            // cout << "freed another allocated block...\n";
            allocations.erase(allocations.begin() + index);
        }
    }
    
    for (void* ptr : allocations)
    {
        free(ptr);
    }

    cout << "Fragmentation test completed." << endl;
    return 0;
}
