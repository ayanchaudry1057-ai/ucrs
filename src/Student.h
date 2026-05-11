#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include <vector>
#include <map>

/**
 * @class Student
 * @brief Represents a student inheriting from Person
 */
class Student : public Person {
private:
    std::string rollNumber;
    std::string major;
    double gpa;
    std::vector<int> enrolledCourses;  // Course IDs using STL vector
    std::map<int, double> grades;       // Course ID -> Grade using STL map
    
public:
    /**
     * @brief Constructor for Student
     */
    Student(int id = 0, const std::string& name = "", 
            const std::string& email = "", const std::string& phone = "",
            const std::string& rollNumber = "", const std::string& major = "");
    
    // Getters
    std::string getRollNumber() const { return rollNumber; }
    std::string getMajor() const { return major; }
    double getGPA() const { return gpa; }
    std::vector<int> getEnrolledCourses() const { return enrolledCourses; }
    double getGrade(int courseId) const;
    
    // Setters
    void setRollNumber(const std::string& roll) { rollNumber = roll; }
    void setMajor(const std::string& major) { this->major = major; }
    void setGPA(double gpa) { this->gpa = gpa; }
    void setGrade(int courseId, double grade);
    
    // Course management
    void enrollCourse(int courseId);
    void dropCourse(int courseId);
    bool isEnrolled(int courseId) const;
    int getEnrolledCourseCount() const { return enrolledCourses.size(); }
    
    // Override virtual methods
    void display() const override;
    std::string getType() const override { return "Student"; }
    
    // Utility
    void calculateGPA();
};

#endif // STUDENT_H
