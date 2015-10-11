#ifndef COMPONENT_H
#define COMPONENT_H
#include "functions.h"
class Connected{
public:
    Connected(){}
    Connected(int n,char **brd,bool **flag,int x,int y){
        read_from(n,brd,flag,x,y);
    }
    VI V;
    char color;
    void read_from(int n,char **brd,bool **flag,int x,int y){
        V.clear();
        color = brd[x][y];
        DFS(n,x,y,flag,brd);
    }
private:
    void DFS(int n,int x,int y,bool **flag,char **brd) {
        flag[x][y]=true;
        V.pb(x*n+y);
        FOR(k,4) {
            int a=x+dx[k],b=y+dy[k];
            if(!inbound(a,b,n,n)||flag[a][b]||brd[a][b]!=color)continue;
            DFS(n,a,b,flag,brd);
        }
    }
};
class NumberConnected:public Connected {
public:
    NumberConnected(){}
    NumberConnected(int n,char **brd,bool **flag,int x,int y){
        read_from(n,brd,flag,x,y);
    }
    int belong;
    void read_from(int n,char **brd,bool **flag,int x,int y) {
        Connected::read_from(n,brd,flag,x,y);
        belong = x*n+y;
    }
};
struct Component{
    vector<NumberConnected> ncs;
    vector<Connected> wcs;
    vector<Connected> bcs;
    Component(){}
    Component(int n,int **num,char **brd) {
        bool **flag;
        new2d(flag,bool,n,n);
        FOR(i,n)FOR(j,n)flag[i][j]=false;
        FOR(i,n)FOR(j,n) if(num[i][j]!=-1)
            ncs.pb(NumberConnected(n,brd,flag,i,j));
        FOR(i,n)FOR(j,n)
            if(!flag[i][j]&&brd[i][j]=='.')
                wcs.pb(Connected(n,brd,flag,i,j));
            else if(!flag[i][j]&&brd[i][j]=='X')
                bcs.pb(Connected(n,brd,flag,i,j));
    }
    void print() {
        printf("%d %d %d\n",ncs.size(),wcs.size(),bcs.size());
    }
};
#endif