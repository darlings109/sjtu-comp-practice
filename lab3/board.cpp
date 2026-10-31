#include <iostream>

#include "board.h"
#include "queue.h"

Board::Board(int num_disk)
: num_disk(num_disk),
rods {Rod(num_disk,1),
     Rod(num_disk,2),
     Rod(num_disk,3)} {
    disks = new Disk[num_disk];//生成对应的盘子的个数
    for (int i = num_disk-1; i >= 0; i--) {
        int id=i+1;
        int val=2*id+1;
        disks[i]=Disk(id,val);
        rods[0].push(disks[i]);
    }
}

Board::~Board() {
    delete [] disks;
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    for (int i = 0; i < 3; i++) {
        rods[i].draw(canvas);
    }
    for (int x=0;x<Canvas::WIDTH;++x) {
        canvas.buffer[10][x]='-';
    }
    canvas.buffer[10][5]='|';
    canvas.buffer[10][20]='|';
    canvas.buffer[10][35]='|';
    canvas.draw();
}

void Board::move(int from, int to, const bool log) {
    if (from == to || from < 1 || to < 1 || from >3 || to > 3 ) {
        return;
    }
    if (rods[from-1].empty()){return;}
    if (!rods[to-1].empty() && rods[from-1].top().val < rods[to-1].top().val || rods[to-1].empty()) {
        rods[to-1].push(rods[from-1].top());
        rods[from-1].pop();
    }
    else return;
    if (log) {
        history.push({from, to});
    }
}

bool Board::win() const {
    if (rods[0].empty() && rods[1].full() && rods[2].empty()) {
        return true;
    }
    else return false;
}

void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution
) {
    if (n == 0) return;
    solve(n-1, src, dest, buf, solution);
    solution.push({src, buf});
    solve(n-1, dest, buf, src, solution);
}

void Board::autoplay() {
    while (!history.empty()) {
        auto step = history.top();
        history.pop();
        int a = step.second;
        int b = step.first;
        std::cout<<"Auto moving:"<<a<<" -> "<<b<<std::endl;
        move(a,b,false);
        draw();
    }
    Queue<std::pair<int, int>> solution;
    solve(num_disk,1 ,2 ,3,solution);
    while (!solution.empty()) {
        auto step = solution.front();
        solution.pop();
        int a = step.first;
        int b = step.second;
        std::cout<<"Auto moving:"<<a<<" -> "<<b<<std::endl;
        move(a,b,false);
        draw();
    }
}
