#include <bits/stdc++.h>
using namespace std;
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")
//#pragma GCC optimize("Ofast")
// #pragma GCC optimize "trapv"

#define ll long long 
#define lld long double

int main() {
	// your code goes here
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t;
    cin>>t;
    while(t--)
    {
        int n,k;
        cin>>n>>k;
        
        vector <int> v;
        
        for(int i=(k+1)/2;i<k;i++)
        v.push_back(i);
        
        for(int i=k+1;i<=n;i++)
        v.push_back(i);
        
        cout<<v.size()<<"\n";
        for(int i=0;i<v.size();i++)
        cout<<v[i]<<" ";
        cout<<"\n";
    }
}
 