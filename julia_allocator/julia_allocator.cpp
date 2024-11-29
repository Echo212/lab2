#include <iostream>
#include <cstring>
#include <string>
#include "Allocator/Allocator.h"
#include <cmath>
#include <fstream>
#include <chrono>
#include <functional>
#include <thread>

using namespace std;

struct Complex {
    double r;
    double i;
    Complex(double a, double b) : r(a), i(b) {}
    Complex() = default;
    double magnitude2() { return r * r + i * i; }
    double magnitude() { return sqrt(r * r + i * i); }

    Complex operator+(const Complex a) {
        return Complex(a.r + r, a.i + i);
    }

    Complex operator*(const Complex a) {
        return Complex(r * a.r - i * a.i, i * a.r + r * a.i);
    }

    friend ostream& operator<<(ostream& os, const Complex& c)
    {
        char sign = '+';
        os << c.r << " " << char(sign + 2 * signbit(c.i)) << " " << fabs(c.i) << "i";
        return os;
    }

};

struct Pixel
{
    DECLARE_ALLOCATOR
    public:
    unsigned char b;
    unsigned char g;
    unsigned char r;

    Pixel(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) : r(r), g(g), b(b) {}
};


class BMP
{
    DECLARE_ALLOCATOR
    public:
    unsigned char info[54];
    Pixel* data;
    int width;
    int height;

    BMP(int w, int h) : width(w), height(h), data(nullptr)
    {
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

    // ~BMP() {
    //     if (pixelAllocator && data) {
    //         pixelAllocator->Deallocate(data);
    //     }
    // }
};

// Функции для заполнения, генерации фракталов и записи изображений остаются прежними

int fill_black(BMP& bmp);
int cpu_julia(BMP& bmp, const Complex& c);
int write_bmp(BMP& bmp, string& filename);
int julia(double jx, double jy, const Complex& C);

// ===================================================================================

IMPLEMENT_ALLOCATOR(BMP, 0, 0)

template <class TimeT  = std::chrono::milliseconds,
          class ClockT = std::chrono::steady_clock>
struct measure
{
    template <class F, class ...Args>
    static auto duration(F&& func, Args&&... args)
    {
        auto start = ClockT::now();
        std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
        return std::chrono::duration_cast<TimeT>(ClockT::now()-start);
    } 
};

void benchmark()
{
    for (int i = 0; i < 10; ++i)
    {
        const int N1 = 400;
        const int N2 = 600;
        const int N = 4;


        BMP** bmps = new BMP*[N];

        Complex* Cs = new Complex[4];
        Cs[0] = Complex(-0.8, 0.156);
        Cs[1] = Complex(0.285, 0.01);
        Cs[2] = Complex(-0.0085, 0.71);
        Cs[3] = Complex(-0.74543, 0.11301);

        const string outname = "julia_";
        string curr_name;


        cout << "Processing images size of " << N1 << " x " << N1 << endl;
        for (int i = 0; i < 4; ++i) {
            cout << "Generating fractal for C = " << Cs[i] << "... ";
            bmps[i] = new BMP(N1, N1);
            cpu_julia(*bmps[i], Cs[i]);
            curr_name = outname + to_string(N1) + "_" + to_string(i) + ".bmp";
            // write_bmp(*bmps[i], curr_name);
            cout << "done.\n";
        }


        cout << "Processing images size of " << N2 << " x " << N2 << endl;
        for (int i = 0; i < 4; ++i) {
            cout << "Generating fractal for C = " << Cs[i] << "... ";
            bmps[i] = new BMP(N2, N2);
            cpu_julia(*bmps[i], Cs[i]);
            curr_name = outname + to_string(N2) + "_" + to_string(i) + ".bmp";
            // write_bmp(*bmps[i], curr_name);
            cout << "done.\n";
        }


        for (int i = 0; i < N; ++i)
        {
            delete bmps[i];
        }
        delete[] bmps;
        delete[] Cs;
    }
    
}

int main()
{
    
    cout << "Run time = "
            << measure<>::duration(benchmark).count() / 10
            << "ms\n";
    exit(0);
}

// ===================================================================================

int fill_black(BMP& bmp)
{
    for (int y = 0; y < bmp.height; ++y)
        for (int x = 0; x < bmp.width; ++x) {
            bmp.data[y * bmp.width + x].b = 20;
            bmp.data[y * bmp.width + x].g = 255;
            bmp.data[y * bmp.width + x].r = 180;
        }
    return 0;
}

int cpu_julia(BMP& bmp, const Complex& c)
{
    const double scale = 1.5;
    for (int y = 0; y < bmp.height; ++y)
        for (int x = 0; x < bmp.width; ++x) {
            double jx = (double)((double)bmp.width / 2.0 - x) / (double)((double)bmp.width / 2.0);
            double jy = (double)((double)bmp.height / 2.0 - y) / (double)((double)bmp.height / 2.0);

            bmp.data[y * bmp.width + x].b = 255 * (1 - julia(jx, jy, c));
            bmp.data[y * bmp.width + x].g = 255 * (1 - julia(jx, jy, c));
            bmp.data[y * bmp.width + x].r = 255 * (1 - julia(jx, jy, c));
        }
    return 0;
}

int julia(double jx, double jy, const Complex& C)
{
    Complex a(jx, jy);

    for (int i = 0; i < 300; ++i) {
        a = (a * a) + C;
        if (a.magnitude() > 2) {
            return 0;
        }
    }
    return 1;
}

int write_bmp(BMP& bmp, string& filename)
{
    ofstream file(filename, std::ios::binary);
    if (!file) {
        cerr << "Error writing file!\n";
        return false;
    }

    file.write((const char*)(bmp.info), 54);

    int numPixels = bmp.width * bmp.height;
    file.write((const char*)(bmp.data), numPixels * sizeof(Pixel));

    file.close();
    return 0;
}
