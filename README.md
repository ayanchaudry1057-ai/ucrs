# UCRS - University Course Registration System

A comprehensive C++ application for managing university course registrations with Qt GUI, SQLite database, and advanced OOP principles.

## Features

- **Object-Oriented Design**: Person → Student/Teacher inheritance hierarchy
- **Exception Handling**: Custom exception classes for robust error management
- **STL Containers**: Efficient use of vectors, maps, and sets
- **SQLite Database**: Persistent storage for all data
- **Qt GUI**: User-friendly graphical interface
- **File I/O**: Import/export data in CSV and JSON formats
- **Data Validation**: Comprehensive input validation
- **Course Management**: Add, remove, update courses
- **Student Registration**: Register students for courses with capacity management
- **Grade Management**: Track and manage student grades

## Project Structure

```
UCRS/
├── src/
│   ├── Exceptions.h
│   ├── Person.h / Person.cpp
│   ├── Student.h / Student.cpp
│   ├── Teacher.h / Teacher.cpp
│   ├── Course.h / Course.cpp
│   ├── Database.h / Database.cpp
│   ├── FileHandler.h / FileHandler.cpp
│   ├── RegistrationSystem.h / RegistrationSystem.cpp
│   ├── mainwindow.h / mainwindow.cpp
│   └── main.cpp
├── resources/
│   └── resources.qrc
├── UCRS.pro
├── CMakeLists.txt
└── README.md
```

## Dependencies

- Qt 5.15 or higher
- SQLite 3
- C++17 or higher
- GCC/Clang compiler

## Build Instructions

### Using Qt Creator
1. Open `UCRS.pro` in Qt Creator
2. Configure the project with your Qt kit
3. Build and run (Ctrl+R)

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
./UCRS
```

### Using qmake directly
```bash
qmake UCRS.pro
make
./UCRS
```

## Features Implemented

### ✅ Exception Handling
- Custom exception hierarchy
- DatabaseException
- ValidationException
- FileException
- RegistrationException
- CourseException
- StudentException
- Try-catch blocks throughout the application

### ✅ STL Containers
- `std::vector<T>` for dynamic arrays
- `std::map<K,V>` for key-value associations
- `std::set<T>` for unique collections
- `std::shared_ptr<T>` for smart memory management
- Efficient algorithms and iterators

### ✅ File Handling
- CSV export/import for students and courses
- JSON export/import for data portability
- File validation and error checking
- Automatic file creation

### ✅ Database (SQLite)
- 4 main tables: students, teachers, courses, registrations
- CRUD operations for all entities
- Persistent data storage
- Foreign key constraints
- Grade tracking

### ✅ Qt GUI
- Main window with tabbed interface
- Student management tab
- Course management tab
- Registration/Enrollment tab
- Grade entry tab
- Reports and statistics tab
- File import/export dialogs
- Input validation dialogs
- Data display tables

### ✅ Object-Oriented Design
- Person base class with virtual methods
- Student class inheriting from Person
- Teacher class inheriting from Person
- Course class for course management
- RegistrationSystem orchestrating all operations
- Clear separation of concerns

## Usage Examples

### Adding a Student
```cpp
RegistrationSystem system;
auto student = system.addStudent(
    "Ali Ahmed", 
    "ali@student.edu", 
    "111-111-1111",
    "CS001", 
    "Computer Science"
);
```

### Adding a Course
```cpp
auto course = system.addCourse(
    "Data Structures", 
    "CS201", 
    3,      // credits
    50,     // capacity
    teacherId
);
```

### Registering a Student for a Course
```cpp
system.registerStudentForCourse(studentId, courseId);
```

### Setting Grades
```cpp
system.setGrade(studentId, courseId, 85.5);
```

### Exporting Data
```cpp
system.exportStudentsToCSV("students.csv");
system.exportCoursesToCSV("courses.csv");
```

## Database Schema

### students table
```sql
CREATE TABLE students (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    email TEXT,
    phone TEXT,
    roll_number TEXT UNIQUE,
    major TEXT,
    gpa REAL DEFAULT 0.0
);
```

### teachers table
```sql
CREATE TABLE teachers (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    email TEXT,
    phone TEXT,
    department TEXT NOT NULL,
    qualification TEXT
);
```

### courses table
```sql
CREATE TABLE courses (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    code TEXT UNIQUE,
    credits INTEGER,
    capacity INTEGER,
    enrolled_count INTEGER DEFAULT 0,
    teacher_id INTEGER,
    FOREIGN KEY(teacher_id) REFERENCES teachers(id)
);
```

### registrations table
```sql
CREATE TABLE registrations (
    student_id INTEGER,
    course_id INTEGER,
    grade REAL,
    PRIMARY KEY(student_id, course_id),
    FOREIGN KEY(student_id) REFERENCES students(id),
    FOREIGN KEY(course_id) REFERENCES courses(id)
);
```

## License

MIT License - Feel free to use, modify, and distribute

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
