#include "Student.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Student::Student() : id_(), name_(), age_(0), course_(), marks_(0.0) {}

Student::Student(const std::string& id, const std::string& name, int age, const std::string& course, double marks)
    : id_(), name_(), age_(0), course_(), marks_(0.0) {
    setId(id);
    setName(name);
    setAge(age);
    setCourse(course);
    setMarks(marks);
}

const std::string& Student::getId() const {
    return id_;
}

const std::string& Student::getName() const {
    return name_;
}

int Student::getAge() const {
    return age_;
}

const std::string& Student::getCourse() const {
    return course_;
}

double Student::getMarks() const {
    return marks_;
}

void Student::setId(const std::string& id) {
    const std::string normalized = trimCopy(id);
    if (!isValidId(normalized)) {
        throw std::invalid_argument("Student ID must not be empty and cannot contain '|'.");
    }
    id_ = normalized;
}

void Student::setName(const std::string& name) {
    const std::string normalized = trimCopy(name);
    if (!isValidName(normalized)) {
        throw std::invalid_argument(
            "Student name must not be empty, cannot contain '|', and must include at least one letter.");
    }
    name_ = normalized;
}

void Student::setAge(int age) {
    if (!isValidAge(age)) {
        throw std::invalid_argument("Student age must be a positive number.");
    }
    age_ = age;
}

void Student::setCourse(const std::string& course) {
    const std::string normalized = trimCopy(course);
    if (!isValidCourse(normalized)) {
        throw std::invalid_argument("Course must not be empty, cannot contain '|', and must include at least one letter.");
    }
    course_ = normalized;
}

void Student::setMarks(double marks) {
    if (!isValidMarks(marks)) {
        throw std::invalid_argument("Marks must be between 0 and 100.");
    }
    marks_ = marks;
}

bool Student::isValid() const {
    return isValidId(id_) && isValidName(name_) && isValidAge(age_) && isValidCourse(course_) && isValidMarks(marks_);
}

std::string Student::toFileRecord() const {
    if (!isValid()) {
        throw std::logic_error("Cannot serialize incomplete or invalid student record.");
    }

    std::ostringstream output;
    output << id_ << "|" << name_ << "|" << age_ << "|" << course_ << "|"
           << std::fixed << std::setprecision(2) << marks_;
    return output.str();
}

void Student::print(std::ostream& os) const {
    os << "ID: " << id_ << ", Name: " << name_ << ", Age: " << age_ << ", Course: " << course_
       << ", Marks: " << std::fixed << std::setprecision(2) << marks_;
}

bool Student::isValidId(const std::string& id) {
    return isNonEmptyTextField(id);
}

bool Student::isValidName(const std::string& name) {
    return isNonEmptyTextField(name) && containsAlphabeticCharacter(name);
}

bool Student::isValidAge(int age) {
    return age > 0;
}

bool Student::isValidCourse(const std::string& course) {
    return isNonEmptyTextField(course) && containsAlphabeticCharacter(course);
}

bool Student::isValidMarks(double marks) {
    return marks >= 0.0 && marks <= 100.0;
}

std::string Student::trimCopy(const std::string& value) {
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

bool Student::isNonEmptyTextField(const std::string& value) {
    return !trimCopy(value).empty() && !containsFileDelimiter(value);
}

bool Student::containsFileDelimiter(const std::string& value) {
    return value.find('|') != std::string::npos;
}

bool Student::containsAlphabeticCharacter(const std::string& value) {
    return std::any_of(value.begin(), value.end(),
                       [](unsigned char character) { return std::isalpha(character) != 0; });
}
