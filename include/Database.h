#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

#include "Student.h"

class Database {
public:
    explicit Database(const std::string& filePath = "data/students.txt");

    bool loadFromFile();
    bool saveToFile() const;
    bool appendToFile(const Student& student) const;

    bool addToMemory(const Student& student);
    const std::vector<Student>& getStudents() const;

private:
    static bool tryParseStudentRecord(const std::string& line, Student& student);
    static bool isDuplicateId(const std::vector<Student>& students, const std::string& id);

    std::vector<Student> students_;
    std::string filePath_;
};

#endif
