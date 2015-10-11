#ifndef BOARD_H
#define BOARD_H
#include "functions.h"
struct Board { //doesn't support number >= 10 QQ NUMBER10
    int n;
    char **brd;
    Board(){n=0;brd=NULL;}
    ~Board() {
        if(!brd) return;
        FOR(i,n)if(brd[i]) {delete [] brd[i];brd[i]=0;}
        delete [] brd;
        brd=0;
    }
    inline int size()const{return n;}
    char* operator[](const int &i)const {
        assert(i<n);
        return brd[i];
    }
    void read() {
        RI(n);
        new2d(brd,char,n,n+1);
        FOR(i,n)scanf("%s",brd[i]);
    }
    void print() {
        FOR(i,n)printf("%s\n",brd[i]);
    }
};

#endif
/*
5
1....
..2..
.....
.....
3.2.5

5
..2.6
.....
.....
.....
.2..2
*/