#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "linux_parser.h"

using std::stol;
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

//Returns Memory usage as a percentage of (used memory/total memory).

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string memTotal, memFree;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> memTotal;
    std::getline(filestream, line);
    linestream.str(line);
    linestream >> key >> memFree;
  }
  return ((stof(memTotal)-stof(memFree))/stof(memTotal)); 
}


// Read and return the system uptime
long LinuxParser::UpTime() {
  string time_up;
  string line;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time_up; 
  }
  return stol(time_up); 
}

/* These functions were unused. I simply returned the jiffy information using the
LinuxParser::UpTime function which now also takes an index value as a parameter to
specify which jiffy is being returned.


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }
*/

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  vector<string> cpuInfo = {};
  
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
  }
  
  cpuInfo.emplace_back(cpu);
  cpuInfo.emplace_back(user);
  cpuInfo.emplace_back(nice);
  cpuInfo.emplace_back(system);
  cpuInfo.emplace_back(idle);
  cpuInfo.emplace_back(iowait);
  cpuInfo.emplace_back(irq);
  cpuInfo.emplace_back(softirq);
  cpuInfo.emplace_back(steal);
  cpuInfo.emplace_back(guest);
  cpuInfo.emplace_back(guestnice);
  
  return cpuInfo;
}


// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  
  string key;
  string value;
  string line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
           return stoi(value);
        }
      }
    }
  }
  return 0; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  
  string key;
  string value;
  string line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
           return stoi(value);
        }
      }
    }
  }
  return 0; 
}

// Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
  string line;
  string cmdline;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmdline;
  }
  return cmdline;
}

// Read and return the memory used by a process

string LinuxParser::Ram(int pid) {
  
  string line;
  string key;
  string value;
  int int_value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          int_value = stol(value)/1000;
          return to_string(int_value);
        }
      }
    }
  }
  return 0;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string user;
  string uid = LinuxParser::Uid(pid); //get the user id from the Uid function
  string id;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> id) {
        if (id == uid) {
          return user;
        }
      }
    }
  }
  return 0;
}


// Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return 0;
}

// Read and return the uptime of a process. This handles the jiffies for processes as well. 
// The index for each if statement correspond to a particular jiffy which is indicated by
// each comment above the if statements in the function.
long LinuxParser::UpTime(int pid, int t_index) { 
  string time_up;
  string line;
  int count = 0;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    
    std::getline(stream, line);
    std::stringstream data(line);
    
    while(data >> time_up) {
      
      // Return process utime
      if ((count == t_index) && (t_index == 13)) {
        return stol(time_up);
      }
      
      // Return process stime
      if ((count == t_index) && (t_index == 14)) {
        return stol(time_up);
      }
      
      // Return process cutime
      if ((count == t_index) && (t_index == 15)) {
        return stol(time_up);
      }
      
      // Return process cstime
      if ((count == t_index) && (t_index == 16)) {
        return stol(time_up);
      }
      
      // Return clock constant
      if ((count == t_index) && (t_index == 21)) {
        return stol(time_up);
      }
      
      count++;
    }
  }
  return 0;
}