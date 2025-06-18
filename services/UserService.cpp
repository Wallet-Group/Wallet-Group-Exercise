#include "../interface/UserService.h"
#include <filesystem>
#include <unordered_map>
#include <iostream>
#include "../helper/HashHelper.h"
#include "../helper/OTPHelper.h"
using namespace std;
using namespace nlohmann;

UserService::UserService()
{
    // Create data directory if it doesn't exist
    filesystem::create_directories("data");
    // Load existing users
    loadUsers();
}

json UserService::userToJson(const User &user) const
{
    json j;
    j["username"] = user.getUsername();
    j["password"] = user.getPassword();
    j["name"] = user.getName();
    j["yearOfBirth"] = user.getYearOfBirth();
    j["role"] = static_cast<int>(user.getRole());
    return j;
}

shared_ptr<User> UserService::jsonToUser(const json &j) const
{
    return make_shared<User>(
        j["username"].get<string>(),
        j["password"].get<string>(),
        j["name"].get<string>(),
        j["yearOfBirth"].get<int>(),
        static_cast<UserRole>(j["role"].get<int>()));
}

bool UserService::loadUsers()
{
    json j;
    if (!JsonFileManager::loadFromFile(USERS_FILE, j))
    {
        // If file doesn't exist or is empty, that's okay
        return true;
    }
    try
    {
        users.clear();
        for (const auto &userJson : j["users"])
        {
            auto userPtr = jsonToUser(userJson);
            users[userPtr->getUsername()] = userPtr;
        }
        return true;
    }
    catch ([[maybe_unused]] const exception &e)
    {
        // Log error if needed
        return false;
    }
}

bool UserService::saveUsers() const
{
    try
    {
        json j;
        j["users"] = json::array();
        for (const auto &pair : users)
        {
            j["users"].push_back(userToJson(*pair.second));
        }
        return JsonFileManager::saveToFile(USERS_FILE, j);
    }
    catch ([[maybe_unused]] const exception &e)
    {
        // Log error if needed
        return false;
    }
}

optional<shared_ptr<User>> UserService::findUserByUsername(const string &username) const
{
    auto it = users.find(username);
    if (it != users.end())
    {
        return it->second;
    }
    return nullopt;
}

bool UserService::registerUser(const string &username, const string &password, const string &name, int yearOfBirth, UserRole role)
{
    if (username == "")
        return false;
    else if (password == "")
        return false;
    if (isUsernameTaken(username))
    {
        return false;
    }
    // Generate salt and hash password with salt
    string salt = HashHelper::generateSalt();
    string hashedPassword = HashHelper::hashPassword(password, salt);
    // Store hashed password (which includes salt) directly
    users[username] = make_shared<User>(username, hashedPassword, name, yearOfBirth, role);
    return saveUsers(); // Save to file after registration
}

optional<shared_ptr<User>> UserService::login(const string &username, const string &password) const
{
    auto user = findUserByUsername(username);
    if (user) {
        // Hash the input password and compare with stored hash
        string hashedInputPassword = HashHelper::md5(password);
        if (user.value()->getPassword() == hashedInputPassword)
        {
            return user;
        }
    }
    return nullopt;
}

bool UserService::isUsernameTaken(const string &username) const
{
    return users.find(username) != users.end();
}

const unordered_map<string, shared_ptr<User>> &UserService::getAllUsers() const
{
    return users;
}

bool UserService::updateUserPassword(const string &username, const string &oldPassword, const string &newPassword)
{
    auto user = findUserByUsername(username);
    if (!user)
    {
        return false;
    }
    // Hash both old and new passwords for comparison and storage
    string hashedOldPassword = HashHelper::md5(oldPassword);
    string hashedNewPassword = HashHelper::md5(newPassword);
    
    if (user.value()->getPassword() == hashedOldPassword)
    {
        user.value()->setPassword(hashedNewPassword);
        return saveUsers(); // Save changes to file
    }
    return false;
}

bool UserService::updateUsername(const string &oldUsername, const string &newUsername)
{
    if (isUsernameTaken(newUsername))
    {
        return false;
    }
    auto user = findUserByUsername(oldUsername);
    if (!user)
    {
        return false;
    }
    if (user.value()->updateUsername(newUsername))
    {
        // Remove old entry and add new one
        users.erase(oldUsername);
        users[newUsername] = user.value();
        return saveUsers(); // Save changes to file
    }
    return false;
}

bool UserService::changePassword(const string &username, const string &newPassword)
{
    auto user = findUserByUsername(username);
    if (!user)
    {
        return false;
    }
    // Hash the new password before storing
    string hashedNewPassword = HashHelper::md5(newPassword);
    user.value()->setPassword(hashedNewPassword);
    return saveUsers();
}

bool UserService::changePasswordWithOTP(const string &username, const string &oldPassword, const string &newPassword, const string &otp)
{
    // Validate input parameters
    if (newPassword.empty()) {
        cout << "Error: New password cannot be empty." << endl;
        return false;
    }
    
    if (newPassword == oldPassword) {
        cout << "Error: New password must be different from old password." << endl;
        return false;
    }
    
    auto user = findUserByUsername(username);
    if (!user) {
        cout << "Error: User not found." << endl;
        return false;
    }
    
    // Verify old password
    string hashedOldPassword = HashHelper::md5(oldPassword);
    if (user.value()->getPassword() != hashedOldPassword) {
        cout << "Error: Old password is incorrect." << endl;
        return false;
    }
    
    // Validate OTP
    if (!OTPHelper::validateOTP(username, otp)) {
        cout << "Error: Invalid or expired OTP." << endl;
        return false;
    }
    
    // Change password
    string hashedNewPassword = HashHelper::md5(newPassword);
    user.value()->setPassword(hashedNewPassword);
    
    // Remove OTP after successful use
    OTPHelper::removeOTP(username);
    
    // Save changes
    if (saveUsers()) {
        cout << "Password changed successfully!" << endl;
        return true;
    } else {
        cout << "Error: Failed to save password change." << endl;
        return false;
    }
}