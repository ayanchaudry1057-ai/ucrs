#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <iostream>

/**
 * @class Course
 * @brief Represents a course that can be taken by students
 */
class Course {
private:
    int courseId;
    std::string name;
    std::string code;
    std::string description;
    int credits;
    int capacity;
    int enrolledCount;
    int teacherId;
    std::vector<int> enrolledStudents;  // Using STL vector for student IDs
    
public:
    /**
     * @brief Constructor for Course
     */
    Course(int id = 0, const std::string& name = "", const std::string& code = "",
           int credits = 3, int capacity = 50, int teacherId = 0);
    
    // Getters
    int getCourseId() const { return courseId; }
    std::string getName() const { return name; }
    std::string getCode() const { return code; }
    std::string getDescription() const { return description; }
    int getCredits() const { return credits; }
    int getCapacity() const { return capacity; }
    int getEnrolledCount() const { return enrolledCount; }
    int getTeacherId() const { return teacherId; }
    std::vector<int> getEnrolledStudents() const { return enrolledStudents; }
    int getAvailableSeats() const { return capacity - enrolledCount; }
    
    // Setters
    void setName(const std::string& n) { name = n; }
    void setDescription(const std::string& desc) { description = desc; }
    void setCapacity(int cap) { capacity = cap; }
    void setTeacherId(int tid) { teacherId = tid; }
    
    // Student enrollment management
    void addStudent(int studentId);
    void removeStudent(int studentId);
    bool hasStudent(int studentId) const;
    bool isFull() const { return enrolledCount >= capacity; }
    
    // Display
    void display() const;
};

#endif // COURSE_H
