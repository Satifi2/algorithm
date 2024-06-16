
#include <iostream>
#include <string>

class Person {
public:
    std::string name;
    int age;

    // 默认构造函数
    Person() : name(""), age(0) {}

    Person(int a){
        age = a;
    }

    // 带参数的构造函数
    Person(std::string n, int a) : name(n), age(a) {}

    // 列表初始化的构造函数
    Person(std::initializer_list<std::string> list) {
        auto it = list.begin();
        name = *it;
        age = std::stoi(*(it + 1));
    }

    void display() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};

int main() {
    // 复制初始化（Copy Initialization）
    Person p1 = Person("Alice", 30);
    p1.display(); // 输出 Name: Alice, Age: 30

    // 直接初始化（Direct Initialization）
    Person p2("Bob", 25);
    p2.display(); // 输出 Name: Bob, Age: 25

    // 列表初始化（List Initialization）
    Person p3 = {"Charlie", "20"};
    p3.display(); // 输出 Name: Charlie, Age: 20

    Person p4{"David", "40"};
    p4.display(); // 输出 Name: David, Age: 40

    // 默认初始化（Default Initialization）
    Person p5;
    p5.display(); // 输出 Name: , Age: 0

    // 值初始化（Value Initialization）
    Person p6{};
    p6.display(); // 输出 Name: , Age: 0

    Person p7 = 1234;
    p7.display();

    Person p8 = {};
    p8.display();

    Person p9 = Person();
    p9.display();

    Person p10();
    p9.display();

    int a = int(1);
    std::cout << a << std::endl;

    int b(2);
    std::cout << b << std::endl;

    int c = {3};
    std::cout << c << std::endl;

    int d{4};
    std::cout << d << std::endl;

    int e;
    std::cout << e << std::endl;

    int f{};
    std::cout << f << std::endl;

    int g = 1;
    std::cout << g << std::endl;

    int h = {};
    std::cout << h << std::endl;

    int i = int();
    std::cout << i << std::endl;

    int j();
    std::cout << j << std::endl;

    p1=Person("Bob", 25);
    p1.display();

    p1=("Bob", 25);
    p1.display();

    p1={"Bob", 25};
    p1.display();

    p1=Person();
    p1.display();

    p1={};
    p1.display();

    p1();

    return 0;
}
