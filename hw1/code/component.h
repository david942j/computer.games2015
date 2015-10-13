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
    VI ids;
    set<int> nbs;
    char color;
    int bnum;
    void read_from(int n,char *brd,bool *flag,int x){
        ids.clear();
        nbs.clear();
        color = brd[x];
        DFS(n,x,flag,brd);
        //sort(ALL(nbs));
        //nbs.resize(unique(ALL(nbs))-nbs.begin());
    }
    void output() {
        printf("boss @ %d\nids: \n",ids[0]);
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
            if(brd[d]=='?') nbs.insert(d);
            else if(brd[d]==color) DFS(n,d,flag,brd);
        }
    }
};
struct Component{
    int n; int **num; // never change
    char *brd;
    Connected* *belong;
    int *Set;
    vector<Connected*>ncs, bcs, wcs;
    #define repeat(magic) for(auto c:ncs)magic;for(auto c:bcs)magic;for(auto c:wcs)magic;
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
        refresh();
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
    }
    void init(int _n, const Board &b, int **_num) {
        n=_n;
        num=_num;
        news();
        FOR(i,n)FOR(j,n) 
            brd[i*n+j]=".?"[b[i][j] == '.'];
        refresh();
    }
    void refresh() {
        clear();
        bool *flag;
        flag = new bool [n*n];
        FOR(i,n*n)flag[i]=false;
        FOR(i,n)FOR(j,n) if(num[i][j]!=-1) {
            Connected *c = new Connected(n,brd,flag,i*n+j,num[i][j]);
            ncs.pb(c);
            for(auto id:c->ids)
                belong[id] = c;
        }
        FOR(i,n*n)if(!flag[i])
            if(brd[i]=='X'||brd[i]=='.') {
                Connected *c = new Connected(n,brd,flag,i,-1);
                if(brd[i]=='X')bcs.pb(c);
                else wcs.pb(c);
                for(auto id:c->ids)
                    belong[id] = c;
            }
        FOR(i,n*n)Set[i]=i;
        repeat(FOR1(i,c->ids.size()-1)U(c->ids[0],c->ids[i]);)
        delete [] flag;
    }
    void clear() {
        repeat(delete c;)
        ncs.clear();
        wcs.clear();
        bcs.clear();
    }
    const char *operator[](const int &i)const{return &brd[i*n];}
    void set(int x,char c) {
        if(brd[x]+c == '.'+'X') throw "Inconsistent set";
        brd[x]=c;
    }
    void set(int a,int b, char c, bool update=true) {
        set(a*n+b,c);
        if(update)update_connections(a,b,c);
    }
    void update_connections(int i,int j, char color) {
        assert(color!='?');
        int id[4],top=0;
        FOR(k,4) {
            int a=i+dx[k],b=j+dy[k];
            if(!inbound(a,b,n,n) || brd[a*n+b]!=color) continue;
            id[top++]=father(a*n+b);
        }
        static int cnt[5]={};
        int x=i*n+j;
        if(top==0) {
            cnt[0]++;
            Connected *c = new Connected;
            c->ids.pb(x);
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n) || brd[a*n+b]!='?') continue;
                c->nbs.insert(a*n+b);
            }
            c->color = color;
            c->bnum = -1;
            belong[x] = c;
            if(color=='.') wcs.pb(c);
            else bcs.pb(c);
            return;
        }
        sort(id,id+top);
        top = unique(id,id+top)-id;
        cnt[top]++;
        if(top==1) {
            //FOR(i,n*n) printf("%d ",Set[i]);
            //printf("%d %c %d %d\n",x,color,id[0],belong[id[0]]);
            Connected *c = belong[id[0]];
            //c->output();
            c->ids.pb(x);
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n) || brd[a*n+b]!='?') continue;
                c->nbs.insert(a*n+b);
            }
            U(id[0],x);
            return;
        }
        refresh();
        //FOR(i,5)printf("%d ",cnt[i]);puts("");
    }
    bool extend() {
        refresh();
        if(extend_bw(bcs,true)) return true;
        if(extend_number()) return true;
        if(extend_bw(wcs,false)) return true;
        return false;
    }
    void output() {
        repeat({c->output();puts("---");})
    }
    bool extend_bw(vector<Connected*>&cs, bool check1) { // need prevent double extension issue
        if(check1 && cs.size()<=1) return false;
        for(auto c:cs) {
            if(c->nbs.size()==0) throw ("GG when extension");
            if(c->nbs.size()>1) continue;
            int x=*c->nbs.begin(),i=x/n,j=x%n;
            set(i,j,c->color);
            return true;
        }
        return false;
    }
    void U(int x,int y){x=father(x);y=father(y);if(x!=y)Set[y]=x;}
    int father(int x){return x==Set[x]?x:Set[x]=father(Set[x]);}
    bool extend_number() { // should have no double extension
        bool ext=false;
        FOR(r,ncs.size()) {
            auto c = ncs[r];
            int bnum = c->bnum;
            if(c->ids.size() > bnum) throw ("More than bnum");
            if(c->ids.size() == bnum){ 
                surround_black(c);
                swap(ncs[r],ncs[ncs.size()-1]);
                ncs.pop_back(); r--;
                delete c;
                continue;
            }
            if(c->nbs.size()==0) throw ("GG when number extension");
            if(c->nbs.size()>1) continue;
            int x=*c->nbs.begin(),i=x/n,j=x%n;
            set(i,j,c->color);
            ext=true;
        }
        return ext;
    }
    void surround_black(Connected *c) {
        for(auto id:c->nbs) set(id,'X'); //bad because keep new / delete
    }
};
#endif