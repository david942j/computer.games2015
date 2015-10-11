//by david942j
#include "board.h"
#include "solver.h"
Solver solver;
Board brd;
int main() {
    brd.read();
    solver.solve(brd);
}
/*
15 * 15
http://cn.puzzle-nurikabe.com/?size=3
id : 9654189

*/