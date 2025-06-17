#include "JsonFileManager.h"
#include <fstream>

using namespace std;

bool JsonFileManager::saveToFile(const string &filename, const nlohmann::json &j)
{
    ofstream file(filename);
    if (!file.is_open())
        return false;
    file << j.dump(4); // Pretty print with 4 spaces
    return true;
}

bool JsonFileManager::loadFromFile(const string &filename, nlohmann::json &j)
{
    ifstream file(filename);
    if (!file.is_open())
        return false;
    try
    {
        file >> j;
    }
    catch (...)
    {
        return false;
    }
    return true;
}