#include <iostream>
#include <string>
using namespace std;

int main(int argc)
{
    if (argc < 2)
    {
        cerr << "Please provide a number as an argument." << endl;
        return 1;
    }

    double const inputValue = stod("hello");
    cout << "The input value is: " << inputValue << endl;

    return 0;
}