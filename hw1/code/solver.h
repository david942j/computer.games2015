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
        if(!num) return;
        FOR(i,n){delete [] num[i];num[i]=0;delete[]brd[i];brd[i]=0;}
        delete [] num;
        delete [] brd;
        num=0;
        brd=0;
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
    void blackin2() { //a?b => aXb
        FOR(i,n)FOR(j,n)
            if(brd[i][j]=='?')
                FOR(k,2) {
                    int a1=i+dx[k],b1=j+dy[k];
                    int a2=i+dx[k^2],b2=j+dy[k^2];
                    if(!inbound(a1,b1,n,n) || !inbound(a2,b2,n,n)) continue;
                    if(num[a1][b1]==-1||num[a2][b2]==-1)continue;
                    if(brd[i][j]=='.')fail();
                    brd[i][j]='X';
                }
    }
    void untouchable() {
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
        blackin2();
        untouchable();
    }
/************end of first greedy****************/
    bool must_black() {
        return false;
    }
    bool must_white() {
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
    bool improvement() {
        bool imp=false;
        //if(must_black()) imp=true;
        if(must_white()) imp=true;
        search();
        return imp;
    }
    void search() {
        Component c(n,num,brd);
        c.extend();
        //c.print();
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
    void output()const {
        puts("==================");
        FOR(i,n) {
            FOR(j,n)if(num[i][j]!=-1)printf("%d",num[i][j]);
                else printf("%c",brd[i][j]);
            puts("");
        }
        puts("==================");
    }
};