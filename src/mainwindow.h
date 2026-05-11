#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include "RegistrationSystem.h"
#include <memory>

/**
 * @class MainWindow
 * @brief Qt GUI Main Window for UCRS application
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Student Management Slots
    void onAddStudent();
    void onRemoveStudent();
    void onRefreshStudents();
    void onStudentSelectionChanged();
    
    // Teacher Management Slots
    void onAddTeacher();
    void onRemoveTeacher();
    void onRefreshTeachers();
    void onTeacherSelectionChanged();
    
    // Course Management Slots
    void onAddCourse();
    void onRemoveCourse();
    void onRefreshCourses();
    void onCourseSelectionChanged();
    
    // Registration Slots
    void onRegisterStudent();
    void onDropStudent();
    void onRefreshRegistrations();
    
    // Grade Management Slots
    void onSetGrade();
    void onRefreshGrades();
    
    // File Operations Slots
    void onExportStudentsCSV();
    void onImportStudentsCSV();
    void onExportCoursesCSV();
    void onImportCoursesCSV();
    void onExportStudentsJSON();
    void onExportCoursesJSON();
    
    // System Slots
    void onShowStatistics();
    void onAbout();

private:
    // UI Setup Methods
    void setupUI();
    void createMenuBar();
    void createStudentTab();
    void createTeacherTab();
    void createCourseTab();
    void createRegistrationTab();
    void createGradeTab();
    void createReportsTab();
    
    // Helper Methods
    void loadStudentsIntoTable();
    void loadTeachersIntoTable();
    void loadCoursesIntoTable();
    void loadRegistrationsIntoTable();
    void showMessage(const std::string& message, bool isError = false);
    
    // Member Variables
    std::unique_ptr<RegistrationSystem> system;
    
    // UI Components - Main
    QTabWidget *tabWidget;
    
    // Student Tab Components
    QLineEdit *studentNameInput;
    QLineEdit *studentEmailInput;
    QLineEdit *studentPhoneInput;
    QLineEdit *studentRollInput;
    QLineEdit *studentMajorInput;
    QPushButton *addStudentBtn;
    QPushButton *removeStudentBtn;
    QTableWidget *studentsTable;
    
    // Teacher Tab Components
    QLineEdit *teacherNameInput;
    QLineEdit *teacherEmailInput;
    QLineEdit *teacherPhoneInput;
    QLineEdit *teacherDeptInput;
    QLineEdit *teacherQualInput;
    QPushButton *addTeacherBtn;
    QPushButton *removeTeacherBtn;
    QTableWidget *teachersTable;
    
    // Course Tab Components
    QLineEdit *courseNameInput;
    QLineEdit *courseCodeInput;
    QSpinBox *courseCreditsInput;
    QSpinBox *courseCapacityInput;
    QComboBox *courseTeacherCombo;
    QPushButton *addCourseBtn;
    QPushButton *removeCourseBtn;
    QTableWidget *coursesTable;
    
    // Registration Tab Components
    QComboBox *regStudentCombo;
    QComboBox *regCourseCombo;
    QPushButton *registerBtn;
    QPushButton *dropBtn;
    QTableWidget *registrationTable;
    
    // Grade Tab Components
    QComboBox *gradeStudentCombo;
    QComboBox *gradeCourseCombo;
    QDoubleSpinBox *gradeInput;
    QPushButton *setGradeBtn;
    QTableWidget *gradesTable;
    
    // Status Label
    QLabel *statusLabel;
};

#endif // MAINWINDOW_H
