#include<iostream>
using namespace std;


int main(){
    string s;
    cin>>s;
    int upper,lower=0;
    for(auto x:s){
        if(islower(x))lower++;
        else upper++;
    }
    cout<<"lower"<<lower<<endl;a
    cout<<"upper"<<upper<<endl;
    if(upper>lower){
        for(auto &x:s)if(islower(x))x=x-'a'+'A';
    }
    else {
        for(auto &x:s)if(isupper(x))x=x-'A'+'a';
    }
    cout<<s;

    return 0;
}