#include "Database.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <limits>
#include <numeric>
#include <ostream>
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

std::string toLowerCopy(const std::string& value) {
    std::string lowered;
    lowered.reserve(value.size());
    for (unsigned char ch : value) {
        lowered.push_back(static_cast<char>(std::tolower(ch)));
    }
    return lowered;
}

std::string normalizeIdForComparison(const std::string& value) {
    return toLowerCopy(trimCopy(value));
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

bool Database::addStudent(const Student& student) {
    if (!addToMemory(student)) {
        return false;
    }

    if (!appendToFile(student)) {
        students_.pop_back();
        return false;
    }

    return true;
}

bool Database::addToMemory(const Student& student) {
    if (!student.isValid() || isDuplicateId(students_, student.getId())) {
        return false;
    }

    students_.push_back(student);
    return true;
}

bool Database::hasStudentId(const std::string& id) const {
    const std::string normalizedId = normalizeIdForComparison(id);
    if (normalizedId.empty()) {
        return false;
    }

    return std::any_of(students_.begin(), students_.end(), [&normalizedId](const Student& student) {
        return normalizeIdForComparison(student.getId()) == normalizedId;
    });
}

void Database::displayStudents(std::ostream& os) const {
    if (students_.empty()) {
        os << "No student records found.\n";
        return;
    }

    os << std::left << std::setw(12) << "ID" << std::setw(26) << "Name" << std::setw(8) << "Age" << std::setw(24)
       << "Course" << std::setw(10) << "Marks" << '\n';
    os << std::string(80, '-') << '\n';

    const std::streamsize oldPrecision = os.precision();
    const std::ios::fmtflags oldFlags = os.flags();

    os << std::fixed << std::setprecision(2);
    for (const Student& student : students_) {
        os << std::left << std::setw(12) << student.getId() << std::setw(26) << student.getName() << std::setw(8)
           << student.getAge() << std::setw(24) << student.getCourse() << std::setw(10) << student.getMarks()
           << '\n';
    }

    os.flags(oldFlags);
    os.precision(oldPrecision);
}

std::vector<Student> Database::searchStudent(const std::string& query) const {
    std::vector<Student> matches;
    const std::string normalizedQuery = trimCopy(query);
    if (normalizedQuery.empty()) {
        return matches;
    }

    const std::string lowerQuery = toLowerCopy(normalizedQuery);
    for (const Student& student : students_) {
        const bool idMatch = normalizeIdForComparison(student.getId()) == normalizeIdForComparison(normalizedQuery);
        const bool nameMatch = toLowerCopy(student.getName()).find(lowerQuery) != std::string::npos;

        if (idMatch || nameMatch) {
            matches.push_back(student);
        }
    }

    return matches;
}

bool Database::calculateAverage(double& average) const {
    if (students_.empty()) {
        average = 0.0;
        return false;
    }

    const double totalMarks =
        std::accumulate(students_.begin(), students_.end(), 0.0,
                        [](double runningTotal, const Student& student) { return runningTotal + student.getMarks(); });
    average = totalMarks / static_cast<double>(students_.size());
    return true;
}

const Student* Database::findTopStudent() const {
    if (students_.empty()) {
        return nullptr;
    }

    const auto topIterator = std::max_element(
        students_.begin(), students_.end(),
        [](const Student& left, const Student& right) { return left.getMarks() < right.getMarks(); });

    return &(*topIterator);
}

const Student* Database::findLowestStudent() const {
    if (students_.empty()) {
        return nullptr;
    }

    const auto lowestIterator = std::min_element(
        students_.begin(), students_.end(),
        [](const Student& left, const Student& right) { return left.getMarks() < right.getMarks(); });

    return &(*lowestIterator);
}

bool Database::calculateStatistics(double passMark, Statistics& statistics) const {
    if (passMark < 0.0 || passMark > 100.0) {
        return false;
    }

    statistics = Statistics{};
    statistics.totalStudents = students_.size();
    if (students_.empty()) {
        return true;
    }

    const Student* highest = &students_.front();
    const Student* lowest = &students_.front();
    double marksTotal = 0.0;

    for (const Student& student : students_) {
        const double marks = student.getMarks();
        marksTotal += marks;

        if (marks >= passMark) {
            ++statistics.passedStudents;
        } else {
            ++statistics.failedStudents;
        }

        if (marks > highest->getMarks()) {
            highest = &student;
        }
        if (marks < lowest->getMarks()) {
            lowest = &student;
        }
    }

    statistics.averageMarks = marksTotal / static_cast<double>(students_.size());
    statistics.highestScorer = *highest;
    statistics.lowestScorer = *lowest;
    statistics.hasData = true;
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
    const std::string normalizedId = normalizeIdForComparison(id);
    return std::any_of(students.begin(), students.end(),
                       [&normalizedId](const Student& existingStudent) {
                           return normalizeIdForComparison(existingStudent.getId()) == normalizedId;
                       });
}
