#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int32_t main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin>>t;
    while(t--)
    {
        int n,k;
        cin>>n>>k;
        vector<int>ans;
        for(int i=(k+1)/2;i<k;i++)
        ans.push_back(i);
        for(int i=k+1;i<=n;i++)
        ans.push_back(i);
        cout<<ans.size()<<"\n";
        for(int i=0;i<ans.size();i++)
        cout<<ans[i]<<" ";
        cout<<endl;
    }
    return 0;
}