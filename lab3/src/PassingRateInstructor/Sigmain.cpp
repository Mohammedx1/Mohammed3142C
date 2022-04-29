#include <iostream>
#include <vector>
#include <fstream>
#include <experimental/filesystem>
#include <unordered_set>
#include <stdio.h>
#include <map>
#include <set>
#include <ctime>
#include "college.cpp"

using namespace std;
namespace fs = experimental::filesystem::v1;


vector<string> dir_filter() {
    // helper function to filter out non-.csv files
    vector<string> csv_files;
    string extension = ".csv";
    for (const auto& file : fs::directory_iterator("../data/")) {
        // the data submodule is iterated over using filesystem class;
        // any file that matches the .csv extension is added to the vector of files to be read
        if (file.path().extension().string() == extension)
            csv_files.push_back(file.path());
    }
    return csv_files;
}

college read() {
    // college io initializer. reads in files and produces a complete college object out of the data within them
    // reading all the .csv files in the /data directory
    ifstream in_stream;
    vector<string> files = dir_filter(); // paths to only the .csv files
    
    unordered_set<string> spring_terms = {"T04", "T07", "T09", "T12", "T15", "T17"};

    college c;

    for (string file : files) {
        in_stream.open(file);
        if (in_stream.good()) {    
            string header;
            in_stream >> header;
            stringstream head (header);
            string column;

            int header_length = 0;
            while (getline(head, column, ',')) {
                header_length++;
            }

            if (header_length != 6) {
                throw runtime_error ("One or more of the .csv files is incorrectly formatted (missing columns).");
            }

            string lines; //where each page of the file will go to

            string emplid;
            string course_no;
            string ins_id;
            string term_id;
            string sect_code;
            string grade;

            while (in_stream >> lines) {
                //each line's delimited contents are streamed to appropriate var
                stringstream line (lines);
                getline(line, emplid, ',');
                getline(line, course_no, ',');
                getline(line, ins_id, ',');
                getline(line, term_id, ',');
                getline(line, sect_code, ',');
                getline(line, grade);

                //each course section instance is labeled with a unique code
                //(concatenation of course_no, term_id, and section code)
                string class_id = term_id + '.' + course_no + '.' + sect_code;
                //we attempt to retrieve the section and term from the college
                section* sect = c.get_section(class_id);
                term* t = c.get_term(term_id);
                //if section is not already in college, we register it now:
                if(!sect) {
                    sect = new section (class_id, course_no, sect_code, ins_id, term_id);
                    c.schedule_course(class_id, sect);
                }

                if(!t) {
                    t = new term(term_id, class_id, spring_terms.count(term_id));
                    c.schedule_term(term_id, t);
                }
                
                //student from the current line gets added to the section
                sect->add_student(emplid);
                t->add_course(class_id);
                //student and instructor from current line are added to the college
                c.enroll_student(emplid, class_id, grade);
                c.assign_instructor(ins_id, class_id);
            }
        }
        in_stream.close();
    }

    return c;
}


// output file declarations. we need a C file object (for fprintf()) as well as a C time object for timestamping 
FILE* out_file;
ofstream out_stream;
time_t timer;
struct tm * timeinfo;

void print_rate(map<string, vector<double>>* aggregate, string criterion1, string criterion2) {
    char dte[30]; // C buffer for time formatting
    time(&timer); 
    timeinfo = localtime(&timer); // current local time
    strftime (dte, 30, "%d-%m-%y_%H:%M", timeinfo); // time format to be concatenated onto the filename
    string date (dte); // quick change into C++ string for easy concatenation
    string filename = "../output/" + date + "_" + criterion1 + "_" + criterion2 + ".txt";
    const char * fname = filename.c_str(); // back to C string 
    out_file = fopen(fname, "w"); // open new file for writing
    fprintf(out_file, "                 ----- %s RATE BY %s -----\n", criterion1.c_str(), criterion2.c_str());
    fprintf(out_file, "%10s: %13s COUNT:         ATTEMPTED:          RATIO:\n", criterion2.c_str(), criterion1.c_str());
    for (const auto& i : *aggregate) {
        // for key in aggregated map
        float ratio = (i.second[0]/i.second[1]*100);
        fprintf(out_file, "%10s %20d %18d %14.1f %% \n", i.first.c_str(), int (i.second[0]), int (i.second[1]), ratio);
    }
    fclose(out_file);
}

void print_achievements(college* bc, bool (*criterion)(unordered_map<string, string>*), string mode) {
    // slightly different printing function as the data is aggregated right before printing:
    char dte[30];
    time(&timer);
    timeinfo = localtime(&timer);
    strftime (dte, 30, "%d-%m-%y_%H:%M", timeinfo);
    string date (dte);
    string filename = "../output/" + date + "_" + mode + ".txt";
    const char * fname = filename.c_str();
    out_file = fopen(fname, "w");
    fprintf(out_file, "--------- STUDENTS WHO COMPLETED THE INTRO SEQUENCE %s ----------\n", mode.c_str());
    double count = 0;
    double total = 0;
    for (const auto& s : *bc->get_students()) {
        if (criterion(&s.second->classes)) {
            count++;
        }
        total++;
    }
    float ratio = (count / total)*100;
    fprintf(out_file, "     Completed: %10d;    Attempted: %10d;     Ratio: %10.1f%%", int(count), int(total), ratio);
    fclose(out_file);
}
