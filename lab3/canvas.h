#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>

class Canvas {
public:
    constexpr static int HEIGHT = 11;
    constexpr static int WIDTH = 41;

    char buffer[HEIGHT][WIDTH];

    void draw() const {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++)
                std::cout << buffer[i][j];
            std::cout << std::endl;
        }
    }

    void reset() {
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
                buffer[i][j] = ' ';
    }//这个是将整个画布的缓冲区给删除了
};

#endif
