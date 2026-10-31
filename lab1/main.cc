#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Class.h"
#include "Student.h"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printScoreStats();
    void printGrade();
    void saveScore();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles() {
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    ifstream stfile("./Students.txt");
    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each column in the line.
        bufv.clear();
        // Split the line into columns.
        //   pos1: beginning of the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }
        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);


        studentVec.push_back(st);
    }
    stfile.close();


    ifstream stfile2("./Classes.txt");
    string key;
    string value;
    vector<string> curStudentIds;
    bool intheclass=false;
    while (getline(stfile2, line )) {
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;
        size_t clonopoint=line.find(':');
        if (clonopoint != string::npos) {
            key= line.substr(0, clonopoint);
            value = line.substr(clonopoint + 1, string::npos);


            if (key=="Class") {
                if (intheclass) {
                    cl=new Class(clsname,point);
                    for (const string& sid : curStudentIds) {
                        Student* stu=nullptr;
                        for (auto s:studentVec) {
                            if (s->id == sid) {
                                stu = s;
                                break;
                            }
                        }
                        if (stu) {
                            cl->addStudent(*stu);
                            stu->addClasses(cl);
                        }
                        else cout<<"Warning :not find the student."<<endl;
                    }
                    classVec.push_back(cl);
                }
                clsname=value;
                point=0;
                curStudentIds.clear();
                intheclass=true;
            }
            if (key=="Points") {
                point=stoi(value);
            }
        }
        else {
            if (intheclass) {
                curStudentIds.push_back(line);
            }
        }
    }
    if (intheclass) {
        cl=new Class(clsname,point);
        for (const string& sid : curStudentIds) {
            Student* stu=nullptr;
            for(auto s:studentVec) {
                if (s->id == sid) {
                    stu = s;
                    break;
                }
            }
            if (stu) {
                cl->addStudent(*stu);
                stu->addClasses(cl);
            }
            else cerr<<"Warning :not find the student."<<endl;
        }
            classVec.push_back(cl);
        }
    // Hint: how is student information read?
    stfile2.close();
}

void AppX::inputScore()
{
    string className;
    while (true) {
        cout << "Please input the name of class (or input 'q' to quit): " << endl;
        cin >> className;
        if (className == "q") break;  // 退出整个输入模式

        Class* cl = nullptr;
        for (auto c : classVec) {
            if (c->name == className) {
                cl = c;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class" << endl;
            continue;  // 继续输入下一个课程名
        }

        // 内层循环：输入该课程的学号和成绩，直到遇到 'q'
        string line;
        while (true) {
            cout << "Please input the student id and the score (or 'q' to quit): " << endl;
            cin >> line;
            if (line == "q") break;  // 结束当前课程的成绩输入

            size_t comma = line.find(',');
            if (comma == string::npos) {
                cerr << "Wrong Score" << endl;
                continue;
            }
            string studentId = line.substr(0, comma);
            string scoreStr = line.substr(comma + 1);
            double score;
            try {
                score = stod(scoreStr);
            } catch (...) {
                cerr << "Wrong Score" << endl;
                continue;
            }

            try {
                StudentWrapper& sw = cl->getStudentWrapper(studentId);
                sw.setScore(score);
            } catch (const char* msg) {
                cerr << msg << endl;  // 可能输出 "No Match Student" 或 "Wrong Score"
            }
        }
    }
}
void AppX::printScoreStats()
{
    string sbuf;
    Class *cl;
    double highest, lowest, avg;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
                it != classVec.end();
                ++ it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class!" << endl;
            continue;
        }

        try {
            highest = cl->getHighestScore();
            lowest = cl->getLowestScore();
            avg = cl->getAvgScore();
            cout << cl->toString() << endl;
            cout << setiosflags(ios::fixed) << setprecision(2)<< "Highest,Lowest,Avg = " << highest << "," << lowest << "," << avg << endl;
        } catch (string e) {
            cerr << e << endl;
        } catch (char const* e){
            cerr << e << endl;
        } catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
    }
}

void AppX::printGrade()
{
    // TODO: implement printGrade.
    // Hint: Take a look at printScoreStats().
    string studentid2;
    while (true) {
        cout << "Please input the student id : ";
        cin >>studentid2;
        if (studentid2 =="q") break;
        Student *st = nullptr;
        for (auto s:studentVec) {
            if (s->id == studentid2) {
                st= s;
                break;
            }
        }
        if (st == nullptr) {
            cerr<<"No Match Student"<<endl;
            continue;
        }
        try {
            double gpa =st->getGpa();
            double avg =st->getAvgScore();
            cout << fixed << setprecision(2) << "GPA,AVG = " << gpa << "," << avg << endl;
        } catch (const char* msg) {
            cerr << msg << endl;
        }
    }
}

void AppX::saveScore() {
    std::ofstream out("scores.txt", std::ios::trunc); // 以截断模式打开（清空）
    if (!out.is_open()) return;
    for (Class* c : classVec) {
        c->saveScore(out);
    }
    out.close();
}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute score statistics of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printScoreStats();
        } else if (cmd == 'g') {
            printGrade();
        } else if (cmd == 'q') {
            saveScore();
            break;
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}
