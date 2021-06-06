#include <string>
#include <iostream>

#include "format.h"

using std::string;
using std::to_string;

// Always ensure time is in HH or MM or SS format
string Format::TwoDigit(int num){
    string num_str = to_string(num);
    if (num < 10)
        num_str = "0" + num_str;
    return num_str;
}

string Format::ElapsedTime(long seconds) { 
    int HH = seconds / 3600;
    int rem_sec = seconds % 3600; 
    int MM = rem_sec / 60;
    int SS = rem_sec % 60;

    string elapTime = Format::TwoDigit(HH) + ":" + 
                      Format::TwoDigit(MM) + ":" + 
                      Format::TwoDigit(SS);
    return elapTime; 
}
