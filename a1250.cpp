//其实和博弈论没有关系，也就是两种颜色不需要区分
//每次连边相当于将两个端点合并,出现环的时候两个端点在同一个集合
//如何将二维点标上编号？x=i*m+j即可，m是列数[前提是行，列从0开始数，x也是从0开始数]
//反之如何从编号到行，列，c=x%m,r=x/m


#include<iostream>
using namespace std;
const int N = 200 * 200 +5;//有200 * 200 个格点，从0开始编号
int father[N];

int find(int u){
    if(father[u]==u)return u;
    return father[u]=find(father[u]);
}

int main(){
    int n,m;
    cin>>n>>m;
    for(int i=0;i<N;i++)father[i]=i;//不确定有多少个点，可以将所有格点算上
    for(int i=1;i<=m;i++){
        int r1,c1,r2,c2;
        cin>>r1>>c1;
        r1=r1-1,c1=c1-1;//从0开始数
        char op;
        cin>>op;
        if(op=='D')r2=r1+1,c2=c1;//注意，坐标轴指向向下和向右
        else r2=r1,c2=c1+1;
        // printf("%d %d %d %d\n",r1,c1,r2,c2);
        int u1=r1*n+c1,u2=r2*n+c2;
        u1=find(u1),u2=find(u2);
        if(u1==u2)cout<<i,exit(0);
        father[u1]=u2;
    }
    cout<<"draw";
    
    return 0;
}



























