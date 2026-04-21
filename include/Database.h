#ifndef DATABASE_H
#define DATABASE_H

#include <iosfwd>
#include <string>
#include <vector>

#include "Student.h"

class Database {
public:
    struct Statistics {
        std::size_t totalStudents = 0;
        std::size_t passedStudents = 0;
        std::size_t failedStudents = 0;
        double averageMarks = 0.0;
        Student highestScorer;
        Student lowestScorer;
        bool hasData = false;
    };

    explicit Database(const std::string& filePath = "data/students.txt");

    bool loadFromFile();
    bool saveToFile() const;
    bool appendToFile(const Student& student) const;

    bool addStudent(const Student& student);
    bool addToMemory(const Student& student);
    void displayStudents(std::ostream& os) const;
    std::vector<Student> searchStudent(const std::string& query) const;
    bool calculateAverage(double& average) const;
    const Student* findTopStudent() const;
    const Student* findLowestStudent() const;
    bool calculateStatistics(double passMark, Statistics& statistics) const;

    const std::vector<Student>& getStudents() const;

private:
    static bool tryParseStudentRecord(const std::string& line, Student& student);
    static bool isDuplicateId(const std::vector<Student>& students, const std::string& id);

    std::vector<Student> students_;
    std::string filePath_;
};

#endif
