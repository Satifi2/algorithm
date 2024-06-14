#include<iostream>
using namespace std;
const int N = 2e5+3;
int m,p,len;

struct node{//struct名字无所谓，因为后面只会用到tr
    int l,r,val;
}tr[N*4];//注意是区间长度的4倍

int update(int u){
    tr[u].val=max(tr[u<<1].val,tr[u<<1|1].val);//注意2*u+1怎么表示
}

void build(int u,int l,int r){//节点管理的区间，构建子树
    //边界，操作，dfs(左),dfs(右)的先序遍历算法
    if(l==r){
        tr[u].l=l,tr[u].r=r;
        return ;
    }
    tr[u]={l,r,0};//可以这样赋值，即使是对于已经定义好的变量
    int mid=l+r>>1;
    build(u<<1,l,mid),build(u<<1|1,mid+1,r);//mid分段法，注意平衡
}

int query(int u,int l,int r){//求相交区间的属性
    if(l<=tr[u].l && r >= tr[u].r)return tr[u].val;//全覆盖，相当于求整个管理区间的最大值 
    //不处理掉导致后面两个递归总是触发，也就是log n变成on
    int res=0;//最大值算法，策略2
    int mid=tr[u].l+tr[u].r>>1;
    if(l<=mid)res=max(res,query(u<<1,l,r));//如果和管理区间左边相交,求相交区间的最大值
    if(r>=mid+1)res=max(res,query(u<<1|1,l,r));//由于已经和大区间相交了，r>=mid+1,不可能和右区间没有相交
    return res;
}

void modify(int u,int pos,int val){//修改区间位置pos上面的节点，修改为val，从节点u传下去
    if(tr[u].l==tr[u].r)tr[u].val=val;//***************我发现这里可能不需要tr[u].l==x，这一因为我们总是保证x在闭区间内
    else{//前面是边界策略，也就是单点修改
        int mid=tr[u].l+tr[u].r>>1;
        if(pos<=mid)modify(u<<1,pos,val);
        else modify(u<<1|1,pos,val);//如果修改的点位于右区间
        update(u);//状态一致性忘记了
    }
}

int main(){
    cin>>m>>p;
    ios::sync_with_stdio(0);
    cin.tie(0);//最好还是关同步
    int last=0;//上次的值,第一次的上次认为是0
    build(1,1,m);//注意首先把所有节点创建好，所有区间最大值都是0，所以所有val=0没有问题
    //最多全部都是add操作，区间大小为m
    // for(int i=1;i<=10;i++)cout<<tr[i].l<<" "<<tr[i].r<<" "<<tr[i].val<<endl;
    
    for(int i=1;i<=m;i++){
        //读入有点多，但是没有多到1e6所以无所谓cin
        char op;
        cin>>op;
        if(op=='A'){
            int val;
            cin>>val;
            modify(1,++len,(val+(long long)last)%p);//在数组的第1,2....位置修改为val
            //根据题目就是和上次相加，然后mod p，元素大小很大，所以 long long 
        }
        else {
            int L;
            cin>>L;
            last = query(1,len-L+1,len);//询问最后L个数构成的区间的属性
            //节点1管理区间[1,n]大于这个范围，所以是相交的
            cout<<last<<endl;
        }
    }
    
    
    
    return 0;
}

/*
1 10 0
1 5 0
6 10 0
1 3 0
4 5 0
6 8 0
9 10 0
1 2 0
3 3 0
4 4 0

本题对于[1-10]区间用线段树进行管理，每一个节点分到的区间如上面

*/

//我成功优化了模板当中的if(tr[u].l==tr[u].r)，因为pos总是在线段范围之内
//但是特别要注意，((long long)val+last)%p是正确的，但是(long long)(val+last)%p是错误的
