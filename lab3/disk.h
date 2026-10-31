#ifndef DISK_H
#define DISK_H

#include "canvas.h"

class Disk {
public:
    int id;//这里的id作用是什么
    int val;//这里是对应的一行的 * 的数量

    Disk() : id(-1), val(-1) {}
    Disk(const int id, const int val) : id(id), val(val) { }

    void draw(Canvas &canvas, const int level, const int rod_loc) const {
        const int s_x = 5 + (rod_loc * 15) - (val / 2);//这里传入的数值为 0 1 2 然后对应相应的杆子
        const int y = Canvas::HEIGHT - (level + 1) * 2;//这里对应的是 0 1 2 3 4 是对应着相应的盘子序号
        for (int i = 0; i < val; i++) {
            canvas.buffer[y][s_x + i] = '*';
        }//这里只是画了一个盘子，
    }
};//这个是绘画的基本函数，设定了对应的图案，

#endif
