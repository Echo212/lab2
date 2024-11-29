#include <iostream>
#include <cstring>

struct Pixel {
    unsigned char r, g, b;
    Pixel(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) {}
};

struct BMP {
    unsigned char info[54];
    Pixel* data;
    int width;
    int height;

    BMP(int w, int h) : width(w), height(h) {
        memset(info, 0, sizeof(info));
        info[0] = 'B';
        info[1] = 'M';
        *(int*)(&info[2]) = 54 + width * height * 3;
        *(int*)(&info[10]) = 54;
        *(int*)(&info[14]) = 40;
        *(int*)(&info[18]) = width;
        *(int*)(&info[22]) = height;
        info[26] = 1;
        info[28] = 24;

        data = new Pixel[width * height];
    }

    BMP(const BMP& other) : width(other.width), height(other.height) {
        memcpy(info, other.info, sizeof(info));
        data = new Pixel[width * height];
        memcpy(data, other.data, width * height * sizeof(Pixel));
    }

    BMP& operator=(const BMP& other) {
        if (this != &other) {
            delete[] data;
            width = other.width;
            height = other.height;
            memcpy(info, other.info, sizeof(info));
            data = new Pixel[width * height];
            memcpy(data, other.data, width * height * sizeof(Pixel));
        }
        return *this;
    }

    // BMP(BMP&& other) noexcept : width(other.width), height(other.height), data(other.data) {
    //     memcpy(info, other.info, sizeof(info));
    //     other.data = nullptr;
    // }

    // BMP& operator=(BMP&& other) noexcept {
    //     if (this != &other) {
    //         delete[] data;
    //         width = other.width;
    //         height = other.height;
    //         memcpy(info, other.info, sizeof(info));
    //         data = other.data;
    //         other.data = nullptr;
    //     }
    //     return *this;
    // }

    ~BMP() {
        delete[] data;
    }

    void fillWithBlack() {
        for (int i = 0; i < width * height; ++i) {
            data[i] = Pixel(0, 0, 0);
        }
    }
};

int main() {
    int width = 800;
    int height = 600;
    int numImages = 4;

    BMP** bmps = new BMP*[numImages]; // Массив указателей на BMP

    for (int i = 0; i < numImages; ++i) {
        bmps[i] = new BMP(width, height); // Выделяем новый BMP
        bmps[i]->fillWithBlack();         // Заполняем черным
        std::cout << "Image " << i + 1 << " created and filled with black.\n";
    }

    // Освобождаем память для каждого объекта BMP и массива указателей
    for (int i = 0; i < numImages; ++i) {
        delete bmps[i];
    }
    delete[] bmps;

    return 0;
}
