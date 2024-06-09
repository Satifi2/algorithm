#include<iostream>
#include<cmath>
using namespace std;
const int N = 760;//最多是3^6=9*9*9=759
char arr[N][N];
int n;
//比如说dfs(0,0,1,false)
void dfs(int r1,int c1,int dis,bool center){//填充r1,c1到r2,c2【看可以忽略】的区域，dis9个方格的边长
//整个区域是否是center由center确定
    if(dis==0){
        if(center)arr[r1][c1]='.';
        else arr[r1][c1]='#';
    }
    else{
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(center||(i==1&&j==1))dfs(r1+i*dis,c1+j*dis,dis/3,true);
                else dfs(r1+i*dis,c1+j*dis,dis/3,false);//dfs(0-2,0-2,0)
            }
        }
    }
}

int main(){
    cin>>n;
    int row = pow(3,n);
    //n=0，就是dfs(0,0,0,0)
    dfs(0,0,row/3,0);//n=1,就是dfs(0,0,3/3)
    for(int i=0;i<row;i++){
        for(int j=0;j<row;j++){
            cout<<arr[i][j];
        //没有空格
        }
        cout<<endl;
    }
    return 0;
}
/*
#########
#.##.##.#
#########
###...###
#.#...#.#
###...###
#########
#.##.##.#
#########

# # # # # # # # #
# . # # . # # . #
# # # # # # # # #
# # # . . . # # #
# . # . . . # . #
# # # . . . # # #
# # # # # # # # #
# . # # . # # . #
# # # # # # # # #
 */