#include <iostream>
#include <cstdlib>
#include <sys/resource.h>


using namespace std;

int main() 
{
    size_t big_size = 1024 * 110;
    void* ptr = malloc(big_size);

    if (ptr == nullptr) {
        cout << "Failed to allocate memory." << endl;
        return 1;
    } else {
        cout << "Memory allocated, but not yet used." << endl;
    }

    // Попытка использовать выделенную память
    char* data = static_cast<char*>(ptr);
    data[0] = 'a'; // Только здесь ОС попробует выделить физическую память

    cout << "First byte set. Memory now in use." << endl;

    // Освобождение памяти
    free(ptr);
    return 0;
}
