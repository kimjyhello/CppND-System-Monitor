#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Reads and returns the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  long value;
  string dummy;

  long memTotal;
  long memFree;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> dummy) {
        if (key == "MemTotal:") {
          memTotal = value;
        } else if (key == "MemFree:") {
          memFree = value;
        }
      }
    }
  }
  return ((float)(memTotal - memFree) / (float)memTotal);
}

// Reads and returns the system uptime
long LinuxParser::UpTime() {
  string line;
  long value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    if (linestream >> value) {
      return value;
    }
  }

  return 0;
}

// Reads and returns the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Reads and returns the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::string line;
  long utime;
  long stime;
  long cutime;
  long cstime;
  string a;

  std::ifstream filestream(LinuxParser::kProcDirectory + '/' + to_string(pid) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      if (linestream >> a >> a >> a >> a >> a >>
          a >> a >> a >> a >> a >>
          a >> a >> a >> utime >> stime >>
          cutime >> cstime) {
          return (utime + stime + cutime + cstime);
      }
  }
  return 0;
}

// Reads and returns the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> vals = LinuxParser::CpuUtilization();
  long active = vals[0] + vals[1] + vals[2] + vals[5] + vals[6] + vals[7]; 
  return active;
}

// Reads and returns the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> vals = LinuxParser::CpuUtilization();
  long idle = vals[3] + vals[4];
  return idle;
}

// Reads and returns CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  std::string line;
  std::string cpuname;
  std::vector<long> cpu;
  long temp;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpuname;
    if (cpuname == "cpu") {
      while (linestream >> temp) {
        cpu.push_back(temp);
      }
    }
  }

  return cpu; 
}

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }

  return 0; 
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }

  return 0;
}

// Reads and returns the command associated with a process
string LinuxParser::Command(int pid) { 
  string line = "";
  std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kCmdlineFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line); 
  }
  
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(value / kKBtoMB);
        }
      }
    }
  }

  return string(); 
}

// Reads and returns the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }

  return string(); 
}

// Reads and returns the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string name;
  string dummy;
  string value;

  string uid = LinuxParser::Uid(pid);
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> name >> dummy >> value) {
        if (value == uid) {
          return name; 
        }
      }
    }
  }

  return string(); 
}

// Reads and returns the uptime of a process
long LinuxParser::UpTime(int pid) {
  long sysuptime = LinuxParser::UpTime();

  std::string line;
  long starttime;
  string a;

  std::ifstream filestream(kProcDirectory + '/' + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      if (linestream >> a >> a >> a >> a >> a >>
          a >> a >> a >> a >> a >>
          a >> a >> a >> a >> a >>
          a >> a >> a >> a >> a >>
          a >> starttime) {
          long uptime = sysuptime - (starttime / sysconf(_SC_CLK_TCK));
          return uptime;
      }

  }

  return 0;
}