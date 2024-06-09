/*
之前是区间加，求单点值，也就是dif的前缀和psum(i)
现在是区间加，求原数组的区间和，相当于是dif的前缀和的前缀和

也就是求arr1+arr2+arr3+...
=dif1 +
+dif1 + dif2
+dif1 + dif2 + dif3
+dif1 + dif2 + dif3 + dif4...

之前说可以用log n求出arr i，但是求多个arr i就是n log n,询问多了就是m n log n，我们要的是m log n

=dif1 + (dif2) (dif3) (dif4)
+dif1 + dif2   (dif3) (dif4)
+dif1 + dif2 + dif3 (dif4)
+dif1 + dif2 + dif3 + dif4...

我们要求的sum(arr i)=i*arri - sum((i-1)*di))
arr i很好求,用tr维护di的区间和即可
但是怎么求sum(i-1)di?答案是我们维护一个数组bi=(i-1)di, 用tr2维护(i-1)di的区间和即可,sum((i-1)*di))就是bi前缀和

*/

#include<iostream>
using namespace std;
#define int long long
#define INT int
const int N = 1e5+3;
int arr[N],diff[N],b[N],tr1[N],tr2[N];//diff数组和b[i]=(i-1)*diff[i]数组隐式存在
int n,m;


int lowbit(int x){
    return x&-x;
}

int psum(int tr[],int pos){//求tr所为维护的数组的前pos个元素的前缀和
    int res=0;
    for(int i=pos;i;i-=lowbit(i))res+=tr[i];
    return res;
}

void add(int tr[],int pos,int val){
    for(int i=pos;i<=n;i+=lowbit(i))tr[i]+=val;
}

int psumarr(int i){//对于数组arr求前缀和，求到arr[i]
    return psum(tr1,i)*i - psum(tr2,i);//根据公式，应该等于arri * i - psum(bi) 
}

#undef int 
INT main(){
    #define int long long
    cin>>n>>m;
    for(int i=1;i<=n;i++)cin>>arr[i];
    for(int i=1;i<=n;i++)add(tr1,i,arr[i]-arr[i-1]),add(tr2,i,(i-1)*(arr[i]-arr[i-1]));//dif[i]=arr[i]-arr[i-1],但是状态一致性导致tr1[i]修改
    //bi=(i-1)*(arr[i]-arr[i-1])//注意后面还有一个括号
    while(m--){
        char op;
        cin>>op;
        if(op=='Q'){
            int l,r;
            cin>>l>>r;
            //首先要求arri加到l的前缀和
            cout<<psumarr(r)-psumarr(l-1)<<endl;
        }
        else {
            int l,r,val;
            cin>>l>>r>>val;
            add(tr1,l,val),add(tr1,r+1,-val);//区间加变端点加减,dif[l]+=val,dif[r+1]-=val,tr保持和dif的状态一致性
            add(tr2,l,(l-1)*val),add(tr2,r+1,-val*(r+1-1));//由于dif[l]+=val,,因为b[i]=dif[i]*(i-1)导致b[l]+=(l-1)*val，
            //b[r+1]=dif[r+1]*r导致b[r+1]-=val*r
        }
    }
    
    return 0;
}















