#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Database.h"
#include "Student.h"

namespace {
bool almostEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}
}

int main() {
    const std::string path = "build/phase4_students_test.txt";
    {
        std::ofstream clear(path, std::ios::trunc);
    }

    Database db(path);

    double average = -1.0;
    if (db.calculateAverage(average)) {
        std::cout << "FAIL:empty-average-should-fail\n";
        return 1;
    }
    if (db.findTopStudent() != nullptr) {
        std::cout << "FAIL:empty-top-should-be-null\n";
        return 1;
    }

    if (!db.addStudent(Student("201", "Alice Brown", 20, "CS", 80.0))) {
        std::cout << "FAIL:add-1\n";
        return 1;
    }
    if (!db.addStudent(Student("202", "Bob Stone", 22, "IT", 70.0))) {
        std::cout << "FAIL:add-2\n";
        return 1;
    }
    if (!db.addStudent(Student("203", "Jane Smith", 19, "SE", 95.0))) {
        std::cout << "FAIL:add-3\n";
        return 1;
    }
    if (db.addStudent(Student("202", "Duplicate", 21, "NET", 88.0))) {
        std::cout << "FAIL:duplicate-id-accepted\n";
        return 1;
    }

    if (!db.calculateAverage(average) || !almostEqual(average, (80.0 + 70.0 + 95.0) / 3.0)) {
        std::cout << "FAIL:average\n";
        return 1;
    }

    const Student* topStudent = db.findTopStudent();
    if (topStudent == nullptr || topStudent->getId() != "203") {
        std::cout << "FAIL:top-student\n";
        return 1;
    }

    const auto byId = db.searchStudent("201");
    if (byId.size() != 1 || byId[0].getName() != "Alice Brown") {
        std::cout << "FAIL:search-by-id\n";
        return 1;
    }

    const auto byName = db.searchStudent("jane");
    if (byName.size() != 1 || byName[0].getId() != "203") {
        std::cout << "FAIL:search-by-name\n";
        return 1;
    }

    std::ostringstream displayOutput;
    db.displayStudents(displayOutput);
    const std::string displayText = displayOutput.str();
    if (displayText.find("Alice Brown") == std::string::npos || displayText.find("Marks") == std::string::npos) {
        std::cout << "FAIL:display\n";
        return 1;
    }

    Database reload(path);
    if (!reload.loadFromFile() || reload.getStudents().size() != 3) {
        std::cout << "FAIL:reload\n";
        return 1;
    }

    std::cout << "PASS:phase4-core-functions\n";
    return 0;
}
