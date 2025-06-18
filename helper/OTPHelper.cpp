#include "OTPHelper.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

// Initialize static member
map<string, pair<string, chrono::system_clock::time_point>> OTPHelper::otpStore;

string OTPHelper::generateOTP(const string &username)
{
    // Clean expired OTPs first
    cleanExpiredOTPs();

    // Generate random 6-digit OTP
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000, 999999);

    string otp = to_string(dis(gen));

    // Store OTP with current timestamp
    auto now = chrono::system_clock::now();
    otpStore[username] = make_pair(otp, now);

    cout << "OTP generated for " << username << ": " << otp << endl;
    return otp;
}

bool OTPHelper::validateOTP(const string &username, const string &otp)
{
    cleanExpiredOTPs();

    auto it = otpStore.find(username);
    if (it == otpStore.end())
    {
        return false; // No OTP found for user
    }

    // Check if OTP matches
    if (it->second.first == otp)
    {
        return true;
    }

    return false;
}

void OTPHelper::cleanExpiredOTPs()
{
    auto now = chrono::system_clock::now();
    auto expiryDuration = chrono::minutes(OTP_EXPIRY_MINUTES);

    for (auto it = otpStore.begin(); it != otpStore.end();)
    {
        if (now - it->second.second > expiryDuration)
        {
            it = otpStore.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void OTPHelper::removeOTP(const string &username)
{
    otpStore.erase(username);
}