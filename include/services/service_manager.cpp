#include "interface/service_manager.h"
#include <iostream>

ServiceManager::ServiceManager() {
    userService = std::make_unique<UserService>();
    dataService = std::make_unique<DataService>();
    authService = std::make_unique<AuthService>();
}

void ServiceManager::runUserService() {
    std::cout << "\n--- User Service ---" << std::endl;
    userService->processUserCommands();
}

void ServiceManager::runDataService() {
    std::cout << "\n--- Data Service ---" << std::endl;
    dataService->processDataCommands();
}

void ServiceManager::runAuthService() {
    std::cout << "\n--- Auth Service ---" << std::endl;
    authService->processAuthCommands();
}