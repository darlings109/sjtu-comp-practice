#include "rod.h"
#include <cstddef>

Rod::Rod(const int capacity, const int id) : capacity(capacity), id(id) { }

bool Rod::push(const Disk d) {
    if (full()) {return false;}
    stack.push(d);
    return true;
}

const Disk &Rod::top() {
    assert(capacity>0);
    return stack.top();//这里如果为空的话到时候会返回空的栈顶
}

void Rod::pop() {
    if (!empty()) {
        stack.pop();
    }
}

size_t Rod::size() const {
    return stack.size();
}

bool Rod::empty() const {
    return stack.empty();
}
bool Rod::full() const {
    if (size()>=capacity) {
        return true;
    }
    else return false;
}
void Rod::draw(Canvas &canvas) {
    int center_x;
    if (id==1) center_x=5;
    else if (id==2) center_x=20;
        else center_x=35;//通过杆子编号确定好中心
    for (int y=0;y<10;y++) {
        canvas.buffer[y][center_x]='|';
    }
    Stack<Disk> temp_stack;
    while (!stack.empty()) {
        temp_stack.push(stack.top());
        stack.pop();
    }
    int level=0;
    while (!temp_stack.empty()) {
        Disk d =temp_stack.top();
        temp_stack.pop();
        d.draw(canvas,level,id-1);
        stack.push(d);//进行先出栈，然后在入栈绘画的流程
        level++;
    }
}
