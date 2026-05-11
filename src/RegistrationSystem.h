#ifndef REGISTRATIONSYSTEM_H
#define REGISTRATIONSYSTEM_H

#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "Database.h"
#include "FileHandler.h"
#include <map>
#include <vector>
#include <memory>

/**
 * @class RegistrationSystem
 * @brief Main system orchestrating all operations (students, teachers, courses, registrations)
 */
class RegistrationSystem {
private:
    std::map<int, std::shared_ptr<Student>> students;      // Using STL map for students
    std::map<int, std::shared_ptr<Teacher>> teachers;      // Using STL map for teachers
    std::map<int, std::shared_ptr<Course>> courses;        // Using STL map for courses
    Database db;                                            // SQLite database
    
    int nextStudentId = 1001;
    int nextTeacherId = 2001;
    int nextCourseId = 3001;
    
public:
    RegistrationSystem();
    ~RegistrationSystem() = default;
    
    // Database operations
    void loadFromDatabase();
    void saveToDatabase();
    
    // ===== STUDENT MANAGEMENT =====
    std::shared_ptr<Student> addStudent(const std::string& name, const std::string& email,
                                       const std::string& phone, const std::string& rollNumber,
                                       const std::string& major);
    void removeStudent(int studentId);
    std::shared_ptr<Student> getStudent(int studentId);
    std::vector<std::shared_ptr<Student>> getAllStudents();
    int getStudentCount() const { return students.size(); }
    
    // ===== TEACHER MANAGEMENT =====
    std::shared_ptr<Teacher> addTeacher(const std::string& name, const std::string& email,
                                       const std::string& phone, const std::string& department,
                                       const std::string& qualification);
    void removeTeacher(int teacherId);
    std::shared_ptr<Teacher> getTeacher(int teacherId);
    std::vector<std::shared_ptr<Teacher>> getAllTeachers();
    int getTeacherCount() const { return teachers.size(); }
    
    // ===== COURSE MANAGEMENT =====
    std::shared_ptr<Course> addCourse(const std::string& name, const std::string& code,
                                     int credits, int capacity, int teacherId);
    void removeCourse(int courseId);
    std::shared_ptr<Course> getCourse(int courseId);
    std::vector<std::shared_ptr<Course>> getAllCourses();
    int getCourseCount() const { return courses.size(); }
    
    // ===== REGISTRATION MANAGEMENT =====
    void registerStudentForCourse(int studentId, int courseId);
    void dropStudentFromCourse(int studentId, int courseId);
    std::vector<std::shared_ptr<Course>> getStudentCourses(int studentId);
    std::vector<std::shared_ptr<Student>> getCourseStudents(int courseId);
    
    // ===== GRADE MANAGEMENT =====
    void setGrade(int studentId, int courseId, double grade);
    double getGrade(int studentId, int courseId);
    
    // ===== REPORTS & DISPLAY =====
    void displayAllStudents() const;
    void displayAllTeachers() const;
    void displayAllCourses() const;
    void displayStudentDetails(int studentId) const;
    void displayCourseDetails(int courseId) const;
    void displaySystemStatistics() const;
    
    // ===== FILE OPERATIONS =====
    void exportStudentsToCSV(const std::string& filename);
    void importStudentsFromCSV(const std::string& filename);
    void exportCoursesToCSV(const std::string& filename);
    void importCoursesFromCSV(const std::string& filename);
    
    void exportStudentsToJSON(const std::string& filename);
    void exportCoursesToJSON(const std::string& filename);
};

#endif // REGISTRATIONSYSTEM_H
