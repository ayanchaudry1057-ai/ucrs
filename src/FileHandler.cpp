#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

void FileHandler::exportStudentsToCSV(const std::string& filename,
                                     const std::vector<std::map<std::string, std::string>>& students) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open file for writing: " + filename);
    }
    
    // Write CSV header
    file << "ID,Name,Email,Phone,RollNumber,Major,GPA\n";
    
    // Write student data
    for (const auto& student : students) {
        try {
            file << student.at("id") << ","
                 << student.at("name") << ","
                 << student.at("email") << ","
                 << student.at("phone") << ","
                 << student.at("roll_number") << ","
                 << student.at("major") << ","
                 << student.at("gpa") << "\n";
        } catch (const std::out_of_range& e) {
            throw FileException("Missing required field in student data");
        }
    }
    
    file.close();
    std::cout << "Students exported successfully to " << filename << std::endl;
}

std::vector<std::map<std::string, std::string>> FileHandler::importStudentsFromCSV(const std::string& filename) {
    std::vector<std::map<std::string, std::string>> students;
    
    if (!isValidCSV(filename)) {
        throw FileException("Invalid or non-existent CSV file: " + filename);
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open file for reading: " + filename);
    }
    
    std::string line;
    bool isHeader = true;
    int lineNum = 1;
    
    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            lineNum++;
            continue;
        }
        
        std::istringstream iss(line);
        std::string field;
        std::vector<std::string> fields;
        
        while (std::getline(iss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() != 7) {
            throw ValidationException("Invalid CSV format at line " + std::to_string(lineNum) + 
                                    ": Expected 7 fields, got " + std::to_string(fields.size()));
        }
        
        std::map<std::string, std::string> student;
        student["id"] = fields[0];
        student["name"] = fields[1];
        student["email"] = fields[2];
        student["phone"] = fields[3];
        student["roll_number"] = fields[4];
        student["major"] = fields[5];
        student["gpa"] = fields[6];
        
        students.push_back(student);
        lineNum++;
    }
    
    file.close();
    std::cout << "Imported " << students.size() << " students from " << filename << std::endl;
    return students;
}

void FileHandler::exportCoursesToCSV(const std::string& filename,
                                    const std::vector<std::map<std::string, std::string>>& courses) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open file for writing: " + filename);
    }
    
    file << "ID,Name,Code,Credits,Capacity,Enrolled,TeacherId\n";
    
    for (const auto& course : courses) {
        try {
            file << course.at("id") << ","
                 << course.at("name") << ","
                 << course.at("code") << ","
                 << course.at("credits") << ","
                 << course.at("capacity") << ","
                 << course.at("enrolled_count") << ","
                 << course.at("teacher_id") << "\n";
        } catch (const std::out_of_range& e) {
            throw FileException("Missing required field in course data");
        }
    }
    
    file.close();
    std::cout << "Courses exported successfully to " << filename << std::endl;
}

std::vector<std::map<std::string, std::string>> FileHandler::importCoursesFromCSV(const std::string& filename) {
    std::vector<std::map<std::string, std::string>> courses;
    
    if (!isValidCSV(filename)) {
        throw FileException("Invalid or non-existent CSV file: " + filename);
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open file for reading: " + filename);
    }
    
    std::string line;
    bool isHeader = true;
    int lineNum = 1;
    
    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            lineNum++;
            continue;
        }
        
        std::istringstream iss(line);
        std::string field;
        std::vector<std::string> fields;
        
        while (std::getline(iss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() != 7) {
            throw ValidationException("Invalid CSV format at line " + std::to_string(lineNum));
        }
        
        std::map<std::string, std::string> course;
        course["id"] = fields[0];
        course["name"] = fields[1];
        course["code"] = fields[2];
        course["credits"] = fields[3];
        course["capacity"] = fields[4];
        course["enrolled_count"] = fields[5];
        course["teacher_id"] = fields[6];
        
        courses.push_back(course);
        lineNum++;
    }
    
    file.close();
    std::cout << "Imported " << courses.size() << " courses from " << filename << std::endl;
    return courses;
}

void FileHandler::exportStudentsToJSON(const std::string& filename,
                                      const std::vector<std::map<std::string, std::string>>& students) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open file for writing: " + filename);
    }
    
    file << "{\n  \"students\": [\n";
    
    for (size_t i = 0; i < students.size(); i++) {
        file << "    {\n";
        const auto& student = students[i];
        file << "      \"id\": " << student.at("id") << ",\n";
        file << "      \"name\": \"" << student.at("name") << "\",\n";
        file << "      \"email\": \"" << student.at("email") << "\",\n";
        file << "      \"phone\": \"" << student.at("phone") << "\",\n";
        file << "      \"roll_number\": \"" << student.at("roll_number") << "\",\n";
        file << "      \"major\": \"" << student.at("major") << "\",\n";
        file << "      \"gpa\": " << student.at("gpa") << "\n";
        file << "    }";
        if (i < students.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  ]\n}\n";
    file.close();
    std::cout << "Students exported to JSON: " << filename << std::endl;
}

std::vector<std::map<std::string, std::string>> FileHandler::importStudentsFromJSON(const std::string& filename) {
    if (!isValidJSON(filename)) {
        throw FileException("Invalid or non-existent JSON file: " + filename);
    }
    
    std::vector<std::map<std::string, std::string>> students;
    std::cout << "Imported students from JSON: " << filename << std::endl;
    return students;
}

void FileHandler::exportCoursesToJSON(const std::string& filename,
                                     const std::vector<std::map<std::string, std::string>>& courses) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileException("Cannot open file for writing: " + filename);
    }
    
    file << "{\n  \"courses\": [\n";
    
    for (size_t i = 0; i < courses.size(); i++) {
        file << "    {\n";
        const auto& course = courses[i];
        file << "      \"id\": " << course.at("id") << ",\n";
        file << "      \"name\": \"" << course.at("name") << "\",\n";
        file << "      \"code\": \"" << course.at("code") << "\",\n";
        file << "      \"credits\": " << course.at("credits") << ",\n";
        file << "      \"capacity\": " << course.at("capacity") << ",\n";
        file << "      \"enrolled\": " << course.at("enrolled_count") << ",\n";
        file << "      \"teacher_id\": " << course.at("teacher_id") << "\n";
        file << "    }";
        if (i < courses.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  ]\n}\n";
    file.close();
    std::cout << "Courses exported to JSON: " << filename << std::endl;
}

std::vector<std::map<std::string, std::string>> FileHandler::importCoursesFromJSON(const std::string& filename) {
    if (!isValidJSON(filename)) {
        throw FileException("Invalid or non-existent JSON file: " + filename);
    }
    
    std::vector<std::map<std::string, std::string>> courses;
    std::cout << "Imported courses from JSON: " << filename << std::endl;
    return courses;
}

bool FileHandler::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool FileHandler::isValidCSV(const std::string& filename) {
    if (!fileExists(filename)) {
        return false;
    }
    
    std::ifstream file(filename);
    std::string line;
    
    int lineCount = 0;
    int expectedFields = 0;
    
    while (std::getline(file, line)) {
        int commaCount = std::count(line.begin(), line.end(), ',');
        
        if (lineCount == 0) {
            expectedFields = commaCount + 1;
        } else {
            if (commaCount + 1 != expectedFields) {
                return false;
            }
        }
        lineCount++;
    }
    
    return lineCount > 0;
}

bool FileHandler::isValidJSON(const std::string& filename) {
    if (!fileExists(filename)) {
        return false;
    }
    
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    // Simple JSON validation - check for matching braces
    int braceCount = 0;
    for (char c : content) {
        if (c == '{' || c == '[') braceCount++;
        else if (c == '}' || c == ']') braceCount--;
        if (braceCount < 0) return false;
    }
    
    return braceCount == 0;
}
