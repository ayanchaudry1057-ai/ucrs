#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// Base exception class for UCRS
class UCRSException : public std::exception {
protected:
    std::string message;
public:
    explicit UCRSException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    virtual ~UCRSException() = default;
};

// Database-related exceptions
class DatabaseException : public UCRSException {
public:
    explicit DatabaseException(const std::string& msg) 
        : UCRSException("[DATABASE ERROR] " + msg) {}
};

// Validation-related exceptions
class ValidationException : public UCRSException {
public:
    explicit ValidationException(const std::string& msg) 
        : UCRSException("[VALIDATION ERROR] " + msg) {}
};

// File-related exceptions
class FileException : public UCRSException {
public:
    explicit FileException(const std::string& msg) 
        : UCRSException("[FILE ERROR] " + msg) {}
};

// Registration-related exceptions
class RegistrationException : public UCRSException {
public:
    explicit RegistrationException(const std::string& msg) 
        : UCRSException("[REGISTRATION ERROR] " + msg) {}
};

// Course-related exceptions
class CourseException : public UCRSException {
public:
    explicit CourseException(const std::string& msg) 
        : UCRSException("[COURSE ERROR] " + msg) {}
};

// Student-related exceptions
class StudentException : public UCRSException {
public:
    explicit StudentException(const std::string& msg) 
        : UCRSException("[STUDENT ERROR] " + msg) {}
};

// Teacher-related exceptions
class TeacherException : public UCRSException {
public:
    explicit TeacherException(const std::string& msg) 
        : UCRSException("[TEACHER ERROR] " + msg) {}
};

#endif // EXCEPTIONS_H
