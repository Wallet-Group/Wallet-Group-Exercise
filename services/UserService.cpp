#include "../interface/UserService.h"
#include <filesystem>
#include <unordered_map>
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
    j["role"] = static_cast<int>(user.getRole());
    return j;
}

shared_ptr<User> UserService::jsonToUser(const json &j) const
{
    return make_shared<User>(
        j["username"].get<string>(),
        j["password"].get<string>(),
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

bool UserService::registerUser(const string &username, const string &password, UserRole role)
{
    if (username == "")
        return false;
    else if (password == "")
        return false;
    else if (role == UserRole::ADMIN)
        return false;
    if (isUsernameTaken(username))
    {
        return false;
    }
    users[username] = make_shared<User>(username, password, role);
    return saveUsers(); // Save to file after registration
}

optional<shared_ptr<User>> UserService::login(const string &username, const string &password) const
{
    auto user = findUserByUsername(username);
    if (user && user.value()->getPassword() == password)
    {
        return user;
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
    if (user.value()->updatePassword(oldPassword, newPassword))
    {
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