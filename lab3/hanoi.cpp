#include <climits>
#include <iostream>
#include <string>

#include "board.h"

#include <limits>

using namespace std;

int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }
        if (input =="1"||input=="2"||input=="3"||input=="4"||input=="5") {//由于是字符串，所以是用双引号
            int num=stoi(input);
            Board board(num);
            board.draw();
            while (true) {
                cout << "Move a disk. Format: x y"<<endl;
                int from,to;
                cin >> from  >> to;
                if (from == 0 && to ==0) board.autoplay();
                else {
                    board.move(from,to,true);//还需要检查一下合理，这个到board里面处理
                    board.draw();
                }
                if (board.win()) {
                    cout<<"Congratulations! You win!"<<endl;
                    break;
                }
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');//因为使用了cin缓冲符号留着换行符号，会输出两遍
        }//这里面是对于正确的输入的操作
        else {
            continue;
        }//这里使用的是getline缓冲区已经被清空，所以只要继续
    }
    return 0;
}
