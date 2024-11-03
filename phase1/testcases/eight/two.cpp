#include<map>
#include<set>
#include<stack>
#include<queue>
#include<cmath>
#include<string>
#include<vector>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include<algorithm>

typedef long double ld;
typedef long long ll;
const int inf = 1e9 + 7;
const int mod = 1e9 + 7;
const int MAX_N = 2e5 + 7;

using namespace std;

int sum[2][MAX_N];

int main()
{
    int T;
	cin>>T;
	while(T--) {
		int N;
		cin>>N;
		for(int i=1;i<=N;i++) {
			cin>>sum[0][i];
			sum[0][i] += sum[0][i-1];
		}
		for(int i=1;i<=N;i++) {
			cin>>sum[1][i];
			sum[1][i] += sum[1][i-1];
		}
		int ans = inf;
		for(int i=1;i<=N;i++) {
			ans = min(ans, max(sum[0][N]-sum[0][i], sum[1][i-1]));
		}
		cout<<ans<<endl;
	}
}