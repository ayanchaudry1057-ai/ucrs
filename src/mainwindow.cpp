#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), system(std::make_unique<RegistrationSystem>()) {
    setWindowTitle("UCRS - University Course Registration System");
    setGeometry(100, 100, 1200, 700);
    
    try {
        setupUI();
        createMenuBar();
        loadStudentsIntoTable();
        loadTeachersIntoTable();
        loadCoursesIntoTable();
        showMessage("Application initialized successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    // Create central widget and main layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create tab widget
    tabWidget = new QTabWidget(this);
    
    // Create tabs
    createStudentTab();
    createTeacherTab();
    createCourseTab();
    createRegistrationTab();
    createGradeTab();
    createReportsTab();
    
    mainLayout->addWidget(tabWidget);
    
    // Status bar
    statusLabel = new QLabel("Ready", this);
    statusBar()->addWidget(statusLabel);
    
    centralWidget->setLayout(mainLayout);
}

void MainWindow::createMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    
    QAction *exportStudCSVAction = fileMenu->addAction(tr("Export Students (CSV)"));
    connect(exportStudCSVAction, &QAction::triggered, this, &MainWindow::onExportStudentsCSV);
    
    QAction *importStudCSVAction = fileMenu->addAction(tr("Import Students (CSV)"));
    connect(importStudCSVAction, &QAction::triggered, this, &MainWindow::onImportStudentsCSV);
    
    fileMenu->addSeparator();
    
    QAction *exportCourseCSVAction = fileMenu->addAction(tr("Export Courses (CSV)"));
    connect(exportCourseCSVAction, &QAction::triggered, this, &MainWindow::onExportCoursesCSV);
    
    QAction *importCourseCSVAction = fileMenu->addAction(tr("Import Courses (CSV)"));
    connect(importCourseCSVAction, &QAction::triggered, this, &MainWindow::onImportCoursesCSV);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::createStudentTab() {
    QWidget *studentTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(studentTab);
    
    // Input Group
    QGroupBox *inputGroup = new QGroupBox("Add Student", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);
    
    // Input fields
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Name:"));
    studentNameInput = new QLineEdit();
    row1->addWidget(studentNameInput);
    row1->addWidget(new QLabel("Email:"));
    studentEmailInput = new QLineEdit();
    row1->addWidget(studentEmailInput);
    inputLayout->addLayout(row1);
    
    QHBoxLayout *row2 = new QHBoxLayout();
    row2->addWidget(new QLabel("Phone:"));
    studentPhoneInput = new QLineEdit();
    row2->addWidget(studentPhoneInput);
    row2->addWidget(new QLabel("Roll #:"));
    studentRollInput = new QLineEdit();
    row2->addWidget(studentRollInput);
    inputLayout->addLayout(row2);
    
    QHBoxLayout *row3 = new QHBoxLayout();
    row3->addWidget(new QLabel("Major:"));
    studentMajorInput = new QLineEdit();
    row3->addWidget(studentMajorInput);
    row3->addStretch();
    inputLayout->addLayout(row3);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addStudentBtn = new QPushButton("Add Student");
    removeStudentBtn = new QPushButton("Remove Selected");
    buttonLayout->addWidget(addStudentBtn);
    buttonLayout->addWidget(removeStudentBtn);
    buttonLayout->addStretch();
    inputLayout->addLayout(buttonLayout);
    
    layout->addWidget(inputGroup);
    
    // Table
    studentsTable = new QTableWidget();
    studentsTable->setColumnCount(7);
    studentsTable->setHorizontalHeaderLabels({"ID", "Name", "Email", "Phone", "Roll #", "Major", "GPA"});
    studentsTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(studentsTable);
    
    // Connect signals
    connect(addStudentBtn, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(removeStudentBtn, &QPushButton::clicked, this, &MainWindow::onRemoveStudent);
    connect(studentsTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onStudentSelectionChanged);
    
    tabWidget->addTab(studentTab, "Students");
}

void MainWindow::createTeacherTab() {
    QWidget *teacherTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(teacherTab);
    
    // Input Group
    QGroupBox *inputGroup = new QGroupBox("Add Teacher", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);
    
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Name:"));
    teacherNameInput = new QLineEdit();
    row1->addWidget(teacherNameInput);
    row1->addWidget(new QLabel("Email:"));
    teacherEmailInput = new QLineEdit();
    row1->addWidget(teacherEmailInput);
    inputLayout->addLayout(row1);
    
    QHBoxLayout *row2 = new QHBoxLayout();
    row2->addWidget(new QLabel("Phone:"));
    teacherPhoneInput = new QLineEdit();
    row2->addWidget(teacherPhoneInput);
    row2->addWidget(new QLabel("Department:"));
    teacherDeptInput = new QLineEdit();
    row2->addWidget(teacherDeptInput);
    inputLayout->addLayout(row2);
    
    QHBoxLayout *row3 = new QHBoxLayout();
    row3->addWidget(new QLabel("Qualification:"));
    teacherQualInput = new QLineEdit();
    row3->addWidget(teacherQualInput);
    row3->addStretch();
    inputLayout->addLayout(row3);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addTeacherBtn = new QPushButton("Add Teacher");
    removeTeacherBtn = new QPushButton("Remove Selected");
    buttonLayout->addWidget(addTeacherBtn);
    buttonLayout->addWidget(removeTeacherBtn);
    buttonLayout->addStretch();
    inputLayout->addLayout(buttonLayout);
    
    layout->addWidget(inputGroup);
    
    // Table
    teachersTable = new QTableWidget();
    teachersTable->setColumnCount(6);
    teachersTable->setHorizontalHeaderLabels({"ID", "Name", "Email", "Phone", "Department", "Qualification"});
    teachersTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(teachersTable);
    
    // Connect signals
    connect(addTeacherBtn, &QPushButton::clicked, this, &MainWindow::onAddTeacher);
    connect(removeTeacherBtn, &QPushButton::clicked, this, &MainWindow::onRemoveTeacher);
    connect(teachersTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTeacherSelectionChanged);
    
    tabWidget->addTab(teacherTab, "Teachers");
}

void MainWindow::createCourseTab() {
    QWidget *courseTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(courseTab);
    
    // Input Group
    QGroupBox *inputGroup = new QGroupBox("Add Course", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);
    
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Name:"));
    courseNameInput = new QLineEdit();
    row1->addWidget(courseNameInput);
    row1->addWidget(new QLabel("Code:"));
    courseCodeInput = new QLineEdit();
    row1->addWidget(courseCodeInput);
    inputLayout->addLayout(row1);
    
    QHBoxLayout *row2 = new QHBoxLayout();
    row2->addWidget(new QLabel("Credits:"));
    courseCreditsInput = new QSpinBox();
    courseCreditsInput->setMaximum(10);
    courseCreditsInput->setValue(3);
    row2->addWidget(courseCreditsInput);
    row2->addWidget(new QLabel("Capacity:"));
    courseCapacityInput = new QSpinBox();
    courseCapacityInput->setMaximum(200);
    courseCapacityInput->setValue(50);
    row2->addWidget(courseCapacityInput);
    row2->addWidget(new QLabel("Teacher:"));
    courseTeacherCombo = new QComboBox();
    row2->addWidget(courseTeacherCombo);
    inputLayout->addLayout(row2);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addCourseBtn = new QPushButton("Add Course");
    removeCourseBtn = new QPushButton("Remove Selected");
    buttonLayout->addWidget(addCourseBtn);
    buttonLayout->addWidget(removeCourseBtn);
    buttonLayout->addStretch();
    inputLayout->addLayout(buttonLayout);
    
    layout->addWidget(inputGroup);
    
    // Table
    coursesTable = new QTableWidget();
    coursesTable->setColumnCount(7);
    coursesTable->setHorizontalHeaderLabels({"ID", "Name", "Code", "Credits", "Capacity", "Enrolled", "Teacher"});
    coursesTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(coursesTable);
    
    // Connect signals
    connect(addCourseBtn, &QPushButton::clicked, this, &MainWindow::onAddCourse);
    connect(removeCourseBtn, &QPushButton::clicked, this, &MainWindow::onRemoveCourse);
    connect(coursesTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onCourseSelectionChanged);
    
    tabWidget->addTab(courseTab, "Courses");
}

void MainWindow::createRegistrationTab() {
    QWidget *regTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(regTab);
    
    // Input Group
    QGroupBox *inputGroup = new QGroupBox("Course Registration", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);
    
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Student:"));
    regStudentCombo = new QComboBox();
    row1->addWidget(regStudentCombo);
    row1->addWidget(new QLabel("Course:"));
    regCourseCombo = new QComboBox();
    row1->addWidget(regCourseCombo);
    inputLayout->addLayout(row1);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    registerBtn = new QPushButton("Register");
    dropBtn = new QPushButton("Drop Course");
    buttonLayout->addWidget(registerBtn);
    buttonLayout->addWidget(dropBtn);
    buttonLayout->addStretch();
    inputLayout->addLayout(buttonLayout);
    
    layout->addWidget(inputGroup);
    
    // Table
    registrationTable = new QTableWidget();
    registrationTable->setColumnCount(3);
    registrationTable->setHorizontalHeaderLabels({"Student", "Course", "Status"});
    registrationTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(registrationTable);
    
    // Connect signals
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterStudent);
    connect(dropBtn, &QPushButton::clicked, this, &MainWindow::onDropStudent);
    
    tabWidget->addTab(regTab, "Registration");
}

void MainWindow::createGradeTab() {
    QWidget *gradeTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(gradeTab);
    
    // Input Group
    QGroupBox *inputGroup = new QGroupBox("Set Grades", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);
    
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Student:"));
    gradeStudentCombo = new QComboBox();
    row1->addWidget(gradeStudentCombo);
    row1->addWidget(new QLabel("Course:"));
    gradeCourseCombo = new QComboBox();
    row1->addWidget(gradeCourseCombo);
    row1->addWidget(new QLabel("Grade (0-100):"));
    gradeInput = new QDoubleSpinBox();
    gradeInput->setMaximum(100.0);
    gradeInput->setDecimals(2);
    row1->addWidget(gradeInput);
    inputLayout->addLayout(row1);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    setGradeBtn = new QPushButton("Set Grade");
    buttonLayout->addWidget(setGradeBtn);
    buttonLayout->addStretch();
    inputLayout->addLayout(buttonLayout);
    
    layout->addWidget(inputGroup);
    
    // Table
    gradesTable = new QTableWidget();
    gradesTable->setColumnCount(4);
    gradesTable->setHorizontalHeaderLabels({"Student", "Course", "Grade", "GPA"});
    gradesTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(gradesTable);
    
    // Connect signals
    connect(setGradeBtn, &QPushButton::clicked, this, &MainWindow::onSetGrade);
    
    tabWidget->addTab(gradeTab, "Grades");
}

void MainWindow::createReportsTab() {
    QWidget *reportsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(reportsTab);
    
    QGroupBox *statsGroup = new QGroupBox("System Statistics", this);
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);
    
    QPushButton *statsBtn = new QPushButton("Show Statistics");
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::onShowStatistics);
    statsLayout->addWidget(statsBtn);
    
    layout->addWidget(statsGroup);
    layout->addStretch();
    
    tabWidget->addTab(reportsTab, "Reports");
}

void MainWindow::loadStudentsIntoTable() {
    studentsTable->setRowCount(0);
    try {
        auto students = system->getAllStudents();
        for (const auto& student : students) {
            int row = studentsTable->rowCount();
            studentsTable->insertRow(row);
            studentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(student->getId())));
            studentsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student->getName())));
            studentsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student->getEmail())));
            studentsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(student->getPhone())));
            studentsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(student->getRollNumber())));
            studentsTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(student->getMajor())));
            studentsTable->setItem(row, 6, new QTableWidgetItem(QString::number(student->getGPA(), 'f', 2)));
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::loadTeachersIntoTable() {
    teachersTable->setRowCount(0);
    try {
        auto teachers = system->getAllTeachers();
        for (const auto& teacher : teachers) {
            int row = teachersTable->rowCount();
            teachersTable->insertRow(row);
            teachersTable->setItem(row, 0, new QTableWidgetItem(QString::number(teacher->getId())));
            teachersTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(teacher->getName())));
            teachersTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(teacher->getEmail())));
            teachersTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(teacher->getPhone())));
            teachersTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(teacher->getDepartment())));
            teachersTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(teacher->getQualification())));
        }
        
        // Update teacher combo boxes
        courseTeacherCombo->clear();
        for (const auto& teacher : teachers) {
            courseTeacherCombo->addItem(
                QString::fromStdString(teacher->getName()),
                teacher->getId()
            );
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::loadCoursesIntoTable() {
    coursesTable->setRowCount(0);
    try {
        auto courses = system->getAllCourses();
        for (const auto& course : courses) {
            int row = coursesTable->rowCount();
            coursesTable->insertRow(row);
            coursesTable->setItem(row, 0, new QTableWidgetItem(QString::number(course->getCourseId())));
            coursesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(course->getName())));
            coursesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(course->getCode())));
            coursesTable->setItem(row, 3, new QTableWidgetItem(QString::number(course->getCredits())));
            coursesTable->setItem(row, 4, new QTableWidgetItem(QString::number(course->getCapacity())));
            coursesTable->setItem(row, 5, new QTableWidgetItem(QString::number(course->getEnrolledCount())));
            try {
                auto teacher = system->getTeacher(course->getTeacherId());
                coursesTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(teacher->getName())));
            } catch (...) {
                coursesTable->setItem(row, 6, new QTableWidgetItem("N/A"));
            }
        }
        
        // Update course combo boxes
        regCourseCombo->clear();
        gradeCourseCombo->clear();
        for (const auto& course : courses) {
            regCourseCombo->addItem(
                QString::fromStdString(course->getName()),
                course->getCourseId()
            );
            gradeCourseCombo->addItem(
                QString::fromStdString(course->getName()),
                course->getCourseId()
            );
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::loadRegistrationsIntoTable() {
    registrationTable->setRowCount(0);
    try {
        auto students = system->getAllStudents();
        for (const auto& student : students) {
            auto courses = system->getStudentCourses(student->getId());
            for (const auto& course : courses) {
                int row = registrationTable->rowCount();
                registrationTable->insertRow(row);
                registrationTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(student->getName())));
                registrationTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(course->getCode())));
                registrationTable->setItem(row, 2, new QTableWidgetItem("Enrolled"));
            }
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::showMessage(const std::string& message, bool isError) {
    statusLabel->setText(QString::fromStdString(message));
    if (isError) {
        QMessageBox::critical(this, "Error", QString::fromStdString(message));
    } else {
        QMessageBox::information(this, "Success", QString::fromStdString(message));
    }
}

// Slot Implementations
void MainWindow::onAddStudent() {
    try {
        if (studentNameInput->text().isEmpty() || studentRollInput->text().isEmpty()) {
            showMessage("Please fill all required fields", true);
            return;
        }
        system->addStudent(
            studentNameInput->text().toStdString(),
            studentEmailInput->text().toStdString(),
            studentPhoneInput->text().toStdString(),
            studentRollInput->text().toStdString(),
            studentMajorInput->text().toStdString()
        );
        studentNameInput->clear();
        studentEmailInput->clear();
        studentPhoneInput->clear();
        studentRollInput->clear();
        studentMajorInput->clear();
        loadStudentsIntoTable();
        showMessage("Student added successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRemoveStudent() {
    try {
        int row = studentsTable->currentRow();
        if (row < 0) {
            showMessage("Please select a student", true);
            return;
        }
        int studentId = studentsTable->item(row, 0)->text().toInt();
        system->removeStudent(studentId);
        loadStudentsIntoTable();
        showMessage("Student removed successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRefreshStudents() {
    loadStudentsIntoTable();
}

void MainWindow::onStudentSelectionChanged() {}

void MainWindow::onAddTeacher() {
    try {
        if (teacherNameInput->text().isEmpty() || teacherDeptInput->text().isEmpty()) {
            showMessage("Please fill all required fields", true);
            return;
        }
        system->addTeacher(
            teacherNameInput->text().toStdString(),
            teacherEmailInput->text().toStdString(),
            teacherPhoneInput->text().toStdString(),
            teacherDeptInput->text().toStdString(),
            teacherQualInput->text().toStdString()
        );
        teacherNameInput->clear();
        teacherEmailInput->clear();
        teacherPhoneInput->clear();
        teacherDeptInput->clear();
        teacherQualInput->clear();
        loadTeachersIntoTable();
        showMessage("Teacher added successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRemoveTeacher() {
    try {
        int row = teachersTable->currentRow();
        if (row < 0) {
            showMessage("Please select a teacher", true);
            return;
        }
        int teacherId = teachersTable->item(row, 0)->text().toInt();
        system->removeTeacher(teacherId);
        loadTeachersIntoTable();
        showMessage("Teacher removed successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRefreshTeachers() {
    loadTeachersIntoTable();
}

void MainWindow::onTeacherSelectionChanged() {}

void MainWindow::onAddCourse() {
    try {
        if (courseNameInput->text().isEmpty() || courseCodeInput->text().isEmpty()) {
            showMessage("Please fill all required fields", true);
            return;
        }
        int teacherId = courseTeacherCombo->currentData().toInt();
        system->addCourse(
            courseNameInput->text().toStdString(),
            courseCodeInput->text().toStdString(),
            courseCreditsInput->value(),
            courseCapacityInput->value(),
            teacherId
        );
        courseNameInput->clear();
        courseCodeInput->clear();
        loadCoursesIntoTable();
        showMessage("Course added successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRemoveCourse() {
    try {
        int row = coursesTable->currentRow();
        if (row < 0) {
            showMessage("Please select a course", true);
            return;
        }
        int courseId = coursesTable->item(row, 0)->text().toInt();
        system->removeCourse(courseId);
        loadCoursesIntoTable();
        showMessage("Course removed successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRefreshCourses() {
    loadCoursesIntoTable();
}

void MainWindow::onCourseSelectionChanged() {}

void MainWindow::onRegisterStudent() {
    try {
        if (regStudentCombo->currentIndex() < 0 || regCourseCombo->currentIndex() < 0) {
            showMessage("Please select student and course", true);
            return;
        }
        int studentId = regStudentCombo->currentData().toInt();
        int courseId = regCourseCombo->currentData().toInt();
        system->registerStudentForCourse(studentId, courseId);
        loadRegistrationsIntoTable();
        loadCoursesIntoTable();
        showMessage("Student registered successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onDropStudent() {
    try {
        int row = registrationTable->currentRow();
        if (row < 0) {
            showMessage("Please select a registration", true);
            return;
        }
        // For simplicity, we'll show a message
        showMessage("Drop functionality requires student-course pair selection");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRefreshRegistrations() {
    loadRegistrationsIntoTable();
}

void MainWindow::onSetGrade() {
    try {
        if (gradeStudentCombo->currentIndex() < 0 || gradeCourseCombo->currentIndex() < 0) {
            showMessage("Please select student and course", true);
            return;
        }
        int studentId = gradeStudentCombo->currentData().toInt();
        int courseId = gradeCourseCombo->currentData().toInt();
        double grade = gradeInput->value();
        system->setGrade(studentId, courseId, grade);
        loadStudentsIntoTable();
        showMessage("Grade set successfully");
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onRefreshGrades() {
    loadStudentsIntoTable();
}

void MainWindow::onExportStudentsCSV() {
    try {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Students", "", "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            system->exportStudentsToCSV(fileName.toStdString());
            showMessage("Students exported successfully");
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onImportStudentsCSV() {
    try {
        QString fileName = QFileDialog::getOpenFileName(this, "Import Students", "", "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            system->importStudentsFromCSV(fileName.toStdString());
            loadStudentsIntoTable();
            showMessage("Students imported successfully");
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onExportCoursesCSV() {
    try {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Courses", "", "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            system->exportCoursesToCSV(fileName.toStdString());
            showMessage("Courses exported successfully");
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onImportCoursesCSV() {
    try {
        QString fileName = QFileDialog::getOpenFileName(this, "Import Courses", "", "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            system->importCoursesFromCSV(fileName.toStdString());
            loadCoursesIntoTable();
            showMessage("Courses imported successfully");
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onExportStudentsJSON() {
    try {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Students", "", "JSON Files (*.json)");
        if (!fileName.isEmpty()) {
            system->exportStudentsToJSON(fileName.toStdString());
            showMessage("Students exported to JSON successfully");
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onExportCoursesJSON() {
    try {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Courses", "", "JSON Files (*.json)");
        if (!fileName.isEmpty()) {
            system->exportCoursesToJSON(fileName.toStdString());
            showMessage("Courses exported to JSON successfully");
        }
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onShowStatistics() {
    try {
        system->displaySystemStatistics();
        QString stats = QString("\nSystem Statistics:\n"
                              "Students: %1\n"
                              "Teachers: %2\n"
                              "Courses: %3")
                        .arg(system->getStudentCount())
                        .arg(system->getTeacherCount())
                        .arg(system->getCourseCount());
        showMessage(stats.toStdString());
    } catch (const std::exception& e) {
        showMessage(std::string(e.what()), true);
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About UCRS",
        "UCRS - University Course Registration System\n\n"
        "A comprehensive C++ application with:\n"
        "- Qt GUI Framework\n"
        "- SQLite Database\n"
        "- Exception Handling\n"
        "- STL Containers\n"
        "- CSV/JSON File I/O\n\n"
        "Version 1.0\n"
        "© 2026 All Rights Reserved");
}
