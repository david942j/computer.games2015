//by david942j
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#ifndef VERSION
#define VERSION 141
#endif

#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <deque>
#include <cassert>
#include <queue>
#include <stack>
#include <cstdlib>
#include "unistd.h"
#define mpr std::make_pair
#define lg(x) (31-__builtin_clz(x))
#define lgll(x) (63-__builtin_clzll(x))
#define __count __builtin_popcount
#define __countll __builtin_popcountll
#define X first
#define Y second
#define mst(x) memset(x,0,sizeof(x))
#define mst1(x) memset(x,-1,sizeof(x))
#define ALL(c) (c).begin(),(c).end()
#define FOR(i,n) for(int i=0;i<n;i++)
#define FOR1(i,n) for(int i=1;i<=n;i++)
#define FORit(it,c) for(__typeof(c.begin()) it=c.begin();it!=c.end();++it)
#define pb push_back
#define RI(x) scanf("%d",&x)
#define RID(x) int x;RI(x)
#define new2d(arr,type,n,m) arr=new type *[n];FOR(i,n)arr[i]=new type [m];
#define delete2d(arr,n) if(arr){FOR(i,n)if(arr[i]){delete[]arr[i];arr[i]=0;}delete[]arr;arr=0;}
using namespace std;
typedef long long LL;
typedef double LD;
typedef vector<int> VI;
typedef std::pair<int,int> PII;
template<class T>inline void maz(T &a,T b){if(a<b)a=b;}
template<class T>inline void miz(T &a,T b){if(a>b)a=b;}
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
#define inbound(i,j,n,m) (i>=0&&j>=0&&i<n&&j<m)
void fail(const char *s="No sol"){printf("%s\n",s); assert(false);}
inline int manhatton(int a,int b,int c,int d){return abs(c-a)+abs(d-b);}
inline int a62toi(char c) {
    if(c>='0' && c<='9') return c-'0';
    if(c>='a' && c<='z') return c-'a'+10;
    if(c>='A' && c<='Z') return c-'A'+10+26;
    return -1;
}
inline char itoa62(int i) {
    if(i<=9) return '0'+i;
    i-=10;
    if(i<26) return 'a'+i;
    i-=26;
    if(i<26) return 'A'+i;
    return 0;
}
#endif