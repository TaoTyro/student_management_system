#include "Database.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {
std::string trimCopy(const std::string& value) {
    const auto begin = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    if (begin == value.end()) {
        return "";
    }

    const auto end = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();
    return std::string(begin, end);
}

std::vector<std::string> splitByDelimiter(const std::string& value, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream stream(value);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        parts.push_back(token);
    }
    return parts;
}

bool parseIntStrict(const std::string& value, int& output) {
    try {
        std::size_t consumed = 0;
        const std::string normalized = trimCopy(value);
        const int parsed = std::stoi(normalized, &consumed);
        if (consumed != normalized.size()) {
            return false;
        }
        output = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

bool parseDoubleStrict(const std::string& value, double& output) {
    try {
        std::size_t consumed = 0;
        const std::string normalized = trimCopy(value);
        const double parsed = std::stod(normalized, &consumed);
        if (consumed != normalized.size()) {
            return false;
        }
        output = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

bool shouldAddLeadingNewLineForAppend(const std::string& filePath) {
    std::ifstream existing(filePath, std::ios::binary | std::ios::ate);
    if (!existing.is_open()) {
        return false;
    }

    const std::streampos size = existing.tellg();
    if (size <= 0) {
        return false;
    }

    existing.seekg(-1, std::ios::end);
    char lastByte = '\0';
    existing.get(lastByte);
    return lastByte != '\n';
}
} // namespace

Database::Database(const std::string& filePath) : students_(), filePath_(filePath) {}

bool Database::loadFromFile() {
    std::ifstream input(filePath_);
    if (!input.is_open()) {
        students_.clear();
        return true;
    }

    std::vector<Student> loadedStudents;
    std::string line;
    bool allRecordsValid = true;

    while (std::getline(input, line)) {
        if (trimCopy(line).empty()) {
            continue;
        }

        Student parsedStudent;
        if (!tryParseStudentRecord(line, parsedStudent)) {
            allRecordsValid = false;
            continue;
        }

        if (isDuplicateId(loadedStudents, parsedStudent.getId())) {
            allRecordsValid = false;
            continue;
        }

        loadedStudents.push_back(parsedStudent);
    }

    if (input.bad()) {
        return false;
    }

    students_ = loadedStudents;
    return allRecordsValid;
}

bool Database::saveToFile() const {
    std::ofstream output(filePath_, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    for (std::size_t index = 0; index < students_.size(); ++index) {
        output << students_[index].toFileRecord();
        if (index + 1 < students_.size()) {
            output << '\n';
        }
    }

    return output.good();
}

bool Database::appendToFile(const Student& student) const {
    if (!student.isValid()) {
        return false;
    }

    const bool shouldPrefixNewLine = shouldAddLeadingNewLineForAppend(filePath_);

    std::ofstream output(filePath_, std::ios::app);
    if (!output.is_open()) {
        return false;
    }

    if (shouldPrefixNewLine) {
        output << '\n';
    }

    output << student.toFileRecord();
    return output.good();
}

bool Database::addToMemory(const Student& student) {
    if (!student.isValid() || isDuplicateId(students_, student.getId())) {
        return false;
    }

    students_.push_back(student);
    return true;
}

const std::vector<Student>& Database::getStudents() const {
    return students_;
}

bool Database::tryParseStudentRecord(const std::string& line, Student& student) {
    const std::vector<std::string> fields = splitByDelimiter(line, '|');
    if (fields.size() != 5) {
        return false;
    }

    int age = 0;
    double marks = 0.0;
    if (!parseIntStrict(fields[2], age) || !parseDoubleStrict(fields[4], marks)) {
        return false;
    }

    try {
        student = Student(fields[0], fields[1], age, fields[3], marks);
        return true;
    } catch (...) {
        return false;
    }
}

bool Database::isDuplicateId(const std::vector<Student>& students, const std::string& id) {
    return std::any_of(students.begin(), students.end(),
                       [&id](const Student& existingStudent) { return existingStudent.getId() == id; });
}
