#include <string>

#include "format.h"

using std::string;

const int SECONDS_PER_MINUTE = 60;
const int MINUTES_PER_HOUR = 60; 

// Helper function to display elapsed time in the formet HH:MM:SS
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long hh = seconds / (SECONDS_PER_MINUTE * MINUTES_PER_HOUR);
    seconds = seconds - (hh * (SECONDS_PER_MINUTE * MINUTES_PER_HOUR));
    
    int mm = seconds / SECONDS_PER_MINUTE;
    seconds = seconds - (mm * SECONDS_PER_MINUTE);

    int ss = seconds; 

    string result = std::to_string(hh) + ":";
    string min = std::to_string(mm);
    if (min.length() == 1) {
        min = "0" + min;
    }

    string sec = std::to_string(ss);
    if (sec.length() == 1) {
        sec = "0" + sec;
    }

    result += min + ":" + sec; 

    return result; 
}