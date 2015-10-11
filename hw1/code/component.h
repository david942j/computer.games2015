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
    bool extend(int n,int **num, char **brd) {
        if(color=='X')puts("extend");
        VI possible;
        for(auto id:V) {
            int i=id/n,j=id%n;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(brd[a][b]=='?') possible.pb(a*n+b);
            }
        }
        if(possible.size()==0) throw "GG when extension";
        else if(possible.size()==1) {
            int i=possible[0]/n,j=possible[0]%n;
            brd[i][j]=color;
            return true;
        }
        return false;
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
    vector<NumberConnected*> ncs;
    vector<Connected*> wbcs;
    int n;
    int **num;
    char **brd;
    Component(){}
    Component(int _n,int **_num,char **_brd) {
        n=_n;num=_num;brd=_brd;
        bool **flag;
        new2d(flag,bool,n,n);
        FOR(i,n)FOR(j,n)flag[i][j]=false;
        FOR(i,n)FOR(j,n) if(num[i][j]!=-1)
            ncs.pb(new NumberConnected(n,brd,flag,i,j));
        FOR(i,n)FOR(j,n) if(!flag[i][j])
            if(brd[i][j]=='X'||brd[i][j]=='.')
                wbcs.pb(new Connected(n,brd,flag,i,j));
    }
    ~Component() {
        for(auto c:ncs) delete c;
        for(auto c:wbcs) delete c;
    }
    bool extend() {
        bool ext=false;
        if(extend_black()) return true;//ext=true;
        //if(extend_white()) ext=true;
        //if(extend_number()) ext=true;
        return ext;
    }
    bool extend_black() {
        vector<Connected*>V;
        for(auto c:wbcs)if(c->color=='X')V.pb(c);
        if(V.size()<=1) return false;
        bool ext=false;
        for(auto c:V)
            if(c->extend(n,num,brd))
                ext=true;
        return ext;
    }
    void print() {
        //printf("%d %d %d\n",ncs.size(),wcs.size(),bcs.size());
    }
};
#endif