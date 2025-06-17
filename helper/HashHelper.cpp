#include "HashHelper.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
using namespace std;

string HashHelper::sha256(const string &input)
{
    // Simple hash function using std::hash
    hash<string> hasher;
    size_t hashValue = hasher(input);
    
    // Convert to hex string
    stringstream ss;
    ss << hex << setfill('0') << setw(16) << hashValue;
    return ss.str();
}

string HashHelper::generateSalt(size_t length)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    vector<unsigned char> salt(length);
    for (auto &byte : salt)
    {
        byte = static_cast<unsigned char>(dis(gen));
    }
    return bytesToHex(salt);
}

string HashHelper::hashPassword(const string &password, const string &salt)
{
    string combined = password + salt;
    return sha256(combined);
}

bool HashHelper::verifyPassword(const string &password, const string &hashedPassword, const string &salt)
{
    string computedHash = hashPassword(password, salt);
    return computedHash == hashedPassword;
}

string HashHelper::bytesToHex(const vector<unsigned char> &bytes)
{
    stringstream ss;
    ss << hex << setfill('0');
    for (unsigned char byte : bytes)
    {
        ss << setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

vector<unsigned char> HashHelper::hexToBytes(const string &hex)
{
    vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}