#include "aggfunc.cpp"

int main () {
    // a college object is instantiated, containing all the requisite data read from the
    // .csv files by the read() function (in io.cpp)
    college bc = read();


    // each type of aggregation is stored in a map (which, unlike unordered_map, is sorted by key)
    map<string, vector<double>> pass_rate_ins = rate_by_ins(&bc, &passed);
    map<string, vector<double>> cs_pass_rate_ins = rate_by_ins(&bc, &cs_passed);
    map<string, vector<double>> withdraw_rate_ins = rate_by_ins(&bc, &withdrew);
    map<string, vector<double>> pass_rate_course = rate_by_course(&bc, &passed);
    map<string, vector<double>> cs_pass_rate_course = rate_by_course(&bc, &cs_passed);
    map<string, vector<double>> withdraw_rate_course = rate_by_course(&bc, &withdrew);
    map<string, vector<double>> pass_rate_term = rate_by_term(&bc, &passed);
    map<string, vector<double>> cs_pass_rate_term = rate_by_term(&bc, &cs_passed);
    map<string, vector<double>> withdraw_rate_term = rate_by_term(&bc, &withdrew);

    // with the above mappings, we can produce the outputs:
    print_rate(&pass_rate_ins, "PASS", "INSTRUCTOR");
    print_rate(&cs_pass_rate_ins, "CS PASS", "INSTRUCTOR");
    print_rate(&withdraw_rate_ins, "WITHDRAW", "INSTRUCTOR");
    print_rate(&pass_rate_course, "PASS", "COURSE");
    print_rate(&cs_pass_rate_course, "CS PASS", "COURSE");
    print_rate(&withdraw_rate_course, "WITHDRAW", "COURSE");
    print_rate(&pass_rate_term, "PASS", "TERM");
    print_rate(&cs_pass_rate_term, "CS PASS", "TERM");
    print_rate(&withdraw_rate_term, "WITHDRAW", "TERM");

    print_achievements(&bc, &one_attempt, "IN ONE ATTEMPT");
    print_achievements(&bc, &straight_As, "WITH A GPA OF 4.0");

    return 0;
}
