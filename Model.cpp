#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Define the User structure
struct User {
    int id;
    string name;
    string email;
};

class Database {
private:
    vector<User> users;
    int nextId = 1;

public:
    User createUser(const string& name, const string& email);
    User getUser(int id);
    void updateUser(int id, const string& name, const string& email);
    void deleteUser(int id);
};

User Database::createUser(const string& name, const string& email) {
    User newUser{nextId++, name, email};
    users.push_back(newUser);
    return newUser;
}

User Database::getUser(int id) {
    for (const auto& user : users) {
        if (user.id == id) {
            return user;
        }
    }
    return User{-1, "", ""}; // User not found
}

void Database::updateUser(int id, const string& name, const string& email) {
    for (auto& user : users) {
        if (user.id == id) {
            user.name = name;
            user.email = email;
            break;
        }
    }
}

void Database::deleteUser(int id) {
    users.erase(remove_if(users.begin(), users.end(), [id](const User& user) {
        return user.id == id;
    }), users.end());
}
