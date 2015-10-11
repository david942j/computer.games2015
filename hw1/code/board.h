#include "functions.h"
struct Board { //doesn't support number > 0 QQ
    int n;
    char **brd;
    Board(){n=0;brd=NULL;}
    ~Board() {
        FOR(i,n)if(brd[i]) {delete brd[i];brd[i]=0;}
        if(brd)delete brd;
        brd=0;
    }
    char* operator[](const int &i) {
        assert(i<n);
        return brd[i];
    }
    void read() {
        RI(n);
        brd = new char*[n];
        FOR(i,n)brd[i]=new char[n+1];
        FOR(i,n)scanf("%s",brd[i]);
    }
    void print() {
        FOR(i,n)printf("%s\n",brd[i]);
    }
};
/*
5
1....
.....
.....
.....
3.4.4

*/