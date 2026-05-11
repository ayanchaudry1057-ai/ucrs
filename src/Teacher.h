#ifndef TEACHER_H
#define TEACHER_H

#include "Person.h"
#include <vector>
#include <set>

/**
 * @class Teacher
 * @brief Represents a teacher inheriting from Person
 */
class Teacher : public Person {
private:
    std::string department;
    std::string qualification;
    std::vector<int> taughtCourses;  // Course IDs using STL vector
    std::set<int> officeHours;        // Hour slots using STL set
    
public:
    /**
     * @brief Constructor for Teacher
     */
    Teacher(int id = 0, const std::string& name = "", 
            const std::string& email = "", const std::string& phone = "",
            const std::string& department = "", const std::string& qualification = "");
    
    // Getters
    std::string getDepartment() const { return department; }
    std::string getQualification() const { return qualification; }
    std::vector<int> getTaughtCourses() const { return taughtCourses; }
    std::set<int> getOfficeHours() const { return officeHours; }
    
    // Setters
    void setDepartment(const std::string& dept) { department = dept; }
    void setQualification(const std::string& qual) { qualification = qual; }
    
    // Course management
    void assignCourse(int courseId);
    void removeCourse(int courseId);
    bool teachesCourse(int courseId) const;
    int getCourseCount() const { return taughtCourses.size(); }
    
    // Office hours management
    void addOfficeHour(int hour);
    void removeOfficeHour(int hour);
    int getOfficeHourCount() const { return officeHours.size(); }
    
    // Override virtual methods
    void display() const override;
    std::string getType() const override { return "Teacher"; }
};

#endif // TEACHER_H
