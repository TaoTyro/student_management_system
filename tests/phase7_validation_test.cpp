#include <iostream>
#include <stdexcept>

#include "Database.h"
#include "Student.h"

int main() {
    try {
        Student invalidName("A1", "12345", 20, "Computer Science", 60.0);
        (void)invalidName;
        std::cout << "FAIL:numeric-name-accepted\n";
        return 1;
    } catch (const std::invalid_argument&) {
    }

    try {
        Student invalidCourse("A2", "Taonga", 20, "9999", 60.0);
        (void)invalidCourse;
        std::cout << "FAIL:numeric-course-accepted\n";
        return 1;
    } catch (const std::invalid_argument&) {
    }

    Database database("build/phase7_validation_test.txt");
    if (!database.addToMemory(Student("BICT0823", "Alpha", 20, "C++", 90.0))) {
        std::cout << "FAIL:seed-add\n";
        return 1;
    }
    if (!database.hasStudentId("BICT0823") || !database.hasStudentId("bict0823")) {
        std::cout << "FAIL:has-id-case-insensitive\n";
        return 1;
    }
    if (database.addToMemory(Student("bict0823", "Bravo", 21, "C++", 88.0))) {
        std::cout << "FAIL:case-variant-duplicate-accepted\n";
        return 1;
    }

    std::cout << "PASS:phase7-validation\n";
    return 0;
}
