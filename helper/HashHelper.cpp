#include "HashHelper.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
#include <cstring>
#include <cstdint>
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

string HashHelper::md5(const string &input)
{
    // Simple MD5-like hash using bit operations
    uint32_t a = 0x67452301;
    uint32_t b = 0xEFCDAB89;
    uint32_t c = 0x98BADCFE;
    uint32_t d = 0x10325476;
    
    // Process input in chunks
    for (size_t i = 0; i < input.length(); i += 4) {
        uint32_t chunk = 0;
        for (int j = 0; j < 4 && i + j < input.length(); j++) {
            chunk |= (uint32_t)input[i + j] << (j * 8);
        }
        
        // MD5 round operations (simplified)
        uint32_t temp = d;
        d = c;
        c = b;
        b = b + ((a + chunk) << 7);
        a = temp;
    }
    
    // Convert to hex string
    stringstream ss;
    ss << hex << setfill('0') << setw(8) << a;
    ss << hex << setfill('0') << setw(8) << b;
    ss << hex << setfill('0') << setw(8) << c;
    ss << hex << setfill('0') << setw(8) << d;
    return ss.str();
}
