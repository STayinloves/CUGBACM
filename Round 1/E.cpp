#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;
typedef unsigned long long LL;

const int M = 2;

struct Matrix //矩阵
{
    LL m[M][M];
};

Matrix A;
Matrix I = {1, 0, 0, 1};

Matrix multi(Matrix a, Matrix b, LL MOD) //矩阵的乘法
{
    Matrix c;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            c.m[i][j] = 0;
            for (int k = 0; k < M; k++) {
                c.m[i][j] = (c.m[i][j] % MOD + (a.m[i][k] % MOD) * (b.m[k][j] % MOD) % MOD) % MOD;
            }
            c.m[i][j] %= MOD;
        }
    }
    return c;
}

Matrix power(Matrix a, LL k, LL MOD) //矩阵的快速幂
{
    Matrix ans = I, p = a;
    while (k) {
        if (k & 1) {
            ans = multi(ans, p, MOD);
            k--;
        }
        k >>= 1;
        p = multi(p, p, MOD);
    }
    return ans;
}

LL gcd(LL a, LL b) {
    return b ? gcd(b, a % b) : a;
}

const int N = 400005;
const int NN = 5005;

LL num[NN], pri[NN];
LL fac[NN];
int cnt, c;

bool prime[N]; //判断是否是素数
int p[N];      //素数表
int k;         //素数个数

void isprime() //打素数表
{
    k = 0;
    memset(prime, true, sizeof(prime));
    for (int i = 2; i < N; i++) {
        if (prime[i]) {
            p[k++] = i;
            for (int j = i + i; j < N; j += i) {
                prime[j] = false;
            }
        }
    }
}

LL quick_mod(LL a, LL b, LL m) //快速幂的模运算
{
    LL ans = 1;
    a %= m;
    while (b) {
        if (b & 1) {
            ans = ans * a % m;
            b--;
        }
        b >>= 1;
        a = a * a % m;
    }
    return ans;
}

LL legendre(LL a, LL p) //赖让德公式  判断a是不是模P的二次剩余
{
    if (quick_mod(a, (p - 1) >> 1, p) == 1) {
        return 1;
    } else {
        return -1;
    }
}

void Solve(LL n, LL pri[], LL num[]) //将n质因数分解 pri存底数数，num存幂
{
    cnt = 0;
    LL t = (LL) sqrt(1.0 * n);
    for (int i = 0; p[i] <= t; i++) {
        if (n % p[i] == 0) {
            int a = 0;
            pri[cnt] = p[i];
            while (n % p[i] == 0) {
                a++;
                n /= p[i];
            }
            num[cnt] = a;
            cnt++;
        }
    }
    if (n > 1) //有余数
    {
        pri[cnt] = n;
        num[cnt] = 1;
        cnt++;
    }
}

void Work(LL n) // n的所有因子
{
    c = 0;
    LL t = (LL) sqrt(1.0 * n);
    for (int i = 1; i <= t; i++) {
        if (n % i == 0) {
            if (i * i == n) {
                fac[c++] = i;
            } else {
                fac[c++] = i;
                fac[c++] = n / i;
            }
        }
    }
}

LL find_loop(LL n) //找循环节
{
    Solve(n, pri, num);
    LL ans = 1;
    for (int i = 0; i < cnt; i++) {
        LL record = 1;
        if (pri[i] == 2) {
            record = 3;
        } else if (pri[i] == 3) {
            record = 8;
        } else if (pri[i] == 5) {
            record = 20;
        } else {
            if (legendre(5, pri[i]) == 1) {
                Work(pri[i] - 1);
            } else {
                Work(2 * (pri[i] + 1));
            }
            sort(fac, fac + c);
            for (int k = 0; k < c; k++) //判断因子中哪一个是循环节的长度
            {
                Matrix a = power(A, fac[k] - 1, pri[i]);
                LL x = (a.m[0][0] % pri[i] + a.m[0][1] % pri[i]) % pri[i];
                LL y = (a.m[1][0] % pri[i] + a.m[1][1] % pri[i]) % pri[i];
                if (x == 1 && y == 0) {
                    record = fac[k];
                    break;
                }
            }
        }
        for (int k = 1; k < num[i]; k++) {
            record *= pri[i];
        }
        ans = ans / gcd(ans, record) * record;
    }
    return ans;
}

void Init() {
    A.m[0][0] = 1;
    A.m[0][1] = 1;
    A.m[1][0] = 1;
    A.m[1][1] = 0;
}

int main() {
    LL n;
    Init();
    isprime();
    while (cin >> n) {
        LL ans = find_loop(n);
        if (ans % 2 == 0) {
            ans /= 2;
        }
        cout << ans << endl;
    }
    return 0;
}