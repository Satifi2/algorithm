//有t个样例，给定n组约束，输入i,j表示第i个数和第j个数相等，或者不相等
//要求判断是否一致（没有矛盾）
// 相等相当于加入同一个集合,也就是unite操作
// 不等相当于不能是同一个集合
// 矛盾相当于，本来同一集合的点要求是不同集合
// 由于顺序不影响是否矛盾，可以先处理相等的，到同一个集合，然后不等的判断不在同一个集合
//本来是可以用father数组的，但是这一题节点编号是1e9所以不能用数组，用map
//还有就是多样例的一般原则：需要重置的用局部变量，函数引用传参，总是读完数据


#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;
typedef pair<int,int> pii;

int find(unordered_map<int,int>&father,int u){
    if(father[u]==u)return u;
    return father[u]=find(father,father[u]);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        unordered_map<int,int>father;
        vector<pii>eq,ne;
        for(int i=1;i<=n;i++){
            int u,v,e;
            cin>>u>>v>>e;
            if(!father.count(u))father[u]=u;
            if(!father.count(v))father[v]=v;
            if(e==1)eq.push_back({u,v});
            else ne.push_back({u,v});
        }
        for(auto [u,v]:eq){
            u=find(father,u),v=find(father,v);//拷贝不会修改原值
            father[u]=v;
        }
        bool consistency=true;
        for(auto [u,v]:ne){
            u=find(father,u),v=find(father,v);
            if(u==v)consistency=false;
        }
        if(consistency==false)cout<<"NO"<<endl;
        else cout<<"YES"<<endl;
    }
    return 0;
}










