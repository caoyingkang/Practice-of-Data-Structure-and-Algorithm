#include <iostream>
#define MAX 1030
using namespace std;

int C[MAX][MAX] = {0};
int lowbit[MAX];
int S;

int Sum_y(int x, int y)
{
    return y == 0 ? 0 : C[x][y] + Sum_y(x, y - lowbit[y]);
}

int Sum(int x, int y)
{
    return x == 0 ? 0 : Sum_y(x, y) + Sum(x - lowbit[x], y);
}

void Update_y(int x, int y, int A)
{
    if (y > S)
        return;
    C[x][y] += A;
    Update_y(x, y + lowbit[y], A);
}

void Update(int x, int y, int A)
{
    if (x > S)
        return;
    Update_y(x, y, A);
    Update(x + lowbit[x], y, A);
}

int main(int argc, char const *argv[])
{
    int op, x, y, A, L, R, B, T;
    cin >> op >> S;
    for (int i = 1; i <= S; ++i)
    {
        lowbit[i] = i & (-i);
    }
    while (1)
    {
        cin >> op;
        if (op == 3)
            break;
        else if (op == 1)
        {
            cin >> x >> y >> A;
            Update(x + 1, y + 1, A);
        }
        else
        {
            cin >> L >> B >> R >> T;
            cout << Sum(R + 1, T + 1) - Sum(L, T + 1) - Sum(R + 1, B) + Sum(L, B) << endl;
        }
    }
    return 0;
}
