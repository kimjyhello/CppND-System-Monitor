#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Returns this process's ID
int Process::Pid() { return pid_; }

// Returns this process's CPU utilization
float Process::CpuUtilization() const {
    long total_time = LinuxParser::ActiveJiffies(pid_);
    float seconds = Process::UpTime();
    return ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    //cpuutilization_ = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    //return cpuutilization_;
}

// Returns the command that generated this process
string Process::Command() {
    if (command_ == "") {
        command_ = LinuxParser::Command(pid_); 
    }
    return command_;
}

// Returns this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_);
}

// Returns the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_); 
}

// Returns the age of this process (in seconds)
long int Process::UpTime() const {
    return LinuxParser::UpTime(pid_);
}

// Overloads the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    //return true;
    return (this->CpuUtilization() > a.CpuUtilization());
}