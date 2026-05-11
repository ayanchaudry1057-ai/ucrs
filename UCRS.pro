QT += core gui widgets sql

CONFIG += c++17

TARGET = UCRS
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/Person.cpp \
    src/Student.cpp \
    src/Teacher.cpp \
    src/Course.cpp \
    src/Database.cpp \
    src/FileHandler.cpp \
    src/RegistrationSystem.cpp

HEADERS += \
    src/mainwindow.h \
    src/Exceptions.h \
    src/Person.h \
    src/Student.h \
    src/Teacher.h \
    src/Course.h \
    src/Database.h \
    src/FileHandler.h \
    src/RegistrationSystem.h

# Platform-specific SQLite linking
unix {
    LIBS += -lsqlite3
}

win32 {
    LIBS += -lsqlite3
    # For Windows, you may need to link against the SQLite library
    # Uncomment below if needed:
    # LIBS += -L"C:/path/to/sqlite3/lib" -lsqlite3
    # INCLUDEPATH += C:/path/to/sqlite3/include
}

macx {
    LIBS += -lsqlite3
}
