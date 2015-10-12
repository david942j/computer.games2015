#ifndef COMPONENT_H
#define COMPONENT_H
#include "functions.h"
#include "board.h"
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
        VI possible;
        for(auto id:V) {
            int i=id/n,j=id%n;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(brd[a][b]=='?') possible.pb(a*n+b);
            }
        }
        if(possible.size()==0) throw ("GG when extension");
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
    bool extend(int n,int **num, char **brd) {
        int bnum = num[belong/n][belong%n];
        if(V.size() > bnum) throw ("More than bnum");
        VI possible;
        if(V.size() == bnum) return surround_black(n,brd);
        for(auto id:V) {
            int i=id/n,j=id%n;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(brd[a][b]=='?') possible.pb(a*n+b); // 4?. issue
            }
        }
        if(possible.size()==0) throw ("GG when number extension");
        else if(possible.size()==1) {
            int i=possible[0]/n,j=possible[0]%n;
            brd[i][j]=color;
            return true;
        }
        return false;
    }
    bool surround_black(int n, char **brd) {
        bool ret=false;
        for(auto id:V) {
            int i=id/n,j=id%n;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(brd[a][b]=='?') brd[a][b]='X',ret=true;
            }
        }
        return ret;
    }
};
struct Component{
    vector<NumberConnected*> ncs;
    vector<Connected*> wbcs;
    int n; int **num; // never change
    char **brd;
    Component(){num=0;brd=0;n=0;}
    Component(const Component& b){
        n=b.n;
        num=b.num;
        new2d(brd,char,n,n);
        FOR(i,n)FOR(j,n) brd[i][j]=b[i][j];
    }
    ~Component() {
        clear();
        delete2d(brd,n);
    }
    void init(int _n, const Board &b, int **_num) {
        n=_n;
        num=_num;
        new2d(brd,char,n,n);
        FOR(i,n)FOR(j,n) 
            if(b[i][j] == '.') brd[i][j]='?';
            else brd[i][j]='.';
    }
    void refresh() {
        clear();
        bool **flag;
        new2d(flag,bool,n,n);
        FOR(i,n)FOR(j,n)flag[i][j]=false;
        FOR(i,n)FOR(j,n) if(num[i][j]!=-1)
            ncs.pb(new NumberConnected(n,brd,flag,i,j));
        FOR(i,n)FOR(j,n) if(!flag[i][j])
            if(brd[i][j]=='X'||brd[i][j]=='.')
                wbcs.pb(new Connected(n,brd,flag,i,j));
        delete2d(flag,n);
    }
    void clear() {
        for(auto c:ncs) delete c;
        for(auto c:wbcs) delete c;
        ncs.clear(); wbcs.clear();
    }
    const char *operator[](const int &i)const{return brd[i];}
    void set(int a,int b, char c) {
        brd[a][b]=c;
    }
    bool extend() {
        refresh();
        if(extend_bw('X')) return true;
        if(extend_number()) return true;
        if(extend_bw('.')) return true;
        return false;
    }
    bool extend_bw(char color) {
        vector<Connected*>V;
        for(auto c:wbcs)if(c->color==color)V.pb(c);
        if(color=='X' && V.size()<=1) return false;
        for(auto c:V)
            if(c->extend(n,num,brd)) //prevent double extension issue
                return true;
        return false;
    }
    bool extend_number() {
        bool ext=false;
        for(auto c:ncs) 
            if(c->extend(n,num,brd)) //double extension issue
                ext=true;
        return ext;
    }
    void print() {
        //printf("%d %d %d\n",ncs.size(),wcs.size(),bcs.size());
    }
};
#endif