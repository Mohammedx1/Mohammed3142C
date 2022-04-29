#include <string>
#include <unordered_map>

using namespace std;

struct student { 
    string emplid;
    unordered_map<string, string> classes; // all the classes the student attempted - key is the unique class_id

    student(string emplid, string course, string grade) {
        this->emplid = emplid;
        classes[course] = grade;
    }

    void add_course(string course, string grade) {
        classes[course] = grade;
    }
};
