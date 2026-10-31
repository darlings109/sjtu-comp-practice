#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
private:
    std::string name;
    std::string year;
    Degree degree;
protected:
    std::vector<Class*> classes;
    Student(const std::string& id, const std::string& name, const std::string& year, Degree deg)
        : id(id), name(name), year(year), degree(deg) {}
public:
    std::string getName() const { return name; }
    const std::string id;
    std::string toString() const;
    virtual ~Student(){}
    virtual double getAvgScore() const=0 ;
    virtual double getGpa ()const =0 ;
    void addClasses(Class *c);
};

class Graduate : public Student {
public:
    double getAvgScore() const override ;
    double getGpa () const override ;
    Graduate(const std::string& id, const std::string& name, const std::string& year)
    : Student(id, name, year, graduate) {}
};

class Undergraduate : public Student {
public:
    double getAvgScore() const override ;
    double getGpa () const override ;
    Undergraduate(const std::string& id, const std::string& name, const std::string& year)
    : Student(id, name, year, undergraduate) {}
};


class StudentWrapper {
private:
    const Student &student;
    double score = -1.0; // Invalid Score
public:
    const std::string id;
    StudentWrapper(const Student &student)
        :id(student.id),student(student){}

    void setScore(double score)
    {
        if (score < 0 || score > 100)
            throw "Wrong Score!";
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
    const Student& getStudent() const { return student; }
};

#endif // STUDENT_H_
