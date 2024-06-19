/*
对于懒标记的理解：线段树维护数组，但是数组是隐变量，假设区间[l,r]修改恰好和节点u管理的区间重合
没有懒标记的做法应该是递归，先修改叶子节点，然后pushup上来，实现真正的修改，这样每一个节点管理的区间属性都是正确的的
但是我们没有这么做，而是记录了区间修改，但是暂时不修改子节点，导致子节点其实没有修改，之后用到子节点的时候再
把修改生效【其实这里违反了状态一致性，但是减少了运算】
这里其实没有了数组的概念，而且节点当中也没有val这一个值，叶子节点的sum就是val导致多此一举

本题和之前【区间加和区间查询】的区别就是本题还有一个区间乘法
之前区间加我们只让节点标记上一个add，但是不传到叶子，如果区间乘，应该也是只在节点上标记一个mul，不传到叶子
显然一个add是不够的，所以使用mul是很合理的

但是要考虑，顺序是有影响的，假设传到到了叶子节点，假设原来sum=20,得知add=20,mul=30我们如何知道正确的sum?
答案是不知道，因为加可以累计(加上20后加上30相当于加上50)，乘也是可以累计的（乘20后乘30相当于乘600）
我们无从得知是(sum+20)*30,还是(sum+20)*10*20,还是 sum*30+20
如何解决呢

这里还有一个问题，假设是先加再乘
当前sum是s,add=a,mul=b,也就是(s+a)*b【先加再乘的定义】
现在来了一个区间乘c,也就是sum=(s+a)*b*c,变成sum=s,add=a,mul=b*c,就可以表示
但是如果是来了一个加c,也就是(s+a)*b+c,现在就不能表示成(s+?)*?的形式，就不是很好搞因为b可能不能整除c
就会导致新的区间和不能用sum,add,mul表示

如果是先乘再加
sum=s,add=a,mul=b,表示区间原来的和是sum,进行了区间加a和区间乘b的操作,和为s*b+a
假设现在来了一个区间乘c,也就是区间和=(s*b+a)*c,=s*(b*c)+ac,可以用sum=s,mul=b*c,add=a*c表示
也就是说，区间乘c，会导致add,mul都乘c
如果来了一个区间+c，也就是区间和=s*b+a+c,可以用sum=s,mul=b,add=a+c来表
也就是说，区间加c，会导致add加c

**懒标记**:表示子孙节点还应该进行什么操作
正确的表述是：add和mul表示子孙节点管理的区间应该先乘mul后加add

假设当前节点有一个add和mul属性，现在对当前节点进行了pushdown操作，
相当于是："根据懒标记，对于子节点进行相应的操作，但是孙节点不动"，
也就是让子节点管理的区间【注意，不同节点虽然维护同一个大的区间，但是懒标记的存在，状态是不同步的，
我们可以认为每个节点维护了自己独立的区间】全部乘以mul，
然后加上add，t.sum = ((LL)t.sum * mul + (LL)(t.r - t.l + 1) * add) % p;
这导致子节点管理的区间的sum可以通过上面的式子计算。
但是pushdown操作只对子节点执行正确的操作，并没有对孙节点进行操作，并且子节点收到的操作可以累计，

我们已经知道了：add和mul表示子孙节点管理的区间应该先乘mul后加add
假设当前节点的属性是add=3,mul=2,请问表示什么？
表示当前节点管理的区间已经进行了乘以 mul=2 后再加 add=3 的操作，但这些操作尚未传播到子孙节点。

但是如果，对于一个mul=2,add=3的节点，又收到一个操作，要把它所管理的区间乘以4,加上5，请问此时它告知子节点时
，应该让子节点管理的区间乘以多少，加上多少
子节点的新 mul 应该是：current_mul * new_mul = 2 * 4 = 8
子节点的新 add 应该是：current_add * new_mul + new_add = 3 * 4 + 5 = 12 + 5 = 17

为什么必须是先乘后加：先乘后加可以将对元素的两次乘加操作累积起来，累计成一次乘加操作
(x*mul1+add1)*mul2+add2 = x*(mul1*mul2) + (add1*mul2+add2)
但是先加后乘不可以
((x+add1)*mul1+add2)*mul2=x*mul1*mul2 + add1*mul1*mul2 + add2*mul2
你没法写成(x+?)*?的形式，因为
x前面不能有系数，导致只能是乘mul1*mul2,但是(x+?)*mul1*mul2,这里面的add2/mul1未必是整数，寄了

综上所述，我们可以节点add和mul表示子孙节点管理的元素应该先乘mul后加add
现在开始思考pushdown操作应该做什么，pushdown需要根据懒标记|，对子节点进行操作，但是不对孙节点进行操作
由于懒标记的定义，子节点的管理的每一个元素应该先乘mul后加add，这会导致子节点的区间和*mul+(r-l+1)*add
假设自己节点本来就有一个node.mul,node.add,相当于是说，在父节点操作传来之前，孙节点元素应该进行*node.mul+node.add的操作
加上新来的操作，就是孙节点的元素应该*mul+add

全部整合在一起，就是，孙节点的元素应该*(node.mul*mul)+node.add*mul+add
这一点我认为是好理解的，例如，上一题当中子节点应该-19，但是还没有，现在得知本节点还应该+17
综合下来就是孙节点应该-2
现在是已知子节点应该*node.mul+node.add，但是还没有做,现在对子节点*mul+add，就应该对孙节点*(node.mul*mul)+node.add*mul+add

**eval**：已知要对节点管理的区间进行一个操作，重算区间属性和懒标记
这里，已经搞定了，重算懒标记就是在问，之前没有做的操作和当前要做的操作合在一起要做什么

思考pushdown，根据懒标记，要对子节点*node.mul+node.add,但是还没有做，因此应该做，用eval解决，并算出子节点懒标记
既然操作做了，就去掉懒标记 

几乎所有线段树build都是一样的，但是要注意，意义要正确，之前懒标记add=0表示所有节点管理的元素都应该加上0，没有问题
但是本题，应该是，所有节点管理的元素都应该*mul+add，也就是mul=1,add=0
思考，为什么需要这么干，对于一个节点mul=1,add=0，假设对这一个节点进行reval，也就是
已知要对这个区间进行操作比如加上3,乘以2，要重算管理的区间属性和懒标记，区间属性很好算，就是区间和*2+3
懒标记实在问对于子区间应该进行什么操作，根据公式，应该*2*1,+0*2+3,也就是*2，加3，这样是对的
但是如果开始的mul=0,就是错的，就变成了要对子区间*0+3

modify操作一样的，还是区间修改,但是要填入两个参数表示要对区间进行*？+？
现在操作算到底，然后修改【注意是节点modify】，最后还要pushup传到根

另外，modify(add=3,mul=0)就不会调用modify(add=3,mul=2),所以说modify可以分成两种，区分乘和加，但是没有必要
因为节点modify不区分乘和加

query就是和上一题一样求区间属性sum
pushup一样，pushdown就是将操作生效到子节点
最后如何体现区间乘，答案是调用modify，只对区间乘和+即可

*/
#include<iostream>
using namespace std;
const int N = 1e5+3;
#define int long long
int arr[N];
int n,m,p;
struct Node{
    int l,r,sum,add,mul;
}tr[N*4];//之前忘记写Node了

void pushup(int u){
    //不用写成push(Node&)因为操作简单
    //根据左右区间重算节点属性，不包含区间范围和懒标记
    tr[u].sum=(tr[u<<1].sum+tr[u<<1|1].sum)%p;
    //多个操作求出来的结果最后%p，只需要操作中间%p就行，这是一个经验，因为多项式每一个项可以任意modp
}

void build(int u,int l,int r){
    //u管理lr
    if(l==r){
        tr[u]={l,r,arr[l],0,1};//单个节点总和是arr[l]，子节点还需要乘以1+0
        return; 
    }
    int mid=l+r>>1;
    build(u<<1,l,mid),build(u<<1|1,mid+1,r);
    tr[u]={l,r,0,0,1},pushup(u);//之前说过pushup只是重算区间属性，但是区间范围和区间懒标记都是不算的，因此要自己写
    //区间属性随便写，因为重算的时候不会参考之前的区间属性
}

void modify(Node &node,int add,int mul){//node是子节点，现在要对他进行操作，具体来说是让它管理的区间*mul+add
    //导致区间和变化,必须是&，不然修改了寂寞
    node.sum = ((node.sum * mul)%p + (add * (node.r-node.l+1))%p )%p;//之前这里算错了，已知对节点管理的区间进行了*mul+add
    //重算区间属性，应该是乘以mul，但是加上区间长度*add
    //[a,b,c],之后是a*mul+add + b*mul+add + c*mul+add ,区间和是原来区间和*mul+add
    
    //我想到一个很好的方法，这里可以同时维护原数组【当然会导致modify变成O(n)操作】
    // for(int i=node.l;i<=node.r;i++)arr[i]=arr[i]*mul+add;
    //然后你会发现，线段树节点懒标记导致不同步一点问题都没有，因为线段树只是维护了区间属性，有的属性错了没有关系
    //因为区间真实修改了
    
    //之前它的孙节点应该*node.mul+node.add但是还没有做，现在又整个区间*mul+add,综合下来就是孙节点应该:
    node.mul = (node.mul * mul)%p;
    node.add = ((node.add * mul)%p + add)%p;
    //如果一个区间乘加，子区间要进行的操作就会累计，状态一致性还需要修改mul和add
}

void pushdown(int u){
    //懒标记表示：子孙节点应做而未做的操作,也就是管理的区间乘mul+add
    //pushdown表示，根据懒标记对子节点但是不是孙节点进行这一个操作，也就是让子节点管理的区间*mul+add
    // 但是有两个节点，之前的代码可以写成分别对左右节点修改sum属性和懒标记add，但是这里最好用函数
    modify(tr[u<<1],tr[u].add,tr[u].mul),modify(tr[u<<1|1],tr[u].add,tr[u].mul),tr[u].add=0,tr[u].mul=1;
    // 这里要对子节点进行修改，也就是之前应该做但是未作的做了，也就是乘node.mul+node.add
    //状态一致性，懒标记表示应做而未做，这里做了
    //就应该清空懒标记
}

int query(int u,int l,int r){
    if(l<=tr[u].l && tr[u].r <= r){// if(l==r){//之前说过不能是单点查询作为边界，那样会导致全递归而不是最小覆盖
        return tr[u].sum;
    }
    int res=0,mid=tr[u].l+tr[u].r>>1;
    pushdown(u);//需要让头上生效的操作生效，叶子节点才是正确的
    if(l<=mid)res= (res + query(u<<1,l,r))%p;//如果有交点l<=mid就是和左区间有交点,查询左区间和查询区间相交区间的属性
    if(r>=mid+1)res = (res + query(u<<1|1,l,r))%p;//和右区间相交区间属性，注意不能是(mid+1,r)之前说过
    return res;//这里可以没有pushup，通过左右区间计算sum隐式包含了，pushup合并的效果
}

void modify(int u,int l,int r,int add,int mul){//已知节点管理的区间和l，r相交，要修改相交区间的属性
    //具体来说是让相交区间*mul+add
    if(l<=tr[u].l  && tr[u].r <=r ){//比如说让整个大区间*mul+add,只需要操作到根节点就行
    //根节点的root.mul=mul,root.add=add,整个区间的sum也是可以算对的
        modify(tr[u],add,mul);//调用之前的节点修改，不要搞错了
        return;
    }
    //我们的目标就是修改节点u，但是需要子节点的信息汇总才能算出修改之后正确的节点sum
    //所以还需要后序遍历搞子节点
    int mid = tr[u].l+tr[u].r>>1;//这里很容易错，y总错了，是要判断和管理的左右区间有没有相交
    pushdown(u);//因为修改的效果还需要汇总【下面节点的sum要重新算，】
    if(l<=mid)modify(u<<1,l,r,add,mul);
    if(r>=mid+1)modify(u<<1|1,l,r,add,mul);
    //上面的操作会找到最小闭覆盖
    pushup(u);//最后这一步才完成节点修改,汇总子节点属性，要让子节点属性正确，就需要pushdown
}

void printnode(int u){
    cout<<" "<<u<<" "<<tr[u].l<<" "<<tr[u].r<<" "<<tr[u].sum<<" "<< tr[u].add<<" "<<tr[u].mul<<endl;
}

void print(){
    for(int i=1;i<=2*n;i++){
        printnode(i);
    }
    cout<<"=================================="<<endl;
}

// print arr(int i){//想到一个好的打印方法，直接打印所有的叶子节点，相当于是原数组，如果叶子节点对了
//pushup也是对的，整个数就是对的，但是想了想发现不是很对，因为懒标记叶子节点其实是不同步的，比如修改
//整个区间，叶子节点不会变
    
// }

void printarr(){//我发现我又想错了，要维护原数组不需要修改所有叶子节点，因为节点只是维护了区间属性
//但是原数组是隐式的，modify操作对于这一个隐式的原数组是真的发生了变化的
    for(int i=1;i<=n;i++)cout<<arr[i]<<" ";
    cout<<endl;
}

signed main(){
    cin>>n>>p;
    for(int i=1;i<=n;i++)cin>>arr[i];
    build(1,1,n);
    // print();
    cin>>m;
    while(m--){
        int op,l,r,val;
        cin>>op>>l>>r;
        if(op==1){
            //区间乘
            cin>>val;
            modify(1,l,r,0,val);
        }
        else if(op==2){
            //区间加
            cin>>val;
            modify(1,l,r,val,1);//这里要相当小心，明确：modify是对相交区间的最小覆盖节点进行modify，
            //对于节点modify是已知进行了操作重算节点属性和懒标记，也就是区间*mul+add
            //加法操作必须看作是*1+add,之前写错了，导致对最小闭覆盖节点进行了*0+9,sum=9，错了
        }
        else {
            //区间和
            cout<<query(1,l,r)<<endl;
        }
        // if(m==4)printarr();
    }
    
    
    return 0;
}


