/*
单点修改是log n的操作，但是区间修改如果按照单点修改的思路，修改整个区间就要修改所有节点，
就是O(n)的操作，没有必要

懒标记的意义就是，区间修改的时候，还是保持是log n的操作，如果修改整个区间，
在根节点打上懒标记，最坏情况如果区间大小为：[1,2^k]，
假设需要修改[1,2^k-1]只需要在管理[1,2^(k-1)],[2^(k-1)+1,2^(k-1)+2^(k-2)]....的节点打上懒标记，总的也才log n


但是具体是如何区间修改的呢，我们考虑完全二叉树的情况，根节点管理[1,2^k]，假设就是要修改(本题是加数)整个大区间
只需要在根节点打上标记，表示整个区间都加上了val即可，而实际上不需要修改

因此节点上应该有一个懒标记(属性add)表示，该节点管理的区间进行了某一个修改，这一个修改在本节点生效了
【比如sum变了，加上了add * 范围，也就是根节点的sum已经正确了】
但是，在子节点没有生效，如果add=0表示，没有对区间修改或者区间修改已经生效到子区间了
简而言之add同时具有两重含义：1add非0表示修改生效到了当前节点，但是没有生效到子节点
2如果生效到子节点，子节点管理的每一个数还要加多少

如果此时，进行了查询操作，如果查询了整个大区间，直接返回根节点的结果就行，因为根节点已经发生了修改，sum是对的
只是子节点没有修改
假设区间大小[1,64]查询[1,31],显然根节点的这一个sum不是答案，就应该查询k个子节点管理的sum，但是之前的add没有生效
所以query之前还需要让add生效，首先，根节点有add值，所以pushdown下去，也就是让[1,32][33,64]
这两个节点上算出正确的sum(也就是长度add)同时打上标记，表示孙子节点还没有修改
但是[33,64]对于我们是没有作用的，让它一直留着懒标记就行,现在需要让[1,32]去掉懒标记，让[1,16][17,32]算出
正确的sum，之后[1,16]不需要分裂，因为sum已经够用了，我们需要让[17,32]分裂[17,24][25,32]
去掉[17,32]的懒标记，算出[17,24][25,32]正确的sum，之后[17,24]不需要分裂，因为sum已经正确
以此类推，知道算出[31,31]的sum【这里有一个疑问，叶子节点是怎么处理的？，叶子节点pushdown不会有问题？
答案是，这种情况永远不发生，因为modify和query遇到叶子节点都是特判直接返回值[因为modify和query的范围和区间总是有交点的]
】
【这里还有一个问题，叶子节点既然不会发生pushdown，叶子节点始终会留着懒标记吗，我感觉是会的
节点sum属性正确的充分必要条件是：所有父节点的add属性都=0
懒标记:表示操作生效到当前节点，而未生效到子节点
】

**懒标记**:表示修改截至到当前节点,而未到子节点，路径都没有懒标记时节点属性正确,修改[1,$2^k$-1]在k个节点上标记O(log n)

**pushdown**:已知有子节点，将当前节点累计的修改截止到子节点.无论修改、查询往下走时都要pushdown将过去的修改生效

**区间modify**:区间修改.已知节点管理的区间和修改区间相交.边界策略=全覆盖(修改截止并标记).操作=汇总

综上所述，我感觉还是比较理解了

注意，本题线段树直接区间修改，区间查询，维护原数组，不需要差分了
*/

#include<iostream>
using namespace std;
int n,m;
const int N = 1e5+5;
int arr[N];
typedef long long ll;//区间和需要long long，没有生效到儿子的修改 也会累计，所以是ll,找到int修改
//下标可以int，值用longlong

struct Node{
    int l,r;
    ll sum,add;
}tr[4*N];//写成N寄了，数组越界一般会segment faught


void pushup(Node &node, Node &left, Node & right){
    node.sum=left.sum+right.sum;//通用性考虑还是这样写
}

void pushup(int u){//明确，pushup表示根据左右区间重算区间属性，简化之后可以是：重算节点u的属性
    pushup(tr[u],tr[u<<1],tr[u<<1|1]);//这里有一个问题，add!=0的节点是否可以向上pushup
    //答案是如果这个节点父路径上面add都是0就可以.因为add=3，表示有+3的修改生效到当前节点但是没有生效到子节点
    //当前节点是生效了的 
}

void pushdown(int u){
    Node & node = tr[u],&left = tr[u<<1],&right=tr[u<<1|1];//这里要是没有写&,可能会寄了，因为这样修改的
    //就不是原来的节点了，left,right,node的修改都是失效了
    if(node.add){//将累积到当前节点的修改（对于当前节点已经生效）生效到子节点
        ll add=node.add;
        left.sum += (left.r-left.l+1)*add ,right.sum+= (right.r-right.l+1)*add;//生效的意思就是子节点管理的区间内所有数
        //都加上3，也就是子节点的属性发生相应的变化
        //想法是让子节点的修改生效到孙子节点，但是懒标记思想是先留着，所以打上一个标记即可
        left.add += add, right.add += add;//之前子节点可能就累计了add，这里应该是继续累计
        node.add=0;//因为node表示有多少修改没有生效到子节点，生效之后就没有修改没有生效了
    }
}

void build(int u,int l,int r){
    if(l==r){
        tr[u]={l,l,arr[l],0};//0表示没有任何修改没有生效到子节点,树上add都是0
        // 也就是一开始所有节点的属性都是正确的
        return ;
    }
    //走到这里肯定是l<r
    int mid=l+r>>1;
    build(u<<1,l,mid),build(u<<1|1,mid+1,r);
    tr[u]={l,r},pushup(u);//注意，pushup没有重算范围
}

Node query(int u,int l,int r){//通用性考虑还是这样写
    //查询节点u管理的区间和[l,r]相交区间的区间和【相当于区间属性，用节点管理】
    if(l<=tr[u].l && tr[u].r<=r)return tr[u];//覆盖，那么这一段区间的属性就是tr[u]的属性
    int mid=tr[u].l+tr[u].r>>1;
    pushdown(u);
    Node res,left={0},right={0};//这里没有初始化【优先问题2】会导致leftsum也不是0，就是错的
    if(l<=mid)left=query(u<<1,l,r);//和左区间相交，左区间交区间的属性
    if(r>=mid+1)right=query(u<<1|1,l,r);//和右区间相交，右区间交区间的属性
    pushup(res,left,right);//这里重算了属性，重要的问题:因为区间相交，所以left，right必定有一个有意义
    //但是如果只有一个有意义怎么办，比如left={0,0,0,0},但是你会发现重构的这一个res表示的区间属性是对的
    //区间范围是没有的，区间属性是0+right.sum，对了就行，相当于返回{0,0,left.sum+right.sum【如果有】,0}
    return res;
}

void modify(int u,int l,int r,ll val){//原来是单点，现在是区间修改，已知节点u管理的区间和
//[l,r]相交，修改相交部分
    if(l<=tr[u].l&& tr[u].r <=r){//覆盖的边界条件很好记，[l, tru.l tru.r  ,r]//修改但是不要深入
        //之前说过，修改可以是加val，而不是=val
        tr[u].sum+=val*(tr[u].r-tr[u].l+1);
        //但是不要深入，标记一下就行
        tr[u].add+=val;//之前可能有修改，要累计
        return ;//边界判了
    }
    int mid=tr[u].l+tr[u].r>>1;
    pushdown(u);//往下走之前都要pushdown,不是很好想，想象节点u只管理两个元素，子节点只管理一个元素
    //add=3表示修改只生效到了当前节点，现在要在右边元素+5，首先需要pushdown让右边元素+3，然后modify让它+5
    if(l<=mid)modify(u<<1,l,r,val);//左区间相交
    if(r>=mid+1)modify(u<<1|1,l,r,val);//两种都可能走，所以用if if结构
    pushup(u);
    //修改完之后，用节点重算属性5+8
}

void printnode(int u){
    cout<<tr[u].l<<" "<<tr[u].r<<" "<<tr[u].sum<<" "<<tr[u].add<<endl;
}

int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++)cin>>arr[i];
    build(1,1,n);//根节点管理1-n,递归
    // for(int i=1;i<=n*2;i++)printnode(i);
    while(m--){
        char op;
        cin>>op;
        if(op=='Q'){
            int l,r;
            cin>>l>>r;
            cout<<query(1,l,r).sum<<endl;//从根节点开始查l-r的区间和
        }
        else {
            ll l,r,val;
            cin>>l>>r>>val;
            modify(1,l,r,val);
        }
    }
    
    
    return 0;
}


/*
还是想得清楚的：
1 懒标记表示有多少修改生效到自己但是没有生效到子节点，这node.add=3表示，当前节点管理的区间已经加上了3【想象中】
但是子区间还没有

2 pushdown表示修改生效到儿子，但是不生效到儿子

3 往下走需要 pushdown

*/


