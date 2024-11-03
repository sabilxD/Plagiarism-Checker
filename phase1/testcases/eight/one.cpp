#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define pii pair<int, int>
#define pll pair<long long, long long>
#define pb push_back
#define ff first
#define ss second
#define YES cout << "YES\n"
#define NO cout << "NO\n"
#define nn "\n"
#define sci(x) scanf("%d", &x)
#define LL_INF (1LL << 62)
#define INF (1 << 30)
#define SetBit(x, k) (x |= (1LL << k))
#define ClearBit(x, k) (x &= ~(1LL << k))
#define CheckBit(x, k) (x & (1LL << k))
#define mod 1000000007
#define N 300005

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;

        ll x[2][n+1];

        memset(x, 0, sizeof x);

        for(int i = 1; i <= n; i++){
            cin >> x[0][i];
            x[0][i] += x[0][i-1];
        }

        for(int i = 1; i <= n; i++){
            cin >> x[1][i];
            x[1][i] += x[1][i-1];
        }

        ll mn = LL_INF;

        for(int i = 1; i <= n; i++){
            mn = min(mn, max(x[0][n]-x[0][i], x[1][i-1]));
        }

        cout << mn << nn;
    
    }
    
    return 0;
}
 