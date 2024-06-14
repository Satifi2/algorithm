#include<iostream>
using namespace std;

void print(int x){
    int res[32];
    for(int i=0;i<32;i++){
        res[i]=x&1,x>>=1;
    }
    for(int i=31;i>=0;i--)cout<<res[i];
    cout<<endl;
}

int main(){
    // int n;
    // cin>>n;
    // print(~n),print(-n);
    long long H = 4096,V=32000,L=32,Ht=11008;
    cout<<2 * V * H<<" "<<L * (4 * H * H) <<" "<<L * 3 * H * Ht<<endl;
    cout<<(2 * V * H + H + L * (4 * H * H + 3 * H * Ht + 2 * H))<<endl;

    return 0;
}
















