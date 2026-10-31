#include "Class.h"
#include <string>
#include <vector>
#include "Student.h"
#include <sstream>
#include<fstream>
#include <iostream>

std::string Class::toString() const {
    std::ostringstream oss;
    oss << "Class Information:"
        << "\n\tname: " << name
        << "\n\tpoint: " << point
        << std::endl;
    return oss.str();
}

void Class::addStudent(const Student& st) {
    StudentWrapper sw(st);
    students.push_back(sw);
}

StudentWrapper& Class::getStudentWrapper(const std::string& studentId) {
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
        if (it->id == studentId)
            return *it;
    }
    throw "No Match Student!";
}

double Class::getHighestScore() {
    double highestScore=-1.00;//数据要求为两位小数
    bool hasvalid=false;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
        it != students.end();
        ++ it) {
        double goalScore = it->getScore();
        if (goalScore >= 0){
            hasvalid=true;
            if (goalScore > highestScore)
            highestScore = goalScore;
        }
    }
    if (!hasvalid) {
        throw "No Valid Score!";
    }
    return highestScore;
}

double Class::getLowestScore() {
    double lowestScore=101.00;
    bool hasvalid=false;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
        it != students.end();
        ++ it) {
        double goalScore = it->getScore();
        if (goalScore>=0){
            hasvalid=true;
            if (goalScore < lowestScore)
                lowestScore = goalScore;
        }
    }
    if (!hasvalid) {
        throw "No Valid Score";
    }
    return lowestScore;
}

double Class::getAvgScore() {
    double tolScore=0.00;
    int i=0;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
        it != students.end();
        ++it ) {
        double goalScore = it->getScore();
        if (goalScore >= 0) {
            tolScore+=goalScore;
            ++i;
        }
    }
    if (i==0) {
        throw "No Valid Score!";
    }
    double avgScore=tolScore/i;
    return avgScore;
}

void Class::saveScore( std::ofstream& out) const{
    out << name << std::endl;
    for (const auto& wrapper : students) {
        double score = wrapper.getScore();

        if (score < 0) continue;

        const Student& stu = wrapper.getStudent();   // 获取学生对象
        out << wrapper.id << ","                       // 学号
            << score << std::endl;                      // 成绩
    }
}
