#include "Person.h"
#include "Exceptions.h"

Person::Person(int id, const std::string& name, 
               const std::string& email, const std::string& phone)
    : id(id), name(name), email(email), phone(phone) {
    if (name.empty()) {
        throw ValidationException("Person name cannot be empty");
    }
}

void Person::display() const {
    std::cout << "ID: " << id << ", Name: " << name 
              << ", Email: " << email << ", Phone: " << phone << std::endl;
}
