#include "RegistrationSystem.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

RegistrationSystem::RegistrationSystem() : db("ucrs.db") {
    try {
        db.connect();
        db.initializeDatabase();
        std::cout << "[INFO] Database initialized successfully" << std::endl;
    } catch (const UCRSException& e) {
        std::cerr << e.what() << std::endl;
    }
}

// ===== STUDENT MANAGEMENT =====
std::shared_ptr<Student> RegistrationSystem::addStudent(const std::string& name, 
                                                       const std::string& email,
                                                       const std::string& phone, 
                                                       const std::string& rollNumber,
                                                       const std::string& major) {
    try {
        auto student = std::make_shared<Student>(nextStudentId++, name, email, phone, rollNumber, major);
        students[student->getId()] = student;
        
        // Save to database
        db.insertStudent(student->getId(), name, email, phone, rollNumber, major, 0.0);
        std::cout << "[SUCCESS] Student " << name << " added successfully" << std::endl;
        return student;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

void RegistrationSystem::removeStudent(int studentId) {
    auto it = students.find(studentId);
    if (it == students.end()) {
        throw StudentException("Student not found with ID: " + std::to_string(studentId));
    }
    
    std::string studentName = it->second->getName();
    students.erase(it);
    try {
        db.deleteStudent(studentId);
        std::cout << "[SUCCESS] Student " << studentName << " removed successfully" << std::endl;
    } catch (const DatabaseException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

std::shared_ptr<Student> RegistrationSystem::getStudent(int studentId) {
    auto it = students.find(studentId);
    if (it == students.end()) {
        throw StudentException("Student not found with ID: " + std::to_string(studentId));
    }
    return it->second;
}

std::vector<std::shared_ptr<Student>> RegistrationSystem::getAllStudents() {
    std::vector<std::shared_ptr<Student>> result;
    for (const auto& pair : students) {
        result.push_back(pair.second);
    }
    return result;
}

// ===== TEACHER MANAGEMENT =====
std::shared_ptr<Teacher> RegistrationSystem::addTeacher(const std::string& name, 
                                                       const std::string& email,
                                                       const std::string& phone, 
                                                       const std::string& department,
                                                       const std::string& qualification) {
    try {
        auto teacher = std::make_shared<Teacher>(nextTeacherId++, name, email, phone, department, qualification);
        teachers[teacher->getId()] = teacher;
        
        db.insertTeacher(teacher->getId(), name, email, phone, department, qualification);
        std::cout << "[SUCCESS] Teacher " << name << " added successfully" << std::endl;
        return teacher;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

void RegistrationSystem::removeTeacher(int teacherId) {
    auto it = teachers.find(teacherId);
    if (it == teachers.end()) {
        throw StudentException("Teacher not found with ID: " + std::to_string(teacherId));
    }
    teachers.erase(it);
}

std::shared_ptr<Teacher> RegistrationSystem::getTeacher(int teacherId) {
    auto it = teachers.find(teacherId);
    if (it == teachers.end()) {
        throw StudentException("Teacher not found with ID: " + std::to_string(teacherId));
    }
    return it->second;
}

std::vector<std::shared_ptr<Teacher>> RegistrationSystem::getAllTeachers() {
    std::vector<std::shared_ptr<Teacher>> result;
    for (const auto& pair : teachers) {
        result.push_back(pair.second);
    }
    return result;
}

// ===== COURSE MANAGEMENT =====
std::shared_ptr<Course> RegistrationSystem::addCourse(const std::string& name, 
                                                     const std::string& code,
                                                     int credits, int capacity, int teacherId) {
    try {
        auto course = std::make_shared<Course>(nextCourseId++, name, code, credits, capacity, teacherId);
        courses[course->getCourseId()] = course;
        
        db.insertCourse(course->getCourseId(), name, code, credits, capacity, teacherId);
        std::cout << "[SUCCESS] Course " << name << " added successfully" << std::endl;
        return course;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

void RegistrationSystem::removeCourse(int courseId) {
    auto it = courses.find(courseId);
    if (it == courses.end()) {
        throw CourseException("Course not found with ID: " + std::to_string(courseId));
    }
    courses.erase(it);
    try {
        db.deleteCourse(courseId);
    } catch (const DatabaseException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

std::shared_ptr<Course> RegistrationSystem::getCourse(int courseId) {
    auto it = courses.find(courseId);
    if (it == courses.end()) {
        throw CourseException("Course not found with ID: " + std::to_string(courseId));
    }
    return it->second;
}

std::vector<std::shared_ptr<Course>> RegistrationSystem::getAllCourses() {
    std::vector<std::shared_ptr<Course>> result;
    for (const auto& pair : courses) {
        result.push_back(pair.second);
    }
    return result;
}

// ===== REGISTRATION MANAGEMENT =====
void RegistrationSystem::registerStudentForCourse(int studentId, int courseId) {
    try {
        auto student = getStudent(studentId);
        auto course = getCourse(courseId);
        
        student->enrollCourse(courseId);
        course->addStudent(studentId);
        
        db.insertRegistration(studentId, courseId);
        std::cout << "[SUCCESS] Student " << student->getName() << " registered for " 
                  << course->getName() << std::endl;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

void RegistrationSystem::dropStudentFromCourse(int studentId, int courseId) {
    try {
        auto student = getStudent(studentId);
        auto course = getCourse(courseId);
        
        student->dropCourse(courseId);
        course->removeStudent(studentId);
        
        db.deleteRegistration(studentId, courseId);
        std::cout << "[SUCCESS] Student " << student->getName() << " dropped from " 
                  << course->getName() << std::endl;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

std::vector<std::shared_ptr<Course>> RegistrationSystem::getStudentCourses(int studentId) {
    std::vector<std::shared_ptr<Course>> studentCourses;
    auto student = getStudent(studentId);
    
    for (int courseId : student->getEnrolledCourses()) {
        try {
            studentCourses.push_back(getCourse(courseId));
        } catch (const CourseException& e) {
            std::cerr << "[WARNING] " << e.what() << std::endl;
        }
    }
    return studentCourses;
}

std::vector<std::shared_ptr<Student>> RegistrationSystem::getCourseStudents(int courseId) {
    std::vector<std::shared_ptr<Student>> courseStudents;
    auto course = getCourse(courseId);
    
    for (int studentId : course->getEnrolledStudents()) {
        try {
            courseStudents.push_back(getStudent(studentId));
        } catch (const StudentException& e) {
            std::cerr << "[WARNING] " << e.what() << std::endl;
        }
    }
    return courseStudents;
}

// ===== GRADE MANAGEMENT =====
void RegistrationSystem::setGrade(int studentId, int courseId, double grade) {
    try {
        auto student = getStudent(studentId);
        student->setGrade(courseId, grade);
        db.setGrade(studentId, courseId, grade);
        std::cout << "[SUCCESS] Grade set for student " << student->getName() << std::endl;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

double RegistrationSystem::getGrade(int studentId, int courseId) {
    try {
        return getStudent(studentId)->getGrade(courseId);
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        throw;
    }
}

// ===== REPORTS & DISPLAY =====
void RegistrationSystem::displayAllStudents() const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << std::setw(40) << std::right << "ALL STUDENTS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    if (students.empty()) {
        std::cout << "No students found." << std::endl;
        return;
    }
    
    for (const auto& pair : students) {
        pair.second->display();
        std::cout << std::string(40, '-') << std::endl;
    }
}

void RegistrationSystem::displayAllTeachers() const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << std::setw(40) << std::right << "ALL TEACHERS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    if (teachers.empty()) {
        std::cout << "No teachers found." << std::endl;
        return;
    }
    
    for (const auto& pair : teachers) {
        pair.second->display();
        std::cout << std::string(40, '-') << std::endl;
    }
}

void RegistrationSystem::displayAllCourses() const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << std::setw(40) << std::right << "ALL COURSES" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    if (courses.empty()) {
        std::cout << "No courses found." << std::endl;
        return;
    }
    
    for (const auto& pair : courses) {
        pair.second->display();
        std::cout << std::string(40, '-') << std::endl;
    }
}

void RegistrationSystem::displayStudentDetails(int studentId) const {
    try {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "STUDENT DETAILS" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        auto student = getStudent(studentId);
        student->display();
        
        std::cout << "\nCourses Enrolled (" << student->getEnrolledCourseCount() << "):" << std::endl;
        for (int courseId : student->getEnrolledCourses()) {
            try {
                auto course = getCourse(courseId);
                std::cout << "  - " << course->getCode() << ": " << course->getName() << std::endl;
                std::cout << "    Grade: " << std::fixed << std::setprecision(2) 
                          << student->getGrade(courseId) << std::endl;
            } catch (const std::exception& e) {
                std::cout << "  - Course ID " << courseId << " not found" << std::endl;
            }
        }
    } catch (const StudentException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::displayCourseDetails(int courseId) const {
    try {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "COURSE DETAILS" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        auto course = getCourse(courseId);
        course->display();
        
        std::cout << "\nEnrolled Students (" << course->getEnrolledCount() << "):" << std::endl;
        for (int studentId : course->getEnrolledStudents()) {
            try {
                auto student = getStudent(studentId);
                std::cout << "  - " << student->getName() << " (" << student->getRollNumber() << ")";
                try {
                    std::cout << " - Grade: " << std::fixed << std::setprecision(2) 
                              << student->getGrade(courseId);
                } catch (...) {
                    std::cout << " - Grade: Not Set";
                }
                std::cout << std::endl;
            } catch (const StudentException& e) {
                std::cout << "  - Student ID " << studentId << " not found" << std::endl;
            }
        }
    } catch (const CourseException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::displaySystemStatistics() const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << std::setw(35) << std::right << "SYSTEM STATISTICS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Total Students: " << students.size() << std::endl;
    std::cout << "Total Teachers: " << teachers.size() << std::endl;
    std::cout << "Total Courses: " << courses.size() << std::endl;
    
    int totalEnrollments = 0;
    for (const auto& pair : courses) {
        totalEnrollments += pair.second->getEnrolledCount();
    }
    std::cout << "Total Enrollments: " << totalEnrollments << std::endl;
}

// ===== FILE OPERATIONS =====
void RegistrationSystem::exportStudentsToCSV(const std::string& filename) {
    try {
        auto studentMaps = db.getStudents();
        FileHandler::exportStudentsToCSV(filename, studentMaps);
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::importStudentsFromCSV(const std::string& filename) {
    try {
        auto studentMaps = FileHandler::importStudentsFromCSV(filename);
        std::cout << "[INFO] Imported " << studentMaps.size() << " students from CSV" << std::endl;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::exportCoursesToCSV(const std::string& filename) {
    try {
        auto courseMaps = db.getCourses();
        FileHandler::exportCoursesToCSV(filename, courseMaps);
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::importCoursesFromCSV(const std::string& filename) {
    try {
        auto courseMaps = FileHandler::importCoursesFromCSV(filename);
        std::cout << "[INFO] Imported " << courseMaps.size() << " courses from CSV" << std::endl;
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::exportStudentsToJSON(const std::string& filename) {
    try {
        auto studentMaps = db.getStudents();
        FileHandler::exportStudentsToJSON(filename, studentMaps);
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

void RegistrationSystem::exportCoursesToJSON(const std::string& filename) {
    try {
        auto courseMaps = db.getCourses();
        FileHandler::exportCoursesToJSON(filename, courseMaps);
    } catch (const UCRSException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}
