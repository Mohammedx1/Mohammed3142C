#include "../headers/CourseReporter.h"

CourseReporter::CourseReporter() {
    reportRatesByInstructor = false;
    reportRatesByCourse = false;
    reportRatesByCourseAndSemester = false;
}

CourseReporter::CourseReporter(bool instructor, bool course, bool courseBySemester) {
    reportRatesByInstructor = instructor;
    reportRatesByCourse = course;
    reportRatesByCourseAndSemester = courseBySemester;
}

bool CourseReporter::reportChosen() {
    if (this->reportRatesByInstructor || this->reportRatesByCourse || this->reportRatesByCourseAndSemester)
        return true;
    else
        return false;
}

void CourseReporter::falsifyOptions() {
    this->reportRatesByInstructor = false;
    this->reportRatesByCourse = false;
    this->reportRatesByCourseAndSemester = false;
}
