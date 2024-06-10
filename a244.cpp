/*
输入
5
1
2
1
0

给定5头牛，每头牛身高为1,2,3,4,5各不相同
从左到右5头牛报数，说明左边有多少牛比自己矮
一次是0【肯定是0，所以没有读入】 1 2 1 0
现在要输出5头牛的身高

策略：正着看不能得到任何结论，例如第一头牛左边没有更矮的牛，但是我们不知道第一头牛身高
所以应该倒着看，最后一头牛说0，也就是剩下4头牛当中没有一头比它矮，他是最矮的，只能是1【反证法】
也就是：
高度：1 2 3 4 5
牛：  5        

倒数第二头牛说1，也就是剩下的3头牛当中【可以理解为剩下4头牛，因为自己不可能比自己矮】，有一头更矮，所以他是剩下4头
当中第二高的，也就是3
也就是：
高度：1 2 3 4 5
牛：  5   4

可以看出，右边的牛已经确定，左边和本身的高度只能是2 4 5当中选
得到结论，第i头牛【总共有1-n头牛】左边有a[i]头牛比他矮，他就是剩下的高度中的第a[i]+1矮的

总结：数1-n，由左边更小元素个数倒推，从右数，左边3个更小=可选大小第4小，tr维护buc+二分
这里的可选大小的意思是，右边的牛高度确定了，左边和自己就不可能是右边牛的高度，如同右边的牛和右边的高度不存在一样
左边有3个更小=可选高度中有3个更小=自己是第四小

因此我们可以得到解题策略，首先存下来arr[i]表示第i头牛左边有arr[i]头牛更矮
然后倒着枚举i=n-1,现在arr[i]表示第i头牛左边有arr[i]头牛更矮,表示自己是可选高度中的第arr[i]+1高
但是，如何求出可选高度中的第arr[i]+1高具体有多高，或者说如何表示可选高度

答案是，用桶排序思想，buc[i]表示这个高度i出现了buc[i]次，由于所有牛高度是1-n并且互不相同，所以
buc=[0,1,1,1,1,1]，每次确定一个高度，计数就-1，比如第一次确定牛5高度1，变成buc[0,0,1,1,1,1]

问题：如何求出可选高度的第a[i]+1高，其实就是求buc当中的第a[i]+1个1
我们可以，比如说牛4是可选高度的第2高，其实就是求第2个1的下标，也就是3

问题：如何求第k个1的下标？等价于把前面的1相加什么时候得到k？答案是二分，求值问题变成判定问题，假设第k个1的下标是idx
那么idx的范围显然是[1,n],假设idx=1,此时psum(idx=1)=0,也就是buc[1]=0,显然不对，第1个1都没有到
假设idx=2,此时psum(idx=2)=1,也就是现在到了第一个1
假设idx=3,此时psum(idx=3)=2,也就是到了第二个1，此时idx=3就是答案
之后区间可能形如buc[0,1,1,00000]，但是显然第2个1的下标就是idx=3，也就是说psum(idx)=2,我们要选最小的idx
【也就是最靠左，一眼可以看出来属于右段左端点模型】

区间2可分，我们将区间分成段，位于左段的idx使得psum(idx)<k=2
右段的idx使得psum(idx)>=k=2
要找到右段左端点（ans在），mid在右段则r=mid,同段少移动，mid在左端则l=mid+1,不同段多移动
mid+1，导致mid定义的时候要减，也就是mid=(l+r)/2

到此为止思路简单了，首先读入n，读入arr[2-n]，定义一个buc=[0,1,1,1,1,1]，倒着枚举i=n-1，读出k=arr[i]+1
二分求idx使得psum(idx)>=k且最小，psum用树状数组求，树状数组对buc进行维护。求出一个牛的idx后，buc[idx]-1【一致性】

*/

//时间复杂度，求n次，每次求idx用二分是log n,二分的inr函数【被调用log n次】用到了psum是log n，所以是n * (log n)^2

#include<iostream>
using namespace std;
const int N = 1e5+3;
int buc[N],tr[N];
int arr[N],res[N];
int n;//最好在外面

int lowbit(int i){
    return i&-i;
}

int add(int pos,int val){
    buc[pos]+=val;
    for(int i=pos;i<=n;i+=lowbit(i))tr[i]+=val;
}

int psum(int pos){
    int res=0;
    for(int i=pos;i;i-=lowbit(i))res+=tr[i];
    return res;
}

bool inr(int mid,int k){
    // cout<<"mid "<<mid<<"psum(mid)"<<psum(mid)<<endl;
    return psum(mid)>=k;
}

int geth(int k){//找到满足psum(idx)>=k的最小idx
    int l=1,r=n;//idx介于1-n
    while(l<r){
        int mid=(l+r)/2;//mid加减相消
        if(inr(mid,k))r=mid;//同段少移动
        else l=mid+1;
    }
    return l;
}

int main(){
    cin>>n;
    for(int i=2;i<=n;i++)cin>>arr[i];
    for(int i=1;i<=n;i++)add(i,1);//经典错误，状态一致性buc[i]=1;//buc[i]表示高度为i的牛有buc[i]个，每去掉一头高度为idx的牛，buc[idx]--
    for(int i=n;i>=1;i--){
        int k=arr[i]+1;//当前牛是可选高度的第k=arr[i]+1高,高度是buc的第k个1的下标，高度是psum(idx)>=k的最小idx
        // cout<<k<<" ";
        int h=geth(k);
        res[i]=h;//res[i]表示第i头牛的高度 
        add(h,-1);//buc[h]-=1
    }
    // cout<<endl;
    for(int i=1;i<=n;i++)cout<<res[i]<<endl;
    
    return 0;
}

