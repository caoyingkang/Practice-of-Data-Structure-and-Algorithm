#include <iostream>

using namespace std;

int log2(int a)
{
    int ret = 0;
    while (a > 1)
    {
        a /= 2;
        ++ret;
    }
    return ret;
}

int main(int argc, char const *argv[])
{
    cout << (160000000000) << endl;
    cout << log2(1600000000) << endl;
    return 0;
}
