#include "Student.h"
#include "Exceptions.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

Student::Student(int id, const std::string& name, 
                 const std::string& email, const std::string& phone,
                 const std::string& rollNumber, const std::string& major)
    : Person(id, name, email, phone), rollNumber(rollNumber), major(major), gpa(0.0) {
    if (rollNumber.empty()) {
        throw ValidationException("Roll number cannot be empty");
    }
}

void Student::enrollCourse(int courseId) {
    if (isEnrolled(courseId)) {
        throw RegistrationException("Student already enrolled in course " + std::to_string(courseId));
    }
    enrolledCourses.push_back(courseId);
}

void Student::dropCourse(int courseId) {
    auto it = std::find(enrolledCourses.begin(), enrolledCourses.end(), courseId);
    if (it == enrolledCourses.end()) {
        throw RegistrationException("Student not enrolled in course " + std::to_string(courseId));
    }
    enrolledCourses.erase(it);
    grades.erase(courseId);
}

bool Student::isEnrolled(int courseId) const {
    return std::find(enrolledCourses.begin(), enrolledCourses.end(), courseId) != enrolledCourses.end();
}

double Student::getGrade(int courseId) const {
    auto it = grades.find(courseId);
    if (it != grades.end()) {
        return it->second;
    }
    throw StudentException("No grade found for course " + std::to_string(courseId));
}

void Student::setGrade(int courseId, double grade) {
    if (grade < 0 || grade > 100) {
        throw ValidationException("Grade must be between 0 and 100");
    }
    if (!isEnrolled(courseId)) {
        throw RegistrationException("Student not enrolled in course " + std::to_string(courseId));
    }
    grades[courseId] = grade;
    calculateGPA();
}

void Student::calculateGPA() {
    if (grades.empty()) {
        gpa = 0.0;
        return;
    }
    double sum = 0.0;
    for (const auto& g : grades) {
        sum += g.second;
    }
    gpa = sum / grades.size();
}

void Student::display() const {
    Person::display();
    std::cout << "Roll: " << rollNumber << ", Major: " << major 
              << std::fixed << std::setprecision(2) << ", GPA: " << gpa << std::endl;
    std::cout << "Enrolled Courses: " << enrolledCourses.size() << std::endl;
}
