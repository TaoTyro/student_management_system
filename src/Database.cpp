#include "Database.h"

#include <fstream>

Database::Database(const std::string& filePath) : students_(), filePath_(filePath) {}

bool Database::loadFromFile() {
    std::ifstream input(filePath_);
    if (!input.is_open()) {
        return false;
    }

    return true;
}

bool Database::saveToFile() const {
    std::ofstream output(filePath_, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    return true;
}

bool Database::appendToFile(const Student& student) const {
    std::ofstream output(filePath_, std::ios::app);
    if (!output.is_open()) {
        return false;
    }

    (void)student;
    return true;
}

bool Database::addToMemory(const Student& student) {
    students_.push_back(student);
    return true;
}

const std::vector<Student>& Database::getStudents() const {
    return students_;
}