#ifndef DATA_SERVICE_H
#define DATA_SERVICE_H

#include "../common/types.h"
#include <string>
#include <map>

class DataService {
private:
    std::map<std::string, std::string> dataStore;

public:
    DataService();
    ~DataService() = default;
    
    Common::ServiceResult storeData(const std::string& key, const std::string& value);
    std::string retrieveData(const std::string& key) const;
    Common::ServiceResult deleteData(const std::string& key);
    void processDataCommands();
};

#endif // DATA_SERVICE_H