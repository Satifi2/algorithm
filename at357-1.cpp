#include<iostream>
using namespace std;


int main(){
    int n,m;
    cin>>n>>m;
    int cnt=0;
    for(int i=1;i<=n;i++){
        int u;
        cin>>u;
        if(m-u<0)break;
        else m-=u,cnt++;
    }
    cout<<cnt;

    return 0;
}