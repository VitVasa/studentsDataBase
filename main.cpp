#include <iostream>
#include <vector>
#include <string>
#include <fstream>   
#include <sstream>   
#include <gtest/gtest.h> 

// Структура, представляющая студента
struct Student {
    std::string name;    // Имя студента
    int age;             // Возраст студента
    std::string major;   // Специальность/направление
    double gpa;          // Средний балл успеваемости
};

/**
 * Функция добавления нового студента в базу данных
 * Запрашивает данные студента и добавляет его в вектор
 * database - ссылка на вектор студентов 
 */
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

/**
 * Функция отображения всех студентов в базе данных
 * Выводит информацию о каждом студенте 
 * database - ссылка на вектор студентов 
 */
void displayStudents(const std::vector<Student>& database) {
    if (database.empty()) {
        std::cout << "База данных пуста.\n";
        return;
    }
    
    std::cout << "Список студентов:\n";
    for (const Student& student : database) {
        std::cout << "Имя: " << student.name << "\n";
        std::cout << "Возраст: " << student.age << "\n";
        std::cout << "Специальность: " << student.major << "\n";
        std::cout << "Средний балл: " << student.gpa << "\n\n";
    }
}

/**
 * ФУНКЦИЯ ДЛЯ ЗАГРУЗКИ БАЗЫ ДАННЫХ ИЗ ФАЙЛА
 * Читает данные студентов из файла и добавляет их в базу данных
 * Формат файла: строка содержит 4 поля, разделенных запятыми: имя, возраст, специальность, средний балл
 * database - ссылка на вектор студентов 
 * filename - имя файла для загрузки данных (если пустое, запрашивается у пользователя)
 */
void loadDatabase(std::vector<Student>& database, const std::string& filename = "") {
    std::string file_to_load = filename;
    
    // Если имя файла не передано, запрашиваем его
    if (file_to_load.empty()) {
        std::cout << "Введите имя файла для загрузки: ";
        std::cin.ignore(); // Очистка буфера ввода от предыдущего ввода
        std::getline(std::cin, file_to_load);
    }
    
    // Предварительная проверка существования файла
    std::ifstream testFile(file_to_load);
    if (!testFile.is_open()) {
        std::cout << "Файл не существует или недоступен!\n";
        testFile.close();
        return;
    }
    testFile.close();
    
    std::ifstream file(file_to_load);
    
    // Проверка успешного открытия файла
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для чтения!\n";
        return;
    }
    
    int loadedCount = 0;  
    int errorCount = 0;   
    std::string line;     
    
    // Чтение файла построчно
    while (std::getline(file, line)) {
        std::vector<std::string> fields;  
        std::stringstream ss(line);       
        std::string field;                
        
        // Разделение строки на поля по запятым
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);  
        }
        
        // Проверка корректности количества полей (должно быть 4)
        if (fields.size() == 4) {
            try {
                Student student;
                student.name = fields[0];             
                student.age = std::stoi(fields[1]);   
                student.major = fields[2];            
                student.gpa = std::stod(fields[3]);     
                
                database.push_back(student);
                loadedCount++;  
            } catch (const std::exception& e) {
                std::cout << "Ошибка преобразования данных в строке: " << line << "\n";
                errorCount++;  
            }
        } else {
            std::cout << "Некорректное количество полей в строке: " << line << "\n";
            errorCount++;       
        }
    }
    
    file.close();
    std::cout << "Загружено " << loadedCount << " студентов, ошибок: " << errorCount << "\n";
}

// Тест для функции addStudent
TEST(StudentDatabase, AddStudentTest) {
    std::vector<Student> database;

    // Тестовые данные
    std::string input = "Sam\n22\nLaw\n4.8\n";
    std::stringstream input_stream(input);
    std::streambuf* old_cin = std::cin.rdbuf();
    std::cin.rdbuf(input_stream.rdbuf());

    addStudent(database);

    std::cin.rdbuf(old_cin);
    
    // Проверка результата
    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].name, "Sam");
    EXPECT_EQ(database[0].age, 22);
    EXPECT_EQ(database[0].major, "Law");
    EXPECT_NEAR(database[0].gpa, 4.8, 1e-6);
}

// Тест для функции displayStudents с пустой базой данных
TEST(StudentDatabase, DisplayEmptyDatabaseTest) {
    std::vector<Student> database;
    
    // Перехватываем вывод
    std::stringstream output_stream;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(output_stream.rdbuf());
    
    displayStudents(database);
    
    // Восстанавливаем std::cout
    std::cout.rdbuf(old_cout);
    
    std::string output = output_stream.str();
    EXPECT_TRUE(output.find("База данных пуста") != std::string::npos);
}

// Тест для функции displayStudents с несколькими студентами
TEST(StudentDatabase, DisplayMultipleStudentsTest) {
    std::vector<Student> database = {
        {"Dean", 26, "history", 3.5},
        {"Bobby", 62, "languages", 4.9}
    };
    
    // Перехватывает вывод
    std::stringstream output_stream;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(output_stream.rdbuf());
    
    displayStudents(database);
    
    // Восстанавливает std::cout
    std::cout.rdbuf(old_cout);
    
    std::string output = output_stream.str();
    EXPECT_TRUE(output.find("Dean") != std::string::npos);
    EXPECT_TRUE(output.find("Bobby") != std::string::npos);
    EXPECT_TRUE(output.find("history") != std::string::npos);
    EXPECT_TRUE(output.find("languages") != std::string::npos);
}

TEST(LoadDatabaseTest, LoadsValidDataCorrectly) {
    std::vector<Student> database;
    size_t initialSize = database.size();
    
    // Создаем временный тестовый CSV-файл
    std::string testFilename = "test_valid.csv";
    std::ofstream testFile(testFilename);
    testFile << "Cas,1000,history,4.6\n";
    testFile << "Charly,27,informatics,5.0\n";
    testFile.close();
    
    // Перехватываем вывод
    std::stringstream output_stream;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(output_stream.rdbuf());
    
    // Используем функцию с передачей имени файла
    loadDatabase(database, testFilename);
    
    // Восстанавливаем std::cout
    std::cout.rdbuf(old_cout);
    
    // Проверяем что размер увеличился на 2
    ASSERT_EQ(database.size(), initialSize + 2);
    
    // Проверяем содержимое
    EXPECT_EQ(database[0].name, "Cas");
    EXPECT_EQ(database[0].age, 1000);
    EXPECT_EQ(database[0].major, "history");
    EXPECT_EQ(database[0].gpa, 4.6);
    
    EXPECT_EQ(database[1].name, "Charly");
    EXPECT_EQ(database[1].age, 27);
    EXPECT_EQ(database[1].major, "informatics");
    EXPECT_EQ(database[1].gpa, 5.0);
    
    remove(testFilename.c_str());
}

TEST(LoadDatabaseTest, HandlesInvalidNumberFormat) {
    std::vector<Student> database;
    size_t initialSize = database.size();
    
    std::string testFilename = "test_invalid.csv";
    std::ofstream testFile(testFilename);
    testFile << "Crowley,many,history,4.7\n"; // Некорректный возраст
    testFile.close();
    
    // Перехватываем вывод
    std::stringstream output_stream;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(output_stream.rdbuf());
    
    // Используем функцию с передачей имени файла
    loadDatabase(database, testFilename);
    
    // Восстанавливаем std::cout
    std::cout.rdbuf(old_cout);
    
    // Проверяем что размер НЕ изменился (данные не добавились)
    EXPECT_EQ(database.size(), initialSize);

    remove(testFilename.c_str());
}

TEST(LoadDatabaseTest, HandlesNonExistentFile) {
    std::vector<Student> database;
    size_t initialSize = database.size();
    
    // Перехватываем вывод
    std::stringstream output_stream;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(output_stream.rdbuf());
    
    // Используем функцию с передачей несуществующего файла
    loadDatabase(database, "non_existent_file.csv");
    
    // Восстанавливаем std::cout
    std::cout.rdbuf(old_cout);
    
    std::string output = output_stream.str();
    
    // Проверяем что база не изменилась
    EXPECT_EQ(database.size(), initialSize);
    // Проверяем что выведено сообщение об ошибке
    EXPECT_TRUE(output.find("Файл не существует") != std::string::npos);
}

void runInteractiveMode() {
    std::vector<Student> database;

    int choice;
    do {
        std::cout << "Меню:\n";
        std::cout << "1. Добавить студента\n";
        std::cout << "2. Вывести список студентов\n";
        std::cout << "3. Загрузить базу данных из файла\n";
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
            case 3:
                loadDatabase(database); 
                break;
            case 0:
                std::cout << "Выход из программы.\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);
}

/**
 * Основная функция программы
 * Содержит главное меню 
 */
int main(int argc, char **argv) {
    // Если есть аргументы командной строки, запускает тесты
    if (argc > 1) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } else {
        // Иначе запускает интерактивное меню
        runInteractiveMode();
        return 0;
    }
}
