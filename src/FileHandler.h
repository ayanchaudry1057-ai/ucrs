#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <map>
#include "Exceptions.h"

/**
 * @class FileHandler
 * @brief Handles all file I/O operations (CSV and JSON)
 */
class FileHandler {
public:
    // CSV operations for students
    static void exportStudentsToCSV(const std::string& filename,
                                   const std::vector<std::map<std::string, std::string>>& students);
    static std::vector<std::map<std::string, std::string>> importStudentsFromCSV(const std::string& filename);
    
    // CSV operations for courses
    static void exportCoursesToCSV(const std::string& filename,
                                  const std::vector<std::map<std::string, std::string>>& courses);
    static std::vector<std::map<std::string, std::string>> importCoursesFromCSV(const std::string& filename);
    
    // JSON operations for students
    static void exportStudentsToJSON(const std::string& filename,
                                    const std::vector<std::map<std::string, std::string>>& students);
    static std::vector<std::map<std::string, std::string>> importStudentsFromJSON(const std::string& filename);
    
    // JSON operations for courses
    static void exportCoursesToJSON(const std::string& filename,
                                   const std::vector<std::map<std::string, std::string>>& courses);
    static std::vector<std::map<std::string, std::string>> importCoursesFromJSON(const std::string& filename);
    
    // Utility methods
    static bool fileExists(const std::string& filename);
    static bool isValidCSV(const std::string& filename);
    static bool isValidJSON(const std::string& filename);
};

#endif // FILEHANDLER_H
