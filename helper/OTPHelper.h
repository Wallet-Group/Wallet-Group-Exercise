#pragma once
#include <string>
#include <map>
#include <chrono>
using namespace std;
class OTPHelper
{
private:
    static map<string, pair<string, chrono::system_clock::time_point>> otpStore;
    static const int OTP_LENGTH = 6;
    static const int OTP_EXPIRY_MINUTES = 5;

public:
    // Generate OTP for a user
    static string generateOTP(const string &username);

    // Validate OTP for a user
    static bool validateOTP(const string &username, const string &otp);

    // Clean expired OTPs
    static void cleanExpiredOTPs();

    // Remove OTP for a user (after successful use)
    static void removeOTP(const string &username);
};