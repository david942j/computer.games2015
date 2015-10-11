#include "functions.h"
#include "board.h"
struct Solver {
    int n;
    int **num;
    char **brd;
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
        FOR(i,n)FOR(j,n) 
            if(b[i][j] == '.') brd[i][j]='?',num[i][j]=-1;
            else brd[i][j]='.',num[i][j]=b[i][j]-'0'; // NUMBER10
    }
    void solve(const Board &b) {
        init(b);
        solve();
    }
    void solve() {
        while(improvement());
        if(solved()) puts("Solved!!");
        else puts("Can\'t improve any more QQ.");
        output();
    }
    bool improvement() {
        return false;
    }
    bool solved() {
        FOR(i,n)FOR(j,n)if(brd[i][j]=='?') return false;
        return true;
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