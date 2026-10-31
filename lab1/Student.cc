#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
}

void Student::addClasses(Class* c) {
    classes.push_back(c);
}
double Undergraduate::getAvgScore() const {
    double totalWeighted = 0.0;
    int totalCredits = 0;
    for (Class* c : classes) {
        try {
            StudentWrapper& sw = c->getStudentWrapper(this->id);
            double score = sw.getScore();
            if (score >= 0) {  // 有效分数
                totalWeighted += score * c->point;
                totalCredits += c->point;
            }
        } catch (...) {
        }
    }
    if (totalCredits == 0) return 0.0;
    return totalWeighted / totalCredits;
}

double Undergraduate::getGpa() const {
    double totalGradePoints = 0.0;
    int totalCredits = 0;
    for (Class* c : classes) {
        try {
            StudentWrapper& sw = c->getStudentWrapper(this->id);
            double score = sw.getScore();
            if (score >= 0) {
                double gradePoint = score / 20.0;  // 本科生绩点算法
                totalGradePoints += gradePoint * c->point;
                totalCredits += c->point;
            }
        } catch (...) {
        }
    }
    if (totalCredits == 0) return 0.0;
    return totalGradePoints / totalCredits;
}
double Graduate::getAvgScore() const {
    double totalWeighted = 0.0;
    int totalCredits = 0;
    for (Class* c : classes) {
        try {
            StudentWrapper& sw = c->getStudentWrapper(this->id);
            double score = sw.getScore();
            if (score >= 0) {
                totalWeighted += score * c->point;
                totalCredits += c->point;
            }
        } catch (...) {}
    }
    if (totalCredits == 0) return 0.0;
    return totalWeighted / totalCredits;
}

double scoreToGradPoint(double score) {
    if (score >= 90) return 4.0;
    else if (score >= 80) return 3.5;
    else if (score >= 70) return 3.0;
    else if (score >= 60) return 2.5;
    else return 2.0;
}

double Graduate::getGpa() const {
    double totalGradePoints = 0.0;
    int totalCredits = 0;
    for (Class* c : classes) {
        try {
            StudentWrapper& sw = c->getStudentWrapper(this->id);
            double score = sw.getScore();
            if (score >= 0) {
                double gradePoint = scoreToGradPoint(score);
                totalGradePoints += gradePoint * c->point;
                totalCredits += c->point;
            }
        } catch (...) {}
    }
    if (totalCredits == 0) return 0.0;
    return totalGradePoints / totalCredits;
}
