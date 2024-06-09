#include<iostream>
#include<cstring>
using namespace std;
const int N = 2e5+3;

int buc[N],tarr[N],a[N],lless[N],rless[N],lgreater[N],rgreater[N];//桶，维护桶的树状数组，原数组，
int n;

int lowbit(int x){
    return x&-x;
}

int psum(int pos){
    int res=0;
    for(int i=pos;i;i-=lowbit(i))res+=tarr[i];
    return res;
}

void add(int pos,int val){
    buc[pos]+=val;//状态一致性，同时维护数组和区间和
    for(int i=pos;i<=n;i+=lowbit(i))tarr[i]+=val;
}

void print(int a[]){
    for(int i=1;i<=n;i++)cout<<a[i]<<" ";
    cout<<endl;
}

int main(){
    cin>>n;
    for(int i=1;i<=n;i++)cin>>a[i];
    for(int i=1;i<=n;i++){
        lless[i]=psum(a[i]-1),lgreater[i]=psum(n)-psum(a[i]);//状态一致性，同时求出左边出现的1到a[i]-1个数
        //a[i]+1到n的个数
        add(a[i],1);//统计之前a[i]还没有记录到桶里面
    }
    memset(tarr,0,sizeof tarr),memset(buc,0,sizeof buc);
    for(int i=n;i>=1;i--){
        rless[i]=psum(a[i]-1),rgreater[i]=psum(n)-psum(a[i]);//大小1-n之间的数总数-大小1-a[i]之间的数的个数=大于a[i]的数的个数
        add(a[i],1);
    }
    long long cntv=0,cnt6=0;//V最多是1e5*1e5,也就是中间的谷底，所以要用long long
    for(int i=1;i<=n;i++){
        cnt6+=(long long)lless[i]*rless[i],cntv+=(long long)lgreater[i]*rgreater[i];
    }
    // print(lless),print(lgreater),print(rless),print(rgreater),print(buc),print(tarr);
    cout<<cntv<<" "<<cnt6;//^不能作变量名，所以用头上的6
    
    return 0;
}