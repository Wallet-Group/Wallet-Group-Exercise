#include "OTPHelper.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Helper.h"
using namespace std;



bool Helper::inputDateWithFormatDateMonthYear(string &date)
{
    cout << "Enter the date (DD/MM/YYYY): ";
    cin >> date;
    if (date.length() != 10 || date[2] != '/' || date[5] != '/')
    {
        cout << "Invalid date format. Please use DD/MM/YYYY." << endl;
        return false;
    }
    return true;
}