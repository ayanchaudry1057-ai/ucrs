#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include "Exceptions.h"

/**
 * @class Database
 * @brief Handles all SQLite database operations for UCRS
 */
class Database {
private:
    sqlite3* db;
    std::string dbPath;
    
    // Helper methods
    void executeSQL(const std::string& sql);
    
public:
    /**
     * @brief Constructor for Database
     * @param path Path to the SQLite database file
     */
    Database(const std::string& path = "ucrs.db");
    ~Database();
    
    // Connection management
    void connect();
    void disconnect();
    bool isConnected() const { return db != nullptr; }
    
    // Schema initialization
    void initializeDatabase();
    
    // ===== STUDENT OPERATIONS =====
    void insertStudent(int id, const std::string& name, const std::string& email,
                      const std::string& phone, const std::string& rollNumber,
                      const std::string& major, double gpa);
    void updateStudent(int id, const std::string& name, const std::string& email,
                      const std::string& phone, double gpa);
    void deleteStudent(int id);
    std::vector<std::map<std::string, std::string>> getStudents();
    std::map<std::string, std::string> getStudent(int id);
    
    // ===== TEACHER OPERATIONS =====
    void insertTeacher(int id, const std::string& name, const std::string& email,
                      const std::string& phone, const std::string& department,
                      const std::string& qualification);
    void updateTeacher(int id, const std::string& name, const std::string& email,
                      const std::string& phone);
    void deleteTeacher(int id);
    std::vector<std::map<std::string, std::string>> getTeachers();
    std::map<std::string, std::string> getTeacher(int id);
    
    // ===== COURSE OPERATIONS =====
    void insertCourse(int id, const std::string& name, const std::string& code,
                     int credits, int capacity, int teacherId);
    void updateCourse(int id, const std::string& name, int capacity);
    void deleteCourse(int id);
    std::vector<std::map<std::string, std::string>> getCourses();
    std::map<std::string, std::string> getCourse(int id);
    
    // ===== REGISTRATION OPERATIONS =====
    void insertRegistration(int studentId, int courseId);
    void deleteRegistration(int studentId, int courseId);
    std::vector<std::pair<int, int>> getRegistrations(int studentId);
    std::vector<int> getCourseStudents(int courseId);
    
    // ===== GRADE OPERATIONS =====
    void setGrade(int studentId, int courseId, double grade);
    double getGrade(int studentId, int courseId);
};

#endif // DATABASE_H
