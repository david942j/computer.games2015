#ifndef COMPONENT_H
#define COMPONENT_H
#include "functions.h"
#include "board.h"
class Connected{
public:
    Connected(){}
    Connected(int n,char *brd,bool *flag,int x,int _bnum){
        bnum=_bnum;
        read_from(n,brd,flag,x);
    }
    VI ids, nbs;
    char color;
    int belong, bnum;
    void read_from(int n,char *brd,bool *flag,int x){
        ids.clear();
        nbs.clear();
        color = brd[x];
        belong = x;
        DFS(n,x,flag,brd);
        sort(ALL(nbs));
        nbs.resize(unique(ALL(nbs))-nbs.begin());
    }
    void output() {
        printf("boss @ %d\nids: \n",belong);
        for(auto c:ids) printf("%d ",c);
        puts("\nneighbors: ");
        for(auto c:nbs) printf("%d ",c);
        puts("");
    }
private:
    void DFS(int n,int x,bool *flag,char *brd) {
        flag[x]=true;
        ids.pb(x);
        FOR(k,4) {
            int a=x/n+dx[k],b=x%n+dy[k], d=a*n+b;
            if(!inbound(a,b,n,n)||flag[d])continue;
            if(brd[d]=='?') nbs.pb(d);
            else if(brd[d]==color) DFS(n,d,flag,brd);
        }
    }
};
/*class NumberConnected:public Connected {
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
        if(V.size() == bnum) return surround_black(n,brd);
        int possible = -1;
        for(auto id:V) {
            int i=id/n,j=id%n;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(brd[a][b]=='?') {
                    if(possible != -1&&possible!=a*n+b) return false; //more than two
                    possible = a*n+b;
                }
            }
        }
        if(possible==-1) throw ("GG when number extension");
        int i=possible/n,j=possible%n;
        brd[i][j]=color;
        return true;
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
};*/
struct Component{
    //vector<NumberConnected*> ncs;
    //vector<Connected*> wbcs;
    int n; int **num; // never change
    char *brd;
    Connected* *belong;
    int *Set;
    vector<Connected*>cs;
    /*
        Connected: ids, neighbors
        extend(): go through neighbors
        merge: bb ww wn
        bb b -> ids merge, neighbors+2 merge uniq, remain one
        ww w -> ids merge, neighbors+2 merge uniq
        wn w -> w into n, neighbors+2 merge uniq
        nn never happend
    */
    Component(){num=0;brd=0;n=0;}
    Component(const Component& b){
        n=b.n;
        num=b.num;
        news();
        FOR(i,n)FOR(j,n) brd[i*n+j]=b[i][j];
//        refresh();
    }
    ~Component() {
        clear();
        delete [] brd;
        delete [] belong;
        delete [] Set;
    }
    void news() {
        brd = new char [n*n];
        belong = new Connected* [n*n];
        Set = new int [n*n];
        FOR(i,n*n)Set[i]=i;
    }
    void init(int _n, const Board &b, int **_num) {
        n=_n;
        num=_num;
        news();
        FOR(i,n)FOR(j,n) 
            brd[i*n+j]=".?"[b[i][j] == '.'];
    }
    void refresh() {
        clear();
        bool *flag;
        flag = new bool [n*n];
        FOR(i,n*n)flag[i]=false;
        FOR(i,n)FOR(j,n) if(num[i][j]!=-1) {
            Connected *c = new Connected(n,brd,flag,i*n+j,num[i][j]);
            cs.pb(c);
            for(auto id:c->ids)
                belong[id] = c;
        }
        FOR(i,n*n)if(!flag[i])
            if(brd[i]=='X'||brd[i]=='.') {
                Connected *c = new Connected(n,brd,flag,i,-1);
                cs.pb(c);
                for(auto id:c->ids)
                    belong[id] = c;
            }
        for(auto c:cs) {
            FOR1(i,c->ids.size()-1)
                U(c->ids[0],c->ids[i]);
        }
        delete [] flag;
    }
    void clear() {
        for(auto c:cs) delete c;
        cs.clear();
    }
    const char *operator[](const int &i)const{return &brd[i*n];}
    void set(int a,int b, char c, bool update=true) {
        if(brd[a*n+b]+c == '.'+'X') throw "Inconsistent set";
        brd[a*n+b]=c;
        if(update)update_connections(a,b,c);
    }
    void update_connections(int a,int b, char c) {
        assert(c!='?');

    }
    bool extend() {
        refresh();
        //output();
        if(extend_bw('X')) return true;
        if(extend_number()) return true;
        if(extend_bw('.')) return true;
        return false;
    }
    void output() {
        for(auto c:cs) {
            c->output();
            puts("---");
        }
    }
    bool extend_bw(char color) { // need prevent double extension issue
        if(color=='X') {
            int cnt=0;
            for(auto c:cs) if(c->color==color)cnt++;
            if(cnt<=1) return false;
        }
        for(auto c:cs) {
            if(c->color!=color) continue;
            if(c->bnum!=-1) continue;
            if(c->nbs.size()==0) throw ("GG when extension");
            if(c->nbs.size()>1) continue;
            int &x=c->nbs[0],i=x/n,j=x%n;
            set(i,j,c->color);
            return true;
        }
        return false;
    }
    void U(int x,int y){x=father(x);y=father(y);if(x!=y)Set[x]=y;}
    int father(int x){return x==Set[x]?x:Set[x]=father(Set[x]);}
    bool extend_number() { // should have no double extension
        bool ext=false;
        for(auto c:cs) {
            int bnum = c->bnum;
            if(bnum==-1)continue;
            if(c->ids.size() > bnum) throw ("More than bnum");
            if(c->ids.size() == bnum){ surround_black(c); continue;}
            if(c->nbs.size()==0) throw ("GG when number extension");
            if(c->nbs.size()>1) continue;
            int x=c->nbs[0],i=x/n,j=x%n;
            set(i,j,c->color);
            ext=true;
        }
        return ext;
    }
    void surround_black(Connected *c) {

    }
    void print() {
        //printf("%d %d %d\n",ncs.size(),wcs.size(),bcs.size());
    }
};
#endif