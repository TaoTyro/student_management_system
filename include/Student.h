#ifndef STUDENT_H
#define STUDENT_H

#include <iosfwd>
#include <string>

class Student {
public:
    Student();
    Student(const std::string& id, const std::string& name, int age, const std::string& course, double marks);

    const std::string& getId() const;
    const std::string& getName() const;
    int getAge() const;
    const std::string& getCourse() const;
    double getMarks() const;

    void setId(const std::string& id);
    void setName(const std::string& name);
    void setAge(int age);
    void setCourse(const std::string& course);
    void setMarks(double marks);

    bool isValid() const;
    std::string toFileRecord() const;
    void print(std::ostream& os) const;

    static bool isValidId(const std::string& id);
    static bool isValidName(const std::string& name);
    static bool isValidAge(int age);
    static bool isValidCourse(const std::string& course);
    static bool isValidMarks(double marks);

private:
    static std::string trimCopy(const std::string& value);
    static bool isNonEmptyTextField(const std::string& value);
    static bool containsFileDelimiter(const std::string& value);

    std::string id_;
    std::string name_;
    int age_;
    std::string course_;
    double marks_;
};

#endif
