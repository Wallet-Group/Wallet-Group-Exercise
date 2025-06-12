#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include "user_service.h"
#include "data_service.h"
#include "auth_service.h"
#include <memory>

class ServiceManager {
private:
    std::unique_ptr<UserService> userService;
    std::unique_ptr<DataService> dataService;
    std::unique_ptr<AuthService> authService;

public:
    ServiceManager();
    ~ServiceManager() = default;
    
    void runUserService();
    void runDataService();
    void runAuthService();
};

#endif // SERVICE_MANAGER_H