#include "Database.h"
#include <iostream>
#include <sstream>

Database::Database(const std::string& path)
    : db(nullptr), dbPath(path) {}

Database::~Database() {
    disconnect();
}

void Database::connect() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        throw DatabaseException("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }
}

void Database::disconnect() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

void Database::initializeDatabase() {
    if (!isConnected()) {
        throw DatabaseException("Database not connected");
    }
    
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS students (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            email TEXT,
            phone TEXT,
            roll_number TEXT UNIQUE,
            major TEXT,
            gpa REAL DEFAULT 0.0
        );
        
        CREATE TABLE IF NOT EXISTS teachers (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            email TEXT,
            phone TEXT,
            department TEXT NOT NULL,
            qualification TEXT
        );
        
        CREATE TABLE IF NOT EXISTS courses (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            code TEXT UNIQUE,
            credits INTEGER,
            capacity INTEGER,
            enrolled_count INTEGER DEFAULT 0,
            teacher_id INTEGER,
            FOREIGN KEY(teacher_id) REFERENCES teachers(id)
        );
        
        CREATE TABLE IF NOT EXISTS registrations (
            student_id INTEGER,
            course_id INTEGER,
            grade REAL,
            PRIMARY KEY(student_id, course_id),
            FOREIGN KEY(student_id) REFERENCES students(id),
            FOREIGN KEY(course_id) REFERENCES courses(id)
        );
    )";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw DatabaseException("Failed to initialize database: " + error);
    }
}

void Database::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw DatabaseException("SQL execution failed: " + error);
    }
}

// ===== STUDENT OPERATIONS =====
void Database::insertStudent(int id, const std::string& name, const std::string& email,
                            const std::string& phone, const std::string& rollNumber,
                            const std::string& major, double gpa) {
    std::stringstream ss;
    ss << "INSERT INTO students (id, name, email, phone, roll_number, major, gpa) "
       << "VALUES (" << id << ", '" << name << "', '" << email << "', '" << phone
       << "', '" << rollNumber << "', '" << major << "', " << gpa << ");";
    executeSQL(ss.str());
}

void Database::updateStudent(int id, const std::string& name, const std::string& email,
                            const std::string& phone, double gpa) {
    std::stringstream ss;
    ss << "UPDATE students SET name = '" << name << "', email = '" << email
       << "', phone = '" << phone << "', gpa = " << gpa << " WHERE id = " << id << ";";
    executeSQL(ss.str());
}

void Database::deleteStudent(int id) {
    std::stringstream ss;
    ss << "DELETE FROM students WHERE id = " << id << ";";
    executeSQL(ss.str());
}

std::vector<std::map<std::string, std::string>> Database::getStudents() {
    std::vector<std::map<std::string, std::string>> students;
    const char* sql = "SELECT * FROM students;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> student;
        student["id"] = std::to_string(sqlite3_column_int(stmt, 0));
        student["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        student["email"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        student["phone"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        student["roll_number"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        student["major"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";
        student["gpa"] = std::to_string(sqlite3_column_double(stmt, 6));
        students.push_back(student);
    }
    
    sqlite3_finalize(stmt);
    return students;
}

std::map<std::string, std::string> Database::getStudent(int id) {
    std::map<std::string, std::string> student;
    std::stringstream ss;
    ss << "SELECT * FROM students WHERE id = " << id << ";";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        student["id"] = std::to_string(sqlite3_column_int(stmt, 0));
        student["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        student["email"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        student["phone"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        student["roll_number"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        student["major"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";
        student["gpa"] = std::to_string(sqlite3_column_double(stmt, 6));
    }
    
    sqlite3_finalize(stmt);
    return student;
}

// ===== TEACHER OPERATIONS =====
void Database::insertTeacher(int id, const std::string& name, const std::string& email,
                            const std::string& phone, const std::string& department,
                            const std::string& qualification) {
    std::stringstream ss;
    ss << "INSERT INTO teachers (id, name, email, phone, department, qualification) "
       << "VALUES (" << id << ", '" << name << "', '" << email << "', '" << phone
       << "', '" << department << "', '" << qualification << "');";
    executeSQL(ss.str());
}

void Database::updateTeacher(int id, const std::string& name, const std::string& email,
                            const std::string& phone) {
    std::stringstream ss;
    ss << "UPDATE teachers SET name = '" << name << "', email = '" << email
       << "', phone = '" << phone << "' WHERE id = " << id << ";";
    executeSQL(ss.str());
}

void Database::deleteTeacher(int id) {
    std::stringstream ss;
    ss << "DELETE FROM teachers WHERE id = " << id << ";";
    executeSQL(ss.str());
}

std::vector<std::map<std::string, std::string>> Database::getTeachers() {
    std::vector<std::map<std::string, std::string>> teachers;
    const char* sql = "SELECT * FROM teachers;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> teacher;
        teacher["id"] = std::to_string(sqlite3_column_int(stmt, 0));
        teacher["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        teacher["email"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        teacher["phone"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        teacher["department"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        teacher["qualification"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) ? 
                                  reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";
        teachers.push_back(teacher);
    }
    
    sqlite3_finalize(stmt);
    return teachers;
}

std::map<std::string, std::string> Database::getTeacher(int id) {
    std::map<std::string, std::string> teacher;
    std::stringstream ss;
    ss << "SELECT * FROM teachers WHERE id = " << id << ";";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        teacher["id"] = std::to_string(sqlite3_column_int(stmt, 0));
        teacher["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        teacher["email"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        teacher["phone"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ? 
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        teacher["department"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        teacher["qualification"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) ? 
                                  reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";
    }
    
    sqlite3_finalize(stmt);
    return teacher;
}

// ===== COURSE OPERATIONS =====
void Database::insertCourse(int id, const std::string& name, const std::string& code,
                           int credits, int capacity, int teacherId) {
    std::stringstream ss;
    ss << "INSERT INTO courses (id, name, code, credits, capacity, teacher_id) "
       << "VALUES (" << id << ", '" << name << "', '" << code << "', " << credits
       << ", " << capacity << ", " << teacherId << ");";
    executeSQL(ss.str());
}

void Database::updateCourse(int id, const std::string& name, int capacity) {
    std::stringstream ss;
    ss << "UPDATE courses SET name = '" << name << "', capacity = " << capacity
       << " WHERE id = " << id << ";";
    executeSQL(ss.str());
}

void Database::deleteCourse(int id) {
    std::stringstream ss;
    ss << "DELETE FROM courses WHERE id = " << id << ";";
    executeSQL(ss.str());
}

std::vector<std::map<std::string, std::string>> Database::getCourses() {
    std::vector<std::map<std::string, std::string>> courses;
    const char* sql = "SELECT * FROM courses;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> course;
        course["id"] = std::to_string(sqlite3_column_int(stmt, 0));
        course["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        course["code"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        course["credits"] = std::to_string(sqlite3_column_int(stmt, 3));
        course["capacity"] = std::to_string(sqlite3_column_int(stmt, 4));
        course["enrolled_count"] = std::to_string(sqlite3_column_int(stmt, 5));
        course["teacher_id"] = std::to_string(sqlite3_column_int(stmt, 6));
        courses.push_back(course);
    }
    
    sqlite3_finalize(stmt);
    return courses;
}

std::map<std::string, std::string> Database::getCourse(int id) {
    std::map<std::string, std::string> course;
    std::stringstream ss;
    ss << "SELECT * FROM courses WHERE id = " << id << ";";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        course["id"] = std::to_string(sqlite3_column_int(stmt, 0));
        course["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        course["code"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        course["credits"] = std::to_string(sqlite3_column_int(stmt, 3));
        course["capacity"] = std::to_string(sqlite3_column_int(stmt, 4));
        course["enrolled_count"] = std::to_string(sqlite3_column_int(stmt, 5));
        course["teacher_id"] = std::to_string(sqlite3_column_int(stmt, 6));
    }
    
    sqlite3_finalize(stmt);
    return course;
}

// ===== REGISTRATION OPERATIONS =====
void Database::insertRegistration(int studentId, int courseId) {
    std::stringstream ss;
    ss << "INSERT INTO registrations (student_id, course_id) VALUES ("
       << studentId << ", " << courseId << ");";
    executeSQL(ss.str());
}

void Database::deleteRegistration(int studentId, int courseId) {
    std::stringstream ss;
    ss << "DELETE FROM registrations WHERE student_id = " << studentId
       << " AND course_id = " << courseId << ";";
    executeSQL(ss.str());
}

std::vector<std::pair<int, int>> Database::getRegistrations(int studentId) {
    std::vector<std::pair<int, int>> registrations;
    std::stringstream ss;
    ss << "SELECT student_id, course_id FROM registrations WHERE student_id = " << studentId << ";";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int student = sqlite3_column_int(stmt, 0);
        int course = sqlite3_column_int(stmt, 1);
        registrations.push_back({student, course});
    }
    
    sqlite3_finalize(stmt);
    return registrations;
}

std::vector<int> Database::getCourseStudents(int courseId) {
    std::vector<int> students;
    std::stringstream ss;
    ss << "SELECT DISTINCT student_id FROM registrations WHERE course_id = " << courseId << ";";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        students.push_back(sqlite3_column_int(stmt, 0));
    }
    
    sqlite3_finalize(stmt);
    return students;
}

// ===== GRADE OPERATIONS =====
void Database::setGrade(int studentId, int courseId, double grade) {
    std::stringstream ss;
    ss << "UPDATE registrations SET grade = " << grade
       << " WHERE student_id = " << studentId << " AND course_id = " << courseId << ";";
    executeSQL(ss.str());
}

double Database::getGrade(int studentId, int courseId) {
    std::stringstream ss;
    ss << "SELECT grade FROM registrations WHERE student_id = " << studentId
       << " AND course_id = " << courseId << ";";
    sqlite3_stmt* stmt;
    double grade = -1.0;
    
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement");
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        grade = sqlite3_column_double(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return grade;
}
