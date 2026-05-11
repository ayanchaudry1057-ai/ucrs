#include "Course.h"
#include "Exceptions.h"
#include <algorithm>
#include <iomanip>

Course::Course(int id, const std::string& name, const std::string& code,
               int credits, int capacity, int teacherId)
    : courseId(id), name(name), code(code), credits(credits), 
      capacity(capacity), enrolledCount(0), teacherId(teacherId) {
    if (name.empty() || code.empty()) {
        throw ValidationException("Course name and code cannot be empty");
    }
    if (credits <= 0 || capacity <= 0) {
        throw ValidationException("Credits and capacity must be positive");
    }
}

void Course::addStudent(int studentId) {
    if (hasStudent(studentId)) {
        throw RegistrationException("Student already enrolled in this course");
    }
    if (isFull()) {
        throw RegistrationException("Course is at full capacity");
    }
    enrolledStudents.push_back(studentId);
    enrolledCount++;
}

void Course::removeStudent(int studentId) {
    auto it = std::find(enrolledStudents.begin(), enrolledStudents.end(), studentId);
    if (it == enrolledStudents.end()) {
        throw RegistrationException("Student not enrolled in this course");
    }
    enrolledStudents.erase(it);
    enrolledCount--;
}

bool Course::hasStudent(int studentId) const {
    return std::find(enrolledStudents.begin(), enrolledStudents.end(), studentId) != enrolledStudents.end();
}

void Course::display() const {
    std::cout << "Course ID: " << courseId << ", Name: " << name 
              << ", Code: " << code << ", Credits: " << credits 
              << ", Enrolled: " << enrolledCount << "/" << capacity << std::endl;
    if (!description.empty()) {
        std::cout << "Description: " << description << std::endl;
    }
}
