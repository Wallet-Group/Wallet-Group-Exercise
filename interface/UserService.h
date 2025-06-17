#pragma once
#include "../models/User.h"
#include "../helper/File/JsonFileManager.h"
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

using namespace std;    

class UserService
{
private:
    unordered_map<string, shared_ptr<User>> users;
    const string USERS_FILE = "data/users.json";

    // Helper function to find a user by username
    optional<shared_ptr<User>> findUserByUsername(const string &username) const;

    // Load users from JSON file
    bool loadUsers();

    // Save users to JSON file
    bool saveUsers() const;

    // Convert User object to JSON
    nlohmann::json userToJson(const User &user) const;

    // Create User object from JSON
    shared_ptr<User> jsonToUser(const nlohmann::json &j) const;

public:
    UserService();

    // Register a new user
    bool registerUser(const string &username, const string &password, UserRole role);

    // Login a user
    optional<shared_ptr<User>> login(const string &username, const string &password) const;

    // Check if a username already exists
    bool isUsernameTaken(const string &username) const;

    // Get all users (might be useful for admin functionality)
    const unordered_map<string, shared_ptr<User>>& getAllUsers() const;

    // Update user information
    bool updateUserPassword(const string &username, const string &oldPassword, const string &newPassword);
    bool updateUsername(const string &oldUsername, const string &newUsername);
};