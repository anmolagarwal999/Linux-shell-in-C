#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL MOD = 1000000007;
#define fr(i, n) for (i = 0; i < n; i++)
#define rep(i, n) for (i = 1; i <= n; i++)
#define lcm(x, y) ((x) * (y) / __gcd((x), (y)))
#define pb push_back
#define all(c) (c).begin(), (c).end()
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "----------------------------------\n";
#include <iostream>
#define intCeil(a, b) ((a) + (b)-1) / (b)

int dx[] = {1, 1, 0, -1, -1, -1, 0, 1}; // trick to explore an implicit 2D grid
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1}; // S,SE,E,NE,N,NW,W,SW neighbors  //GO FOR EVEN FOR 4 moves

#define fastinput                     \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);

LL POW(LL x, LL y)
{
    LL ans = 1;
    LL base = x;
    while (y)
    {
        if (y & 1)
        {
            ans *= base;
        }
        base = base * base;
        y = y >> 1;
    }
    return ans;
}

#define lim 6

double dp[lim][lim];
bool check[lim][lim];

double cal(LL i, LL j)
{
    // if (i == 1 && j == 2)
    // {
    //     exit(0);
    // }
    if (i < 0 || j < 0)
    {
        return 0;
    }
    if (i == 0)
    {
        check[i][j] = true;

        return dp[i][j] = 0;
    }
    if (i == 1 && j == 0)
    {
        return dp[i][j] = 1;
    }
    if (i == 0 && j == 1)
    {
        printf("--------------\n");
        check[i][j] = true;

        return dp[i][j] = 0;
    }
    if (i == 1 && j == 1)
    {
        return dp[i][j] = 1;
    }
    if (check[i][j] == false)
    {
        //cal(i - 1, j);
        // cal(i, j - 1);
        double a = cal(i - 1, j);
        double b = cal(i, j - 1);
        if (i == 1 && j == 2)
        {
            printf("a is %lf and b is %lf\n", a, b);
        }
        dp[i][j] = (((i - 1) * a) + ((j-1) * b)) / ((double)(i + j - 1));
        check[i][j] = true;
    }

    return dp[i][j];
}
int main()
{
    // fastinput;
    LL n, i, j, k, t, temp, tc;

    cin>>n;

    for (i = 0; i < lim; i++)
    {
        for (j = 0; j < lim; j++)
        {
            dp[i][j] = -1;
            check[i][j] = false;

            if (i == 0)
            {
                dp[i][j] = 0;
                check[i][j] = true;
            }
        }
    }

    dp[1][0] = 1;
    dp[1][1] = 1;
    dp[2][0]=0;
    check[2][0]=true;
    check[1][0] = check[1][1] = true;

    cal(lim - 1, lim - 1);

    for (i = 0; i < lim; i++)
    {
        for (j = 0; j < lim; j++)
        {
            printf("%10lf ", dp[i][j]);
            // if (check[i][j] == true)
            // {
            //     printf("1 ");
            // }
            // else
            // {
            //     printf("0 ");
            // }
        }
        printf("\n");
    }

    printf("ans is %lf\n", dp[50][50]);

    return 0;
}
