#include "interface/data_service.h"
#include <iostream>

DataService::DataService() {
    // Initialize with some default data
    dataStore["config"] = "default_config";
    dataStore["version"] = "1.0.0";
}

Common::ServiceResult DataService::storeData(const std::string& key, const std::string& value) {
    dataStore[key] = value;
    return Common::ServiceResult(true, "Data stored successfully");
}

std::string DataService::retrieveData(const std::string& key) const {
    auto it = dataStore.find(key);
    if (it != dataStore.end()) {
        return it->second;
    }
    return "";
}

Common::ServiceResult DataService::deleteData(const std::string& key) {
    auto it = dataStore.find(key);
    if (it == dataStore.end()) {
        return Common::ServiceResult(false, "Key not found");
    }
    
    dataStore.erase(it);
    return Common::ServiceResult(true, "Data deleted successfully");
}

void DataService::processDataCommands() {
    std::cout << "1. Store Data\n2. Retrieve Data\n3. Delete Data\n4. List All Keys\n5. Back to main menu" << std::endl;
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            std::string key, value;
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "Enter value: ";
            std::cin >> value;
            auto result = storeData(key, value);
            std::cout << result.message << std::endl;
            break;
        }
        case 2: {
            std::string key;
            std::cout << "Enter key: ";
            std::cin >> key;
            std::string value = retrieveData(key);
            if (!value.empty()) {
                std::cout << "Value: " << value << std::endl;
            } else {
                std::cout << "Key not found" << std::endl;
            }
            break;
        }
        case 3: {
            std::string key;
            std::cout << "Enter key to delete: ";
            std::cin >> key;
            auto result = deleteData(key);
            std::cout << result.message << std::endl;
            break;
        }
        case 4: {
            std::cout << "Stored keys:" << std::endl;
            for (const auto& pair : dataStore) {
                std::cout << "- " << pair.first << " = " << pair.second << std::endl;
            }
            break;
        }
        case 5:
            return;
        default:
            std::cout << "Invalid choice" << std::endl;
    }
}