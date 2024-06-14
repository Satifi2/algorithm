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
    int a(1);
    cout<<a<<" ";
    int a={2};
    cout<<a<<" ";

    return 0;
}
















