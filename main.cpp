#include <iostream>
#include <string>
#include <memory>
#include "services/service_manager.h"

void printMenu() {
    std::cout << "\n=== Service Menu ===" << std::endl;
    std::cout << "1. User Service" << std::endl;
    std::cout << "2. Data Service" << std::endl;
    std::cout << "3. Auth Service" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose service: ";
}

int main() {
    std::cout << "Service Application Started" << std::endl;
    
    auto serviceManager = std::make_unique<ServiceManager>();
    
    int choice;
    bool running = true;
    
    while (running) {
        printMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                serviceManager->runUserService();
                break;
            case 2:
                serviceManager->runDataService();
                break;
            case 3:
                serviceManager->runAuthService();
                break;
            case 0:
                running = false;
                std::cout << "Exiting application..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
    
    return 0;
}