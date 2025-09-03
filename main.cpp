#include <iostream>
#include <vector>
#include <string>

struct Student {
    std::string name;
    int age;
    std::string major;
    double gpa;
};

// Функция для добавления студента в базу данных
void addStudent(std::vector<Student>& database) {
    Student student;
    std::cout << "Введите имя студента: ";
    std::cin >> student.name;
    std::cout << "Введите возраст студента: ";
    std::cin >> student.age;
    std::cout << "Введите специальность студента: ";
    std::cin >> student.major;
    std::cout << "Введите средний балл студента: ";
    std::cin >> student.gpa;

    database.push_back(student);
    std::cout << "Студент добавлен в базу данных.\n";
}

// Функция для вывода всех студентов из базы данных
void displayStudents(const std::vector<Student>& database) {
    std::cout << "Список студентов:\n";
    for (const Student& student : database) {
        std::cout << "Имя: " << student.name << "\n";
        std::cout << "Возраст: " << student.age << "\n";
        std::cout << "Специальность: " << student.major << "\n";
        std::cout << "Средний балл: " << student.gpa << "\n\n";
    }
}

void loadDatabase(std::vector<Student>& database, const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для чтения!\n";
        return;
    }
    
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() == 4) {
            Student student;
            student.name = fields[0];
            student.age = std::stoi(fields[1]);
            student.major = fields[2];
            student.gpa = std::stod(fields[3]);
            
            database.push_back(student);
        }
    }
    
    file.close();
    std::cout << "База данных загружена из файла: " << filename << "\n";
}

int main() {
    std::vector<Student> database;

    int choice;
    do {
        std::cout << "Меню:\n";
        std::cout << "1. Добавить студента\n";
        std::cout << "2. Вывести список студентов\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addStudent(database);
                break;
            case 2:
                displayStudents(database);
                break;
            case 0:
                std::cout << "Выход из программы.\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);

    return 0;
}
