#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

// 定义一个结构体表示学生信息
struct Student {
    std::string name;
    int age;
    double grade;
};

// 函数用于比较两个学生的成绩
bool compareStudentsByGrade(const Student& s1, const Student& s2) {
    return s1.grade > s2.grade;
}

int main() {
    // 创建一个学生信息的向量
    std::vector<Student> students;
    students.push_back({"Alice", 18, 85.5});
    students.push_back({"Bob", 19, 90.2});
    students.push_back({"Charlie", 17, 78.9});

    // 使用算法库对学生信息进行排序
    std::sort(students.begin(), students.end(), compareStudentsByGrade);

    // 输出排序后的学生信息
    for (const auto& student : students) {
        std::cout << "Name: " << student.name << ", Age: " << student.age << ", Grade: " << student.grade << std::endl;
    }

    // 使用哈希表存储学生姓名和年龄
    std::unordered_map<std::string, int> studentAgeMap;
    for (const auto& student : students) {
        studentAgeMap[student.name] = student.age;
    }

    // 查询特定学生的年龄
    std::string queryName = "Bob";
    if (studentAgeMap.find(queryName)!= studentAgeMap.end()) {
        std::cout << queryName << "'s age is: " << studentAgeMap[queryName] << std::endl;
    } else {
        std::cout << queryName << " not found." << std::endl;
    }

    return 0;
}