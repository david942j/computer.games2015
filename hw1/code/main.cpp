//by david942j
#include "board.h"
#include "solver.h"
Solver solver;
Board brd;
char buf[1000];
int main() {
    printf("Nurikabe Solver Ver %d.%d.%d\n",VERSION/100,VERSION/10%10,VERSION%10);
    brd.read();
    while(fgets(buf,1000,stdin)) printf("%s",buf);
    solver.solve(brd);
}
/*
TODO: support number > 9
20*20:
4619434
5603745

very long time: 20*20 86033

*/