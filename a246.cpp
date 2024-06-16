/*
操作C，区间加上一个数,相当于差分数组端点加减【如果线段树维护arr，区间修改还是非常困难的】
操作Q，查询区间gcd
首先gcd满足结合律很好证明，还是证gcd 变成 证cd

线段树维护差分数组[a,b-a,c-b]，而不是原数组[a,b,c]

但是你会惊讶发现gcd(a,b-a,c-b)=gcd(a,b,c)
还是cd(a,b-a,c-b)=cd(a,b,c),如果d|后面三个数，则d|前面三个数
也就是差分数组的gcd=原来数组的gcd

5 5
1 3 5 7 9
Q 1 5
C 1 5 1
Q 1 5
C 3 3 6
Q 2 4

模拟一下：
5个数，1 3 5 7 9
5组操作:
Q 1 5
C 1 5 1
Q 1 5
C 3 3 6
Q 2 4

首先查询，[1,5]最大公约数，是1
把[1,5]都加上1，[2,4,6,8,10]
然后查询，[1,5]最大公约数是2
然后[3,3]加上6，[2,4,12,8,10]
[2,4]最大公约数是4

但是这里有一个问题，对于差分数组
[a1,a2-a1,a3-a2,a4-a3,a5-a4...]
我们想要求[3,5]，范围内的gcd，也就是gcd(a3,a4,a5)
可以看出，应该是gcd(a3,a4-a3,a5-a4)，但是不是gcd(a3-a2,a4-a3,a5-a4)
因为整除(a3,a4,a5)，未必能够整除(a3-a2,a4-a3,a5-a4)因为其中含有a2,故

求gcd([l=3,r=5])的时候，应该先求出a3=dif数组[1,l]前缀和,然后求出dif4,dif5的gcd//如果至少有两个数的话
也就是dif数组[l+1,r]的gcd

所以本题的思路明确了，首先是区间加，变成dif[l]+=val,dif[r+1]-=val，状态一致性需要通过线段树函数
区间查询求psum(1,l),gcd(l+1,r)
*/

#include<iostream>
using namespace std;
#define int long long
int n,m;
const int N = 5e5+3;
int arr[N],dif[N];
struct Node {
    int l,r,sum,gcd;
}tr[N*4];

int gcd(int a,int b){//看出来了，之所以求错了是因为，最大公约数还是有点小问题
//gcd(4,-4),如果最大公约数认为是4就完全自洽了，这样两种方式求出来的一定是同一个最大公约数，后面不需要abs
    if(b==0)return abs(a);
    return gcd(b,a%b);
}

void pushup(Node &u, Node &l, Node &r){//原地重算，引用不会新开一个空间，是写给编译器看的,机器不知道有没有引用
    u.sum=l.sum+r.sum,u.gcd=gcd(l.gcd,r.gcd);
}

void pushup(int u){
    pushup(tr[u],tr[u<<1],tr[u<<1|1]);
}

void build(int u,int l,int r){//已知l<=r的合理性,构建节点u，它管理[l,r].然后构建子节点 
    if(l==r){//边界:写法1
        tr[u]={l,l,dif[l],dif[l]};//如果只管理[l,l]的一个数,sum,gcd都是这一个数.维护dif的区间属性，所以不是arr
        //好像不是很难理解，自己的sum是自己，gcd也是自己，之后gcd(gcd(a),gcd(b))=gcd(a,b)才能算对
        //很显然，只有自己一个数的最大值还是自己
        return;
    }    
    int mid=l+r>>1;
    build(u<<1,l,mid),build(u<<1|1,mid+1,r);
    tr[u]={l,r},pushup(u);//很容易忘了tr[u]={l,r}因为没有意识到，重算区间属性不能重构范围
}

void modify(int u,int pos,int val){//已知节点管理的区间上有一点pos，修改它的值
    if(tr[u].r == tr[u].l){//边界，u管理单点数值,由于pos在l，r上，所以pos=l
        int nval=val+tr[u].sum;//意义变成了添加，单点数值一定和gcd,sum相同
        tr[u]={pos,pos,nval,nval};
        return;
    }
    //管理不止一个数就一定会有子节点，否则没有子节点，这是build 代码所示的,也是因为数和叶子节点一一对应
    int mid=tr[u].l+tr[u].r>>1;
    if(pos<=mid)modify(u<<1,pos,val);
    else modify(u<<1|1,pos,val);//else if(pos>mid)不需要，因为不会发生
    pushup(u);
    //这里忘记|1了
}

Node query(int u,int l,int r){//已知相交求相交区间属性
    if(l<=tr[u].l&&r>=tr[u].r)return tr[u];
    
    int mid=tr[u].l+tr[u].r>>1;
    if(r<=mid)return query(u<<1,l,r);//相交，r<=mid只能是和左区间相交
    if(l>=mid+1)return query(u<<1|1,l,r);
    Node left=query(u<<1,l,r),right=query(u<<1|1,l,r),res;
    pushup(res,left,right);
    return res;
}

void printnode(int u){
    cout<<tr[u].l<<" "<<tr[u].r<<" "<<tr[u].sum<<" "<<tr[u].gcd<<endl;
}

signed main(){
    // cout<<gcd(-4,4);
    // cout<<gcd(4,-4);
    cin>>n>>m;
    //枚举对象(arr[i],dif[i])
    for(int i=1;i<=n;i++)cin>>arr[i],dif[i]=arr[i]-arr[i-1];//注意这里和树状数组不太一样
    //树状数组可以用add代替,for循环里面就初始化好，但是线段树是先数组摆好之后初始化。【树状数组应该也可以】
    build(1,1,n+1);//如果没有这一步，tr[i].

    while(m--){
        char op;
        int l,r,val;
        cin>>op>>l>>r;
        if(op=='Q'){
            if(l==r){cout<<query(1,1,l).sum<<endl;continue;}//如果只查一个点al的gcd，就是al自己，就是查[1,l]前缀和
            Node left=query(1,1,l),right=query(1,l+1,r);//否则求出al,[l+1,r]的gcd，取gcd即可
            cout<<gcd(left.sum,right.gcd)<<endl;
            //这里我没有理解，说好这两种方法都可以求出相同的最大公约数，但是为什么这里是相反数了呢？
            //假设数组是[8,4,2],差分数组是[8,-4,-2],最大公约数是?
            //就很诡异，因为这样很难定义，关键是(8,-4)的最大公约数是什么？
            //理论上-4，4都可以整除，那就是4.但是根据我们的求法，gcd(-4,4)==
            //gcd(4,0)但是gcd(4,-4)=gcd(-4,0)，导致求出来的是gcd或者gcd相反数
        }
        else {
            cin>>val;
            modify(1,l,val);//这里注意，我们以前只能进行修改操作，但是这里是添加操作，如果能找到ai,
            //修改成d+ai即可，但是找不到ai，因为dif已经不维护丢弃了，ai被哪一个节点维护不知道
            //所以modify本身就要写成添加操作
            modify(1,r+1,-val);//但是差分数组修改有可能修改到外面去,我们可以采取的策略就是
            //如果修改到外面了就只修改左边的点
        }
    }
        // for(int i=1;i<=10;i++)printnode(i);
    /*
    最后有2个问题，为什么没有build导致segment fault，而不是WA
    
    为什么超过n的右端点修改modify(1,r+1,-val)会导致答案反而错误
    
    我好像理解了，差分的目的是为了快速区间加，然后前缀和，
    之后求前缀和都只会[1,n]在内求,而如果没有dif[n+1]-=val,求[1,n]之内的前缀和依然正确
    只是求[n+1,...]之后的前缀和错了，这是由于，dif[n+1]-=val相当于是arr[n+1]之后-val，但是这一段我们不维护
    爱多少都可以，所以之后求arr还是对的
    
    当然，dif[n+1]-=val也是对的，前提是把管理的区间改成1-n,不然没有节点管理dif[n+1]
    
    */
    return 0;
}
/*
时间复杂度还是建立树n 
查询log n
修改log n
pushup O(1)
*/