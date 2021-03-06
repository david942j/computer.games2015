#include "functions.h"
#include "board.h"
#include "component.h"
struct Solver {
    int n;
    Component brd;
    int **num;
    vector<PII> numbers; // {number, i*n+j}
    PII *order;
/****************************
* init functions
****************************/
    Solver(){num=0;n=0;}
    ~Solver(){
        delete2d(num,n);
        delete [] order;
        n=0;
    }
    void init(const Board &b) {
        n=b.size();
        new2d(num,int,n,n);
        order = new PII[n*n];
        FOR(i,n)FOR(j,n)
            if(b[i][j]!='.')num[i][j]=a62toi(b[i][j]);
            else num[i][j]=-1;
        brd.init(n, b, num);
        make_numbers();
    }
    void make_numbers() {
        numbers.clear();
        FOR(i,n)FOR(j,n)if(num[i][j]!=-1)
            numbers.pb({num[i][j],i*n+j});
    }
/****************************
* solve functions
****************************/
    void solve(const Board &b) {
        init(b);
        solve();
    }
    #define NOT_FINISH 0
    #define SUCCESS 1
    #define INVALID -1
    void solve() {
        first_greedy();
        while(improvement());
        int s = solved();
        if(s==SUCCESS) puts("Solved!!");
        else if(s==NOT_FINISH) puts("Can\'t improve any more QQ.");
        else if(s==INVALID) output(),fail("Invalid Solution!!");
        output();
    }
/****************************
* first greedy
****************************/
    void surround1() { // surround 1 with black
        for(auto c:numbers) if(c.X==1){
            int i=c.Y/n,j=c.Y%n;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(brd[a][b]=='.')fail();
                brd.set(a,b,'X', false);
            }
        }
    }
    void untouchable() { //simple untouchable
        FOR(i,n)FOR(j,n) if(brd[i][j]=='?'){
            bool touch=false;
            for(auto c:numbers) {
                int a=c.Y/n,b=c.Y%n;
                if(manhatton(i,j,a,b) < c.X){
                    touch=true;break;
                }
            }
            if(!touch)brd.set(i,j,'X', false);
        }
    }
    void first_greedy() {
        surround1();
        untouchable();
        brd.refresh();
    }
/************end of first greedy****************/
    bool must_black(Component& brd) { //4?2 => 4X2
        //brd.refresh()
        bool got=false;
        FOR(i,n)FOR(j,n) if(brd[i][j]=='?') {
            VI V;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n) || brd[a][b]=='?') continue;
                int f = brd.father(a*n+b);
                if( brd.belong[f]->bnum==-1) continue;
                V.pb(f);
            }
            V.resize(unique(ALL(V))-V.begin());
            if(V.size()>=2) {
                got=true,brd.set(i,j,'X');
            }
        }
        return got;
    }
    bool must_white(Component& brd) {
        bool got=false;
        for(int i=0;i<n-1;i++)for(int j=0;j<n-1;j++){
            char c[4]={brd[i][j],brd[i+1][j],brd[i][j+1],brd[i+1][j+1]};
            // 3b 1?
            int b=0,q=0,id;
            FOR(k,4){
                b+=(c[k]=='X');
                if(c[k]=='?')q++,id=k;
            }
            if(b!=3 || q!=1) continue;
            brd.set(i+(id&1),j+(!!(id&2)),'.');
            got=true;
        }
        return got;
    }
    bool musts(Component&  brd) {
        bool tmp=must_white(brd) | must_black(brd);
        if(brd.extend()) tmp=true;
        return tmp;
    }
    void heuristic_order() {
        FOR(i,n*n)order[i]={0,i};
        for(auto c:brd.ncs) for(auto x:c->nbs)
            order[x].X -= 60/c->nbs.size();
        sort(order,order+n*n);
    }
    bool improvement() {
        if(musts(brd)) return true;
        heuristic_order();
        FOR1(depth,2) //IDFS 
            if(search_extend(depth)) return true;
        return false;
    }
    bool search(Component b, int dlimit, int pos) { // true: dont' know; false: no sol
        if(dlimit == 0) {
            try {
                while(musts(b));
            } catch(const char* e) {
                return false;
            }
            return true;
        }
        while(musts(b));
        for(int i=pos+1;i<n*n;i++){
            int x=order[i].Y;
            if(b.brd[x]=='?') FOR(k,2) {
                b.set(x,".X"[k], false);
                bool r = search(b,dlimit-1,i);
                b.set(x,'?', false);
                if(r)continue;
                b.set(x,"X."[k], false);
                if(search(b,dlimit-1,i)) {b.set(x,'?', false);continue;}
                // no matter .X leads false
                return false;
            }
        }
        return true;
    }
    bool search_extend(int dlimit) {
        FOR(i,n*n) {
            int x=order[i].Y;
            if(brd.brd[x]=='?')FOR(k,2) {
                brd.set(x,".X"[k], false);
                if(!search(brd,dlimit-1,i)) {
                    brd.set(x,'?',false);
                    brd.set(x,"X."[k],true);
                    return true;
                }
                brd.set(x,'?', false);
            }
        }
        return false;
    }
/****************************
* helper functions
****************************/
    int solved() {
        FOR(i,n)FOR(j,n)if(brd[i][j]=='?') return NOT_FINISH;
        brd.refresh();
        for(auto c:brd.ncs) if(c->ids.size() != c->bnum) return INVALID;
        if(brd.bcs.size() != 1) return INVALID;
        if(brd.wcs.size() != 0) return INVALID;
        return SUCCESS;
    }
    void output(Component& b)const {
        b.output_brd();
    }
    void output(){output(brd);}
};