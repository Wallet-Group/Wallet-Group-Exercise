#pragma once
#include <string>
#include "json.hpp"

class JsonFileManager {
public:
    // Save a JSON object to a file
    static bool saveToFile(const std::string& filename, const nlohmann::json& j);

    // Load a JSON object from a file
    static bool loadFromFile(const std::string& filename, nlohmann::json& j);
}; 