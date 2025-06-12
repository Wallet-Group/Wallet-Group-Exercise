#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <memory>

namespace Common {
    enum class ServiceType {
        USER_SERVICE,
        DATA_SERVICE,
        AUTH_SERVICE
    };
    
    struct ServiceResult {
        bool success;
        std::string message;
        
        ServiceResult(bool s = false, const std::string& msg = "") 
            : success(s), message(msg) {}
    };
}

#endif // TYPES_H