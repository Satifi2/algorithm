#include<iostream>
#include<cmath>
using namespace std;
typedef long long ll;
const ll mod =998244353;
ll qpow(ll a,ll b){
    ll res=1;
    while(b){
        if(b&1)res=(res*a)%mod;
        a=(a*a)%mod,b>>=1;
    }
    return res;
}

int main(){
    ll n;
    cin>>n;
    int len=to_string(n).size();
    // ll q=pow((ll)10,len);
    // q=q%mod;
    ll q=1;
    ll t=n;
    while(t){
        t/=10;
        q=(q*10)%mod;
    }

    ll res=(n%mod);

    res = res *((qpow(q,n)-1 + mod)%mod )%mod;

    res = res * qpow(q-1,mod-2) % mod;
    cout<<res;

    return 0;
}