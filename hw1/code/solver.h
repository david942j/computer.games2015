#include "functions.h"
#include "board.h"
#include "component.h"
struct Solver {
    int n;
    Component brd;
    int **num;
    vector<PII> numbers; // {number, i*n+j}
/****************************
* init functions
****************************/
    Solver(){num=0;n=0;}
    ~Solver(){
        delete2d(num,n);
        n=0;
    }
    void init(const Board &b) {
        n=b.size();
        new2d(num,int,n,n);
        FOR(i,n)FOR(j,n)
            if(b[i][j]!='.')num[i][j]=b[i][j]-'0'; // NUMBER10
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
    void solve() {
        first_greedy();
        while(improvement());
        if(solved()) puts("Solved!!");
        else puts("Can\'t improve any more QQ.");
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
                brd.set(a,b,'X');
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
            if(!touch)brd.set(i,j,'X');
        }
    }
    void first_greedy() {
        surround1();
        untouchable();
    }
/************end of first greedy****************/
    bool must_black(Component& brd) { //4?2 => 4X2
        int **s;new2d(s,int,n,n);
        brd.refresh();
        FOR(i,n)FOR(j,n)s[i][j]=-1;
        for(auto nc: brd.ncs){
            for(auto c:nc->V) s[c/n][c%n] = nc->belong;
        }
        bool got=false;
        FOR(i,n)FOR(j,n) if(brd[i][j]=='?') {
            VI V;
            FOR(k,4) {
                int a=i+dx[k],b=j+dy[k];
                if(!inbound(a,b,n,n)) continue;
                if(s[a][b]==-1) continue;
                V.pb(s[a][b]);
            }
            V.resize(unique(ALL(V))-V.begin());
            if(V.size()>=2) got=true,brd.set(i,j,'X');
        }
        delete2d(s,n);
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
    bool improvement() {
        if(musts(brd)) return true;
        FOR1(depth,2) //IDFS
            if(stupid_search_extend(depth)) return true;
        return false;
    }
    bool stupid_search(Component& brd, int dlimit) { // true: dont' know; false: no sol
        Component b(brd);
        if(dlimit == 0) {
            try {
                while(musts(b));
            } catch(const char* e) {
                return false;
            }
            return true;
        }
        while(musts(b));
        FOR(i,n)FOR(j,n) if(b[i][j]=='?') FOR(k,2) {
            b.set(i,j,".X"[k]);
            bool r = stupid_search(b,dlimit-1);
            b.set(i,j,'?');
            if(r)continue;
            b.set(i,j,"X."[k]);
            if(stupid_search(b,dlimit-1)) {b.set(i,j,'?');continue;}
            // no matter .X leads false
            return false;
        }
        return true;
    }
    bool stupid_search_extend(int dlimit) {
        Component b(brd);
        FOR(i,n)FOR(j,n) if(b[i][j]=='?') FOR(k,2){
            b.set(i,j,".X"[k]);
            if(!stupid_search(b,dlimit-1)) {
                brd.set(i,j,"X."[k]);
                return true;
            }
            b.set(i,j,'?'); // hard to handle...
        }
        return false;
    }
/****************************
* helper functions
****************************/
    bool solved() {
        FOR(i,n)FOR(j,n)if(brd[i][j]=='?') return false;
        return true;
    }
    inline char char_at(int y) {
        return brd[y/n][y%n];
    }
    void output(Component& b)const {
        puts("==================");
        FOR(i,n) {
            FOR(j,n)if(num[i][j]!=-1)printf(" %d",num[i][j]);
                else printf(" %c",b[i][j]);
            puts("");
        }
        puts("==================");
    }
    void output(){output(brd);}
};