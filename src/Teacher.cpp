#include "Teacher.h"
#include "Exceptions.h"
#include <algorithm>
#include <iostream>

Teacher::Teacher(int id, const std::string& name, 
                 const std::string& email, const std::string& phone,
                 const std::string& department, const std::string& qualification)
    : Person(id, name, email, phone), department(department), qualification(qualification) {
    if (department.empty()) {
        throw ValidationException("Department cannot be empty");
    }
}

void Teacher::assignCourse(int courseId) {
    if (teachesCourse(courseId)) {
        throw CourseException("Teacher already teaches course " + std::to_string(courseId));
    }
    taughtCourses.push_back(courseId);
}

void Teacher::removeCourse(int courseId) {
    auto it = std::find(taughtCourses.begin(), taughtCourses.end(), courseId);
    if (it == taughtCourses.end()) {
        throw CourseException("Teacher does not teach course " + std::to_string(courseId));
    }
    taughtCourses.erase(it);
}

bool Teacher::teachesCourse(int courseId) const {
    return std::find(taughtCourses.begin(), taughtCourses.end(), courseId) != taughtCourses.end();
}

void Teacher::addOfficeHour(int hour) {
    if (hour < 0 || hour > 23) {
        throw ValidationException("Hour must be between 0 and 23");
    }
    officeHours.insert(hour);
}

void Teacher::removeOfficeHour(int hour) {
    officeHours.erase(hour);
}

void Teacher::display() const {
    Person::display();
    std::cout << "Department: " << department << ", Qualification: " << qualification << std::endl;
    std::cout << "Courses Teaching: " << taughtCourses.size() << std::endl;
    std::cout << "Office Hours: " << officeHours.size() << std::endl;
}
