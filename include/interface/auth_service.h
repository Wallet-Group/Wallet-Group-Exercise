#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include "../common/types.h"
#include <string>
#include <map>

class AuthService {
private:
    std::map<std::string, std::string> credentials;
    std::string currentUser;

public:
    AuthService();
    ~AuthService() = default;
    
    Common::ServiceResult login(const std::string& username, const std::string& password);
    Common::ServiceResult logout();
    Common::ServiceResult registerUser(const std::string& username, const std::string& password);
    bool isAuthenticated() const;
    std::string getCurrentUser() const;
    void processAuthCommands();
};

#endif // AUTH_SERVICE_H