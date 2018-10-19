#include <iostream>
#define MAX 8010

using namespace std;

int pre_num[MAX];
bool flag[MAX] = {false}; // flag[i] is true iff the cow of brand i has been found

int main(int argc, char const *argv[])
{
    int n;
    cin >> n;
    pre_num[1] = 0;
    for (int i = 2; i <= n; ++i)
    {
        cin >> pre_num[i];
    }
    for (int i = n; i >= 1; --i)
    {
        int pre = pre_num[i];
        int j;
        for (j = 1; j <= pre + 1; ++j)
        {
            if (flag[j])
                ++pre;
        }
        flag[j - 1] = true;
        pre_num[i] = j - 1;
    }
    for (int i = 1; i <= n; ++i)
        cout << pre_num[i] << endl;
    return 0;
}
