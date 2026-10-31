#ifndef CLASS_H_
#define CLASS_H_

#include "Student.h"
#include <string>
#include <vector>

class Class {
private:
    std::vector<StudentWrapper> students;

public:
    const std::string name;
    const int point;
    Class(const std::string &name, const int point)
        : name(name), point(point) { }//这里定义了构造函数为带有两个参数的

    std::string toString() const;//这个为显示课程与成绩的函数
    void addStudent(const Student &st);//将学生放入vector
    StudentWrapper& getStudentWrapper(const std::string &studentId);//用于输入学生学号进行查找
    double getHighestScore();
    double getLowestScore();
    double getAvgScore();
    void saveScore(std::ofstream& filename) const;
};

#endif // CLASS_H_
