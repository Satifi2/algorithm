#include <iostream>
#include<bitset>
using namespace std;
const int N =20;
int st[N],rec[N],stand[N];
int n,m,k,ans=1;

void printbit(int x){
    // while(x)cout<<(x&1)<<" ",x>>=1;//初始对象=(1001101),之后(100110)...
    //最后一满足的对象(1),第一个不满足的对象(0)
    bitset<8>n=x;
    // cout<<n<<endl;
}

bool check(){
    int res=0;
    for(int i=0;i<k;i++){
        res|=stand[rec[i]];
    }
    // cout<<"         "<<bitset<8>(res)<<" "<<bitset<8>(ans)<<endl;
    if(res==ans)return true;
    return false;
}

bool dfs(int i){
    if(i==k){
        // for(int j=0;j<k;j++)cout<<rec[j]<<" ";
        // cout<<endl;
        return check();
    }
    for(int j=1;j<=n;j++){
        if(st[j]==0 && rec[i-1]<j){
            st[j]=1,rec[i]=j;
            if(dfs(i+1))return true;
            st[j]=0;
        }
    }
    return false;
}

int main() {
    cin>>n>>m;
    for(int i=1;i<m;i++,ans<<=1,ans|=1);//(1,1),(2,11),3(111),4(1111)
    // cout<<bitset<8>(ans)<<endl;
    // k=2;
    // dfs(0);//填空的时候从0开始填，和后面的契合
    for(int i=1;i<=n;i++){
        string str;
        cin>>str;
        int state=1;

//假设str是xxoxxo，对应的state=001001，当然是反过来的也就是100100
        for(int j=0;j<str.size();j++,state<<=1){//枚举了(x,1),(x,10),(o,100)
            if(str[j]=='o')stand[i]|=state;
        }
    }
    // for(int i=1;i<=n;i++){cout<<bitset<8>(stand[i])<<endl;}

    for(int i=1;i<=m;i++){//一定是有解的
        k=i;
        if(dfs(0)){
            cout<<i;break;
        }
    }


    return 0;
}
