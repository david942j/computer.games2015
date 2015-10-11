#include "functions.h"
#include "board.h"
#include "component.h"
struct Solver {
    int n;
    int **num;
    char **brd;
    vector<PII> numbers; // {number, i*n+j}
/****************************
* init functions
****************************/
    Solver(){num=0;brd=0;n=0;}
    ~Solver(){
        delete2d(num,n);
        delete2d(brd,n);
        n=0;
    }
    void init(const Board &b) {
        n=b.size();
        new2d(num,int,n,n);
        new2d(brd,char,n,n);
        numbers.clear();
        FOR(i,n)FOR(j,n) 
            if(b[i][j] == '.') brd[i][j]='?',num[i][j]=-1;
            else brd[i][j]='.',num[i][j]=b[i][j]-'0'; // NUMBER10
        make_numbers();
    }
    void make_numbers() {
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
                brd[a][b]='X';
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
            if(!touch)brd[i][j]='X';
        }
    }
    void first_greedy() {
        surround1();
        untouchable();
    }
/************end of first greedy****************/
    bool must_black(char **brd) { //4?2 => 4X2
        bool **flag; int **s;
        new2d(flag,bool,n,n); new2d(s,int,n,n);
        FOR(i,n)FOR(j,n)flag[i][j]=false,s[i][j]=-1;
        FOR(i,n)FOR(j,n)if(num[i][j]!=-1){
            NumberConnected nc(n,brd,flag,i,j);
            for(auto c:nc.V) s[c/n][c%n] = nc.belong;
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
            if(V.size()>=2) got=true,brd[i][j]='X';
        }
        delete2d(flag,n); delete2d(s,n);
        return got;
    }
    bool must_white(char **brd) {
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
            brd[i+(id&1)][j+(!!(id&2))]='.';
            got=true;
        }
        return got;
    }
    bool musts(char **brd) {
        bool tmp=must_white(brd) | must_black(brd);
        Component c(n,num,brd);
        if(c.extend()) tmp=true;
        return tmp;
    }
    bool improvement() {
        if(musts(brd)) return true;
        FOR1(depth,2) //IDFS
            if(stupid_search_extend(brd,depth)) return true;
        return false;
    }
    bool stupid_search(char **brd, int dlimit) { // true: dont' know; false: no sol
        char **b; new2d(b,char,n,n);
        FOR(i,n)FOR(j,n)b[i][j]=brd[i][j];
        if(dlimit == 0) {
            try {
                while(musts(b));
            } catch(const char* e) {
                delete2d(b,n);
                return false;
            }
            delete2d(b,n);
            return true;
        }
        while(musts(b));
        FOR(i,n)FOR(j,n) if(b[i][j]=='?') FOR(k,2) {
            b[i][j]=".X"[k];
            bool r = stupid_search(b,dlimit-1);
            b[i][j]='?';
            if(r)continue;
            b[i][j]="X."[k];
            if(stupid_search(b,dlimit-1)) {b[i][j]='?';continue;}
            // no matter .X leads false
            return false;
        }
        return true;
    }
    bool stupid_search_extend(char **brd,int dlimit) {
        if(VERSION < 120) return false;
        char **b; new2d(b,char,n,n);
        FOR(i,n)FOR(j,n)b[i][j]=brd[i][j];
        FOR(i,n)FOR(j,n) if(b[i][j]=='?') FOR(k,2){
            b[i][j]=".X"[k];
            if(!stupid_search(b,dlimit-1)) {
                brd[i][j]="X."[k];
                delete2d(b,n);
                return true;
            }
            b[i][j]='?';
        }
        delete2d(b,n);
        return false;
    }
    int search(char **brd, int dlimit) { //-1 don't know; 0 GG; 1 Good
        if(dlimit<=0) return -1;
        char **b; new2d(b,char,n,n);
        FOR(i,n)FOR(j,n)b[i][j]=brd[i][j];
        Component c(n,num,b);

        delete2d(b,n);
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
    void output(char **b=NULL)const {
        if(!b)b=brd;
        puts("==================");
        FOR(i,n) {
            FOR(j,n)if(num[i][j]!=-1)printf(" %d",num[i][j]);
                else printf(" %c",b[i][j]);
            puts("");
        }
        puts("==================");
    }
};