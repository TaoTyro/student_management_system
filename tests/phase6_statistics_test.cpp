#include <cmath>
#include <iostream>

#include "Database.h"
#include "Student.h"

namespace {
bool almostEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}
}

int main() {
    Database database("build/phase6_statistics_test.txt");

    Database::Statistics emptyStats;
    if (!database.calculateStatistics(60.0, emptyStats)) {
        std::cout << "FAIL:empty-stats-call\n";
        return 1;
    }
    if (emptyStats.totalStudents != 0 || emptyStats.hasData) {
        std::cout << "FAIL:empty-stats-values\n";
        return 1;
    }
    if (database.calculateStatistics(-1.0, emptyStats)) {
        std::cout << "FAIL:accepted-negative-pass-mark\n";
        return 1;
    }

    if (!database.addToMemory(Student("401", "Alpha", 20, "CS", 80.0))) {
        std::cout << "FAIL:add-alpha\n";
        return 1;
    }
    if (!database.addToMemory(Student("402", "Bravo", 21, "IT", 50.0))) {
        std::cout << "FAIL:add-bravo\n";
        return 1;
    }
    if (!database.addToMemory(Student("403", "Charlie", 22, "SE", 30.0))) {
        std::cout << "FAIL:add-charlie\n";
        return 1;
    }

    Database::Statistics stats;
    if (!database.calculateStatistics(50.0, stats)) {
        std::cout << "FAIL:stats-call\n";
        return 1;
    }

    if (stats.totalStudents != 3 || stats.passedStudents != 2 || stats.failedStudents != 1) {
        std::cout << "FAIL:counts\n";
        return 1;
    }
    if (!almostEqual(stats.averageMarks, (80.0 + 50.0 + 30.0) / 3.0)) {
        std::cout << "FAIL:average\n";
        return 1;
    }
    if (!stats.hasData || stats.highestScorer.getId() != "401" || stats.lowestScorer.getId() != "403") {
        std::cout << "FAIL:range\n";
        return 1;
    }

    const Student* lowest = database.findLowestStudent();
    if (lowest == nullptr || lowest->getId() != "403") {
        std::cout << "FAIL:find-lowest\n";
        return 1;
    }

    std::cout << "PASS:phase6-statistics\n";
    return 0;
}
