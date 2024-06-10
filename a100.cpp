/*
本来说不就是裸的差分，但是这题是让多炒此操作可以让所有数都一样，而不是求出区间加之后的结果
并且一样有多种一样的方法，比如说都一样的是5，都是3...

猜测1：结果必然是原数组当中的某一个数，因为这些数不用调整。如果最优操作将所有数变成了不是原数组当中任意一个数【不对，猜错了】

猜测2：方案数有可能是众数的不同种类【不对，猜错了】

【下面的思路也错了】
假设上面的猜想成立，问题可以变成把多少次操作把所有数变成一个数num（众数）
我们可以先把所有数减去num，然后区间加，所有数调整成0，所需次数相同【好想象，例如1 2 2 3 等价于-1 0 0 1，1+=1，3-=1即可】
问题又变成了如何把所有数变成0，用最少操作次数，比如说现在数组是：
0 0 1 3 4 0 0 6 5 4 -3....
好像和差分数组想不起来，所以本题还是要看视频，不然写不了
但是还要注意，所有区间操作都只能加一或者减一

*/
/*
正确理解：
首先看原数组差分数组
4
1
1
2
2
差分数组是
1 0 1 0
完成之后的差分数组是
1 0 0 0【原数组1 1 1 1】
或者是
2 0 0 0 【原数组2 2 2 2】

【结论：如果差分数组 除了dif1之外全0 ====等价于 原数组全部一样，证明，dif1=arr1，arr2-arr1=0...所以数组全一样，反之亦然】

区间加1=（完全等价）端点加减1
区间减1= 端点加减-1= 端点减加1
可以单点加1 = 这个位置差分加1 后面位置差分减1【不需要从定义思考，相当于一种特例，之前的加减位置不相邻，现在相邻】
总而言之，任意一种区间加操作，都等价于加一减去1，反之亦然

因此，我们想办法
也就是说我们要差分挑选一正一负两个数然后消去

假设原来的差分是：
[0,a1,1,5,-2,3,0,1,-9,-4]
我们忽略a1，统计所有的正数，绝对值之和是p=1+5+3+1=10
所有的负数，绝对值之和是2+9+4=15
每次负数+1，正数-1，可以看作一次区间加操作，我们的目标是把后面n-1个数全部变成0
*******************有两种策略：策略1，同时修改a1，和后面某个数
策略2：同时修改后面两个数，正数加，负数减，显然，策略2会更快将后面清0，并且效率是两倍

由于操作先后次序不影响结果【显然】，所以我们可以先把策略2做完

这样操作10次之后，还会留下一些负数，绝对值之和大小为5
例如[0,a1,0,0,0,0,0,-5,0]，问题是如何把它变成正确形式，非常简单，拿a[1]+=5即可
因此还需要5次操作，所以操作次数=max(p,q)【如果正数和更大同理】

然后考虑最后的数可以全部是多少，相当于问a1最后是多少
我们看
1 1 2 2
差分数组是
1 0 1 0
的这一个例子

策略2已经不能继续执行，使用策略1，a1+=1, 1-=1
变成[2,0,0,0]，等价于前面两个数+1，是第一个解

但是看起来就没有其他解了，错因为我们可以后面两个数-1，相当于同时操作2到n的某个数 和第n+1个数
这应该归纳到策略1，比起策略2慢
相当于[1 0 1-1 0 0+1]=[1 0 0 0 1]也就是对应[1 1 1 1]

所以刚才的-5有两种解决方法，一种是a1+1，还有一种最后一个数+1
这就导致a1最多是a1+5,最少是a1
也就是有1+5个不同解，也就是(abs(p-q)+1)，正数绝对值和大也同理

*/
#include<iostream>
using namespace  std;
const int N = 1e5+3;
int arr[N];

int main(){
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)cin>>arr[i];
    for(int i=n;i>=1;i--)arr[i]=arr[i]-arr[i-1];//练习原地差分，a[1]不变可以不枚举，也可以枚举
    // for(int i=1;i<=n;i++)cout<<arr[i]<<" ";
    // cout<<endl;
    long long pos=0,neg=0;//正数负数个数//居然会爆炸int，因为arr[i]可以比较大
    for(int i=2;i<=n;i++)if(arr[i]>0)pos+=arr[i];else neg-=arr[i];//注意0不记录,但是-0不影响
    cout<<max(pos,neg)<<endl;
    cout<<abs(pos-neg)+1;
    
    return 0;
}



