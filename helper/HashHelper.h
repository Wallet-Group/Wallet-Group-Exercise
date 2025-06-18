#pragma once
#include <string>
#include <array>
#include <vector>
using namespace std;
class HashHelper
{
public:
    // Hash a string using SHA-256
    static string sha256(const string &input);

    // Hash a string using MD5
    static string md5(const string &input);

    // Generate a random salt
    static string generateSalt(size_t length = 16);

    // Hash a password with a salt
    static string hashPassword(const string &password, const string &salt);

    // Verify a password against a hashed password and salt
    static bool verifyPassword(const string &password, const string &hashedPassword, const string &salt);

private:
    // Convert byte array to hex string
    static string bytesToHex(const vector<unsigned char> &bytes);

    // Convert hex string to byte array
    static vector<unsigned char> hexToBytes(const string &hex);
};