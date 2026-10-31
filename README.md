# sjtu-comp-practice

上海交通大学《计算机编程实践》课程作业，三个实验放在同一个仓库里，方便一起看。

| 实验 | 内容 | 主要文件 |
| :-- | :-- | :-- |
| **lab1** | C++ 类与对象：学生成绩管理系统 | `Class.cc` `Student.cc` `main.cc` |
| **lab2** | y86-64 指令集模拟器：存储器与寄存器 | `simulator.cpp` `instructions.cpp` `memory.cpp` `registers.cpp` |
| **lab3** | 汉诺塔可视化：栈与队列的应用 | `board.cpp` `canvas.h` `disk.h` |

## 构建

每个实验用 CMake：

```bash
cd lab1
cmake -B build && cmake --build build
```

## 说明

- 课程提供的实验说明与 PDF 不在此仓库内。
- `lab1/Bug Report.txt` 是当时调试 const 成员初始化时写的排查记录，一并留着。
- 测试数据（`Students.txt` / `scores.txt`）是课程给的样例，不是真实学生信息。

## License

课程作业，仅供学习参考。
