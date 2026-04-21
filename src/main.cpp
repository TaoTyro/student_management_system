#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Database.h"
#include "Student.h"
#include "Utils.h"

namespace {
void showMenu() {
    std::cout << "\n=== STUDENT MANAGEMENT SYSTEM ===\n";
    std::cout << "1. Add New Student\n";
    std::cout << "2. Display All Students\n";
    std::cout << "3. Search Student\n";
    std::cout << "4. View Statistics\n";
    std::cout << "5. Save & Exit\n";
    std::cout << "=================================\n";
}

bool parseIntStrict(const std::string& value, int& output) {
    try {
        std::size_t consumed = 0;
        const int parsed = std::stoi(value, &consumed);
        if (consumed != value.size()) {
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
        const double parsed = std::stod(value, &consumed);
        if (consumed != value.size()) {
            return false;
        }
        output = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

std::string promptRequiredText(const std::string& label) {
    for (;;) {
        std::cout << label;

        std::string value;
        std::getline(std::cin, value);
        value = Utils::trim(value);

        if (Utils::isBlank(value)) {
            std::cout << "Input cannot be empty. Please try again.\n";
            continue;
        }
        if (value.find('|') != std::string::npos) {
            std::cout << "Character '|' is not allowed in this field. Please try again.\n";
            continue;
        }

        return value;
    }
}

int promptPositiveAge() {
    for (;;) {
        std::cout << "Enter Age: ";
        std::string line;
        std::getline(std::cin, line);
        line = Utils::trim(line);

        int age = 0;
        if (!parseIntStrict(line, age) || age <= 0) {
            std::cout << "Invalid age. Enter a positive whole number.\n";
            continue;
        }

        return age;
    }
}

double promptValidMarks() {
    for (;;) {
        std::cout << "Enter Marks (0-100): ";
        std::string line;
        std::getline(std::cin, line);
        line = Utils::trim(line);

        double marks = 0.0;
        if (!parseDoubleStrict(line, marks) || marks < 0.0 || marks > 100.0) {
            std::cout << "Invalid marks. Enter a number between 0 and 100.\n";
            continue;
        }

        return marks;
    }
}

double promptPassMarkThreshold() {
    for (;;) {
        std::cout << "Enter Pass Threshold (0-100): ";
        std::string line;
        std::getline(std::cin, line);
        line = Utils::trim(line);

        double passMark = 0.0;
        if (!parseDoubleStrict(line, passMark) || passMark < 0.0 || passMark > 100.0) {
            std::cout << "Invalid threshold. Enter a number between 0 and 100.\n";
            continue;
        }

        return passMark;
    }
}

int promptMenuChoice() {
    for (;;) {
        std::cout << "Enter your choice: ";

        std::string line;
        std::getline(std::cin, line);
        line = Utils::trim(line);

        int choice = 0;
        if (!parseIntStrict(line, choice) || choice < 1 || choice > 5) {
            std::cout << "Invalid menu choice. Select a value from 1 to 5.\n";
            continue;
        }

        return choice;
    }
}

void printSearchResults(const std::vector<Student>& matches) {
    if (matches.empty()) {
        std::cout << "No matching student found.\n";
        return;
    }

    std::cout << "Found " << matches.size() << " matching student(s):\n";
    for (const Student& student : matches) {
        student.print(std::cout);
        std::cout << '\n';
    }
}

void showDetailedStatistics(const Database& database) {
    if (database.getStudents().empty()) {
        std::cout << "No student data available for statistics.\n";
        return;
    }

    const double passThreshold = promptPassMarkThreshold();
    Database::Statistics statistics;
    if (!database.calculateStatistics(passThreshold, statistics)) {
        std::cout << "Could not compute statistics due to invalid threshold.\n";
        return;
    }

    const double passPercentage =
        statistics.totalStudents == 0
            ? 0.0
            : (static_cast<double>(statistics.passedStudents) * 100.0 / static_cast<double>(statistics.totalStudents));
    const double failPercentage =
        statistics.totalStudents == 0
            ? 0.0
            : (static_cast<double>(statistics.failedStudents) * 100.0 / static_cast<double>(statistics.totalStudents));

    const std::streamsize oldPrecision = std::cout.precision();
    const std::ios::fmtflags oldFlags = std::cout.flags();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== STUDENT STATISTICS ===\n";
    std::cout << "Pass Threshold: " << passThreshold << '\n';
    std::cout << "Total Students: " << statistics.totalStudents << '\n';
    std::cout << "Average Marks: " << statistics.averageMarks << "\n\n";

    std::cout << "Pass/Fail Analysis:\n";
    std::cout << "- Passed: " << statistics.passedStudents << " students (" << passPercentage << "%)\n";
    std::cout << "- Failed: " << statistics.failedStudents << " students (" << failPercentage << "%)\n\n";

    if (statistics.hasData) {
        std::cout << "Score Range:\n";
        std::cout << "- Highest: " << statistics.highestScorer.getMarks() << " (" << statistics.highestScorer.getName()
                  << ")\n";
        std::cout << "- Lowest: " << statistics.lowestScorer.getMarks() << " (" << statistics.lowestScorer.getName()
                  << ")\n";
    }
    std::cout << "==========================\n";

    std::cout.flags(oldFlags);
    std::cout.precision(oldPrecision);
}
} // namespace

int main() {
    Database database;
    const bool loadFullySuccessful = database.loadFromFile();
    if (loadFullySuccessful) {
        std::cout << "Student records loaded from file.\n";
    } else {
        std::cout << "Warning: Some records could not be loaded due to invalid format or read issues.\n";
    }

    int choice = 0;
    do {
        showMenu();
        choice = promptMenuChoice();

        switch (choice) {
            case 1: {
                try {
                    const std::string id = promptRequiredText("Enter Student ID: ");
                    const std::string name = promptRequiredText("Enter Student Name: ");
                    const int age = promptPositiveAge();
                    const std::string course = promptRequiredText("Enter Course: ");
                    const double marks = promptValidMarks();

                    Student student(id, name, age, course, marks);
                    if (database.addStudent(student)) {
                        std::cout << "Student added successfully.\n";
                    } else {
                        std::cout << "Failed to add student. Ensure the ID is unique and file is writable.\n";
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Invalid student data: " << ex.what() << '\n';
                }
                break;
            }
            case 2:
                database.displayStudents(std::cout);
                break;
            case 3: {
                const std::string query = promptRequiredText("Enter Student ID or Name to search: ");
                const std::vector<Student> matches = database.searchStudent(query);
                printSearchResults(matches);
                break;
            }
            case 4:
                showDetailedStatistics(database);
                break;
            case 5:
                if (database.saveToFile()) {
                    std::cout << "Data saved successfully. Exiting...\n";
                } else {
                    std::cout << "Warning: Could not save all data before exit.\n";
                }
                break;
            default:
                std::cout << "Invalid menu choice.\n";
                break;
        }
    } while (choice != 5);

    return 0;
}
