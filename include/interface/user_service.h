#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "../common/types.h"
#include <string>
#include <vector>

class UserService {
private:
    std::vector<std::string> users;

public:
    UserService();
    ~UserService() = default;
    
    Common::ServiceResult createUser(const std::string& username);
    Common::ServiceResult deleteUser(const std::string& username);
    std::vector<std::string> listUsers() const;
    void processUserCommands();
};

#endif // USER_SERVICE_H