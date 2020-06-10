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
float Process::CpuUtilization() {
    long total_time = LinuxParser::ActiveJiffies(pid_);
    float seconds = Process::UpTime();
    cpuutilization_ = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    return cpuutilization_;

/*
    std::string line;
    long utime;
    long stime;
    long cutime;
    long cstime;
    long starttime;
    string a;

    std::ifstream filestream(LinuxParser::kProcDirectory + '/' + to_string(pid_) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        if (linestream >> a >> a >> a >> a >> a >>
            a >> a >> a >> a >> a >>
            a >> a >> a >> utime >> stime >>
            cutime >> cstime) {
            int total_time = utime + stime;
            total_time += cutime + cstime;
            float seconds = Process::UpTime(); // - (starttime / sysconf(_SC_CLK_TCK));

            float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
            cpuutilization_ = cpu_usage;
            return cpu_usage;
        }

    }
    return 0.0; */

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
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

float Process::Cpu() const {
    return cpuutilization_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return (this->Cpu() < a.Cpu());
//     return true; 
}