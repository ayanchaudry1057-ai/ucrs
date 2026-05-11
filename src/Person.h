#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

/**
 * @class Person
 * @brief Base class for all persons in the system (Students and Teachers)
 */
class Person {
protected:
    int id;
    std::string name;
    std::string email;
    std::string phone;
    
public:
    /**
     * @brief Constructor for Person
     * @param id Person's unique identifier
     * @param name Person's full name
     * @param email Person's email address
     * @param phone Person's phone number
     */
    Person(int id = 0, const std::string& name = "", 
           const std::string& email = "", const std::string& phone = "");
    
    virtual ~Person() = default;
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    
    // Setters
    void setId(int id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }
    void setEmail(const std::string& email) { this->email = email; }
    void setPhone(const std::string& phone) { this->phone = phone; }
    
    // Virtual methods
    virtual void display() const;
    virtual std::string getType() const = 0;
};

#endif // PERSON_H
